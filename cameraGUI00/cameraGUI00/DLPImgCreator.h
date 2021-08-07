#pragma once
#include "BasicFunc.h"
class DLPImgCreator :
	public BasicFunc
{
public:
	DLPImgCreator(Size _imgSize);
	~DLPImgCreator();
	void creatCLPic(int step, int maxHeight, vector<Mat> & outputArray);
	Mat creatCLPic(int maxHeight);
private:
	Size imgSize;


};

