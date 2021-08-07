#include "EPTime_GUI.h"

EPTime::EPTime(Camera_GUI *parent)
	: QWidget(parent, Qt::Window)
{
	mainWindow = parent;
	ui.setupUi(this);
}

EPTime::~EPTime()
{
}

void EPTime::getvalue()
{
	double maxvalue = ui.lineEdit->text().toDouble();
	if ( maxvalue> 1000000)
	{
		maxvalue = 1000000.0;
	}
	double minvalue = ui.lineEdit_2->text().toDouble();
	if (minvalue < 10)
	{
		minvalue = 10.0;
	}
	double stepvalue = ui.lineEdit_3->text().toDouble();
	if (stepvalue < 1|| stepvalue > maxvalue)
	{
		stepvalue = minvalue;
	}
	mainWindow->maxEPTime = maxvalue;
	mainWindow->minEPTime = minvalue;
	mainWindow->increaseStep = stepvalue;
	for (int i = 0; i < mainWindow->objDevicePtrQue.size(); i++)
	{

		CFloatFeaturePointer objFloatPtr = mainWindow->objFeatureControlPtrQue[i]->GetFloatFeature("ExposureTime");
		objFloatPtr->SetValue(minvalue);
	}


	mainWindow->control = "collectEPTimeQue";
	this->close();
	//emit(sendValue(maxvalue, minvalue, stepvalue));
}