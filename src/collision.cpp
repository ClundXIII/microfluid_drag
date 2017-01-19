#include "collision.h"

#include "cell.h"
#include "fluid_simulation.h"

#include <iostream>
#include <math.h>

///Weight Factors
#define _W_0 ((bdt)1/(bdt)3)
#define _W_1 ((bdt)1/(bdt)18)
#define _W_2 ((bdt)1/(bdt)36)

collision::collision(fluid_simulation *u){
    fluidSim = u;
}

void collision::setCellList(cell **allCells){
    this->allCells = allCells;
}

bdt collision::_v[DIRECTION_FLOW_SIZE][3] = {
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
        vec[i] += collision::_v[d][i] * u;
    }

}

int del(int i, int j){
    return (i==j) ? 1:0;
}

bdt collision::w_lq_func(int q){
    if (q==_000) return 1.f/3.f;
    if ((q>=1)          && (q<_size_D3Q7 )) return 1.f/18.f;
    if ((q>=_size_D3Q7) && (q<_size_D3Q19)) return 1.f/36.f;
    return 0;
}

void collision::buildVecFromFlow(bdt *outbound_flow, bdt u[]){
    for (int q=0; q<DIRECTION_FLOW_SIZE; q++){
        buildVecFromDirection(outbound_flow[q], q, u);
    }
}

bdt* collision::f_eq(bdt flow[]){

    bdt rho=0;

    const bdt c_s = 1.f/sqrt(3);

    bdt *retValue = new bdt[DIRECTION_FLOW_SIZE];

    for (int i=0; i<DIRECTION_FLOW_SIZE; i++){
        rho += flow[i];
    }

    bdt u[3] = {0, 0, 0};

    for (int q=0; q<DIRECTION_FLOW_SIZE; q++){
        buildVecFromDirection(flow[q], q, u);
    }

    //std::cout << "u: " << u[0] << ":" << u[1] << ":" << u[2] << std::endl;

    for (int q=0; q<DIRECTION_FLOW_SIZE; q++){

        ///ORIGINAL FORMULAR:
        /*retValue[q] = 1;

        for (int i=0; i<3; i++){

            retValue[q] += (u[i]*_v[q][i]) / (c_s*c_s);

            for (int j=0; j<3; j++){

                retValue[q] += (u[i]*u[j]*_v[q][i]*_v[q][j])/(2.f*c_s*c_s*c_s*c_s);

                retValue[q] -= (u[i]*u[j]*del(i, j))/(2.f*c_s*c_s);

            }
        }

        retValue[q] *= rho * w_lq_func(q);*/


        ///WALBERA FORMULAR:
        retValue[q] = rho;

        for (int i=0; i<3; i++){

            retValue[q] += (u[i]*_v[q][i]) / (c_s*c_s);

            for (int j=0; j<3; j++){

                retValue[q] += (u[i]*u[j]*_v[q][i]*_v[q][j])/(2.f*c_s*c_s*c_s*c_s);

                retValue[q] -= (u[i]*u[j]*del(i, j))/(2.f*c_s*c_s);

            }
        }

        retValue[q] *= w_lq_func(q);

    }

    //std::cout << "0:" << retValue[0] << std::endl;

    return retValue;
}

void collision::buildSourceTerm(bdt newFlow[], bdt inflowVec[], cell *boundaryCell){

    const bdt c_s = 1.f/sqrt(3);

    for (int q=0; q<DIRECTION_FLOW_SIZE; q++){
        newFlow[q] = 0;

        for (int s=0; s<3; s++){
            ///-1 weil antiq und so
            newFlow[q] += ((-1) * _v[q][s]) * inflowVec[s];
        }

        newFlow[q] *= - (2 * w_lq_func(q) / (c_s * c_s) ) * ( 1/* + boundaryCell->collideRho */);
    }
}

collision::~collision()
{
    //dtor
}
