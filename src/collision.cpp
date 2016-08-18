#include "collision.h"

#include "cell.h"
#include "fluid_simulation.h"

#include <iostream>

#if ( _USE_VEMC2 == 1 )
collision::collision(fluid_simulation *u) : effect(u){
#else
collision::collision(fluid_simulation *u){
#endif
    //ctor
}

void collision::tick(){
    std::cout << "tick c" << std::endl;
}

void collision::upValues(){
    std::cout << "up c" << std::endl;
}

void collision::setCellList(Cell **allCells){
    this->allCells = allCells;
}

collision::~collision()
{
    //dtor
}
