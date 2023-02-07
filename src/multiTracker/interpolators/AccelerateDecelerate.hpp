//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef Interpolator_AccelerateDecelerate_h
#define Interpolator_AccelerateDecelerate_h

#include "../Animation.hpp"
/**
 * An interpolator where the rate of change starts and ends slowly
 * but accelerates through the middle.
 *
 * 余弦函数的半个周期，起点和终点增长缓慢，而中间快速增长
*/
class AccelerateDecelerateInterpolator: public Interpolator{
public:
    double getInterpolator(double input){
        return (cos((input + 1) * M_PI) / 2.0) + 0.5;
    }
};

#endif /* Interpolator_AccelerateDecelerate_h */
