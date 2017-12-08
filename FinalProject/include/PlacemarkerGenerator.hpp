#pragma once
#include "generator.hpp"

struct PlacemarkerGenerator : public Generator {
    int numPlacemarks = 0;
    PlacemarkerGenerator(const NodePtr &_baseAst);
    PlacemarkerGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode);
    virtual void generate(std::ostream &out);
    void placemarksBase(const NodePtr &ast, std::ostream &out);
    void placemarksOverlay(const NodePtr &ast, std::ostream &out);
};