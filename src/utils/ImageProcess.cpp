//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#include "ImageProcess.hpp"
using namespace cv;
#include <map>
#include "cvutil.hpp"


// 只支持 CV_8UC3 和 CV_8UC4 两种mat
void compositeMat(cv::Mat &bgMat, cv::Mat mat,int locX, int locY){
    if (bgMat.type() != CV_8UC3 && bgMat.type() != CV_8UC4 ) {
        CV_Error(Error::StsBadArg, "composite bg mat MUST be CV_8UC3 or CV_8UC4");
    }
    if (mat.type() != CV_8UC3 && mat.type() != CV_8UC4 ) {
        CV_Error(Error::StsBadArg, "composite mat MUST be CV_8UC3 or CV_8UC4");
    }
    
    cv::Rect bgRoiRect(locX,locY,mat.cols,mat.rows);
    cv::Rect matRoiRect(0,0,mat.cols,mat.rows);
    if(locX <= -mat.cols || locY <= -mat.rows || locX >= bgMat.cols || locY >= bgMat.rows){
        cout<<"无需合成"<<endl;
        return;
    }
    if(locX < 0){
        matRoiRect.x = -locX;
        bgRoiRect.x  = 0;
        matRoiRect.width += locX;
        bgRoiRect.width  += locX;
    }
    if(locY < 0){
        matRoiRect.y = -locY;
        bgRoiRect.y  = 0;
        matRoiRect.height += locY;
        bgRoiRect.height  += locY;
    }
    if(bgRoiRect.x + matRoiRect.width > bgMat.cols){
        matRoiRect.width = bgMat.cols - bgRoiRect.x;
        bgRoiRect.width  = bgMat.cols - bgRoiRect.x;
    }
    if(bgRoiRect.y + matRoiRect.height > bgMat.rows){
        matRoiRect.height = bgMat.rows - bgRoiRect.y;
        bgRoiRect.height  = bgMat.rows - bgRoiRect.y;
    }
    
    if (bgMat.type() == CV_8UC4 || mat.type() == CV_8UC4) {
        cv::Mat alphaBgMat,alphaMat;
        if (bgMat.type() == CV_8UC3) {
            vector<cv::Mat> ch;
            split(bgMat, ch);
            cv::Mat alpha(bgMat.rows,bgMat.cols,CV_8U,cv::Scalar::all(255));
            ch.push_back(alpha);
            merge(ch, alphaBgMat);
        } else {
            alphaBgMat = bgMat;
        }
        if (mat.type() == CV_8UC3) {
            vector<cv::Mat> ch;
            split(mat, ch);
            cv::Mat alpha(mat.rows,mat.cols,CV_8U,cv::Scalar::all(255));
            ch.push_back(alpha);
            merge(ch, alphaMat);
        } else {
            alphaMat = mat;
        }
        
        // 两个带透明通道的 alpha 合成规则
        // dst.alpha= min(mat.alpha + bg.alpha, 255)
        // 前景alpha占比就是 mat.alpha
        // 背景alpha占比是 dst.alpha - mat.alpha
        // 举例:
        //    前景alpha = 255, 背景alpha=255 合成后 alpha=255 其中前景占比255 背景占比0
        //    前景alpha = 20, 背景alpha=30 合成后 alpha=50 其中前景占比20 背景占比30
        //    前景alpha = 200, 背景alpha=100 合成后 alpha=255 其中前景占比200 背景占比55
        cv::Mat matRoi = alphaMat(matRoiRect);
        cv::Mat bgRoi  = alphaBgMat(bgRoiRect);
        int dstAlpha;
        float matAlpha;
        for(int row = 0; row < bgRoi.rows; row++){
            cv::Vec4b *matRow   = matRoi.ptr<cv::Vec4b>(row);
            cv::Vec4b *bgRow    = bgRoi.ptr<cv::Vec4b>(row);
            for(int col = 0; col < bgRoi.cols; col++){
                dstAlpha = min(bgRow[col][3] + matRow[col][3],255);
                bgRow[col][3] = dstAlpha;
                matAlpha = matRow[col][3]/(float)dstAlpha;
                
                bgRow[col][0] = matRow[col][0] * matAlpha + bgRow[col][0] * (1-matAlpha);
                bgRow[col][1] = matRow[col][1] * matAlpha + bgRow[col][1] * (1-matAlpha);
                bgRow[col][2] = matRow[col][2] * matAlpha + bgRow[col][2] * (1-matAlpha);
            }
        }
        
        if (bgMat.type() == CV_8UC3) {
            vector<Mat> bgChannels;
            cv::split(bgMat, bgChannels);
            cvtColor(alphaBgMat, bgMat, COLOR_BGRA2BGR);
        }
        return;
    }
    cv::Mat srcRoi = mat(matRoiRect);
    cv::Mat dstRoi = bgMat(bgRoiRect);
    srcRoi.copyTo(dstRoi);
}

