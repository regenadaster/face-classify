#ifndef __ALIGN_H__
#define __ALIGN_H__

#include "flandmark_detector.h"
#include "linreg.h"
#include "Illumination.h"
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;
Rect detect_face(const Mat& image, CascadeClassifier cascade);
void get_rotated_points(const std::vector<cv::Point2d> &points, std::vector<cv::Point2d> &dst_points, const cv::Mat &rot_mat);
void show_landmarks(const std::vector<cv::Point2d> &landmarks, const cv::Mat& image, cv::Mat& result_image);
double align(const Mat &image, Mat &dst_image, vector<Point2d> &landmarks, vector<Point2d> &dst_landmarks,const cv::Mat face_mask);
vector<cv::Point2d> detectLandmarks(FLANDMARK_Model* model, const Mat & image, const Rect & face);
void detect_align(Mat &image,CascadeClassifier face_cascade,FLANDMARK_Model* model,Mat face_mask,Mat &aligned_image);
#endif // !__ALIGN_H__