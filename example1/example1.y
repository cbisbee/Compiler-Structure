%{
#include <stdio.h>
#include <string.h>
#include "example1.h"

  void yyerror(const char *str);
  int yylex();
%}

%token NUMBER START STOP WORD EOL WS

%%

commands :
     /* empty */
     |
     commands command
     ;

command :
     start_command
     |
     stop_command
     ;

start_command :
     optional_ws START WS WORD WS NUMBER optional_ws EOL
     ;

stop_command :
     optional_ws STOP WS WORD optional_ws EOL
     ;

optional_ws :
     /* empty */
     |
     WS
     ;

%%

void yyerror(const char *str)
{
  fprintf(stderr,"error: %s\n",str);
}

int yywrap(void) {
    return 1;
}

int main(int argc, char *argv[])
{
  yyparse();
  return 0;
} 
