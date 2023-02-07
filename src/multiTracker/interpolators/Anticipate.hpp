//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef Interpolator_Anticipate_h
#define Interpolator_Anticipate_h

#include "../Animation.hpp"

/**
 * An interpolator where the change starts backward then flings forward.
 * 起点的时候回往回一定值，而后再往前。
*/
class AnticipateInterpolator: public Interpolator{
public:
    AnticipateInterpolator(){
        _tension = 2.0;
    }
    double getInterpolator(double input){
        return input * input * ((_tension + 1) * input - _tension);
    }

    /**
     * tension Amount of anticipation.
     * When tension equals 0.0f, there is no anticipation
     * and the interpolator becomes a simple acceleration interpolator.
    */
    double _tension;
};

#endif /* Interpolator_Anticipate_h */
