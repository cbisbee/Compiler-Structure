#pragma once
#include "generator.hpp"

struct LineStringGenerator : public Generator {
    LineStringGenerator(const NodePtr &_baseAst);
    LineStringGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode);
    
    virtual void generate(std::ostream &out);
    OrderedTriplet findPolyEdge(OrderedTriplet prev, OrderedTriplet cur, int count);
    void linestringsMap(const NodePtr &ast, std::ostream &out);
    void linestrings(const NodePtr &ast, std::ostream &out, OrderedTriplet prevPoint, bool prevInPoly);
};
