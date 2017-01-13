#ifndef FLUID_SIMULATION_H
#define FLUID_SIMULATION_H

#include "define.h"

#include "cell.h"

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
        fluid_simulation(int cellcount);

        void createCellGrid(int size_x, int size_y, int size_z, bool periodic_boundaries, cell_type boundary_typets, bool create_source_sink);
        void createCellGrid(int size_x, int size_y, int size_z, bool periodic_boundaries, cell_type boundary_typets, bool create_source_sink, bdt initialFlowVec[]);

        void setupEffects();

        #if !( _USE_VEMC2 == 1 )
        void unpause();
        void run(bdt duration);
        #endif

        cell **cellArray;
        int    cellArraySize;

        void print_debug();

        virtual ~fluid_simulation();

        direction_mode dir_mode = DIRECTION_FLOW_MODEL;

        int getArrayPosByXYZ(int x, int y, int z);
        cell* getCellByXYZ(int x, int y, int z);

        using universe::out;

        int size_x=0, size_y=0, size_z=0;

    protected:

        #if !( _USE_VEMC2 == 1 )
        effect **effectArray;
        int      effectCount;
        #endif

        bool use_loop_boundary=false;

    private:
};

#endif // FLUID_SIMULATION_H
