%{
#include <string.h>
#include "util.h"
#include "tokens.h"
#include "errormsg.h"

int charPos = 1;

int yywrap(void)
{
	charPos=1;
	return 1;
}

#define ADJ \
{ \
    EM_tokPos = charPos; \
    charPos += yyleng; \
}

static int cmtLevel = 0;

#define INIT_LEN 32
static string sBuf;
static int sLen;
static int sCap;
static void init_buf(void);
static void append_char(char ch);

%}

blank       [" "\t]
number		[0-9]+
id			[_a-zA-Z][_a-zA-Z0-9]*

%x COMMENT STRING

%%

{blank}		{ ADJ; continue; }
\n			{ ADJ; EM_newline(); continue; }

","			{ ADJ; return COMMA; }
":"			{ ADJ; return COLON; }
";"			{ ADJ; return SEMICOLON; }
"("			{ ADJ; return LPARAN; }
")"			{ ADJ; return RPARAN; }
"["			{ ADJ; return LBRACK; }
"]"			{ ADJ; return RBRACK; }
"{"			{ ADJ; return LBRACE; }
"}"			{ ADJ; return RBRACE; }
"."			{ ADJ; return DOT; }
"+"			{ ADJ; return PLUS; }
"-"			{ ADJ; return MINUS; }
"*"			{ ADJ; return TIMES; }
"/"			{ ADJ; return DIVIDE; }
"="			{ ADJ; return EQ; }
"<>"		{ ADJ; return NEQ; }
"<"			{ ADJ; return LT; }
"<="		{ ADJ; return LE; }
">"			{ ADJ; return GT; }
">="		{ ADJ; return GE; }
"&"			{ ADJ; return AND; }
"|"			{ ADJ; return OR; }
":="		{ ADJ; return ASSIGN; }

array		{ ADJ; return ARRAY; }
if			{ ADJ; return IF; }
then		{ ADJ; return THEN; }
else		{ ADJ; return ELSE; }
while		{ ADJ; return WHILE; }
for			{ ADJ; return FOR; }
to			{ ADJ; return TO; }
do			{ ADJ; return DO; }
let			{ ADJ; return LET; }
in			{ ADJ; return IN; }
end			{ ADJ; return END; }
of			{ ADJ; return OF; }
break		{ ADJ; return BREAK; }
nil			{ ADJ; return NIL; }
function	{ ADJ; return FUNCTION; }
var			{ ADJ; return VAR; }
type		{ ADJ; return TYPE; }

{number}	{ ADJ; yylval.ival = atoi(yytext); return INT; }
{id}		{ ADJ; yylval.sval = String(yytext); return ID; }

"/*"		{ ADJ; cmtLevel = 1; BEGIN(COMMENT); continue; }
<COMMENT>[^*/\n]	{ ADJ; continue; }
<COMMENT>\n			{ ADJ; EM_newline(); continue; }
<COMMENT>"/*"		{ ADJ; cmtLevel++; continue; }
<COMMENT>"*/"		
{
	ADJ;
	cmtLevel--;
	if (!cmtLevel) BEGIN(INITIAL);
}

\"			{ ADJ; init_buf(); BEGIN(STRING); continue; }
<STRING>\\\\	{ ADJ; append_char('\\'); continue; }
<STRING>\\\"	{ ADJ; append_char('\"'); continue; }
<STRING>\\n		{ ADJ; append_char('\n'); continue; }
<STRING>\\t		{ ADJ; append_char('\t'); continue; }
<STRING>\\r		{ ADJ; append_char('\r'); continue; }
<STRING>\\\n	{ ADJ; EM_newline(); continue; }
<STRING>\\[0-3][0-8]{2}
{
	// ASCII code in Octal
	char c = 0;
	ADJ;
	c = yytext[1]<<6 + yytext[2]<<3 +yytext[3];
	append_char(c);
	continue;
}
<STRING>\"
{
	// End of string
    ADJ;
    BEGIN(INITIAL);
    yylval.str = sBuf;
    return STRING;
}
<STRING>[^\\]+
{
	// Normal string content
	char *p = yytext;
	ADJ;
	while (*p) append_char(*p++);
	continue;
}
.	{ ADJ; EM_error(EM_tokPos, "Illegal character"); }

%%

static void init_buf(void)
{
    sBuf = checked_malloc(INIT_LEN);
    sBuf[0] = 0;
    sLen = 0;
    sCap = INIT_LEN;
}

static void append_char(char ch)
{
    if (++sLen == sCap)
    {
        char *p;

        sCap *= 2;
        p = checked_malloc(sCap);
        memcpy(p, sBuf, sLen);
        free(sBuf);
        sBuf = p;
    }
    sBuf[sLen - 1] = ch;
    sBuf[sLen] = 0;
}