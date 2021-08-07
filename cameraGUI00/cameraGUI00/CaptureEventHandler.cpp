#include "CaptureEventHandler.h"

CSampleCaptureEventHandler::CSampleCaptureEventHandler(int _camNum, int _colour, int _RG812)
{
	colour = _colour;
	RG8212 = _RG812;
	camNum = _camNum;
}

void CSampleCaptureEventHandler::DoOnImageCaptured(CImageDataPointer & objImageDataPtr, void * pUserParam)
{

	/*cout << "收到一帧图像!" << endl;

	cout << "ImageInfo: " << objImageDataPointer->GetStatus() << endl;

	cout << "ImageInfo: " << objImageDataPointer->GetWidth() << endl;

	cout << "ImageInfo: " << objImageDataPointer->GetHeight() << endl;

	cout << "ImageInfo: " << objImageDataPointer->GetPayloadSize() << endl;*/
	//int camNum = *(int *)pUserParam;
	if (exit.tryLock())
	{
		exit.unlock();
	}
	else
	{
		return;
	}

	pause.lock();

	CImageProcessConfigPointer* ProcessConfigPtr = (CImageProcessConfigPointer*)pUserParam;
	if (objImageDataPtr->GetStatus() == GX_FRAME_STATUS_SUCCESS)
	{
		//char cameraNum[10];
		char picNum[10];
		char windowsName[256] = { 0 };
		//彩色采集
		if (colour==1)
		{
			Mat imgRGB(objImageDataPtr->GetHeight(), objImageDataPtr->GetWidth(), CV_8UC3);
			void *pRGB24Buffer = NULL;
			pRGB24Buffer = objImageDataPtr->ImageProcess(*ProcessConfigPtr);
			//信号传出图片数据
			memcpy(imgRGB.data, pRGB24Buffer, (objImageDataPtr->GetHeight()) * (objImageDataPtr->GetWidth()) * 3);

			////锁定主窗口读写权
			//QMutexLocker lockGUI0Data(&(GUI0->picQueDataOperate));
			//GUI0->picQue++;
			//lockGUI0Data.unlock();
			emit(getAPic(imgRGB.clone(), int(camNum)));

		}
		else//灰度采集
		{
			void* pRaw8Buffer = NULL;
			if (RG8212==0)
			{

				pRaw8Buffer = objImageDataPtr->GetBuffer();

			}
			else
			{
				pRaw8Buffer = objImageDataPtr->ConvertToRaw8(GX_BIT_4_11);
			}
			emit(getAPic(Mat(objImageDataPtr->GetHeight(), objImageDataPtr->GetWidth(), CV_8UC1, pRaw8Buffer).clone(), int(camNum)));
		}

	}
	else
	{
		Size textSize = cv::getTextSize("NO SIGNAL!", cv::FONT_HERSHEY_COMPLEX, 2, 2, int());
		Mat temp = Mat::zeros(Size(640, 480), CV_8UC1);
		cv::putText(temp, "NO SIGNAL!", Point(temp.cols / 2 - textSize.width / 2, temp.rows / 2 + textSize.height / 2), 2, 2, cv::Scalar(255));
		emit(nosignal(temp, int(camNum)));
		//emit(restart(cameraNum));
	}
}
