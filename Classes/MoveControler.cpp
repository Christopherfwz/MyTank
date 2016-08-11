#include "MoveControler.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Scene* MoveControler::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MoveControler::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool MoveControler::init()
{
	if (!Layer::init())
	{
		return false;
	}

	return true;
}

MoveControler* MoveControler::create(Vec2 pos)
{
	MoveControler* layer = MoveControler::create();
	if (layer)
	{
		layer->initMove(pos);
		return layer;
	}
	CC_SAFE_DELETE(layer);
	return NULL;
}

bool MoveControler::initMove(Vec2 pos) 
{
	if (!Layer::init()) 
	{
		return false;
	}
	// ҡ�˱��� ͼƬ  
	Sprite* spRockerBG = Sprite::create("spi_joystickBG.png");
	spRockerBG->setPosition(pos);
	spRockerBG->setTag(1);
	this->addChild(spRockerBG, 0);

	// ҡ������ ͼƬ  
	Sprite* spRockerCenter = Sprite::create("spi_joystickCenter.png");
	spRockerCenter->setPosition(pos);
	spRockerCenter->setTag(2);
	this->addChild(spRockerCenter, 1);

	// ���� ҡ������ λ��  
	moveCenterPos = pos;
	// ��ȡ ҡ�˱��� �뾶  
	rockerBGR = spRockerBG->getContentSize().width*0.5;
	// ���� ҡ�� ��ʼ����  
	angleToHor = 0;


	// �¼���������  
	auto listener = EventListenerTouchOneByOne::create();
	// �̵��������  
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(MoveControler::TouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MoveControler::TouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MoveControler::TouchEnded, this);

	// ע���¼���������  
	auto eventDispatcher = Director::getInstance()->getEventDispatcher();


	return true;
}

void MoveControler::startRocker(void)
{
	Sprite *rocker = (Sprite*)this->getChildByTag(2);
	rocker->setVisible(true);

	Sprite *rockerBG = (Sprite*)this->getChildByTag(1);
	rockerBG->setVisible(true);

	eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}


//ֹͣҡ��(����ҡ�ˣ�ȡ��ҡ�˵Ĵ�������)
void MoveControler::stopRocker(void)
{
	Sprite *rocker = (Sprite *)this->getChildByTag(2);
	rocker->setVisible(false);

	Sprite * rockerBG = (Sprite *)this->getChildByTag(1);
	rockerBG->setVisible(false);

	eventDispatcher->removeEventListener(listener);
}

bool MoveControler::TouchBegan(Touch* touch, Event* event)
{
	Sprite* sp = (Sprite*)this->getChildByTag(2);

	//�õ�����������    
	Vec2 point = touch->getLocation();
	log("aa");
	//�ж��Ƿ�����sp������飺boundingBox()�����С֮�ڵ���������    
	if (sp->boundingBox().containsPoint(point))
	{
		// �����ƶ���  
		isCanMove = true;
	}

	return true;
}

void MoveControler::TouchMoved(Touch* touch, Event* event)
{
	// ��������ƶ���ֱ�ӷ���  
	if (!isCanMove)
	{
		return;
	}

	Sprite* sp = (Sprite*)getChildByTag(2);
	Vec2 point = touch->getLocation();

	//�õ�ҡ���봥�������γɵĽǶ�  
	float angle = getRad(moveCenterPos, point);

	//�ж�����Բ��Բ�ľ��Ƿ����ҡ�˱����İ뾶  
	if (sqrt(pow((moveCenterPos.x - point.x), 2) + pow((moveCenterPos.y - point.y), 2)) >= rockerBGR)
	{
		//��֤�ڲ�СԲ�˶��ĳ�������  
		sp->setPosition(Vec2(Vec2(rockerBGR*cos(angle), rockerBGR*cos(angle)).x+moveCenterPos.x, Vec2(rockerBGR*cos(angle), rockerBGR*cos(angle)).y+moveCenterPos.y));
	}
	else
	{
		//��û�г�������ҡ�˸����û��������ƶ�����  
		sp->setPosition(point);
	}

}

float MoveControler::getRad(Vec2 pos1, Vec2 pos2)
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
	return rad;
}

void MoveControler::TouchEnded(Touch* touch, Event* event)
{
	if (!isCanMove)
	{
		return;
	}

	// ��ȡ ҡ�˱��� �� ҡ������  
	Sprite* rocker = (Sprite*)getChildByTag(2);
	Sprite* rockerBG = (Sprite*)getChildByTag(1);

	// �� ҡ������ ֹ֮ͣǰ���ж�����Ȼ��ʼ ִ�й�λ  
	rocker->stopAllActions();
	rocker->runAction(MoveTo::create(0.08, rockerBG->getPosition()));

	// ���� ����Ϊ stay������ ���´δ�����ʼǰ �����ƶ�  
	angleToHor = inSitu;
	isCanMove = false;
}

