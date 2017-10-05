# Compiler-Structure
Repo for CSCI 450 - Compiler Structure, Colorado Mesa University Fall 2017

# Assignment 2:


TOKENS:                         Regular Expressions:
open_kml_tag                    <kml>
close_kml_tag                   <\/kml>
open_document_tag               <document>
close_document_tag              <\/document>
open_placemark_tag              <placemark>
close_placemark_tag             <\/placemark>
open_name_tag                   <name>
close_name_tag                  <\/name>
open_description_tag            <description>
close_description_tag           <\/description>
open_point_tag                  <point>
close_point_tag                 <\/point>
open_coordinate_tag             <coordinate>
close_coordinate_tag            <\/coordinate>
comma_delimeter                 \,                       
number_literal                  [+|-]?[0-9]+\.?[(0-9)+]*
string_literal                  \"(\\.|[^"])*\"



A BNF Representation of a simple KML file that allows you to only add <Placemark> tags and basic descriptions of those tags

<KML> ::= <OPEN_KML_TAG> <DOCUMENT> <CLOSE_KML_TAG>
<DOCUMENT> ::= <OPEN_DOCUMENT_TAG> <MAJOR_TAG> <CLOSE_DOCUMENT_TAG>
<MAJOR_TAG> ::= <OPEN_PLACEMARK_TAG> <DESCRIPTOR> <CLOSE_PLACEMARK_TAG> <MAJOR_TAG> | empty
<DESCRIPTOR> ::= <MINOR_TAG> <DESCRIPTOR> | empty
<MINOR_TAG> ::= <NAME_TAG> | <DESCRIPTION_TAG> | <POINT_TAG>
<NAME_TAG> ::= <OPEN_NAME_TAG> <STRING_LITERAL> <CLOSE_NAME_TAG>
<DESCRIPTION_TAG> ::= <OPEN_DESCRIPTION_TAG> <STRING_LITERAL> <CLOSE_DESCRIPTION_TAG>
<POINT_TAG> ::= <OPEN_POINT_TAG> <COORDINATE_TAG> <CLOSE_POINT_TAG>
<COORDINATE_TAG> ::= <OPEN_COORDINATE_TAG> <NUMBER_LITERAL> <COMMA_DELIMITER> <NUMBER_LITERAL> <COMMA_DELIMITER> <NUMBER_LITERAL> <CLOSE_COORDINATE_TAG>
<STRING_LITERAL> ::= a string
<NUMBER_LITERAL> ::= a number


This regular expression technically would identify a valid program 
<kml>(<document>((<placemark>(<name>[a-z| | A-Z]+<\/name>)?(<description>([a-z| | A-Z]+)<\/description>)?(<point>(<coordinate>(([+|-]?[0-9]+\.?[(0-9)+]*)\,([+|-]?[0-9]+\.?[(0-9)+]*)\,([+|-]?[0-9]+\.?[(0-9)+]*))<\/coordinate>)<\/point>)?<\/placemark>)+)?<\/document>)?<\/kml>


Sample file using this grammar:

<kml>
    <document>
        <placemark>
            <name>Mee Canyon</name>
            <description>A Canyon in the Southwest</description>
            <point>
                <coordinate>48.05,-15.17,0</coordinate>
            </point>
        </placemark>
    </document>
</kml>


Example Tree Structure (AST):

                                 --------------KML--------------
                                 |              |              |
                           OPEN_KML_TAG         |       CLOSE_KML_TAG   
                                 |              |              |
                              "<kml>"           |           "</kml>"
                                                |
                                    --------DOCUMENT--------
                                    |           |          |
                            OPEN_DOCUMENT_TAG   | CLOSE_DOCUMENT_TAG       
                                    |           |          |
                               "<document>"     |    "</document>"
                                                |
                               -------------MAJOR_TAG-------------
                               |                |                |
                       OPEN_PLACEMARK_TAG       |       CLOSE_PLACEMARK_TAG
                               |                |                |
                          "<placemark>"         |          "</placemark>"           
                                                |
                ----------------------------DESCRIPTOR--------------------------
                |                               |                              |
            MINOR_TAG                       MINOR_TAG                      MINOR_TAG
                |                               |                              |
      -------NAME_TAG------            --DESCRIPTION_TAG--               --POINT_TAG--
      |         |         |            |        |        |               |     |     |
OPEN_NAME_TAG   |  CLOSE_NAME_TAG      |        |        |      OPEN_POINT_TAG | OPEN_POINT_TAG
      |         |         |            |        |        |               |     |     |    
  "<name>"      |     "</name>"        |        |        |           "<point>" | "</point>"
                |                      |        |        |                     |
                |          OPEN_DESCRIPTION_TAG | CLOSE_DESCRIPTION_TAG        |
          STRING_LITERAL               |        |        |                     |
                |               "<description>" | "</description>"             |                    
                |                               |                              |
           "Mee Canyon"                   STRING_LITERAL           ------COORDINATE_TAG-------
                                                |                  |           |             |
                                   "A Canyon in the Southwest"     |           |             |
                                                                   |           |             |
                                                            "<coordinate>"     |      "</coordinate>"
                                                                               |
                                            --------------------------------------------------------------------------
                                            |                |                 |                 |                   |
                                      NUMBER_LITERAL  COMMA_DELIMITER    NUMBER_LITERAL    COMMA_DELIMITER     NUMBER_LITERAL
                                            |                |                 |                 |                   |
                                          48.05              ,              -15.17               ,                   0     
