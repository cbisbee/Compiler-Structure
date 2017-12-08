#include "PlacemarkerGenerator.hpp"

PlacemarkerGenerator::PlacemarkerGenerator(const NodePtr &_baseAst) : Generator(_baseAst) { }
PlacemarkerGenerator::PlacemarkerGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode)
        : Generator(_baseAst,_overlayAst, _mode) { }

void PlacemarkerGenerator::generate(std::ostream &out){
    switch(mode){
        case MAP:{
            out << "# Creating the placemarks on the canvas" << std::endl;
            placemarksBase(baseAst,out);
            out << "# End of the placemarks section" << std::endl;
        }
        break;
        case SUBSET:{
            placemarksOverlay(baseAst, out);
        }
        break;
        case SETADD:{
            placemarksOverlay(baseAst,out);
            placemarksOverlay(overlayAst,out);
        }
        break;
        default:{
            //Do nothing for now
        }
        break;
    }
}

void PlacemarkerGenerator::placemarksBase(const NodePtr &ast, std::ostream &out){        
    switch(ast->type()){
        case Node::DESCRIPTORS:{
            DescriptorsNodePtr descriptors = std::dynamic_pointer_cast<DescriptorsNode>(ast);
            std::string nameStr = "";
            std::string descStr = "";
            std::string xStr = "";
            std::string yStr = "";
            double xCor = 0;
            double yCor = 0;
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
                            out << "url += " << "\"" << numToString(yCor) << "," << numToString(xCor) << "\"" << std::endl; 
                        } else {
                            out << "url += " << "\"|" << numToString(yCor) << "," << numToString(xCor) << "\"" << std::endl;
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
                placemarksBase(child,out);
            }
        }
        break;
    }
}

void PlacemarkerGenerator::placemarksOverlay(const NodePtr &ast, std::ostream &out){
    switch(ast->type()){
        case Node::DESCRIPTORS:{
            DescriptorsNodePtr descriptors = std::dynamic_pointer_cast<DescriptorsNode>(ast);
            std::string nameStr = "";
            std::string descStr = "";
            std::string xStr = "";
            std::string yStr = "";
            std::string zStr = "";
            double xCor = 0;
            double yCor = 0;
            double zCor = 0;
            bool containedInOverlayPoly = false;
            for(auto child : descriptors->children){
                switch(child->type()){
                    case Node::NAME:{
                        StringLiteralNodePtr nameStringNode = std::dynamic_pointer_cast<StringLiteralNode>(child->children.at(0));
                        nameStr = nameStringNode->stringLiteral;
                    }
                    break;
                    case Node::DESCRIPTION:{
                        StringLiteralNodePtr descriptionStringNode = std::dynamic_pointer_cast<StringLiteralNode>(child->children.at(0));
                        descStr = descriptionStringNode->stringLiteral;                        
                    }
                    break;
                    case Node::POINT:{
                        CoordinateNodePtr corNode = std::dynamic_pointer_cast<CoordinateNode>(child->children.at(0));
                        NumberLiteralNodePtr xCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(corNode->children.at(0));
                        NumberLiteralNodePtr yCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(corNode->children.at(1));
                        NumberLiteralNodePtr zCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(corNode->children.at(2));

                        xCor = xCorNode->numberLiteral;
                        yCor = yCorNode->numberLiteral;
                        zCor = zCorNode->numberLiteral;

                        //Figure out if this coordinate falls within the overlay layer poly
                        OrderedTriplet curPoint(xCor, yCor, zCor);
                        containedInOverlayPoly = pointInPolygon(curPoint);                
                    }
                    break;
                }
            }
            if(mode == SUBSET){
                //Check if the placemarker was found in the overlay polygon and if so, add it to the output kml file
                if(containedInOverlayPoly){
                    out << "\t\t<Placemarker>" << std::endl;
                    if(nameStr != "")
                        out <<"\t\t\t<name>" << nameStr << "</name>" << std::endl;
                    if(descStr != "")
                        out << "\t\t\t<description>" << descStr << "</description>" << std::endl;
                    out << "\t\t\t<Point>" << std::endl;
                    out << std::fixed << std::showpoint << std::setprecision(15);
                    out << "\t\t\t\t<coordinates>" << xCor << ", " << yCor << ", " << zCor << "</coordinates>" << std::endl;
                    out << "\t\t\t</Point>" << std::endl;
                    out << "\t\t</Placemarker>" << std::endl;
                }
            }
            else if(mode == SETADD){
                out << "\t\t<Placemarker>" << std::endl;
                if(nameStr != "")
                    out <<"\t\t\t<name>" << nameStr << "</name>" << std::endl;
                if(descStr != "")
                    out << "\t\t\t<description>" << descStr << "</description>" << std::endl;
                out << "\t\t\t<Point>" << std::endl;
                out << std::fixed << std::showpoint << std::setprecision(15);
                out << "\t\t\t\t<coordinates>" << xCor << ", " << yCor << ", " << zCor << "</coordinates>" << std::endl;
                out << "\t\t\t</Point>" << std::endl;
                out << "\t\t</Placemarker>" << std::endl;
            }
        }
        break;
        default:{
            for(auto child : ast->children){
                placemarksOverlay(child,out);
            }
        }
        break;
    }
}