#pragma once

#include "ast.hpp"
#include <memory>
#include <vector>

struct OrderedTriplet{
    double x,y,z;
    OrderedTriplet(double _x, double _y, double _z);
};

struct OverlayGenerator {
    NodePtr baseLayerAst;
    NodePtr overlayLayerAst;
    std::vector<OrderedTriplet> overlayPolyPoints;
    OverlayGenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst);
    virtual void generateOverlay(std::ostream &out) = 0;
    bool pointInPolygon(OrderedTriplet &point);
};

typedef std::shared_ptr <OverlayGenerator> OverlayGeneratorPtr;

OverlayGeneratorPtr overlaygenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst);