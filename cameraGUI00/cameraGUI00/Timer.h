#pragma once
#include <QObject>
#include <QRunnable>
#include <time.h>
class Timer :
	 public QObject, public QRunnable
{
	Q_OBJECT
public:
	Timer(int _type,int _limit);

	int limit = 0;
	int type = 0;
signals:
	void timerOut(int );
protected:
	void run();
};

