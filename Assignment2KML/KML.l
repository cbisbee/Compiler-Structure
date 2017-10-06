%option noyywrap

%{
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <string.h>
#include "KML.h"
#include "kml.tab.hpp"

#define DEBUG_LEXER 1
%}

%%
(<kml>|<\/kml>)   {
  std::cout << "WTF" << std::endl;
  std::cout << "yytext = " << yytext << std::endl;
  if(strcmp(yytext,"<kml>") == 0){
    std::cout << "Made it past the strcmp()" << std::endl;
    #if DEBUG_LEXER == 1
      std::cout << "Made it into the DEBUG_LEXER" << std::endl;
      std::cout << "lexed " << yytext << " as OPEN_KML" << std::endl;
    #endif
    std::cout << "Made it past the if DEBUG_LEXER == 1" << std::endl;
    return OPEN_KML;
  }
  if(strcmp(yytext,"</kml>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as CLOSE_KML" << std::endl;
    #endif
    return CLOSE_KML;
  }
  //assert(0);
}

(<document<|<\/document>)   {
  if(strcmp(yytext,"<document>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as OPEN_DOCUMENT" <<  std::endl;
    #endif
    return OPEN_DOCUMENT;
  }
  if(strcmp(yytext,"</document>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as CLOSE_DOCUMENT" << std::endl;
    #endif
    return CLOSE_DOCUMENT;
  }
}

(<placemark>|<\/placemark>)   {
  if(strcmp(yytext,"<placemark>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as OPEN_PLACEMARK" <<  std::endl;
    #endif
    return OPEN_PLACEMARK;
  }
  if(strcmp(yytext,"</placemark>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as CLOSE_PLACEMARK" << std::endl;
    #endif
    return CLOSE_PLACEMARK;
  }
}

(<name<|<\/name>)   {
  if(strcmp(yytext,"<name>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as OPEN_NAME" << std::endl;
    #endif
    return OPEN_NAME;
  }
  if(strcmp(yytext,"</name>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as CLOSE_KML" << std::endl;
    #endif
    return CLOSE_NAME;
  }
}

(<description>|<\/description>)   {
  if(strcmp(yytext,"<description>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as OPEN_DESCRIPTION" << std::endl;
    #endif
    return OPEN_DESCRIPTION;
  }
  if(strcmp(yytext, "</description>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as CLOSE_DESCRIPTION" << std::endl;
    #endif
    return CLOSE_DESCRIPTION;
  }
}

(<point>|<\/point>)   {
  if(strcmp(yytext, "<point>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as OPEN_POINT" << std::endl;
    #endif
    return OPEN_POINT;
  }
  if(strcmp(yytext, "</point>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as CLOSE_POINT" << std::endl;
    #endif
    return CLOSE_POINT;
  }
}

(<coordinate>|<\/coordinate>)   {
  if(strcmp(yytext, "<coordinate>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as OPEN_COORDINATE" << std::endl;
    #endif
    return OPEN_COORDINATE;
  }
  if(strcmp(yytext, "</coordinate>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as CLOSE_COORDINATE" << std::endl;
    #endif
    return CLOSE_COORDINATE;
  }
}

\,   {
  #if DEBUG_LEXER == 1
    std::cout << "lexed " << yytext << " as COMMA_DELIMETER" << std::endl;
  #endif
  return COMMA_DELIMETER;
}

[+|-]?[0-9]+\.?([0-9]+)?    {
  yylval = node(atoi(yytext));
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as NUMBER_LITERAL: " << yylval << std::endl;
    #endif
    return NUMBER_LITERAL;
}

(\"(\\.|[^"])*\")   {
  yylval = node(std::string(yytext));
  #if DEBUG_LEXER == 1
    std::cout << "lexed " << yytext << " as STRING_LITERAL: " << yylval << std::endl;
  #endif
  return STRING_LITERAL;
}

[ \t]+ {
  #if DEBUG_LEXER == 1
    std::cout << "lexed " << yytext << " as WS" << std::endl;
  #endif
  return WS;
}

\r?\n {
  #if DEBUG_LEXER == 1
    std::cout << "lexed " << yytext << " as EOL" << std::endl;
  #endif
  return EOL;
}

.   {
  std::cout << "syntax error '" << yytext << "'" << std::endl;
  assert(0);
}
%%