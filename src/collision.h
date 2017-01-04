#ifndef COLLISION_H
#define COLLISION_H

#include "define.h"

#if ( _USE_VEMC2 == 1 )
#include <vemc2/core/universe.h>
#endif // _USE_VEMC2

class cell;
class fluid_simulation;

class collision {
    public:

        collision(fluid_simulation *u);

        void setCellList(cell **allCells);

        static bdt* f_eq(bdt flow[]);

        //static std::vector<bdt> getFlowVector(bdt flow[]);

        static void buildVecFromFlow(bdt *outbound_flow, bdt u[]);

        static void buildSourceTerm(bdt newFlow[], bdt inflowVec[], cell *boundaryCell);

        virtual ~collision();
    protected:
    private:

        fluid_simulation *fluidSim;
        cell **allCells;
};

#endif // COLLISION_H
