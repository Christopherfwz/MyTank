#pragma once
#include "cocos2d.h"

class MoveControler : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	static MoveControler* create(cocos2d::Vec2 pos);
	bool initMove(cocos2d::Vec2 pos);
	float getRad(cocos2d::Vec2 pos1,cocos2d::Vec2 pos2);
	bool TouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void TouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void TouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void startRocker(void);
	void stopRocker(void);


	// implement the "static create()" method manually
	CREATE_FUNC(MoveControler);

private:
	double angleToHor;
	bool inSitu;
	bool isCanMove;
	cocos2d::Vec2 moveCenterPos;
	double rockerBGR;
	cocos2d::EventListenerTouchOneByOne* listener;
	cocos2d::EventDispatcher* eventDispatcher;
};
