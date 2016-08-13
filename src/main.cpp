#include <iostream>

#include "define.h"

#if ( _USE_VEMC2 == 1 )
#include <vemc2/core/universe.h>
#endif // _USE_VEMC2

int main(){

    std::cout << "Loading Lattice Boltzmann ..." << std::endl;
    std::cout << "              (c) Simon Michalke, 2016" << std::endl;

    #if ( _USE_VEMC2 == 1 )
        vemc2::universe u(-1);
        u.out << "Hi" << Vesper::LoggingTypes::eom;
    #else

    #endif // _USE_VEMC2

    std::cout << "exit(0);" << std::endl;
    return 0;
}
