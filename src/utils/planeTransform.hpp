//
// 图像二维平面变换
//    平面坐标系以图像中心点为原点, 向右为X正方向, 向下为Y正方向
//    所有平面变换都是围绕图像中心点(原点)进行, 即不管怎么变换， 图像变换前后原点是重合的
//    目前支持的变换包括 scale,rotate
//    旋转方向: X轴正方向往Y轴正方向的顺时针旋转为正方向
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef planeTransform_hpp
#define planeTransform_hpp

#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
using namespace std;

cv::Size getFrontImgDstSize(cv::Size srcSize,double scale,double rotate);

/*
 * 平面变换
 */
class PlaneTransform{
public:
    // rotate 单位为弧度
    // maxLen 如果设置了， 在x,y都是int 的变换中将使用到
    PlaneTransform(double scale,double rotate,int maxLen){
        _scale  = scale;
        _rotate = rotate;
        _cosRotate = cos(rotate);
        _sinRotate = sin(rotate);
    }
    void transform(double x,double y,double &outx,double &outy);
private:
    double _scale;
    double _rotate;
    double _cosRotate;
    double _sinRotate;
};

void buildPlaneTransformMap(
    cv::Size dstSize,
    double scale,
    double rotate,
    cv::Mat &xmap,
    cv::Mat &ymap
);

bool planeTransformImage(
    cv::Mat src,
    cv::Size dstSize,
    cv::Mat &dst,
    double scale,
    double rotate
);


#endif /* planeTransform_hpp */
