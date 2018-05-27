#include <stdio.h>
#include "utils.h"
#include "past.h"
#include "errormsg.h"

extern int yyparse(void);
extern exp_t ast_root;

void parse(string_t fname) 
{
    EM_reset(fname);
    if (yyparse() == 0) /* parsing worked */
        fprintf(stderr,"Parsing successful!\n");
    else 
        fprintf(stderr,"Parsing failed\n");
}


int main(int argc, char **argv) {
    if (argc!=2) {
        fprintf(stderr,"usage: a.out filename\n"); 
        exit(1);
    }
    parse(argv[1]);

    // print ast
    FILE *out = fopen(argv[2], "w");
    pr_exp(out, ast_root, 0);
    return 0;
}
