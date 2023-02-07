//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef Interpolator_Shake_h
#define Interpolator_Shake_h

#include "../Animation.hpp"

// 震荡插值
// 一般 friction 设置为 ((2*_shakeCycle-2)/(2*_shakeCycle-1))
class ShakeInterpolator: public Interpolator{
public:
    ShakeInterpolator():Interpolator(){
        _shakeCycle = 4;
        _friction   = 0.857;
    }
    double getInterpolator(double input){
        int seg = input * 2 * _shakeCycle;
        if (seg == 2 * _shakeCycle) {
            return 1;
        }
        double pw = pow(_friction, seg);
        double start = seg % 2 == 0 ? 1 - pw : 1 + pw;
        pw *= _friction;
        double end   = seg % 2 == 0 ? 1 + pw : 1 - pw;
        if (seg == 2 * _shakeCycle -1 ) {
            // 最后一个周期需要回到 1
            end = 1;
        }
        // 先加速后减速
        double adiInput = input * (_shakeCycle * 2) - seg;
        double adiVal = _adi.getInterpolator(adiInput);
        double val = start + (end-start) * adiVal;
        // cout<<"shake trans "<<input<<" -> "<<val<<endl;
         return val;
    }
    
    int _shakeCycle;
    double _friction;
    
private:
    AccelerateDecelerateInterpolator _adi;
};

#endif /* Interpolator_Shake_h */
