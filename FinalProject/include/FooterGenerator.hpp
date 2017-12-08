#pragma once
#include "generator.hpp"

struct FooterGenerator : public Generator {
    FooterGenerator(const NodePtr &_baseAst);
    FooterGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode);

    virtual void generate(std::ostream &out);
};