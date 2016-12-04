#ifndef LATTICE_BOLTZMANN_H
#define LATTICE_BOLTZMANN_H

#include "define.h"

#if ( _USE_VEMC2 == 1 )
#include <vemc2/core/universe.h>
#endif // _USE_VEMC2

class cell;
class fluid_simulation;

#if ( _USE_VEMC2 == 1 )
class lattice_boltzmann : public vemc2::simulation::effect::effect{
#else
class lattice_boltzmann : public effect{
#endif
    public:
        #if ( _USE_VEMC2 == 1 )
            lattice_boltzmann(fluid_simulation *u);
        #else
            lattice_boltzmann(fluid_simulation *u);
        #endif

        void tick();

        void upValues();

        void setCellList(cell **allCells);

        virtual ~lattice_boltzmann();
    protected:
    private:

        fluid_simulation *fluidSim;
        cell **allCells;
};



#endif // LATTICE_BOLTZMANN_H
