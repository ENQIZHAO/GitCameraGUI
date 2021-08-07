#pragma once
#include<opencv2/opencv.hpp>
#include<iostream>
#include<fstream>
#include<io.h>
#include <regex>
#include <ctime>

using namespace std;
using namespace cv;
class BasicFunc
{
public:
	void getSubdirs(string matchstr, string path, vector<string>& files);
	void LoadPicArrayDate(const string & fileslocate, const string & FilenamesBob, vector<Mat> & dstImagedateArray, vector<string> & namelist, int flag);
	string Type2String(int type);
};

