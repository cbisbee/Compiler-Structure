%{
#include <stdio.h>
#include "example1.h"

#define NUMBER  1000
#define KEYWORD 1001
#define EOL     '\n'

%}

%%
[0-9]+                  { yylval.ivalue = atoi(yytext); printf("NUMBER\n"); }
(start|stop)            { printf("KEYWORD\n"); }
[a-zA-Z][a-zA-Z0-9]*    { printf("WORD\n"); } 
\r?\n                   { printf("EOL\n"); }
. { printf("syntax error: %s\n", yytext); }

%%