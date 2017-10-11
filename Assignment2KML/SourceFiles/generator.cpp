#include <set>
#include <string>
#include <sstream>
#include "generator.hpp"


template <class T>
std::string numToString(T f)
{
   std::ostringstream ss;
   ss << f;
   return ss.str();
}

Generator::Generator(const NodePtr &_ast)
  : ast(_ast) { }


struct HeaderGenerator : public Generator {
    HeaderGenerator(const NodePtr &_ast) : Generator(_ast) {

    }

    virtual void generate(std::ostream &out) {
        out << "import requests" << std::endl;
        out << "APIKEY = \"ENTER YOUR OWN APIKEY\"" << std::endl;
        out << "url = \"http://maps.google.com/maps/api/staticmap?&maptype=hybrid&size=640x640\"" << std::endl;
        out << std::endl;
    }
};


struct PlacemarkerGenerator : public Generator {
    int numPlacemarks = 0;
    PlacemarkerGenerator(const NodePtr &_ast) : Generator(_ast){

    }
    virtual void generate(std::ostream &out){
        out << "# Creating the placemarks on the canvas" << std::endl;
        placemarks(ast,out);
        out << "# End of the placemarks section" << std::endl;
    }

    void placemarks(const NodePtr &ast, std::ostream &out){
        switch(ast->type()){
            case Node::DESCRIPTORS:{
                DescriptorsNodePtr descriptors = std::dynamic_pointer_cast<DescriptorsNode>(ast);
                std::string nameStr = "";
                std::string descStr = "";
                std::string xStr = "";
                std::string yStr = "";
                float xCor = 0;
                float yCor = 0;
                for(auto child : descriptors->children){
                    switch(child->type()){
                        case Node::NAME:{
                            StringLiteralNodePtr nameStringNode = std::dynamic_pointer_cast<StringLiteralNode>(child->children.at(0));
                        }
                        break;
                        case Node::DESCRIPTION:{
                            StringLiteralNodePtr descriptionStringNode = std::dynamic_pointer_cast<StringLiteralNode>(child->children.at(0));
                        }
                        break;
                        case Node::POINT:{
                            CoordinateNodePtr corNode = std::dynamic_pointer_cast<CoordinateNode>(child->children.at(0));
                            NumberLiteralNodePtr xCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(corNode->children.at(0));
                            NumberLiteralNodePtr yCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(corNode->children.at(1));
                            xCor = xCorNode->numberLiteral;
                            yCor = yCorNode->numberLiteral;
                            if(numPlacemarks == 0){
                                out << "url += " << "\"" << "&markers=color:blue%7Clabel:S%7C" << "\"" << std::endl;
                                out << "url += " << "\"" << numToString(xCor) << "," << numToString(yCor) << "\"" << std::endl; 
                            } else {
                                out << "url += " << "\"|" << numToString(xCor) << "," << numToString(yCor) << "\"" << std::endl;
                            }
                            ++numPlacemarks;
                        }
                        break;
                    }
                }
            }
            break;
            default:{
                for (auto child: ast->children) {
                    placemarks(child,out);
                }
            }
            break;
        }
    }
};

struct LineStringGenerator : public Generator {
    LineStringGenerator(const NodePtr &_ast) : Generator(_ast){
    }
    virtual void generate(std::ostream &out){
        out << "#Beginning of the line string section" << std::endl;
        linestrings(ast, out);
        out << "#End of the line string section" << std::endl;
    }
    void linestrings(const NodePtr &ast, std::ostream &out){
        switch(ast->type()){
            case Node::LINESTRING:{
                LineStringNodePtr lineString = std::dynamic_pointer_cast<LineStringNode>(ast);
                CoordinateListNodePtr coordinateList = std::dynamic_pointer_cast<CoordinateListNode>(lineString->children.at(0));
                int count = 0;
                for(auto child : coordinateList->children){
                    //get the current coordinate's x and y val
                    NumberLiteralNodePtr xCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(0));
                    NumberLiteralNodePtr yCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(1));
                    if(count == 0){
                        out << "url += \"&path=color:0xff0000ff|weight:2\"" << std::endl;
                        out << "url += \"" << "|" << numToString(xCorNode->numberLiteral) << "," << numToString(yCorNode->numberLiteral) << "\"" << std::endl;
                    } else {
                        out << "url += \"" << "|" << numToString(xCorNode->numberLiteral) << "," << numToString(yCorNode->numberLiteral) << "\"" << std::endl;
                    }
                    ++count;
                }
            }
            break;
            default:{
                for(auto child : ast->children){
                    linestrings(child,out);
                }
            }
            break;
        }
    }
};

struct FooterGenerator : public Generator {
    FooterGenerator(const NodePtr &_ast) : Generator(_ast) {

    }
    virtual void generate(std::ostream &out) {
        out << "url += \"&key=\"" << std::endl;
        out << "url += APIKEY" << std::endl;
        out << "session = requests.Session()" << std::endl;
        out << "r = session.get(url)" << std::endl;
        out << "f = open('testMap.png','wb')" << std::endl;
        out << "f.write(r.content)" << std::endl;
        out << "f.close()" << std::endl;
    }
};


struct ProgramGenerator : Generator {
  HeaderGenerator header;
  PlacemarkerGenerator placemarkers;
  LineStringGenerator linestrings;
  FooterGenerator footer;  

  ProgramGenerator(const NodePtr &_ast)
    : Generator(_ast), header(_ast), placemarkers(_ast), linestrings(_ast), footer(_ast) {}

  virtual void generate(std::ostream &out) {
    header.generate(out);
    placemarkers.generate(out);
    linestrings.generate(out);
    footer.generate(out);    
  }
};

GeneratorPtr generator(const NodePtr &ast) {
  return GeneratorPtr(new ProgramGenerator(ast));
}