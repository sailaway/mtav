//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef Interpolator_Accelerate_h
#define Interpolator_Accelerate_h

#include "../Animation.hpp"

/**
 * An interpolator where the rate of change starts out slowly
 * and then accelerates.
 *
 * 返回input的n次幂，即抛物线的右半部分，起点缓慢，然后加速
*/
class AccelerateInterpolator: public Interpolator{
public:
    AccelerateInterpolator(){
        _factor = 1.0;
        _doubleFactor = 2.0;
    }
    double getInterpolator(double input){
        if (abs(_factor - 1.0) < 0.0001) {
            return input * input;
        } else {
            return pow(input,_doubleFactor);
        }
    }

    double _factor;
    double _doubleFactor;
};

#endif /* Interpolator_Accelerate_h */
