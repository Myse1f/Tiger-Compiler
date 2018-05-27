#ifndef _ERRORMSG_H_
#define _ERRORMSG_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "utils.h"

extern bool EM_anyErrors;

void EM_newline(void);

extern int EM_tokPos;

void EM_error(int, string_t,...);
//void EM_impossible(string,...);
void EM_reset(string_t filename);

#endif