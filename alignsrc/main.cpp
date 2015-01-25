#include "flandmark_detector.h"
#include "linreg.h"
#include "Illumination.h"
#include <opencv2/opencv.hpp>
#include "align.h"
#include "LBPHFaceRecognizer.h"
#include <cstdlib>
#include "LBPFeatureExtractor.h"

//------------------------------------------------------------------------------
// Record the execution time of some code, in milliseconds. By Shervin Emami, May 4th 2011.
// eg:
//      DECLARE_TIMING(myTimer);
//      START_TIMING(myTimer);
//        printf("A slow calc = %f\n", 1.0/sqrt(2.0) );
//      STOP_TIMING(myTimer);
//      SHOW_TIMING(myTimer, "My Timer");
//------------------------------------------------------------------------------
#define DECLARE_TIMING(s)       int64 timeStart_##s; int64 timeDiff_##s; int64 timeTally_##s = 0; int64 countTally_##s = 0
#define START_TIMING(s)         timeStart_##s = cvGetTickCount()
#define STOP_TIMING(s)          timeDiff_##s = (cvGetTickCount() - timeStart_##s); timeTally_##s += timeDiff_##s; countTally_##s++
#define GET_TIMING(s)           (double)(0.001 * ( (double)timeDiff_##s / (double)cvGetTickFrequency() ))
#define GET_AVERAGE_TIMING(s)   (double)(countTally_##s ? 0.001 * ( (double)timeTally_##s / ((double)countTally_##s * cvGetTickFrequency()) ) : 0)
#define GET_TIMING_COUNT(s)     (int)(countTally_##s)
#define CLEAR_AVERAGE_TIMING(s) timeTally_##s = 0; countTally_##s = 0
#define SHOW_TIMING(s, msg)     printf("%s time: \t %dms \t (%dms average across %d runs).\n", msg, cvRound(GET_TIMING(s)), cvRound(GET_AVERAGE_TIMING(s)), GET_TIMING_COUNT(s) )

#define VIDEO_FILE_MODE 1
#define CAMERA_MODE 2

using namespace cv;
using namespace std;

	// Get access to the webcam.
	void initWebcam(VideoCapture &videoCapture, int cameraNumber)
	{
		// Get access to the default camera.
		try {   // Surround the OpenCV call by a try/catch block so we can give a useful error message!
			videoCapture.open(cameraNumber);
		} catch (cv::Exception &e) {}
		if ( !videoCapture.isOpened() ) {
			cerr << "ERROR: Could not access the camera!" << endl;
			exit(1);
		}
		cout << "Loaded camera " << cameraNumber << "." << endl;
	}
     
    int main( /*int argc, char** argv*/ )
    {
		int mode = CAMERA_MODE;    //VIDEO_FILE_MODE  or   CAMERA_MODE
		vector<int> scales;
		int a[5] = {300, 212, 150, 106, 75};
		for(int i = 0;i<5;i++)
		scales.push_back(a[i]);
		const int patchSize = 10;
		const int gridNumX = 4;
		const int gridNumY = 4;
		const int lbpDim = 59;
		const int landmarkNum = 10;
		const int totalDim = scales.size()*gridNumX*gridNumY*lbpDim*landmarkNum;

		//add a mask on face
		Mat face_mask = imread("face_mask.png");
		cvtColor(face_mask,face_mask,CV_BGR2GRAY);

		VideoCapture videoCapture;
		switch (mode)
		{
		case VIDEO_FILE_MODE:
			{
				string video_file_path = "C:\\Users\\Johnson\\Web\\RecordFiles\\2014-09-23\\192.168.1.139_01_20140923152457686.mp4";
				try
				{
					videoCapture.open(video_file_path);
				}
				catch (cv::Exception &e)
				{
					cerr<<&e<<endl;
				}
				break; 
			}
		case CAMERA_MODE:
			initWebcam(videoCapture,0);
			break;
		}

		string fn_haar = "D:\\VS2012 workspace\\FaceAlignment\\FaceAlignment\\haarcascade_frontalface_alt.xml";
		string flandmarks_model_name = "D:\\VS2012 workspace\\FaceAlignment\\FaceAlignment\\flandmark_model.dat";
		// Get cascade for face detection
		CascadeClassifier face_cascade;
		face_cascade.load(fn_haar);

		if(face_cascade.empty()){
			cout<<"error loading face_cascade. Check path"<<endl;
			exit(1);
		}

		// Initialize flandmakrs with the model
		FLANDMARK_Model* model = flandmark_init(flandmarks_model_name.c_str());

		LBPFeatureExtractor lbpFeatureExtractor(scales, patchSize, gridNumX, gridNumY, true);

		while(true)
		{
			// Grab the next camera frame. Note that you can't modify camera frames.
			Mat image;
			videoCapture >> image;
			if( image.empty() ) {
				cerr << "ERROR: Couldn't grab the next frame." << endl;
				break;
				//exit(1);
			}
     
            if(image.empty()){
                    cout<<"error loading image. Check path"<<endl;
					break;
                    //exit(1);
            }
     
            // Get grayscale image
            Mat gray_image;
			cvtColor(image, gray_image, CV_BGR2GRAY);
     
     
            // Find face in image:
            Rect r = detect_face(image,face_cascade);
			if (r.width > 0)
			{
				rectangle(image, r, CV_RGB(255, 255, 0), 2, CV_AA);
			}
			
			imshow("original frame",image);
     
            if(r == Rect()){
                    cout<<"No faces found on image. Try with another"<<endl;
                    //exit(1);
            }
     
            Mat aligned_image;
            vector<cv::Point2d> aligned_landmarks;
     
            DECLARE_TIMING(alignTimer);
            START_TIMING(alignTimer);
     
            // Detect landmarks
            vector<cv::Point2d> landmarks = detectLandmarks(model, gray_image, Rect(r.x,r.y,r.width,r.height));
     
            if(landmarks.size() == 0)
			{
                    cout<<"landmarks not found on the face"<<endl;
                    //exit(1);
            }              
			else
			{
				//align the face
				align(gray_image,aligned_image,landmarks,aligned_landmarks,face_mask);
				//align(image,aligned_image,landmarks,aligned_landmarks,face_mask);
				
				//int *hdlbpFeatures = new int[totalDim];
				/*vector<int> hdlbpFeatures;
				hdlbpFeatures = lbpFeatureExtractor.extractAt(aligned_image,aligned_landmarks);*/

				//to do:得到高维特征之后根据高维特征进行分类

				imshow("test",aligned_image);
				STOP_TIMING(alignTimer);
				SHOW_TIMING(alignTimer, "Alignment of image and landmark points");

	
				if(!aligned_image.empty()){
					//Mat tan_triggs_image = tan_triggs_preprocessing(aligned_image);
					//Mat result_image;
					//show_landmarks(aligned_landmarks,tan_triggs_image,result_image);
					//result_image = norm_0_255(result_image);

					equalizeLeftAndRightHalves(aligned_image);
					Mat result_image;
					show_landmarks(aligned_landmarks,aligned_image,result_image);
					imshow("aligned landmarks",result_image);

					char saveFilename[100];
					std::sprintf(saveFilename, ".\\savedImages\\%d.jpg", rand());
					imwrite(saveFilename,aligned_image);
				}
			}
            waitKey(1);
		}
			system("pause");
            return 0;
    }

