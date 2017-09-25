#pragma once

class WordNode : public Node {
public: std::string word;
public: WordNode(const std::string &_word) : word(_word){}
public: int type() {return WORD_LITERAL; }
};

class NumberNode : public Node {
public: int number;
public: NumberNode(int _number) : number(_number){}
public: int type() { return NUMBER; }
};

