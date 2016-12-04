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

    inbound_flow[0] = 1;
    outbound_flow[0] = 0;
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
            vemc2::graphic::draw(motherU, pos_x*3, pos_y*3, pos_z*3, 0.1+abs((last_pressure-1)*10.f));
            //vemc2::graphic::draw(motherU, pos_x*3, pos_y*3, pos_z*3, 0.1+abs((inbound_flow[0]-1)));
            glColor3f(1.f, 1.f, 1.f);
        #endif

    }
    else {
        #if ( _USE_VEMC2 == 1 )
            vemc2::graphic::draw(motherU, pos_x*3, pos_y*3, pos_z*3, 0.1+abs((last_pressure-1)*10.f));
            //vemc2::graphic::draw(motherU, pos_x*3, pos_y*3, pos_z*3, 0.1+abs((inbound_flow[0]-1)));
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
    switch (DIRECTION_FLOW_MODEL){

        case _D3Q19:


            //no break here because we need to do the 6 dirs anyway
        case _D3Q7:
            /*
            if (!neighbour[_m00]) {
                    outbound_flow[_p00] += outbound_flow[_m00];
                    outbound_flow[_m00] = 0;
            }
            if (!neighbour[_p00]) {
                    outbound_flow[_m00] += outbound_flow[_p00];
                    outbound_flow[_p00] = 0;
            }
            if (!neighbour[_0m0]) {
                    outbound_flow[_0p0] += outbound_flow[_0m0];
                    outbound_flow[_0m0] = 0;
            }
            if (!neighbour[_0p0]) {
                    outbound_flow[_0m0] += outbound_flow[_0p0];
                    outbound_flow[_0p0] = 0;
            }
            if (!neighbour[_00m]) {
                    outbound_flow[_00p] += outbound_flow[_00m];
                    outbound_flow[_00m] = 0;
            }
            if (!neighbour[_00p]) {
                    outbound_flow[_00m] += outbound_flow[_00p];
                    outbound_flow[_00p] = 0;
            }
*/
            break;

        default:
            std::cout << "wrong flow model!" << std::endl;
            break;
    }
}

void cell::stream(){
    /*for (unsigned i=0; i<DIRECTION_FLOW_SIZE; i++){
        inbound_flow[i] = 0;
    }*/
    switch (DIRECTION_FLOW_MODEL){

        case _D3Q19:

            if (neighbour[_mm0]) inbound_flow[_pp0] = neighbour[_mm0]->outbound_flow[_pp0];
            if (neighbour[_mp0]) inbound_flow[_pm0] = neighbour[_mp0]->outbound_flow[_pm0];
            if (neighbour[_m0m]) inbound_flow[_p0p] = neighbour[_m0m]->outbound_flow[_p0p];
            if (neighbour[_m0p]) inbound_flow[_p0m] = neighbour[_m0p]->outbound_flow[_p0m];

            if (neighbour[_pm0]) inbound_flow[_mp0] = neighbour[_pm0]->outbound_flow[_mp0];
            if (neighbour[_pp0]) inbound_flow[_mm0] = neighbour[_pp0]->outbound_flow[_mm0];
            if (neighbour[_p0m]) inbound_flow[_m0p] = neighbour[_p0m]->outbound_flow[_m0p];
            if (neighbour[_p0p]) inbound_flow[_m0m] = neighbour[_p0p]->outbound_flow[_m0m];

            if (neighbour[_0mm]) inbound_flow[_0pp] = neighbour[_0mm]->outbound_flow[_0pp];
            if (neighbour[_0mp]) inbound_flow[_0pm] = neighbour[_0mp]->outbound_flow[_0pm];

            if (neighbour[_0pm]) inbound_flow[_0mp] = neighbour[_0pm]->outbound_flow[_0mp];
            if (neighbour[_0pp]) inbound_flow[_0mm] = neighbour[_0pp]->outbound_flow[_0mm];

            //no break here because we need to do the 6 dirs anyway
        case _D3Q7:

            if (neighbour[_m00]) inbound_flow[_p00] = neighbour[_m00]->outbound_flow[_p00];
            if (neighbour[_p00]) inbound_flow[_m00] = neighbour[_p00]->outbound_flow[_m00];
            if (neighbour[_0m0]) inbound_flow[_0p0] = neighbour[_0m0]->outbound_flow[_0p0];
            if (neighbour[_0p0]) inbound_flow[_0m0] = neighbour[_0p0]->outbound_flow[_0m0];
            if (neighbour[_00m]) inbound_flow[_00p] = neighbour[_00m]->outbound_flow[_00p];
            if (neighbour[_00p]) inbound_flow[_00m] = neighbour[_00p]->outbound_flow[_00m];

            //if (neighbour[_000]) inbound_flow[_000] = neighbour[_000]->outbound_flow[_000]; //aka this
            inbound_flow[_000] = outbound_flow[_000];

            break;

        default:
            std::cout << "wrong flow model!" << std::endl;
            break;
    }
}

void cell::reset_outbound(){

    for (unsigned i=0; i<DIRECTION_FLOW_SIZE; i++){
        std::cout << i << ":" << outbound_flow[i] << std::endl;
    }

    for (unsigned i=0; i<DIRECTION_FLOW_SIZE; i++){
        outbound_flow[i] = 0;
    }
}

cell::~cell(){
    //dtor
}
