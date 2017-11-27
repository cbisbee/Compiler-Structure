%option noyywrap

%{
#include <stdio.h>
#include <iostream>
#include <assert.h>
#include <string.h>
#include "KML.hpp"
#include "kml.tab.hpp"

#define DEBUG_LEXER 0
%}

%%
("<?xml version=\"1.0\" encoding=\"UTF-8\"?>")  {
  #if DEBUG_LEXER == 1
    std::cout << "lexed " << yytext << " as XML_VERSION" << std::endl;
  #endif
  return XML_VERSION;
}

(<Kml>|<\/Kml>)   {
  if(strcmp(yytext,"<Kml>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as OPEN_KML" << std::endl;
    #endif
    return OPEN_KML;
  }
  if(strcmp(yytext,"</Kml>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as CLOSE_KML" << std::endl;
    #endif
    return CLOSE_KML;
  }
  //assert(0);
}

(<Document>|<\/Document>)   {
  if(strcmp(yytext,"<Document>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as OPEN_DOCUMENT" <<  std::endl;
    #endif
    return OPEN_DOCUMENT;
  }
  if(strcmp(yytext,"</Document>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as CLOSE_DOCUMENT" << std::endl;
    #endif
    return CLOSE_DOCUMENT;
  }
}

(<LineString>|<\/LineString>) {
  if(strcmp(yytext,"<LineString>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as OPEN_LINESTRING" << std::endl;
    #endif
    return OPEN_LINESTRING;
  }
  if(strcmp(yytext,"</LineString>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as CLOSE_LINESTRING" << std::endl;
    #endif
    return CLOSE_LINESTRING;
  }
}

(<Placemarker>|<\/Placemarker>)   {
  if(strcmp(yytext,"<Placemarker>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as OPEN_PLACEMARK" <<  std::endl;
    #endif
    return OPEN_PLACEMARK;
  }
  if(strcmp(yytext,"</Placemarker>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as CLOSE_PLACEMARK" << std::endl;
    #endif
    return CLOSE_PLACEMARK;
  }
}

(<name>|<\/name>)   {
  if(strcmp(yytext,"<name>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as OPEN_NAME" << std::endl;
    #endif
    return OPEN_NAME;
  }
  if(strcmp(yytext,"</name>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as CLOSE_NAME" << std::endl;
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

(<Point>|<\/Point>)   {
  if(strcmp(yytext, "<Point>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as OPEN_POINT" << std::endl;
    #endif
    return OPEN_POINT;
  }
  if(strcmp(yytext, "</Point>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as CLOSE_POINT" << std::endl;
    #endif
    return CLOSE_POINT;
  }
}

(<coordinates>|<\/coordinates>)   {
  if(strcmp(yytext, "<coordinates>") == 0){
    #if DEBUG_LEXER == 1
      std::cout << "lexed " << yytext << " as OPEN_COORDINATE" << std::endl;
    #endif
    return OPEN_COORDINATE;
  }
  if(strcmp(yytext, "</coordinates>") == 0){
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
  yylval = node(atof(yytext));
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