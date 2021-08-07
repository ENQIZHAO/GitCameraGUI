#include "DLPC_GUI.h"

DLPC_GUI::DLPC_GUI(QWidget *parent)
	: QWidget(parent)
{
	//窗口使能


	ui.setupUi(this);
	for (int i = 0; i < 256; i++)
	{
		ui.comboBox_2->addItem(QString::number(i));

	}

	c0 = new DLPController();
}

DLPC_GUI::~DLPC_GUI()
{
}
void DLPC_GUI::closeEvent(QCloseEvent*event)
{
	c0->USBdisConnect();
	event->accept();
}


void DLPC_GUI::on_horizontalScrollBar_valueChanged(int width)
{
	c0->stopplay();
	c0->clearCommond();
	c0->addCommond(0, picIndex, width,1, 1);
	
	c0->sendCommond(1, 10000, 10000);
	c0->startplay();
	//ui.comboBox->setCurrentIndex(c0->getImageIndex());
	
}

void DLPC_GUI::on_comboBox_currentIndexChanged(int)
{
	c0->stopplay();
	picIndex = ui.comboBox->currentIndex();
}

void DLPC_GUI::on_comboBox_2_currentIndexChanged(int)
{
	c0->setLEDbrightness(ui.comboBox_2->currentIndex(), 0, 0);
}



void DLPC_GUI::on_pushButton_pressed()
{
	if (!c0->USBconnect())
	{
		ui.textBrowser->clear();
		ui.textBrowser->append("USB连线失败");
	}
	else
	{
		ui.textBrowser->clear();
		ui.textBrowser->append("USB连线成功");
		//初始化图片选择combox
		c0->getPicNum(picNum);
		for (int i = 0; i < picNum; i++)
		{
			ui.comboBox->addItem(QString::number(i));
		}
	}
}