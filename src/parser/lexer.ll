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

blank    [ \t]
symbol   [a-zA-Z_][0-9a-zA-Z_]*
numeric  [0-9]*([0-9]\.|\.[0-9]|[0-9])[0-9]*([eE][+-]?[0-9]+)?
dqstring \"(\\.|[^\\"])*\"
sqstring \'(\\.|[^\\'])*\'

%%

{blank}+ ;

"def" { return token::DEF; }
"extern" { return token::EXTERNAL; }
"if" { return token::IF; }
"then" { return token::THEN; }
"else" { return token::ELSE; }
"for" { return token::FOR; }
"in" { return token::IN; }
"let" { return token::LET; }

"(" { return token::OPEN_PAREN; }
")" { return token::CLOSE_PAREN; }
"," { return token::COMMA; }

"=" { return token::ASSIGN; }
"+" { return token::PLUS; }
"-" { return token::MINUS; }
">" { return token::GT; }
"<" { return token::LT; }
"*" { return token::MULTIPLY; }
"/" { return token::DIVIDE; }

{numeric} {
    yylval->fval = strtod(yytext, nullptr);
    return token::FLOAT;
}

{symbol} {
    yylval->str = new std::string(yytext);
    return token::SYMBOL;
}

{dqstring} {
    yytext[strlen(yytext)-1] = 0;
    yylval->str = new std::string(&yytext[1]);
    return token::DQ_STRING;
}

{sqstring} {
    yytext[strlen(yytext)-1] = 0;
    yylval->str = new std::string(&yytext[1]);
    return token::SQ_STRING;
}
