#ifndef COLLISION_H
#define COLLISION_H

#include "define.h"

#if ( _USE_VEMC2 == 1 )
#include <vemc2/core/universe.h>
#endif // _USE_VEMC2

class Cell;
class fluid_simulation;

#if ( _USE_VEMC2 == 1 )
class collision : public vemc2::simulation::effect::effect{
#else
class collision : public effect{
#endif
    public:

        collision(fluid_simulation *u);

        void tick();

        void upValues();

        void setCellList(Cell **allCells);

        virtual ~collision();
    protected:
    private:

        fluid_simulation *fluidSim;
        Cell **allCells;
};

#endif // COLLISION_H
