#include "OverlayPolyPointsGenerator.hpp"

OverlayPolyPointsGenerator::OverlayPolyPointsGenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst, COMPILE_MODE _mode)
        : Generator(_baseLayerAst, _overlayLayerAst, _mode){}

void OverlayPolyPointsGenerator::generate(std::ostream &out){
    //This really doesn't feel like it belongs here   
    if(mode != MAP){         
        getOverlayPolyPoints(overlayAst);
    }
}

void OverlayPolyPointsGenerator::getOverlayPolyPoints(const NodePtr &ast){
    switch(ast->type()){
        case Node::POLYGON:{                
            PolygonNodePtr polyNode = std::dynamic_pointer_cast<PolygonNode>(ast);                
            OuterBoundaryNodePtr outerBoundary = std::dynamic_pointer_cast<OuterBoundaryNode>(polyNode->children.at(0));                
            LinearRingNodePtr linearRing = std::dynamic_pointer_cast<LinearRingNode>(outerBoundary->children.at(0));                
            CoordinateListNodePtr coordinateList = std::dynamic_pointer_cast<CoordinateListNode>(linearRing->children.at(0));            
            //int count = 0;
            for(auto child : coordinateList->children){
                //get the current coordinate's x, y, and z values
                NumberLiteralNodePtr xCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(0));
                NumberLiteralNodePtr yCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(1));
                NumberLiteralNodePtr zCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(2));
        
                //Make a new ordered triplet
                OrderedTriplet newCoor(xCorNode->numberLiteral, yCorNode->numberLiteral, zCorNode->numberLiteral);

                //add it to the vector of points in the overlay polygon
                overlayPolyPoints.push_back(newCoor);
            }
        }
        break;
        default:{
            for(auto child : ast->children){
                getOverlayPolyPoints(child);
            }
        }
        break;
    }
}