//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#include "planeTransform.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "cvutil.hpp"

#define DOUBLE_MIN 0.00000001

cv::Size getFrontImgDstSize(cv::Size srcSize,double scale,double rotate){
    PlaneTransform planeTransform(scale,rotate,0);
    double halfW = srcSize.width  / 2;
    double halfH = srcSize.height / 2;
    double ltx,lty,lbx,lby,rtx,rty,rbx,rby;
    double outltx,outlty,outlbx,outlby,outrtx,outrty,outrbx,outrby;
    ltx = -halfW; lty = -halfH;
    lbx = -halfW; lby =  halfH;
    rtx =  halfW; rty = -halfH;
    rbx =  halfW; rby =  halfH;
    planeTransform.transform(ltx, lty, outltx, outlty);
    planeTransform.transform(lbx, lby, outlbx, outlby);
    planeTransform.transform(rtx, rty, outrtx, outrty);
    planeTransform.transform(rbx, rby, outrbx, outrby);
    double minx = min(min(min(outltx,outlbx),outrtx),outrbx);
    double maxx = max(max(max(outltx,outlbx),outrtx),outrbx);
    double miny = min(min(min(outlty,outlby),outrty),outrby);
    double maxy = max(max(max(outlty,outlby),outrty),outrby);
    
    cv::Size dstSize;
    dstSize.width  = (int)(maxx - minx + 0.5);
    dstSize.height = (int)(maxy - miny + 0.5);
    
    if (dstSize.width %2 == 1) {
        dstSize.width += 1;
    }
    if (dstSize.height %2 == 1) {
        dstSize.height += 1;
    }
    
    return dstSize;
}

//
// 旋转矩阵 (cos(t)  -sin(t))
//        (sin(t)   cos(t))
//
void PlaneTransform::transform(double x, double y, double &outx, double &outy){
    double tx = _cosRotate * x  + _sinRotate * y;
    double ty = -_sinRotate * x + _cosRotate * y;
    outx = _scale * tx;
    outy = _scale * ty;
}

void buildPlaneTransformMap(cv::Size srcSize,cv::Size dstSize,double scale,double rotate,cv::Mat &xmap,cv::Mat &ymap){
    
    xmap.create(dstSize.height,dstSize.width,CV_32FC1);
    ymap.create(dstSize.height,dstSize.width,CV_32FC1);
    
    double halfSrcRow = srcSize.height / 2.0;
    double halfSrcCol = srcSize.width  / 2.0;
    
    int halfRow = dstSize.height/2;
    int halfCol = dstSize.width /2;
    PlaneTransform planeTransform(1/scale,rotate,0);
    int imgx,imgy;
    double srcimgx,srcimgy;
    for (unsigned int row = 0; row < xmap.rows; row++) {
        float *xptr = xmap.ptr<float>(row);
        float *yptr = ymap.ptr<float>(row);
        imgy = row - halfRow;
        for (unsigned int col = 0; col < xmap.cols; col++) {
            imgx = col - halfCol;
            planeTransform.transform(imgx, imgy, srcimgx, srcimgy);
            srcimgx += halfSrcCol;
            srcimgy += halfSrcRow;
            xptr[col] = srcimgx;
            yptr[col] = srcimgy;
        }
    }
}

bool planeTransformImage(cv::Mat src,cv::Size dstSize,cv::Mat &dst,double scale,double rotate){
    cv::Mat xmap,ymap;
    buildPlaneTransformMap(src.size(),dstSize,scale,rotate,xmap,ymap);
    if (src.type() == CV_8UC4) {
        // 带透明通道超出范围只能隐藏
        cv::remap(src, dst, xmap, ymap, cv::INTER_LINEAR,cv::BORDER_CONSTANT,cv::Scalar::all(0));
    } else {
        if (abs(rotate) > DOUBLE_MIN) {
            cv::Mat alphaSrc;
            cvtColor(src, alphaSrc, COLOR_BGR2BGRA);
            cv::remap(alphaSrc, dst, xmap, ymap, cv::INTER_LINEAR,cv::BORDER_CONSTANT,cv::Scalar::all(0));
        } else {
            cv::remap(src, dst, xmap, ymap, cv::INTER_LINEAR,cv::BORDER_CONSTANT,cv::Scalar::all(0));
        }
    }
    return true;
}
