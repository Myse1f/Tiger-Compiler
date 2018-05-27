%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "past.h"
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
    ty_t ty;
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
%token
    COMMA COLON LPAREN RPAREN LBRACK RBRACK 
    LBRACE RBRACE DOT 
    ARRAY IF THEN ELSE WHILE FOR TO LET IN END OF 
    BREAK NIL FUNCTION VAR TYPE

%left SEMICOLON
%nonassoc DO
%nonassoc ASSIGN
%left OR
%left AND
%nonassoc EQ NEQ LT LE GT GE
%left PLUS MINUS
%left TIMES DIVIDE
%left UMINUS

%type <exp> program exp
%type <expList> exp_seq arg_seq
%type <ty> ty
%type <var> lvalue
%type <dec> dec vardec
%type <decLsit> decs 
%type <namety> tydec
%type <nametyList> tydecs
%type <fundec> fundec
%type <fundecList> fundecs
%type <fieldList> fields field
%type <efieldList> efield_seq

%start program

%%

program:
    exp
    {
        ast_root = $1;
    }
    ;

exp:
    lvalue
    {
        $$ = VarExp(EM_tokPos, $1);
    }
    | lvalue ASSIGN exp 
    {
        $$ = AssignExp(EM_tokPos, $1, $3);
    }
    | INT
    {
        $$ = IntExp(EM_tokPos, $1);
    }
    | STRING
    {
        $$ = StringExp(EM_tokPos);
    }
    | NIL 
    {
        $$ = NilExp(EM_tokPos, $1);
    }

    | LPAREN RPAREN
    {
        $$ = SeqExp(EM_tokPos, NULL);
    }
    | LPAREN exp_seq RPAREN
    {
        $$ = SeqExp(EM_tokPos, $2);
    }

    /* binary operation */
    | MINUS exp %prec UMINUS
    {
        $$ = OpExp(EM_tokPos, A_minusOp, IntExp(EM_tokPos, 0), $2);
    }
    | exp PLUS exp
    {
        $$ = OpExp(EM_tokPos, A_plusOp, $1, $3);
    }
    | exp MINUS exp 
    {
        $$ = OpExp(EM_tokPos, A_minusOp, $1, $3);
    }
    | exp TIMES exp
    {
        $$ = OpExp(EM_tokPos, A_timesOp, $1, $3);
    }
    | exp DIVIDE exp 
    {
        $$ = OpExp(EM_tokPos, A_divideOp, $1, $3);
    }
    | exp EQ exp 
    {
        $$ = OpExp(EM_tokPos, A_eqOp, $1, $3);
    } 
    | exp NEQ exp 
    {
        $$ = OpExp(EM_tokPos, A_neqOp, $1, $3);
    }
    | exp LT exp 
    {
        $$ = OpExp(EM_tokPos, A_ltOp, $1, $3);
    }
    | exp GT exp 
    {
        $$ = OpExp(EM_tokPos, A_gtOp, $1, $3);
    }
    | exp LE exp 
    {
        $$ = OpExp(EM_tokPos, A_leOp, $1, $3);
    }
    | exp GE exp 
    {
        $$ = OpExp(EM_tokPos, A_geOp, $1, $3);
    }
    
    /* condition AND and OR */
    | exp AND exp 
    {
        $$ = IfExp(EM_tokPos, $1, $3, IntExp(EM_tokPos, 0));
    }
    | exp OR exp 
    {
        $$ = IfExp(EM_tokPos, $1, IntExp(EM_tokPos, 1), $3);
    }

    /* function call */
    | ID LPAREN RPAREN
    {
        $$ = CallExp(EM_tokPos, Symbol($1), NULL);
    }
    | ID LPAREN arg_seq RPAREN
    {
        $$ = CallExp(EM_tokPos, Symbol($1), $3);
    }
    
    /* array */
    | ID LBRACK exp RBRACK OF exp 
    {
        $$ = ArrayExp(EM_tokPos, Symbol($1), $3, $6);
    }
    /* record */
    | ID LBRACE RBRACE
    {
        $$ = RecordExp(EM_tokPos, Symbol($1), NULL);
    }
    | ID LBRACE efield_seq RBRACE
    {
        $$ = RecordExp(EM_tokPos, Symbol($1), $3);
    }

    /* if then */
    | IF exp THEN exp 
    {
        $$ = IfExp(EM_tokPos, $2, $4, NULL);
    }
    | IF exp THEN exp ELSE exp
    {
        $$ = IfExp(EM_tokPos, $2, $4, $6);
    }
    /* loop */
    | WHILE exp DO exp
    {
        $$ = WhileExp(EM_tokPos, $2, $4);
    }
    | FOR ID ASSIGN exp TO exp DO exp 
    {
        $$ = ForExp(EM_tokPos, Symbol($2), $4, $6, $8);
    }
    | BREAK 
    {
        $$ = BreakExp(EM_tokPos);
    }

    /* let */
    | LET decs IN END
    {
        $$ = LetExp(EM_tokPos, $2, SeqExp(EM_tokPos, NULL));
    }
    | LET decs IN exp_seq END 
    {
        $$ = LetExp(EM_tokPos, $2, SeqExp(EM_tokPos, $4));
    }
    
    /* error */
    | LPAREN error RPAREN
    {
        $$ = SeqExp(EM_tokPos, NULL);
    }
    | error SEMICOLON exp 
    {
        $$ = $3;
    }
    ;

lvalue: 
    ID
    {
        $$ = SimpleVar(EM_tokPos, Symbol($1));
    }
    | lvalue DOT ID 
    {
        $$ = FieldVar(EM_tokPos, $1, Symbol($3));
    }
    | lvalue LBRACK exp RBRACK 
    {
        $$ = SubscriptVar(EM_tokPos, $1, $3);
    }
    | ID LBRACK exp RBRACK 
    {
        $$ = SubscriptVar(EM_tokPos, SimpleVar(EM_tokPos, Symbol($1)), $3);
    }
    ;

exp_seq: 
    exp 
    {
        $$ = ExpList($1, NULL);
    }
    | exp SEMICOLON exp_seq
    {
        $$ = ExpList($1, $3);
    }
    ;

arg_seq:
    exp 
    {
        $$ = ExpList($1, NULL);
    }
    | exp COMMA arg_seq
    {
        $$ = ExpList($1, $3);
    }
    ;

efield_seq:
    ID EQ exp 
    {
        $$ = EfieldList(Efield(Symbol($1), $3), NULL);
    }
    | ID EQ exp COMMA efield_seq
    {
        $$ = EfieldList(Efield(Symbol($1), $3), $5);
    }
    ;

decs:
    dec decs 
    {
        $$ = DecList($1, $2);
    }
    | /* empty */
    {
        $$ = NULL;
    }
    ;

dec:
    tydecs
    {
        $$ = TypeDec(EM_tokPos, $1);
    }
    |
    vardec
    {
        $$ = $1;
    }
    | fundecs 
    {
        $$ = FunctionDec(EM_tokPos, $1);
    }
    ;

tydecs:
    tydec 
    {
        $$ = NametyList($1, NULL);
    }
    |
    tydec tydecs 
    {
        $$ = NametyList($1, $2);
    }
    ;

tydec:
    TYPE ID EQ ty 
    {
        $$ = Namety(Symbol($2), $4);
    }
    ;

ty:
    ID 
    {
        $$ = NameTy(EM_tokPos, S_Symbol($1));
    }
    | LBRACE fields RBRACE
    {
        $$ = RecordTy(EM_tokPos, $2);
    }
    | ARRAY OF ID 
    {
        $$ = ArrayTy(EM_tokPos, Symbol($3));
    }
    ;

fields:
    field 
    {
        $$ = $1;
    }
    | /* empty */
    {
        $$ = NULL;
    }
    ;

field:
    ID COLON ID 
    {
        $$ = FieldList(Field(EM_tokPos, Symbol($1), Symbol($3)), NULL);
    }
    | ID COLON ID COMMA field
    {
        $$ = FieldList(Field(EM_tokPos, Symbol($1), Symbol($3)), $5);
    }
    ;

vardec:
    VAR ID ASSIGN exp
    {
        $$ = VarDec(EM_tokPos, Symbol($2), NULL, $4);
    }
    | VAR ID COLON ID ASSIGN exp
    {
        $$ = VarDec(EM_tokPos, Symbol($2), Symbol($4), $6);
    }
    ;

fundecs:
    fundec
    {
        $$ = FundecList($1, NULL);
    }
    | fundec fundecs 
    {
        $$ = FundecList($1, $2);
    }
    ;

fundec:
    FUNCTION ID LPAREN fields RPAREN EQ exp
    {
        $$ = Fundec(EM_tokPos, Symbol($2), $4, NULL, $7);
    }
    | FUNCTION ID LPAREN fields RPAREN COLON ID EQ exp
    {
        $$ = Fundec(EM_tokPos, Symbol($2), $4, Symbol($7), $9);
    }
    ;

%%

void yyerror(char *msg) {
    EM_error(EM_tokPos, "%s", msg);
}
