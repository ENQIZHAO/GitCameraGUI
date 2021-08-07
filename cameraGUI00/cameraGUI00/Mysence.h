#pragma once
#include <QGraphicsScene> 
#include <QMouseEvent>
#include <qpoint.h>
#include <QGraphicsSceneMouseEvent>

class MyScene : public QGraphicsScene
{

public:
	//记录数据
	QPointF fristPos, secondPos;
	//清除Roi
	bool cleanRoi=false;
	//拖拽状态
	bool draging=false;
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
	/*void leaveEvent(QEvent *);*/

};

