#include "qsseditorutils.h"
#include "qsslexer.h"
#include "qssparser.h"
namespace QssEditor {
namespace Internal {

int qssMakeBlockState(Lexer::States lexerState, Parser::State parserState)
{
    return (((int)lexerState & 0xFF) | (((int)parserState & 0xFF) << 8));
}

Lexer::States qssLexerState(int blockState)
{
    return (Lexer::States)(blockState & 0xFF);
}

Parser::State qssParserState(int blockState)
{
    return (Parser::State)((blockState & 0xFF00) >> 8);
}

}
}
