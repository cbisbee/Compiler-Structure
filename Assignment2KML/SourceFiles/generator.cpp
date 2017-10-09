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
        out << "import turtle" << std::endl << std::endl;
        out << "pen = turtle.Turtle()" << std::endl;
        out << "pen.hideturtle()" << std::endl;
        out << "pen.penup()" << std::endl;
        out << std::endl;
    }
};


struct PlacemarkerGenerator : public Generator {
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
                            nameStr = "pen.write(" + nameStringNode->stringLiteral + ",align=\"left\", font=(\"Arial\",13,\"normal\"))";
                        }
                        break;
                        case Node::DESCRIPTION:{
                            StringLiteralNodePtr descriptionStringNode = std::dynamic_pointer_cast<StringLiteralNode>(child->children.at(0));
                            descStr = "pen.write(" + descriptionStringNode->stringLiteral + ",align=\"left\", font=(\"Arial\",10,\"normal\"))";
                        }
                        break;
                        case Node::POINT:{
                            CoordinateNodePtr corNode = std::dynamic_pointer_cast<CoordinateNode>(child->children.at(0));
                            NumberLiteralNodePtr xCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(corNode->children.at(0));
                            NumberLiteralNodePtr yCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(corNode->children.at(1));
                            xCor = xCorNode->numberLiteral;
                            yCor = yCorNode->numberLiteral;
                            xStr = "pen.setx(" + numToString(xCor) + ")";
                            yStr = "pen.sety(" + numToString(yCor) + ")";
                        }
                        break;
                    }
                }
                out << xStr << std::endl;
                out << yStr << std::endl;
                out << nameStr << std::endl;
                out << "pen.sety(" + numToString(yCor-10) + ")" << std::endl;
                out << "pen.setx(" + numToString(xCor+10) + ")" << std::endl;
                out << descStr << std::endl;
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

struct FooterGenerator : public Generator {
    FooterGenerator(const NodePtr &_ast) : Generator(_ast) {

    }
    virtual void generate(std::ostream &out) {
        out << std::endl;
        out << "turtle.done()" << std::endl;
    }
};


struct ProgramGenerator : Generator {
  HeaderGenerator header;
  PlacemarkerGenerator placemarker;
  FooterGenerator footer;  

  ProgramGenerator(const NodePtr &_ast)
    : Generator(_ast), header(_ast), placemarker(_ast), footer(_ast) {}

  virtual void generate(std::ostream &out) {
    header.generate(out);
    placemarker.generate(out);
    footer.generate(out);    
  }
};

GeneratorPtr generator(const NodePtr &ast) {
  return GeneratorPtr(new ProgramGenerator(ast));
}