#include "qsshoverhandler.h"
#include "qsseditor.h"
#include "qsseditorconstants.h"
#include "qsslexer.h"
#include "qssparser.h"
#include "qsseditorutils.h"
#include "gradienttipwidget.h"
#include <coreplugin/id.h>
#include <texteditor/basehoverhandler.h>
#include <texteditor/tooltip/tooltip.h>
#include <texteditor/tooltip/tipcontents.h>
#include <QObject>
#include <QDebug>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextBlock>
#include <QConicalGradient>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QLabel>

using namespace QssEditor;
using namespace QssEditor::Internal;

QssHoverHandler::QssHoverHandler(QObject *parent) :
    TextEditor::BaseHoverHandler(parent),
    m_editor(0),
    m_gradient(0)
{
}

QssHoverHandler::~QssHoverHandler()
{
}

bool QssHoverHandler::acceptEditor(Core::IEditor *editor)
{
    if (editor->id() == Constants::C_QSSEDITOR_ID)
        return true;
    return false;
}

void QssHoverHandler::identifyMatch(TextEditor::ITextEditor *editor, int pos)
{
    m_editor = editor;
    m_pos = pos;
    m_color = QColor();
    m_text = QString();
    delete m_gradient;
    m_gradient = 0;
    QSSTextEditorWidget *qssEditor = qobject_cast<QSSTextEditorWidget *>(editor->widget());
    if (!qssEditor)
        return;
    if (!qssEditor->extraSelectionTooltip(pos).isEmpty()) {
        setToolTip(qssEditor->extraSelectionTooltip(pos));
    } else {
        QTextBlock block = qssEditor->document()->findBlock(pos);
        QTextBlock prevBlock = block.previous();
        Lexer lexer;
        Parser parser;
        if (prevBlock.isValid()) {
            lexer.setState(qssLexerState(prevBlock.userState()));
            parser.setState(qssParserState(prevBlock.userState()));
        }
        QList<Token> tokenList = lexer.scanMore(block.text());
        int index = findStartOfName(pos) - block.position();
        Parser::State parserState;
        Token currentToken, prevToken;
        QString prevString, currentString;
        Q_FOREACH (const Token &t, tokenList) {
            currentToken = t;
            if (t.begin() <= index && index < t.end()) {
                parserState = parser.state();
                currentString = block.text().mid(t.offset, t.length);
                break;
            }
            parser.parseMore(t);
            prevToken = t;
            prevString = currentString;
        }
        if (matchColor(parserState, prevToken, prevString, currentToken, currentString)) {
            setToolTip("Color");
            return;
        }
        if (matchGradient(parserState, prevToken, prevString, currentToken, currentString)) {
            setToolTip("Gradient");
            return;
        }

    }
}

void QssHoverHandler::operateTooltip(TextEditor::ITextEditor *editor, const QPoint &point)
{
    if (toolTip().isEmpty())
        TextEditor::ToolTip::instance()->hide();
    else {
        if (m_color.isValid()) {
            TextEditor::ToolTip::instance()->show(point,
                                                  TextEditor::ColorContent(m_color),
                                                  editor->widget());
        } else if (m_gradient) {
            GradientTipWidget *tw = new GradientTipWidget(m_gradient);
            m_tooltipWidgets.append(tw);
            TextEditor::ToolTip::instance()->show(point,
                                                  TextEditor::WidgetContent(tw),
                                                  editor->widget());
        } else {
            TextEditor::ToolTip::instance()->show(point,
                                                  TextEditor::TextContent(toolTip()),
                                                  editor->widget());
        }
    }
}

int QssHoverHandler::findStartOfName(int pos) const
{
    if (!m_editor)
        return -1;
    QChar chr;
    int p = pos;
    // Skip to the start of a name
    do {
        chr = m_editor->characterAt(--p);
    } while (chr.isLetterOrNumber() || chr == QLatin1Char('_') ||
             (chr == QLatin1Char('-') && m_editor->characterAt(pos - 1).isLetterOrNumber()));

    return p + 1;
}

bool QssHoverHandler::matchColor(Parser::State parserState, const Token &prevToken, const QString &prevString,
                                 const Token &currentToken, const QString &currentString)
{
    Q_UNUSED(prevString)
    m_color = QColor();
    if (parserState == Parser::AttributeBodyState) {
        if (prevToken.is(Token::Sharp)
                && (currentToken.is(Token::Number) || currentToken.is(Token::Identifier))) {
            m_color.setNamedColor("#" + currentString);
            return true;
        }
        if (currentToken.is(Token::Identifier) && QColor::isValidColor(currentString)) {
            m_color.setNamedColor(currentString);
            return true;
        }
        if (parserState == Parser::AttributeBodyState && currentToken.is(Token::Identifier)) {
            if (currentString == "rgb" || currentString == "rgba"
                    || currentString == "hsv" || currentString == "hsva") {
                QSSTextEditorWidget *qssEditor = qobject_cast<QSSTextEditorWidget *>(m_editor->widget());
                QTextBlock block = qssEditor->document()->findBlock(m_pos);
                QTextBlock prevBlock = block.previous();
                Lexer lexer;
                if (prevBlock.isValid())
                    lexer.setState(qssLexerState(prevBlock.userState()));
                QList<Token> colorTokens;
                QList<Token> tokenList = lexer.scanMore(block.text());
                while (!tokenList.isEmpty() && tokenList.first() != currentToken) {
                    tokenList.removeFirst();
                }
                bool terminated = mergeGradientTokens(colorTokens, tokenList);
                block = block.next();
                while (!terminated && block.isValid()) {
                    tokenList = lexer.scanMore(block.text());
                    terminated = mergeGradientTokens(colorTokens, tokenList);
                    block = block.next();
                }
                if (!terminated)
                    return false;
                Parser parser;
                m_color = parser.parseColor(colorTokens);
                if (m_color.isValid())
                    return true;
            }
        }

    }
    return false;
}

bool QssHoverHandler::matchGradient(Parser::State parserState, const Token &prevToken, const QString &prevString,
                                    const Token &currentToken, const QString &currentString)
{
    Q_UNUSED(prevString)
    Q_UNUSED(prevToken)
    if (m_gradient) {
        delete m_gradient;
        m_gradient = 0;
    }

    if (parserState == Parser::AttributeBodyState && currentToken.is(Token::Identifier)) {
        if (currentString == "qlineargradient" || currentString == "qradialgradient" || currentString == "qconicalgradient") {
            QSSTextEditorWidget *qssEditor = qobject_cast<QSSTextEditorWidget *>(m_editor->widget());
            QTextBlock block = qssEditor->document()->findBlock(m_pos);
            QTextBlock prevBlock = block.previous();
            Lexer lexer;
            if (prevBlock.isValid())
                lexer.setState(qssLexerState(prevBlock.userState()));
            QList<Token> gradientTokens;
            QList<Token> tokenList = lexer.scanMore(block.text());
            while (!tokenList.isEmpty() && tokenList.first() != currentToken) {
                tokenList.removeFirst();
            }
            bool terminated = mergeGradientTokens(gradientTokens, tokenList);
            block = block.next();
            while (!terminated && block.isValid()) {
                tokenList = lexer.scanMore(block.text());
                terminated = mergeGradientTokens(gradientTokens, tokenList);
                block = block.next();
            }
            if (!terminated)
                return false;
            Parser parser;
            m_gradient = parser.parseGradient(gradientTokens);
            if (m_gradient)
                return true;
        }
    }
    return false;
}

bool QssHoverHandler::mergeGradientTokens(QList<Token> &gradientList, const QList<Token> &tokenList)
{
    Q_FOREACH (const Token &t, tokenList) {
        gradientList.append(t);
        if (t.is(Token::RightParenthesis))
            return true;
    }
    return false;
}

void QssHoverHandler::purgeTooltips()
{
    qDebug() << Q_FUNC_INFO;
    QList<GradientTipWidget *> newTooltipWidgets;
    Q_FOREACH (GradientTipWidget *tw, m_tooltipWidgets) {
        if (tw->isVisible())
            newTooltipWidgets.append(tw);
        else {
            delete tw;
        }
    }
    m_tooltipWidgets = newTooltipWidgets;
}
