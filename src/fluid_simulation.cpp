#include "fluid_simulation.h"
#include "cell.h"
#include "collision.h"
#include "flow.h"

#if ( _USE_VEMC2 == 1 )
    //#define out v_out
    #define out_endl Vesper::LoggingTypes::eom
#else
    #include <iostream>
    #define out std::cout
    #define out_endl std::endl;
#endif // _USE_VEMC2


#if ( _USE_VEMC2 == 1 )
fluid_simulation::fluid_simulation() : vemc2::universe(1){
#else
fluid_simulation::fluid_simulation(){
#endif
    //ctor
}

void fluid_simulation::createCellGrid(unsigned size_x, unsigned size_y, unsigned size_z){

    this->size_x = size_x;
    this->size_y = size_y;
    this->size_z = size_z;

    cellArray = new cell*[size_x*size_y*size_z];

    int cell_pos = 0;
    for (int i=0; i<size_x; i++){
        for (int j=0; j<size_y; j++){
            for (int k=0; k<size_z; k++){

                cellArray[cell_pos] = new cell();
                cellArray[cell_pos]->pos_x = i;
                cellArray[cell_pos]->pos_y = j;
                cellArray[cell_pos]->pos_z = k;

                cell_pos++;
            }
        }
    }

    out << "crated " << size_x*size_y*size_z << " Cells!" << out_endl;
}

void fluid_simulation::setupEffects(){

    effectArray = new effect*[3];
    effectCount = 2;

    effectArray[0] = new collision(this);
    effectArray[1] = new flow(this);
    effectArray[2] = 0;

}

int fluid_simulation::getArrayPosByXYZ(unsigned x, unsigned y, unsigned z){

    if (x >= size_x)
        return -1;

    if (y >= size_y)
        return -2;

    if (z >= size_z)
        return -3;

    int retSum = 0;

    retSum += x * size_y * size_z;
    retSum +=          y * size_z;
    retSum +=                   z;

    return retSum;
}

cell* fluid_simulation::getCellByXYZ(unsigned x, unsigned y, unsigned z){
    int pos = getArrayPosByXYZ(x, y, z);

    if (pos < 0) return 0;

    return cellArray[pos];
}

std::vector<unsigned> fluid_simulation::getXYZbyArrayPos(unsigned aPos){
}

fluid_simulation::~fluid_simulation(){
    //dtor
}
