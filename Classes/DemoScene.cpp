
#include "DemoScene.h"

Scene* DemoScene::createScene()
{
	auto scene = Scene::create();
	auto layer = DemoScene::create();
	scene->addChild(layer);
	return scene;
}

bool DemoScene::init()
{
	if( !Layer::init() )	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	

	tileMap = TMXTiledMap::create("map/map2.tmx");
	
	tileMap->setPosition(Vec2::ZERO);
	this->addChild(tileMap);
	
	myTank = new TankSprite();
	myTank->tank_body->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	myTank->tank_body->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(myTank->tank_body);
	

	jr = JoyRocker::create(Vec2(100,100));
	this->addChild(jr);

	jr->startRocker();

	jr2=JoyRocker::create(Vec2(visibleSize.width-100, 100));
	this->addChild(jr2);

	jr2->startRocker();

	this->scheduleUpdate();
	return true;
}

void DemoScene::update(float ft)
{
	//车身全方向摇杆
	float radjr = jr->angle;
	if (jr->getDirection()) {
		myTank->tank_body->setPosition((Vec2(myTank->tank_body->getPosition().x + 2 * cos(radjr), myTank->tank_body->getPosition().y + 2 * sin(radjr))));
		myTank->tank_body->setRotation(90 - radjr * 180 / PI);
	}

	//炮筒角度摇杆
	float radjr2 = jr2->angle;
	if (jr2->getDirection()) {
		myTank->tank_gun->setRotation((radjr - radjr2) * 180 / PI);
	}

	if (jr2->reachborder) {
		static int i = 1;
		if (i % 5 == 0) {
			bullet = new Bullet();
			bullet->bullet->setRotation(90 - radjr2 * 180 / PI);
			bullet->bullet->setAnchorPoint(Vec2(0.5, 0.5));
			bullet->bullet->setPosition(myTank->tank_body->getPosition().x + 30 * cos(jr2->angle)-5*cos(jr->angle), myTank->tank_body->getPosition().y + 30 * sin(jr2->angle) - 5*sin(jr->angle));
			this->addChild(bullet->bullet);
			bullet->bullet->runAction(MoveBy::create(0.3, Vec2(200 * cos(jr2->angle), 200 * sin(jr2->angle))));
			i = 1;
		}
		else {
			i++;
		}
	}
    //八方向
	//switch( jr->getDirection() )
 //   {
 //   case 1:
	//		bg->setPosition(Vec2(bg->getPosition().x+2,bg->getPosition().y));		//向右走

 //       break;
 //   case 2:
	//	
	//		
	//		bg->setPosition(Vec2(bg->getPosition().x, bg->getPosition().y+2));		//向上走
	//	
	//	break;
 //   case 3:
	//	
	//		bg->setPosition(Vec2(bg->getPosition().x-2,bg->getPosition().y));		//向左走
	//	
	//	break;
 //   case 4:
	//	
	//		bg->setPosition(Vec2(bg->getPosition().x,bg->getPosition().y-2));		//向下走
	//	
	//	break;
	//case 5:
	//		bg->setPosition(Vec2(bg->getPosition().x-1,bg->getPosition().y+1));
	//	;break;
	//case 6:
	//		bg->setPosition(Vec2(bg->getPosition().x+1,bg->getPosition().y+1));
	//	;break;
	//case 7:
	//		bg->setPosition(Vec2(bg->getPosition().x-1,bg->getPosition().y-1));
	//	;break;
	//case 8:
	//		bg->setPosition(Vec2(bg->getPosition().x+1,bg->getPosition().y-1));
	//	;break;
 //   default:
 //       
 //       break;
 //   }
}