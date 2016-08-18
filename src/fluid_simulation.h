#ifndef FLUID_SIMULATION_H
#define FLUID_SIMULATION_H

#include "define.h"

#if ( _USE_VEMC2 == 1 )
#include <vemc2/core/universe.h>
#endif // _USE_VEMC2

class cell;

#if ( _USE_VEMC2 == 1 )
class fluid_simulation : public vemc2::universe{
#else
class fluid_simulation{
#endif // _USE_VEMC2
    public:
        fluid_simulation();
        virtual ~fluid_simulation();

        cell **cellArray;

        int a;
    protected:
    private:
};

#endif // FLUID_SIMULATION_H
