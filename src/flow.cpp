#include "flow.h"

#include "cell.h"
#include "fluid_simulation.h"

#include <iostream>

#if ( _USE_VEMC2 == 1 )
flow::flow(fluid_simulation *u) : effect(u){
#else
flow::flow(fluid_simulation *u){
#endif
    //ctor
}

void flow::tick(){
    std::cout << "tick f" << std::endl;
}

void flow::upValues(){
    std::cout << "up f" << std::endl;
}

void flow::setCellList(cell **allCells){
    this->allCells = allCells;
}

flow::~flow()
{
    //dtor
}
