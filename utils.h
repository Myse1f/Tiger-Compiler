#ifndef _UTILS_H_
#define _UTILS_H_

#include <assert.h>

typedef char* string_t;
string_t string(const char *str);

typedef char bool;

#define TRUE 1
#define FALSE 0

void* checked_malloc(int);

typedef struct list_s *list_t;

struct list_s {
    union {
        int i;
        bool b;
        void *data;
    };
    struct list_s *next;
};

list_t list(void* data, list_t next);

#endif