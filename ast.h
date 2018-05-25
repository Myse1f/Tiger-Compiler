#ifndef _AST_H_
#define _AST_H_

#include "utils.h"
#include "symbol.h"

typedef int pos_t;

typedef struct var_s *var_t;
typedef struct exp_s *exp_t;
typedef struct dec_s *dec_t;
typedef struct ty_s  *ty_t;

typedef struct decList_s     *decList_t;
typedef struct expList_s     *expList_t;
typedef struct field_s       *field_t;
typedef struct fieldList_s   *fieldList_t;
typedef struct fundec_s      *fundec_t;
typedef struct fundecList_s  *fundecList_t;
typedef struct namety_s      *namety_t;
typedef struct nametyList_s  *nametyList_t;
typedef struct efield_s      *efield_t;
typedef struct efieldList_s  *efieldList_t;

typedef enum {A_plusOp, A_minusOp, A_timesOp, A_divideOp,
	     A_eqOp, A_neqOp, A_ltOp, A_leOp, A_gtOp, A_geOp} A_oper;

struct var_s{   
    enum {A_simpleVar, A_fieldVar, A_subscriptVar} kind;
    pos_t pos;
    union {
        symbol_t simple;
        struct {
            var_t var;
            symbol_t sym;
        } field;
        struct {
            var_t var;
            exp_t exp;
        } subscript;
    } u;
};

struct exp_s{
	enum {
		A_varExp, A_nilExp, A_intExp, A_stringExp, A_callExp,
		A_opExp, A_recordExp, A_seqExp, A_assignExp, A_ifExp,
		A_whileExp, A_forExp, A_breakExp, A_letExp, A_arrayExp
	} kind;
	pos_t pos;
	union {
		var_t var;
		/* nil; - needs only the pos */
		int intt;
		string_t string;
		struct {symbol_t func; expList_t args;} call;
		struct {A_oper oper; exp_t left; exp_t right;} op;
		struct {symbol_t typ; efieldList_t fields;} record;
		expList_t seq;
		struct {var_t var; exp_t exp;} assign;
		struct {exp_t test, then, elsee;} iff; /* elsee is optional */
		struct {exp_t test, body;} whilee;
		struct {symbol_t var; exp_t lo,hi,body; bool escape;} forr;
		/* breakk; - need only the pos */
		struct {decList_t decs; exp_t body;} let;
		struct {symbol_t typ; exp_t size, init;} array;
	} u;
};

struct dec_s {
	enum {A_functionDec, A_varDec, A_typeDec} kind;
	pos_t pos;
	union {
		fundecList_t function;
		/* escape may change after the initial declaration */
	    struct {symbol_t var; symbol_t typ; exp_t init; bool escape;} var;
	    nametyList_t type;
	} u;
};

struct ty_s {
	enum {A_nameTy, A_recordTy, A_arrayTy} kind;
	pos_t pos;
	union {
		symbol_t var;
		fieldList_t record;
		symbol_t array;
	} u;
};

//linked lists
struct field_s {symbol_t name, typ; pos_t pos; bool escape;};
struct fieldList_s {field_t head; fieldList_t tail;};
struct expList_s {exp_t head; expList_t tail;};
struct fundec_s {
	pos_t pos;
    symbol_t name; 
	fieldList_t params; 
	symbol_t result; 
	exp_t body;
};
struct fundecList_s {fundec_t head; fundecList_t tail;};
struct decList_s {dec_t head; decList_t tail;};
struct namety_s {symbol_t name; ty_t ty;};
struct nametyList_s {namety_t head; nametyList_t tail;};
struct efield_s {symbol_t name; exp_t exp;};
struct efieldList_s {efield_t head; efieldList_t tail;};

//function prototypes
var_t 			SimpleVar(pos_t pos, symbol_t sym);
var_t 			FieldVar(pos_t pos, var_t var, symbol_t sym);
var_t 			SubscriptVar(pos_t pos, var_t var, exp_t exp);
exp_t 			VarExp(pos_t pos, var_t var);
exp_t 			NilExp(pos_t pos);
exp_t			IntExp(pos_t pos, int i);
exp_t 			StringExp(pos_t pos, string_t s);
exp_t			CallExp(pos_t pos, symbol_t func, expList_t args);
exp_t 			OpExp(pos_t pos, A_oper oper, exp_t left, exp_t right);
exp_t			RecordExp(pos_t pos, symbol_t typ, efieldList_t fields);
exp_t 			SeqExp(pos_t pos, expList_t seq);
exp_t 			AssignExp(pos_t pos, var_t var, exp_t exp);
exp_t 			IfExp(pos_t pos, exp_t test, exp_t then, exp_t elsee);
exp_t 			WhileExp(pos_t pos, exp_t test, exp_t body);
exp_t 			ForExp(pos_t pos, symbol_t var, exp_t lo, exp_t hi, exp_t body);
exp_t 			BreakExp(pos_t pos);
exp_t 			LetExp(pos_t pos, decList_t decs, exp_t body);
exp_t 			ArrayExp(pos_t pos, symbol_t typ, exp_t size, exp_t init);
dec_t 			FunctionDec(pos_t pos, fundecList_t function);
dec_t 			VarDec(pos_t pos, symbol_t var, symbol_t typ, exp_t init);
dec_t 			TypeDec(pos_t pos, nametyList_t type);
ty_t  			NameTy(pos_t pos, symbol_t name);
ty_t  			RecordTy(pos_t pos, fieldList_t record);
ty_t  			ArrayTy(pos_t pos, symbol_t array);
field_t 		Field(pos_t pos, symbol_t name, symbol_t typ);
fieldList_t 	FieldList(field_t head, fieldList_t tail);
expList_t 		ExpList(exp_t head, expList_t tail);
fundec_t 		Fundec(pos_t pos, symbol_t name, fieldList_t params, symbol_t result, exp_t body);
fundecList_t 	FundecList(fundec_t head, fundecList_t tail);
decList_t 		DecList(dec_t head, decList_t tail);
namety_t 		Namety(symbol_t name, ty_t ty);
nametyList_t 	NametyList(namety_t head, nametyList_t tail);
efield_t		Efield(symbol_t name, exp_t exp);
efieldList_t 	EfieldList(efield_t head, efieldList_t tail);


#endif
