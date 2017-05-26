%{
#include <iostream>

#include "lexer.hpp"
#include "gen_parser.hpp"

typedef bison::Parser::token token;
%}

%option debug
%option c++
%option yyclass="Lexer"
%option noyywrap
%option outfile="gen_lexer.cpp"
%option nounistd

%%

[ \t\n] ;
sNaZZle {
    return token::SNAZZLE;
}
type {
    return token::TYPE;
}
end {
    return token::END;
}
[0-9]+\.[0-9]+ {
    yylval->fval = atof(yytext);
    return token::FLOAT;
}
[0-9]+ {
    yylval->ival = atoi(yytext);
    return token::INT;
}
[a-zA-Z0-9]+ {
    yylval->sval = strdup(yytext);
    return token::STRING;
}
