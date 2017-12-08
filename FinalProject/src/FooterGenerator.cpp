#include "FooterGenerator.hpp"

FooterGenerator::FooterGenerator(const NodePtr &_baseAst) : Generator(_baseAst) { }
FooterGenerator::FooterGenerator(const NodePtr &_baseAst, const NodePtr &_overlayAst, COMPILE_MODE _mode)
        : Generator(_baseAst,_overlayAst, _mode) { }

void FooterGenerator::generate(std::ostream &out) {
    switch(mode){
        case MAP:{
            out << "url += \"&key=\"" << std::endl;
            out << "url += APIKEY" << std::endl;
            out << "session = requests.Session()" << std::endl;
            out << "r = session.get(url)" << std::endl;
            out << "f = open('testMap.png','wb')" << std::endl;
            out << "f.write(r.content)" << std::endl;
            out << "f.close()" << std::endl;
        }
        break;
        case SUBSET:{
            out << "\t</Document>" << std::endl;
            out << "</kml>";
        }
        break;
        case SETADD:{
            out << "\t</Document>" << std::endl;
            out << "</kml>";                
        }
        break;
        default:{
            //Do nothing for now
        }
        break;
    }
}