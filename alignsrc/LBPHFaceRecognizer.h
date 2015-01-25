#ifndef LBPH_FACE_RECOGNIZER_H
#define LBPH_FACE_RECOGNIZER_H
#include <opencv2\core\core.hpp>
#include <opencv2\contrib\contrib.hpp>
#include <opencv2\highgui\highgui.hpp>
using namespace std;
using namespace cv;

void initRecognizer(/*const int num_trainingImages,const string image_Path*/);
/*int predictedLabel(const Mat faceMat);*/
Ptr<FaceRecognizer>  trained_model();
void read_csv(const string& csvfilename, vector<Mat>& images, vector<int>& labels, char separator);
void read_pic_csv(const string& csvfilename, vector<Mat>& images);
void read_csv_v3(const string& csvfilename, vector<Mat>& images, vector<int>& labels,vector<string>& paths, char separator);
#endif