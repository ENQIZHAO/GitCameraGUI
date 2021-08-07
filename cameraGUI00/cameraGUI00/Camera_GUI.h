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

//Matlab�ӿ�
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


//��������
class Camera_GUI : public QMainWindow
{
    Q_OBJECT

public:



    Camera_GUI(QWidget *parent = Q_NULLPTR);
    //******************************************����
	//------------------------------------------���ܺ���
	//��д�ر��¼�
	void closeEvent(QCloseEvent*event);

	//ֹͣ�ɼ�
	void stopCollect();
	//��ʼ�ɼ�
	void startCollect();

	//Matת��
	void Mat2QImage(Mat &srcMat, QImage &dstQimage);
    //------------------------------------------���������غ���
	//���ָ��
	vector< CGXDevicePointer > objDevicePtrQue;
	//����ɼ���ָ��
	vector< CGXStreamPointer> objStreamPtrQue;
	//������Կ�����ָ��
	vector<CImageProcessConfigPointer>objImageProcessConfigPtrQue;
	//�����ͨ��������ָ��
	vector<CGXFeatureControlPointer> objFeatureControlPtrQue;
	//����豸���
	vector<string> CamID;
	
    //******************************************����
	//------------------------------------------�������������ر���

	//��ɫ�ڰױ�־λ
	int CorU = C_GRAY;
	//���ظ�ʽ��־λ
	int RG8212 = C_RG8;
    //------------------------------------------�����־������
	//ͼƬ�ɼ�������ʾ
	string control = "show";
	//�洢��ʶ��
	bool savenow=false;
    //------------------------------------------���ݱ���
	//�����ع�ɼ�����
	double minEPTime = 1000.0;
	double maxEPTime = 100000.0;
	double increaseStep = 1000.0;



private:


    Ui::Camera_GUIClass ui;
    //******************************************����
	//���ע������
	void cameraOffline();
	//�������
	void cameraOnline();
	//�����ʼ������
	void mainWindowInit();
	//�����������
	bool cameraRestart(int camNum);

	//���ڳ�ʼ������
	void serialPortInitail();
	//�ɼ��Ӵ��ڳ�ʼ��
	void collect_GUI_Initail();
	//������GUI�������ݳ�ʼ��
	void guiDateInitialize();
    //------------------------------------------����ʹ����̬
	//����ʹ����̬����ʼ
	void windowStateInitial();
	//����ʹ����̬����ʼ
	void windowStateStart();
	//����ʹ����̬���ɼ�
	void windowStateCollect();
	//����ʹ��״̬���궨
	void windowStateCalib();
    //------------------------------------------�ڲ����ܺ���
	//�洢һ��ͼƬ
	void saveAPic(Mat srcImage, int cameraNum);
	//��ʾһ��ͼƬ
	void showPic(cv::Mat, int);
	//ΪͼƬ���Ͽ�ѡ��
	void addROI(cv::Mat&, int);
	//ͼƬ�ع�ȷ���
	void ExposureAnalysis(Mat srcImage,int cameraNum);
	//�洢�ع��������
	void saveExposureData();
	//���Ҹ���Ŀ¼�µ�ȫ���ļ���
	bool getFiles(string path, vector<string>& files);
	//Ϊͼ�����EV����
	void addEVpic(cv::Mat &, cv::Mat &, Rect srcROI);
	//�ɼ������ع�ͼƬ����ʵ��
	void collectQueEPPic(int cameraNum, Mat & srcImage);
	//�����µĴ洢Ŀ¼
	void creatASaveDir();
	//ɾ����Ŀ¼
	void deleteEmptySaveDir();

    //******************************************����
    //------------------------------------------���ݱ���
	//ͼƬ�洢����
	vector<int> savePicCount;
	//ͼƬ�洢��ͷ
	string picHeadName="0";
	//ͼƬ�洢�ļ�������
	string dirName;
	//��¼�ع�����
	vector<double>  explosePercent1;
	vector<double> unExplosePercent1;
	vector<double>  explosePercent2;
	vector<double> unExplosePercent2;
	//�궨ͼƬ����
	vector<Mat> calibACamera;
	vector<Mat> calibBCamera;
	//Ԥ����ʾͼƬ
	Mat noSignal;
	//ROI��¼
	Rect ROIRect=Rect(Point(0,0),Point(0,0));
	Rect ROIRect2= Rect(Point(0, 0), Point(0, 0));
	//EVͼƬ��¼
	Mat * EVpic0;
	Mat * EVpic1;
	bool EVpic0Enable = false;
	bool EVpic1Enable = false;
	//�����ع�ɼ�����
	int picStep=0;
	//�����������
	int CamRestartCount = 0;
	//����֡����
	uint frameCount = 0;




    //------------------------------------------��־λ����
	//������߱�־λ
	bool cameraIsonline = false;
	//ͼƬ�洢��ʶ
	vector<bool> saveDone;
	//��¼�ع�����ʹ��
	bool saveAnalysisDataEnable=false;
	//�ɼ���������־λ
	vector<bool> screamIsProcessing;
	//�ɼ���־λ
	bool isCollecting = false;
    //------------------------------------------�Զ��岿��ʵ��
	//��д��������������ͼ���ʵ��
	MyScene scene1;
	MyScene scene2;
    //------------------------------------------�̲߳�������
	//�ɼ��Ӵ���ָ��
	Collect_GUI *w1;
	//�ɼ��̣߳�ʵ�����׶Σ�Camera_GUIʵ����ͬʱ��
	//vector<CameraPicScream *> scream;
	//�ɼ��ص�����ʵ��ָ��
	vector<CSampleCaptureEventHandler*> captureHandler;
	//�ع�ͳ���̳߳�
	QThreadPool pool;
	//��ʱ���̳߳�
	QThreadPool timerThreadPoll0;
	//��ʱ���̳߳�
	QThreadPool timerThreadPoll1;
	//�궨�߳�ָ��
	CalibThread* CalibThread0;
	////��ʱ���߳�
	//Timer* camRestartTimer;
	//matlab����ָ��
	Engine * ep=NULL;
	//��Դ�����Ӵ���
	lightControl* controlA;
	//DLP�����Ӵ���
	DLPC_GUI* controlB;
signals:
	void nosignal(cv::Mat, int);

private slots:
	//���²ɼ�ģʽ��ť
	void collect_BT();
	//����ֹͣ�ɼ���ť
	void unCollect_BT();
	//���°�ƽ�������ť
	void whiteBalance_BT();
	//���²ɼ�ͼƬ��ť
	void saveAPic_BT();
	//���¿�ʼ������̰�ť
	void startProgress_BT();
	//���±궨ͼƬ��ť
	void calibAPic_BT();
	//���±궨��ť
	void calib_BT();
	//����ֹͣ�궨��ť
	void unCalib_BT();
	//���·����عⰴť
	void analysisExplose_BT();
	//����ֹͣ�����عⰴť
	void unAnalysisExplose_BT();
	//���¼�¼���ݰ�ť
	void saveAnalysisData();
	//�յ��ɼ��߳�ͼƬ
	void reciveAPicFromSream(cv::Mat, int);
	//�ɼ��߳����ź�
	void SreamNoSignal(cv::Mat, int);
	//�յ��ع�ȷ�������
	void reciveEVAnalysData(double,double,int);
	//�յ��궨�߳�����
	void reciveCalibMessage(QString);
	//����MATLAB����
	void openMatlabEngin();
	//�ر�MATLAB����
	void closeMatlabEngin();
	//�յ�EVtest��ͼƬ
	void reciveEVPic(cv::Mat,int);
	//���²ɼ������ع�ͼƬ��ť
	void collectQueEPTimePic_BT();
	//���¹�Դ���ư�ť
	void lightControl_BT();
	//����ͶӰ�ǿ��ư�ť
	void on_pushButton_16_clicked();
	//������¼����ຯ��
	void timeOutSlot(int);





};

////���໯QGraphicsScene��QGraphicsPixmapItemʵ����Ӧ����¼�
