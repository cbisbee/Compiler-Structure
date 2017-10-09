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



NumberLiteralNode::NumberLiteralNode(float _numberLiteral)
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


CoordinateNode::CoordinateNode(const NodePtr &number1, const NodePtr &number2, const NodePtr &number3) {
    children.push_back(number1);
    children.push_back(number2);
    children.push_back(number3);
}

int CoordinateNode::type() const {
    return COORDINATE;
}

void CoordinateNode::print(std::ostream &out) const {
    out << "coordinate(xCor=" << children.at(0) <<
        ", yCor=" << children.at(1) << ", zCor=" <<
        children.at(2) << ")";
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



int DescriptorsNode::type() const {
    return DESCRIPTORS;
}

void DescriptorsNode::print(std::ostream &out) const {
    out << "descriptors=[" << std::endl;
    for(size_t i = 0; i < children.size(); ++i){
        out << children.at(i) << " // child " << i << std::endl;
    }
    out << "] //descriptors";
}


PlacemarkerNode::PlacemarkerNode(const NodePtr &descriptors){
    children.push_back(descriptors);
}

int PlacemarkerNode::type() const {
    return PLACEMARKER;
}

void PlacemarkerNode::print(std::ostream &out) const {
    out << "placemarker(" << children[0] << ")" << std::endl;
}



int DocumentNode::type() const {
    return DOCUMENT;
}

void DocumentNode::print(std::ostream &out) const {
    out << "document(children=[" << std::endl;
    for(size_t i = 0; i < children.size(); ++i){
        out << children[i] << "// child " << i << std::endl;  
    }
    out << "]) // document" << std::endl;
}


KMLNode::KMLNode(const NodePtr &document){
    children.push_back(document);
}

int KMLNode::type() const {
    return KML;
}

void KMLNode::print(std::ostream &out) const {
    out << "kml(";
    if(!children.empty()){
        out << children[0];
    }
    out << ") // kml" << std::endl;
}


int ProgramNode::type() const { return PROGRAM; }

void ProgramNode::print(std::ostream &out) const {
  out << "program(children=[" << std::endl;
  for (size_t i=0; i<children.size(); ++i) {
    out << "  " << children[i] << " // child " << i << std::endl;
  }
  out << "]) // program" << std::endl;
}



NodePtr node(float number) {
  return NodePtr(new NumberLiteralNode(number));
}

NodePtr node(const std::string &word) {
  return NodePtr(new StringLiteralNode(word));
}

std::ostream &operator<< (std::ostream& out, const NodePtr &p) {
  p->print(out);
  return out;
}