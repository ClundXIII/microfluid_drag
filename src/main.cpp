#include <iostream>
#include <list>
#include <vector>
#include <string>

#include "define.h"
#include "fluid_simulation.h"

#if ( _USE_VEMC2 == 1 )
    #include <vemc2/core/universe.h>
    Vesper::Logging v_out(Vesper::LoggingTypes::client);
    #define out v_out
    #define out_endl Vesper::LoggingTypes::eom
#else
    #define out std::cout
    #define out_endl std::endl;
#endif // _USE_VEMC2

int main(int argc, const char* argv[]){

    out << "Loading Lattice Boltzmann ..." << out_endl;
    out << "              (c) Simon Michalke, 2016" << out_endl;


    std::list<std::string> argList;

    for (int i=0; i<argc; i++){
        argList.push_back(argv[i]);
    }

    for (int i=0; i<argc; i++){
        //if (argList.at(i) == "")
    }

    fluid_simulation *u = new fluid_simulation();

    delete(u);

    out << "exit(0);" << out_endl;
    return 0;
}
