#include "Mysence.h"

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	//����������
	if (mouseEvent->button() == Qt::LeftButton)
	{
		//��ʼ��ѡ��
		draging = true;
		fristPos = mouseEvent->scenePos();
		secondPos = mouseEvent->scenePos();
		//��¼��갴�µĵ�
		//start_point = mouseEvent->pos(); //��Դ�������---->����õ�������ȫ��0
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
		//��¼��갴�µĵ�
	}

}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
	if (draging)
	{

		secondPos = mouseEvent->scenePos();
		//��¼��갴�µĵ�
	}
}

