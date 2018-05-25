#include "parse.h"

extern int yyparse(void);
extern exp_t ast_root;

/* parse source file fname; 
   return abstract syntax data structure */
exp_t parse(string_t fname) {
  	EM_reset(fname);
 	if (yyparse() == 0) /* parsing worked */
   		return ast_root;
 	else 
		return NULL;
}
