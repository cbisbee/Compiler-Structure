#pragma once
#include "generator.hpp"

struct HeaderGenerator : public Generator {
    HeaderGenerator(const NodePtr &_baseAst);
    HeaderGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode);
        
    virtual void generate(std::ostream &out);
};