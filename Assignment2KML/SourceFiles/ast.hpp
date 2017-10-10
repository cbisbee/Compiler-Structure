#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iostream>

class Node {
public: enum {
    PROGRAM,
    KML,
    DOCUMENT,
    PLACEMARKER,
    LINESTRING,
    DESCRIPTORS,
    NAME,
    DESCRIPTION,
    POINT,
    COORDINATE_LIST,
    COORDINATE,
    NUMBER_LITERAL,
    STRING_LITERAL
  };
public: std::vector < std::shared_ptr < Node > > children;
public: virtual int type() const = 0;
public: virtual void print(std::ostream &out) const = 0;
public: virtual ~Node();
};

typedef std::shared_ptr < Node > NodePtr;

class StringLiteralNode : public Node {
public: std::string stringLiteral;
public: StringLiteralNode(const std::string &_stringLiteral);
public: int type() const;
public: void print(std::ostream &out) const;
};
typedef std::shared_ptr < StringLiteralNode > StringLiteralNodePtr;

class NumberLiteralNode : public Node {
public: float numberLiteral;
public: NumberLiteralNode(float _numberLiteral);
public: int type() const;
public: void print(std::ostream &out) const;  
};
typedef std::shared_ptr < NumberLiteralNode > NumberLiteralNodePtr;

class CoordinateListNode : public Node {
public: int type() const;
public: void print(std::ostream &out) const;
};
typedef std::shared_ptr <CoordinateListNode> CoordinateListNodePtr;

class CoordinateNode : public Node {
public: CoordinateNode(const NodePtr &number1, const NodePtr &number2, const NodePtr &number3);
public: int type() const;
public: void print(std::ostream &out) const;
};
typedef std::shared_ptr < CoordinateNode > CoordinateNodePtr;

class PointNode : public Node {
public: PointNode(const NodePtr &coordinate);
public: int type() const;
public: void print(std::ostream &out) const;
};
typedef std::shared_ptr < PointNode > PointNodePtr;

class DescriptionNode : public Node {
public: DescriptionNode(const NodePtr &stringLiteral);
public: int type() const;
public: void print(std::ostream &out) const;
};
typedef std::shared_ptr < DescriptionNode > DescriptionNodePtr;

class NameNode : public Node {
public: NameNode(const NodePtr &stringLiteral);
public: int type() const;
public: void print(std::ostream &out) const;
};
typedef std::shared_ptr < NameNode > NameNodePtr;

class DescriptorsNode : public Node {
public: int type() const;
public: void print(std::ostream &out) const;
};
typedef std::shared_ptr < DescriptorsNode > DescriptorsNodePtr;

class LineStringNode : public Node {
public: LineStringNode(const NodePtr &coordinateList);
public: int type() const;
public: void print(std::ostream &out) const;
};
typedef std::shared_ptr < LineStringNode > LineStringNodePtr;

class PlacemarkerNode : public Node {
public: PlacemarkerNode(const NodePtr &descriptors);
public: int type() const;
public: void print(std::ostream & out) const;
};
typedef std::shared_ptr < PlacemarkerNode > PlacemarkerNodePtr;

class DocumentNode : public Node {
public: int type() const;
public: void print(std::ostream &out) const;
};
typedef std::shared_ptr < DocumentNode > DocumentNodePtr;

class KMLNode : public Node {
public: KMLNode(const NodePtr &document);
public: int type() const;
public: void print(std::ostream &out) const;
};
typedef std::shared_ptr < KMLNode > KMLNodePtr;

class ProgramNode : public Node {
public: int type() const;
public: void print(std::ostream &out) const;
};
typedef std::shared_ptr < ProgramNode > ProgramNodePtr;

int numberLiteral(const NodePtr &p);

const std::string & stringLiteral(const NodePtr &p);

NodePtr node(float numberLiteral);

NodePtr node(const std::string &stringLiteral);

std::ostream &operator<< (std::ostream& out, const NodePtr &p);