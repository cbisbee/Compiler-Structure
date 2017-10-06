#include "ast.hpp"

Node::~Node() {};



StringLiteralNode::StringLiteralNode(const std::string &_stringLiteral)
{
  stringLiteral = _stringLiteral;
}

int StringLiteralNode::type() const
{
  return STRING_LITERAL;
}

void StringLiteralNode::print(std::ostream &out) const {
  out << "stringLiteral(" << stringLiteral << ")";
}



NumberLiteralNode::NumberLiteralNode(int _numberLiteral)
{
  numberLiteral = _numberLiteral;
}

int NumberLiteralNode::type() const
{
  return NUMBER_LITERAL;
}

void NumberLiteralNode::print(std::ostream &out) const {
  out << "numberLiteral(" << numberLiteral << ")";
}



CommaDelimeter::CommaDelimeter(char _commaDelimeter){
    commaDelimeter = _commaDelimeter;
}

int CommaDelimeter::type() const {
    return COMMA_DELIMETER;
}

void CommaDelimeter::print(std::ostream &out) const {
    out << "commaDelimiter(" << commaDelimeter << ")";
}



CoordinateNode::CoordinateNode(const NodePtr &number1, const NodePtr &comma1, const NodePtr &number2, const NodePtr &comma2, const NodePtr &number2) {
    children.push_back(number1);
    children.push_back(comma1);
    children.push_back(number2);
    children.push_back(comma2);
    children.push_back(number3);
}

int CoordinateNode::type() const {
    return COORDINATE;
}

void CoordinateNode:print(std::ostream &out) const {
    out << "coordinate(xCor=" << children.at(0) <<
        ", comma='" << children.at(1) << "' , " <<
        ", yCor=" << children.at(2) << ", comma='" 
        << children.at(3) << "' , zCor=" <<
        children.at(4) << ")";
}



PointNode::PointNode(const NodePtr &coordinate){
    children.push_back(coordinate);
}

int PointNode::type() const {
    return POINT;
}

void PointNode::print(std::ostream &out) const {
    out << "point(coordinate=" << children.at(0) << ")"; 
}



DescriptionNode::DescriptionNode(const NodePtr &stringLiteral){
    children.push_back(stringLiteral);
}

int DescriptionNode::type() const {
    return DESCRIPTION;
}

void DescriptionNode::print(std::ostream &out) const {
    out << "description(stringLiteral=" << children.at(0) << ")";
}



NameNode::NameNode(const NodePtr &stringLiteral) {
    children.push_back(stringLiteral);
}

int NameNode::type() const {
    return NAME;
}

void NameNode::print(std::ostream &out) const {
    out << "name(stringLiteral=" << children.at(0) << ")";
}



PlacemarkerNode::PlacemarkerNode(const NodePtr &name){
    children.push_back(name);
}

int PlacemarkerNode::type() const {
    return PLACEMARKER;
}

void PlacemarkerNode::print(std::ostream &out){
    out << "placemarker(name=" << children.at(0) << ")";
}


StartCommandNode::StartCommandNode(const NodePtr &target,
                                   const NodePtr &power)
{
  children.push_back(target);
  children.push_back(power);
}

int StartCommandNode::type() const { return START_COMMAND; }
void StartCommandNode::print(std::ostream &out) const {
  out << "start(target=" << children.at(0)
      << ", power=" << children.at(1) << ")";
}

const std::string & StartCommandNode::target() const
{
  return std::dynamic_pointer_cast<WordNode>(children.at(0))->word;
}

int StartCommandNode::power() const
{
  return std::dynamic_pointer_cast<NumberNode>(children.at(1))->number;
}

StopCommandNode::StopCommandNode(const NodePtr &target) {
  children.push_back(target);
}


int ProgramNode::type() const { return PROGRAM; }

void ProgramNode::print(std::ostream &out) const {
  out << "program(children=[" << std::endl;
  for (size_t i=0; i<children.size(); ++i) {
    out << "  " << children[i] << " // child " << i << std::endl;
  }
  out << "]) // program" << std::endl;
}

int number(const NodePtr &p) {
  return std::dynamic_pointer_cast < NumberNode >(p)->number;
}
 
const std::string & word(const NodePtr &p) {
  return std::dynamic_pointer_cast < WordNode >(p)->word;
}

NodePtr node(int number) {
  return NodePtr(new NumberNode(number));
}

NodePtr node(const std::string &word) {
  return NodePtr(new WordNode(word));
}

std::ostream &operator<< (std::ostream& out, const NodePtr &p) {
  p->print(out);
  return out;
}
