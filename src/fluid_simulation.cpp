#include "fluid_simulation.h"
#include "cell.h"
#include "collision.h"
#include "flow.h"

#include <iostream>

#if ( _USE_VEMC2 == 1 )
    //#define out v_out
    #define out_endl Vesper::LoggingTypes::eom
#else
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

    cellArraySize = size_x*size_y*size_z;

    cellArray = new cell*[cellArraySize];

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

    for (int i=0; i<size_x; i++){
        for (int j=0; j<size_y; j++){
            for (int k=0; k<size_z; k++){
                switch (DIRECTION_FLOW_MODEL){

                    case _D3Q18:


                    //no break here because we need to do the 6 dirs anyway
                    case _D3Q6:

                        if (getCellByXYZ(i-1, j, k)) getCellByXYZ(i, j, k)->add_neighbour(_m00, getCellByXYZ(i-1, j, k));
                        if (getCellByXYZ(i+1, j, k)) getCellByXYZ(i, j, k)->add_neighbour(_p00, getCellByXYZ(i+1, j, k));
                        if (getCellByXYZ(i, j-1, k)) getCellByXYZ(i, j, k)->add_neighbour(_0m0, getCellByXYZ(i, j-1, k));
                        if (getCellByXYZ(i, j+1, k)) getCellByXYZ(i, j, k)->add_neighbour(_0p0, getCellByXYZ(i, j+1, k));
                        if (getCellByXYZ(i, j, k-1)) getCellByXYZ(i, j, k)->add_neighbour(_00m, getCellByXYZ(i, j, k-1));
                        if (getCellByXYZ(i, j, k+1)) getCellByXYZ(i, j, k)->add_neighbour(_00p, getCellByXYZ(i, j, k+1));
                        break;

                    default:
                        out << "wrong flow model!" << out_endl;
                        break;
                }
            }
        }
    }

    out << "crated " << cell_pos << " Cells!" << out_endl;
}

void fluid_simulation::setupEffects(){

    effectArray = new effect*[3];
    effectCount = 2;

    effectArray[0] = new collision(this);
    effectArray[1] = new flow(this);
    effectArray[2] = 0;

}

#if !( _USE_VEMC2 == 1 )
//dummy function. does nothing. bugfix for vemc2.
void fluid_simulation::unpause(){}

void fluid_simulation::run(bdt duration){
    for (int i=0; i<duration; i++){
        effectArray[0]->tick();
        effectArray[1]->tick();

        effectArray[0]->upValues();
        effectArray[1]->upValues();
    }
}
#endif

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

    std::cout << "not implemented" << std::endl;

}

void fluid_simulation::print_debug(){
    for (int i=0; i<(size_x*size_y*size_z); i++){
        cellArray[i]->debug_info();
    }
}

fluid_simulation::~fluid_simulation(){
    //dtor
}
