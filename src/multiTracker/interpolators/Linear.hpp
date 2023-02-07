//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef Interpolator_Linear_h
#define Interpolator_Linear_h

#include "../Animation.hpp"

// 线性插值
class LinearInterpolator: public Interpolator{
public:
    LinearInterpolator():Interpolator(){}
    double getInterpolator(double input){
        return input;
    }
};

#endif /* Interpolator_Linear_h */
