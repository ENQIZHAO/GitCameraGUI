#include "DLPImgCreator.h"

DLPImgCreator::DLPImgCreator(Size _imgSize)
{
	imgSize = _imgSize;
}

DLPImgCreator::~DLPImgCreator()
{
}

void DLPImgCreator::creatCLPic(int step, int maxHeight, vector<Mat> & outputArray)
{
	if (maxHeight*2 > imgSize.height)
	{
		cout << "Be sure' maxHeight*2 <imgSize.height' " << endl;
		throw(0);
	}
	Mat temp = Mat(imgSize, CV_8UC3, Scalar::all(0));
	for (int i = step; i < maxHeight; i = i + step)
	{
		Mat templight = temp(Rect(0, imgSize.height/2-i, imgSize.width, i*2));
		templight.setTo(Scalar::all(255));
		outputArray.push_back(temp.clone());
	}


}

Mat DLPImgCreator::creatCLPic(int step)
{
	if (step*24>imgSize.height/2)
	{
		cout << "Be sure' maxHeight/step<=24' " << endl;
		throw(0);
	}
	Mat temp = Mat(imgSize, CV_8UC3, Scalar::all(0));
	for (int i =1; i <= 24; i++)
	{
		Scalar pixelData;
		if (i<=8)
		{
			pixelData = Scalar(0, 0, pow(2, (i - 1)));
		}
		else if (i>8&&i<=16)
		{
			pixelData = Scalar(0, pow(2, (i - 9)), 0);
		}
		else if (i>16)
		{
			pixelData = Scalar(pow(2, (i - 17)), 0,0 );
		}
		Mat templight = temp(Rect(0, imgSize.height / 2 - i*step, imgSize.width, i * 2*step));
		Mat add = Mat(templight.size(), CV_8UC3, pixelData);
		templight = templight + add;
		//templight.setTo(Scalar::all(255));
	}


	return temp.clone();
}

