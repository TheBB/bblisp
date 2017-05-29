#pragma once

#include <sstream>

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "gen_parser.hpp"

class Lexer: public yyFlexLexer
{
    int yylex();
    bison::Parser::semantic_type *yylval;

public:
    Lexer(std::istream *in);
    int yylex(bison::Parser::semantic_type *yylval);
};
