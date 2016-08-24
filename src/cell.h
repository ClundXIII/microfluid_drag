#ifndef CELL_H
#define CELL_H

#include "define.h"

#if ( _USE_VEMC2 == 1 )
#include <vemc2/core/universe.h>
#endif // _USE_VEMC2

class fluid_simulation;

#if ( _USE_VEMC2 == 1 )
class cell : public vemc2::simulation::drawable{
#else
class cell{
#endif // _USE_VEMC2
    public:

        cell(fluid_simulation *u);

        virtual void draw();

        int add_neighbour(direction where, cell *neighbour);

        unsigned pos_x, pos_y, pos_z;

        void debug_info();

        ///simulation functions:
        void collide();
        void apply_boundary();
        void stream();

        void reset_outbound();

        virtual ~cell();

        bdt  inbound_flow[DIRECTION_FLOW_SIZE];
        bdt outbound_flow[DIRECTION_FLOW_SIZE];
    protected:
    private:

        cell **neighbour;

        fluid_simulation* motherU;
};

#endif // CELL_H
