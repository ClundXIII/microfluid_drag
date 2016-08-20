#include "cell.h"

#include <iostream>

cell::cell(){
    #if ( _USE_VEMC2 == 1 )
        setActive(true);
    #endif

    neighbour = new cell*[DIRECTION_FLOW_SIZE];
}

void cell::draw(){

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

void cell::collide(){

    switch (DIRECTION_FLOW_MODEL){

        case _D3Q18:


            //no break here because we need to do the 6 dirs anyway
        case _D3Q6:

            outbound_flow[_p00] += inbound_flow[_p00];
            outbound_flow[_m00] += inbound_flow[_m00];
            outbound_flow[_0p0] += inbound_flow[_0p0];
            outbound_flow[_0m0] += inbound_flow[_0m0];
            outbound_flow[_00p] += inbound_flow[_00p];
            outbound_flow[_00m] += inbound_flow[_00m];

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

            if (!neighbour[_m00]) outbound_flow[_p00] += outbound_flow[_m00];
            if (!neighbour[_p00]) outbound_flow[_m00] += outbound_flow[_p00];
            if (!neighbour[_0m0]) outbound_flow[_0p0] += outbound_flow[_0m0];
            if (!neighbour[_0p0]) outbound_flow[_0m0] += outbound_flow[_0p0];
            if (!neighbour[_00m]) outbound_flow[_00p] += outbound_flow[_00m];
            if (!neighbour[_00p]) outbound_flow[_00m] += outbound_flow[_00p];

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
}

cell::~cell(){
    //dtor
}
