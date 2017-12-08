CXXFLAGS=-std=gnu++11 -Iinclude
all : dirs bin/kml tests

dirs : tmp bin
	if [ ! -d tmp ] ; then mkdir tmp; fi
	if [ ! -d bin ] ; then mkdir bin; fi

tmp/kml.lex.cpp : src/KML.l
	flex -o tmp/kml.lex.cpp src/KML.l
	sed -i- -e 's/register //g' tmp/kml.lex.cpp

tmp/kml.tab.hpp tmp/kml.tab.cpp : src/KML.ypp
	bison -d -o tmp/kml.tab.cpp src/KML.ypp

tmp/kml.lex.o : tmp/kml.lex.cpp tmp/kml.tab.hpp include/ast.hpp include/KML.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

tmp/ast.o : src/ast.cpp include/ast.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

tmp/generator.o : src/generator.cpp include/generator.hpp include/ast.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

tmp/HeaderGenerator.o : src/HeaderGenerator.cpp include/HeaderGenerator.hpp include/generator.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

tmp/FooterGenerator.o : src/FooterGenerator.cpp include/FooterGenerator.hpp include/generator.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

tmp/OverlayPolyPointsGenerator.o : src/OverlayPolyPointsGenerator.cpp include/OverlayPolyPointsGenerator.hpp include/generator.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

tmp/LinestringGenerator.o : src/LinestringGenerator.cpp include/LinestringGenerator.hpp include/generator.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

tmp/PlacemarkerGenerator.o : src/PlacemarkerGenerator.cpp include/PlacemarkerGenerator.hpp include/generator.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

tmp/PolygonGenerator.o : src/PolygonGenerator.cpp include/PolygonGenerator.hpp include/generator.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

tmp/ProgramGenerator.o : src/ProgramGenerator.cpp include/ProgramGenerator.hpp include/generator.hpp include/HeaderGenerator.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

tmp/kml.tab.o : tmp/kml.tab.cpp  include/ast.hpp include/KML.hpp include/generator.hpp  tmp/HeaderGenerator.o tmp/FooterGenerator.o tmp/OverlayPolyPointsGenerator.o tmp/LinestringGenerator.o tmp/PlacemarkerGenerator.o tmp/PolygonGenerator.o tmp/ProgramGenerator.o
	$(CXX) $(CXXFLAGS) -c -o $@ $<

bin/kml : tmp/kml.tab.o tmp/kml.lex.o tmp/ast.o tmp/generator.o tmp/HeaderGenerator.o tmp/FooterGenerator.o tmp/OverlayPolyPointsGenerator.o tmp/LinestringGenerator.o tmp/PlacemarkerGenerator.o tmp/PolygonGenerator.o tmp/ProgramGenerator.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean :
	/bin/rm -rf tests/*.out tmp/* bin/kml

.PHONY: tests
tests: bin/kml
	c=1; while [ -f tests/hvac.$$c.in  ]; do if [ tests/hvac.$$c.out -ot tests/hvac.$$c.in -o tests/hvac.$$c.out -ot bin/hvac ] ; then ./bin/hvac < tests/hvac.$$c.in > tests/hvac.$$c.out ; fi ; c=$$(($$c+1)); done