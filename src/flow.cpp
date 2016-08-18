#include "flow.h"

#include "cell.h"

#if ( _USE_VEMC2 == 1 )
flow::flow(vemc2::universe *u) : effect(u){
#else
flow::flow(){
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
