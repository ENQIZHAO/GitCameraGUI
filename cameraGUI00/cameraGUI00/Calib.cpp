#include "Calib.h"


CalibThread::CalibThread(vector<Mat>& src1, vector<Mat>& src2, Size _chessBoardSize, Size _chessBoardRectSize)
{
	//���ܱ궨����
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
	//��ʼ�����̸���������
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
				//�洢
				//��ʾ���
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
				//�洢
				//��ʾ���
				imshow("���̸�λA", TE);
				waitKey(50);
				message = "���A����" + QString::number(i + 1) + "��ͼƬ�޷�������̸�㣬������";
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
				//�洢

				//��ʾ���
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
				//�洢

				//��ʾ���
				imshow("chessB", TE);
				waitKey(50);
				message = "���B����" + QString::number(i + 1) + "��ͼƬ�޷�������̸�㣬������";
				emit(calibMessage(message));
				continue;
			}
			ROIImage1_points_seq.push_back(ROI1_points_buf);
			ROIImage2_points_seq.push_back(ROI2_points_buf);
			double a = calibBCameraPic.size();
			double percent = ((i+1)/a)*80;
			message = "������ȣ�" + QString::number(percent, 10, 2) + "%";
			emit(calibMessage(message));
		}


	}
	destroyAllWindows();
	//��Ŀ�궨
	calibrateCamera(Image_Object_points, ROIImage1_points_seq, calibACameraPic[0].size(), camera1Matrix, camera1DistCoeffs, camera1RvecsMat, camera1TvecsMat, 0);
	message = "������ȣ�" + QString::number(85) + "%";
	emit(calibMessage(message));
	
	calibrateCamera(Image_Object_points, ROIImage2_points_seq, calibBCameraPic[0].size(), camera2Matrix, camera2DistCoeffs, camera2RvecsMat, camera2TvecsMat, 0);
	message = "������ȣ�" + QString::number(90) + "%";
	emit(calibMessage(message));
	
	//˫Ŀ�궨
	stereoCalibrate(Image_Object_points, ROIImage1_points_seq, ROIImage2_points_seq, camera1Matrix, camera1DistCoeffs, camera2Matrix, camera2DistCoeffs, calibBCameraPic[0].size(), R, T, E, F, CALIB_FIX_INTRINSIC + CALIB_USE_INTRINSIC_GUESS );
	
	message = "������ȣ�" + QString::number(100) + "%";
	emit(calibMessage(message));
	//������ͶӰ���
	double total_err1 = 0.0; /* ����ͼ���ƽ�������ܺ� */
	double total_err2 = 0.0; /* ����ͼ���ƽ�������ܺ� */
	double err = 0.0; /* ÿ��ͼ���ƽ����� */
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

	
	
	//׼���궨��Ϣ
	ofstream fout1;
	string resultFliePath="Documents\\Cailbresult.txt";
	fout1.open(resultFliePath);
	fout1 << "����" << ROIImage1_points_seq.size() << "��ͼƬ�Ĵ�������" << endl;

	fout1 << "���1�ڲ�������" << endl;
	fout1 << camera1Matrix << endl << endl;
	fout1 << "���1����ϵ����\n";
	fout1 << camera1DistCoeffs << endl;

	fout1 << endl;

	fout1 << "���2�ڲ�������" << endl;
	fout1 << camera2Matrix << endl << endl;
	fout1 << "���2����ϵ����\n";
	fout1 << camera2DistCoeffs << endl;

	fout1 << endl;

	fout1 << "���1to2��R:" << endl;
	fout1 << R << "\n";
	fout1 << "���1to2��T:" << endl;
	fout1 << T << "\n";

	fout1 << "���1����ͶӰ��" << endl;
	fout1 << total_err1/calibACameraPic.size() << endl;
	fout1 << "���2����ͶӰ��" << endl;
	fout1 << total_err2 / calibBCameraPic.size() << endl;
	
	ifstream fin;
	fin.open(resultFliePath);
	stringstream buffer;
	buffer << fin.rdbuf();
	string calibData(buffer.str());

	

	//�������ڷ�����Ϣ
	emit(calibMessage(QString::fromStdString(calibData)));
}
