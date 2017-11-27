CXXFLAGS=-std=gnu++11
all : kml


kml.lex.cpp : kml.l
	flex -o kml.lex.cpp kml.l
	sed -i- -e 's/register //g' kml.lex.cpp


kml.tab.hpp kml.tab.cpp : kml.ypp
	bison -d -o kml.tab.cpp kml.ypp


kml.lex.o : kml.lex.cpp kml.tab.hpp ast.hpp kml.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


ast.o : ast.cpp ast.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


generator.o : generator.cpp generator.hpp ast.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


kml.tab.o : kml.tab.cpp  ast.hpp kml.hpp generator.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<


kml : kml.tab.o kml.lex.o ast.o generator.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)


clean :
	/bin/rm -rf \#* *~ *.o example1 example1.tab.cpp example1.tab.hpp example1.lex.cpp 