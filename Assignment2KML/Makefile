CXXFLAGS=-std=gnu++11
all : kml

kml.lex.cpp : kml.l
	flex -o kml.lex.cpp kml.l
	sed -i- -e 's/register //g' kml.lex.cpp

kml.tab.hpp kml.tab.cpp : kml.ypp
	bison -d -o kml.tab.cpp kml.ypp

kml : ast.cpp kml.lex.cpp kml.tab.cpp kml.tab.hpp
	$(CXX) $(CXXFLAGS) ast.cpp kml.lex.cpp kml.tab.cpp -o kml $(LDFLAGS)
