#include "cell.h"

cell::cell(){
    #if ( _USE_VEMC2 == 1 )
        setActive(true);
    #endif
}

void cell::draw(){

}

cell::~cell(){
    //dtor
}
