#include "ast.h"

var_t SimpleVar(pos_t pos, symbol_t sym){
    var_t p = checked_malloc(sizeof(*p));
    p->kind = A_simpleVar;
    p->pos = pos;
    p->u.simple = sym;

    return p;
}

var_t FieldVar(pos_t pos, var_t var, symbol_t sym) {
    var_t p = checked_malloc(sizeof(*p));
    p->kind = A_fieldVar;
    p->pos = pos;
    p->u.field.var = var;
    p->u.field.sym = sym;

    return p;
}

var_t SubscriptVar(pos_t pos, var_t var, exp_t exp) {
    var_t p = checked_malloc(sizeof(*p));
    p->kind = A_subscriptVar;
    p->pos = pos;
    p->u.subscript.var = var;
    p->u.subscript.exp = exp;

    return p;   
}

exp_t VarExp(pos_t pos, var_t var) {
    exp_t p = checked_malloc(sizeof(*p));
    p->kind = A_varExp;
    p->pos = pos;
    p->u.var = var;

    return p;
}

exp_t NilExp(pos_t pos) {
    exp_t p = checked_malloc(sizeof(*p));
    p->kind = A_nilExp;
    p->pos = pos;

    return p;
}

exp_t IntExp(pos_t pos, int i) {
    exp_t p = checked_malloc(sizeof(*p));
    p->kind = A_intExp;
    p->pos = pos;
    p->u.intt = i;

    return p;
}

exp_t StringExp(pos_t pos, string_t s) {
    exp_t p = checked_malloc(sizeof(*p));
    p->kind = A_stringExp;
    p->pos = pos;
    p->u.string = s;

    return p;
}

exp_t CallExp(pos_t pos, symbol_t func, expList_t args) {
    exp_t p = checked_malloc(sizeof(*p));
    p->kind = A_callExp;
    p->pos = pos;
    p->u.call.func = func;
    p->u.call.args = args;

    return p;
}

exp_t OpExp(pos_t pos, A_oper oper, exp_t left, exp_t right) {
    exp_t p = checked_malloc(sizeof(*p));
    p->kind = A_opExp;
    p->pos = pos;
    p->u.op.oper = oper;
    p->u.op.left = left;
    p->u.op.right = right;

    return p;
}

exp_t RecordExp(pos_t pos, symbol_t typ, efieldList_t fields) {
    exp_t p = checked_malloc(sizeof(*p));
    p->kind = A_recordExp;
    p->pos = pos;
    p->u.record.typ = typ;
    p->u.record.fields = fields;

    return p;
}

exp_t SeqExp(pos_t pos, expList_t seq) {
    exp_t p = checked_malloc(sizeof(*p));
    p->kind = A_seqExp;
    p->pos = pos;
    p->u.seq = seq;

    return p;
}

exp_t AssignExp(pos_t pos, var_t var, exp_t exp) {
    exp_t p = checked_malloc(sizeof(*p));
    p->kind = A_assignExp;
    p->pos = pos;
    p->u.assign.var = var;
    p->u.assign.exp = exp;

    return p;    
}

exp_t IfExp(pos_t pos, exp_t test, exp_t then, exp_t elsee) {
    exp_t p = checked_malloc(sizeof(*p));
    p->kind = A_ifExp;
    p->pos = pos;
    p->u.iff.test = test;
    p->u.iff.then = then;
    p->u.iff.elsee = elsee;

    return p;
}

exp_t WhileExp(pos_t pos, exp_t test, exp_t body) {
    exp_t p = checked_malloc(sizeof(*p));
    p->kind = A_whileExp;
    p->pos = pos;
    p->u.whilee.test = test;
    p->u.whilee.body = body;
    
    return p;
}

exp_t ForExp(pos_t pos, symbol_t var, exp_t lo, exp_t hi, exp_t body) {
    exp_t p = checked_malloc(sizeof(*p));
    p->kind = A_forExp;
    p->pos = pos;
    p->u.forr.var = var;
    p->u.forr.lo = lo;
    p->u.forr.hi = hi;
    p->u.forr.body = body;
    p->u.forr.escape = TRUE;
    
    return p;    
}

exp_t BreakExp(pos_t pos) {
    exp_t p = checked_malloc(sizeof(*p));
    p->kind = A_breakExp;
    p->pos = pos;

    return p;
}

exp_t LetExp(pos_t pos, decList_t decs, exp_t body) {
    exp_t p = checked_malloc(sizeof(*p));
    p->kind = A_letExp;
    p->pos = pos;
    p->u.let.decs = decs;
    p->u.let.body = body;

    return p;
}

exp_t ArrayExp(pos_t pos, symbol_t typ, exp_t size, exp_t init) {
    exp_t p = checked_malloc(sizeof(*p));
    p->kind = A_arrayExp;
    p->pos = pos;
    p->u.array.typ = typ;
    p->u.array.size = size;
    p->u.array.init = init;

    return p;
}

dec_t FunctionDec(pos_t pos, fundecList_t function) {
    dec_t p = checked_malloc(sizeof(*p));
    p->kind = A_functionDec;
    p->pos = pos;
    p->u.function = function;

    return p;
}

dec_t VarDec(pos_t pos, symbol_t var, symbol_t typ, exp_t init) {
    dec_t p = checked_malloc(sizeof(*p));
    p->kind = A_varDec;
    p->pos = pos;
    p->u.var.var = var;
    p->u.var.typ = typ;
    p->u.var.init = init;
    p->u.var.escape = TRUE;

    return p;
}

dec_t TypeDec(pos_t pos, nametyList_t type) {
    dec_t p = checked_malloc(sizeof(*p));
    p->kind = A_typeDec;
    p->pos = pos;
    p->u.type = type;

    return p;
}

ty_t NameTy(pos_t pos, symbol_t name) {
    ty_t p = checked_malloc(sizeof(*p));
    p->kind = A_nameTy;
    p->pos = pos;
    p->u.var = name;

    return p;    
}

ty_t RecordTy(pos_t pos, fieldList_t record) {
    ty_t p = checked_malloc(sizeof(*p));
    p->kind = A_recordTy;
    p->pos = pos;
    p->u.record = record;
}

ty_t ArrayTy(pos_t pos, symbol_t array) {
    ty_t p = checked_malloc(sizeof(*p));
    p->kind = A_arrayTy;
    p->pos = pos;
    p->u.array = array;
    return p;
}

field_t Field(pos_t pos, symbol_t name, symbol_t typ) {
    field_t p = checked_malloc(sizeof(*p));
    p->pos = pos;
    p->name = name;
    p->typ = typ;
    p->escape = TRUE;
    return p;
}

fieldList_t FieldList(field_t head, fieldList_t tail) {
    fieldList_t p = checked_malloc(sizeof(*p));
    p->head = head;
    p->tail = tail;

    return p; 
}

expList_t ExpList(exp_t head, expList_t tail) {
    expList_t p = checked_malloc(sizeof(*p));
    p->head = head;
    p->tail = tail;

    return p;
}

fundec_t Fundec(pos_t pos, symbol_t name, fieldList_t params, symbol_t result, exp_t body) {
    fundec_t p = checked_malloc(sizeof(*p));
    p->pos = pos;
    p->name = name;
    p->params = params;
    p->result = result;
    p->body = body;

    return p;
}

fundecList_t FundecList(fundec_t head, fundecList_t tail) {
    fundecList_t p = checked_malloc(sizeof(*p));
    p->head = head;
    p->tail = tail;

    return p;
}

decList_t DecList(dec_t head, decList_t tail) {
    decList_t p = checked_malloc(sizeof(*p));
    p->head = head;
    p->tail = tail;

    return p;
}

namety_t Namety(symbol_t name, ty_t ty) {
    namety_t p = checked_malloc(sizeof(*p));
    p->name = name;
    p->ty = ty;

    return p;
}

nametyList_t NametyList(namety_t head, nametyList_t tail) {
    nametyList_t p = checked_malloc(sizeof(*p));
    p->head = head;
    p->tail = tail;

    return p;
}

efield_t Efield(symbol_t name, exp_t exp) {
    efield_t p = checked_malloc(sizeof(*p));
    p->name = name;
    p->exp = exp;

    return p;
}

efieldList_t EfieldList(efield_t head, efieldList_t tail) {
    efieldList_t p = checked_malloc(sizeof(*p));
    p->head = head;
    p->tail = tail;
    
    return p;
}
