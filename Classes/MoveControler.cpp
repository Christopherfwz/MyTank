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
	// 摇杆背景 图片  
	Sprite* spRockerBG = Sprite::create("spi_joystickBG.png");
	spRockerBG->setPosition(pos);
	spRockerBG->setTag(1);
	this->addChild(spRockerBG, 0);

	// 摇杆中心 图片  
	Sprite* spRockerCenter = Sprite::create("spi_joystickCenter.png");
	spRockerCenter->setPosition(pos);
	spRockerCenter->setTag(2);
	this->addChild(spRockerCenter, 1);

	// 设置 摇杆中心 位置  
	moveCenterPos = pos;
	// 获取 摇杆背景 半径  
	rockerBGR = spRockerBG->getContentSize().width*0.5;
	// 设置 摇杆 初始方向  
	angleToHor = 0;


	// 事件监听部分  
	auto listener = EventListenerTouchOneByOne::create();
	// 吞掉这个触摸  
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(MoveControler::TouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(MoveControler::TouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(MoveControler::TouchEnded, this);

	// 注册事件监听机制  
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


//停止摇杆(隐藏摇杆，取消摇杆的触屏监听)
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

	//得到触屏点坐标    
	Vec2 point = touch->getLocation();
	log("aa");
	//判断是否点击到sp这个精灵：boundingBox()精灵大小之内的所有坐标    
	if (sp->boundingBox().containsPoint(point))
	{
		// 可以移动了  
		isCanMove = true;
	}

	return true;
}

void MoveControler::TouchMoved(Touch* touch, Event* event)
{
	// 如果不能移动，直接返回  
	if (!isCanMove)
	{
		return;
	}

	Sprite* sp = (Sprite*)getChildByTag(2);
	Vec2 point = touch->getLocation();

	//得到摇杆与触屏点所形成的角度  
	float angle = getRad(moveCenterPos, point);

	//判断两个圆的圆心距是否大于摇杆背景的半径  
	if (sqrt(pow((moveCenterPos.x - point.x), 2) + pow((moveCenterPos.y - point.y), 2)) >= rockerBGR)
	{
		//保证内部小圆运动的长度限制  
		sp->setPosition(Vec2(Vec2(rockerBGR*cos(angle), rockerBGR*cos(angle)).x+moveCenterPos.x, Vec2(rockerBGR*cos(angle), rockerBGR*cos(angle)).y+moveCenterPos.y));
	}
	else
	{
		//当没有超过，让摇杆跟随用户触屏点移动即可  
		sp->setPosition(point);
	}

}

float MoveControler::getRad(Vec2 pos1, Vec2 pos2)
{
	float px1 = pos1.x;
	float py1 = pos1.y;
	float px2 = pos2.x;
	float py2 = pos2.y;

	//得到两点x的距离  
	float x = px2 - px1;
	//得到两点y的距离  
	float y = py1 - py2;
	//算出斜边长度  
	float xie = sqrt(pow(x, 2) + pow(y, 2));
	//得到这个角度的余弦值(通过三角函数中的点里：角度余弦值=斜边/斜边)  
	float cosAngle = x / xie;
	//通过反余弦定理获取到期角度的弧度  
	float rad = acos(cosAngle);
	//注意：当触屏的位置Y坐标<摇杆的Y坐标，我们要去反值-0~-180  
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

	// 获取 摇杆背景 与 摇杆中心  
	Sprite* rocker = (Sprite*)getChildByTag(2);
	Sprite* rockerBG = (Sprite*)getChildByTag(1);

	// 让 摇杆中心 停止之前所有动作，然后开始 执行归位  
	rocker->stopAllActions();
	rocker->runAction(MoveTo::create(0.08, rockerBG->getPosition()));

	// 设置 方向为 stay，并且 在下次触摸开始前 不可移动  
	angleToHor = inSitu;
	isCanMove = false;
}

