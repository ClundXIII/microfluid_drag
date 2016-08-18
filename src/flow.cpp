#include "flow.h"

#include "cell.h"

#if ( _USE_VEMC2 == 1 )
flow::flow(fluid_simulation *u) : effect(u){
#else
flow::flow(fluid_simulation *u){
#endif
    //ctor
}

void flow::tick(){
}

void flow::upValues(){
}

void flow::setCellList(cell **allCells){
    this->allCells = allCells;
}

flow::~flow()
{
    //dtor
}
