#include "Camera_GUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	//ΪMat��ʽע��qt�źŲ���
	qRegisterMetaType<cv::Mat>("cv::Mat");

    QApplication a(argc, argv);
    Camera_GUI w;
    w.show();
    return a.exec();
	
	
}
