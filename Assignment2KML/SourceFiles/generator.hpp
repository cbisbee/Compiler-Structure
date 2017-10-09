#pragma once

#include "ast.hpp"
#include <memory>

struct Generator {
  NodePtr ast;
  Generator(const NodePtr &_ast);
  virtual void generate(std::ostream &out) = 0;
};

typedef std::shared_ptr <Generator> GeneratorPtr;

GeneratorPtr generator(const NodePtr &ast);