%{
#include <iostream>

#include "lexer.hpp"
#include "gen_parser.hpp"

typedef bison::Parser::token token;

#define yyterminate() return token::END
%}

%option debug
%option c++
%option yyclass="Lexer"
%option noyywrap
%option outfile="gen_lexer.cpp"
%option nounistd

blank   [ \t]
symbol  [a-zA-Z_][0-9a-zA-Z_]*
numeric [0-9]*([0-9]\.|\.[0-9]|[0-9])[0-9]*([eE][+-]?[0-9]+)?

%%

{blank}+ ;

{numeric} {
    yylval->fval = strtod(yytext, nullptr);
    return token::FLOAT;
}

{symbol} {
    yylval->str = new std::string(yytext);
    return token::SYMBOL;
}
