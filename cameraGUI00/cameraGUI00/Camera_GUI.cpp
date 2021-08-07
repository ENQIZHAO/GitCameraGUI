#include "Camera_GUI.h"


Camera_GUI::Camera_GUI(QWidget *parent)
    : QMainWindow(parent)
{

    ui.setupUi(this);

	
	connect(this, SIGNAL(nosignal(cv::Mat, int)), this, SLOT(SreamNoSignal(cv::Mat, int)));

	//��ʼ��Ԥ��ͼƬ
	//1NoSiganl��ʾ
	Size textSize = cv::getTextSize("NO SIGNAL!", cv::FONT_HERSHEY_COMPLEX, 2, 2, int());
	noSignal = Mat::zeros(Size(640, 480), CV_8UC1);
	cv::putText(noSignal, "NO SIGNAL!", Point(noSignal.cols / 2 - textSize.width / 2, noSignal.rows / 2 + textSize.height / 2), 2, 2, cv::Scalar(255));
	
	windowStateInitial();
	mainWindowInit();
	
}



//������ر��¼�������д
void Camera_GUI::closeEvent(QCloseEvent * event)
{
	//ע������ٽ��йر�
	cameraOffline();


	//���û�д洢ͼƬ��������δ洢ͼƬ���ļ���
	deleteEmptySaveDir();

	event->accept();

}


void Camera_GUI::collect_BT()
{
	//��ʼ���ɼ��Ӵ���
	windowStateCollect();
	//��ʼ�ɼ�
	startCollect();
	//�����洢ͼƬ���ļ���
	creatASaveDir();

	
}



void Camera_GUI::unCollect_BT()
{
	
	//ֹͣͼƬ�ɼ��߳�
	stopCollect();

	deleteEmptySaveDir();
	//����UI����Ϊ��ʼ״̬
	windowStateStart();



}

void Camera_GUI::whiteBalance_BT()
{
	bool bIsImplemented;
	for (int i = 0; i < objDevicePtrQue.size(); i++)
	{
		bIsImplemented = objFeatureControlPtrQue[i]->IsImplemented("BalanceWhiteAuto");
		GxIAPICPP::gxstring_vector vectorEnumEntry;
		if (bIsImplemented)
		{
			CEnumFeaturePointer objEnumPtr = objFeatureControlPtrQue[i]->GetEnumFeature("BalanceWhiteAuto");

			vectorEnumEntry = objEnumPtr->GetEnumEntryList();
			//for (int i = 0; i < vectorEnumEntry.size(); i++)
			//{
			//	cout << vectorEnumEntry[i] << endl;//��ӡ����ö����
			//}
			objEnumPtr->SetValue(vectorEnumEntry[2]);//
		}
	}

}

void Camera_GUI::saveAPic_BT()
{
	savenow = true;
	//ˢ�´洢��ɱ�־
	for (int i = 0; i < saveDone.size(); i++)
	{
		saveDone[i] = false;
	}
}

void Camera_GUI::startProgress_BT()
{
	collect_GUI_Initail();
	//control = "save";
	////ˢ�´洢��ɱ�־
	//for (int i = 0; i < saveDone.size(); i++)
	//{
	//	saveDone[i] = false;
	//}
}

void Camera_GUI::calibAPic_BT()
{
	control = "saveAcalibPic";
	for (int i = 0; i < saveDone.size(); i++)
	{
		saveDone[i] = false;
	}

}

void Camera_GUI::calib_BT()
{
	startCollect();
	creatASaveDir();
	windowStateCalib();
}

void Camera_GUI::unCalib_BT()
{
	stopCollect();
	deleteEmptySaveDir();
	windowStateStart();
	CalibThread0 = new CalibThread(calibACamera, calibBCamera, Size(6, 7), Size(3, 3));
	connect(CalibThread0, SIGNAL(calibMessage(QString)), this, SLOT(reciveCalibMessage(QString)));
	CalibThread0->start();
}

void Camera_GUI::analysisExplose_BT()
{
	//�����������
	explosePercent1.clear();
	explosePercent2.clear();
	unExplosePercent1.clear();
	unExplosePercent2.clear();

	control = "EVTest";
	//Ƿ���ع�ٷֱ���
	//ui.progressBar->setVisible(true);
	ui.progressBar->setEnabled(true);
	//ui.progressBar_2->setVisible(true);
	ui.progressBar_2->setEnabled(true);
	//ui.progressBar_3->setVisible(true);
	ui.progressBar_3->setEnabled(true);
	//ui.progressBar_4->setVisible(true);
	ui.progressBar_4->setEnabled(true);

	//
	ui.pushButton_9->setEnabled(true);
	//ui.pushButton_9->setVisible(true);
	ui.pushButton_8->setEnabled(false);
	//ui.pushButton_8->setVisible(false);
	//��¼���ݰ�ť
	ui.pushButton_10->setEnabled(true);
	//ui.pushButton_10->setVisible(true);
	//�׶��ع�ɼ���ť
	ui.pushButton_14->setEnabled(false);
	//ui.pushButton_14->setVisible(false);
}

void Camera_GUI::unAnalysisExplose_BT()
{
	control = "show";

	windowStateCollect();
	//matlab
	if (ep == NULL)
	{
		//ep = engOpen(NULL);
	}
	else if (ep != NULL)
	{
		string commond;
		string exploseData;
		commond = "x=1:1:" + to_string(explosePercent1.size())+ ';';
		engEvalString(ep, commond.c_str());
		commond = "y1=[],y2=[];";
		engEvalString(ep, commond.c_str());
		for (int i = 0; i < explosePercent1.size(); i++)
		{
			commond = "y1=[y1," + to_string(explosePercent1[i]*100)+"];";
			engEvalString(ep, commond.c_str());
			commond = "y2=[y2," + to_string(unExplosePercent1[i]*100)+ "];";
			engEvalString(ep, commond.c_str());
		}
		commond = "subplot(1,2,1);";
		engEvalString(ep, commond.c_str());

		commond = "plot(x,y1,\'-*r\','lineWidth',3),hold on;";
		engEvalString(ep, commond.c_str());
		commond = "plot(x,y2,\'-*b\','lineWidth',3),hold off;";
		engEvalString(ep, commond.c_str());
		commond = "xticks(x)";
		engEvalString(ep, commond.c_str());
		for (int i = 0; i < explosePercent1.size(); i++)
		{
			
			exploseData=to_string(explosePercent1[i] * 100);
			commond = "text(" + to_string(i+1) + "," + to_string(explosePercent1[i] * 100+1) + ",[\'" + exploseData.substr(0, exploseData.find(".") + 3) + "\'],\'color\',[1 0 0])";
			engEvalString(ep, commond.c_str());
			exploseData = to_string(unExplosePercent1[i] * 100);
			commond = "text(" + to_string(i+1) + "," + to_string(unExplosePercent1[i] * 100+1) + ",[\'" + exploseData.substr(0, exploseData.find(".") + 3) + "\'],\'color\',[0 0 1])";
			engEvalString(ep, commond.c_str());
		}

		commond = "title('camera1'),xlabel('picNum'),ylabel('%'),ylim([0 100]);";
		engEvalString(ep, commond.c_str());

		commond = "x=1:1:" + to_string(explosePercent2.size()) + ';';
		engEvalString(ep, commond.c_str());
		commond = "y1=[],y2=[];";
		engEvalString(ep, commond.c_str());
		for (int i = 0; i < explosePercent2.size(); i++)
		{
			commond = "y1=[y1," + to_string(explosePercent2[i]*100) + "];";
			engEvalString(ep, commond.c_str());
			commond = "y2=[y2," + to_string(unExplosePercent2[i]*100) + "];";
			engEvalString(ep, commond.c_str());
		}
		commond = "subplot(1,2,2);";
		engEvalString(ep, commond.c_str());

		commond = "plot(x,y1,\'-*r\','lineWidth',3),hold on;";
		engEvalString(ep, commond.c_str());
		commond = "plot(x,y2,\'-*b\','lineWidth',3),hold off;";
		engEvalString(ep, commond.c_str());
		commond = "xticks(x)";
		engEvalString(ep, commond.c_str());
		for (int i = 0; i < explosePercent2.size(); i++)
		{
			exploseData = to_string(explosePercent2[i] * 100);
			commond = "text(" + to_string(i+1) + "," + to_string(explosePercent2[i] * 100+1) + ",[\'" + exploseData.substr(0, exploseData.find(".") + 3) + "\'],\'color\',[1 0 0])";
			engEvalString(ep, commond.c_str());
			exploseData = to_string(unExplosePercent2[i] * 100);
			commond = "text(" + to_string(i+1) + "," + to_string(unExplosePercent2[i] * 100+1) + ",[\'" + exploseData.substr(0, exploseData.find(".") + 3) + "\'],\'color\',[0 0 1])";
			engEvalString(ep, commond.c_str());
		}
		commond = "title('camera2'),xlabel('picNum'),ylabel('%'),ylim([0 100]);";
		engEvalString(ep, commond.c_str());
	}
	//�洢����
	saveExposureData();

}

void Camera_GUI::saveAnalysisData()
{
	saveAnalysisDataEnable = true;

}


void Camera_GUI::reciveAPicFromSream(Mat srcImage, int cameraNum)
{
	//�ɼ������ڹ���
	//uint  tempcount = objFeatureControlPtrQue[cameraNum]->GetIntFeature("StreamDeliveredPacketCount")->GetValue();
	//if (tempcount -frameCount  <= 2&&frameCount!=0)
	//{
	//	return;
	//}
	//frameCount = tempcount;
	


	screamIsProcessing[cameraNum] = true;
	addROI(srcImage, cameraNum);
	if (control == "show")
	{
		showPic(srcImage,cameraNum);
	}
	else if (control == "EVTest")
	{
		showPic(srcImage, cameraNum);
		ExposureAnalysis(srcImage, cameraNum);
	}
	else if (control == "collectEPTimeQue")
	{
		showPic(srcImage, cameraNum);
		collectQueEPPic(cameraNum, srcImage);

	}
	else if (control == "saveAcalibPic")
	{
		if (saveDone[cameraNum] == false)
		{
			if (cameraNum == 0)
			{
				calibACamera.push_back(srcImage);
				saveDone[0] = true;
			}
			else if (cameraNum == 1)
			{
				calibBCamera.push_back(srcImage);
				saveDone[1] = true;
			}
		}
		//˫���ͬ���洢����
		for (int i = 0; i < saveDone.size(); i++)
		{
			if (saveDone[i] == false)
			{
				control = "saveAcalibPic";
				break;
			}
			else
			{
				control = "show";

			}

		}

	}

	if (savenow)
	{
		if (saveDone[cameraNum] == false)
		{
			saveAPic(srcImage, cameraNum);
		}
		for (int i = 0; i < saveDone.size(); i++)
		{
			if (saveDone[i] == false)
			{
				savenow = true;
				break;
			}
			else
			{
				savenow = false;
			}

		}
	}
	//���Ŀǰ���ڲɼ���������ʱ��ѭ����زɼ�״̬
	if (isCollecting)
	{
		Timer* workThread;
		workThread = new Timer(cameraNum, 1);
		connect(workThread, SIGNAL(timerOut(int)), this, SLOT(timeOutSlot(int)));
		workThread->setAutoDelete(true);
		if (cameraNum == 0)
		{
			timerThreadPoll0.start(workThread);
		}
		else
		{
			timerThreadPoll1.start(workThread);
		}
	}


	captureHandler[cameraNum]->pause.tryLock();
	captureHandler[cameraNum]->pause.unlock();



	//if (cameraNum == 0)
	//{
	//	scream[0]->pause.tryLock();
	//	scream[0]->pause.unlock();
	//}
	//else if(cameraNum == 1)
	//{
	//	scream[1]->pause.tryLock();
	//	scream[1]->pause.unlock();
	//}
	//��ͼƬ�������֮ǰ��ֹ��һ�����


}

void Camera_GUI::SreamNoSignal(cv::Mat srcImage, int cameraNum)
{
	//Timer* workThread;
	//if (cameraNum == 0&&!Cam0InRestart)
	//{
	//	workThread=new Timer(0, 3);
	//	connect(workThread, SIGNAL(timerOut(int)), this, SLOT(timeOutSlot(int)));
	//	Cam0InRestart = true;
	//	workThread->setAutoDelete(true);
	//	timerThreadPoll.start(workThread);
	//}
	//else if(cameraNum == 1 && !Cam1InRestart)
	//{
	//	workThread = new Timer(1, 3);
	//	connect(workThread, SIGNAL(timerOut(int)), this, SLOT(timeOutSlot(int)));
	//	Cam1InRestart = true;
	//	workThread->setAutoDelete(true);
	//	timerThreadPoll.start(workThread);
	//}

	//addROI(srcImage, cameraNum);
	showPic(srcImage, cameraNum);
}

void Camera_GUI::reciveEVAnalysData(double overEVPercent, double underEVPercent,int cameraNum)
{
	if (cameraNum == 0)
	{
		ui.progressBar->setValue(overEVPercent*100);
		ui.progressBar->setFormat(QString("���ع��") + QString::number(overEVPercent * 100, 10, 2));
		ui.progressBar_2->setValue(underEVPercent*100);
		ui.progressBar_2->setFormat(QString("Ƿ�ع��") + QString::number(underEVPercent * 100, 10, 2));
		if (saveAnalysisDataEnable)
		{

			
			explosePercent1.push_back(overEVPercent);
			unExplosePercent1.push_back(underEVPercent);
			saveAnalysisDataEnable = false;

		}
		if (explosePercent1.size() < explosePercent2.size())
		{
			explosePercent1.push_back(overEVPercent);
			unExplosePercent1.push_back(underEVPercent);
		}
	}
	else if (cameraNum == 1)
	{
		ui.progressBar_3->setValue(overEVPercent*100);
		ui.progressBar_3->setFormat(QString("���ع��") + QString::number(overEVPercent * 100, 10, 2));
		ui.progressBar_4->setValue(underEVPercent*100);
		ui.progressBar_4->setFormat(QString("Ƿ�ع��") + QString::number(underEVPercent * 100, 10, 2));
		if (saveAnalysisDataEnable)
		{
			explosePercent2.push_back(overEVPercent);
			unExplosePercent2.push_back(underEVPercent);
			saveAnalysisDataEnable = false;

		}
		if (explosePercent1.size() > explosePercent2.size())
		{
			explosePercent2.push_back(overEVPercent);
			unExplosePercent2.push_back(underEVPercent);
		}
	}


}

void Camera_GUI::reciveCalibMessage(QString message)
{
	//��ʾ�궨����
	ui.textBrowser->clear();
	ui.textBrowser->append(message);
}


void Camera_GUI::openMatlabEngin()
{
	ui.pushButton_11->setEnabled(false);
	ep = engOpen(NULL);
	ui.pushButton_11->setText("�ر�matlab����");
	disconnect(ui.pushButton_11, SIGNAL(clicked()), this, SLOT(openMatlabEngin()));
	connect(ui.pushButton_11, SIGNAL(clicked()), this, SLOT(closeMatlabEngin()));
	ui.pushButton_11->setEnabled(true);

}

void Camera_GUI::closeMatlabEngin()
{

	engEvalString(ep, "close;");//�رջ�ͼ����
	engClose(ep);// �ͷ��ڴ�ռ䣬�رռ�������
	ui.pushButton_11->setText("��matlab����");
	disconnect(ui.pushButton_11, SIGNAL(clicked()), this, SLOT(closeMatlabEngin()));
	connect(ui.pushButton_11, SIGNAL(clicked()), this, SLOT(openMatlabEngin()));
}

void Camera_GUI::reciveEVPic(cv::Mat EVPic, int cameraNum)
{
	if (cameraNum == 0)
	{
		EVpic0 = &EVPic;
		EVpic0Enable = true;
	}
	else if (cameraNum == 1)
	{
		EVpic1 = &EVPic;
		EVpic1Enable = true;
	}
}

void Camera_GUI::collectQueEPTimePic_BT()
{
	EPTime *w1 = new EPTime(this);
	w1->setAttribute(Qt::WA_DeleteOnClose);
	w1->show();

	//minEPTime = 1000;
	//maxEPTime = 100000;
	//increaseStep = 1000;
	//ui.textBrowser->clear();
	//ui.textBrowser->append(QString("�ع�ʱ�䣺"));
	//ui.textBrowser->append(QString::fromStdString(to_string(minEPTime)));

	for (int i = 0; i < saveDone.size(); i++)
	{
		saveDone[i] = false;
	}


}

void Camera_GUI::lightControl_BT()
{
	controlA = new lightControl(this);
	controlA->setAttribute(Qt::WA_DeleteOnClose);
	controlA->show();
}

void Camera_GUI::on_pushButton_16_clicked()
{
	controlB = new DLPC_GUI();
	controlB->setAttribute(Qt::WA_DeleteOnClose);
	controlB->show();
}

void Camera_GUI::timeOutSlot(int type)
{
	if (isCollecting)
	{
		if (screamIsProcessing[type])
		{
			screamIsProcessing[type] = false;
		}
		else
		{
			if (cameraRestart(type))
			{

			}
			else
			{
				Timer* workThread;
				workThread = new Timer(type, 1);
				connect(workThread, SIGNAL(timerOut(int)), this, SLOT(timeOutSlot(int)));
				workThread->setAutoDelete(true);
				if (type == 0)
				{
					timerThreadPoll0.start(workThread);

				}
				else
				{
					timerThreadPoll1.start(workThread);
				}

				//������ʾ���ڻ���
				Size textSize = cv::getTextSize("NO SIGNAL!", cv::FONT_HERSHEY_COMPLEX, 2, 2, int());
				Mat temp = Mat::zeros(Size(640, 480), CV_8UC1);
				cv::putText(temp, "NO SIGNAL!", Point(temp.cols / 2 - textSize.width / 2, temp.rows / 2 + textSize.height / 2), 2, 2, cv::Scalar(255));
				emit(nosignal(temp, int(type)));
			}

		}
	}


}




//�ڶ�Ӧ�ؼ�����ʾͼƬ


//Mat Camera_GUI::QImage2cvMat(QImage image)
//{
//	Mat dstMat;
//	switch (image.format())
//	{
//	case QImage::Format_ARGB32:
//	case QImage::Format_RGB32:
//	case QImage::Format_ARGB32_Premultiplied:
//		dstMat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
//		break;
//	case QImage::Format_RGB888:
//		dstMat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
//		cv::cvtColor(dstMat, dstMat, CV_BGR2RGB);
//		break;
//	case QImage::Format_Indexed8:
//		dstMat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
//		break;
//	}
//	return dstMat;
//}

//�洢һ��ͼƬ


void Camera_GUI::Mat2QImage(Mat &srcMat, QImage &dstQimage)
{
	if (srcMat.channels() == 1)
	{
		dstQimage = QImage((const unsigned char*)(srcMat.data), srcMat.cols, srcMat.rows, static_cast<int>(srcMat.step), QImage::Format_Indexed8);
	}
	else
	{
		dstQimage = QImage((const unsigned char*)(srcMat.data), srcMat.cols, srcMat.rows, srcMat.cols*srcMat.channels(), QImage::Format_BGR888);

	}
}

void Camera_GUI::stopCollect()
{

	//for (int i = 0; i < objDevicePtrQue.size(); i++)
	//{
	//	scream[i]->pause.tryLock();
	//	scream[i]->pause.unlock();
	//	scream[i]->requestInterruption();
	//	scream[i]->wait();
	//	//scream[i]->pause.~QMutex();
	//}
	isCollecting = false;
	for (int i = 0; i < objDevicePtrQue.size(); i++)
	{

		
		objFeatureControlPtrQue[i]->GetCommandFeature("AcquisitionStop")->Execute();
		


	}
	timerThreadPoll0.clear();
	timerThreadPoll1.clear();
	timerThreadPoll0.waitForDone(-1);
	timerThreadPoll1.waitForDone(-1);
	emit(nosignal(noSignal, 0));
	emit(nosignal(noSignal, 1));
}

void Camera_GUI::startCollect()
{
	//��ʼ�ɼ���־λ
	isCollecting = true;
	if (CorU == C_COLOUR)
	{
		for (int i = 0; i < objStreamPtrQue.size(); i++)
		{
			//�������
			objImageProcessConfigPtrQue[i]->Reset();
			//0������û�б仯
			//����0����������
			//С��0����С����
			objImageProcessConfigPtrQue[i]->SetLightnessParam(60);
			//ͼ��ת
			objImageProcessConfigPtrQue[i]->EnableConvertFlip(true);
			//64�����Ͷ�û�б仯
			//����64�����ӱ��Ͷ�
			//	С��64����С���Ͷ�
			//	128�����Ͷ�Ϊ��ǰ����
			//	0���ڰ�ͼ��
			objImageProcessConfigPtrQue[i]->SetSaturationParam(80);
			//����������Чλ
			if (RG8212 == C_RG12)
			{

				objImageProcessConfigPtrQue[i]->SetValidBit(GX_BIT_4_11);
			}
			else
			{
				objImageProcessConfigPtrQue[i]->SetValidBit(GX_BIT_0_7);
			}

		}
	}
	//���̲߳ɵ���
	//for (int i = 0; i < objDevicePtrQue.size(); i++)
	//{
	//	scream[i]->start();
	//	saveDone.push_back(false);
	//	savePicCount.push_back(int(0));
	//}
	//���òɼ��ٶȼ���


	//�ص��ɼ����Ϳ�������
	for (int i = 0; i < objDevicePtrQue.size(); i++)
	{

		objFeatureControlPtrQue[i]->GetCommandFeature("AcquisitionStart")->Execute();
		savePicCount[i] = 0;
	}
}



void Camera_GUI::saveAPic(Mat srcImage, int cameraNum)
{
	//��ָ��Roi����Ϳ��
	Rect *dstRoi;
	Mat *srcEVPic;
	//���������ͬ������ָ�븳ֵ
	if (cameraNum == 1)
	{
		dstRoi = &ROIRect2;
		srcEVPic = EVpic1;
	}
	else
	{
		dstRoi = &ROIRect;
		srcEVPic = EVpic0;
	}
	//�������ROI����Է�ROI����Ϳ��
	if (dstRoi->area() != 0)
	{
		Mat showMask = Mat(srcImage.rows, srcImage.cols, srcImage.type(), Scalar::all(255));
		Mat RoiMask = showMask(*dstRoi);
		RoiMask.setTo(Scalar::all(0));
		subtract(srcImage, showMask, srcImage);
	}
	//����������ع�ͼƬ��ͼƬ���½�����ع�ʱ������
	if (control == "collectEPTimeQue")
	{
		char* chCode = new char[20];
		sprintf(chCode, "%.2lf us", minEPTime);
		putText(srcImage, chCode, Point(srcImage.size().width - 200, srcImage.size().height - 50), FONT_HERSHEY_SIMPLEX, 1.0, Scalar::all(255), 2);
	}
	
	//cout << "��ʼ�ɼ�ͼƬ" << endl;
;	//�洢ͼƬ��Ŀ¼
	string txtfileadr;
	txtfileadr = dirName + '/' + picHeadName +to_string(cameraNum+1)+"Namelist.txt";
	ofstream fout(txtfileadr, ios::app);
	fout << picHeadName << cameraNum + 1 << "_" << savePicCount[cameraNum] + 1 << ".png" << endl;
	fout.close();
	//�洢ͼƬROI
	txtfileadr = dirName + '/' + picHeadName + to_string(cameraNum + 1)+ "ROI.txt";
	fout.open(txtfileadr, ios::app);
	if (fout.is_open() )
	{
		fout << picHeadName << cameraNum + 1 << "_" << savePicCount[cameraNum] + 1 << "ROI" << "(" << dstRoi->x << "," << dstRoi->y << "," << dstRoi->width << "," << dstRoi->height << ")" << endl;
	}
	//�洢ͼƬROI
	string filename;
	filename = dirName + '/' + picHeadName + to_string(cameraNum + 1) + '_' + to_string(savePicCount[cameraNum] + 1) + ".png";
	imwrite(filename, srcImage);

	string outPutText;
	outPutText += " �Ѿ������" + to_string(savePicCount[cameraNum] + 1);
	outPutText += " ��Camera" + to_string(cameraNum + 1);
	outPutText += "��ͼƬ������λ�ã�\n";
	outPutText += filename +'\n';
	ui.textBrowser->append(QString::fromStdString(outPutText));
	savePicCount[cameraNum]++;
	//�����ɼ�
	saveDone[cameraNum] = true;
	

}

void Camera_GUI::showPic(Mat srcImage, int cameraNum)
{
	Mat temp = srcImage.clone();
	Mat showPic;
	MyScene * dstSence;
	QGraphicsView *dstGrapgicsView;
	Mat *srcEVpic;
	Rect * srcROI;
	if (cameraNum == 0)
	{
		dstSence = &scene1;
		dstGrapgicsView = ui.graphicsView;
		srcEVpic = EVpic0;
		srcROI = &ROIRect;
	}
	else if (cameraNum == 1)
	{
		dstSence = &scene2;
		dstGrapgicsView = ui.graphicsView_2;
		srcEVpic = EVpic1;
		srcROI = &ROIRect2;
	}
	//if (control == "EVTest")
	//{
	//	addEVpic(*srcEVpic, temp, *srcROI);
	//}
	//int Gwidth, Gheight;
	//int gx, gy;
	//ui.graphicsView->geometry().getRect(&gx, &gy, &Gwidth, &Gheight);
	;

	double scale = ((double)temp.cols/ ui.graphicsView->width() > (double)temp.rows/ ui.graphicsView->height()) ? ((ui.graphicsView->width() -5)/ (double)temp.cols) : ((ui.graphicsView->height()-5)/ (double)temp.rows);
	if (scale < 10)
	{
		cv::resize(temp, showPic, Size(/*ui.graphicsView->width(), ui.graphicsView->height()*/), scale, scale, INTER_NEAREST);
		dstSence->setSceneRect(QRect((qreal)0, (qreal)0, showPic.cols, showPic.rows));
		QImage output;
		Mat2QImage(showPic, output);
		ui.graphicsView->width();
		ui.graphicsView->height();
		dstSence->clear();
		dstSence->addPixmap(QPixmap::fromImage(output));
	}
	

	//if (cameraNum == 0)
	//{
	//	ui.label_5->setPixmap(QPixmap::fromImage(output));
	//	ui.label_5->resize(output.size());
	//	ui.label_5->show();

	//}
	//dstGrapgicsView->setScene(dstSence);
	//dstGrapgicsView->show();
	
}

void Camera_GUI::addROI(cv::Mat& srcImage, int cameraNum)
{
	MyScene * dstScene;
	Rect *dstRoi;


	if (cameraNum == 1)
	{
		dstScene = &scene2;
		dstRoi = &ROIRect2;
	}
	else
	{
		dstScene = &scene1;
		dstRoi = &ROIRect;
	}

	if (dstScene->draging)
	{

		double scale = ((double)srcImage.cols / ui.graphicsView->width() > (double)srcImage.rows / ui.graphicsView->height()) ? ((ui.graphicsView->width() - 5) / (double)srcImage.cols) : ((ui.graphicsView->height() - 5) / (double)srcImage.rows);
		//��¼��ѡ
		Point A, B;
		A.x = (int)((dstScene->fristPos.rx()) / scale);
		A.y = (int)((dstScene->fristPos.ry()) / scale);
		B.x = (int)((dstScene->secondPos.rx()) / scale);
		B.y = (int)((dstScene->secondPos.ry()) / scale);
		B.x = B.x > srcImage.cols ? srcImage.cols : B.x;
		B.y = B.y > srcImage.rows ? srcImage.rows : B.y;

		
		*dstRoi = Rect(A, B);

		
	}


	//ȷ���Ƿ������Roi���ź�
	if (dstScene->cleanRoi)
	{
		*dstRoi = Rect(0, 0, 0, 0);
		dstScene->cleanRoi = false;
	}
	//��Roi����ʱ���ROI
	if (dstRoi->area() != 0)
	{

		//������ģ
		Mat showMask = Mat(srcImage.rows, srcImage.cols, srcImage.type(), Scalar::all(50));
		Mat RoiMask = showMask(*dstRoi);
		RoiMask.setTo(Scalar::all(0));
		//��ʾRoi
		subtract(srcImage, showMask, srcImage);

	}



}

void Camera_GUI::ExposureAnalysis(Mat srcImage, int cameraNum)
{
	//���̳߳ؿ���һ�������߳�;
	CameraEVTest *workThread;
	if (ROIRect.area() != 0 && cameraNum == 0)
	{

			workThread = new CameraEVTest(srcImage, cameraNum,ROIRect);
	}
	else if (ROIRect2.area() != 0 && cameraNum == 1)
	{
		workThread = new CameraEVTest(srcImage, cameraNum, ROIRect2);
	}
	else
	{
		workThread = new CameraEVTest(srcImage, cameraNum);
	}
	workThread->setAutoDelete(true);
	connect(workThread, SIGNAL(sendAdata(double, double,int)), this, SLOT(reciveEVAnalysData(double, double,int)));
	//connect(workThread, SIGNAL(sendAPic(cv::Mat, int)), this, SLOT(reciveEVPic(cv::Mat, int)));
	pool.start(workThread);
}

void Camera_GUI::saveExposureData()
{
	if (explosePercent1.size() != 0 && explosePercent2.size() != 0 && unExplosePercent1.size() != 0 && unExplosePercent2.size() != 0)
	{
		//��ȡʱ��
		tm t;
		time_t now;
		char timeChar[256] = { 0 };
		time(&now);
		localtime_s(&t, &now);
		strftime(timeChar, sizeof(timeChar), "%H-%M-%S", &t);

		string txtfileadr;
		txtfileadr = dirName + '/' + "ExposureData" + timeChar + ".txt";
		ofstream fout(txtfileadr, ios::app);
		fout << "A����ع������\n";
		fout << "���ع⣺\n";
		for (int i = 0; i < explosePercent1.size(); i++)
		{
			fout << i << ':' << explosePercent1[i]*100 << '\n';
		}
		fout << "Ƿ�ع⣺\n";
		for (int i = 0; i < unExplosePercent1.size(); i++)
		{
			fout << i << ':' << unExplosePercent1[i]*100 << '\n';
		}

		fout << "B����ع������\n";
		fout << "���ع⣺\n";
		for (int i = 0; i < explosePercent2.size(); i++)
		{
			fout << i << ':' << explosePercent2[i]*100 << '\n';
		}
		fout << "Ƿ�ع⣺\n";
		for (int i = 0; i < unExplosePercent2.size(); i++)
		{
			fout << i << ':' << unExplosePercent2[i]*100 << '\n';
		}
		fout.close();
	}

}

bool Camera_GUI::getFiles(string path, vector<string>& files)
{
	std::experimental::filesystem::path str(path+"\\");
	//���·���Ƿ����
	if (exists(str))
	{
		std::experimental::filesystem::directory_entry entry(str);
		//���·���Ƿ���һ��Ŀ¼

		std::experimental::filesystem::directory_iterator list(str);
		for (auto& it : list)
		{
			files.push_back(it.path().string());
		}
				

	}
	else
	{
		return false;
	}


	return true;


}

void Camera_GUI::addEVpic(cv::Mat & EVPic, cv::Mat & srcPic, Rect srcROI)
{
	if (EVpic0Enable&&EVpic1Enable)
	{
		if (CorU == C_GRAY)
		{
			cvtColor(srcPic, srcPic, COLOR_GRAY2BGR);
		}

		EVPic.copyTo(srcPic(srcROI));
	}
}

void Camera_GUI::collectQueEPPic(int cameraNum, Mat & srcImage)
{
	if (picStep >= 20)
	{
		
		CFloatFeaturePointer objFloatPtr = objFeatureControlPtrQue[cameraNum]->GetFloatFeature("ExposureTime");
		if (minEPTime <= maxEPTime)
		{
			if (minEPTime == objFloatPtr->GetValue())
			{
				if (saveDone[cameraNum] == false)
				{
					saveAPic(srcImage, cameraNum);
				}

			}
			for (int i = 0; i < saveDone.size(); i++)
			{
				if (saveDone[i] == false)
				{

					break;
				}
				if (i == (saveDone.size() - 1))
				{
					
					ui.textBrowser->append(QString("�ع�ʱ�䣺"));
					ui.textBrowser->append(QString::fromStdString(to_string(minEPTime)));
					for (int j = 0; j < saveDone.size(); j++)
					{
						saveDone[j] = false;
					}
					minEPTime += increaseStep;
					for (int j = 0; j < objDevicePtrQue.size(); j++)
					{
						objFloatPtr = objFeatureControlPtrQue[j]->GetFloatFeature("ExposureTime");
						objFloatPtr->SetValue(minEPTime);
						picStep = 0;

					}





				}


			}

		}
		else if (minEPTime > maxEPTime)
		{
			control = "show";
			for (int i = 0; i < objDevicePtrQue.size(); i++)
			{
				objFloatPtr = objFeatureControlPtrQue[i]->GetFloatFeature("ExposureTime");
				objFloatPtr->SetValue(10000.0);


			}
		}
	}
	else
	{
		picStep++;
	}


}

void Camera_GUI::creatASaveDir()
{
	//�������ݳ�ʼ��
//�����洢ͼƬ���ļ���
	tm t;
	time_t now;
	char name[256] = { 0 };
	time(&now);
	localtime_s(&t, &now);
	strftime(name, sizeof(name), "Documents\\Data%Y.%m.%d.%H-%M-%S", &t);
	if (!mkdir(name))
	{
		string temp("�Ѵ����洢Ŀ¼��");
		temp += name;
		ui.textBrowser->clear();
		ui.textBrowser->append(QString::fromStdString(temp));
	}
	dirName = string(name);
	//��ɳ�ʼ�������뿪ʼui��̬
}

void Camera_GUI::deleteEmptySaveDir()
{
	//���û�д洢ͼƬ��������δ洢ͼƬ���ļ���
	vector<string> fileslist;
	if (getFiles(dirName, fileslist))
	{
		if (fileslist.size() == 0)
		{
			std::experimental::filesystem::remove_all(std::experimental::filesystem::path(dirName));
			ui.textBrowser->clear();
			ui.textBrowser->append(QString::fromStdString(string("�ڵ�ǰ�洢Ŀ¼���Ҳ����κ��ļ����Ѿ�ɾ��Ŀ¼") + dirName));
		}
	}

}

void Camera_GUI::cameraOffline()
{


	//������ֹ�ɼ��߳�
	stopCollect();
	for (int i = 0; i < objDevicePtrQue.size(); i++)
	{
		captureHandler[i]->pause.tryLock();
		captureHandler[i]->pause.unlock();
		captureHandler[i]->exit.tryLock();
	}
	//�ȴ���ʱ��ȫ���˳�

	//Ȼ��ɾ��ʵ��
	
	for (int i = 0; i < captureHandler.size(); i++)
	{
		delete captureHandler[i];
	}
	//for (int i = 0; i < objDevicePtrQue.size(); i++)
	//{
	//	delete scream[i];
	//}
	//���ע������豸
	if (cameraIsonline)
	{
		
		//���ڹر�ǰ������豸����ע������
		for (int i = 0; i < objDevicePtrQue.size(); i++)
		{

				objStreamPtrQue[i]->StopGrab();
				objStreamPtrQue[i]->UnregisterCaptureCallback();

				
				objStreamPtrQue[i]->Close();
				objDevicePtrQue[i]->Close();

				
				

		}
		IGXFactory::GetInstance().Uninit();
	}

	


}

void Camera_GUI::cameraOnline()
{
	objImageProcessConfigPtrQue.clear();
	objFeatureControlPtrQue.clear();
	objDevicePtrQue.clear();
	objStreamPtrQue.clear();
	CamID.clear();
	captureHandler.clear();
	//����ͷ��ʼ��
	try
	{
		IGXFactory::GetInstance().Init();
	}
	catch (CGalaxyException&e)
	{
		cout << "������: " << e.GetErrorCode() << endl;
		cout << "����������Ϣ: " << e.what() << endl;
	}
	//����豸��Ϣ��
	GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
	//ö���豸��Ϣ 
	IGXFactory::GetInstance().UpdateAllDeviceList(1000, vectorDeviceInfo);
	for (uint32_t i = 0; i < vectorDeviceInfo.size(); i++)
	{
		string outPutText = "���" + to_string(i + 1) + ":\n���쳧�̣�" + string(vectorDeviceInfo[i].GetVendorName()) + "\n�豸�ͺ����ƣ�" + string(vectorDeviceInfo[i].GetModelName()) + "\nDeviceID��" + string(vectorDeviceInfo[i].GetDeviceID()) + "\n�豸���ͣ�" + (vectorDeviceInfo[i].GetDeviceClass() == 1 ? "USB2.0" : (vectorDeviceInfo[i].GetDeviceClass() == 2 ? "USB3.0" : "Giga")) + "\n�豸����״̬��" + (vectorDeviceInfo[i].GetAccessStatus() == 1 ? "����" : "�쳣") + "\n";
		//to_string(vectorDeviceInfo[i].GetModelName());
		CamID.push_back(string(vectorDeviceInfo[i].GetDeviceID()));
		ui.textBrowser->append(QString::fromStdString(outPutText));
	}


	//�����豸
	CGXDevicePointer  objDevicePtr;
	CGXStreamPointer  objStreamPtr;
	CImageProcessConfigPointer objImageProcessConfigPtr;
	for (int i = 0; i < vectorDeviceInfo.size(); i++)
	{
		GxIAPICPP::gxstring strSN = vectorDeviceInfo[i].GetSN();
		objDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(strSN, GX_ACCESS_EXCLUSIVE);
		objStreamPtr = objDevicePtr->OpenStream(0);
		objImageProcessConfigPtr = objDevicePtr->CreateImageProcessConfig();
		objStreamPtrQue.push_back(objStreamPtr);
		objDevicePtrQue.push_back(objDevicePtr);
		objImageProcessConfigPtrQue.push_back(objImageProcessConfigPtr);
		//ע��ص�

		ICaptureEventHandler* pCaptureEventHandler = new CSampleCaptureEventHandler(i);
		connect((CSampleCaptureEventHandler*)pCaptureEventHandler, SIGNAL(getAPic(cv::Mat, int)), this, SLOT(reciveAPicFromSream(cv::Mat, int)));
		connect((CSampleCaptureEventHandler*)pCaptureEventHandler, SIGNAL(nosignal(cv::Mat, int)), this, SLOT(SreamNoSignal(cv::Mat, int)));
		objStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, &objImageProcessConfigPtrQue[i]);
		captureHandler.push_back((CSampleCaptureEventHandler*)pCaptureEventHandler);
	}
	//������ͨ��
	for (int i = 0; i < vectorDeviceInfo.size(); i++)
	{
		objStreamPtrQue[i]->StartGrab();
		CGXFeatureControlPointer objFeatureControlPtr = objDevicePtrQue[i]->GetRemoteFeatureControl();
		objFeatureControlPtrQue.push_back(objFeatureControlPtr);
	}
	cameraIsonline = true;
}

void Camera_GUI::mainWindowInit()
{
	//����ͷ��ʼ��
	try
	{
		IGXFactory::GetInstance().Init();
	}
	catch (CGalaxyException&e)
	{
		cout << "������: " << e.GetErrorCode() << endl;
		cout << "����������Ϣ: " << e.what() << endl;
	}
	//����豸��Ϣ��
	GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
	//ö���豸��Ϣ 
	IGXFactory::GetInstance().UpdateAllDeviceList(1000, vectorDeviceInfo);
	for (uint32_t i = 0; i < vectorDeviceInfo.size(); i++)
	{
		string outPutText = "���" + to_string(i + 1) + ":\n���쳧�̣�" + string(vectorDeviceInfo[i].GetVendorName()) + "\n�豸�ͺ����ƣ�" + string(vectorDeviceInfo[i].GetModelName()) + "\nDeviceID��" + string(vectorDeviceInfo[i].GetDeviceID())+ "\n�豸���ͣ�" + (vectorDeviceInfo[i].GetDeviceClass() == 1 ? "USB2.0" : (vectorDeviceInfo[i].GetDeviceClass() == 2 ? "USB3.0" : "Giga")) + "\n�豸����״̬��" + (vectorDeviceInfo[i].GetAccessStatus() == 1 ? "����" : "�쳣") + "\n";
		//to_string(vectorDeviceInfo[i].GetModelName());
		CamID.push_back(string(vectorDeviceInfo[i].GetDeviceID()));
		ui.textBrowser->append(QString::fromStdString(outPutText));
	}
	

	//�����豸
	CGXDevicePointer  objDevicePtr;
	CGXStreamPointer  objStreamPtr;
	CImageProcessConfigPointer objImageProcessConfigPtr;
	CGXFeatureControlPointer objFeatureControlPtr;
	for (int i = 0; i < vectorDeviceInfo.size(); i++)
	{
		GxIAPICPP::gxstring strSN = vectorDeviceInfo[i].GetSN();
		objDevicePtr = IGXFactory::GetInstance().OpenDeviceBySN(strSN, GX_ACCESS_EXCLUSIVE);
		objStreamPtr = objDevicePtr->OpenStream(0);
		objImageProcessConfigPtr = objDevicePtr->CreateImageProcessConfig();
		objFeatureControlPtr = objDevicePtr->GetRemoteFeatureControl();
		objStreamPtrQue.push_back(objStreamPtr);
		objDevicePtrQue.push_back(objDevicePtr);
		objImageProcessConfigPtrQue.push_back(objImageProcessConfigPtr);
		objFeatureControlPtrQue.push_back(objFeatureControlPtr);
		//ע��ص�
		ICaptureEventHandler* pCaptureEventHandler = new CSampleCaptureEventHandler(i);
		connect((CSampleCaptureEventHandler*)pCaptureEventHandler, SIGNAL(getAPic(cv::Mat, int)), this, SLOT(reciveAPicFromSream(cv::Mat, int)));
		connect((CSampleCaptureEventHandler*)pCaptureEventHandler, SIGNAL(nosignal(cv::Mat, int)), this, SLOT(SreamNoSignal(cv::Mat, int)));
		objStreamPtr->RegisterCaptureCallback(pCaptureEventHandler, &objImageProcessConfigPtrQue[i]);
		captureHandler.push_back((CSampleCaptureEventHandler*)pCaptureEventHandler);
		//�����ٶ�
		CIntFeaturePointer objIntPtr = objFeatureControlPtrQue[i]->GetIntFeature("AcquisitionSpeedLevel");
		objIntPtr->SetValue(1);




	}
	//������ͨ��
	for (int i = 0; i < vectorDeviceInfo.size(); i++)
	{
		objStreamPtrQue[i]->StartGrab();


	

		//���������ͼƬ����
		savePicCount.push_back(int(0));
		//
		saveDone.push_back(false);
		//������ͨ����־λ
		screamIsProcessing.push_back(true);
	}
	cameraIsonline = true;
	//�������ݳ�ʼ��
	//��ʼ����ʾ�Ӵ�ͼ��
	ui.graphicsView->setScene(&scene1);
	ui.graphicsView_2->setScene(&scene2);

	ui.graphicsView->show();
	ui.graphicsView_2->show();

	emit(nosignal(noSignal, 0));
	emit(nosignal(noSignal, 1));
	//�趨�ع�ͳ���̳߳�
	pool.setMaxThreadCount(1);
	//��ʱ����ʼ��
	timerThreadPoll0.setMaxThreadCount(1);
	timerThreadPoll1.setMaxThreadCount(1);
	//






	//��ɳ�ʼ�������뿪ʼui��̬
	windowStateStart();
}

bool Camera_GUI::cameraRestart(int camNum)
{
	ui.textBrowser->clear();
	ui.textBrowser->append(QString("��⵽���")+ QString::number(camNum) + QString("���ߣ�������.....") +QString("��") + QString::number(CamRestartCount) + QString("�γ���....."));
	//�����ɼ���
	//for (int i = 0; i < scream.size(); i++)
	//{
	//	scream[i]->pause.tryLock();
	//}
	//ͣ�ɲ�ע���ɼ��ص�����

	//objFeatureControlPtrQue[camNum]->GetCommandFeature("AcquisitionStop")->Execute();
	//objStreamPtrQue[camNum]->StopGrab();
	//objStreamPtrQue[camNum]->UnregisterCaptureCallback();
	//objStreamPtrQue[camNum]->Close();
	//objDevicePtrQue[camNum]->Close();
	//scream[camNum]->failconnect = true;

	//����ͷ��ʼ��
	//try
	//{
	//	IGXFactory::GetInstance().Init();
	//}
	//catch (CGalaxyException&e)
	//{
	//	cout << "������: " << e.GetErrorCode() << endl;
	//	cout << "����������Ϣ: " << e.what() << endl;
	//}
	//����豸��Ϣ��
	GxIAPICPP::gxdeviceinfo_vector vectorDeviceInfo;
	//ö���豸��Ϣ 
	IGXFactory ::GetInstance().UpdateDeviceList(500, vectorDeviceInfo);
	//�ɹ�ö�ٱ�־λ
	bool getinfo = false;
	int tempindex;
	
	for (int i = 0; i < vectorDeviceInfo.size(); i++)
	{
		string tempID= string(vectorDeviceInfo[i].GetDeviceID());
		if (tempID == CamID[camNum])
		{
			getinfo = true;
			tempindex = i;
		}
		//else
		//{
		//	objDevicePtrQue[i] = IGXFactory::GetInstance().OpenDeviceBySN(strSN, GX_ACCESS_EXCLUSIVE);
		//}
	}
	if (getinfo)
	{
		cameraOffline();
		cameraOnline();
		startCollect();
		//deleteEmptySaveDir();
		ui.textBrowser->append(QString("\n��������ɹ�"));
		CamRestartCount = 0;
		return true;
		
	}
	else
	{
		
		//for (int i = 0; i < scream.size(); i++)
		//{
		//	scream[i]->pause.unlock();
		//}
		ui.textBrowser->append("\n����ʧ��");
		CamRestartCount++;
		return false;

	}

	//if (camNum == 0)
	//{
	//	Cam0InRestart = false;
	//}
	//else
	//{
	//	Cam1InRestart = false;
	//}

}

//void Camera_GUI::serialPortInitail()
//{
//	isOpen = false;
//	//port config
//	global_port.setParity(QSerialPort::NoParity);
//	global_port.setDataBits(QSerialPort::Data8);
//	global_port.setStopBits(QSerialPort::OneStop);
//
//	//connect
//	connect(ui->btn_open, &QPushButton::clicked, this, &MainWindow::openClose);
//	connect(ui->btn_send, &QPushButton::clicked, this, &MainWindow::btn_send_data);
//	connect(&global_port, &QSerialPort::readyRead, this, &MainWindow::receive_data);
//
//
//
//	//ʶ��ϵͳ��Ч���ں�
//	QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
//	for (int i = 0; i < availablePorts.size(); ++i) {
//		ui->cmb_port_name->addItem(availablePorts.at(i).portName());
//	}
//
//}

void Camera_GUI::collect_GUI_Initail()
{
	w1=new Collect_GUI(ROIRect,ROIRect2,this);
	w1->setAttribute(Qt::WA_DeleteOnClose);
	w1->show();
}


void Camera_GUI::guiDateInitialize()
{


	//system(doscommand);
}

void Camera_GUI::windowStateInitial()
{
	//�궨ģʽ�����ͽ�����ť
	ui.pushButton->setEnabled(false);
	//ui.pushButton->setVisible(false);
	ui.pushButton_5->setEnabled(false);
	//ui.pushButton_5->setVisible(false);

	//�ɼ�ģʽ�����ͽ�����ť
	ui.pushButton_4->setEnabled(false);
	/*ui.pushButton_4->setVisible(false);*/
	ui.pushButton_6->setEnabled(false);
	/*ui.pushButton_6->setVisible(false);*/

	//�ɼ�ͼƬ��ť
	ui.pushButton_2->setEnabled(false);
	/*ui.pushButton_2->setVisible(false);*/
	//�궨ͼƬ��ť
	ui.pushButton_7->setEnabled(false);
	/*ui.pushButton_7->setVisible(false);*/
	//�����عⰴť
	ui.pushButton_8->setEnabled(false);
	/*ui.pushButton_8->setVisible(false);*/
	//ֹͣ�����عⰴť
	ui.pushButton_9->setEnabled(false);
	/*ui.pushButton_9->setVisible(false);*/
	//Ƿ���ع�ٷֱ���
	/*ui.progressBar->setVisible(false);*/
	ui.progressBar->setEnabled(false);
	/*ui.progressBar_2->setVisible(false);*/
	ui.progressBar_2->setEnabled(false);
	/*ui.progressBar_3->setVisible(false);*/
	ui.progressBar_3->setEnabled(false);
	/*ui.progressBar_4->setVisible(false);*/
	ui.progressBar_4->setEnabled(false);
	//��¼�ع����ݰ�ť
	ui.pushButton_10->setEnabled(false);
	//ui.pushButton_10->setVisible(false);

	ui.pushButton_3->setEnabled(false);


}

void Camera_GUI::windowStateStart()
{
	//�궨ģʽ�����ͽ�����ť
	ui.pushButton->setEnabled(true);
	/*ui.pushButton->setVisible(true);*/
	ui.pushButton_5->setEnabled(false);
	/*ui.pushButton_5->setVisible(false);*/
	
	//�ɼ�ģʽ�����ͽ�����ť
	ui.pushButton_4->setEnabled(true);
	//ui.pushButton_4->setVisible(true);
	ui.pushButton_6->setEnabled(false);
	//ui.pushButton_6->setVisible(false);

	//�ɼ�ͼƬ��ť
	ui.pushButton_2->setEnabled(false);
	//ui.pushButton_2->setVisible(true);
	//�궨ͼƬ��ť
	ui.pushButton_7->setEnabled(false);
	//ui.pushButton_7->setVisible(false);
	//�����عⰴť
	ui.pushButton_8->setEnabled(false);
	//ui.pushButton_8->setVisible(false);
	//ֹͣ�����عⰴť
	ui.pushButton_9->setEnabled(false);
	//ui.pushButton_9->setVisible(false);
	//Ƿ���ع�ٷֱ���
	//ui.progressBar->setVisible(false);
	ui.progressBar->setEnabled(false);
	//ui.progressBar_2->setVisible(false);
	ui.progressBar_2->setEnabled(false);
	//ui.progressBar_3->setVisible(false);
	ui.progressBar_3->setEnabled(false);
	//ui.progressBar_4->setVisible(false);
	ui.progressBar_4->setEnabled(false);
	//��¼�ع����ݰ�ť
	ui.pushButton_10->setEnabled(false);
	//ui.pushButton_10->setVisible(false);
	//��¼ͼƬ��ť
	ui.pushButton_12->setEnabled(false);
	//ui.pushButton_12->setVisible(false);
	//�����Խ���ť
	ui.pushButton_13->setEnabled(false);
	//ui.pushButton_13->setVisible(false);
	//�����ع�ɼ���ť
	ui.pushButton_14->setEnabled(false);
	//ui.pushButton_14->setVisible(false);
	//��Դ����


	ui.pushButton_3->setEnabled(true);
	

	ui.comboBox->setEnabled(true);
	ui.comboBox_2->setEnabled(true);


}

void Camera_GUI::windowStateCollect()
{
	//�궨ģʽ�����ͽ�����ť
	ui.pushButton->setEnabled(false);
	//ui.pushButton->setVisible(false);
	ui.pushButton_5->setEnabled(false);
	//ui.pushButton_5->setVisible(false);
	//�ɼ�ģʽ�����ͽ�����ť
	ui.pushButton_4->setEnabled(false);
	//ui.pushButton_4->setVisible(false);
	ui.pushButton_6->setEnabled(true);
	//ui.pushButton_6->setVisible(true);

	//�ɼ�ͼƬ��ť
	ui.pushButton_2->setEnabled(true);
	//ui.pushButton_2->setVisible(true);
	//�궨ͼƬ��ť
	ui.pushButton_7->setEnabled(false);
	//ui.pushButton_7->setVisible(false);
	//�����عⰴť
	ui.pushButton_8->setEnabled(true);
	//ui.pushButton_8->setVisible(true);
	//ֹͣ�����عⰴť
	ui.pushButton_9->setEnabled(false);
	//ui.pushButton_9->setVisible(false);
	//Ƿ���ع�ٷֱ���
	//ui.progressBar->setVisible(false);
	ui.progressBar->setEnabled(false);
	//ui.progressBar_2->setVisible(false);
	ui.progressBar_2->setEnabled(false);
	//ui.progressBar_3->setVisible(false);
	ui.progressBar_3->setEnabled(false);
	//ui.progressBar_4->setVisible(false);
	ui.progressBar_4->setEnabled(false);

	//��¼�ع����ݰ�ť
	ui.pushButton_10->setEnabled(false);
	//ui.pushButton_10->setVisible(false);
	//��¼ͼƬ��ť
	ui.pushButton_12->setEnabled(true);
	//ui.pushButton_12->setVisible(true);
	//�����Խ���ť
	ui.pushButton_13->setEnabled(true);
	//ui.pushButton_13->setVisible(true);
	//�����ع�ɼ���ť
	ui.pushButton_14->setEnabled(true);
	//ui.pushButton_14->setVisible(true);


	ui.pushButton_3->setEnabled(true);


	ui.comboBox->setEnabled(false);
	ui.comboBox_2->setEnabled(false);


}

void Camera_GUI::windowStateCalib()
{
	//�궨ģʽ�����ͽ�����ť
	ui.pushButton->setEnabled(false);
	//ui.pushButton->setVisible(false);
	ui.pushButton_5->setEnabled(true);
	//ui.pushButton_5->setVisible(true);

	//�ɼ�ģʽ�����ͽ�����ť
	ui.pushButton_4->setEnabled(true);
	//ui.pushButton_4->setVisible(true);
	ui.pushButton_6->setEnabled(false);
	//ui.pushButton_6->setVisible(false);
	//�ɼ�ͼƬ��ť
	ui.pushButton_2->setEnabled(false);
	//ui.pushButton_2->setVisible(false);
	//�궨ͼƬ��ť
	ui.pushButton_7->setEnabled(true);
	//ui.pushButton_7->setVisible(true);
	//�����عⰴť
	ui.pushButton_8->setEnabled(false);
	//ui.pushButton_8->setVisible(false);
	//ֹͣ�����عⰴť
	ui.pushButton_9->setEnabled(false);
	//ui.pushButton_9->setVisible(false);
	//Ƿ���ع�ٷֱ���
	//ui.progressBar->setVisible(false);
	ui.progressBar->setEnabled(false);
	//ui.progressBar_2->setVisible(false);
	ui.progressBar_2->setEnabled(false);
	//ui.progressBar_3->setVisible(false);
	ui.progressBar_3->setEnabled(false);
	//ui.progressBar_4->setVisible(false);
	ui.progressBar_4->setEnabled(false);
	//��¼ͼƬ��ť
	ui.pushButton_12->setEnabled(false);
	//ui.pushButton_12->setVisible(false);
	//�����Խ���ť
	ui.pushButton_13->setEnabled(true);
	//ui.pushButton_13->setVisible(true);
	//�����ع�ɼ���ť
	ui.pushButton_14->setEnabled(false);
	//ui.pushButton_14->setVisible(false);

	ui.pushButton_3->setEnabled(true);


	ui.comboBox->setEnabled(true);
	ui.comboBox_2->setEnabled(true);


}
