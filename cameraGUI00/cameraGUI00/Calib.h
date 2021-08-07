#pragma once
#include<qthread.h>
#include <QMutex>
#include <opencv2/opencv.hpp>
#include<iostream>
#include<sstream>
#include<fstream>

#pragma execution_character_set("utf-8")

using namespace std;
using namespace cv;

class CalibThread :public QThread
{
	Q_OBJECT;
public:
	CalibThread(vector<Mat> & src1, vector<Mat> & src2, Size _chessBoardSize, Size _chessBoardRectSize);
	QMutex pause;
	//图片序列
	vector<Mat> calibACameraPic;
	vector<Mat> calibBCameraPic;
	//标定版信息
	Size chessBoardSize;
	Size chessBoardRectSize;
	//标定内参矩阵
	Mat camera1Matrix;
	Mat camera2Matrix;
	//畸变参数
	Mat camera1DistCoeffs;
	Mat camera2DistCoeffs;
	//外参序列
	vector<Mat> camera1TvecsMat;
	vector<Mat> camera2TvecsMat;
	vector<Mat> camera1RvecsMat;
	vector<Mat> camera2RvecsMat;

	Mat   R, T, E, F;
signals:
	void calibMessage(QString);
protected:
	void run();
};