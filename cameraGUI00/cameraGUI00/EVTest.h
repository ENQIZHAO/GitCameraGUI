#pragma once

//Qt
#include <QMutex>
#include <QObject>
#include <QRunnable>

//Opencv
#include <opencv2/opencv.hpp>



using namespace cv;
class CameraEVTest :public QObject, public QRunnable
{
	Q_OBJECT
public:
	CameraEVTest(Mat & srcImage,int _cameraNum);
	CameraEVTest(Mat & srcImage, int _cameraNum,Rect _ROI);
	//互斥量
	QMutex pause;
	

	//标识这是来自第几个相机的图片
	int cameraNum = 0;
	//待处理图片
	Mat dstImage;
	//两相机的掩模
	Rect ROI = Rect(0, 0, 0, 0);

	Mat redMask;
	Mat blueMask;
	Mat whiteMask;
	Mat Zero;
	Mat blue2RedMask;

protected:
	void run();
signals:
	void sendAdata(double,double,int);
	void sendAPic(cv::Mat, int);
};
