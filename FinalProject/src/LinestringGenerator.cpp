#include "LinestringGenerator.hpp"

LineStringGenerator::LineStringGenerator(const NodePtr &_baseAst) : Generator(_baseAst){ }
LineStringGenerator::LineStringGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode)
        : Generator(_baseAst, _overlayAst,_mode) { }

void LineStringGenerator::generate(std::ostream &out){
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

OrderedTriplet LineStringGenerator::findPolyEdge(OrderedTriplet prev, OrderedTriplet cur, int count){
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

void LineStringGenerator::linestringsMap(const NodePtr &ast, std::ostream &out){
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

void LineStringGenerator::linestrings(const NodePtr &ast, std::ostream &out, OrderedTriplet prevPoint, bool prevInPoly){
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
