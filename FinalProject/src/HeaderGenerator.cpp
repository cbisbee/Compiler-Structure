 #include "HeaderGenerator.hpp"

HeaderGenerator::HeaderGenerator(const NodePtr &_baseAst)
        : Generator(_baseAst) { }
HeaderGenerator::HeaderGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode)
        : Generator(_baseAst, _overlayAst, _mode) { }
        
void HeaderGenerator::generate(std::ostream &out) {
    switch(mode){
        case MAP:{
            out << "import requests" << std::endl;
            out << "APIKEY = \"ENTER YOUR OWN APIKEY\"" << std::endl;
            out << "url = \"http://maps.google.com/maps/api/staticmap?&maptype=hybrid&size=640x640\"" << std::endl;
            out << std::endl;
        }
        break;
        case SUBSET:{
            out << "<kml>" << std::endl;
            out << "\t<Document>" << std::endl;
        }
        break;
        case SETADD:{
            out << "<kml>" << std::endl;
            out << "\t<Document>" << std::endl;                
        }
        break;
        default:{
            //Do nothing for now
        }
        break;
    }
}