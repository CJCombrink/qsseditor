#ifndef QSSEDITORUTILS_H
#define QSSEDITORUTILS_H

#include "qsslexer.h"
#include "qssparser.h"

namespace QssEditor {
namespace Internal {

int qssMakeBlockState(Lexer::States lexerState, Parser::State parserState);
Lexer::States qssLexerState(int blockState);
Parser::State qssParserState(int blockState);

} // Internal
} // QssEditor

#endif // QSSEDITORUTILS_H
