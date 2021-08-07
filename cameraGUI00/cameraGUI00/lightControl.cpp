#include "lightControl.h"

lightControl::lightControl(Camera_GUI * parent)
{
	mainWindow = parent;
	ui.setupUi(this);
	controlInitial();
	ui.comboBox_2->setEnabled(false);
	//ui.comboBox_2->setEnabled(false);
	ui.lineEdit->setEnabled(false);
	//ui.lineEdit->setEnabled(false);

}

lightControl::~lightControl()
{
}

void lightControl::controlInitial()
{
	isOpen = false;
	lightControl_port.setParity(QSerialPort::NoParity);
	lightControl_port.setDataBits(QSerialPort::Data8);
	lightControl_port.setStopBits(QSerialPort::OneStop);

	lightControl_port.setBaudRate(QSerialPort::Baud9600);

	//connect
	connect(&lightControl_port, &QSerialPort::readyRead, this, &lightControl::receive_data);




	//识别系统有效串口号
	QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
	for (int i = 0; i < availablePorts.size(); ++i) {
		ui.comboBox->addItem(availablePorts.at(i).portName());
	}
}

void lightControl::receive_data()
{
	array.append(lightControl_port.readAll());
	char *ch;
	char asciich[256];

	ch = array.data();
	HexToAscii(ch, asciich, 10);
	ui.textBrowser->insertPlainText(QString(asciich));
}

void lightControl::HexToAscii(char * pHex, char * pAscii, int nLen)
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

QByteArray lightControl::QString2Hex(QString str)
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

void lightControl::openClose()
{
	if (isOpen == false) {
		lightControl_port.setPortName(ui.comboBox->currentText());
		bool isok=lightControl_port.open(QIODevice::ReadWrite);

		ui.comboBox->setEnabled(false);
		ui.pushButton->setText(QString("关闭串口"));
		ui.lineEdit->setEnabled(true);
		ui.comboBox_2->setEnabled(true);


		isOpen = true;
		return;
	}
	else if (isOpen == true) {
		lightControl_port.close();
		ui.comboBox->setEnabled(true);
		ui.pushButton->setText(QString("打开串口"));
		ui.lineEdit->setEnabled(false);
		ui.comboBox_2->setEnabled(false);
		//qDebug() << "串口已关闭";
		isOpen = false;
		return;
	}
}

char lightControl::ConvertHexChar(char ch)
{
	if ((ch >= '0') && (ch <= '9'))
		return ch - 0x30;
	else if ((ch >= 'A') && (ch <= 'F'))
		return ch - 'A' + 10;
	else if ((ch >= 'a') && (ch <= 'f'))
		return ch - 'a' + 10;
	else return (-1);
}

void lightControl::channel_SW(int num)
{
	channelNum = num;
}

void lightControl::levelSet()
{
	if (channelNum != 0)
	{
		QString commond;

			
		if (ui.lineEdit->text().toInt() > 255)
		{
			commond = "$" + QString("3") + QString::number(channelNum) + QString("%1").arg(255,3,16, QLatin1Char('0'));
		}
		else 
		{
			commond = "$" + QString("3") + QString::number(channelNum) + QString("%1").arg(ui.lineEdit->text().toInt(), 3, 16, QLatin1Char('0')) ;
		}

		QByteArray assic = commond.toUtf8();
		vector<QString> binaryUpper;
		vector<QString> binaryBelow;
		for (int i = 0; i < assic.size(); i++)
		{
			QString binary;
			binary=QString("%1").arg(assic.data()[i], 2, 2, QLatin1Char('0'));
			binaryBelow.push_back(binary.right(4));
			binaryUpper.push_back(binary.left(binary.size() - 4));
		}
	
		int intUpper,intBelow;
		for (int i = 0; i < binaryBelow.size(); i++)
		{
			int tempUpper, tempBelow;
			
			tempUpper =binaryUpper[i].toInt(bool(), 2);
			tempBelow =binaryBelow[i].toInt(bool(), 2);
			if (i == 0)
			{
				intUpper = tempUpper;
				intBelow = tempBelow;

			}
			else
			{
				intUpper = (intUpper ^ tempUpper);
				intBelow = (intBelow ^ tempBelow);
			}

		}
		assic.append(QString::number(intUpper));
		assic.append(QString::number(intBelow));


		lightControl_port.write(assic);
	}
}
