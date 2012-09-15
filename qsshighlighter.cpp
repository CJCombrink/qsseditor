#include "qsshighlighter.h"
#include "qsseditorconstants.h"
#include "qsslexer.h"
#include "qssparser.h"
#include "qsseditorutils.h"
#include <QString>
#include <QDebug>
#include <QTextBlock>
using namespace QssEditor;
using namespace QssEditor::Internal;

Highlighter::Highlighter(QTextDocument *document) :
    TextEditor::SyntaxHighlighter(document)
{
}

void Highlighter::highlightBlock(const QString &text)
{
    Lexer lexer;
    lexer.setScanComments(true);
    Parser parser;
    int blockState = previousBlockState();
    Lexer::States lexerState;
    Parser::State parserState;
    if (blockState == -1) {
        lexerState = Lexer::Normal;
        parserState = Parser::ObjectState;
    } else {
        lexerState = qssLexerState(blockState);
        parserState = qssParserState(blockState);
    }
    parser.setState(parserState);
    QList<Token> tokenList = lexer.scan(text, lexerState);
    Format idFormat;
    Token lastToken;
    Q_FOREACH (const Token &t, tokenList) {
        parser.parseMore(t);
        switch (t.kind) {
        case Token::Identifier:
            if (parser.state() == Parser::ObjectState)
                idFormat = ObjectFormat;
            else if (parser.state() == Parser::PseudoStatesState)
                idFormat = PseudoStateFormat;
            else if (parser.state() == Parser::SubControlState)
                idFormat = SubControlFormat;
            else if (parser.state() == Parser::AttributeNameState)
                idFormat = AttributeNameFormat;
            else if (parser.state() == Parser::AttributeBodyState) {
                if (lastToken.is(Token::Sharp) && lastToken.end() == t.begin()) {
                    setFormat(lastToken.offset, lastToken.length, m_formats[NumberFormat]);
                    idFormat = NumberFormat;
                } else if (lexer.isAttributeCtor(text.mid(t.offset, t.length)))
                    idFormat = ObjectFormat;
                else if (lexer.isAttributeKeyword(text.mid(t.offset, t.length)))
                    idFormat = AttributeNameFormat;
                else
                    idFormat = IdentifilerFormat;
            } else
                idFormat = IdentifilerFormat;
            setFormat(t.offset, t.length, m_formats[idFormat]);
            break;
        case Token::Number:
            if (lastToken.is(Token::Sharp) && lastToken.end() == t.begin())
                setFormat(lastToken.offset, lastToken.length, m_formats[NumberFormat]);
            setFormat(t.offset, t.length, m_formats[NumberFormat]);
            break;
        case Token::String:
            setFormat(t.offset, t.length, m_formats[StringFormat]);
            break;
        case Token::Comment:
            setFormat(t.offset, t.length, m_formats[CommentFormat]);
            break;
        default:
            ;
        }
        lastToken = t;
    }
    setCurrentBlockState(qssMakeBlockState(lexer.state(), parser.state()));
    applyFormatToSpaces(text, m_formats[VisualWhiteSpaceFormat]);
}
