#include "Calib.h"


CalibThread::CalibThread(vector<Mat>& src1, vector<Mat>& src2, Size _chessBoardSize, Size _chessBoardRectSize)
{
	//接受标定数据
	calibACameraPic.swap(src1);
	calibBCameraPic.swap(src2);
	chessBoardSize = _chessBoardSize;
	chessBoardRectSize = _chessBoardRectSize;
}

void CalibThread::run()
{
	QString message;
	vector<Point2f> ROI1_points_buf;
	vector<Point2f> ROI2_points_buf;
	vector<vector<Point2f>> ROIImage1_points_seq;
	vector<vector<Point2f>> ROIImage2_points_seq;
	vector<vector<Point3f>> Image_Object_points;
	//初始化棋盘格世界坐标
	for (int i = 0; i < calibACameraPic.size(); i++)
	{
		vector<Point3f> tempImages;
		for (int k = 0; k < chessBoardSize.height; k++)
		{
			for (int j = 0; j < chessBoardSize.width; j++)
			{
				Point3f tempPoints;
				tempPoints.x = j * chessBoardRectSize.width;
				tempPoints.y = k * chessBoardRectSize.height;
				tempPoints.z = 0;
				tempImages.push_back(tempPoints);
			}
		}
		Image_Object_points.push_back(tempImages);

	}
	if (calibACameraPic.size() == calibBCameraPic.size())
	{
		for (int i = 0; i < calibACameraPic.size(); i++)
		{
			if (findChessboardCornersSB(calibACameraPic[i], chessBoardSize, ROI1_points_buf, CALIB_CB_EXHAUSTIVE + CALIB_CB_ACCURACY))	
			{
				Mat TE;
				calibACameraPic[i].copyTo(TE);
				drawChessboardCorners(TE, chessBoardSize, ROI1_points_buf, true);
				resize(TE, TE, TE.size(), 0.5, 0.5, INTER_CUBIC);
				//outputVideo.write(TE);
				//存储
				//显示结果
				imshow("chessA", TE);
				waitKey(50);
			}
			else
			{
				Mat TE;
				calibACameraPic[i].copyTo(TE);
				drawChessboardCorners(TE, chessBoardSize, ROI1_points_buf, true);
				resize(TE, TE, TE.size(), 0.5, 0.5, INTER_CUBIC);
				//outputVideo.write(TE);
				//存储
				//显示结果
				imshow("棋盘格定位A", TE);
				waitKey(50);
				message = "相机A，第" + QString::number(i + 1) + "幅图片无法检测棋盘格点，跳过。";
				emit(calibMessage(message));
				continue;
			}
			if (findChessboardCornersSB(calibBCameraPic[i], chessBoardSize, ROI2_points_buf, CALIB_CB_EXHAUSTIVE + CALIB_CB_ACCURACY))
			{
				Mat TE;
				calibBCameraPic[i].copyTo(TE);
				drawChessboardCorners(TE, chessBoardSize, ROI2_points_buf, true);
				resize(TE, TE, TE.size(), 0.5, 0.5, INTER_CUBIC);
				//outputVideo.write(TE);
				//存储

				//显示结果
				waitKey(50);
				imshow("chessB", TE);
			}
			else
			{
				//cornerSubPix(image2good[i], ROI2_points_buf, Size(5, 5), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
				Mat TE;
				calibBCameraPic[i].copyTo(TE);
				drawChessboardCorners(TE, chessBoardSize, ROI2_points_buf, true);
				resize(TE, TE, TE.size(), 0.5, 0.5, INTER_CUBIC);
				//outputVideo.write(TE);
				//存储

				//显示结果
				imshow("chessB", TE);
				waitKey(50);
				message = "相机B，第" + QString::number(i + 1) + "幅图片无法检测棋盘格点，跳过。";
				emit(calibMessage(message));
				continue;
			}
			ROIImage1_points_seq.push_back(ROI1_points_buf);
			ROIImage2_points_seq.push_back(ROI2_points_buf);
			double a = calibBCameraPic.size();
			double percent = ((i+1)/a)*80;
			message = "处理进度：" + QString::number(percent, 10, 2) + "%";
			emit(calibMessage(message));
		}


	}
	destroyAllWindows();
	//单目标定
	calibrateCamera(Image_Object_points, ROIImage1_points_seq, calibACameraPic[0].size(), camera1Matrix, camera1DistCoeffs, camera1RvecsMat, camera1TvecsMat, 0);
	message = "处理进度：" + QString::number(85) + "%";
	emit(calibMessage(message));
	
	calibrateCamera(Image_Object_points, ROIImage2_points_seq, calibBCameraPic[0].size(), camera2Matrix, camera2DistCoeffs, camera2RvecsMat, camera2TvecsMat, 0);
	message = "处理进度：" + QString::number(90) + "%";
	emit(calibMessage(message));
	
	//双目标定
	stereoCalibrate(Image_Object_points, ROIImage1_points_seq, ROIImage2_points_seq, camera1Matrix, camera1DistCoeffs, camera2Matrix, camera2DistCoeffs, calibBCameraPic[0].size(), R, T, E, F, CALIB_FIX_INTRINSIC + CALIB_USE_INTRINSIC_GUESS );
	
	message = "处理进度：" + QString::number(100) + "%";
	emit(calibMessage(message));
	//计算重投影误差
	double total_err1 = 0.0; /* 所有图像的平均误差的总和 */
	double total_err2 = 0.0; /* 所有图像的平均误差的总和 */
	double err = 0.0; /* 每幅图像的平均误差 */
	for (int i = 0; i < calibACameraPic.size(); i++)
	{
		vector<Point2f> image_points1;
		projectPoints(Image_Object_points[i], camera1RvecsMat[i], camera1TvecsMat[i], camera1Matrix, camera1DistCoeffs, image_points1);
		vector<Point2f> tempImagePoint = ROIImage1_points_seq[i];
		Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
		Mat image_points2Mat = Mat(1, image_points1.size(), CV_32FC2);
		for (int j = 0; j < tempImagePoint.size(); j++)
		{
			image_points2Mat.at<Vec2f>(0, j) = Vec2f(image_points1[j].x, image_points1[j].y);
			tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
		}
		err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
		total_err1 += err /= chessBoardSize.width*chessBoardSize.height;
	}
	for (int i = 0; i < calibBCameraPic.size(); i++)
	{
		vector<Point2f> image_points2;
		vector<Point2f> tempImagePoint = ROIImage2_points_seq[i];
		projectPoints(Image_Object_points[0], camera2RvecsMat[i], camera2TvecsMat[i], camera2Matrix, camera2DistCoeffs, image_points2);
		Mat tempImagePointMat = Mat(1, tempImagePoint.size(), CV_32FC2);
		Mat image_points2Mat = Mat(1, image_points2.size(), CV_32FC2);
		for (int j = 0; j < tempImagePoint.size(); j++)
		{
			image_points2Mat.at<Vec2f>(0, j) = Vec2f(image_points2[j].x, image_points2[j].y);
			tempImagePointMat.at<Vec2f>(0, j) = Vec2f(tempImagePoint[j].x, tempImagePoint[j].y);
		}
		err = norm(image_points2Mat, tempImagePointMat, NORM_L2);
		total_err2 += err /= chessBoardSize.width*chessBoardSize.height;
	}

	
	
	//准备标定信息
	ofstream fout1;
	string resultFliePath="Documents\\Cailbresult.txt";
	fout1.open(resultFliePath);
	fout1 << "基于" << ROIImage1_points_seq.size() << "副图片的处理结果：" << endl;

	fout1 << "相机1内参数矩阵：" << endl;
	fout1 << camera1Matrix << endl << endl;
	fout1 << "相机1畸变系数：\n";
	fout1 << camera1DistCoeffs << endl;

	fout1 << endl;

	fout1 << "相机2内参数矩阵：" << endl;
	fout1 << camera2Matrix << endl << endl;
	fout1 << "相机2畸变系数：\n";
	fout1 << camera2DistCoeffs << endl;

	fout1 << endl;

	fout1 << "相机1to2的R:" << endl;
	fout1 << R << "\n";
	fout1 << "相机1to2的T:" << endl;
	fout1 << T << "\n";

	fout1 << "相机1的重投影误差：" << endl;
	fout1 << total_err1/calibACameraPic.size() << endl;
	fout1 << "相机2的重投影误差：" << endl;
	fout1 << total_err2 / calibBCameraPic.size() << endl;
	
	ifstream fin;
	fin.open(resultFliePath);
	stringstream buffer;
	buffer << fin.rdbuf();
	string calibData(buffer.str());

	

	//向主窗口发送信息
	emit(calibMessage(QString::fromStdString(calibData)));
}
