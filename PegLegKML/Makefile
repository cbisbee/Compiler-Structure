CXXFLAGS=-std=gnu++11
all : kml

kml.cpp : kml.leg
	leg -o kml.cpp kml.leg

kml : ast.hpp ast.cpp kml.cpp generator.hpp generator.cpp
	$(CXX) $(CXXFLAGS) ast.cpp kml.cpp generator.cpp -o kml $(LDFLAGS)