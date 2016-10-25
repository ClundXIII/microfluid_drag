#include "collision.h"

#include "cell.h"
#include "fluid_simulation.h"

#include <iostream>


///Weight Factors
#define _W_0 ((bdt)1/(bdt)3)
#define _W_1 ((bdt)1/(bdt)18)
#define _W_2 ((bdt)1/(bdt)36)

#if ( _USE_VEMC2 == 1 )
collision::collision(fluid_simulation *u) : effect(u){
#else
collision::collision(fluid_simulation *u){
#endif
    fluidSim = u;
}

void collision::tick(){
    for (int i=0; i<fluidSim->cellArraySize; i++){
        fluidSim->cellArray[i]->collide();
        fluidSim->cellArray[i]->apply_boundary();
    }
}

void collision::upValues(){
}

void collision::setCellList(Cell **allCells){
    this->allCells = allCells;
}

bdt _v[DIRECTION_FLOW_SIZE][3] = {
    { 0,  0,  0},
    { 1,  0,  0},
    {-1,  0,  0},
    { 0,  1,  0},
    { 0, -1,  0},
    { 0,  0,  1},
    { 0,  0, -1}
    #if (DIRECTION_FLOW_SIZE > 7)
    ,
    { 1,  1,  0},
    { 1, -1,  0},
    { 1,  0,  1},
    { 1,  0, -1},
    { 0,  1,  1},
    { 0,  1, -1},
    {-1,  1,  0},
    {-1, -1,  0},
    {-1,  0,  1},
    {-1,  0, -1},
    { 0, -1,  1},
    { 0, -1, -1}
    #endif
};

const bdt sqrt_2 = sqrt(2);

void buildVecFromDirection(bdt u, int d, bdt *vec){

    for (int i=0; i<3; i++){
        vec[i] += _v[d][i] * u;
    }

    /*switch (d){

        case _000:
            break;
        case _p00:
            vec[0] +=  u;
            break;
        case _m00:
            vec[0] += -u;
            break;
        case _0p0:
            vec[1] +=  u;
            break;
        case _0m0:
            vec[1] += -u;
            break;
        case _00p:
            vec[2] +=  u;
            break;
        case _00m:
            vec[2] += -u;
            break;
        //
        case _pp0:
            vec[0] += sqrt_2*u;
            vec[1] += sqrt_2*u;
            break;
        case _pm0:
            vec[0] += sqrt_2*u;
            vec[1] -= sqrt_2*u;
            break;
        case _p0p:

            break;
        case _p0m:

            break;
        case _0pp:

            break;
        case _0pm:

            break;
        case _mp0:

            break;
        case _mm0:

            break;
        case _m0p:

            break;
        case _m0m:

            break;
        case _0mp:

            break;
        case _0mm:

            break;

        default:
            std::cout << "direction not recognized in collision.cpp::buildVecFromDirection" << std::endl;
            break;
    }*/

}

int del(int i, int j){
    /*if (i==j)
        return 1;
    else
        return 0;*/
    return (i==j)?1:0;
}

bdt w_lq_func(int q){
    if (q==_000) return 1.d/3.d;
    if ((q>=1)          && (q<_size_D3Q7 )) return 1.d/18.d;
    if ((q>=_size_D3Q7) && (q<_size_D3Q19)) return 1.d/36.d;
    return 0;
}

bdt* collision::f_eq(bdt flow[]){

    bdt rho=0;

    const bdt c_s = 1.f/sqrt(3);

    bdt *retValue = new bdt[DIRECTION_FLOW_SIZE];

    for (int i=0; i<DIRECTION_FLOW_SIZE; i++){
        rho += flow[i];
    }

    bdt u[3] = {0, 0, 0};

    //std::cout << rho << std::endl;

    for (int q=(DIRECTION_FLOW_SIZE-1); q>0; q--){
        buildVecFromDirection(flow[q]/*/rho*/, q, u);
    }

    for (int q=(DIRECTION_FLOW_SIZE-1); q>0; q--){

        retValue[q] = 1;

        for (int i=0; i<3; i++){

            retValue[q] += (u[i]*_v[q][i]) / (c_s*c_s);

            for (int j=0; j<3; j++){

                /*retValue[q] += ( (u[i]*u[j])/(2.f*c_s*c_s) ) *
                            ( (_v[q][i]*_v[q][j]) / (c_s*c_s) - del(i, j) );*/

                retValue[q] += (u[i]*u[j]*_v[q][i]*_v[q][j])/(2.f*c_s*c_s*c_s*c_s);

                retValue[q] -= (u[i]*u[j]*del(i, j))/(2.f*c_s*c_s);

                //if (retValue[q] != 1) std::cout << "q: " << q << ", i:" << i << ", j:" << j << ", retValue[q]:" << retValue[q] << std::endl;
            }
        }

        retValue[q] *= rho * w_lq_func(q);

        //if (retValue[q] != 0){
        //    std::cout << retValue[q] << std::endl;
        //}
    }

    retValue[0] = 0;

    return retValue;
}

collision::~collision()
{
    //dtor
}
