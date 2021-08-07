#pragma once

#include<iostream>
#include<sstream>
#include<fstream>
#include<direct.h>
#include<filesystem>
#include<regex>
#include<QVector> 
#include<opencv2/opencv.hpp>

//#include<opencv/highgui.h>
#include <QCloseEvent>
#include <QThreadPool>
#include <QtWidgets/QMainWindow>
#include <QMetaType>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "ui_Camera_GUI.h"
#include "inc\GalaxyIncludes.h"

//Matlab接口
#include <engine.h>

class Camera_GUI;

#pragma execution_character_set("utf-8")
#include "Mysence.h"
#include "Collect_GUI.h"
#include "lightControl.h"
#include "EVTest.h"
#include "Calib.h"
#include "EPTime_GUI.h"
#include "DLPC_GUI.h"
#include "Timer.h"
#include "CaptureEventHandler.h"

using namespace std;
using namespace cv;


#define C_RG12 1
#define C_RG8 0
#define C_COLOUR 1
#define C_GRAY 0


//主窗口类
class Camera_GUI : public QMainWindow
{
    Q_OBJECT

public:



    Camera_GUI(QWidget *parent = Q_NULLPTR);
    //******************************************函数
	//------------------------------------------功能函数
	//重写关闭事件
	void closeEvent(QCloseEvent*event);

	//停止采集
	void stopCollect();
	//开始采集
	void startCollect();

	//Mat转化
	void Mat2QImage(Mat &srcMat, QImage &dstQimage);
    //------------------------------------------相机交互相关函数
	//相机指针
	vector< CGXDevicePointer > objDevicePtrQue;
	//相机采集流指针
	vector< CGXStreamPointer> objStreamPtrQue;
	//相机属性控制器指针
	vector<CImageProcessConfigPointer>objImageProcessConfigPtrQue;
	//相机流通道控制器指针
	vector<CGXFeatureControlPointer> objFeatureControlPtrQue;
	//相机设备编号
	vector<string> CamID;
	
    //******************************************变量
	//------------------------------------------相机属性设置相关变量

	//彩色黑白标志位
	int CorU = C_GRAY;
	//像素格式标志位
	int RG8212 = C_RG8;
    //------------------------------------------程序标志符变量
	//图片采集操作提示
	string control = "show";
	//存储标识符
	bool savenow=false;
    //------------------------------------------数据变量
	//序列曝光采集数据
	double minEPTime = 1000.0;
	double maxEPTime = 100000.0;
	double increaseStep = 1000.0;



private:


    Ui::Camera_GUIClass ui;
    //******************************************函数
	//相机注销函数
	void cameraOffline();
	//相机上线
	void cameraOnline();
	//相机初始化函数
	void mainWindowInit();
	//相机重连函数
	bool cameraRestart(int camNum);

	//串口初始化函数
	void serialPortInitail();
	//采集子窗口初始化
	void collect_GUI_Initail();
	//主窗口GUI部件数据初始化
	void guiDateInitialize();
    //------------------------------------------窗口使能姿态
	//窗口使能姿态，初始
	void windowStateInitial();
	//窗口使能姿态，开始
	void windowStateStart();
	//窗口使能姿态，采集
	void windowStateCollect();
	//窗口使能状态，标定
	void windowStateCalib();
    //------------------------------------------内部功能函数
	//存储一张图片
	void saveAPic(Mat srcImage, int cameraNum);
	//显示一张图片
	void showPic(cv::Mat, int);
	//为图片加上框选框
	void addROI(cv::Mat&, int);
	//图片曝光度分析
	void ExposureAnalysis(Mat srcImage,int cameraNum);
	//存储曝光分析数据
	void saveExposureData();
	//查找给出目录下的全部文件名
	bool getFiles(string path, vector<string>& files);
	//为图像添加EV数据
	void addEVpic(cv::Mat &, cv::Mat &, Rect srcROI);
	//采集序列曝光图片功能实现
	void collectQueEPPic(int cameraNum, Mat & srcImage);
	//创建新的存储目录
	void creatASaveDir();
	//删除空目录
	void deleteEmptySaveDir();

    //******************************************变量
    //------------------------------------------数据变量
	//图片存储计数
	vector<int> savePicCount;
	//图片存储标头
	string picHeadName="0";
	//图片存储文件夹名称
	string dirName;
	//记录曝光数据
	vector<double>  explosePercent1;
	vector<double> unExplosePercent1;
	vector<double>  explosePercent2;
	vector<double> unExplosePercent2;
	//标定图片序列
	vector<Mat> calibACamera;
	vector<Mat> calibBCamera;
	//预设显示图片
	Mat noSignal;
	//ROI记录
	Rect ROIRect=Rect(Point(0,0),Point(0,0));
	Rect ROIRect2= Rect(Point(0, 0), Point(0, 0));
	//EV图片记录
	Mat * EVpic0;
	Mat * EVpic1;
	bool EVpic0Enable = false;
	bool EVpic1Enable = false;
	//序列曝光采集数据
	int picStep=0;
	//相机重连次数
	int CamRestartCount = 0;
	//接受帧计数
	uint frameCount = 0;




    //------------------------------------------标志位变量
	//相机上线标志位
	bool cameraIsonline = false;
	//图片存储标识
	vector<bool> saveDone;
	//记录曝光数据使能
	bool saveAnalysisDataEnable=false;
	//采集流工作标志位
	vector<bool> screamIsProcessing;
	//采集标志位
	bool isCollecting = false;
    //------------------------------------------自定义部件实例
	//重写带鼠标操作方法的图像框实例
	MyScene scene1;
	MyScene scene2;
    //------------------------------------------线程操作变量
	//采集子窗口指针
	Collect_GUI *w1;
	//采集线程（实例化阶段：Camera_GUI实例化同时）
	//vector<CameraPicScream *> scream;
	//采集回调函数实例指针
	vector<CSampleCaptureEventHandler*> captureHandler;
	//曝光统计线程池
	QThreadPool pool;
	//计时器线程池
	QThreadPool timerThreadPoll0;
	//计时器线程池
	QThreadPool timerThreadPoll1;
	//标定线程指针
	CalibThread* CalibThread0;
	////计时器线程
	//Timer* camRestartTimer;
	//matlab引擎指针
	Engine * ep=NULL;
	//光源控制子窗口
	lightControl* controlA;
	//DLP控制子窗口
	DLPC_GUI* controlB;
signals:
	void nosignal(cv::Mat, int);

private slots:
	//按下采集模式按钮
	void collect_BT();
	//按下停止采集按钮
	void unCollect_BT();
	//按下白平衡矫正按钮
	void whiteBalance_BT();
	//按下采集图片按钮
	void saveAPic_BT();
	//按下开始检测流程按钮
	void startProgress_BT();
	//按下标定图片按钮
	void calibAPic_BT();
	//按下标定按钮
	void calib_BT();
	//按下停止标定按钮
	void unCalib_BT();
	//按下分析曝光按钮
	void analysisExplose_BT();
	//按下停止分析曝光按钮
	void unAnalysisExplose_BT();
	//按下记录数据按钮
	void saveAnalysisData();
	//收到采集线程图片
	void reciveAPicFromSream(cv::Mat, int);
	//采集线程无信号
	void SreamNoSignal(cv::Mat, int);
	//收到曝光度分析数据
	void reciveEVAnalysData(double,double,int);
	//收到标定线程数据
	void reciveCalibMessage(QString);
	//开启MATLAB引擎
	void openMatlabEngin();
	//关闭MATLAB引擎
	void closeMatlabEngin();
	//收到EVtest的图片
	void reciveEVPic(cv::Mat,int);
	//按下采集序列曝光图片按钮
	void collectQueEPTimePic_BT();
	//按下光源控制按钮
	void lightControl_BT();
	//按下投影仪控制按钮
	void on_pushButton_16_clicked();
	//计完成事件分类函数
	void timeOutSlot(int);





};

////子类化QGraphicsScene，QGraphicsPixmapItem实现响应鼠标事件
