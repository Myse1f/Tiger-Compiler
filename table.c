#include "table.h"
#include "utils.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define TABSIZE 127

typedef struct binder_s *binder_t;
struct binder_s
{
    void *key;
    void *value;
    binder_t next;
    void *prevtop;
};

struct table_s
{
    binder_t table[TABSIZE];
    void* top;
};

static binder_t Binder(void *key, void *value, binder_t next, void* prevtop)
{
    binder_t b = checked_malloc(sizeof(*b));
    b->key = key;
    b->value = value;
    b->next = next;
    b->prevtop = prevtop;
    return b;
}

table_t TableEmpty(void) {
    table_t t = checked_malloc(sizeof(*t));
    int i;
    t->top = NULL;
    for(i=0; i<TABSIZE; i++)
        t->table[i] = NULL;
    
    return t;
}

void TableEnter(table_t tab, void *key, void *value) {
    int index;
    assert(tab && key);
    index = ((unsigned)key) % TABSIZE;
    tab->table[index] = Binder(key, value, tab->table[index], tab->top);
    tab->top = key;
}

void *TableLookup(table_t tab, void *key) {
    int index;
    binder_t b;
    assert(tab && key);
    index = ((unsigned)key) % TABSIZE;
    for(b=tab->table[index]; b; b=b->next) 
        if(b->key == key) 
            return b->value;
    
    return NULL;
}

void *TablePop(table_t tab) {
    void* k;
    binder_t b;
    int index;
    k = tab->top;
    assert(k);
    index = ((unsigned)k) % TABSIZE;
    b = tab->table[index];
    assert(b);
    tab->table[index] = b->next;
    tab->top = b->prevtop;

    return b->key;
}

void TableDump(table_t tab, tab_dump_func_t show) {
    void *k = tab->top;
    int index = ((unsigned)k) % TABSIZE;
    binder_t b = tab->table[index];
    if(b == NULL) 
        return;
    tab->table[index] = b->next;
    tab->top = b->prevtop;
    show(b->key, b->value);
    TableDump(tab, show);
    assert(tab->top == b->prevtop && tab->table[index] == b->next);
    tab->top = k;
    tab->table[index] = b;
}
