#include "PolygonGenerator.hpp"

PolygonGenerator::PolygonGenerator(const NodePtr &_baseAst) : Generator(_baseAst) { }
PolygonGenerator::PolygonGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode)
        : Generator(_baseAst, _overlayAst, _mode) { }

void PolygonGenerator::generate(std::ostream &out){
    switch(mode){
        case MAP:{
            out << "#Start of polygon section" << std::endl;
            polygonsBase(baseAst,out);
            out << "#End of polygon section" << std::endl;
        }
        break;
        case SUBSET:{
            //Need to figure this out
            polygonsOverlay(baseAst,out);
        }
        break;
        case SETADD:{
            polygonsOverlay(baseAst,out);
            polygonsOverlay(overlayAst,out);
        }
        break;
        default:{
            //Do nothing for now
        }
        break;
    }
}

void PolygonGenerator::polygonsBase(const NodePtr &ast, std::ostream &out){
    switch(ast->type()){
        case Node::POLYGON:{
            out << "url += \"&path=color%3ared|weight:1|fillcolor%3aorange\"" << std::endl;

            PolygonNodePtr polyNode = std::dynamic_pointer_cast<PolygonNode>(ast);                
            OuterBoundaryNodePtr outerBoundary = std::dynamic_pointer_cast<OuterBoundaryNode>(polyNode->children.at(0));                
            LinearRingNodePtr linearRing = std::dynamic_pointer_cast<LinearRingNode>(outerBoundary->children.at(0));                
            CoordinateListNodePtr coordinateList = std::dynamic_pointer_cast<CoordinateListNode>(linearRing->children.at(0));           
            for(auto child : coordinateList->children){
                //get the current coordinate's x, y, and z values
                NumberLiteralNodePtr xCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(0));
                NumberLiteralNodePtr yCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(1));
                NumberLiteralNodePtr zCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(2));
                
                out << "url += \"|" << yCorNode->numberLiteral << "," << xCorNode->numberLiteral << "\"" << std::endl;
            }
        }
        break;
        default:{
            for(auto child : ast->children){
                polygonsBase(child,out);
            }
        }
        break;
    }
}

void PolygonGenerator::polygonsOverlay(const NodePtr &ast, std::ostream &out){
    switch(ast->type()){
        case Node::POLYGON:{
            out << "\t\t<Polygon>" << std::endl;
            out << "\t\t\t<outerBoundaryIs>" << std::endl;
            out << "\t\t\t\t<LinearRing>" << std::endl;
            out << "\t\t\t\t\t<coordinates>" << std::endl;

            PolygonNodePtr polyNode = std::dynamic_pointer_cast<PolygonNode>(ast);                
            OuterBoundaryNodePtr outerBoundary = std::dynamic_pointer_cast<OuterBoundaryNode>(polyNode->children.at(0));                
            LinearRingNodePtr linearRing = std::dynamic_pointer_cast<LinearRingNode>(outerBoundary->children.at(0));                
            CoordinateListNodePtr coordinateList = std::dynamic_pointer_cast<CoordinateListNode>(linearRing->children.at(0));           
            for(auto child : coordinateList->children){
                //get the current coordinate's x, y, and z values
                NumberLiteralNodePtr xCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(0));
                NumberLiteralNodePtr yCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(1));
                NumberLiteralNodePtr zCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(2));

                if(mode == SUBSET){
                    OrderedTriplet curPoint(xCorNode->numberLiteral, yCorNode->numberLiteral, zCorNode->numberLiteral);
                    if(pointInPolygon(curPoint)){
                        out << std::fixed << std::showpoint << std::setprecision(15);
                        out << "\t\t\t\t\t\t" << xCorNode->numberLiteral << ", " << yCorNode->numberLiteral << ", " << zCorNode->numberLiteral << std::endl;
                    }
                }
                else if(mode == SETADD){
                    out << std::fixed << std::showpoint << std::setprecision(15);
                    out << "\t\t\t\t\t\t" << xCorNode->numberLiteral << ", " << yCorNode->numberLiteral << ", " << zCorNode->numberLiteral << std::endl;
                }
            }
            out << "\t\t\t\t\t</coordinates>" << std::endl;
            out << "\t\t\t\t</LinearRing>" << std::endl;
            out << "\t\t\t</outerBoundaryIs>" << std::endl;
            out << "\t\t</Polygon>" << std::endl;
        }
        break;
        default:{
            for(auto child : ast->children){
                polygonsOverlay(child,out);
            }
        }
        break;
    }
}