//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#include "cvutil.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "strutil.hpp"

bool mergeAlphaChannel(cv::Mat src,cv::Mat alpha,cv::Mat &dst){
    if (src.type() != CV_8UC3 || alpha.type() != CV_8U) {
        return false;
    }
    std::vector<cv::Mat> channel;
    cv::split(src, channel);
    channel.push_back(alpha);
    cv::merge(channel, dst);
    return true;
}


cv::Mat imreadUnchangedApplyExifOrientation(string fileName){
    string lowFileName = str2LowerCase(fileName);
    bool isPng = strEndWith(lowFileName, ".png");
    if (isPng) {
        return cv::imread(fileName,cv::IMREAD_UNCHANGED);
    }
    return cv::imread(fileName);
}

void imshowLimitHeight(string winname,cv::Mat mat,int maxH){
    if (mat.rows <= maxH) {
        cv::imshow(winname, mat);
        return;
    }
    cv::Mat dst;
    cv::Size dsize;
    dsize.width = maxH * mat.cols / mat.rows;
    dsize.height = maxH;
    cv::resize(mat, dst, dsize);
    cv::imshow(winname, dst);
}

// 十六进制字符串转int
// 'a' -> 10
// 'aa' -> 170
bool hex2int(string str,int &intVal){
    if (str.empty()) {
        return false;
    }
    intVal = 0;
    int len = (int)str.length();
    int val;
    for (int i = 0; i < str.length(); i++) {
        switch (str[i]) {
            case '0':
                val = 0;
                break;
            case '1':
                val = 1;
                break;
            case '2':
                val = 2;
                break;
            case '3':
                val = 3;
                break;
            case '4':
                val = 4;
                break;
            case '5':
                val = 5;
                break;
            case '6':
                val = 6;
                break;
            case '7':
                val = 7;
                break;
            case '8':
                val = 8;
                break;
            case '9':
                val = 9;
                break;
            case 'a':
            case 'A':
                val = 10;
                break;
            case 'b':
            case 'B':
                val = 11;
                break;
            case 'c':
            case 'C':
                val = 12;
                break;
            case 'd':
            case 'D':
                val = 13;
                break;
            case 'e':
            case 'E':
                val = 14;
                break;
            case 'f':
            case 'F':
                val = 15;
                break;
            default:
                return false;
        }
        
        val = val * pow(16, len - i - 1);
        intVal += val;
    }
    return true;
}

// #ffddcc 或者 #77ffddcc 或者 #333 这样的颜色字符串转Scalar
bool strColorParse(string colorStr,cv::Scalar &scalar){
    if (colorStr[0] != '#') {
        return false;
    }
    if (colorStr.length() != 4 && colorStr.length() != 7 && colorStr.length() != 9) {
        return false;
    }
    int rval=0,gval=0,bval = 0,aval=0;
    if (colorStr.length() == 4) {
        string r = colorStr.substr(1,1);
        string g = colorStr.substr(2,1);
        string b = colorStr.substr(3,1);
        if (hex2int(r, rval) && hex2int(g, gval) && hex2int(b, bval)) {
            scalar[0] = bval * 16 + bval;
            scalar[1] = gval * 16 + gval;
            scalar[2] = rval * 16 + rval;
            return true;
        }
    } else if (colorStr.length() == 7) {
        string r = colorStr.substr(1,2);
        string g = colorStr.substr(3,2);
        string b = colorStr.substr(5,2);
        if (hex2int(r, rval) && hex2int(g, gval) && hex2int(b, bval)) {
            scalar[0] = bval;
            scalar[1] = gval;
            scalar[2] = rval;
            return true;
        }
    } else if(colorStr.length() == 9){
        string a = colorStr.substr(1,2);
        string r = colorStr.substr(3,2);
        string g = colorStr.substr(5,2);
        string b = colorStr.substr(7,2);
        if (hex2int(a, aval) && hex2int(r, rval) && hex2int(g, gval) && hex2int(b, bval)) {
            scalar[0] = bval;
            scalar[1] = gval;
            scalar[2] = rval;
            scalar[3] = aval;
            return true;
        }
    }
    return false;
}
