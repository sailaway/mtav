//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef Interpolator_Overshoot_h
#define Interpolator_Overshoot_h

#include "../Animation.hpp"

class OvershootInterpolator: public Interpolator{
public:
    OvershootInterpolator(){
        _tension = 2.0;
    }
    double getInterpolator(double input){
        input -= 1.0;
        return input * input * ((_tension + 1) * input + _tension) + 1.0;
    }
    
    double _tension;
};

#endif /* Interpolator_Overshoot_h */
