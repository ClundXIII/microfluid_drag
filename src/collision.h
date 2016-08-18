#ifndef COLLISION_H
#define COLLISION_H

#include "define.h"

#if ( _USE_VEMC2 == 1 )
#include <vemc2/core/universe.h>
#endif // _USE_VEMC2

class Cell;

#if ( _USE_VEMC2 == 1 )
class collision : public vemc2::simulation::effect::effect{
#else
class collision : public effect{
#endif
    public:

        #if ( _USE_VEMC2 == 1 )
            collision(vemc2::universe *u);
        #else
            collision();
        #endif


        void tick();

        void upValues();

        void setCellList(Cell **allCells);

        virtual ~collision();
    protected:
    private:

        Cell **allCells;
};

#endif // COLLISION_H
