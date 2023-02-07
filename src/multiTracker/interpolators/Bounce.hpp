//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef Interpolator_Bounce_h
#define Interpolator_Bounce_h

#include "../Animation.hpp"

/**
 * An interpolator where the change bounces at the end
 * 类似于球掉落地面的效果
*/
class BounceInterpolator: public Interpolator{
public:
    BounceInterpolator(){}
    double bounce(double t){
        return t * t * 8.0;
    }
    double getInterpolator(double input){
        input *= 1.1226;
        if (input < 0.3535)
            return bounce(input);
        else if (input < 0.7408)
            return bounce(input - 0.54719) + 0.7;
        else if (input < 0.9644)
            return bounce(input - 0.8526) + 0.9;
        else
            return bounce(input - 1.0435) + 0.95;
    }
};

#endif /* Interpolator_Bounce_h */
