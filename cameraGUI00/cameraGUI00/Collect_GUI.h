#pragma once
#include<opencv2/opencv.hpp>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <qwidget.h>
#include <ui_Collection.h>

class Collect_GUI;
#include "Camera_GUI.h"

using namespace cv;

class Collect_GUI :public QWidget
{
	Q_OBJECT
public:
	explicit Collect_GUI(Rect _ROI1= Rect(Point(0, 0), Point(0, 0)), Rect _ROI2 = Rect(Point(0, 0), Point(0, 0)), Camera_GUI *parent=0);
	Camera_GUI * mainWindow;
private:
	Ui::Collecting ui;
	void serialPortSystemInit();
	void receive_data();
	char ConvertHexChar(char ch);
	void HexToAscii(char * pHex, char * pAscii, int nLen);
	QByteArray QString2Hex(QString str);

	//²É¼¯ROI
	Rect ROI1,ROI2;
	QByteArray array;
	bool isOpen;
	QSerialPort global_port;
	int degree = 0;

private slots:
	void stopCollectButttonPush();
	void startCollect();
	void collectProgress();
	void getAPic(int cameraNum,Mat &dstImg);
	//void btn_send_data(void);
	void openClose(void);
signals:
	void sendAPic(cv::Mat, int);
	void nextMove();
};