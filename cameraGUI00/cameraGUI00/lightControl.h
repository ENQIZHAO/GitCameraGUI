#pragma once
#include<opencv2/opencv.hpp>


#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QFile>
#include <QFileDialog>

//提前声明类
class lightControl;

#include "Camera_GUI.h"
#include "ui_lightSource.h"

class lightControl :public QWidget
{
	Q_OBJECT
public:
	lightControl(Camera_GUI * parent);
	~lightControl();
	void controlInitial();
	void receive_data();
	void HexToAscii(char * pHex, char * pAscii, int nLen);
	QByteArray QString2Hex(QString str);

	QByteArray array;
	QSerialPort lightControl_port;
	Camera_GUI *mainWindow;
	Ui::lightSource ui;

private:
	char ConvertHexChar(char ch);
	bool isOpen;
	int channelNum = 0;
private slots:
	void openClose();
	void channel_SW(int);
	void levelSet();


};