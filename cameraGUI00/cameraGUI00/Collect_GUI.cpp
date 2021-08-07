#include "Collect_GUI.h"
Collect_GUI::Collect_GUI(Rect _ROI1,Rect _ROI2, Camera_GUI * parent)
{

	mainWindow = parent;
	ui.setupUi(this);
	ROI1 = _ROI1;
	ROI2 = _ROI2;
	connect(this, SIGNAL(sendAPic(cv::Mat, int)),mainWindow, SLOT(reciveAPic(cv::Mat, int)));
	connect(this, SIGNAL(nextMove()), this, SLOT(collectProgress()));
	serialPortSystemInit();
	ui.pushButton->setEnabled(false);
}
void Collect_GUI::stopCollectButttonPush()
{
	mainWindow->startCollect();
	global_port.close();
	this->close();
	
}

void Collect_GUI::startCollect()
{
	mainWindow->stopCollect();
	QString order("00 00 40 01 53 00 13 00 00 00");
	QByteArray orderHex = QString2Hex(order);
	global_port.write(orderHex);
	
	collectProgress();

}

void Collect_GUI::getAPic(int cameraNum,Mat &dstImg)
{
	CImageDataPointer objImageDataPtr;
	objImageDataPtr = mainWindow->objStreamPtrQue[cameraNum]->GetImage(800);
	if (objImageDataPtr->GetStatus() == GX_FRAME_STATUS_SUCCESS)
	{
		//char cameraNum[10];
		char picNum[10];
		char windowsName[256] = { 0 };
		//��ɫ�ɼ�
		if (mainWindow->CorU == C_COLOUR)
		{
			Mat imgRGB(objImageDataPtr->GetHeight(), objImageDataPtr->GetWidth(), CV_8UC3);
			void *pRGB24Buffer = NULL;
			pRGB24Buffer = objImageDataPtr->ImageProcess(mainWindow->objImageProcessConfigPtrQue[cameraNum]);
			//�źŴ���ͼƬ����
			memcpy(imgRGB.data, pRGB24Buffer, (objImageDataPtr->GetHeight()) * (objImageDataPtr->GetWidth()) * 3);
			dstImg = imgRGB.clone();
		}
		else//�ҶȲɼ�
		{
			void* pRaw8Buffer = NULL;
			if (mainWindow->RG8212 == C_RG8)
			{

				pRaw8Buffer = objImageDataPtr->GetBuffer();

			}
			else
			{
				pRaw8Buffer = objImageDataPtr->ConvertToRaw8(GX_BIT_4_11);
			}
			dstImg = Mat(objImageDataPtr->GetHeight(), objImageDataPtr->GetWidth(), CV_8UC1, pRaw8Buffer).clone();

			
		}

	}
}

void Collect_GUI::openClose(void)
{
	if (isOpen == false) {
		global_port.setPortName(ui.comboBox->currentText());
		global_port.open(QIODevice::ReadWrite);
		ui.textBrowser->setText("�����Ѵ�");
		ui.pushButton_3->setText("�رմ���");
		ui.pushButton->setEnabled(true);
		//qDebug() << "�����Ѵ�";
		isOpen = true;
		return;
	}
	else if (isOpen == true) {
		global_port.close();
		ui.textBrowser->setText("�����ѹر�");
		ui.pushButton_3->setText("�򿪴���");
		ui.pushButton->setEnabled(false);
		//qDebug() << "�����ѹر�";
		isOpen = false;
		return;
	}
}
void Collect_GUI::serialPortSystemInit()
{
	isOpen = false;
	//port config
	global_port.setParity(QSerialPort::NoParity);
	global_port.setDataBits(QSerialPort::Data8);
	global_port.setStopBits(QSerialPort::OneStop);

	global_port.setBaudRate(QSerialPort::Baud9600);

	//connect
	connect(&global_port, &QSerialPort::readyRead, this, &Collect_GUI::receive_data);




	//ʶ��ϵͳ��Ч���ں�
	QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
	for (int i = 0; i < availablePorts.size(); ++i) {
		ui.comboBox->addItem(availablePorts.at(i).portName());
	}
}
void  Collect_GUI::receive_data(void)
{
	array.append(global_port.readAll());

	//QByteArray a += array;
	if (array.size() == 10)
	{
		char *ch;
		char asciich[256];

		ch = array.data();
		HexToAscii(ch, asciich, 10);
		ui.textBrowser->insertPlainText(QString(asciich));
		if (asciich[4] == 83)
		{
			ui.textBrowser->insertPlainText(QString("�ٶ��������"));
			//�ٶ�������Ϻ����÷���
			QString order("00 00 40 01 44 00 01 00 00 00");
			QByteArray orderHex = QString2Hex(order);
			global_port.write(orderHex);
		}
		else if (asciich[4] == 68)
		{
			ui.textBrowser->insertPlainText(QString("�����������"));
		}
		else if (asciich[4] == 71)
		{
			ui.textBrowser->insertPlainText(QString("��ʼ�ƶ�"));
		}
		else if (degree = 360)
		{
			degree = 0;
			ui.textBrowser->insertPlainText(QString("�ɼ��������"));
		}
		else if (asciich[4] == 107&& degree != 360)
		{
			ui.textBrowser->insertPlainText(QString("�ƶ����"));
			emit(nextMove());
		}
		//qDebug() << array;
		array.clear();
	}
}
char Collect_GUI::ConvertHexChar(char ch)
{
	if ((ch >= '0') && (ch <= '9'))
		return ch - 0x30;
	else if ((ch >= 'A') && (ch <= 'F'))
		return ch - 'A' + 10;
	else if ((ch >= 'a') && (ch <= 'f'))
		return ch - 'a' + 10;
	else return (-1);
}
void Collect_GUI::HexToAscii(char *pHex, char *pAscii, int nLen)
{
	char Nibble[2];
	int i, j;
	for (i = 0; i < nLen; i++) {
		Nibble[0] = (pHex[i] & 0xF0) >> 4;
		Nibble[1] = pHex[i] & 0x0F;
		for (j = 0; j < 2; j++) {
			if (Nibble[j] < 10) {
				Nibble[j] += 0x30;
			}
			else {
				if (Nibble[j] < 16)
					Nibble[j] = Nibble[j] - 10 + 'A';
			}
			*pAscii++ = Nibble[j];
		}
		*pAscii++ = ' ';
		// for (int j = ...)
	}
	*pAscii++ = '\n';
	// for (int i = ...)
}
QByteArray Collect_GUI::QString2Hex(QString str)
{
	QByteArray senddata;
	int hexdata, lowhexdata;
	int hexdatalen = 0;
	int len = str.length();
	senddata.resize(len / 2);
	char lstr, hstr;
	for (int i = 0; i < len; )
	{
		hstr = str[i].toLatin1();
		if (hstr == ' ')
		{
			i++;
			continue;
		}
		i++;
		if (i >= len)
			break;
		lstr = str[i].toLatin1();
		hexdata = ConvertHexChar(hstr);
		lowhexdata = ConvertHexChar(lstr);
		if ((hexdata == 16) || (lowhexdata == 16))
			break;
		else
			hexdata = hexdata * 16 + lowhexdata;
		i++;
		senddata[hexdatalen] = (char)hexdata;
		hexdatalen++;
	}
	senddata.resize(hexdatalen);
	return senddata;
}

void Collect_GUI::collectProgress()
{
	////�����ƶ�N��
	//QString step = ui.lineEdit->text();
	//QByteArray stepHex =step.toUtf8();
	//
	QString order("00 00 40 01 50 00 0a 68 10 00");
	QByteArray orderHex = QString2Hex(order);
	global_port.write(orderHex);
	degree += 30;

	Mat rightPic, leftPic;
	getAPic(0, rightPic);
	getAPic(1, leftPic);
	emit(sendAPic(leftPic.clone(), 0));
	emit(sendAPic(rightPic.clone(), 1));

}
