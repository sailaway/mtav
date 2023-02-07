//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef MultiTracker_h
#define MultiTracker_h

#include "../precomp.h"
#include "../jsonxx/jsonxx.h"
#include "Animation.hpp"
using namespace jsonxx;

// 普通图像轨道
#define TRACKER_SIMPLE_IMAGE        "simpleImage"
// 普通视频轨道
#define TRACKER_SIMPLE_VIDEO        "simpleVideo"
// 全景小行星视角轨道
#define TRACKER_SPHERE_LITTLE_PLANT "sphereLittlePlant"
// 全景小行星视角轨道
#define TRACKER_SPHERE_FISHEYE      "sphereFisheye"

#define INTER_TYPE_ACCELERATE            "accelerate"
#define INTER_TYPE_ACCELERATEDECELERATE  "accelerateDecelerate"
#define INTER_TYPE_ANTICIPATE            "anticipate"
#define INTER_TYPE_ANTICIPATEOVERSHOOT   "anticipateOvershoot"
#define INTER_TYPE_BOUNCE                "bounce"
#define INTER_TYPE_CYCLE                 "cycle"
#define INTER_TYPE_DECELERATE            "decelerate"
#define INTER_TYPE_OVERSHOOT             "overshoot"
#define INTER_TYPE_LINEAR                "linear"
#define INTER_TYPE_SHAKE                 "shake"

inline void logError(string msg){
    cout<<msg<<endl;
}

class MatPos{
public:
    MatPos(cv::Mat mat,int posX,int posY){
        _mat = mat;
        _posX = posX;
        _posY = posY;
    }

    cv::Mat _mat;
    int _posX;
    int _posY;
};

inline bool getObjectNumberVal(Object obj,string key,int &val){
    if (!obj.has<Number>(key)) {
        return false;
    }
    val = obj.get<Number>(key);
    return true;
};
inline bool getObjectNumberVal(Object obj,string key,float &val){
    if (!obj.has<Number>(key)) {
        return false;
    }
    val = obj.get<Number>(key);
    return true;
};
inline bool getObjectNumberVal(Object obj,string key,double &val){
    if (!obj.has<Number>(key)) {
        return false;
    }
    val = obj.get<Number>(key);
    return true;
};
inline bool getObjectStringVal(Object obj,string key,string &val){
    if (!obj.has<string>(key)) {
        return false;
    }
    val = obj.get<string>(key);
    return true;
};

class TrackerBase{
public:
    TrackerBase(Object trackerCfg) {
        _trackerCfg = trackerCfg;
    }
    virtual ~TrackerBase(){
        for (int i = 0; i < _animations.size(); i++) {
            if (_animations[i]._interpolator != NULL) {
                delete _animations[i]._interpolator;
            }
        }
        _animations.clear();
    }
    
    // 通用初始化，_startFrame,_endFrame, 对齐点初始化, 必须input,initProperties参数检查等
    bool commonCheckAndInit();
    
    bool initAnimations();
    
    string getInputStrVal(string key,string defaultVal);
    string getInitPropertiesStrVal(string key,string defaultVal);
    int getInitPropertiesIntVal(string key,int defaultVal);
    double getInitPropertiesDoubleVal(string key,double defaultVal);
    
    // 校验配置参数 并进行必要的初始化
    virtual bool checkAndInit(){
        commonCheckAndInit();
        return true;
    }
    
    // tracker中input字段必须具备的key列表
    virtual vector<string> requiredInputKeys(){
        vector<string> keys;
        return keys;
    }
    
    // tracker中properties字段必须具备的key列表
    // 推荐将所有属性都置为必填，让外部显式指定初始值
    // 但是在获取值的时候可以使用带默认值的获取方式获取
    virtual vector<string> requiredPropertiesKeys(){
        vector<string> keys;
        return keys;
    }
    
    double getPropertyValByOffsetFrame(string property,int offsetFrame,double defaultVal);
    
    // 通用属性的属性动画作用在通用属性上
    virtual void applyCommonPropertyAnimation(int offsetFrame);
    
    // 生成当前视频帧图像, 返回的Mat 只能是 CV_8UC3 或者是 CV_8UC4 类型
    // offsetFrame 已经是视频的帧减去当前Traker的startFrame的偏移量
    // 实现在processGetMat基础上应用通用属性animation的变换
    virtual MatPos process(int offsetFrame,int videoW,int videoH);
    
    virtual void applyOtherPropertyAnimation(int offsetFrame){
        throw "未实现的命令";
    }
    virtual cv::Mat processGetMat(int offsetFrame){
        throw "未实现的命令";
    }
    
    virtual int getStartFrame(){
        return _startFrame;
    }
    virtual int getEndFrame(){
        return _endFrame;
    }
    
protected:
    Object _trackerCfg;
    int _startFrame;
    int _endFrame;
    vector<Animation> _animations;
    
    // 通用属性, 我们用对齐点描述tracker的位置
    // trackerAlignPt 是tracker图像相对坐标系的坐标, 原点在tracker图像的左上方, 以tracker图像宽度为X轴的单位长度1,tracker图像高度为Y轴单位长度1
    // videoAlignPt   是video图像相对坐标系的坐标,原点在video图像的左上方, 以video图像宽度为X轴的单位长度1,video图像高度为Y轴单位长度1
    double _videoAlignPtX;
    double _videoAlignPtY;
    double _trackerAlignPtX;
    double _trackerAlignPtY;
    
    // 0~1, 0全透明 1:完全不透明
    double _alpha;
    // 小于等于0 图像消失
    double _scale;
    // 旋转角度(单位:角度) 旋转中心点在图像中心 
    double _rotate;
};

// 根据配置属性获取轨道实体类
TrackerBase* getTrackerByCfg(Object trackerCfgObj);
bool setAnimationByCfg(Object aniCfgObj,Animation &animation);

#endif /* MultiTracker_h */
