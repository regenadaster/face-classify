#include<iostream>
#include<fstream>
#include<string>
#include<map>
#include<vector>
using namespace std;
vector<string> split(string src,char sep) {
    int i,index;
    vector<string> result;
    index = -1;
    for(i=0;i<src.size();i++) {
        if(src[i]==sep) {
            index = i;
        }
    }
    if(index!= -1) {
        result.push_back(src.substr(0,index));
        result.push_back(src.substr(index+1,src.size()));
    }
    return result;
}

map<string,string> readConf(string confFilePath) {
    ifstream infile(confFilePath.c_str(), ifstream::in);
    cout<<confFilePath.c_str()<<endl;
    if(!infile) {
        cout<<"open conf file error"<<endl;
    }
    string line;
    map<string,string> result;
    while (getline(infile,line)) {
        vector<string> str;
        str = split(line,'=');
        cout<<line<<endl;
        cout<<str[0]<<endl;
        cout<<str[1]<<endl;
        result[str[0]] = str[1];
    }
    return result;

}
