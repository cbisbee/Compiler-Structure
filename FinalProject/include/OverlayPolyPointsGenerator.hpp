#pragma once
#include "generator.hpp"

//This is where we get all of the points in the overlay poly(s)
//I am not entirely convinced that this needs to inherit from OverlayGenerator...
struct OverlayPolyPointsGenerator : public Generator {
    OverlayPolyPointsGenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst, COMPILE_MODE _mode);
    virtual void generate(std::ostream &out);
    void getOverlayPolyPoints(const NodePtr &ast);
};