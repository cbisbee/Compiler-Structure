%option noyywrap

%{
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "example1.h"
#include "example1.tab.hpp"
#define DEBUG_LEXER 1
%}

%%
[0-9]+                  { 
    yylval.ivalue = atoi(yytext); 
#if DEBUG_LEXER == 1
    printf("lexed '%s' as NUMBER(token=%d, value=%d)\n",yytext,NUMBER,yylval.ivalue);
#endif
    return NUMBER; 
}
(start|stop)            { 
    if(strcmp(yytext,"start") == 0) {
#if DEBUG_LEXER == 1
    printf("lexed '%s' as START(token=%d)\n",yytext,START);
#endif
      return START;
    }
    if(strcmp(yytext,"stop") == 0) {
#if DEBUG_LEXER == 1
    printf("lexed '%s' as STOP(token=%d)\n",yytext,STOP);
#endif
      return STOP;
    }
    assert(0); 
}
[a-zA-Z][a-zA-Z0-9]*    {
    yylval.svalue = strdup(yytext); 
#if DEBUG_LEXER == 1
    printf("lexed '%s' as WORD(token=%d, value='%s')\n",yytext,WORD,yylval.svalue);
#endif
    return WORD; 
}
[ \t]+                  {
#if DEBUG_LEXER == 1
    printf("lexed '%s' as WS(token=%d)\n",yytext,WS);
#endif
    return WS;
} 
\r?\n                   { 
#if DEBUG_LEXER == 1
    printf("lexed '%s' as EOL(token=%d)\n",yytext,EOL);
#endif
    return EOL; 
}
.                       { 
    printf("syntax error: '%s'\n", yytext); 
    assert(0);
}
%% 