#ifndef __SKINFILTER_H__
#define __SKINFILTER_H__
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;
double skin_rgb_cpp(Mat source,Mat& _dst);

#endif