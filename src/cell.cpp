#include "cell.h"
#include "fluid_simulation.h"

#include "collision.h"

#include <iostream>
#include <chrono>
#include <thread>

#if ( _USE_VEMC2 == 1 )
    #include <vemc2/graphic/draws.h>
#endif

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
    outbound_flow[0] = 1;
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
            vemc2::graphic::draw(motherU, pos_x*3, pos_y*3, pos_z*3, 0.1+abs((this->inbound_flow[0])*5.f));
            glColor3f(1.f, 1.f, 1.f);
        #endif

    }
    else {
        #if ( _USE_VEMC2 == 1 )
            if ((this->type == boundary_noslip)||(this->type == boundary_freeslip))
                glColor3f(0.f, 1.f, 0.f);
            vemc2::graphic::draw(motherU, pos_x*3, pos_y*3, pos_z*3, 0.1+abs((this->inbound_flow[0])*5.f));
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

void cell::debug_info(){
    std::cout << pos_x << " " << pos_y << " " << pos_z << std::endl;
    for (int i = 0; i<DIRECTION_FLOW_SIZE; i++){
        //if (!neighbour[i]) {
        std::cout << "  >" << i << " :" << neighbour[i];
        if (neighbour[i]) std::cout << ":" << neighbour[i]->pos_x << ":" << neighbour[i]->pos_y << ":" << neighbour[i]->pos_z;
        std::cout  << std::endl;
        //}
    }
}

constexpr bdt omega = 2.f/3.f;

void cell::collide(){

    bdt *f_eq_flow;
    f_eq_flow = collision::f_eq(inbound_flow);

    for (int q=0; q<DIRECTION_FLOW_SIZE; q++){
        outbound_flow[q] = inbound_flow[q] + omega * (f_eq_flow[q] - inbound_flow[q]);
    }

    delete[](f_eq_flow);

    last_pressure = 0;

    for (int q=0; q<DIRECTION_FLOW_SIZE; q++){
        last_pressure += outbound_flow[q];
    }

}

void cell::apply_boundary(){

    if (type == boundary_fluid)
        return;

    switch (type){
        case boundary_noslip:

            switch (DIRECTION_FLOW_MODEL){

                case _D3Q19:


                    //no break here because we need to do the 6 dirs anyway
                case _D3Q7:
                    outbound_flow[_p00] = inbound_flow[_m00];
                    outbound_flow[_m00] = inbound_flow[_p00];
                    outbound_flow[_0p0] = inbound_flow[_0m0];
                    outbound_flow[_0m0] = inbound_flow[_0p0];
                    outbound_flow[_00p] = inbound_flow[_00m];
                    outbound_flow[_00m] = inbound_flow[_00p];

                    break;

                default:
                    std::cout << "wrong flow model!" << std::endl;
                    break;
            }
          break;
        case boundary_freeslip:
            std::cout << "boundary_freeslip not implemented" << std::endl;
            exit(32);
          break;
        default:
            std::cout << "unknown boundary in cell.cpp::apply_boundary()" << std::endl;
          break;
    }

}

void cell::stream(){

    if (this->type != boundary_fluid)
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

cell::~cell(){
    //dtor
}
