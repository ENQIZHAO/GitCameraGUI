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
	//ͼƬ����
	vector<Mat> calibACameraPic;
	vector<Mat> calibBCameraPic;
	//�궨����Ϣ
	Size chessBoardSize;
	Size chessBoardRectSize;
	//�궨�ڲξ���
	Mat camera1Matrix;
	Mat camera2Matrix;
	//�������
	Mat camera1DistCoeffs;
	Mat camera2DistCoeffs;
	//�������
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