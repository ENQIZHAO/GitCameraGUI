#include "EVTest.h"
CameraEVTest::CameraEVTest(Mat & srcImage, int _cameraNum)
{
	cameraNum = _cameraNum;
	dstImage = srcImage.clone();
}
CameraEVTest::CameraEVTest(Mat & srcImage, int _cameraNum, Rect _ROI)
{
	cameraNum = _cameraNum;
	dstImage = srcImage.clone();
	ROI = _ROI;
}

void CameraEVTest::run()
{
	//只有当像素格式是CV_8UC1的情况下才做处理
	if (dstImage.type() == 0)
	{
		Mat overEVPic, underEVPic,oneMask;
		double overEVPixels, underEVPixels;
		double pixelCount;
		Mat colour;
		if (ROI.area() == 0)
		{
			//得到和原图同尺寸的1矩阵掩模
			oneMask = Mat(dstImage.size(), dstImage.type(), Scalar::all(1));


			//得到过曝光二值图
			threshold(dstImage, overEVPic, 240, 1, THRESH_BINARY);
			//得到欠曝光二值图
			threshold(dstImage, underEVPic, 10, 1, THRESH_BINARY_INV);
		
			//得到过曝光和欠曝光像素个数
			overEVPixels = oneMask.dot(overEVPic);
			underEVPixels = oneMask.dot(underEVPic);
			pixelCount = dstImage.cols*dstImage.rows;




		}
		else if (ROI.area() != 0)
		{

			Mat ROIdstMat = dstImage(ROI);
			oneMask = Mat(ROIdstMat.size(), ROIdstMat.type(), Scalar::all(1));
			threshold(ROIdstMat, overEVPic, 240, 1, THRESH_BINARY);
			threshold(ROIdstMat, underEVPic, 10, 1, THRESH_BINARY_INV);
			overEVPixels = oneMask.dot(overEVPic);
			underEVPixels = oneMask.dot(underEVPic);
			pixelCount = ROIdstMat.cols*ROIdstMat.rows;
			
			Mat overEVPicC3, underEVPicC3;
			cvtColor(overEVPic, overEVPicC3, COLOR_BGRA2BGR);
			cvtColor(underEVPic, underEVPicC3, COLOR_BGRA2BGR);
			
			//redMask = Mat(ROIdstMat.size(), CV_8UC3, Scalar(0, 0, 255));
			//blueMask = Mat(ROIdstMat.size(), CV_8UC3, Scalar(255, 0,0 ));
			//whiteMask = Mat(ROIdstMat.size(), CV_8UC3, Scalar(255, 255, 255));
			//Zero = Mat(ROIdstMat.size(), CV_8UC3, Scalar(0, 0, 0));
			//blue2RedMask = Mat(ROIdstMat.size(), CV_8UC3, Scalar(255,255, 255));


			//bitwise_and(redMask, overEVPicC3*255, redMask);
			//bitwise_and(blueMask, underEVPicC3*255, blueMask);
			//bitwise_xor(blue2RedMask,(overEVPicC3+underEVPicC3)*255,blue2RedMask);
			//bitwise_and(whiteMask, blue2RedMask, whiteMask);

			//colour = whiteMask + blueMask + redMask;
		}

		


		double overEVPercent, underEVPercent;
		overEVPercent = overEVPixels / pixelCount;
		underEVPercent = underEVPixels / pixelCount;
		//发送结果
		emit(sendAdata(overEVPercent, underEVPercent,cameraNum));
		//emit(sendAPic(colour.clone(), cameraNum));
	}
	

}
