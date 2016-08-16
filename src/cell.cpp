#include "cell.h"

cell::cell(){
    #if ( _USE_VEMC2 == 1 )
        setActive(true);
    #endif

    neighbour = new cell*[DIRECTION_FLOW_SIZE];
}

void cell::draw(){

}

int cell::add_neighbour(direction where, cell *neighbour){

    if (where > DIRECTION_FLOW_SIZE) return -1;
    if (where < 0) return -2;

    this->neighbour[where] = neighbour;

    return 0;
}

cell::~cell(){
    //dtor
}
