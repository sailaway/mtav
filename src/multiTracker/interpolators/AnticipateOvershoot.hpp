//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef Interpolator_AnticipateOvershoot_h
#define Interpolator_AnticipateOvershoot_h

#include "../Animation.hpp"

/**
 * An interpolator where the change starts backward
 * then flings forward and overshoots the target value
 * and finally goes back to the final value.
 * 起点往回一定值，然后往前，到终点再超出一定值，然后返回
*/
class AnticipateOvershootInterpolator: public Interpolator{
public:
    AnticipateOvershootInterpolator(){
        _tension = 2.0 * 1.5;
    }
    
    double a(double t,double s){
        return t * t * ((s + 1) * t - s);
    }
    double o(double t,double s){
        return t * t * ((s + 1) * t + s);
    }
    
    double getInterpolator(double input){
        // a(t, s) = t * t * ((s + 1) * t - s)
        // o(t, s) = t * t * ((s + 1) * t + s)
        // f(t) = 0.5 * a(t * 2, tension * extraTension), when t
        // f(t) = 0.5 * (o(t * 2 - 2, tension * extraTension) + 2), when t <= 1.0
        if (input < 0.5){
            return 0.5 * a(input * 2.0, _tension);
        } else {
            return 0.5 * (o(input * 2.0 - 2.0, _tension) + 2.0);
        }
    }
    
    /**
     * tension Amount of anticipation/overshoot. When tension equals 0.0f,
     * there is no anticipation/overshoot and the interpolator becomes
     * a simple acceleration/deceleration interpolator.
    */
    double _tension;
};

#endif /* Interpolator_AnticipateOvershoot_h */
