//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#include "SimpleVideo.hpp"
#include "../../utils/cvutil.hpp"

// tracker中input字段必须具备的key列表
vector<string> SimpleVideoTracker::requiredInputKeys(){
    vector<string> keys;
    keys.push_back("video");
    return keys;
}
// tracker中properties字段必须具备的key列表
// 推荐将所有属性都置为必填，让外部显式指定初始值
// 但是在获取值的时候可以使用带默认值的获取方式获取
vector<string> SimpleVideoTracker::requiredPropertiesKeys(){
    vector<string> keys;
    keys.push_back("videoFrameIdx");
    keys.push_back("scale");
    keys.push_back("rotate");
    return keys;
}

// 校验配置参数 并进行必要的初始化
bool SimpleVideoTracker::checkAndInit(){
    if (!commonCheckAndInit()) {
        return false;
    }
    string videoFile = getInputStrVal("video", "");
    if (videoFile.empty()) {
        return false;
    }
    _videoCapture.open(videoFile);
    if (!_videoCapture.isOpened()){
        cout<<"视频文件打开失败"<<endl;
        return false;
    }
    _frameCount = _videoCapture.get(cv::CAP_PROP_FRAME_COUNT);
    if (_frameCount <= 0) {
        cout<<"视频文件没有图像"<<endl;
        return false;
    }
    _videoFrameIdx = getInitPropertiesIntVal("videoFrameIdx", 0);
    _scale = getInitPropertiesDoubleVal("scale", 1);
    _rotate = getInitPropertiesDoubleVal("rotate", 0);
    
    return true;
}

void SimpleVideoTracker::applyOtherPropertyAnimation(int offsetFrame){
    _videoFrameIdx = getPropertyValByOffsetFrame("videoFrameIdx", offsetFrame, _videoFrameIdx);
}

cv::Mat SimpleVideoTracker::processGetMat(int offsetFrame){
    // 视频抽帧
    int frameIndex = _videoFrameIdx % _frameCount;
    _videoCapture.set(cv::CAP_PROP_POS_FRAMES, frameIndex);
    cv::Mat frame;
    _videoCapture.read(frame);
    return frame;
}



