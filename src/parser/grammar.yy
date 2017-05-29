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
    char chr;
    ASTNode *node;
    PrototypeNode *proto_node;
    FunctionNode *func_node;
    SymbolNode *sym_node;
    std::vector<SymbolNode*> *syms;
    binding_t *binding;
    bindings_t *bindings;
}

%destructor {
    if ($$) { delete ($$); ($$) = nullptr; }
} <str> <node>

%token                  END 0
%token                  OPEN_PAREN CLOSE_PAREN
%token                  IF THEN ELSE
%token                  FOR COMMA IN
%token                  LET
%token                  DEF EXTERNAL
%token  <fval>          FLOAT
%token  <str>           SYMBOL DQ_STRING SQ_STRING

%type   <node>          expr operator branch loop let lit_number lit_dq_string lit_sq_string
%type   <proto_node>    prototype external
%type   <func_node>     function
%type   <sym_node>      symbol
%type   <syms>          arguments
%type   <binding>       binding
%type   <bindings>      bindings

%%

%start top;

top:            function END { tree.set_root($1); }
        |       external END { tree.set_root($1); }
        |       expr END { tree.set_root($1); }
        ;

function:       DEF prototype expr { $$ = new FunctionNode($2, $3); }
        ;

external:       EXTERNAL prototype { $$ = $2; }
        ;

prototype:      symbol OPEN_PAREN arguments CLOSE_PAREN { $$ = new PrototypeNode($1, *$3); }
        ;

arguments:      { $$ = new std::vector<SymbolNode*>(); }
        |       arguments COMMA symbol { $$ = $1; $$->push_back($3); }
        |       symbol { $$ = new std::vector<SymbolNode*>(); $$->push_back($1); }
        ;

expr:           operator
        |       branch
        |       loop
        |       let
        |       lit_number
        |       lit_dq_string
        |       lit_sq_string
        |       symbol { $$ = $1; }
        |       OPEN_PAREN expr CLOSE_PAREN { $$ = $2; }
        ;

%left ASSIGN;
%left GT LT;
%left PLUS MINUS;
%left MULTIPLY DIVIDE;
%left NEGATE;

operator:       MINUS expr %prec NEGATE { $$ = new UnaryNode(UnaryNode::Operator::minus, $2); }
        |       expr ASSIGN expr { $$ = new BinaryNode(BinaryNode::Operator::assign, $1, $3); }
        |       expr PLUS expr { $$ = new BinaryNode(BinaryNode::Operator::plus, $1, $3); }
        |       expr MINUS expr { $$ = new BinaryNode(BinaryNode::Operator::minus, $1, $3); }
        |       expr GT expr { $$ = new BinaryNode(BinaryNode::Operator::gt, $1, $3); }
        |       expr LT expr { $$ = new BinaryNode(BinaryNode::Operator::lt, $1, $3); }
        |       expr MULTIPLY expr { $$ = new BinaryNode(BinaryNode::Operator::multiply, $1, $3); }
        |       expr DIVIDE expr { $$ = new BinaryNode(BinaryNode::Operator::divide, $1, $3); }
        ;

lit_number:     FLOAT { $$ = new NumberNode($1); }
        ;

lit_dq_string:  DQ_STRING { $$ = new StringNode(*$1, '"'); }
        ;

lit_sq_string:  SQ_STRING { $$ = new StringNode(*$1, '\''); }
        ;

symbol:         SYMBOL { $$ = new SymbolNode(*$1); }
        ;

%left ELSE THEN;
%right COMMA IN;

branch:         IF expr THEN expr ELSE expr { $$ = new BranchNode($2, $4, $6); }
        ;

loop:           FOR symbol ASSIGN expr COMMA expr IN expr {
                  $$ = new LoopNode($2, $4, $6, nullptr, $8);
                }
        |       FOR symbol ASSIGN expr COMMA expr COMMA expr IN expr {
                  $$ = new LoopNode($2, $4, $6, $8, $10);
                }
        ;

let:            LET bindings IN expr { $$ = new LetNode(*$2, $4); }
        ;

bindings:       { $$ = new bindings_t(); }
        |       bindings COMMA binding { $$ = $1; $$->push_back(*$3); }
        |       binding { $$ = new bindings_t(); $$->push_back(*$1); }
        ;

binding:        symbol ASSIGN expr { $$ = new binding_t($1, $3); }
        |       symbol { $$ = new binding_t($1, nullptr); }
        ;

%%

#include "lexer.hpp"

static int yylex(bison::Parser::semantic_type *yylval, Lexer &lexer) {
    return lexer.yylex(yylval);
}

void bison::Parser::error(const std::string &message) {
    std::cerr << "Error: " << message << std::endl;
}
