#pragma once

#include <QWidget>
#include "ui_EPTime.h"

#include "Camera_GUI.h"

class EPTime : public QWidget
{
	Q_OBJECT

public:
	explicit EPTime(Camera_GUI *parent = Q_NULLPTR);
	~EPTime();
	Camera_GUI *mainWindow;
private:
	Ui::EPTime ui;
private slots:
	void getvalue();
signals:
	void sendValue(double, double, double);
};
