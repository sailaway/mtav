//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#include "SimpleImage.hpp"
#include "../../utils/cvutil.hpp"
#include "../../utils/ImageProcess.hpp"

// tracker中input字段必须具备的key列表
vector<string> SimpleImageTracker::requiredInputKeys(){
    vector<string> keys;
    keys.push_back("image");
    return keys;
}

// rotate 是基于图像中心点的旋转
vector<string> SimpleImageTracker::requiredPropertiesKeys(){
    vector<string> keys;
    keys.push_back("scale");
    keys.push_back("rotate");
    return keys;
}

// 校验配置参数 并进行必要的初始化
bool SimpleImageTracker::checkAndInit(){
    if (!commonCheckAndInit()) {
        return false;
    }
    string imageFile = getInputStrVal("image", "");
    if (imageFile.empty()) {
        return false;
    }
    _imgMat = imreadUnchangedApplyExifOrientation(imageFile);
    if (_imgMat.empty()) {
        return false;
    }
    _scale = getInitPropertiesDoubleVal("scale", 1);
    _rotate = getInitPropertiesDoubleVal("rotate", 0);
    
    return true;
}

void SimpleImageTracker::applyOtherPropertyAnimation(int offsetFrame){
    // no other property
}

cv::Mat SimpleImageTracker::processGetMat(int offsetFrame){
    return _imgMat;
}
