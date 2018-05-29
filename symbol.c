#include "symbol.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

#define SIZE 109
static symbol_t hashtable[SIZE];

static symbol_t MakeSymbol(string_t name, symbol_t next)
{
    symbol_t sym = checked_malloc(sizeof(*sym));
    sym->name = name;
    sym->next = next;

    return sym;
}

static unsigned int hash(string_t str)
{
    unsigned int h = 0;
    char *p;

    for (p = str; *p; p++)
        h = 65599 * h + *p;

    return h;
}

symbol_t Symbol(string_t name) {
    int index = hash(name) % SIZE;
    symbol_t p;

    for (p = hashtable[index]; p; p = p->next)
        if (strcmp(p->name, name) == 0)
            return p;
    p = MakeSymbol(name, hashtable[index]);
    hashtable[index] = p;

    return p;
}

string_t SymbolName(symbol_t sym) {
    return sym->name;
}

table_t SymbolEmpty(void) {
    return TableEmpty();
}

void SymbolEnter(table_t tab, symbol_t sym, void *value) {
    TableEnter(tab, sym, value);
}

void *SymbolLookup(table_t tab, symbol_t sym) {
    return TableLookup(tab, sym);
}

static struct symbol_s marksym = {"<mark>", NULL};

void SymbolBeginScope(table_t tab) {
    SymbolEnter(tab, &marksym, NULL);
}

void SymbolEndScope(table_t tab) {
    symbol_t sym;
    do
        sym = TablePop(tab);
    while (sym != &marksym);
}