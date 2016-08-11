#include "WelcomeScene.h"
#include "DemoScene.h"

USING_NS_CC;

Scene* WelcomeScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = WelcomeScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

bool WelcomeScene::init() {

	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	Sprite* battlecity = Sprite::create("BattleCity.png");
	battlecity->setPosition(visibleSize.width / 2, visibleSize.height * 3 / 4);
	this->addChild(battlecity);

	MenuItemFont* startItem = MenuItemFont::create("start", CC_CALLBACK_1(WelcomeScene::menuGoToBattle, this));
	MenuItemFont* settingItem = MenuItemFont::create("setting", CC_CALLBACK_1(WelcomeScene::menuGoToSetting, this));
	MenuItemFont* quitItem = MenuItemFont::create("exit", CC_CALLBACK_1(WelcomeScene::menuToQuit, this));
	Menu* menus = Menu::create(startItem, settingItem, quitItem, NULL);
	menus->alignItemsVertically();
	menus->setPosition(visibleSize.width / 2, visibleSize.height / 3);
	this->addChild(menus);

	return true;
}

void WelcomeScene::menuGoToBattle(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, DemoScene::createScene()));
}

void WelcomeScene::menuGoToSetting(Ref* pSender) {
	Director::getInstance()->replaceScene(TransitionFade::create(1.5f, DemoScene::createScene()));
}

void WelcomeScene::menuToQuit(Ref* pSender) {
	Director::getInstance()->end();
}