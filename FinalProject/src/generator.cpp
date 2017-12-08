#include <set>
#include "generator.hpp"

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