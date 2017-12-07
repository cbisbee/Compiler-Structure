# Compiler-Structure
Repo for CSCI 450 - Compiler Structure, Colorado Mesa University Fall 2017

# Final Project:
- In this project we were to create a compiler for a language of our choice using tools of our choice. I decided to build off of the work that I had done for my KML parser/scanner and create a KML compiler capable of taking in simple KML style files (not actual KML!) and performing various operations on those files.

# Producing a python script to retrieve PNG of a KML file using Google Static Maps API
- This compiler is capable of taking a KML file, finding the polygons, placemarkers, and linestring paths described by the file to produce a python script which retrieves a corresponding PNG of the map data from Google's Static Map service.

- USAGE: 
  1) kml -map -in [INPUT_FILENAME].kml -out [OUTPUT_FILENAME].py
  2) python2 [OUTPUT_FILENAME].py

- Example PNG file produced:
  1) display example image here

# Performing a set union of two KML files
- This compiler is capable of taking two KML files and performing a basic set union of the two files. That is, the map items detailed in the first file will be combined with the map items in the second file to produce a single output KML file.

- USAGE:
  1) kml -add -bf [INPUT_FILENAME1].kml -of [INPUT_FILENAME2].kml -out [OUTPUT_FILENAME].kml
  
- The resulting file of this operation can then be used as input to the compiler in map mode to create a PNG file of the union KML file.
- Example of using the union feature of the compiler:
  1) Display the three pictures here
  
  
# Performing a subset operation on two KML files
- This compiler is capable of taking two KML files, the first describing path information and the second describing an overlay poylgon, and produce a seperate KML file which is the subset of the two files. That is, the output file will include only the features of the first KML file that were contained in the polygon described in the second KML file.

- USAGE:
  1) kml -sub -bf [BASE_FILENAME].kml -of [OVERLAY_FILENAME].kml -out [OUTPUT_FILENAME].kml
  
- The resulting file of this operation can then be used as input to the compiler in map mode to create a PNG file of the subset KML file.
- Example of using the subset feature of the compiler:
  1) Display the two pictures here
