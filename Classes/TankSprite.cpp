#include "TankSprite.h"

TankSprite::TankSprite() {
	tank_body = Sprite::create("tank_body.png");
	tank_gun = Sprite::create("tank_gun.png");
	tank_body->addChild(tank_gun);
	tank_gun->setPosition(tank_body->getContentSize().width / 2, tank_body->getContentSize().height / 2 - 5);
}

void TankSprite::tankPositionSchedule(float a) {

}