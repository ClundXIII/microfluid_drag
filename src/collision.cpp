#include "collision.h"

#include "cell.h"

#if ( _USE_VEMC2 == 1 )
collision::collision(vemc2::universe *u) : effect(u){
#else
collision::collision(){
#endif
    //ctor
}

void collision::tick(){
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
