//#include <opencv2/opencv.hpp>
//#include "SkinDetection.h"
//#include "LBPHFaceRecognizer.h"
//using namespace cv;
//using namespace std;
//int main()
//{
//		vector<Mat> images;
//		Mat dst_image;
//		string file_csv = "D:\\VS2012 workspace\\face-detect-recognize\\face-detect-recognize\\faces\\at.csv";
//		read_pic_csv(file_csv,images);
//		double min_skin_cover_rate = 1.2;  //���ڵ���1����
//		double current_cover_rate;
//		for(int current_image = 0;current_image < images.size();current_image++)
//		{
//			current_cover_rate = skin_rgb_cpp(images[current_image],dst_image);
//			imshow("dst-image",dst_image);
//			if (current_cover_rate < min_skin_cover_rate )
//			{
//				min_skin_cover_rate = current_cover_rate;
//				cout<<"��ǰ��СƤ�������ʣ�"<<min_skin_cover_rate<<endl;
//				imshow("min-cover-rate-image",dst_image);
//			}
//			waitKey(1);
//		}
//		system("pause");
//}