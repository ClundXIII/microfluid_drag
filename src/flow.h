#ifndef FLOW_H
#define FLOW_H

#include "define.h"

#if ( _USE_VEMC2 == 1 )
#include <vemc2/core/universe.h>
#endif // _USE_VEMC2

class cell;

#if ( _USE_VEMC2 == 1 )
class flow : public vemc2::simulation::effect::effect{
#else
class flow : public effect{
#endif
    public:
        #if ( _USE_VEMC2 == 1 )
            flow(vemc2::universe *u);
        #else
            flow();
        #endif

        void tick();

        void upValues();

        void setCellList(cell **allCells);

        virtual ~flow();
    protected:
    private:

        cell **allCells;
};

#endif // FLOW_H
