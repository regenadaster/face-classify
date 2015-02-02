#include <iostream>
#include <direct.h>
#include <stdlib.h>
#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <assert.h>
#include <algorithm>
#include <queue>
#include "align.h"
using namespace std;


vector<Mat> images;
vector<string> g_first_dir;
vector<string> g_second_dir;
int current_index = 0;
//string save_path = "D:\\VS2012 workspace\\RecognizeFace_hj\\RecognizeFace_hj\\faces\\";
//string csv_file_name = "at.csv";
//string save_path = "E:\\face data\\colorferet\\unziped-dvd\\dvd1\\data\\images\\";
//string save_path = "E:\\contest\\output\\";
string save_path = "E:\\face data\\baidu_pic\\";
string csv_file_name = "baidu_output_";
CascadeClassifier face_cascade;
FLANDMARK_Model* model;
Mat face_mask;

void read_csv_v4(const string& csvfilename, vector<Mat>& images, vector<string>& first_dir,vector<string>& second_dir, char separator) 
{
	std::ifstream file(csvfilename.c_str(), ifstream::in);//c_str()�������ÿɲ��ã����践��һ����׼C���͵��ַ���
	if (!file) 
	{
		string error_message ="No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, second_string,first_string;
	while (getline(file,line))//���ı��ļ��ж�ȡһ���ַ���δָ���޶���Ĭ���޶���Ϊ��/n��
	{

		stringstream liness(line);//�������stringstream��Ҫ���������ַ����ķָ�
		getline(liness, path, separator);//����ͼƬ�ļ�·���Էֺ���Ϊ�޶���
		getline(liness, first_string,separator);
		getline(liness,second_string);//����ͼƬ��ǩ��Ĭ���޶���
		if(!path.empty()&&!first_string.empty()&&!second_string.empty())//�����ȡ�ɹ�����ͼƬ�Ͷ�Ӧ��ǩѹ���Ӧ������ 
		{
			images.push_back(imread(path, 1));
			first_dir.push_back(first_string);
			second_dir.push_back(second_string);
		}
	}
}

void process_image()
{
	Mat frame;
	Mat dst_image;
	int label;
	int file_index;
	string first_dir,second_dir;
	char save_folder[100];//�ϲ��ļ���
	if (current_index < images.size())
	{
		frame = images[current_index];
		first_dir = g_first_dir[current_index];
		second_dir = g_second_dir[current_index];
		file_index = current_index;
		detect_align(frame,face_cascade,model,face_mask,dst_image);
		cout<<"current_index:"<<current_index<<"total:"<<images.size()<<endl;
		current_index++;
		sprintf(save_folder,"%slabeled",save_path.c_str());
		mkdir(save_folder);
		sprintf(save_folder,"%s\\%s",save_folder,first_dir.c_str());
		mkdir(save_folder);//����label�½��ļ���
		sprintf(save_folder,"%s\\%s",save_folder,second_dir.c_str());
		mkdir(save_folder);
		sprintf(save_folder,"%s\\%dfb.jpg",save_folder,file_index);
			//detect_align(frame,face_cascade,model,face_mask,dst_image);
		if (!dst_image.empty())
		{
			imwrite(save_folder,dst_image);
		}
		else
		{
			cout<<"dst_image is empty!"<<endl;
		}
			
          /*waitKey(1)*/;

	}
}

int main()
{
	
	string fn_haar = "D:\\VS2012 workspace\\FaceAlignment\\FaceAlignment\\haarcascade_frontalface_alt.xml";
	string flandmarks_model_name = "D:\\VS2012 workspace\\FaceAlignment\\FaceAlignment\\flandmark_model.dat";
	face_cascade.load(fn_haar);

	if(face_cascade.empty()){
		cout<<"error loading face_cascade. Check path"<<endl;
		exit(1);
	}

	model = flandmark_init(flandmarks_model_name.c_str());

	for(int i = 1;i<=20;i++){
		current_index = 0;
		stringstream ss;
		ss<<i;
		string csv_path = save_path + csv_file_name + ss.str() + ".csv";
		cout<<csv_path<<endl;
			//read_csv(csv_path,images,labels,';');
		read_csv_v4(csv_path,images,g_first_dir,g_second_dir,';');

			//std::this_thread::sleep_for(std::chrono::milliseconds(3000));

			//while (!images.empty())
			//{
			//	pool.run(process_image);
			//}
			

		for (int i = 0; i < images.size(); i++) {
			process_image();
		}
			//while (current_index<images.size())
			//{
			//	cout<<"waiting all threads quit..."<<endl;
			//	cout<<current_index<<" "<<images.size()<<endl;
			//}
		cout<<current_index<<" "<<images.size()<<endl;	
		images.clear();
		g_first_dir.clear();
		g_second_dir.clear();
	}
	cout<<"finished!"<<endl;
	system("pause");
	return 0;
}
