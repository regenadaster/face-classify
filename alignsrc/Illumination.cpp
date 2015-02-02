#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Illumination.h"
using namespace cv;
using namespace std;
// Normalizes a given image into a value range between 0 and 255.
Mat norm_0_255(const Mat& src) {
	// Create and return normalized image:
	Mat dst;
	switch(src.channels()) {
	case 1:
		cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
		break;
	case 3:
		cv::normalize(src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
		break;
	default:
		src.copyTo(dst);
		break;
	}
	return dst;
}
//
// Calculates the TanTriggs Preprocessing as described in:
//
// Tan, X., and Triggs, B. "Enhanced local texture feature sets for face
// recognition under difficult lighting conditions.". IEEE Transactions
// on Image Processing 19 (2010), 1635¨C650.
//
// Default parameters are taken from the paper.
//
Mat tan_triggs_preprocessing(InputArray src,
							 float alpha, float tau , float gamma , int sigma0,
							 int sigma1 ) 
{
	// Convert to floating point:
	Mat X = src.getMat();
	X.convertTo(X, CV_32FC1);
	// Start preprocessing:
	Mat I;
	pow(X, gamma, I);
	// Calculate the DOG Image:
	{
		Mat gaussian0, gaussian1;
		// Kernel Size:
		int kernel_sz0 = (3*sigma0);
		int kernel_sz1 = (3*sigma1);
		// Make them odd for OpenCV:
		kernel_sz0 += ((kernel_sz0 % 2) == 0) ? 1 : 0;
		kernel_sz1 += ((kernel_sz1 % 2) == 0) ? 1 : 0;
		GaussianBlur(I, gaussian0, Size(kernel_sz0,kernel_sz0), sigma0, sigma0, BORDER_CONSTANT);
		GaussianBlur(I, gaussian1, Size(kernel_sz1,kernel_sz1), sigma1, sigma1, BORDER_CONSTANT);
		subtract(gaussian0, gaussian1, I);
	}
	{
		double meanI = 0.0;
		{
			Mat tmp;
			pow(abs(I), alpha, tmp);
			meanI = mean(tmp).val[0];
		}
		I = I / pow(meanI, 1.0/alpha);
	}
	{
		double meanI = 0.0;
		{
			Mat tmp;
			pow(min(abs(I), tau), alpha, tmp);
			meanI = mean(tmp).val[0];
		}
		I = I / pow(meanI, 1.0/alpha);
	}
	// Squash into the tanh:
	{
		for(int r = 0; r < I.rows; r++) {
			for(int c = 0; c < I.cols; c++) {
				I.at<float>(r,c) = tanh(I.at<float>(r,c) / tau);
			}
		}
		I = tau * I;
	}
	return I;
}


void equalizeLeftAndRightHalves(Mat &faceImg)
{
	// It is common that there is stronger light from one half of the face than the other. In that case,
	// if you simply did histogram equalization on the whole face then it would make one half dark and
	// one half bright. So we will do histogram equalization separately on each face half, so they will
	// both look similar on average. But this would cause a sharp edge in the middle of the face, because
	// the left half and right half would be suddenly different. So we also histogram equalize the whole
	// image, and in the middle part we blend the 3 images together for a smooth brightness transition.

	int w = faceImg.cols;
	int h = faceImg.rows;

	// 1) First, equalize the whole face.
	Mat wholeFace;
	equalizeHist(faceImg, wholeFace);

	// 2) Equalize the left half and the right half of the face separately.
	int midX = w/2;
	Mat leftSide = faceImg(Rect(0,0, midX,h));
	Mat rightSide = faceImg(Rect(midX,0, w-midX,h));
	equalizeHist(leftSide, leftSide);
	equalizeHist(rightSide, rightSide);

	// 3) Combine the left half and right half and whole face together, so that it has a smooth transition.
	for (int y=0; y<h; y++) {
		for (int x=0; x<w; x++) {
			int v;
			if (x < w/4) {          // Left 25%: just use the left face.
				v = leftSide.at<uchar>(y,x);
			}
			else if (x < w*2/4) {   // Mid-left 25%: blend the left face & whole face.
				int lv = leftSide.at<uchar>(y,x);
				int wv = wholeFace.at<uchar>(y,x);
				// Blend more of the whole face as it moves further right along the face.
				float f = (x - w*1/4) / (float)(w*0.25f);
				v = cvRound((1.0f - f) * lv + (f) * wv);
			}
			else if (x < w*3/4) {   // Mid-right 25%: blend the right face & whole face.
				int rv = rightSide.at<uchar>(y,x-midX);
				int wv = wholeFace.at<uchar>(y,x);
				// Blend more of the right-side face as it moves further right along the face.
				float f = (x - w*2/4) / (float)(w*0.25f);
				v = cvRound((1.0f - f) * wv + (f) * rv);
			}
			else {                  // Right 25%: just use the right face.
				v = rightSide.at<uchar>(y,x-midX);
			}
			faceImg.at<uchar>(y,x) = v;
		}// end x loop
	}//end y loop
}
