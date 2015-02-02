#include <iostream>
#include <dirent.h>
#include <stdlib.h>
#include <functional>
#include <string>
#include <vector>
#include <memory>
#include <assert.h>
#include <algorithm>
#include <queue>
#include <map>
#include "align.h"
#include "commonUtil.h"
#include "flandmark_detector.h"
using namespace std;
vector<Mat> images;
vector<vector<string> >allClasses;
vector<string> faceClasses;
string save_path;
string csv_file_name;
CascadeClassifier face_cascade;
FLANDMARK_Model* model;
Mat face_mask;
vector<string> saveName;
bool isPic(string path) {
	if (path.size() < 4) {
		return false;
	} else {
		string tmp = path.substr(path.size()-3,path.size());
		if (tmp == "jpg"||tmp == "png"||tmp == "PNG"||tmp == "JPG") {
                        cout<<"true"<<endl;
			return true;
		}
		else {
                        cout<<"false"<<endl;
			return false;
		}
	}
}

vector<string> getClasses(const string &dirPath) {
	DIR *dp;
	struct dirent *dirp;
	vector<string> classNames;

	if((dp=opendir(dirPath.c_str()))==NULL) {
		cout<<"cannot open"<<dirPath<<endl;
	}
	while((dirp=readdir(dp))!=NULL) {
        string tmp = dirp->d_name;
        if (tmp != "." && tmp!="..") {
            classNames.push_back(tmp);
        }
	}
	closedir(dp);
	return classNames;
}

vector<string> readOneClassPic(const string &dirPath, vector<Mat>& images, const string &savePath) {
	DIR *dp;
	struct dirent *dirp;
	vector<string> fileNames;
        string savePicPath;
	if((dp=opendir(dirPath.c_str()))==NULL) {
		cout<<"cannot open"<<dirPath<<endl;
	}
	while((dirp=readdir(dp))!=NULL) {
		string tmpPicPath;
	    cout<<dirp->d_name<<endl;
        tmpPicPath = dirPath + dirp->d_name;
        savePicPath = savePath + dirp->d_name;
        if (isPic(tmpPicPath)) {
            cout<<"image:path"<<tmpPicPath<<endl;
            images.push_back(imread(tmpPicPath,1));
            fileNames.push_back(savePicPath);
        }
	}
	closedir(dp);
	return fileNames;
}

void process_image(int picIndex, string path){
	Mat frame;
	Mat dst_image;
	if (picIndex < images.size()) {
		frame = images[picIndex];
		detect_align(frame,face_cascade,model,face_mask,dst_image);

			//detect_align(frame,face_cascade,model,face_mask,dst_image);
		if (!dst_image.empty()) {
			imwrite(path,dst_image);
		}
		else {
			cout<<"dst_image is empty!"<<endl;
		}
			
          /*waitKey(1)*/;

	}
}

int main() {
	string confPath = "./conf.properties";
	string fn_haar,flandmarks_model_name, picPath,savePath;
    map<string,string> conf;
    cout<<"start"<<endl;

    conf = readConf(confPath);
    cout<<"read conf"<<endl;
    fn_haar = conf["fn_harr"];
    flandmarks_model_name = conf["flandmarks_model_name"];

    
	face_cascade.load(fn_haar);

	if(face_cascade.empty()){
		cout<<"error loading face_cascade. Check path"<<endl;
		exit(1);
	}
        cout<<"add model"<<endl;
	model = flandmark_init(flandmarks_model_name.c_str());
    
    picPath = conf["picpath"];
    savePath = conf["savepath"];

    faceClasses = getClasses(picPath);
    cout<<"after getclasses"<<endl;
	for(int i = 0;i < faceClasses.size();i++){
                cout<<"faceclasses is:"+faceClasses[i]<<endl;
		vector<string> tmpClass = readOneClassPic(picPath +faceClasses[i]+"/", images, savePath);
		allClasses.push_back(tmpClass);
		for(int j = 0; j<tmpClass.size(); j++) {
            process_image(j, tmpClass[j]);
        }
        images.clear();
	}


	cout<<"finished!"<<endl;
	system("pause");
	return 0;
}
