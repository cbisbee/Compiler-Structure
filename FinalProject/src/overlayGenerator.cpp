// This is the file where I will take the base layer ast and 
// apply the overlay layer ast to generate a completely new .skml file
// that is the subset of the base layer that is contained within the overlay layer

// The algorithm that I am currently using is really only valid for points on a plane
// technically it doesn't work great for polar coordinates or lat/long stuff because a 'straight'line
// between to lat/long coordinates isn't actually a straight line but rather curved in real life. Also
// crossing the international date line and fun things like that will not be handled very well by this 
// algorithm because of how the lat/long system works. To get reasonably accurate kml files generated by
// this code is best to not describe a polygon larger than a US state in the overlay kml layer.

// Algorithm steps for now:
// 1) Go through the overlay layer Polygon(s) and get all of the points
// 2) Go through the base layer LineString(s) and get all of the points that fall within the overlay poly(s)
// 3) Go through the base layer Placemarker(s) and get all of the placemarkers that fall with the overlay poly(s)


#include <set> //May not need this
#include <string>
#include <sstream>
#include <iomanip>
#include "overlayGenerator.hpp"

OrderedTriplet::OrderedTriplet(double _x, double _y, double _z) : x(_x), y(_y), z(_z){}

OverlayGenerator::OverlayGenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst)
    : baseLayerAst(_baseLayerAst), overlayLayerAst(_overlayLayerAst){}

bool OverlayGenerator::pointInPolygon(OrderedTriplet &point) {
    int i, j = overlayPolyPoints.size()-1;
    bool oddNodes = false;
    for (i=0; i<overlayPolyPoints.size(); ++i) {
        if (
            (
                (overlayPolyPoints.at(i).y < point.y && overlayPolyPoints.at(j).y >= point.y)
                ||
                (overlayPolyPoints.at(j).y < point.y && overlayPolyPoints.at(i).y >= point.y)
            )
            &&
            (overlayPolyPoints.at(i).x <= point.x || overlayPolyPoints.at(j).x <= point.x))
            {
                //figure this out...
                oddNodes^=(overlayPolyPoints.at(i).x + (point.y-overlayPolyPoints.at(i).y)
                /(overlayPolyPoints.at(j).y - overlayPolyPoints.at(i).y)*(overlayPolyPoints.at(j).x
                - overlayPolyPoints.at(i).x) < point.x);
            }
        j=i; 
    }
    return oddNodes;
}

//This is where we get all of the points in the overlay poly(s)
//I am not entirely convinced that this needs to inherit from OverlayGenerator...
struct OverlayPolyPointsGenerator : public OverlayGenerator {
    OverlayPolyPointsGenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst)
        : OverlayGenerator(_baseLayerAst, _overlayLayerAst){}
    
    virtual void generateOverlay(std::ostream &out){
        //This really doesn't feel like it belongs here        
        getOverlayPolyPoints(overlayLayerAst);        
    }

    void getOverlayPolyPoints(const NodePtr &ast){
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
};


//This is where we find all of the LineString stuff in the base layer that is contained inside the overlay layer
struct OverlayLineStringGenerator : public OverlayGenerator {
    OverlayLineStringGenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst)
        : OverlayGenerator(_baseLayerAst, _overlayLayerAst){}
    
    virtual void generateOverlay(std::ostream &out){
        linestrings(baseLayerAst,out);
    }

    void linestrings(const NodePtr &ast, std::ostream &out){
        switch(ast->type()){
            case Node::LINESTRING:{
                LineStringNodePtr lineString = std::dynamic_pointer_cast<LineStringNode>(ast);
                CoordinateListNodePtr coordinateList = std::dynamic_pointer_cast<CoordinateListNode>(lineString->children.at(0));
                int count = 0;

                //TODO what do we do if we find a linestring that is not in the overlay poly? we probably shouldn't print this
                out << "<LineString>" << std::endl;
                out << "\t<coordinates>" << std::endl;

                for(auto child : coordinateList->children){
                    //Get the current coordinate's x, y, and z values
                    NumberLiteralNodePtr xCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(0));
                    NumberLiteralNodePtr yCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(1));
                    NumberLiteralNodePtr zCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(2));

                    //Need to find out if this coordinate is contained within the overlay layer's polygon(s)
                    //Then I need to do something with it after I figure that out
                    OrderedTriplet currentPoint(xCorNode->numberLiteral, yCorNode->numberLiteral, zCorNode->numberLiteral);
                    if(pointInPolygon(currentPoint)){
                        out << std::fixed << std::showpoint << std::setprecision(15);
                        out << "\t\t" << currentPoint.x << ", " << currentPoint.y << ", " << currentPoint.z << std::endl;
                        ++count;
                    }

                }

                out << "\t</coordinates>" << std::endl;
                out << "</LineString>" <<std::endl;                
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

//This is where we find all of the Placemarker stuff in the base layer that is contained inside the overlay layer
struct OverlayPlacemarkerGenerator : public OverlayGenerator {
    int numPlacemarkers = 0;
    
    OverlayPlacemarkerGenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst)
        : OverlayGenerator(_baseLayerAst, _overlayLayerAst) {}
    
    virtual void generateOverlay(std::ostream &out) {
        placemarks(baseLayerAst, out);
    }

    void placemarks(const NodePtr &ast, std::ostream &out){
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
                //Check if the placemarker was found in the overlay polygon and if so, add it to the output kml file
                if(containedInOverlayPoly){
                    out << "<Placemarker>" << std::endl;
                    if(nameStr != "")
                        out <<"\t<name>" << nameStr << "</name>" << std::endl;
                    if(descStr != "")
                        out << "\t<description>" << descStr << "</description>" << std::endl;
                    out << "\t<Point>" << std::endl;
                    out << std::fixed << std::showpoint << std::setprecision(15);
                    out << "\t\t<coordinates>" << xCor << ", " << yCor << ", " << zCor << "</coordinates>" << std::endl;
                    out << "\t</Point>" << std::endl;
                    out << "</Placemarker" << std::endl;
                }
            }
            break;
            default:{
                for(auto child : ast->children){
                    placemarks(child,out);
                }
            }
            break;
        }
    }
};

struct KmlIntersectGenerator : public OverlayGenerator {
    OverlayPolyPointsGenerator polyPoints;
    OverlayLineStringGenerator lineStrings;
    OverlayPlacemarkerGenerator placemarkers;

    KmlIntersectGenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst)
        : OverlayGenerator(_baseLayerAst,_overlayLayerAst), polyPoints(_baseLayerAst,_overlayLayerAst),
        lineStrings(_baseLayerAst,_overlayLayerAst), placemarkers(_baseLayerAst,_overlayLayerAst){}
    
    virtual void generateOverlay(std::ostream &out){
        polyPoints.generateOverlay(out);

        //This seems like a hack, worth checking out in the future
        overlayPolyPoints = polyPoints.overlayPolyPoints;
        lineStrings.overlayPolyPoints = polyPoints.overlayPolyPoints;
        placemarkers.overlayPolyPoints = polyPoints.overlayPolyPoints;

        lineStrings.generateOverlay(out);
        placemarkers.generateOverlay(out);
    }
};

OverlayGeneratorPtr overlaygenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst){
    return OverlayGeneratorPtr(new KmlIntersectGenerator(_baseLayerAst,_overlayLayerAst));
}