#ifndef _SYMBOL_H_
#define _SYMBOL_H_

#include "utils.h"
#include "table.h"

typedef struct symbol_s *symbol_t;
struct symbol_s {
    string_t name; 
    symbol_t next;
};

symbol_t Symbol(string_t name);
string_t SymbolName(symbol_t sym);
table_t  SymbolEmpty(void);
void     SymbolEnter(table_t tab, symbol_t sym, void *value);
void    *SymbolLookup(table_t tab, symbol_t sym);
void     SymbolBeginScope(table_t tab);
void     SymbolEndScope(table_t tab);

#endif