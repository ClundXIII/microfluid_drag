#ifndef MD_DEFINE_H_INCLUDED
#define MD_DEFINE_H_INCLUDED

#define _USE_VEMC2 0

enum direction{
    _000 =  0,
    _p00 =  1,
    _m00 =  2,
    _0p0 =  3,
    _0m0 =  4,
    _00p =  5,
    _00m =  6,
    //
    _pp0 =  7,
    _pm0 =  8,
    _p0p =  9,
    _p0m = 10,
    _0pp = 11,
    _0pm = 12,
    _mp0 = 13,
    _mm0 = 14,
    _m0p = 15,
    _m0m = 16,
    _0mp = 17,
    _0mm = 18
};

enum direction_mode{
    _D3Q7  = 1,
    _D3Q19 = 2
};


#if !( _USE_VEMC2 == 1 )
    class effect{
        public:
        virtual void tick() =0;

        virtual void upValues() =0;
    };
#endif

#define _size_D3Q7   7
#define _size_D3Q19 19

///define which model to use:
//#define DIRECTION_FLOW_MODEL _D3Q18
//#define DIRECTION_FLOW_SIZE  _size_D3Q18
#define DIRECTION_FLOW_MODEL _D3Q19
#define DIRECTION_FLOW_SIZE  _size_D3Q19

#define M_PI 3.14159265358979323846  /* pi */

///define which datatype to use:
typedef double bdt;

#endif // MD_DEFINE_H_INCLUDED
