#pragma once

#include "ast.hpp"
#include <memory>
#include <vector>

enum COMPILE_MODE {MAP, SUBSET, SETADD };

struct OrderedTriplet{
    double x,y,z;
    OrderedTriplet(double _x, double _y, double _z);
};

struct Generator {
  NodePtr baseAst;
  NodePtr overlayAst;
  std::vector<OrderedTriplet> overlayPolyPoints;
  COMPILE_MODE mode;
  Generator(const NodePtr &_baseAst);
  Generator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode);
  virtual void generate(std::ostream &out) = 0;
  bool pointInPolygon(OrderedTriplet &point);
};

typedef std::shared_ptr <Generator> GeneratorPtr;

GeneratorPtr generator(const NodePtr &ast);
GeneratorPtr generator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode);
