#include "Camera_GUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	//为Mat格式注册qt信号参数
	qRegisterMetaType<cv::Mat>("cv::Mat");

    QApplication a(argc, argv);
    Camera_GUI w;
    w.show();
    return a.exec();
	
	
}
