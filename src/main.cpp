#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>

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
    #define out_custom std::cout
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

        int _x_size=30,
            _y_size=30,
            _z_size=100;

        fluid_simulation *u = new fluid_simulation((_x_size+2)*(_y_size+2)*(_z_size+2)+10);


        #if ( _USE_VEMC2 == 1 )
        out_custom << "open vemc2 graphic window" << out_custom_endl;

        vemc2::graphic::graphicgl *g = new vemc2::graphic::graphicgl(argc, (char**) argv);

        #endif

        out_custom << "setting up cells ..." << out_custom_endl;

        cell::initial_flow = 0;

        u->createCellGrid(_x_size, _y_size, _z_size, false, boundary_noslip, true);
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

            std::string::size_type sz;   // alias of size_t

            int x_size = std::stoi(argList.at(2), &sz),
                y_size = std::stoi(argList.at(3), &sz),
                z_size = std::stoi(argList.at(4), &sz);

            bdt init_Flow =  std::stof(argList.at(5), &sz);

            fluid_simulation *u = new fluid_simulation((x_size+2)*(y_size+2)*(z_size+2)+10);

            out_custom << "setting up cells ..." << out_custom_endl;

            cell::initial_flow = 0;

            bdt init_Flow_Vec[] = {0, 0, init_Flow};

            u->createCellGrid(x_size, y_size, z_size, false, boundary_noslip, true, init_Flow_Vec);
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

            #if ( _USE_VEMC2 == 1 )
            u->unpause();
            u->run(5000);

            #else

            u->run(2000);

            #endif

            std::stringstream ss;

            ss << "simulation1_" << x_size << "_" << y_size << "_" << z_size << "_" << init_Flow << ".dat";

            std::fstream out_customF(ss.str(), std::ios::out | std::ios::trunc);
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


            std::stringstream ss2;

            ss2 << "simulation1_" << x_size << "_" << y_size << "_" << z_size << "_" << init_Flow << "_onlypipe.dat";

            std::fstream out_customF2(ss2.str(), std::ios::out | std::ios::trunc);
            out_customF2 << "#X Y Z ValX ValY ValZ Abs" << std::endl;

            for (int i=0; i<x_size; i++){
                for (int j=0; j<y_size; j++){
                    if (!( ( ((i-(x_size-1)/2)*(i-(x_size-1)/2))+((j-(y_size-1)/2)*(j-(y_size-1)/2)) ) > ((x_size-1)*(y_size-1)/4) )){
                        int k=z_size/2;
                        cell *thisCell = u->getCellByXYZ(i, j, k);
                        out_customF2 << i << " " << j << " " << k << " " << thisCell->getFlowVecX()
                        << " " << thisCell->getFlowVecY() << " " << thisCell->getFlowVecZ()
                        << " " << thisCell->getFlowVecAbs() << std::endl;
                    }
                }
            }

            out_customF2.close();

            delete(u);

            out_custom << "exit(0);" << out_custom_endl;
            return 0;
        }
        if (argList.at(1) == "--s2"){

            std::string::size_type sz;   // alias of size_t

            int x_size = std::stoi(argList.at(2), &sz),
                y_size = std::stoi(argList.at(3), &sz),
                z_size = std::stoi(argList.at(4), &sz);

            bdt init_Flow =  std::stof(argList.at(5), &sz);

            bdt barrier_diameter =  std::stof(argList.at(6), &sz);

            fluid_simulation *u = new fluid_simulation((x_size+2)*(y_size+2)*(z_size+2)+10);

            out_custom << "setting up cells ..." << out_custom_endl;

            cell::initial_flow = 0;

            bdt init_Flow_Vec[] = {0, 0, -init_Flow};

            u->createCellGrid(x_size, y_size, z_size, false, boundary_noslip, false, nullptr);
            u->setupEffects();

            out_custom << "barrier diameter: " << barrier_diameter << out_custom_endl;
            out_custom << "barrier diameter squared / 4: " << ((barrier_diameter)*(barrier_diameter)/4) << out_custom_endl;

            for (int i=0; i<x_size; i++){
                for (int j=0; j<y_size; j++){
                    for (int k=1; k<(z_size-1); k++){
                        if ( ( ((i-(x_size-1)/2.0)*(i-(x_size-1)/2.0))+((j-(y_size-1)/2.0)*(j-(y_size-1)/2.0))+((k-(z_size-1)/2.0)*(k-(z_size-1)/2.0)) ) <=
                            (barrier_diameter*barrier_diameter/4.0) ){
                            u->getCellByXYZ(i, j, k)->type = source;
                            u->getCellByXYZ(i, j, k)->inflowVec[0] = init_Flow_Vec[0];
                            u->getCellByXYZ(i, j, k)->inflowVec[1] = init_Flow_Vec[1];
                            u->getCellByXYZ(i, j, k)->inflowVec[2] = init_Flow_Vec[2];
                            u->getCellByXYZ(i, j, k)->solid_object = true;
                        }
                    }
                }
            }

            for (int i=0; i<x_size; i++){
                for (int j=0; j<y_size; j++){
                    for (int k=1; k<(z_size-1); k++){
                        if (u->getCellByXYZ(i, j, k)->solid_object){
                            for (int q=1; q<DIRECTION_FLOW_SIZE; q++){
                                if (u->getCellByXYZ(i, j, k)->get_neighbour((direction)q) && u->getCellByXYZ(i, j, k)->get_neighbour((direction)q)->solid_object){
                                    u->getCellByXYZ(i, j, k)->add_neighbour((direction)q, 0);
                                }
                            }
                        }
                    }
                }
            }

            #if ( _USE_VEMC2 == 1 )
            u->unpause();
            u->run(2000);

            #else

            u->run(2000);

            #endif

            #pragma omp parallel for
            for (int i=0; i<u->cellArraySize; i++){
                u->cellArray[i]->apply_boundary();
            }

            #pragma omp parallel for
            for (int i=0; i<u->cellArraySize; i++){
                u->cellArray[i]->stream();
            }

            std::stringstream ss;

            ss << "simulation2_" << x_size << "_" << y_size << "_" << z_size << "_" << init_Flow << "_" << barrier_diameter << ".dat";

            std::fstream out_customF(ss.str(), std::ios::out | std::ios::trunc);
            out_customF << "#X Y Z ValX ValY ValZ Abs ForceX ForceY ForceZ ForceAbs" << std::endl;

            bdt drag_force[] = {0, 0, 0};

            //for (int i=0; i<x_size; i++){
                for (int j=0; j<y_size; j++){
                    for (int k=0; k<z_size; k++){
                        int i=x_size/2;
                        cell *thisCell = u->getCellByXYZ(i, j, k);

                        bdt tmpDragForce[] = {0, 0, 0};
                        thisCell->buildDragForce(tmpDragForce);

                        out_customF << i << " " << j << " " << k << " " << thisCell->getFlowVecX()
                            << " " << thisCell->getFlowVecY() << " " << thisCell->getFlowVecZ()
                            << " " << thisCell->getFlowVecAbs() << " " << tmpDragForce[0]
                            << " " << tmpDragForce[1] << " " << tmpDragForce[2]
                            << " " << sqrt(tmpDragForce[0]*tmpDragForce[0]+tmpDragForce[1]*tmpDragForce[1]+tmpDragForce[2]*tmpDragForce[2])
                            << std::endl;

                        if (thisCell->solid_object){
                            drag_force[0] += tmpDragForce[0];
                            drag_force[1] += tmpDragForce[1];
                            drag_force[2] += tmpDragForce[2];
                        }
                    }
                }
            //}

            out_customF << "#total Force: " << drag_force[0] << " " << drag_force[1] << " " << drag_force[2] << std::endl;

            out_customF.close();

            delete(u);

            out_custom << "exit(0);" << out_custom_endl;
            return 0;
        }
    }
}
