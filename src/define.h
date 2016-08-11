#ifndef DEFINE_H_INCLUDED
#define DEFINE_H_INCLUDED

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
    _mp0 = 11,
    _mm0 = 12,
    _m0p = 13,
    _m0m = 14,
    _0pp = 15,
    _0pm = 16,
    _0mp = 17,
    _0mm = 18
};

#define _size_D3Q6   7
#define _size_D3Q18 19

#endif // DEFINE_H_INCLUDED
