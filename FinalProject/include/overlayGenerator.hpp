#pragma once

#include "ast.hpp"
#include <memory>
#include <vector>

struct OrderedTriplet{
    int x,y,z;
    OrderedTriplet(int _x, int _y, int _z);
}

struct OverlayGenerator {
    vector<OrderedTriplet> overlayPolyPoints;
    NodePtr baseLayerAst;
    NodePtr overlayLayerAst;
    OverlayGenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst);
    virtual void generateOverlay(std::ostream &out) = 0;
};

typedef std::shared_ptr <OverlayGenerator> OverlayGeneratorPtr;

OverlayGeneratorPtr overlaygenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst);