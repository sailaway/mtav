//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#include "MultiTrackerVideoGen.hpp"
#include "MultiTracker.hpp"
#include "../utils/cvutil.hpp"
#include "../utils/ImageProcess.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

bool setBgMat(int width,int height,string bgFile,string bgColorStr,cv::Mat &bgMat){
    cv::Scalar bgColor = cv::Scalar::all(0);
    if (bgFile.empty()) {
        if (!bgColorStr.empty()) {
            if (!strColorParse(bgColorStr, bgColor)) {
                logError("背景颜色配置错误");
                return false;
            }
        }
        bgMat.create(height, width, CV_8UC3);
        bgMat.setTo(bgColor);
    } else {
        bgMat = imreadUnchangedApplyExifOrientation(bgFile);
        if (bgMat.cols != width || bgMat.rows != height) {
            logError("背景图片尺寸必须和视频生成尺寸一致");
            return false;
        }
        if (bgMat.type() == CV_8UC4) {
            cvtColor(bgMat, bgMat, COLOR_BGRA2BGR);
        }
    }
    return true;
}

void freeAndClearTrackers(vector<TrackerBase*> &trackers){
    for (int i = 0; i < trackers.size(); i++) {
        delete trackers[i];
    }
    trackers.clear();
}

vector<TrackerBase*> getTrackersByCfgArray(Array trackerCfgs){
    vector<TrackerBase*> trackers;
    Object trackerCfg;
    for (int i = 0; i < trackerCfgs.values().size(); i++) {
        trackerCfg = trackerCfgs.get<Object>(i);
        TrackerBase *tracker = getTrackerByCfg(trackerCfg);
        if (tracker == NULL) {
            freeAndClearTrackers(trackers);
            cout<<"第"<<i<<"个轨道(下标从0开始)定义错误"<<endl;
            return trackers;
        }
        if (!tracker->checkAndInit()) {
            freeAndClearTrackers(trackers);
            cout<<"第"<<i<<"个轨道(下标从0开始)初始化校验失败"<<endl;
            return trackers;
        }
        trackers.push_back(tracker);
    }
    return trackers;
}

bool genMultiTrackerVideo(string configFile){
    Object config;
    std::ifstream ifs;
    ifs.open(configFile);
    bool ok = config.parse(ifs);
    if (!ok) {
        logError("配置文件解析失败");
        return false;
    }
    int width,height,frameCount,fps;
    string outVideoFile,bgFile,bgColorStr;
    
    getObjectStringVal(config, "outVideoFile", outVideoFile);
    getObjectStringVal(config, "bgFile",       bgFile);
    getObjectStringVal(config, "bgColor",      bgColorStr);
    getObjectNumberVal(config, "width",        width);
    getObjectNumberVal(config, "height",       height);
    getObjectNumberVal(config, "frameCount",   frameCount);
    getObjectNumberVal(config, "fps",          fps);
    if (width <= 0 || height <= 0) {
        logError("视频尺寸配置错误");
        return false;
    }
    if (frameCount <= 0 || fps <= 0) {
        logError("视频frame配置错误");
        return false;
    }
    if (outVideoFile.empty()) {
        logError("视频输出路径配置错误");
        return false;
    }
    cv::Mat bgMat;
    if (!setBgMat(width, height, bgFile, bgColorStr, bgMat)) {
        return false;
    }
    VideoWriter outputVideo;
    // 找了个MP4 拿出来的值,Get出来的fourcc 不知道是啥
    int ex = 828601953;
    outputVideo.open(outVideoFile, ex, fps, bgMat.size(), true);
    if (!outputVideo.isOpened()){
        cout<< "ERROR: Could not open the output video file"<<endl;
        return false;
    }
    
    vector<TrackerBase*> trackers;
    if (config.has<Array>("trackers")) {
        Array trackerCfgs = config.get<Array>("trackers");
        trackers = getTrackersByCfgArray(trackerCfgs);
    }
    
    cv::Mat frameMat;
    frameMat.create(bgMat.rows, bgMat.cols, bgMat.type());
    int offsetFrame;
    for(int frameIndex = 0; frameIndex < frameCount;frameIndex++){
        if (trackers.empty()) {
            outputVideo << bgMat;
            continue;
        }
        bgMat.copyTo(frameMat);
        for (int i = 0; i < trackers.size(); i++) {
            TrackerBase *tracker = trackers[i];
            if (frameIndex >= tracker->getStartFrame() && frameIndex <= tracker->getEndFrame()) {
                offsetFrame = frameIndex - tracker->getStartFrame();
                MatPos matPos = tracker->process(offsetFrame,frameMat.cols,frameMat.rows);
                if (!matPos._mat.empty()) {
                    compositeMat(frameMat, matPos._mat, matPos._posX, matPos._posY);
                }
            }
        }
        
        outputVideo << frameMat;
    }
    
    freeAndClearTrackers(trackers);
    return true;
}
