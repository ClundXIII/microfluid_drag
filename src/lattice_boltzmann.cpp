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
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (!a) {std::cout << "apply boundary>";std::cin >> a;}

    for (int i=0; i<fluidSim->cellArraySize; i++){
        fluidSim->cellArray[i]->apply_boundary();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (!a) {std::cout << "stream>";std::cin >> a;}

    for (int i=0; i<fluidSim->cellArraySize; i++){
        fluidSim->cellArray[i]->stream();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (!a) {std::cout << "collide>";std::cin >> a;}

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
