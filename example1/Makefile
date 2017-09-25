all : example1.exe

example1.lex.cpp : example1.l
	win_flex -o example1.lex.cpp example1.l

example1.tab.hpp example1.tab.cpp : example1.ypp
	win_bison -d -o example1.tab.cpp example1.ypp

example1.exe : example1.lex.cpp example1.tab.cpp example1.tab.hpp
	g++ example1.lex.cpp example1.tab.cpp -o example1.exe
