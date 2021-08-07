#pragma once
#include <QGraphicsScene> 
#include <QMouseEvent>
#include <qpoint.h>
#include <QGraphicsSceneMouseEvent>

class MyScene : public QGraphicsScene
{

public:
	//��¼����
	QPointF fristPos, secondPos;
	//���Roi
	bool cleanRoi=false;
	//��ק״̬
	bool draging=false;
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
	/*void leaveEvent(QEvent *);*/

};

