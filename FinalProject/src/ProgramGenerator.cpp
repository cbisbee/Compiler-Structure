#include "ProgramGenerator.hpp"

ProgramGenerator::ProgramGenerator(const NodePtr &_baseAst)
        : Generator(_baseAst), header(_baseAst), placemarkers(_baseAst), polyPoints(NULL, NULL, MAP), linestrings(_baseAst),
        polygons(_baseAst), footer(_baseAst) { }

ProgramGenerator::ProgramGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode)
        : Generator(_baseAst,_overlayAst, _mode), header(_baseAst, _overlayAst, _mode), placemarkers(_baseAst,_overlayAst, _mode),
        polyPoints(_baseAst,_overlayAst, _mode), linestrings(_baseAst, _overlayAst, _mode),
        polygons(_baseAst,_overlayAst,_mode), footer(_baseAst, _overlayAst, _mode) { }

void ProgramGenerator::generate(std::ostream &out) {
    //if we are only compiling a single KML file
    if(mode == MAP){         
    header.generate(out); 
    placemarkers.generate(out);
    linestrings.generate(out);
    polygons.generate(out);
    footer.generate(out);
    }
    //if we are compiling a base KML file with an overlay KML file
    else{
    header.generate(out);
    polyPoints.generate(out);

    //This seems like a hack, worth checking out in the future
    overlayPolyPoints = polyPoints.overlayPolyPoints;
    linestrings.overlayPolyPoints = polyPoints.overlayPolyPoints;
    placemarkers.overlayPolyPoints = polyPoints.overlayPolyPoints;
    linestrings.generate(out);
    placemarkers.generate(out);
    polygons.generate(out);
    footer.generate(out);
    }    
}



GeneratorPtr generator(const NodePtr &ast) {
    return GeneratorPtr(new ProgramGenerator(ast));
}

GeneratorPtr generator(const NodePtr &baseAst, const NodePtr &overlayAst, COMPILE_MODE _mode) {
  return GeneratorPtr(new ProgramGenerator(baseAst,overlayAst, _mode));
}