#pragma once
#include <QWidget>
#include "ui_DLPC_GUI.h"
#include"DLPController.h"

class DLPC_GUI;
#include"Camera_GUI.h"

class DLPC_GUI : public QWidget
{
	Q_OBJECT

public:
	void closeEvent(QCloseEvent*event);
	DLPC_GUI(QWidget *parent = Q_NULLPTR);
	~DLPC_GUI();
	DLPController * c0;
public slots:
	void on_pushButton_pressed();
	void on_horizontalScrollBar_valueChanged(int);
	void on_comboBox_currentIndexChanged(int);
	void on_comboBox_2_currentIndexChanged(int);

private:
	Ui::DLPC_GUI ui;
	uint picNum = 0;
	uint picIndex=0;

};
