#include "collision.h"

#include "cell.h"
#include "fluid_simulation.h"

#include <iostream>

#if ( _USE_VEMC2 == 1 )
collision::collision(fluid_simulation *u) : effect(u){
#else
collision::collision(fluid_simulation *u){
#endif
    fluidSim = u;
}

void collision::tick(){
    for (int i=0; i<fluidSim->cellArraySize; i++){
        fluidSim->cellArray[i]->collide();
        fluidSim->cellArray[i]->apply_boundary();
    }
}

void collision::upValues(){
}

void collision::setCellList(Cell **allCells){
    this->allCells = allCells;
}

collision::~collision()
{
    //dtor
}
