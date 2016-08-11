#include "AngleRocker.h"

AngleRocker* AngleRocker::create(Vec2 pos)
{
	AngleRocker* layer = AngleRocker::create();
	if (layer)
	{
		layer->initRocker(pos);
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

bool AngleRocker::initRocker(Vec2 pos)
{
	// ҡ�˱��� ͼƬ
	Sprite* spRockerBG = Sprite::create("spi_joystickBG.png");
	spRockerBG->setPosition(pos);
	spRockerBG->setTag(rockerBackGroundOfAngle);
	spRockerBG->setVisible(false);
	this->addChild(spRockerBG, 0);

	// ҡ������ ͼƬ
	Sprite* spRockerCenter = Sprite::create("spi_joystickCenter.png");
	spRockerCenter->setPosition(pos);
	spRockerCenter->setTag(rockerCenterOfAngle);
	spRockerCenter->setVisible(false);
	this->addChild(spRockerCenter, 1);

	// ���� ҡ������ λ��
	rockerCenterPos = pos;
	// ��ȡ ҡ�˱��� �뾶
	rockerBGR = spRockerBG->getContentSize().width*0.5;
	//��ʾҡ�˷��򲻱�
	rocketDirection = 0;


	// �¼���������
	listener = EventListenerTouchOneByOne::create();
	// �̵��������
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(AngleRocker::TouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(AngleRocker::TouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(AngleRocker::TouchEnded, this);

	// ע���¼���������
	eventDispatcher = Director::getInstance()->getEventDispatcher();

	return true;
}

void AngleRocker::startRocker(void)
{
	Sprite *rocker = (Sprite*)this->getChildByTag(rockerCenterOfAngle);
	rocker->setVisible(true);

	Sprite *rockerBG = (Sprite*)this->getChildByTag(rockerBackGroundOfAngle);
	rockerBG->setVisible(true);

	//eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	eventDispatcher->addEventListenerWithFixedPriority(listener, 1);
}


//ֹͣҡ��(����ҡ�ˣ�ȡ��ҡ�˵Ĵ�������)
void AngleRocker::stopRocker(void)
{
	Sprite *rocker = (Sprite *)this->getChildByTag(rockerCenterOfAngle);
	rocker->setVisible(false);

	Sprite * rockerBG = (Sprite *)this->getChildByTag(rockerBackGroundOfAngle);
	rockerBG->setVisible(false);

	eventDispatcher->removeEventListener(listener);
}

float AngleRocker::getRad(Vec2 pos1, Vec2 pos2)
{
	float px1 = pos1.x;
	float py1 = pos1.y;
	float px2 = pos2.x;
	float py2 = pos2.y;

	//�õ�����x�ľ���
	float x = px2 - px1;
	//�õ�����y�ľ���
	float y = py1 - py2;
	//���б�߳���
	float xie = sqrt(pow(x, 2) + pow(y, 2));
	//�õ�����Ƕȵ�����ֵ(ͨ�����Ǻ����еĵ���Ƕ�����ֵ=б��/б��)
	float cosAngle = x / xie;
	//ͨ�������Ҷ����ȡ���ڽǶȵĻ���
	float rad = acos(cosAngle);
	//ע�⣺��������λ��Y����<ҡ�˵�Y���꣬����Ҫȥ��ֵ-0~-180
	if (py2 < py1)
	{
		rad = -rad;
	}
	angle = rad;
	return rad;
}

// ���ݽǶȣ����ص�����
Vec2 AngleRocker::getAnglePosition(float r, float angle)
{
	return Vec2(r*cos(angle), r*sin(angle));
}


bool AngleRocker::TouchBegan(Touch* touch, Event* event)
{
	Sprite* sp = (Sprite*)this->getChildByTag(rockerCenterOfAngle);

	//�õ�����������  
	Vec2 point = touch->getLocation();

	//�ж��Ƿ�����sp������飺boundingBox()�����С֮�ڵ���������  
	if (sp->boundingBox().containsPoint(point))
	{
		// �����ƶ���
		isCanMove = true;
	}

	return true;
}

// ��ȡ�ƶ�����
int AngleRocker::getDirection(void)
{
	return rocketDirection;
}

// ��ȡ���泯��trueΪ���ң�false����
bool AngleRocker::getIsLeft(void)
{
	return isLeft;
}

void AngleRocker::TouchMoved(Touch* touch, Event* event)
{
	// ��������ƶ���ֱ�ӷ���
	if (!isCanMove)
	{
		return;
	}

	Sprite* sp = (Sprite*)getChildByTag(rockerCenterOfAngle);
	Vec2 point = touch->getLocation();

	//�õ�ҡ���봥�������γɵĽǶ�
	float angle = getRad(rockerCenterPos, point);

	//�ж�����Բ��Բ�ľ��Ƿ����ҡ�˱����İ뾶
	if (sqrt(pow((rockerCenterPos.x - point.x), 2) + pow((rockerCenterPos.y - point.y), 2)) >= rockerBGR)
	{
		//��֤�ڲ�СԲ�˶��ĳ�������
		sp->setPosition(ccpAdd(getAnglePosition(rockerBGR, angle), Vec2(rockerCenterPos.x, rockerCenterPos.y)));
	}
	else
	{
		//��û�г�������ҡ�˸����û��������ƶ�����
		sp->setPosition(point);
	}

	rocketDirection = rocker_turn;
	//�жϷ���
	if (abs(angle) > PI / 2) {
		isLeft = true;
	}
	else {
		isLeft = false;
	}
	//// �ҷ�
	//if( angle>=-PI/8 && angle<PI/8 )	{
	//	rocketDirection = rocker_right;
	//	isLeft = false;
	//}
	//// ���Ϸ�
	//else if( angle>=PI/8 && angle<3*PI/8 )	{
	//	rocketDirection = rocker_rightUp;
	//	isLeft = false;
	//}
	//// �Ϸ�
	//else if( angle>=3*PI/8 && angle<5*PI/8 )	{
	//	rocketDirection = rocker_up;
	//}
	//// ���Ϸ�
	//else if( angle>=5*PI/8 && angle<7*PI/8 )	{
	//	rocketDirection = rocker_leftUp;
	//	isLeft = true;
	//}
	//// ��
	//else if( (angle>=7*PI/8&&angle<=PI) || (angle>=-PI&&angle<-7*PI/8) )	{
	//	rocketDirection = rocker_left;
	//	isLeft = true;
	//}
	//// ���·�
	//else if( angle>=-7*PI/8 && angle<-5*PI/8 )	{
	//	rocketDirection = rocker_leftDown;
	//	isLeft = true;
	//}
	//// �·�
	//else if( angle>=-5*PI/8 && angle<-3*PI/8 )	{
	//	rocketDirection = rocker_down;
	//}
	//// ���·�
	//else if( angle>=-3*PI/8 && angle<-PI/8 )	{
	//	rocketDirection = rocker_rightDown;
	//	isLeft = false;
	//}

}

void AngleRocker::TouchEnded(Touch* touch, Event* event)
{
	if (!isCanMove)
	{
		return;
	}

	// ��ȡ ҡ�˱��� �� ҡ������
	Sprite* rocker = (Sprite*)getChildByTag(rockerCenterOfAngle);
	Sprite* rockerBG = (Sprite*)getChildByTag(rockerBackGroundOfAngle);

	// �� ҡ������ ֹ֮ͣǰ���ж�����Ȼ��ʼ ִ�й�λ
	rocker->stopAllActions();
	rocker->runAction(MoveTo::create(0.08, rockerBG->getPosition()));

	// ���� ����Ϊ stay������ ���´δ�����ʼǰ �����ƶ�
	rocketDirection = rocker_stop;
	isCanMove = false;
}