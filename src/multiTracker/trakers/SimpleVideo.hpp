//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef TRACKER_SimpleVideo_h
#define TRACKER_SimpleVideo_h

#include "../MultiTracker.hpp"

class SimpleVideoTracker: public TrackerBase{
public:
    SimpleVideoTracker(Object tracker):TrackerBase(tracker){}
    vector<string> requiredInputKeys();
    vector<string> requiredPropertiesKeys();
    // 校验配置参数 并进行必要的初始化
    bool checkAndInit();
    void applyOtherPropertyAnimation(int offsetFrame);
    cv::Mat processGetMat(int offsetFrame);
    
private:
    cv::VideoCapture _videoCapture;
    int _frameCount;
    int _videoFrameIdx;
};

#endif /* TRACKER_SimpleVideo_h */
