#include <iostream>
#include <list>
#include <vector>
#include <string>

#include "define.h"
#include "fluid_simulation.h"

#if ( _USE_VEMC2 == 1 )

    #include <vemc2/graphic/graphic.h>

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

    out << "bdt is double" << out_endl;

    std::vector<std::string> argList;

    for (int i=0; i<argc; i++){
        argList.push_back(argv[i]);
    }

    if (argList.size() == 1) {

        for (int i=0; i<argc; i++){
            //if (argList.at(i) == "")
        }

        int x_size=10,
            y_size=10,
            z_size=200;

        fluid_simulation *u = new fluid_simulation((x_size+2)*(y_size+2)*(z_size+2)+10);


        #if ( _USE_VEMC2 == 1 )
        out << "open vemc2 graphic window" << out_endl;

        vemc2::graphic::graphicgl *g = new vemc2::graphic::graphicgl(argc, (char**) argv);

        #endif

        out << "setting up cells ..." << out_endl;
        u->createCellGrid(x_size, y_size, z_size, boundary_noslip, true);
        u->setupEffects();

        //u->print_debug();

        #if ( _USE_VEMC2 == 1 )
        u->simulationThread->time_to_stop = -1;
        u->start();
        u->unpause();

        out << "attach universe ..." << out_endl;
        //g->attachUniverse(u);

        std::this_thread::sleep_for(std::chrono::milliseconds(50000));

        #else

        u->run(300);

        #endif


        #if ( _USE_VEMC2 == 1 )

        g->stop();
        delete(g);

        #endif

        delete(u);

        out << "exit(0);" << out_endl;
        return 0;
    }
    else {
        if (argList.at(1) == "test"){
            //run tests
        }
    }
}
