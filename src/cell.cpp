#include "cell.h"
#include "fluid_simulation.h"

#include "collision.h"

#include <iostream>
#include <chrono>
#include <thread>
#include <math.h>

#if ( _USE_VEMC2 == 1 )
    #include <vemc2/graphic/draws.h>
#endif

bdt cell::initial_flow = 1;

cell::cell(fluid_simulation *u){
    #if ( _USE_VEMC2 == 1 )
        setActive(true);
    #endif
    motherU = u;
    neighbour = new cell*[DIRECTION_FLOW_SIZE];
    for (int q=0; q<DIRECTION_FLOW_SIZE; q++){
        neighbour[q] = 0;
    }

    inbound_flow[0] = 0;
    outbound_flow[0] = initial_flow;
    for (int q=1; q<DIRECTION_FLOW_SIZE; q++){
        inbound_flow[q] = 0;
        outbound_flow[q] = 0;
    }
}

void cell::draw(){

    //if (last_pressure != 0){std::cout << "last pressure: " << last_pressure << std::endl;}

    //if ((inbound_flow[0]-1) < 0){
    if (last_pressure < 0){
        //std::cout << "last pressure: " << last_pressure << std::endl;
        #if ( _USE_VEMC2 == 1 )
            glColor3f(1.f, 0.f, 0.f);
            //vemc2::graphic::draw(motherU, pos_x*3, pos_y*3, pos_z*3, 0.1+abs((this->inbound_flow[0])*5.f));
            vemc2::graphic::draw(motherU, pos_x*3, pos_y*3, pos_z*3, 0.1+abs((this->last_pressure)*5.f));
            glColor3f(1.f, 1.f, 1.f);
        #endif

    }
    else {
        #if ( _USE_VEMC2 == 1 )
            if ((this->type == boundary_noslip)||(this->type == boundary_freeslip))
                glColor3f(0.f, 1.f, 0.f);
            //vemc2::graphic::draw(motherU, pos_x*3, pos_y*3, pos_z*3, 0.1+abs((this->inbound_flow[0])*5.f));
            vemc2::graphic::draw(motherU, pos_x*3, pos_y*3, pos_z*3, 0.1+abs((this->last_pressure)*5.f));
            if ((this->type == boundary_noslip)||(this->type == boundary_freeslip))
                glColor3f(1.f, 1.f, 1.f);
        #endif
    }
}

int cell::add_neighbour(direction where, cell *neighbour){

    if (where > DIRECTION_FLOW_SIZE) { std::cout << "error -1" << std::endl; return -1;}
    if (where < 0) { std::cout << "error -2" << std::endl; return -2;}

    this->neighbour[where] = neighbour;

    return 0;
}

cell* cell::get_neighbour(direction where){
    return neighbour[where];
}

void cell::debug_info(){
    std::cout << pos_x << " " << pos_y << " " << pos_z << std::endl;
    for (int i = 0; i<DIRECTION_FLOW_SIZE; i++){
        //if (!neighbour[i]) {
        std::cout << "  >" << i << " :" << neighbour[i];
        if (neighbour[i]) std::cout << ":" << neighbour[i]->pos_x << ":" << neighbour[i]->pos_y << ":" << neighbour[i]->pos_z << ":flow-in:" << inbound_flow[i] << ":flow-out:" <<outbound_flow[i];
        std::cout  << std::endl;
        //}
    }
}

constexpr bdt omega = 2.f/3.f;

void cell::collide(){

    if (type != fluid)
        return;

    bdt *f_eq_flow;
    f_eq_flow = collision::f_eq(inbound_flow);

    for (int q=0; q<DIRECTION_FLOW_SIZE; q++){
        outbound_flow[q] = inbound_flow[q] + omega * (f_eq_flow[q] - inbound_flow[q]);
    }

    delete[](f_eq_flow);

    last_pressure = 0;

    for (int q=1; q<DIRECTION_FLOW_SIZE; q++){
        last_pressure += outbound_flow[q];
    }

    collideRho = 0;
    for (int q=0; q<DIRECTION_FLOW_SIZE; q++){
        collideRho += outbound_flow[q];
    }
}

void cell::apply_boundary(){

    if (type == fluid) return;

    for (int i=0; i<DIRECTION_FLOW_SIZE; i++)
        outbound_flow[i] = 0;

    if ((type == source) ||
        (type == sink) ||
        (type == boundary_noslip)){

        switch (DIRECTION_FLOW_MODEL){

            case _D3Q19:

                if (neighbour[_pp0]) outbound_flow[_pp0] += neighbour[_pp0]->outbound_flow[_mm0];
                if (neighbour[_pm0]) outbound_flow[_pm0] += neighbour[_pm0]->outbound_flow[_mp0];
                if (neighbour[_p0p]) outbound_flow[_p0p] += neighbour[_p0p]->outbound_flow[_m0m];
                if (neighbour[_p0m]) outbound_flow[_p0m] += neighbour[_p0m]->outbound_flow[_m0p];
                if (neighbour[_0pp]) outbound_flow[_0pp] += neighbour[_0pp]->outbound_flow[_0mm];
                if (neighbour[_0pm]) outbound_flow[_0pm] += neighbour[_0pm]->outbound_flow[_0mp];

                if (neighbour[_mp0]) outbound_flow[_mp0] += neighbour[_mp0]->outbound_flow[_pm0];
                if (neighbour[_mm0]) outbound_flow[_mm0] += neighbour[_mm0]->outbound_flow[_pp0];
                if (neighbour[_m0p]) outbound_flow[_m0p] += neighbour[_m0p]->outbound_flow[_p0m];
                if (neighbour[_m0m]) outbound_flow[_m0m] += neighbour[_m0m]->outbound_flow[_p0p];
                if (neighbour[_0mp]) outbound_flow[_0mp] += neighbour[_0mp]->outbound_flow[_0pm];
                if (neighbour[_0mm]) outbound_flow[_0mm] += neighbour[_0mm]->outbound_flow[_0pp];

                //no break here because we need to do the 6 dirs anyway
            case _D3Q7:
                if (neighbour[_p00]) outbound_flow[_p00] += neighbour[_p00]->outbound_flow[_m00];
                if (neighbour[_m00]) outbound_flow[_m00] += neighbour[_m00]->outbound_flow[_p00];
                if (neighbour[_0p0]) outbound_flow[_0p0] += neighbour[_0p0]->outbound_flow[_0m0];
                if (neighbour[_0m0]) outbound_flow[_0m0] += neighbour[_0m0]->outbound_flow[_0p0];
                if (neighbour[_00p]) outbound_flow[_00p] += neighbour[_00p]->outbound_flow[_00m];
                if (neighbour[_00m]) outbound_flow[_00m] += neighbour[_00m]->outbound_flow[_00p];

                break;

            default:
                std::cout << "wrong flow model!" << std::endl;
                break;
        }
    }

    if ((type == source) ||
        (type == sink)){

        bdt *tmpOutboundFlow = new bdt[DIRECTION_FLOW_SIZE];

        collision::buildSourceTerm(tmpOutboundFlow, inflowVec, this);

        for (int i=0; i<DIRECTION_FLOW_SIZE; i++){
            outbound_flow[i] += tmpOutboundFlow[i];
        }

        delete[] tmpOutboundFlow;
    }

}

void cell::stream(){

    if (this->type != fluid)
        return;

    switch (DIRECTION_FLOW_MODEL){

        case _D3Q19:

            inbound_flow[_pp0] = neighbour[_mm0]->outbound_flow[_pp0];
            inbound_flow[_pm0] = neighbour[_mp0]->outbound_flow[_pm0];
            inbound_flow[_p0p] = neighbour[_m0m]->outbound_flow[_p0p];
            inbound_flow[_p0m] = neighbour[_m0p]->outbound_flow[_p0m];

            inbound_flow[_mp0] = neighbour[_pm0]->outbound_flow[_mp0];
            inbound_flow[_mm0] = neighbour[_pp0]->outbound_flow[_mm0];
            inbound_flow[_m0p] = neighbour[_p0m]->outbound_flow[_m0p];
            inbound_flow[_m0m] = neighbour[_p0p]->outbound_flow[_m0m];

            inbound_flow[_0pp] = neighbour[_0mm]->outbound_flow[_0pp];
            inbound_flow[_0pm] = neighbour[_0mp]->outbound_flow[_0pm];

            inbound_flow[_0mp] = neighbour[_0pm]->outbound_flow[_0mp];
            inbound_flow[_0mm] = neighbour[_0pp]->outbound_flow[_0mm];

            //no break here because we need to do the 6 dirs anyway
        case _D3Q7:

            inbound_flow[_p00] = neighbour[_m00]->outbound_flow[_p00];
            inbound_flow[_m00] = neighbour[_p00]->outbound_flow[_m00];
            inbound_flow[_0p0] = neighbour[_0m0]->outbound_flow[_0p0];
            inbound_flow[_0m0] = neighbour[_0p0]->outbound_flow[_0m0];
            inbound_flow[_00p] = neighbour[_00m]->outbound_flow[_00p];
            inbound_flow[_00m] = neighbour[_00p]->outbound_flow[_00m];

            //if (neighbour[_000]) inbound_flow[_000] = neighbour[_000]->outbound_flow[_000]; //aka this
            inbound_flow[_000] = outbound_flow[_000];

            break;

        default:
            std::cout << "wrong flow model!" << std::endl;
            break;
    }
}

void cell::printFlowVec(){

    bdt u[3] = {0, 0, 0};

    collision::buildVecFromFlow(outbound_flow, u);

    std::cout << u[0] << " " << u[1] << " " << u[2] << std::endl;
}


bdt cell::getFlowVecX(){

    bdt u[3] = {0, 0, 0};

    collision::buildVecFromFlow(outbound_flow, u);

    return u[0];
}

bdt cell::getFlowVecY(){

    bdt u[3] = {0, 0, 0};

    collision::buildVecFromFlow(outbound_flow, u);

    return u[1];
}

bdt cell::getFlowVecZ(){

    bdt u[3] = {0, 0, 0};

    collision::buildVecFromFlow(outbound_flow, u);

    return u[2];
}

bdt cell::getFlowVecAbs(){

    bdt u[3] = {0, 0, 0};

    collision::buildVecFromFlow(outbound_flow, u);

    return sqrt(u[0]*u[0]+u[1]*u[1]+u[2]*u[2]);
}

void cell::buildDragForce(bdt toBuild[]){

    if (!solid_object)
        return;

    const bdt c_s = 1.f/sqrt(3);

    for (int r=0; r<3; r++){
        for (int q=0; q<DIRECTION_FLOW_SIZE ; q++){

            if (!neighbour[q])
                continue;

            bdt tmpForce = 0;

            tmpForce += neighbour[q]->outbound_flow[q];

            bdt tmpA = 0;
            for (int i=0; i<3; i++){
                tmpA += collision::_v[q][i]*this->inflowVec[i];
            }
            tmpForce -= (collision::w_lq_func(q) / (c_s*c_s)) *
                        (1/* + neighbour[q]->collideRho*/)*tmpA;
            tmpForce *= 2 * collision::_v[q][r];

            toBuild[r] += tmpForce;
        }
    }

}

cell::~cell(){
    //dtor
}
