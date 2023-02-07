//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef Animation_h
#define Animation_h

#include "../precomp.h"

class Interpolator{
public:
    //
    // 实现类都使用无参构造器
    // 需要设置的类参数都public 方便设置
    //
    // input代表动画的进度，范围是从0到1.0 当input为1.0时，则动画结束
    // 返回值，插值结果，范围也是从0到1.0
    // 但根据需要可以返回小于0或大于1.0的数值
    // 分别表示起点往回以及超出终点（undershoot/overshoot）
    virtual double getInterpolator(double input){
        throw "未实现的命令";
    }
};

class Animation{
public:
    Animation(){}
    Animation(string property,double startVal,double endVal,int startOffsetFrame,int endOffsetFrame,Interpolator* it){
        _property = property;
        _startVal = startVal;
        _endVal   = endVal;
        _startOffsetFrame = startOffsetFrame;
        _endOffsetFrame   = endOffsetFrame;
        _interpolator     = it;
    }

    string _property;
    double _startVal;
    double _endVal;
    int _startOffsetFrame;
    int _endOffsetFrame;
    Interpolator* _interpolator;
    
};

#endif /* Animation_h */
