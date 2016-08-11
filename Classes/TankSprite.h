#pragma once
#include "cocos2d.h"
#include "Bullet.h"
#include "JoyRocker.h"

USING_NS_CC;

class TankSprite {
public:
	TankSprite();
	cocos2d::Sprite* tank_body;
	cocos2d::Sprite* tank_gun;
	void tankPositionSchedule(float a);
private:
	int life;
	int level;
	JoyRocker* rocker;
	
};