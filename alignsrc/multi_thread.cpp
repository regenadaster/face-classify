#include <iostream>
#include <direct.h>
#include <stdlib.h>
#include <functional>
#include <thread>
#include <string>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <memory>
#include <assert.h>
#include <algorithm>
#include <queue>
#include <process.h>
#include <Windows.h>
#include "LBPHFaceRecognizer.h"
#include "align.h"
using namespace std;


vector<Mat> images;
vector<string> g_first_dir;
vector<string> g_second_dir;
int current_index = 0;
std::timed_mutex mtx;
//string save_path = "D:\\VS2012 workspace\\RecognizeFace_hj\\RecognizeFace_hj\\faces\\";
//string csv_file_name = "at.csv";
//string save_path = "E:\\face data\\colorferet\\unziped-dvd\\dvd1\\data\\images\\";
//string save_path = "E:\\contest\\output\\";
string save_path = "E:\\face data\\baidu_pic\\";
string csv_file_name = "baidu_output_";
CascadeClassifier face_cascade;
FLANDMARK_Model* model;
Mat face_mask;

class ThreadPool
{
public:
	typedef std::function<void()> Task;

	ThreadPool(int num)
		: num_(num)
		, maxQueueSize_(0)
		, running_(false)
	{
	}

	~ThreadPool()
	{
		if (running_) {
			stop();
		}
	}

	//ThreadPool(const ThreadPool&) = delete;
	//void operator=(const ThreadPool&) = delete;

	void setMaxQueueSize(int maxSize)
	{
		maxQueueSize_ = maxSize;
	}

	void start()
	{
		assert(threads_.empty());
		running_ = true;
		threads_.reserve(num_);
		for (int i = 0; i<num_; i++) {
			threads_.push_back(std::thread(std::bind(&ThreadPool::threadFunc, this)));
		}
	}

	void stop()
	{
		{
			std::unique_lock<std::mutex> ul(mutex_);
			running_ = false;
			notEmpty_.notify_all();
		}

		for (auto &iter : threads_) {
			iter.join();
		}
	}
	void join_all()
	{
		for (auto &iter : threads_) {
			iter.join();
		}
	}

	void run(const Task &t)
	{
		if (threads_.empty()) {
			t();
		}
		else {
			std::unique_lock<std::mutex> ul(mutex_);
			while (isFull()) {
				notFull_.wait(ul);
			}
			assert(!isFull());
			queue_.push_back(t);
			notEmpty_.notify_one();
		}
	}

private:
	bool isFull() const
	{
		return maxQueueSize_ > 0 && queue_.size() >= maxQueueSize_;
	}

	void threadFunc()
	{
		printf("create id:%d\n", ::GetCurrentThreadId());
		while (running_) {
			Task task(take());
			if (task) {
				task();
			}
		}
		printf("thread id:%d\n", ::GetCurrentThreadId());
	}

	Task take()
	{
		std::unique_lock<std::mutex> ul(mutex_);
		while (queue_.empty() && running_) {
			notEmpty_.wait(ul);
		}
		Task task;
		if (!queue_.empty()) {
			task = queue_.front();
			queue_.pop_front();
			if (maxQueueSize_ > 0) {
				notFull_.notify_one();
			}
		}
		return task;
	}

private:
	int num_;
	std::mutex mutex_;
	std::condition_variable notEmpty_;
	std::condition_variable notFull_;
	std::vector<std::thread> threads_;
	std::deque<Task> queue_;
	size_t maxQueueSize_;
	bool running_;
};

void fun()
{
	printf("[id:%d] hello, world!\n", ::GetCurrentThreadId());
}
void read_csv_v4(const string& csvfilename, vector<Mat>& images, vector<string>& first_dir,vector<string>& second_dir, char separator) 
{
	std::ifstream file(csvfilename.c_str(), ifstream::in);//c_str()函数可用可不用，无需返回一个标准C类型的字符串
	if (!file) 
	{
		string error_message ="No valid input file was given, please check the given filename.";
		CV_Error(CV_StsBadArg, error_message);
	}
	string line, path, second_string,first_string;
	while (getline(file,line))//从文本文件中读取一行字符，未指定限定符默认限定符为“/n”
	{

		stringstream liness(line);//这里采用stringstream主要作用是做字符串的分割
		getline(liness, path, separator);//读入图片文件路径以分好作为限定符
		getline(liness, first_string,separator);
		getline(liness,second_string);//读入图片标签，默认限定符
		if(!path.empty()&&!first_string.empty()&&!second_string.empty())//如果读取成功，则将图片和对应标签压入对应容器中 
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
	char save_folder[100];//上层文件夹
	if (mtx.try_lock_for(chrono::milliseconds(2000)))
	{
		if (current_index < images.size())
		{
			frame = images[current_index];
			first_dir = g_first_dir[current_index];
			second_dir = g_second_dir[current_index];
			file_index = current_index;
			detect_align(frame,face_cascade,model,face_mask,dst_image);
			cout<<"current_index:"<<current_index<<"total:"<<images.size()<<endl;
			current_index++;
			mtx.unlock();
			//char window_name[50];
			//sprintf(window_name,"%d window",GetCurrentThreadId());
			//imshow(window_name,frame);
			sprintf(save_folder,"%slabeled",save_path.c_str());
			mkdir(save_folder);
			sprintf(save_folder,"%s\\%s",save_folder,first_dir.c_str());
			mkdir(save_folder);//根据label新建文件夹
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
		else
		{
			mtx.unlock();
			exit(1);
		}
	}
}

int main()
{
	{
		//string csv_path = "E:\\face data\\colorferet\\unziped-dvd\\dvd1\\data\\images\\labeled_dvd1.csv";
		//string csv_path = "D:\\VS2012 workspace\\RecognizeFace_hj\\RecognizeFace_hj\\faces\\at.csv";
		string fn_haar = "D:\\VS2012 workspace\\FaceAlignment\\FaceAlignment\\haarcascade_frontalface_alt.xml";
		string flandmarks_model_name = "D:\\VS2012 workspace\\FaceAlignment\\FaceAlignment\\flandmark_model.dat";
		// Get cascade for face detection
		face_cascade.load(fn_haar);

		if(face_cascade.empty()){
			cout<<"error loading face_cascade. Check path"<<endl;
			exit(1);
		}

		// Initialize flandmakrs with the model
		model = flandmark_init(flandmarks_model_name.c_str());
		//face_mask =imread("face_mask.png");

		for(int i = 1;i<=20;i++)
		{
			current_index = 0;
			stringstream ss;
			ss<<i;
			string csv_path = save_path + csv_file_name + ss.str() + ".csv";
			cout<<csv_path<<endl;
			//read_csv(csv_path,images,labels,';');
			read_csv_v4(csv_path,images,g_first_dir,g_second_dir,';');
			printf("main thread id:%d\n", ::GetCurrentThreadId());
			ThreadPool pool(50);
			pool.setMaxQueueSize(500);
			pool.start();
			//std::this_thread::sleep_for(std::chrono::milliseconds(3000));

			//while (!images.empty())
			//{
			//	pool.run(process_image);
			//}
			

			for (int i = 0; i < images.size(); i++) {
				pool.run(process_image);
			}
			//while (current_index<images.size())
			//{
			//	cout<<"waiting all threads quit..."<<endl;
				std::this_thread::sleep_for(std::chrono::milliseconds(60000)); //300 seconds
			//	cout<<current_index<<" "<<images.size()<<endl;
			//}
			cout<<current_index<<" "<<images.size()<<endl;	
			images.clear();
			g_first_dir.clear();
			g_second_dir.clear();
		}
		cout<<"finished!"<<endl;
		//pool.join_all();
	}

	system("pause");
	return 0;
}
