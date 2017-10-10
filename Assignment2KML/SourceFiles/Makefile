CXXFLAGS=-std=gnu++11
all : kml

kml.lex.cpp : kml.l
	flex -o kml.lex.cpp kml.l
	sed -i- -e 's/register //g' kml.lex.cpp

kml.tab.hpp kml.tab.cpp : kml.ypp
	bison -d -o kml.tab.cpp kml.ypp

kml : ast.hpp ast.cpp kml.lex.cpp kml.tab.cpp kml.tab.hpp generator.hpp generator.cpp
	$(CXX) $(CXXFLAGS) ast.cpp kml.lex.cpp kml.tab.cpp generator.cpp -o kml $(LDFLAGS)