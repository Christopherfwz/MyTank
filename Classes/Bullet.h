#pragma once
#include "cocos2d.h"

class Bullet {
public:
	Bullet();
	cocos2d::Sprite* bullet;
private:
	float rotation;
	int speed;
	bool isEnemy;
};