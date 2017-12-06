#include <set>
#include <string>
#include <sstream>
#include <iomanip>
#include "generator.hpp"


template <class T>
std::string numToString(T num)
{
   std::ostringstream ss;
   ss << std::fixed << std::showpoint << std::setprecision(3) << num;
   return ss.str();
}

Generator::Generator(const NodePtr &_baseAst)
    : baseAst(_baseAst), overlayAst(NULL), mode(MAP) { }

Generator::Generator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode)
    : baseAst(_baseAst), overlayAst(_overlayAst), mode(_mode) { }

OrderedTriplet::OrderedTriplet() : x(0), y(0), z(0), isdefault(true) { }
OrderedTriplet::OrderedTriplet(double _x, double _y, double _z) : x(_x), y(_y), z(_z), isdefault(false) { }


bool Generator::pointInPolygon(OrderedTriplet &point) {
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
                /*
                oddNodes^=(overlayPolyPoints.at(i).x + (point.y-overlayPolyPoints.at(i).y)
                /(overlayPolyPoints.at(j).y - overlayPolyPoints.at(i).y)*(overlayPolyPoints.at(j).x
                - overlayPolyPoints.at(i).x) < point.x);
                */
                if (overlayPolyPoints.at(i).x + (point.y - overlayPolyPoints.at(i).y)
                    /(overlayPolyPoints.at(j).y - overlayPolyPoints.at(i).y)
                    *(overlayPolyPoints.at(j).x-overlayPolyPoints.at(i).x) < point.x) {
                        oddNodes=!oddNodes;
                    }
            }
        j=i; 
    }
    return oddNodes;
}



struct HeaderGenerator : public Generator {
    HeaderGenerator(const NodePtr &_baseAst)
        : Generator(_baseAst) { }
    HeaderGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode)
        : Generator(_baseAst, _overlayAst, _mode) { }
        
    virtual void generate(std::ostream &out) {
        switch(mode){
            case MAP:{
                out << "import requests" << std::endl;
                out << "APIKEY = \"ENTER YOUR OWN APIKEY\"" << std::endl;
                out << "url = \"http://maps.google.com/maps/api/staticmap?&maptype=hybrid&size=640x640\"" << std::endl;
                out << std::endl;
            }
            break;
            case SUBSET:{
                out << "<kml>" << std::endl;
                out << "\t<Document>" << std::endl;
            }
            break;
            case SETADD:{
                out << "<kml>" << std::endl;
                out << "\t<Document>" << std::endl;                
            }
            break;
            default:{
                //Do nothing for now
            }
            break;
        }
    }
};


struct FooterGenerator : public Generator {
    FooterGenerator(const NodePtr &_baseAst) : Generator(_baseAst) { }
    FooterGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode)
        : Generator(_baseAst,_overlayAst, _mode) { }

    virtual void generate(std::ostream &out) {
        switch(mode){
            case MAP:{
                out << "url += \"&key=\"" << std::endl;
                out << "url += APIKEY" << std::endl;
                out << "session = requests.Session()" << std::endl;
                out << "r = session.get(url)" << std::endl;
                out << "f = open('testMap.png','wb')" << std::endl;
                out << "f.write(r.content)" << std::endl;
                out << "f.close()" << std::endl;
            }
            break;
            case SUBSET:{
                out << "\t</Document>" << std::endl;
                out << "</kml>";
            }
            break;
            case SETADD:{
                out << "\t</Document>" << std::endl;
                out << "</kml>";                
            }
            break;
            default:{
                //Do nothing for now
            }
            break;
        }
    }
};

//This is where we get all of the points in the overlay poly(s)
//I am not entirely convinced that this needs to inherit from OverlayGenerator...
struct OverlayPolyPointsGenerator : public Generator {
    OverlayPolyPointsGenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst, COMPILE_MODE _mode)
        : Generator(_baseLayerAst, _overlayLayerAst, _mode){}
    
    virtual void generate(std::ostream &out){
        //This really doesn't feel like it belongs here   
        if(mode != MAP){         
            getOverlayPolyPoints(overlayAst);
        }
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


struct LineStringGenerator : public Generator {
    LineStringGenerator(const NodePtr &_baseAst) : Generator(_baseAst){ }
    LineStringGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode)
        : Generator(_baseAst, _overlayAst,_mode) { }
    
    virtual void generate(std::ostream &out){
        switch(mode){
            case MAP:{
                out << "#Beginning of the line string section" << std::endl;
                linestringsMap(baseAst, out);
                out << "#End of the line string section" << std::endl;
            }
            break;
            case SUBSET:{
                OrderedTriplet prevPoint;
                bool prevInPoly = false;
                linestrings(baseAst, out, prevPoint,prevInPoly);
            }
            break;
            case SETADD:{    
                OrderedTriplet prevPoint;
                bool prevInPoly = false;            
                linestrings(baseAst, out, prevPoint, prevInPoly);
                linestrings(overlayAst, out, prevPoint, prevInPoly);
            }
            break;
            default:{
                //Do nothing for now
            }
            break;
        }
    }

    OrderedTriplet findPolyEdge(OrderedTriplet prev, OrderedTriplet cur, int count){
        double aveX = (prev.x + cur.x) / 2.0;
        double aveY = (prev.y + cur.y) / 2.0;
        double aveZ = (prev.z + cur.z) / 2.0;
        OrderedTriplet ave(aveX, aveY, aveZ);
        if(count == 20)
            return ave;
        else {
            if(pointInPolygon(ave))
                return findPolyEdge(prev, ave, ++count);
            else
                return findPolyEdge(ave, cur, ++count);
        }
    }

    void linestringsMap(const NodePtr &ast, std::ostream &out){
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
                        out << "url += \"" << "|" << numToString(yCorNode->numberLiteral) << "," << numToString(xCorNode->numberLiteral) << "\"" << std::endl;
                    } else {                        
                        out << "url += \"" << "|" << numToString(yCorNode->numberLiteral) << "," << numToString(xCorNode->numberLiteral) << "\"" << std::endl;
                    }
                    ++count;
                }
            }
            break;
            default:{
                for(auto child : ast->children){
                    linestringsMap(child,out);
                }
            }
            break;
        }
    }

    void linestrings(const NodePtr &ast, std::ostream &out, OrderedTriplet prevPoint, bool prevInPoly){
        switch(ast->type()){
            case Node::LINESTRING:{
                LineStringNodePtr lineString = std::dynamic_pointer_cast<LineStringNode>(ast);
                CoordinateListNodePtr coordinateList = std::dynamic_pointer_cast<CoordinateListNode>(lineString->children.at(0));
                int count = 0;

                out << "\t\t<LineString>" << std::endl;
                out << "\t\t\t<coordinates>" << std::endl;

                for(auto child : coordinateList->children){
                    //Get the current coordinate's x, y, and z values
                    NumberLiteralNodePtr xCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(0));
                    NumberLiteralNodePtr yCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(1));
                    NumberLiteralNodePtr zCorNode = std::dynamic_pointer_cast<NumberLiteralNode>(child->children.at(2));

                    //Need to find out if this coordinate is contained within the overlay layer's polygon(s)
                    //Then I need to do something with it after I figure that out
                    OrderedTriplet currentPoint(xCorNode->numberLiteral, yCorNode->numberLiteral, zCorNode->numberLiteral);
                    if(mode == SUBSET){
                        if(pointInPolygon(currentPoint)){
                            if(!prevInPoly && !prevPoint.isdefault){
                                //Find the edge of the polygon
                                OrderedTriplet polyEdgePoint = findPolyEdge(prevPoint,currentPoint,0);   
                                //Add a new point to the linestring so we get better subset accuracy
                                out << std::fixed << std::showpoint << std::setprecision(15);
                                out << "\t\t\t\t" << polyEdgePoint.x << ", " << polyEdgePoint.y << ", " << polyEdgePoint.z << std::endl;           
                            }
                            out << std::fixed << std::showpoint << std::setprecision(15);
                            out << "\t\t\t\t" << currentPoint.x << ", " << currentPoint.y << ", " << currentPoint.z << std::endl;
                            ++count;
                            prevInPoly = true;
                            prevPoint = currentPoint;
                        }
                        //The current point was not in the polygon
                        else{
                            if(prevInPoly && !prevPoint.isdefault){
                                //Find the edge of the polygon
                                OrderedTriplet polyEdgePoint = findPolyEdge(prevPoint,currentPoint,0);   
                                //Add a new point to the linestring so we get better subset accuracy
                                out << std::fixed << std::showpoint << std::setprecision(15);
                                out << "\t\t\t\t" << polyEdgePoint.x << ", " << polyEdgePoint.y << ", " << polyEdgePoint.z << std::endl;   
                            }
                            prevInPoly = false;
                            prevPoint = currentPoint;
                        }
                    }
                    else if(mode == SETADD){
                        out << std::fixed << std::showpoint << std::setprecision(15);
                        out << "\t\t\t\t" << currentPoint.x << ", " << currentPoint.y << ", " << currentPoint.z << std::endl;
                        ++count;
                    }
                }

                out << "\t\t\t</coordinates>" << std::endl;
                out << "\t\t</LineString>" <<std::endl;                
            }
            break;
            default:{
                for(auto child : ast->children){
                    linestrings(child,out, prevPoint, prevInPoly);
                }
            }
            break;
        }
    }

};

struct PlacemarkerGenerator : public Generator {
    int numPlacemarks = 0;
    PlacemarkerGenerator(const NodePtr &_baseAst) : Generator(_baseAst) { }
    PlacemarkerGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode)
        : Generator(_baseAst,_overlayAst, _mode) { }
    
    virtual void generate(std::ostream &out){
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

    void placemarksBase(const NodePtr &ast, std::ostream &out){        
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

    void placemarksOverlay(const NodePtr &ast, std::ostream &out){
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
};

struct PolygonGenerator : public Generator {
    PolygonGenerator(const NodePtr &_baseAst) : Generator(_baseAst) { }
    PolygonGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode)
        : Generator(_baseAst, _overlayAst, _mode) { }

    virtual void generate(std::ostream &out){
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

    void polygonsBase(const NodePtr &ast, std::ostream &out){
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

    void polygonsOverlay(const NodePtr &ast, std::ostream &out){
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
};


struct ProgramGenerator : public Generator {
    HeaderGenerator header;
    PlacemarkerGenerator placemarkers;
    OverlayPolyPointsGenerator polyPoints;
    LineStringGenerator linestrings;
    PolygonGenerator polygons;
    FooterGenerator footer;  

    ProgramGenerator(const NodePtr &_baseAst)
        : Generator(_baseAst), header(_baseAst), placemarkers(_baseAst), polyPoints(NULL, NULL, MAP), linestrings(_baseAst),
        polygons(_baseAst), footer(_baseAst) { }
    ProgramGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode)
        : Generator(_baseAst,_overlayAst, _mode), header(_baseAst, _overlayAst, _mode), placemarkers(_baseAst,_overlayAst, _mode),
        polyPoints(_baseAst,_overlayAst, _mode), linestrings(_baseAst, _overlayAst, _mode),
        polygons(_baseAst,_overlayAst,_mode), footer(_baseAst, _overlayAst, _mode) { }

  virtual void generate(std::ostream &out) {
      //if we are only compiling a single KML file
      if(mode == MAP){         
        header.generate(out); 
        placemarkers.generate(out);
        linestrings.generate(out);
        polygons.generate(out);
        footer.generate(out);
      }
      //if we are compiling a base KML file with an overlay KML file
      else{
        header.generate(out);
        polyPoints.generate(out);

        //This seems like a hack, worth checking out in the future
        overlayPolyPoints = polyPoints.overlayPolyPoints;
        linestrings.overlayPolyPoints = polyPoints.overlayPolyPoints;
        placemarkers.overlayPolyPoints = polyPoints.overlayPolyPoints;
        linestrings.generate(out);
        placemarkers.generate(out);
        polygons.generate(out);
        footer.generate(out);
      }    
  }
};

GeneratorPtr generator(const NodePtr &ast) {
    return GeneratorPtr(new ProgramGenerator(ast));
}

GeneratorPtr generator(const NodePtr &baseAst, const NodePtr &overlayAst, COMPILE_MODE _mode) {
  return GeneratorPtr(new ProgramGenerator(baseAst,overlayAst, _mode));
}