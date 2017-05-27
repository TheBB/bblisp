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
numeric [0-9]*([0-9]\.|\.[0-9]|[0-9])[0-9]*

%%

{blank}+ ;

{numeric} {
    yylval->fval = atof(yytext);
    return token::FLOAT;
}
