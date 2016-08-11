#pragma once

#include "cocos2d.h"

USING_NS_CC;

#define PI 3.1415926

//用于标识摇杆方向
typedef enum {
	rocker_stop = 0,
	rocker_turn
}rockerDirectonOfAngle;

// 标志 摇杆背景与中心
typedef enum
{
	rockerBackGroundOfAngle = 2,
	rockerCenterOfAngle,
};

class AngleRocker : public Layer
{
public:

	//创建摇杆
	static AngleRocker* create(Vec2 pos);

	// 使用摇杆
	void startRocker(void);
	// 停止使用摇杆
	void stopRocker(void);


	// 获取 摇杆 方向
	int getDirection(void);
	// 获取 人物 朝向
	bool getIsLeft(void);


	//触屏事件  
	virtual bool TouchBegan(Touch* touch, Event* event);
	virtual void TouchMoved(Touch* touch, Event* event);
	virtual void TouchEnded(Touch* touch, Event* event);

	CREATE_FUNC(AngleRocker);
	float angle;
private:

	bool initRocker(Vec2 pos);
	//获取当前摇杆与用户触屏点的角度
	float getRad(Vec2 pos1, Vec2 pos2);
	// 根据角度，返回点坐标
	Vec2 getAnglePosition(float r, float angle);

	// 摇杆 是否可以移动（是否超过摇杆背景）
	bool isCanMove;
	//摇杆 中心的坐标
	Vec2 rockerCenterPos;
	//摇杆背景的半径
	float rockerBGR;
	//摇杆 的半径
	float rockerCenterR;
	//判断控制杆方向，用来判断精灵上、下、左、右、左上、右上、左下、右下 运动
	int rocketDirection;
	// 判断人物是否朝向左面
	bool isLeft;

	EventListenerTouchOneByOne* listener;
	//EventListenerTouchAllAtOnce* listener;
	EventDispatcher* eventDispatcher;
};
