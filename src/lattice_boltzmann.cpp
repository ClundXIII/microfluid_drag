#include "lattice_boltzmann.h"

#include "cell.h"
#include "fluid_simulation.h"
#include <sstream>
#include <fstream>

#include <iostream>

#include <math.h>

#if ( _USE_VEMC2 == 1 )
    #define custom_out fluidSim->out
    #define out_endl Vesper::LoggingTypes::eom
#else
    #define custom_out std::cout
    #define out_endl std::endl;
#endif // _USE_VEMC2

#if ( _USE_VEMC2 == 1 )
lattice_boltzmann::lattice_boltzmann(fluid_simulation *u, bdt init_Flow, bdt barrier_diameter) : effect(u){
#else
lattice_boltzmann::lattice_boltzmann(fluid_simulation *u, bdt init_Flow, bdt barrier_diameter){
#endif
    fluidSim = u;

    std::stringstream ss;

    ss << "simulation2_" << (u->size_x-2) << "_" << (u->size_y-2) << "_" << (u->size_z-2) << "_" << init_Flow << "_" << barrier_diameter << "_force.dat";

    out_customF = new std::fstream(ss.str(), std::ios::out | std::ios::trunc);
    *out_customF << "#Time ForceX ForceY ForceZ ForceAbs" << std::endl;
}

int i=0;

void lattice_boltzmann::tick(){


    if (! (i%100)){
        custom_out << "###########" << i << "############" << out_endl;
        fluidSim->getCellByXYZ(fluidSim->size_x/2,fluidSim->size_y/2,fluidSim->size_z/2)->printFlowVec();
        std::cout << fluidSim->getCellByXYZ(fluidSim->size_x/2,fluidSim->size_y/2,fluidSim->size_z/2)->outbound_flow[_00p] << std::endl;
        bdt tmpForce[3] = {0, 0, 0};
        getTotalF(tmpForce);
        *out_customF << i << " " << tmpForce[0] << " " << tmpForce[1] << " " << tmpForce[2] << " " << " " <<
        sqrt(tmpForce[0]*tmpForce[0]+tmpForce[1]*tmpForce[1]+tmpForce[2]*tmpForce[2]) << std::endl;
    }
    i++;

    #pragma omp parallel for
    for (int i=0; i<fluidSim->cellArraySize; i++){
        fluidSim->cellArray[i]->apply_boundary();
    }

    #pragma omp parallel for
    for (int i=0; i<fluidSim->cellArraySize; i++){
        fluidSim->cellArray[i]->stream();
    }

    #pragma omp parallel for
    for (int i=0; i<fluidSim->cellArraySize; i++){
        fluidSim->cellArray[i]->collide();
    }
}

void lattice_boltzmann::upValues(){
}

void lattice_boltzmann::setCellList(cell **allCells){
    this->allCells = allCells;
}

void lattice_boltzmann::getTotalF(bdt fToGet[]){

    for (int i=0; i<fluidSim->size_x; i++){
        for (int j=0; j<fluidSim->size_y; j++){
            for (int k=0; k<fluidSim->size_z; k++){
                cell *thisCell = fluidSim->getCellByXYZ(i, j, k);

                bdt tmpDragForce[] = {0, 0, 0};
                thisCell->buildDragForce(tmpDragForce);

                fToGet[0] += tmpDragForce[0];
                fToGet[1] += tmpDragForce[1];
                fToGet[2] += tmpDragForce[2];
            }
        }
    }

}

lattice_boltzmann::~lattice_boltzmann()
{
    out_customF->close();
    delete(out_customF);
}
