//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#include "MultiTracker.hpp"
#include "../utils/planeTransform.hpp"
#include "../utils/cvutil.hpp"
#include "trakers/SimpleImage.hpp"
#include "trakers/SimpleVideo.hpp"
#include "interpolators/Accelerate.hpp"
#include "interpolators/AccelerateDecelerate.hpp"
#include "interpolators/Anticipate.hpp"
#include "interpolators/AnticipateOvershoot.hpp"
#include "interpolators/Bounce.hpp"
#include "interpolators/Cycle.hpp"
#include "interpolators/Decelerate.hpp"
#include "interpolators/Overshoot.hpp"
#include "interpolators/Linear.hpp"
#include "interpolators/Shake.hpp"

#define DOUBLE_MIN 0.000001

bool TrackerBase::initAnimations(){
    if (!_trackerCfg.has<Array>("animation")) {
        return true;
    }
    Array animationCfgArr = _trackerCfg.get<Array>("animation");
    for (int i = 0; i < animationCfgArr.size(); i++) {
        if (!animationCfgArr.has<Object>(i)) {
            return false;
        }
        Object aniCfg = animationCfgArr.get<Object>(i);
        Animation animation;
        if(!setAnimationByCfg(aniCfg, animation)){
            return false;
        }
        _animations.push_back(animation);
    }
    
    return true;
}

bool TrackerBase::commonCheckAndInit(){
    if (!getObjectNumberVal(_trackerCfg, "startFrame", _startFrame)
        || !getObjectNumberVal(_trackerCfg, "endFrame", _endFrame)) {
        return false;
    }
    
    vector<string> keys = requiredInputKeys();
    if (!keys.empty()) {
        if (!_trackerCfg.has<Object>("input")) {
            return false;
        }
        Object input = _trackerCfg.get<Object>("input");
        std::map<std::string, Value*> kvMap = input.kv_map();
        for (int i = 0; i < keys.size(); i++) {
            if (kvMap.find(keys[i]) == kvMap.end()) {
                ostringstream oss;
                oss<<"必须输入字段("<<keys[i]<<")缺失";
                logError(oss.str());
                return false;
            }
        }
    }
    
    _videoAlignPtX   = getInitPropertiesDoubleVal("videoAlignPtX", 0);
    _videoAlignPtY   = getInitPropertiesDoubleVal("videoAlignPtY", 0);
    _trackerAlignPtX = getInitPropertiesDoubleVal("trackerAlignPtX", 0);
    _trackerAlignPtY = getInitPropertiesDoubleVal("trackerAlignPtY", 0);
    
    _scale  = getInitPropertiesDoubleVal("scale",  0);
    _rotate = getInitPropertiesDoubleVal("rotate", 0);
    _alpha  = getInitPropertiesDoubleVal("alpha",  0);
    
    keys = requiredPropertiesKeys();
    if (!keys.empty()) {
        if (!_trackerCfg.has<Object>("initProperties")) {
            return false;
        }
        Object iniProperties = _trackerCfg.get<Object>("initProperties");
        std::map<std::string, Value*> kvMap = iniProperties.kv_map();
        for (int i = 0; i < keys.size(); i++) {
            if (kvMap.find(keys[i]) == kvMap.end()) {
                ostringstream oss;
                oss<<"必须初始化属性字段("<<keys[i]<<")缺失";
                logError(oss.str());
                return false;
            }
        }
    }
    
    // 初始化animation列表
    if(!initAnimations()){
        return false;
    }
    
    return true;
}

string TrackerBase::getInputStrVal(string key,string defaultVal){
    if (!_trackerCfg.has<Object>("input")) {
        return defaultVal;
    }
    Object input = _trackerCfg.get<Object>("input");
    return input.get<string>(key, defaultVal);
}
string TrackerBase::getInitPropertiesStrVal(string key,string defaultVal){
    if (!_trackerCfg.has<Object>("initProperties")) {
        return defaultVal;
    }
    Object initProperties = _trackerCfg.get<Object>("initProperties");
    return initProperties.get<string>(key, defaultVal);
}
int TrackerBase::getInitPropertiesIntVal(string key,int defaultVal){
    if (!_trackerCfg.has<Object>("initProperties")) {
        return defaultVal;
    }
    int val = defaultVal;
    Object initProperties = _trackerCfg.get<Object>("initProperties");
    getObjectNumberVal(initProperties, key, val);
    return val;
}
double TrackerBase::getInitPropertiesDoubleVal(string key,double defaultVal){
    if (!_trackerCfg.has<Object>("initProperties")) {
        return defaultVal;
    }
    double val = defaultVal;
    Object initProperties = _trackerCfg.get<Object>("initProperties");
    getObjectNumberVal(initProperties, key, val);
    return val;
}

double TrackerBase::getPropertyValByOffsetFrame(string property,int offsetFrame,double defaultVal){
    for (int i = 0; i < _animations.size(); i++) {
        Animation animation = _animations[i];
        if (animation._property == property
            && offsetFrame >= animation._startOffsetFrame
            && offsetFrame < animation._endOffsetFrame) {
            double input    = (offsetFrame - animation._startOffsetFrame) / (double)(animation._endOffsetFrame-1 - animation._startOffsetFrame);
            double interVal = animation._interpolator->getInterpolator(input);
            return animation._startVal + interVal * (animation._endVal - animation._startVal);
        }
    }
    return defaultVal;
}

// 通用属性的属性动画作用在通用属性上
void TrackerBase::applyCommonPropertyAnimation(int offsetFrame){
    _videoAlignPtX   = getPropertyValByOffsetFrame("videoAlignPtX",   offsetFrame, _videoAlignPtX  );
    _videoAlignPtY   = getPropertyValByOffsetFrame("videoAlignPtY",   offsetFrame, _videoAlignPtY  );
    _trackerAlignPtX = getPropertyValByOffsetFrame("trackerAlignPtX", offsetFrame, _trackerAlignPtX);
    _trackerAlignPtY = getPropertyValByOffsetFrame("trackerAlignPtY", offsetFrame, _trackerAlignPtY);
    
    _alpha  = getPropertyValByOffsetFrame("alpha",  offsetFrame, _alpha );
    _scale  = getPropertyValByOffsetFrame("scale",  offsetFrame, _scale );
    _rotate = getPropertyValByOffsetFrame("rotate", offsetFrame, _rotate);
}

MatPos TrackerBase::process(int offsetFrame,int videoW,int videoH){
    applyCommonPropertyAnimation(offsetFrame);
    applyOtherPropertyAnimation(offsetFrame);
    if (_scale <= 0 || _alpha <= 0) {
        cv::Mat empty;
        MatPos matPos(empty,0,0);
        return matPos;
    }
    cv::Mat mat = processGetMat(offsetFrame);
    
    // scale/rotate 作用在mat上
    if (abs(_scale-1) > DOUBLE_MIN || abs(_rotate) > DOUBLE_MIN) {
        cv::Mat dst;
        double rotate = _rotate * M_PI / 180;
        cv::Size dstSize = getFrontImgDstSize(mat.size(),_scale,rotate);
        planeTransformImage(mat, dstSize, dst, _scale, rotate);
        
        mat = dst;
    }
    
    // alpha 作用在mat上
    double alpha = min(_alpha,1.0);
    if (alpha < 1) {
        if (mat.type() == CV_8UC3) {
            int alphaVal = 255 * alpha;
            cv::Mat alphaChannel(mat.rows,mat.cols,CV_8U,cv::Scalar::all(alphaVal));
            cv::Mat dst;
            bool ok = mergeAlphaChannel(mat, alphaChannel, dst);
            if (ok) {
                mat = dst;
            }
        } else if(mat.type() == CV_8UC4){
            vector<cv::Mat> channels;
            cv::split(mat, channels);
            cv::Mat alphaChannel = channels[3];
            alphaChannel *= alpha;
            channels[3] = alphaChannel;
            // 不能叠加到原来的mat上
            cv::Mat dst;
            cv::merge(channels,dst);
            mat = dst;
        }
    }
    
    double trackerX = _trackerAlignPtX * mat.cols;
    double trackerY = _trackerAlignPtY * mat.rows;
    double videoX = _videoAlignPtX * videoW;
    double videoY = _videoAlignPtY * videoH;
    double posx = videoX - trackerX;
    double posy = videoY - trackerY;
    
    MatPos matPos(mat,posx,posy);
    return matPos;
}

TrackerBase* getTrackerByCfg(Object trackerCfgObj){
    string trackerType;
    if(!getObjectStringVal(trackerCfgObj, "trackType", trackerType)){
        cout<<"未定义轨道类型"<<endl;
        return NULL;
    }
    if (trackerType == TRACKER_SIMPLE_IMAGE) {
        SimpleImageTracker *tracker = new SimpleImageTracker(trackerCfgObj);
        return static_cast<TrackerBase *>(tracker);
    } else if(trackerType == TRACKER_SIMPLE_VIDEO){
        SimpleVideoTracker *tracker = new SimpleVideoTracker(trackerCfgObj);
        return static_cast<TrackerBase *>(tracker);
    } else if(trackerType == TRACKER_SPHERE_LITTLE_PLANT){
        // 专业通道
    } else if(trackerType == TRACKER_SPHERE_FISHEYE){
        // 专业通道
    }
    cout<<"未支持的轨道类型:"<<trackerType<<endl;
    return NULL;
}

Interpolator* getInterpolatorByCfg(Object interCfgObj){
    string interType;
    if (!getObjectStringVal(interCfgObj, "interType", interType)) {
        return NULL;
    }
    
    if (interType == INTER_TYPE_ACCELERATE) {
        AccelerateInterpolator *interpolator = new AccelerateInterpolator();
        getObjectNumberVal(interCfgObj, "factor", interpolator->_factor);
        getObjectNumberVal(interCfgObj, "doubleFactor", interpolator->_doubleFactor);
        return static_cast<Interpolator *>(interpolator);
    } else if (interType == INTER_TYPE_ACCELERATEDECELERATE) {
        AccelerateDecelerateInterpolator *interpolator = new AccelerateDecelerateInterpolator();
        return static_cast<Interpolator *>(interpolator);
    } else if (interType == INTER_TYPE_ANTICIPATE) {
        AnticipateInterpolator *interpolator = new AnticipateInterpolator();
        getObjectNumberVal(interCfgObj, "tension", interpolator->_tension);
        return static_cast<Interpolator *>(interpolator);
    } else if (interType == INTER_TYPE_ANTICIPATEOVERSHOOT) {
        AnticipateOvershootInterpolator *interpolator = new AnticipateOvershootInterpolator();
        getObjectNumberVal(interCfgObj, "tension", interpolator->_tension);
        return static_cast<Interpolator *>(interpolator);
    } else if (interType == INTER_TYPE_BOUNCE) {
        BounceInterpolator *interpolator = new BounceInterpolator();
        return static_cast<Interpolator *>(interpolator);
    } else if (interType == INTER_TYPE_CYCLE) {
        CycleInterpolator *interpolator = new CycleInterpolator();
        getObjectNumberVal(interCfgObj, "cycles", interpolator->_cycles);
        return static_cast<Interpolator *>(interpolator);
    } else if (interType == INTER_TYPE_DECELERATE) {
        DecelerateInterpolator *interpolator = new DecelerateInterpolator();
        getObjectNumberVal(interCfgObj, "factor", interpolator->_factor);
        return static_cast<Interpolator *>(interpolator);
    } else if (interType == INTER_TYPE_OVERSHOOT) {
        OvershootInterpolator *interpolator = new OvershootInterpolator();
        getObjectNumberVal(interCfgObj, "tension", interpolator->_tension);
        return static_cast<Interpolator *>(interpolator);
    } else if (interType == INTER_TYPE_LINEAR) {
        LinearInterpolator *interpolator = new LinearInterpolator();
        return static_cast<Interpolator *>(interpolator);
    } else if (interType == INTER_TYPE_SHAKE) {
        ShakeInterpolator *interpolator = new ShakeInterpolator();
        getObjectNumberVal(interCfgObj, "shakeCycle", interpolator->_shakeCycle);
        if (interpolator->_shakeCycle <= 1) {
            interpolator->_shakeCycle = 2;
        }
        bool ok = getObjectNumberVal(interCfgObj, "friction", interpolator->_friction);
        if (!ok) {
            double h = 2 * interpolator->_shakeCycle-1;
            interpolator->_friction = (h-1)/h;
        }
        return static_cast<Interpolator *>(interpolator);
    }
    
    return NULL;
}

bool setAnimationByCfg(Object aniCfgObj,Animation &animation){
    if (!getObjectStringVal(aniCfgObj, "property", animation._property)
        || !getObjectNumberVal(aniCfgObj, "startVal", animation._startVal)
        || !getObjectNumberVal(aniCfgObj, "endVal", animation._endVal)
        || !getObjectNumberVal(aniCfgObj, "startOffsetFrame", animation._startOffsetFrame)
        || !getObjectNumberVal(aniCfgObj, "endOffsetFrame", animation._endOffsetFrame)
        || !aniCfgObj.has<Object>("interpolator")
    ) {
        return false;
    }
    Object interCfgObj = aniCfgObj.get<Object>("interpolator");
    Interpolator *inter = getInterpolatorByCfg(interCfgObj);
    if (inter == NULL) {
        return false;
    }
    animation._interpolator = inter;
    
    return true;
}
