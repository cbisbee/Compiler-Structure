#pragma once

#include "ast.hpp"
#include <memory>
#include <vector>

struct OrderedTriplet{
    double x,y,z;
    OrderedTriplet(double _x, double _y, double _z);
};

struct Generator {
  NodePtr baseAst;
  NodePtr overlayAst;
  std::vector<OrderedTriplet> overlayPolyPoints;
  Generator(const NodePtr &_baseAst);
  Generator(const NodePtr &_baseAst, const NodePtr &_overlayAst);
  virtual void generate(std::ostream &out) = 0;
  bool pointInPolygon(OrderedTriplet &point);
};

typedef std::shared_ptr <Generator> GeneratorPtr;

GeneratorPtr generator(const NodePtr &ast);
GeneratorPtr generator(const NodePtr &_baseAst, const NodePtr &_overlayAst);
