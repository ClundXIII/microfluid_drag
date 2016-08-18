#ifndef FLUID_SIMULATION_H
#define FLUID_SIMULATION_H

#include "define.h"

#include <vector>

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

        void createCellGrid(unsigned size_x, unsigned size_y, unsigned size_z);

        void setupEffects();

        cell **cellArray;

        int a;

        virtual ~fluid_simulation();

    protected:

        int getArrayPosByXYZ(unsigned x, unsigned y, unsigned z);
        cell* getCellByXYZ(unsigned x, unsigned y, unsigned z);
        std::vector<unsigned> getXYZbyArrayPos(unsigned aPos);

        unsigned size_x=0, size_y=0, size_z=0;

        #if !( _USE_VEMC2 == 1 )
            effect **effectArray;
            int      effectCount;
        #endif

    private:
};

#endif // FLUID_SIMULATION_H
