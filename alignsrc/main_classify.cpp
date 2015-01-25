//#include "LBPHFaceRecognizer.h"
//#include <opencv2/opencv.hpp>
//using namespace cv;
//using namespace std;
//int main()
//{
//	Ptr<FaceRecognizer>  model = trained_model();
//	model->set("threshold",100.00);
//	cout<<"threshold"<<model->getDouble("threshold")<<endl;
//	vector<Mat> images;
//	vector<int> labels;
//	//string file_csv = ".\\savedImages\\savedImages.csv";
//	string file_csv = "E:\\contest\\output\\labeled\\align_labeled.csv";
//	read_csv(file_csv,images,labels,';');
//
//	int error_count = 0;
//	int output_label;
//	double confidence;  //associated confidence (e.g. distance) for a given input image
//	Mat gray_image;
//	for(int current_image = 0;current_image < images.size();current_image++)
//	{
//		cvtColor(images[current_image],gray_image,CV_RGB2GRAY);
//		model->predict(gray_image,output_label,confidence);
//		cout<<"Ô¤²âlabel£º"<<output_label<<"    Êµ¼Êlabel£º"<<labels[current_image]<<endl;
//		
//		if (output_label!=labels[current_image])
//		{
//			error_count++;
//			cout<<"error count:"<<error_count<<endl;
//			cout<<"confidence:"<<confidence<<endl;
//		}
//	}
//	cout<<"´íÎóÂÊ£º"<<(double)error_count/images.size()<<endl;
//	system("pause");
//}