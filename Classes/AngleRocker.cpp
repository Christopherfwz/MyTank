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
	// 摇杆背景 图片
	Sprite* spRockerBG = Sprite::create("spi_joystickBG.png");
	spRockerBG->setPosition(pos);
	spRockerBG->setTag(rockerBackGroundOfAngle);
	spRockerBG->setVisible(false);
	this->addChild(spRockerBG, 0);

	// 摇杆中心 图片
	Sprite* spRockerCenter = Sprite::create("spi_joystickCenter.png");
	spRockerCenter->setPosition(pos);
	spRockerCenter->setTag(rockerCenterOfAngle);
	spRockerCenter->setVisible(false);
	this->addChild(spRockerCenter, 1);

	// 设置 摇杆中心 位置
	rockerCenterPos = pos;
	// 获取 摇杆背景 半径
	rockerBGR = spRockerBG->getContentSize().width*0.5;
	//表示摇杆方向不变
	rocketDirection = 0;


	// 事件监听部分
	listener = EventListenerTouchOneByOne::create();
	// 吞掉这个触摸
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(AngleRocker::TouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(AngleRocker::TouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(AngleRocker::TouchEnded, this);

	// 注册事件监听机制
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


//停止摇杆(隐藏摇杆，取消摇杆的触屏监听)
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
	angle = rad;
	return rad;
}

// 根据角度，返回点坐标
Vec2 AngleRocker::getAnglePosition(float r, float angle)
{
	return Vec2(r*cos(angle), r*sin(angle));
}


bool AngleRocker::TouchBegan(Touch* touch, Event* event)
{
	Sprite* sp = (Sprite*)this->getChildByTag(rockerCenterOfAngle);

	//得到触屏点坐标  
	Vec2 point = touch->getLocation();

	//判断是否点击到sp这个精灵：boundingBox()精灵大小之内的所有坐标  
	if (sp->boundingBox().containsPoint(point))
	{
		// 可以移动了
		isCanMove = true;
	}

	return true;
}

// 获取移动方向
int AngleRocker::getDirection(void)
{
	return rocketDirection;
}

// 获取正面朝向，true为向右，false向左
bool AngleRocker::getIsLeft(void)
{
	return isLeft;
}

void AngleRocker::TouchMoved(Touch* touch, Event* event)
{
	// 如果不能移动，直接返回
	if (!isCanMove)
	{
		return;
	}

	Sprite* sp = (Sprite*)getChildByTag(rockerCenterOfAngle);
	Vec2 point = touch->getLocation();

	//得到摇杆与触屏点所形成的角度
	float angle = getRad(rockerCenterPos, point);

	//判断两个圆的圆心距是否大于摇杆背景的半径
	if (sqrt(pow((rockerCenterPos.x - point.x), 2) + pow((rockerCenterPos.y - point.y), 2)) >= rockerBGR)
	{
		//保证内部小圆运动的长度限制
		sp->setPosition(ccpAdd(getAnglePosition(rockerBGR, angle), Vec2(rockerCenterPos.x, rockerCenterPos.y)));
	}
	else
	{
		//当没有超过，让摇杆跟随用户触屏点移动即可
		sp->setPosition(point);
	}

	rocketDirection = rocker_turn;
	//判断方向
	if (abs(angle) > PI / 2) {
		isLeft = true;
	}
	else {
		isLeft = false;
	}
	//// 右方
	//if( angle>=-PI/8 && angle<PI/8 )	{
	//	rocketDirection = rocker_right;
	//	isLeft = false;
	//}
	//// 右上方
	//else if( angle>=PI/8 && angle<3*PI/8 )	{
	//	rocketDirection = rocker_rightUp;
	//	isLeft = false;
	//}
	//// 上方
	//else if( angle>=3*PI/8 && angle<5*PI/8 )	{
	//	rocketDirection = rocker_up;
	//}
	//// 左上方
	//else if( angle>=5*PI/8 && angle<7*PI/8 )	{
	//	rocketDirection = rocker_leftUp;
	//	isLeft = true;
	//}
	//// 左方
	//else if( (angle>=7*PI/8&&angle<=PI) || (angle>=-PI&&angle<-7*PI/8) )	{
	//	rocketDirection = rocker_left;
	//	isLeft = true;
	//}
	//// 左下方
	//else if( angle>=-7*PI/8 && angle<-5*PI/8 )	{
	//	rocketDirection = rocker_leftDown;
	//	isLeft = true;
	//}
	//// 下方
	//else if( angle>=-5*PI/8 && angle<-3*PI/8 )	{
	//	rocketDirection = rocker_down;
	//}
	//// 右下方
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

	// 获取 摇杆背景 与 摇杆中心
	Sprite* rocker = (Sprite*)getChildByTag(rockerCenterOfAngle);
	Sprite* rockerBG = (Sprite*)getChildByTag(rockerBackGroundOfAngle);

	// 让 摇杆中心 停止之前所有动作，然后开始 执行归位
	rocker->stopAllActions();
	rocker->runAction(MoveTo::create(0.08, rockerBG->getPosition()));

	// 设置 方向为 stay，并且 在下次触摸开始前 不可移动
	rocketDirection = rocker_stop;
	isCanMove = false;
}