#include "qsscompletionassist.h"
#include "qsseditorconstants.h"
#include "qsslexer.h"
#include "qssparser.h"
#include "qsseditorutils.h"

#include <texteditor/codeassist/iassistinterface.h>
#include <texteditor/codeassist/genericproposal.h>
#include <texteditor/completionsettings.h>
#include <texteditor/texteditorsettings.h>
#include <texteditor/basetexteditor.h>

#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <QDebug>

using namespace TextEditor;
using namespace QssEditor::Internal;

// -------------------------
// QssAssistProposalItem
// -------------------------
QssAssistProposalItem::QssAssistProposalItem() :
    BasicProposalItem()
{}

QssAssistProposalItem::~QssAssistProposalItem()
{}

bool QssAssistProposalItem::prematurelyApplies(const QChar &c) const
{
    return BasicProposalItem::prematurelyApplies(c);
}

void QssAssistProposalItem::applyContextualContent(TextEditor::BaseTextEditor *editor,
                                                        int basePosition) const
{
    return BasicProposalItem::applyContextualContent(editor, basePosition);
}

// -------------------------------
// QssCompletionAssistProvider
// -------------------------------
QssCompletionAssistProvider::QssCompletionAssistProvider()
{}

QssCompletionAssistProvider::~QssCompletionAssistProvider()
{}

bool QssCompletionAssistProvider::supportsEditor(const Core::Id &editorId) const
{
    return editorId == QssEditor::Constants::C_QSSEDITOR_ID;
}

bool QssCompletionAssistProvider::isAsynchronous() const
{
    return false;
}

int QssCompletionAssistProvider::activationCharSequenceLength() const
{
    return 0;
}

bool QssCompletionAssistProvider::isActivationCharSequence(const QString &sequence) const
{
    Q_UNUSED(sequence);
    return false;
}

bool QssCompletionAssistProvider::isContinuationChar(const QChar &c) const
{
    return c.isLetterOrNumber() || c == QLatin1Char('_');
}

IAssistProcessor *QssCompletionAssistProvider::createProcessor() const
{
    return new QssCompletionAssistProcessor;
}

// --------------------------------
// QssCompletionAssistProcessor
// --------------------------------
QssCompletionAssistProcessor::QssCompletionAssistProcessor()
    : m_startPosition(-1)
{}

QssCompletionAssistProcessor::~QssCompletionAssistProcessor()
{}

IAssistProposal *QssCompletionAssistProcessor::perform(const IAssistInterface *interface)
{
    m_interface.reset(interface);

    if (isInComment())
        return 0;

    if (interface->reason() == IdleEditor && !acceptsIdleEditor())
        return 0;

    if (m_startPosition == -1)
        m_startPosition = findStartOfName();

    QTextBlock block = interface->textDocument()->findBlock(interface->position());
    QTextBlock prevBlock = block.previous();
    Lexer lexer;
    Parser parser;
    if (prevBlock.isValid()) {
        lexer.setState(qssLexerState(prevBlock.userState()));
        parser.setState(qssParserState(prevBlock.userState()));
    }
    QList<Token> tokenList = lexer.scanMore(block.text());
    int index = m_startPosition - block.position();
    Q_FOREACH (const Token &t, tokenList) {
        if (index <= t.begin())
            break;
        parser.parseMore(t);
    }
    QStringList keywords;
    QIcon icon;
    switch (parser.state()) {
    case Parser::ObjectState:
        keywords << Lexer::objects();
        icon = QIcon(":/qsseditor/images/class.png");
        break;
    case Parser::PseudoStatesState:
        keywords << Lexer::pseudoStates();
        icon = QIcon(":/qsseditor/images/func.png");
        break;
    case Parser::SubControlState:
        keywords << Lexer::subControls();
        icon = QIcon(":/qsseditor/images/func.png");
        break;
    case Parser::AttributeNameState:
        keywords << Lexer::attributeNames();
        icon = QIcon(":/qsseditor/images/var.png");
        break;
    case Parser::AttributeBodyState:
        keywords << Lexer::attributeCtors() << Lexer::attributeKeywords();
        icon = QIcon(":/qsseditor/images/keyword.png");
        break;
    default:
        ;
    }
    keywords.removeDuplicates();
    QList<TextEditor::BasicProposalItem *> items;
    for (int i = 0; i < keywords.count(); i++) {
        BasicProposalItem *item = new QssAssistProposalItem;
        item->setText(keywords[i]);
        item->setIcon(icon);
        items.append(item);
    }
    return new GenericProposal(m_startPosition, new QssAssistProposalModel(items));
}

bool QssCompletionAssistProcessor::acceptsIdleEditor()
{
    const int pos = m_interface->position();
    QChar characterUnderCursor = m_interface->characterAt(pos);
    if (!characterUnderCursor.isLetterOrNumber()) {
        m_startPosition = findStartOfName();
        if (pos - m_startPosition >= 3  && !isInComment())
            return true;
    }
    return false;
}

int QssCompletionAssistProcessor::findStartOfName(int pos) const
{
    if (pos == -1)
        pos = m_interface->position();
    QChar chr;
    int p = pos;

    // Skip to the start of a name
    do {
        chr = m_interface->characterAt(--p);
    } while (chr.isLetterOrNumber() || chr == QLatin1Char('_') ||
             (chr == QLatin1Char('-') && m_interface->characterAt(pos - 1).isLetterOrNumber()));

    return p + 1;
}

bool QssCompletionAssistProcessor::isInComment() const
{
    QTextBlock block = m_interface->textDocument()->findBlock(m_interface->position());
    QTextBlock prevBlock = block.previous();
    QList<Token> tokenList;
    Lexer lexer;
    lexer.setScanComments(true);
    if (prevBlock.isValid())
        lexer.setState(qssLexerState(prevBlock.userState()));
    tokenList = lexer.scanMore(block.text());
    int index = m_interface->position() - block.position();
    Q_FOREACH (const Token &t, tokenList) {
        if (t.is(Token::Comment) && index >= t.begin() && index < t.end()) {
            qDebug() << "InComment";
            return true;
        }
    }
    if (index == block.length() && (lexer.state() & Lexer::MultiLineComment)) {
        qDebug() << "InComment";
        return true;
    }
    return false;
}

// --------------------------
// QssAssistProposalModel
// --------------------------
QssAssistProposalModel::QssAssistProposalModel(
    const QList<BasicProposalItem *> &items)
    : BasicProposalItemListModel(items)
{}

QssAssistProposalModel::~QssAssistProposalModel()
{}

bool QssAssistProposalModel::isSortable(const QString &prefix) const
{
    Q_UNUSED(prefix)
    return false;
}
