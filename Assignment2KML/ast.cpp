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



CommaDelimeterNode::CommaDelimeterNode(char _commaDelimeter){
    commaDelimeter = _commaDelimeter;
}

int CommaDelimeterNode::type() const {
    return COMMA_DELIMETER;
}

void CommaDelimeterNode::print(std::ostream &out) const {
    out << "commaDelimiter('" << commaDelimeter << "')";
}



CoordinateNode::CoordinateNode(const NodePtr &number1, const NodePtr &comma1, const NodePtr &number2, const NodePtr &comma2, const NodePtr &number3) {
    children.push_back(number1);
    children.push_back(comma1);
    children.push_back(number2);
    children.push_back(comma2);
    children.push_back(number3);
}

int CoordinateNode::type() const {
    return COORDINATE;
}

void CoordinateNode::print(std::ostream &out) const {
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


//Check out the grammar rule for this, could use some modifications
PlacemarkerNode::PlacemarkerNode(const NodePtr &name, const NodePtr &description, const NodePtr &point){
    children.push_back(name);
    children.push_back(description);
    children.push_back(point);
}

int PlacemarkerNode::type() const {
    return PLACEMARKER;
}

void PlacemarkerNode::print(std::ostream &out) const {
    out << "placemarker(name=" << children.at(0)
    << ", desciption=" << children.at(1)
    << ", point=" <<children.at(2) << ")";
}



int DocumentNode::type() const {
    return DOCUMENT;
}

void DocumentNode::print(std::ostream &out) const {
    out << "document(children=[" << std::endl;
    for(size_t i = 0; i < children.size(); ++i){
        out << " " << children[i] << "// child " << i << std::endl;  
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
    out << "kml(children=[" << std::endl;
    for (size_t i=0; i<children.size(); ++i) {
      out << "  " << children[i] << " // child " << i << std::endl;
    }
    out << "]) // kml" << std::endl;
}


int ProgramNode::type() const { return PROGRAM; }

void ProgramNode::print(std::ostream &out) const {
  out << "program(children=[" << std::endl;
  for (size_t i=0; i<children.size(); ++i) {
    out << "  " << children[i] << " // child " << i << std::endl;
  }
  out << "]) // program" << std::endl;
}



NodePtr node(int number) {
  return NodePtr(new NumberLiteralNode(number));
}

NodePtr node(const std::string &word) {
  return NodePtr(new StringLiteralNode(word));
}

std::ostream &operator<< (std::ostream& out, const NodePtr &p) {
  p->print(out);
  return out;
}