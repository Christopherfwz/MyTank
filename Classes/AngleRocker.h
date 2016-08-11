#pragma once

#include "cocos2d.h"

USING_NS_CC;

#define PI 3.1415926

//���ڱ�ʶҡ�˷���
typedef enum {
	rocker_stop = 0,
	rocker_turn
}rockerDirectonOfAngle;

// ��־ ҡ�˱���������
typedef enum
{
	rockerBackGroundOfAngle = 2,
	rockerCenterOfAngle,
};

class AngleRocker : public Layer
{
public:

	//����ҡ��
	static AngleRocker* create(Vec2 pos);

	// ʹ��ҡ��
	void startRocker(void);
	// ֹͣʹ��ҡ��
	void stopRocker(void);


	// ��ȡ ҡ�� ����
	int getDirection(void);
	// ��ȡ ���� ����
	bool getIsLeft(void);


	//�����¼�  
	virtual bool TouchBegan(Touch* touch, Event* event);
	virtual void TouchMoved(Touch* touch, Event* event);
	virtual void TouchEnded(Touch* touch, Event* event);

	CREATE_FUNC(AngleRocker);
	float angle;
private:

	bool initRocker(Vec2 pos);
	//��ȡ��ǰҡ�����û�������ĽǶ�
	float getRad(Vec2 pos1, Vec2 pos2);
	// ���ݽǶȣ����ص�����
	Vec2 getAnglePosition(float r, float angle);

	// ҡ�� �Ƿ�����ƶ����Ƿ񳬹�ҡ�˱�����
	bool isCanMove;
	//ҡ�� ���ĵ�����
	Vec2 rockerCenterPos;
	//ҡ�˱����İ뾶
	float rockerBGR;
	//ҡ�� �İ뾶
	float rockerCenterR;
	//�жϿ��Ƹ˷��������жϾ����ϡ��¡����ҡ����ϡ����ϡ����¡����� �˶�
	int rocketDirection;
	// �ж������Ƿ�������
	bool isLeft;

	EventListenerTouchOneByOne* listener;
	//EventListenerTouchAllAtOnce* listener;
	EventDispatcher* eventDispatcher;
};
