//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef cvutil_hpp
#define cvutil_hpp

#include <stdio.h>
#include <opencv2/core/core.hpp>
using namespace std;
using namespace cv;

bool mergeAlphaChannel(cv::Mat src,cv::Mat alpha,cv::Mat &dst);

// cv::imread直接用 cv::IMREAD_UNCHANGED 标记进行图像的读取，不会应用ApplyExifOrientation进行图像方向的处理
// 此时如果图像s文件是手机拍摄的jpg照片， 会有旋转上的问题
// 因此该函数对PNG文件直接使用cv::IMREAD_UNCHANGED 对其他文件使用默认的 IMREAD_COLOR 标识
cv::Mat imreadUnchangedApplyExifOrientation(string fileName);

// 缩放图像到最大指定高度， 然后使用imshow 进行图像展示
void imshowLimitHeight(string winname,cv::Mat mat,int maxH);

// 十六进制字符串转int
// 'a' -> 10
// 'aa' -> 170
bool hex2int(string str,int &intVal);
// #ffddcc 或者 #77ffddcc 或者 #333 这样的颜色字符串转Scalar (BGRA)
bool strColorParse(string colorStr,cv::Scalar &scalar);

#endif /* cvutil_hpp */
