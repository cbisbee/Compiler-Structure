all : example1

example1.lex.cpp : example1.l
	flex -o example1.lex.cpp example1.l

example1.tab.hpp example1.tab.cpp : example1.ypp
	bison -d -o example1.tab.cpp example1.ypp

example1 : example1.lex.cpp example1.tab.cpp example1.tab.hpp
	c++ example1.lex.cpp example1.tab.cpp -o example1