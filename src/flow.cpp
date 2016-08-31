#include "flow.h"

#include "cell.h"
#include "fluid_simulation.h"

#include <iostream>

#if ( _USE_VEMC2 == 1 )
flow::flow(fluid_simulation *u) : effect(u){
#else
flow::flow(fluid_simulation *u){
#endif
    fluidSim = u;
}

void flow::tick(){
}

void flow::upValues(){
    for (int i=0; i<fluidSim->cellArraySize; i++){
        fluidSim->cellArray[i]->stream();
    }
    for (int i=0; i<fluidSim->cellArraySize; i++){
        fluidSim->cellArray[i]->reset_outbound();
    }
}

void flow::setCellList(cell **allCells){
    this->allCells = allCells;
}

flow::~flow()
{
    //dtor
}
