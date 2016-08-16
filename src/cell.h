#ifndef CELL_H
#define CELL_H

#include "define.h"

#if ( _USE_VEMC2 == 1 )
#include <vemc2/core/universe.h>
#endif // _USE_VEMC2

#if ( _USE_VEMC2 == 1 )
class cell : public vemc2::simulation::drawable{
#else
class cell{
#endif // _USE_VEMC2
    public:

        cell();

        virtual void draw();

        int add_neighbour(direction where, cell *neighbour);

        virtual ~cell();
    protected:
    private:

        bdt flow[DIRECTION_FLOW_SIZE];
        cell **neighbour;
};

#endif // CELL_H
