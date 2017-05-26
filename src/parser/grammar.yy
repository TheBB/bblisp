%skeleton "lalr1.cc"

%code requires{
#include <iostream>
class Lexer;
}

%defines
%define parser_class_name {Parser}
%define api.namespace {bison}

%param { Lexer &lexer }

%code{
static int yylex(bison::Parser::semantic_type *yylval, Lexer &lexer);
}

%union {
    int ival;
    float fval;
    char *sval;
}

%token SNAZZLE
%token TYPE
%token END

%token  <ival>          INT
%token  <fval>          FLOAT
%token  <sval>          STRING

%%

snazzle:        header template body_section footer { std::cout << "done with snazzle" << std::endl; }
        ;

header:         SNAZZLE FLOAT { std::cout << "snazzle file version " << $2 << std::endl; }
        ;

template:       type_lines
        ;

type_lines:     type_lines type_line
        |       type_line
        ;

type_line:      TYPE STRING { std::cout << "new snazzle type: " << $2 << std::endl; }
        ;

body_section:   body_lines
        ;

body_lines:     body_lines body_line
        |       body_line
        ;

body_line:      INT INT INT INT STRING { std::cout << "new snazzle: " << $1 << " " << $2 << " " << $3 << " " << $4 << " " << $5 << std::endl; }
        ;

footer:         END
        ;

%%

#include "lexer.hpp"

static int yylex(bison::Parser::semantic_type *yylval, Lexer &lexer) {
    return lexer.yylex(yylval);
}

void bison::Parser::error(const std::string &message) {
    std::cerr << "Error: " << message << std::endl;
}
