#include "qssindenter.h"
#include "qsslexer.h"
#include <texteditor/icodestylepreferences.h>
#include <texteditor/tabsettings.h>
#include <QTextBlock>
#include <QTextDocument>
#include <QStack>
#include <QList>
#include <QDebug>
using namespace QssEditor::Internal;

QSSIndenter::QSSIndenter() :
    TextEditor::Indenter()
{
}

bool QSSIndenter::isElectricCharacter(const QChar &ch) const
{
    if (ch == QLatin1Char('{') || (ch == QLatin1Char('}')))
        return true;
    return false;
}

void QSSIndenter::indentBlock(QTextDocument *doc, const QTextBlock &block, const QChar &typedChar, const TextEditor::TabSettings &tabSettings)
{
    if (!block.previous().isValid())
        return;
    int indentDepth = getIndentDepth(doc, block, typedChar);
    tabSettings.indentLine(block, indentDepth * tabSettings.m_indentSize);
}

void QSSIndenter::setCodeStylePreferences(TextEditor::ICodeStylePreferences *preferences)
{
    Q_UNUSED(preferences)
    qDebug() << Q_FUNC_INFO;
}

void QSSIndenter::invalidateCache(QTextDocument *doc)
{
    Q_UNUSED(doc)
    qDebug() << Q_FUNC_INFO;
}

int QSSIndenter::getIndentDepth(QTextDocument *doc, const QTextBlock &block, const QChar &typedChar)
{
    Lexer lexer;
    QList<Token> tokenList;
    QStack<Token> stack;
    int lineStartIndent = 0;
    int lineEndIndent = 0;
    int indentDepth = 0;
    for (QTextBlock b = doc->firstBlock(); b != block.next(); b = b.next()) {
        Lexer::States oldState = lexer.state();
        lineStartIndent = stack.count();
        tokenList = lexer.scanMore(b.text());
        Lexer::States newState = lexer.state();
        Q_FOREACH (const Token &t, tokenList) {
            switch (t.kind) {
            case Token::LeftParenthesis:
                stack.push(t);
                break;
            case Token::LeftBrace:
                stack.push(t);
                break;
            case Token::Colon:
                if (stack.count())
                    stack.push(t);
                break;
            case Token::RightParenthesis:
                if (stack.count() && stack.top().is(Token::Colon))
                    stack.pop();
                if (stack.count() && stack.top().is(Token::LeftParenthesis))
                    stack.pop();
                break;
            case Token::RightBrace:
                if (stack.count() && stack.top().is(Token::LeftBrace))
                    stack.pop();
                break;
            case Token::Semicolon:
            case Token::Comma:
                if (stack.count() && stack.top().is(Token::Colon))
                    stack.pop();
                break;
            default:
                ;
            }
        }
        lineEndIndent = stack.count();
        if (oldState == Lexer::Normal) {
            if (newState & Lexer::MultiLineMask)
                lineEndIndent++;
        } else {
            if (newState == Lexer::Normal)
                lineEndIndent--;
        }
    }
    if ((typedChar == QLatin1Char('}')) || (block.text().trimmed().startsWith('}')))
        indentDepth = qMax(0, lineEndIndent);
    else
        indentDepth = qMax(0, lineStartIndent);
    return indentDepth;
}
