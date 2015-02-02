#ifndef COMMONUTIL_H
#define COMMONUTIL_H


#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;

vector<string> split(string src,char sep);

map<string,string> readConf(string confFilePath);

#endif
