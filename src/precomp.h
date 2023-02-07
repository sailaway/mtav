//
//  precomp.h
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General PublicLicense as published by
// the Free Software Foundation, version 3 of the License
// Author:
//   sailaway <foxitwang@gmail.com>
//

#ifndef ooVisio_precomp_h
#define ooVisio_precomp_h

#define Match_Result_Reliable 100
#define Match_Result_May_Reliable 0
#define Match_Result_Unreliable -100

#define Match_Method_ORB 0
#define Match_Method_SURF 1

#define HAVE_JPEG

#ifdef HAVE_CVCONFIG_H
#include "cvconfig.h"
#endif

#include <stdio.h>
#include <iostream>


//#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/core.hpp>
#include <opencv2/stitching/warpers.hpp>

//using namespace cv;
using namespace cv::detail;
using namespace std;

#include <opencv2/stitching/detail/util.hpp>

#endif
