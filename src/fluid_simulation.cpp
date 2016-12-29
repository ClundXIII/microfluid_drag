#include "fluid_simulation.h"
#include "cell.h"
#include "collision.h"
#include "lattice_boltzmann.h"

#include <iostream>

#if ( _USE_VEMC2 == 1 )
    //#define out v_out
    #define out_endl Vesper::LoggingTypes::eom
#else
    #define out std::cout
    #define out_endl std::endl;
#endif // _USE_VEMC2


#if ( _USE_VEMC2 == 1 )
fluid_simulation::fluid_simulation(int cellcount) :
    vemc2::universe(0, cellcount, 1, 1, 1)
    {
    settings.sim.dt = 1;
    unpause();
#else
fluid_simulation::fluid_simulation(){
#endif
    //ctor
}

void fluid_simulation::createCellGrid(int size_x, int size_y, int size_z, cell_type typets, bool create_source_sink){

    switch (typets){
        case boundary_noslip:
        case boundary_freeslip:
            size_x += 2;
            size_y += 2;
            size_z += 2;
          break;
        default:
          break;
    }

    this->size_x = size_x;
    this->size_y = size_y;
    this->size_z = size_z;

    cellArraySize = size_x*size_y*size_z;

    cellArray = new cell*[cellArraySize];

    int cell_pos = 0;
    for (int i=0; i<size_x; i++){
        for (int j=0; j<size_y; j++){
            for (int k=0; k<size_z; k++){

                cellArray[cell_pos] = new cell(this);
                #if ( _USE_VEMC2 == 1 )
                    insertDrawable(cellArray[cell_pos]);
                #endif

                cellArray[cell_pos]->pos_x = i;
                cellArray[cell_pos]->pos_y = j;
                cellArray[cell_pos]->pos_z = k;

                cell_pos++;
            }
        }
    }

    #pragma omp parallel for
    for (int i=0; i<size_x; i++){
        #pragma omp parallel for
        for (int j=0; j<size_y; j++){
            #pragma omp parallel for
            for (int k=0; k<size_z; k++){
                switch (DIRECTION_FLOW_MODEL){

                    case _D3Q19:

                        if (getCellByXYZ(i-1, j-1, k)) getCellByXYZ(i, j, k)->add_neighbour(_mm0, getCellByXYZ(i-1, j-1, k));
                        if (getCellByXYZ(i-1, j+1, k)) getCellByXYZ(i, j, k)->add_neighbour(_mp0, getCellByXYZ(i-1, j+1, k));
                        if (getCellByXYZ(i-1, j, k-1)) getCellByXYZ(i, j, k)->add_neighbour(_m0m, getCellByXYZ(i-1, j, k-1));
                        if (getCellByXYZ(i-1, j, k+1)) getCellByXYZ(i, j, k)->add_neighbour(_m0p, getCellByXYZ(i-1, j, k+1));

                        if (getCellByXYZ(i+1, j-1, k)) getCellByXYZ(i, j, k)->add_neighbour(_pm0, getCellByXYZ(i+1, j-1, k));
                        if (getCellByXYZ(i+1, j+1, k)) getCellByXYZ(i, j, k)->add_neighbour(_pp0, getCellByXYZ(i+1, j+1, k));
                        if (getCellByXYZ(i+1, j, k-1)) getCellByXYZ(i, j, k)->add_neighbour(_p0m, getCellByXYZ(i+1, j, k-1));
                        if (getCellByXYZ(i+1, j, k+1)) getCellByXYZ(i, j, k)->add_neighbour(_p0p, getCellByXYZ(i+1, j, k+1));

                        if (getCellByXYZ(i, j-1, k-1)) getCellByXYZ(i, j, k)->add_neighbour(_0mm, getCellByXYZ(i, j-1, k-1));
                        if (getCellByXYZ(i, j-1, k+1)) getCellByXYZ(i, j, k)->add_neighbour(_0mp, getCellByXYZ(i, j-1, k+1));

                        if (getCellByXYZ(i, j+1, k-1)) getCellByXYZ(i, j, k)->add_neighbour(_0pm, getCellByXYZ(i, j+1, k-1));
                        if (getCellByXYZ(i, j+1, k+1)) getCellByXYZ(i, j, k)->add_neighbour(_0pp, getCellByXYZ(i, j+1, k+1));


                    case _D3Q7:

                        if (getCellByXYZ(i-1, j, k)) getCellByXYZ(i, j, k)->add_neighbour(_m00, getCellByXYZ(i-1, j, k));
                        if (getCellByXYZ(i+1, j, k)) getCellByXYZ(i, j, k)->add_neighbour(_p00, getCellByXYZ(i+1, j, k));
                        if (getCellByXYZ(i, j-1, k)) getCellByXYZ(i, j, k)->add_neighbour(_0m0, getCellByXYZ(i, j-1, k));
                        if (getCellByXYZ(i, j+1, k)) getCellByXYZ(i, j, k)->add_neighbour(_0p0, getCellByXYZ(i, j+1, k));
                        if (getCellByXYZ(i, j, k-1)) getCellByXYZ(i, j, k)->add_neighbour(_00m, getCellByXYZ(i, j, k-1));
                        if (getCellByXYZ(i, j, k+1)) getCellByXYZ(i, j, k)->add_neighbour(_00p, getCellByXYZ(i, j, k+1));

                        getCellByXYZ(i, j, k)->add_neighbour(_000, getCellByXYZ(i, j, k));
                        break;

                    default:
                        out << "wrong flow model!" << out_endl;
                        break;
                }
            }
        }
    }


            for (int i=1; i<(size_x-1); i++){
                for (int j=1; j<(size_y-1); j++){
                    getCellByXYZ(i, j, 1)->outbound_flow[_00p] = 0.5;
                    //getCellByXYZ(i, j, 1)->outbound_flow[_00m] = 0.1;
                }
            }

    switch (typets){
        case boundary_noslip:
        case boundary_freeslip:

            for (int i=0; i<(size_x); i++){
                for (int j=0; j<(size_y); j++){
                    if (create_source_sink){
                        getCellByXYZ(i, j, 0)->type = source;
                        getCellByXYZ(i, j, 0)->outbound_flow[_00p] = 0.1;
                        getCellByXYZ(i, j, size_z-1)->type = sink;
                    }
                    else {
                        getCellByXYZ(i, j, 0)->type = typets;
                        getCellByXYZ(i, j, size_z-1)->type = typets;
                    }
                }
            }
            for (int i=0; i<size_x; i++){
                for (int j=0; j<size_z; j++){
                    getCellByXYZ(i, 0, j)->type = typets;
                    getCellByXYZ(i, size_y-1, j)->type = typets;
                }
            }

            for (int i=0; i<size_y; i++){
                for (int j=0; j<size_z; j++){
                    getCellByXYZ(0, i, j)->type = typets;
                    getCellByXYZ(size_x-1, i, j)->type = typets;
                }
            }
          break;
        default:
          break;
    }

    out << "crated " << cell_pos << " Cells!" << out_endl;
}

void fluid_simulation::setupEffects(){

    effectArray = new effect*[2];
    effectCount = 1;

    effectArray[0] = new lattice_boltzmann(this);
    effectArray[1] = 0;

}

#if !( _USE_VEMC2 == 1 )
//dummy function. does nothing. bugfix for vemc2.
void fluid_simulation::unpause(){}

void fluid_simulation::run(bdt duration){
    for (int i=0; i<duration; i++){
        effectArray[0]->tick();

        effectArray[0]->upValues();
    }
}
#endif

int fluid_simulation::getArrayPosByXYZ(int x, int y, int z){

    if (x >= size_x)
        return -1;

    if (y >= size_y)
        return -2;

    if (z >= size_z)
        return -3;


    if (x < 0)
        return -4;

    if (y < 0)
        return -5;

    if (z < 0)
        return -6;

    int retSum = 0;

    retSum += x * size_y * size_z;
    retSum +=          y * size_z;
    retSum +=                   z;

    return retSum;
}

cell* fluid_simulation::getCellByXYZ(int x, int y, int z){
    if (use_loop_boundary){
        if (x >= size_x)
            x -= size_x;

        if (y >= size_y)
            y -= size_y;

        if (z >= size_z)
            z -= size_z;


        if (x < 0)
            x += size_x;

        if (y < 0)
            y += size_y;

        if (z < 0)
            z += size_z;
    }

    int pos = getArrayPosByXYZ(x, y, z);

    if (pos < 0) return 0;

    return cellArray[pos];
}

void fluid_simulation::print_debug(){
    for (int i=0; i<(size_x*size_y*size_z); i++){
        cellArray[i]->debug_info();
    }
}

fluid_simulation::~fluid_simulation(){
    //dtor
}
