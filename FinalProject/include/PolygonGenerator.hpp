#pragma once
#include "generator.hpp"

struct PolygonGenerator : public Generator {
    PolygonGenerator(const NodePtr &_baseAst);
    PolygonGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode);
    virtual void generate(std::ostream &out);
    void polygonsBase(const NodePtr &ast, std::ostream &out);
    void polygonsOverlay(const NodePtr &ast, std::ostream &out);
};