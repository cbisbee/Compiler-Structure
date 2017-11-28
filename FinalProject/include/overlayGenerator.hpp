#pragma once

#include "ast.hpp"
#include <memory>
#include <vector>

struct OrderedTriplet{
    float x,y,z;
    OrderedTriplet(float _x, float _y, float _z);
};

struct OverlayGenerator {
    NodePtr baseLayerAst;
    NodePtr overlayLayerAst;
    std::vector<OrderedTriplet> overlayPolyPoints;
    OverlayGenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst);
    virtual void generateOverlay(std::ostream &out) = 0;
};

typedef std::shared_ptr <OverlayGenerator> OverlayGeneratorPtr;

OverlayGeneratorPtr overlaygenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst);