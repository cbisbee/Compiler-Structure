#pragma once

typedef union {
  int ivalue;
  char *svalue;
} yystype_t;

#define YYSTYPE yystype_t

extern YYSTYPE yylval;

