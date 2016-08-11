#include "Bullet.h"

USING_NS_CC;

Bullet::Bullet() {
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("images.plist");
	bullet = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("bullet.png"));
}