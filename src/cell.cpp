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
}

void cell::draw(){

    //if (last_pressure != 0)
    //    std::cout << "last pressure: " << last_pressure << std::endl;

    #if ( _USE_VEMC2 == 1 )
        vemc2::graphic::draw(motherU, pos_x*3, pos_y*3, pos_z*3, 0.1+last_pressure);
    #endif
}

int cell::add_neighbour(direction where, cell *neighbour){

    if (where > DIRECTION_FLOW_SIZE) return -1;
    if (where < 0) return -2;

    this->neighbour[where] = neighbour;

    return 0;
}

void cell::debug_info(){
    std::cout << pos_x << " " << pos_y << " " << pos_z << std::endl;
    for (int i = 0; i<DIRECTION_FLOW_SIZE; i++){
        std::cout << "  >" << i << " :" << neighbour[i];
        if (neighbour[i]) std::cout << ":" << neighbour[i]->pos_x << ":" << neighbour[i]->pos_y << ":" << neighbour[i]->pos_z;
        std::cout  << std::endl;
    }
}

bdt omega = .25/.3;

void cell::collide(){

    switch (DIRECTION_FLOW_MODEL){

        case _D3Q18:


            //no break here because we need to do the 6 dirs anyway
        case _D3Q6:

            bdt *f_eq_flow;
            f_eq_flow = collision::f_eq(inbound_flow);
/*
            outbound_flow[_p00] += inbound_flow[_p00];
            outbound_flow[_m00] += inbound_flow[_m00];
            outbound_flow[_0p0] += inbound_flow[_0p0];
            outbound_flow[_0m0] += inbound_flow[_0m0];
            outbound_flow[_00p] += inbound_flow[_00p];
            outbound_flow[_00m] += inbound_flow[_00m];
*/
            outbound_flow[_p00] += inbound_flow[_p00] + omega * (f_eq_flow[_p00] - inbound_flow[_p00]);
            outbound_flow[_m00] += inbound_flow[_m00] + omega * (f_eq_flow[_m00] - inbound_flow[_m00]);
            outbound_flow[_0p0] += inbound_flow[_0p0] + omega * (f_eq_flow[_0p0] - inbound_flow[_0p0]);
            outbound_flow[_0m0] += inbound_flow[_0m0] + omega * (f_eq_flow[_0m0] - inbound_flow[_0m0]);
            outbound_flow[_00p] += inbound_flow[_00p] + omega * (f_eq_flow[_00p] - inbound_flow[_00p]);
            outbound_flow[_00m] += inbound_flow[_00m] + omega * (f_eq_flow[_00m] - inbound_flow[_00p]);

            delete[](f_eq_flow);

            last_pressure = 0;
            last_pressure += outbound_flow[_p00];
            last_pressure += outbound_flow[_m00];
            last_pressure += outbound_flow[_0p0];
            last_pressure += outbound_flow[_0m0];
            last_pressure += outbound_flow[_00p];
            last_pressure += outbound_flow[_00m];

            break;

        default:
            std::cout << "wrong flow model!" << std::endl;
            break;
    }

}

void cell::apply_boundary(){
    switch (DIRECTION_FLOW_MODEL){

        case _D3Q18:


            //no break here because we need to do the 6 dirs anyway
        case _D3Q6:

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

            break;

        default:
            std::cout << "wrong flow model!" << std::endl;
            break;
    }
}

void cell::stream(){
    switch (DIRECTION_FLOW_MODEL){

        case _D3Q18:


            //no break here because we need to do the 6 dirs anyway
        case _D3Q6:

            if (neighbour[_m00]) inbound_flow[_p00] = neighbour[_m00]->outbound_flow[_p00];
            if (neighbour[_p00]) inbound_flow[_m00] = neighbour[_p00]->outbound_flow[_m00];
            if (neighbour[_0m0]) inbound_flow[_0p0] = neighbour[_0m0]->outbound_flow[_0p0];
            if (neighbour[_0p0]) inbound_flow[_0m0] = neighbour[_0p0]->outbound_flow[_0m0];
            if (neighbour[_00m]) inbound_flow[_00p] = neighbour[_00m]->outbound_flow[_00p];
            if (neighbour[_00p]) inbound_flow[_00m] = neighbour[_00p]->outbound_flow[_00m];

            break;

        default:
            std::cout << "wrong flow model!" << std::endl;
            break;
    }
}

void cell::reset_outbound(){
    for (unsigned i=0; i<DIRECTION_FLOW_SIZE; i++){
        outbound_flow[i] = 0;
    }


    if ((pos_x==5)&&(pos_y==5)&&(pos_z==5)){
        //std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << ">";
        int a;
        std::cin >> a;
    }

}

cell::~cell(){
    //dtor
}
