lex.yy.c : example1.l
	win_flex example1.l

example1.tab.h example1.tab.c : example1.y
	win_bison -d -o example1.tab.c example1.y

example1.exe : lex.yy.c example1.tab.c
	cc lex.yy.c example1.tab.c -o example1.exe
