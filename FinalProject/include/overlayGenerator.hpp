#pragma once

#include "ast.hpp"
#include <memory>

struct OverlayGenerator {
    NodePtr baseLayerAst;
    NodePtr overlayLayerAst;
    OverlayGenerator(const NodePtr *_baseLayerAst, const NodePtr &_overlayLayerAst);
    virtual void generateOverlay(std::ostream &out) = 0;
};

typedef std::shared_ptr <OverlayGenerator> OverlayGeneratorPtr;

OverlayGeneratorPtr overlaygenerator(const NodePtr &_baseLayerAst, const NodePtr &_overlayLayerAst);