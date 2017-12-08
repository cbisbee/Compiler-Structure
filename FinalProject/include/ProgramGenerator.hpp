#pragma once
#include "HeaderGenerator.hpp"
#include "generator.hpp"
#include "FooterGenerator.hpp"
#include "OverlayPolyPointsGenerator.hpp"
#include "LinestringGenerator.hpp"
#include "PlacemarkerGenerator.hpp"
#include "PolygonGenerator.hpp"

struct ProgramGenerator : public Generator {
    HeaderGenerator header;
    PlacemarkerGenerator placemarkers;
    OverlayPolyPointsGenerator polyPoints;
    LineStringGenerator linestrings;
    PolygonGenerator polygons;
    FooterGenerator footer;  
    ProgramGenerator(const NodePtr &_baseAst);
    ProgramGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode);
    virtual void generate(std::ostream &out);
};

GeneratorPtr generator(const NodePtr &ast);
GeneratorPtr generator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode);

