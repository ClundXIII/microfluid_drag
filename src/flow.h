#ifndef FLOW_H
#define FLOW_H

#include "define.h"

#if ( _USE_VEMC2 == 1 )
#include <vemc2/core/universe.h>
#endif // _USE_VEMC2

class cell;
class fluid_simulation;

#if ( _USE_VEMC2 == 1 )
class flow : public vemc2::simulation::effect::effect{
#else
class flow : public effect{
#endif
    public:
        #if ( _USE_VEMC2 == 1 )
            flow(fluid_simulation *u);
        #else
            flow(fluid_simulation *u);
        #endif

        void tick();

        void upValues();

        void setCellList(cell **allCells);

        virtual ~flow();
    protected:
    private:

        fluid_simulation *fluidSim;
        cell **allCells;
};

#endif // FLOW_H
