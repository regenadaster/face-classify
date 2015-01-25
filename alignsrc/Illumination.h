#ifndef __ILLUMINATION_H__
#define __ILLUMINATION_H__
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace cv;
using namespace std;
Mat norm_0_255(const Mat& src);

Mat tan_triggs_preprocessing( InputArray src,
							 float alpha = 0.1, float tau = 10.0, float gamma = 0.2, int sigma0 = 1,
							 int sigma1 = 2) ;

void equalizeLeftAndRightHalves(Mat &faceImg);

#endif