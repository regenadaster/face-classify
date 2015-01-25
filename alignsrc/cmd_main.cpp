//#include "flandmark_detector.h"
//#include "linreg.h"
//#include "Illumination.h"
//#include <opencv2/opencv.hpp>
//#include "align.h"
//#include "LBPHFaceRecognizer.h"
//#include <cstdlib>
//#include "LBPFeatureExtractor.h"
//#include "multiScalePreprocess.h"
//using namespace cv;
//using namespace std;
//int main(int argc, char** argv)
//{
//	string src,dst;
//	//src = argv[0];
//	//dst = argv[1];
//	src = "E:\\face data\\baidu_pic\\baidu_images\\Angelababy\\0bd162d9f2d3572cfb9592db8813632762d0c346.jpg";
//	dst = "e:\\m-001-07.jpg";
//	string fn_haar = "D:\\VS2012 workspace\\FaceAlignment\\FaceAlignment\\haarcascade_frontalface_alt.xml";
//	string flandmarks_model_name = "D:\\VS2012 workspace\\FaceAlignment\\FaceAlignment\\flandmark_model.dat";
//
//	//add a mask on face
//	Mat face_mask ;//= imread("D:\\VS2012 workspace\\face_align_classify\\face_align_classify\\face_mask.png");
//	//cvtColor(face_mask,face_mask,CV_BGR2GRAY);
//
//	// Get cascade for face detection
//	CascadeClassifier face_cascade;
//	face_cascade.load(fn_haar);
//
//	if(face_cascade.empty()){
//		cout<<"error loading face_cascade. Check path"<<endl;
//		//exit(1);
//	}
//
//	// Initialize flandmakrs with the model
//	FLANDMARK_Model* model = flandmark_init(flandmarks_model_name.c_str());
//	Mat image;
//	image = imread(src);
//	if( image.empty() ) {
//		cerr << "ERROR: Couldn't grab the next frame." << endl;
//		//exit(1);
//	}
//
//	if(image.empty()){
//		cout<<"error loading image. Check path"<<endl;
//		//exit(1);
//	}
//
//	// Get grayscale image
//	Mat gray_image;
//	cvtColor(image, gray_image, CV_BGR2GRAY);
//
//
//	// Find face in image:
//	Rect r = detect_face(image,face_cascade);
//	//Rect r(0,0,image.cols,image.rows);
//	//cout<<r.width<<r.height<<endl;
//	if (r.width > 0)
//	{
//		//rectangle(image, r, CV_RGB(255, 255, 0), 2, CV_AA);
//	}
//
//	imshow("original frame",image);
//
//	if(r == Rect()){
//		cout<<"No faces found on image. Try with another"<<endl;
//		//exit(1);
//	}
//
//	Mat aligned_image;
//	vector<cv::Point2d> aligned_landmarks;
//
//	// Detect landmarks
//	vector<cv::Point2d> landmarks = detectLandmarks(model, gray_image, Rect(r.x,r.y,r.width,r.height));
//
//	if(landmarks.size() == 0)
//	{
//		cout<<"landmarks not found on the face"<<endl;
//		//exit(1);
//	}              
//	else
//	{
//		align(image,aligned_image,landmarks,aligned_landmarks,face_mask);
//		imshow("test",aligned_image);
//		imwrite(dst,aligned_image);
//		vector<Mat> patches;
//		if(getMultiScaleImage(aligned_image, aligned_landmarks, patches) == true)
//		{
//			for (int i = 0; i < patches.size(); i++)
//			{
//				stringstream windowname;
//				windowname<<i;
//				imshow(windowname.str(), patches[i]);
//				waitKey(1);
//			}
//		}
//	}
//	waitKey(1);
//	system("pause");
//	destroyAllWindows();
//}