//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef Interpolator_Cycle_h
#define Interpolator_Cycle_h

#include "../Animation.hpp"

/**
 * Repeats the animation for a specified number of cycles.
 * The rate of change follows a sinusoidal pattern
 * 正弦曲线， 循环播放mCycles次。
 * mCycles取值为2，那么在0到1这个区间内，函数有2个周期，也就是动画播放2次
*/
class CycleInterpolator: public Interpolator{
public:
    CycleInterpolator(){
        _cycles = 2;
    }
    double getInterpolator(double input){
        return sin(2 * _cycles * M_PI * input);
    }
    
    double _cycles;
};

#endif /* Interpolator_Cycle_h */
