#include "JoyRocker.h"

JoyRocker* JoyRocker::create(Vec2 pos)
{
	JoyRocker* layer = JoyRocker::create();
    if ( layer )
    {
        layer->initRocker(pos);
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return NULL;
}

bool JoyRocker::initRocker(Vec2 pos)
{
	angle = PI/2;
	reachborder = false;
	// ҡ�˱��� ͼƬ
	Sprite* spRockerBG = Sprite::create("spi_joystickBG.png");
	spRockerBG->setPosition(pos);
	spRockerBG->setTag(rockerBackGround);
	spRockerBG->setVisible(false);
    this->addChild(spRockerBG,0);
 
	// ҡ������ ͼƬ
	Sprite* spRockerCenter = Sprite::create("spi_joystickCenter.png");
	spRockerCenter->setPosition(pos);
	spRockerCenter->setTag(rockerCenter);
	spRockerCenter->setVisible(false);
    this->addChild(spRockerCenter,1);
 
	// ���� ҡ������ λ��
	rockerCenterPos = pos;
	// ��ȡ ҡ�˱��� �뾶
    rockerBGR = spRockerBG->getContentSize().width*0.5;
    //��ʾҡ�˷��򲻱�
	rocketDirection = 0;


	// �¼���������
	listener = EventListenerTouchAllAtOnce::create();
	//listener = EventListenerTouchAllAtOnce::create();
	// �̵��������
	//listener->setSwallowTouches(true);

	listener->onTouchesBegan = CC_CALLBACK_2(JoyRocker::TouchesBegan,this);
	listener->onTouchesMoved = CC_CALLBACK_2(JoyRocker::TouchesMoved,this);
	listener->onTouchesEnded = CC_CALLBACK_2(JoyRocker::TouchesEnded,this);

	// ע���¼���������
	eventDispatcher = Director::getInstance()->getEventDispatcher();
	return true;
}

void JoyRocker::startRocker( void )
{
	Sprite *rocker = (Sprite*)this->getChildByTag(rockerCenter);
    rocker->setVisible(true);
 
    Sprite *rockerBG = (Sprite*)this->getChildByTag(rockerBackGround);
    rockerBG->setVisible(true);

	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	//eventDispatcher ->addEventListenerWithFixedPriority(listener, 1); 
}

 
//ֹͣҡ��(����ҡ�ˣ�ȡ��ҡ�˵Ĵ�������)
void JoyRocker::stopRocker(void)
{
	Sprite *rocker = (Sprite *)this->getChildByTag(rockerCenter);
    rocker->setVisible(false);
 
	Sprite * rockerBG = (Sprite *)this->getChildByTag(rockerBackGround);
    rockerBG->setVisible(false);
 
	eventDispatcher->removeEventListener(listener);
}

float JoyRocker::getRad(Vec2 pos1,Vec2 pos2)
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
    float xie = sqrt(pow(x,2) + pow(y,2));
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
Vec2 JoyRocker::getAnglePosition(float r,float angle)
{
    return Vec2(r*cos(angle),r*sin(angle));
}


bool JoyRocker::TouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
	Sprite* sp = (Sprite*)this->getChildByTag(rockerCenter); 
	Vec2 point;
	float distance = -1;
	for (auto vec_touch : touches) {
		float distance2 = pow(abs(vec_touch->getLocation().x - rockerCenterPos.x), 2) + pow(abs(vec_touch->getLocation().y - rockerCenterPos.y), 2);
		if (distance < 0) {
			distance = distance2;
			point = vec_touch->getLocation();
		}
		else if (distance>distance2) {
			distance = distance2;
			point = vec_touch->getLocation();
		}
	}
    //�õ�����������  
      
    //�ж��Ƿ�����sp������飺boundingBox()�����С֮�ڵ���������  
    if(sp->getBoundingBox().containsPoint(point))  
	{  
		// �����ƶ���
		isCanMove = true; 
		reachborder = false;
	}

	return true;
}

// ��ȡ�ƶ�����
int JoyRocker::getDirection(void)
{
	return rocketDirection;
}

// ��ȡ���泯��trueΪ���ң�false����
bool JoyRocker::getIsLeft(void)
{
	return isLeft;
}

void JoyRocker::TouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
	// ��������ƶ���ֱ�ӷ���
	if(!isCanMove)  
    {  
        return;  
    }  
      
	Sprite* sp = (Sprite*)getChildByTag(rockerCenter);  
	Vec2 point;
	float distance = -1;
	for (auto vec_touch : touches) {
		float distance2 = pow(abs(vec_touch->getLocation().x - rockerCenterPos.x), 2) + pow(abs(vec_touch->getLocation().y - rockerCenterPos.y), 2);
		if (distance < 0) {
			distance = distance2;
			point = vec_touch->getLocation();
		}
		else if (distance>distance2) {
			distance = distance2;
			point = vec_touch->getLocation();
		}
	}
      
	//�õ�ҡ���봥�������γɵĽǶ�
	float angle = getRad(rockerCenterPos,point);

    //�ж�����Բ��Բ�ľ��Ƿ����ҡ�˱����İ뾶
	if (sqrt(pow((rockerCenterPos.x - point.x),2) + pow((rockerCenterPos.y - point.y),2)) >= rockerBGR)
    {
		reachborder = true;
        //��֤�ڲ�СԲ�˶��ĳ�������
		sp->setPosition(getAnglePosition(rockerBGR,angle)+Vec2(rockerCenterPos.x,rockerCenterPos.y));
    }
    else
    {
		reachborder = false;
        //��û�г�������ҡ�˸����û��������ƶ�����
        sp->setPosition(point);
    }
    
	rocketDirection = rocker_move;
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

void JoyRocker::TouchesEnded(const std::vector<Touch*>&, Event* event)
{
	if(!isCanMove)  
    {  
		return;  
    }  
      
	// ��ȡ ҡ�˱��� �� ҡ������
	Sprite* rocker = (Sprite*)getChildByTag(rockerCenter);  
	Sprite* rockerBG = (Sprite*)getChildByTag(rockerBackGround);  
      
	// �� ҡ������ ֹ֮ͣǰ���ж�����Ȼ��ʼ ִ�й�λ
    rocker->stopAllActions();  
    rocker->runAction(MoveTo::create(0.08, rockerBG->getPosition()));  
     
	// ���� ����Ϊ stay������ ���´δ�����ʼǰ �����ƶ�
	rocketDirection=rocker_stay;
    isCanMove = false;  
	reachborder = false;
}
