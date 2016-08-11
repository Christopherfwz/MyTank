#pragma once

#include "cocos2d.h"

USING_NS_CC;

#define PI 3.1415926

//���ڱ�ʶҡ�˷���
typedef enum{
    rocker_stay = 0,
	rocker_move
    /*rocker_right,
    rocker_up,
    rocker_left,
    rocker_down,
	rocker_leftUp,
	rocker_rightUp,
	rocker_leftDown,
	rocker_rightDown,*/
}rockerDirecton;

// ��־ ҡ�˱���������
typedef enum
{
	rockerBackGround = 2,
	rockerCenter,
};

class JoyRocker : public Layer
{
public:
	
	//����ҡ��
    static JoyRocker* create(Vec2 pos);

	// ʹ��ҡ��
	void startRocker(void);
	// ֹͣʹ��ҡ��
	void stopRocker(void);
	

	// ��ȡ ҡ�� ����
	int getDirection(void);
	// ��ȡ ���� ����
	bool getIsLeft(void);


	//�����¼�  
    virtual bool TouchesBegan(const std::vector<Touch*>&, Event* event);  
    virtual void TouchesMoved(const std::vector<Touch*>&, Event* event);
    virtual void TouchesEnded(const std::vector<Touch*>&, Event* event);

	CREATE_FUNC(JoyRocker);
	float angle;
	bool reachborder;
private:

	bool initRocker(Vec2 pos);
    //��ȡ��ǰҡ�����û�������ĽǶ�
    float getRad(Vec2 pos1,Vec2 pos2);
	// ���ݽǶȣ����ص�����
	Vec2 getAnglePosition(float r,float angle);
	
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

	EventListenerTouchAllAtOnce* listener; 
	//EventListenerTouchAllAtOnce* listener;
	EventDispatcher* eventDispatcher;
};
