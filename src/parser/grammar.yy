%skeleton "lalr1.cc"

%code requires{
#include <iostream>
#include "ast/ast.hpp"
#include "tree.hpp"
class Lexer;
}

%defines
%define parser_class_name {Parser}
%define api.namespace {bison}

%param { Lexer &lexer }
%parse-param { SourceTree &tree }

%code{
static int yylex(bison::Parser::semantic_type *yylval, Lexer &lexer);
}

%union {
    float fval;
    ASTNode *node;
}

%destructor {
    if ($$) { delete ($$); ($$) = nullptr; }
} <node>

%token                  END 0
%token  <fval>          FLOAT

%type   <node>          number_literal

%%

%start top;

top:            number_literal END { tree.set_root($1); }
        ;

number_literal: FLOAT { $$ = new NumberNode($1); }
        ;

%%

#include "lexer.hpp"

static int yylex(bison::Parser::semantic_type *yylval, Lexer &lexer) {
    return lexer.yylex(yylval);
}

void bison::Parser::error(const std::string &message) {
    std::cerr << "Error: " << message << std::endl;
}
