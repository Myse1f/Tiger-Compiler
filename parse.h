#ifndef _PARSE_H_
#define _PARSE_H_

#include <stdio.h>
#include "utils.h"
#include "symbol.h"
#include "ast.h"
#include "errormsg.h"

/* function prototype from parse.c */
exp_t parse(string_t fname);

#endif