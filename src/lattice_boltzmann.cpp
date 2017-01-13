#include "lattice_boltzmann.h"

#include "cell.h"
#include "fluid_simulation.h"

#include <iostream>

#if ( _USE_VEMC2 == 1 )
    #define out fluidSim->out
    #define out_endl Vesper::LoggingTypes::eom
#else
    #define out std::cout
    #define out_endl std::endl;
#endif // _USE_VEMC2

#if ( _USE_VEMC2 == 1 )
lattice_boltzmann::lattice_boltzmann(fluid_simulation *u) : effect(u){
#else
lattice_boltzmann::lattice_boltzmann(fluid_simulation *u){
#endif
    fluidSim = u;
}

int a=0;

int i=0;

void lattice_boltzmann::tick(){


    if (! (i%100)){
        out << "###########" << i << "############" << out_endl;
        fluidSim->getCellByXYZ(fluidSim->size_x/2,fluidSim->size_y/2,fluidSim->size_z/2)->printFlowVec();
        std::cout << fluidSim->getCellByXYZ(fluidSim->size_x/2,fluidSim->size_y/2,fluidSim->size_z/2)->outbound_flow[_00p] << std::endl;
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

lattice_boltzmann::~lattice_boltzmann()
{
    //dtor
}
