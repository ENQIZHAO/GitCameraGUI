#pragma once
#include<inc/GalaxyIncludes.h>
#include<QMutex>
#include<qobject.h>
#include<opencv2/opencv.hpp>
//相机回调函数
using cv::Mat;
using cv::Size;
using cv::Point;
class CSampleCaptureEventHandler :
	 public QObject,public ICaptureEventHandler
{
	Q_OBJECT;
public:
	CSampleCaptureEventHandler(int _camNum, int _colour=0, int _RG812=0);
	QMutex pause;
	QMutex exit;
	int colour = 0;
	int RG8212 = 0;
	int camNum = 0;
	void DoOnImageCaptured(CImageDataPointer&objImageDataPointer, void* pUserParam);
signals:
	void getAPic(cv::Mat, int);
	void nosignal(cv::Mat, int);





};

