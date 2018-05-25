%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "errmsg.h"
#include "symbol.h"
#include "utils.h"

static exp_t ast_root;

int yylex(void);
void yyerror(char *msg);
%}

%debug

%union {
    int pos;
    int num;
    string_t str;
    symbol_t sym;
    var_t var;
    exp_t exp;
    dec_t dec;
    ty_t type;
    decList_t decList;
    expList_t expList;
    field_t field;
    fieldList_t fieldList;
    fundec_t fundec;
    fundecList_t funList;
    namety_t namety;
    nametyList_t nametyList;
    efield_t efield;
    efieldList_t efieldList;
}

%token <str> ID STRING
%token <num> INT
%token <pos> 
    COMMA COLON LPAREN RPAREN LBRACK RBRACK 
    LBRACE RBRACE DOT 
    ARRAY IF THEN ELSE WHILE FOR TO LET IN END OF 
    BREAK NIL FUNCTION VAR TYPE

%left <pos> SEMICOLON
%nonassoc <pos> DO
%nonassoc <pos> ASSIGN
%left <pos> OR
%left <pos> AND
%nonassoc <pos> EQ NEQ LT LE GT GE
%left <pos> PLUS MINUS
%left <pos> TIMES DIVIDE
%left <pos> UMINUS

%type <dec> dec var_dec
%type <exp> program exp
%type <type> type
%type <var> lvalue lvalue_
%type <expList> exp_seq arg_seq
%type <decLsit> decs 
%type <namety> tydec
%type <nametyList> tydecs
%type <fundec> fundec
%type <fundecList> fundecs
%type <ty> ty
%type <fieldList> fields field
%type <efieldList> efield_seq

%start program

%%

program:
    exp
    {
        ast_root = $1;
    }

exp:
    lvalue
    {
        $$ = VarExp($1->pos, $1);
    }