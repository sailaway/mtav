//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef Interpolator_Decelerate_h
#define Interpolator_Decelerate_h

#include "../Animation.hpp"

class DecelerateInterpolator: public Interpolator{
public:
    DecelerateInterpolator(){}
    double getInterpolator(double input){
        if (_factor == 1.0) {
            return 1.0 - (1.0 - input) * (1.0 - input);
        } else {
            return 1.0 - pow((1.0 - input), 2 * _factor);
        }
    }
    
    double _factor = 1;
};

#endif /* Interpolator_Decelerate_h */
