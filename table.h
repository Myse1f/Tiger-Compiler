#ifndef _TABLE_H_
#define _TABLE_H_

typedef struct table_s *table_t;

table_t TableEmpty(void);
void    TableEnter(table_t tab, void *key, void *value);
void    *TableLookup(table_t tab, void *key);
void    *TablePop(table_t tab);

typedef void (*tab_dump_func_t)(void *key, void *value);
void    TableDump(table_t tab, tab_dump_func_t show);

#endif