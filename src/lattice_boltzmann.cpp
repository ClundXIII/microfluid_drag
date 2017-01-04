#include "lattice_boltzmann.h"

#include "cell.h"
#include "fluid_simulation.h"

#include <iostream>

#if ( _USE_VEMC2 == 1 )
lattice_boltzmann::lattice_boltzmann(fluid_simulation *u) : effect(u){
#else
lattice_boltzmann::lattice_boltzmann(fluid_simulation *u){
#endif
    fluidSim = u;
}

int a=0;

void lattice_boltzmann::tick(){
    //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    if (!a) {std::cout << "apply boundary>";std::cin >> a;}

    #pragma omp parallel for
    for (int i=0; i<fluidSim->cellArraySize; i++){
        fluidSim->cellArray[i]->apply_boundary();
    }

    /*this->fluidSim->getCellByXYZ(2,2,5)->debug_info();
    this->fluidSim->getCellByXYZ(2,2,11)->debug_info();*/

    fluidSim->getCellByXYZ(2,2,20)->printFlowVec();
    std::cout << fluidSim->getCellByXYZ(2,2,20)->outbound_flow[_00p] << std::endl;

    //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    if (!a) {std::cout << "stream>";std::cin >> a;}

    #pragma omp parallel for
    for (int i=0; i<fluidSim->cellArraySize; i++){
        fluidSim->cellArray[i]->stream();
    }

    fluidSim->getCellByXYZ(2,2,20)->printFlowVec();
    std::cout << fluidSim->getCellByXYZ(2,2,20)->outbound_flow[_00p] << std::endl;

    /*this->fluidSim->getCellByXYZ(2,2,5)->debug_info();
    this->fluidSim->getCellByXYZ(2,2,11)->debug_info();*/

    //std::this_thread::sleep_for(std::chrono::milliseconds(10));
    if (!a) {std::cout << "collide>";std::cin >> a;}

    #pragma omp parallel for
    for (int i=0; i<fluidSim->cellArraySize; i++){
        fluidSim->cellArray[i]->collide();
    }

    fluidSim->getCellByXYZ(2,2,20)->printFlowVec();
    std::cout << fluidSim->getCellByXYZ(2,2,20)->outbound_flow[_00p] << std::endl;

    /*this->fluidSim->getCellByXYZ(2,2,5)->debug_info();
    this->fluidSim->getCellByXYZ(2,2,11)->debug_info();*/
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
