#include "lexer.hpp"


Lexer::Lexer(std::istream *in) : yyFlexLexer(in), yylval(nullptr) {}


int Lexer::yylex(bison::Parser::semantic_type *lval)
{
    yylval = lval;
    return yylex();
}
