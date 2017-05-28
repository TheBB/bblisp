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
    double fval;
    std::string *str;
    ASTNode *node;
}

%destructor {
    if ($$) { delete ($$); ($$) = nullptr; }
} <str> <node>

%token                  END 0
%token  <fval>          FLOAT
%token  <str>           SYMBOL DQ_STRING SQ_STRING

%type   <node>          expr lit_number lit_dq_string lit_sq_string symbol

%%

%start top;

top:            expr END { tree.set_root($1); }
        ;

expr:           lit_number
        |       lit_dq_string
        |       lit_sq_string
        |       symbol
        ;

lit_number:     FLOAT { $$ = new NumberNode($1); }
        ;

lit_dq_string:  DQ_STRING { $$ = new StringNode(*$1, '"'); }
                ;

lit_sq_string:  SQ_STRING { $$ = new StringNode(*$1, '\''); }
                ;

symbol:         SYMBOL { $$ = new SymbolNode(*$1); }
        ;

%%

#include "lexer.hpp"

static int yylex(bison::Parser::semantic_type *yylval, Lexer &lexer) {
    return lexer.yylex(yylval);
}

void bison::Parser::error(const std::string &message) {
    std::cerr << "Error: " << message << std::endl;
}
