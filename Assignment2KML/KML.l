%option noyywrap

%{
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "KML.h"
//#include "example1.tab.hpp"
#define DEBUG_LEXER 1
%}

%%
(<kml>|<\/kml>)   {
  if(strcmp(yytext,"<kml>") == 0){
    return OPEN_KML_TAG;
  }
  if(strcmp(yytext,"</kml>") == 0){
    return CLOSE_KML_TAG;
  }
  //assert(0);
}

(<document<|<\/document>)   {
  if(strcmp(yytext,"<document>") == 0){
    return OPEN_DOCUMENT_TAG;
  }
  if(strcmp(yytext,"</document>") == 0){
    return CLOSE_DOCUMENT_TAG;
  }
}

(<placemark>|<\/placemark>)   {
  if(strcmp(yytext,"<placemark>") == 0){
    return OPEN_PLACEMARK_TAG;
  }
  if(strcmp(yytext,"</placemark>") == 0){
    return CLOSE_PLACEMARK_TAG;
  }
}

(<name<|<\/name>)   {
  if(strcmp(yytext,"<name>") == 0){
    return OPEN_NAME_TAG;
  }
  if(strcmp(yytext,"</name>") == 0){
    return CLOSE_NAME_TAG;
  }
}

(<description>|<\/description>)   {
  if(strcmp(yytext,"<description>") == 0){
    return OPEN_DESCIPTION_TAG;
  }
  if(strcmp(yytext, "</description>") == 0){
    return CLOSE_DESCRIPTION_TAG;
  }
}

(<point>|<\/point>)   {
  if(strcmp(yytext, "<point>") == 0){
    return OPEN_POINT_TAG;
  }
  if(strcmp(yytext, "</point>") == 0){
    return CLOSE_POINT_TAG;
  }
}

(<coordinate>|<\/coordinate>)   {
  if(strcmp(yytext, "<coordinate>") == 0){
    return OPEN_COORDINATE_TAG;
  }
  if(strcmp(yytext, "</coordinate>") == 0){
    return CLOSE_COORDINATE_TAG;
  }
}

\,   {
  return COMMA_DELIMETER;
}

[+|-]?[0-9]+\.?([0-9]+)?    {
    #if DEBUG_LEXER == 1
        printf("lexed '%s' as NUMBER_LITERAL(token=%d,value=%d)\n",yytext, NUMBER_LITERAL,
        number(yylval));
    #endif
    return NUMBER_LITERAL;
}

(\"(\\.|[^"])*\")   {
  return STRING_LITERAL;
}

.   {
  printf("syntax error");
}
%%


