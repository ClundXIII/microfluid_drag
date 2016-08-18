#include "fluid_simulation.h"

#if ( _USE_VEMC2 == 1 )
fluid_simulation::fluid_simulation() : vemc2::universe(1){
#else
fluid_simulation::fluid_simulation(){
#endif
    //ctor
}

fluid_simulation::~fluid_simulation(){
    //dtor
}
