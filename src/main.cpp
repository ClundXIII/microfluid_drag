#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <fstream>

#include "define.h"
#include "fluid_simulation.h"

#include "cell.h"

#if ( _USE_VEMC2 == 1 )

    #include <vemc2/graphic/graphic.h>

    #include <vemc2/core/universe.h>
    Vesper::Logging v_out_custom(Vesper::LoggingTypes::client);
    #define out_custom v_out_custom
    #define out_custom_endl Vesper::LoggingTypes::eom
#else
    #define out_custom std::cout_custom
    #define out_custom_endl std::endl;
#endif // _USE_VEMC2

int main(int argc, const char* argv[]){

    out_custom << "Loading Lattice Boltzmann ..." << out_custom_endl;
    out_custom << "              (c) Simon Michalke, 2016" << out_custom_endl;

    out_custom << "bdt is double" << out_custom_endl;

    std::vector<std::string> argList;

    for (int i=0; i<argc; i++){
        argList.push_back(argv[i]);
    }

    if (argList.size() == 1) {

        for (int i=0; i<argc; i++){
            //if (argList.at(i) == "")
        }

        int x_size=30,
            y_size=30,
            z_size=100;

        fluid_simulation *u = new fluid_simulation((x_size+2)*(y_size+2)*(z_size+2)+10);


        #if ( _USE_VEMC2 == 1 )
        out_custom << "open vemc2 graphic window" << out_custom_endl;

        vemc2::graphic::graphicgl *g = new vemc2::graphic::graphicgl(argc, (char**) argv);

        #endif

        out_custom << "setting up cells ..." << out_custom_endl;

        cell::initial_flow = 0;

        u->createCellGrid(x_size, y_size, z_size, false, boundary_noslip, true);
        u->setupEffects();

        //u->print_debug();

        #if ( _USE_VEMC2 == 1 )
        /*u->simulationThread->time_to_stop = -1;
        u->start();
        u->unpause();

        out_custom << "attach universe ..." << out_custom_endl;
        g->attachUniverse(u);

        std::this_thread::sleep_for(std::chrono::milliseconds(50000));*/

        u->unpause();
        u->run(300);

        #else

        u->run(300);

        #endif


        #if ( _USE_VEMC2 == 1 )

        g->stop();
        delete(g);

        #endif

        delete(u);

        out_custom << "exit(0);" << out_custom_endl;
        return 0;
    }
    else {
        if (argList.at(1) == "test"){
            //run tests
        }
        if (argList.at(1) == "--s1"){

            int x_size=30,
                y_size=30,
                z_size=100;

            fluid_simulation *u = new fluid_simulation((x_size+2)*(y_size+2)*(z_size+2)+10);

            out_custom << "setting up cells ..." << out_custom_endl;

            cell::initial_flow = 0;

            u->createCellGrid(x_size, y_size, z_size, false, boundary_noslip, true);
            u->setupEffects();

            out_custom << "max sum squared: " << (x_size*y_size/4) << out_custom_endl;

            for (int i=0; i<x_size; i++){
                for (int j=0; j<y_size; j++){
                    for (int k=1; k<(z_size-1); k++){
                        if ( ( ((i-(x_size-1)/2)*(i-(x_size-1)/2))+((j-(y_size-1)/2)*(j-(y_size-1)/2)) ) > ((x_size-1)*(y_size-1)/4) ){
                            u->getCellByXYZ(i, j, k)->type = boundary_noslip;
                        }
                    }
                }
            }

        /*vemc2::graphic::graphicgl *g = new vemc2::graphic::graphicgl(argc, (char**) argv);

        g->attachUniverse(u);*/

            #if ( _USE_VEMC2 == 1 )
            u->unpause();
            u->run(2000);

            #else

            u->run(300);

            #endif

            std::fstream out_customF("simulation1.dat", std::ios::out);
            out_customF << "#X Y Z ValX ValY ValZ Abs" << std::endl;

            for (int i=0; i<x_size; i++){
                for (int j=0; j<y_size; j++){
                    //for (int k=0; k<z_size; k++){
                    int k=z_size/2;
                        cell *thisCell = u->getCellByXYZ(i, j, k);
                        out_customF << i << " " << j << " " << k << " " << thisCell->getFlowVecX()
                        << " " << thisCell->getFlowVecY() << " " << thisCell->getFlowVecZ()
                        << " " << thisCell->getFlowVecAbs() << std::endl;
                    //}
                }
            }

            out_customF.close();

            delete(u);

            out_custom << "exit(0);" << out_custom_endl;
            return 0;
        }
        if (argList.at(1) == "--s2"){
            //run tests
        }
    }
}
