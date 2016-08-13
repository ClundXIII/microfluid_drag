#ifndef FLOW_H
#define FLOW_H

#include "define.h"

#if ( _USE_VEMC2 == 1 )
#include <vemc2/core/universe.h>
#endif // _USE_VEMC2

class Cell;

#if ( _USE_VEMC2 == 1 )
class flow : vemc2::simulation::effect::effect{
#else
class flow{
#endif
    public:
        #if ( _USE_VEMC2 == 1 )
            flow(vemc2::universe *u);
        #else
            flow();
        #endif

        void setCellList(Cell **allCells);

        virtual ~flow();
    protected:
    private:

        Cell **allCells;
};

#endif // FLOW_H
