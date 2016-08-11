#pragma once
#include "cocos2d.h"

class WelcomeScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	void menuGoToBattle(Ref* pSender);
	void menuGoToSetting(Ref* pSender);
	void menuToQuit(Ref* pSender);

	CREATE_FUNC(WelcomeScene);
};