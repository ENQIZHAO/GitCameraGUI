#include "Mysence.h"

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	//鼠标左键按下
	if (mouseEvent->button() == Qt::LeftButton)
	{
		//开始拖选框
		draging = true;
		fristPos = mouseEvent->scenePos();
		secondPos = mouseEvent->scenePos();
		//记录鼠标按下的点
		//start_point = mouseEvent->pos(); //相对窗口坐标---->这里得到的坐标全是0
	}
	else if (mouseEvent->button() == Qt::RightButton)
	{
		cleanRoi = true;
	}
}

void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	if (mouseEvent->button() == Qt::LeftButton)
	{
		draging = false;
		secondPos = mouseEvent->scenePos();
		//记录鼠标按下的点
	}

}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	if (draging)
	{

		secondPos = mouseEvent->scenePos();
		//记录鼠标按下的点
	}
}

