#pragma once

#include <vector>
#include <memory>
#include <string>

class Node {
public: enum { KML, DOCUMENT, MAJOR_TAG, MINOR_TAG, NAME_TAG, DESCRIPTION_TAG, POINT_TAG, COORD_TAG, STRING_LITERAL, NUMBER_LITERAL};
public: std::vector < std::shared_ptr < Node > > children;
public: virtual int type() const = 0;
public: virtual ~Node();
};

class StringNode : public Node {
public: std::string strLiteral;
public: StringNode(const std::string &_strLiteral);
public: int type() const;
};

class NumberNode : public Node {
public: int number;
public: NumberNode(int _number);
public: int type() const;
};

class StartCommandNode : public Node {
public: StartCommandNode(std::shared_ptr< Node > word, std::shared_ptr < Node > number);
public: int type() const;

public: const std::string & target() const;

public: int power() const;
  
};

class StopCommandNode : public Node {
public: StopCommandNode(std::shared_ptr< Node > word);
public: int type() const;

public: const std::string & target() const;

};

class KMLNode : public Node {
  public: int type() const;
};


typedef std::shared_ptr < Node > NodePtr;
typedef std::shared_ptr < ProgramNode > ProgramNodePtr;
typedef std::shared_ptr < WordNode > WordNodePtr;
typedef std::shared_ptr < NumberNode > NumberNodePtr;
typedef std::shared_ptr < StartCommandNode > StartCommandNodePtr;
typedef std::shared_ptr < StopCommandNode > StopCommandNodePtr;

int number(const NodePtr p);

const std::string & word(const NodePtr p);

NodePtr node(int number);

NodePtr node(const std::string &word);