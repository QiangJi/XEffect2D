#ifndef _JIA_XLIQUIDBALL_
#define _JIA_XLIQUIDBALL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.12.10
//--------------------------------
#include "XEffeEngine.h"

extern float gravity; //�������ٶ�
//Ĭ���������ٶȵķ���Ϊ��ֱ����

//����һ��Һ�������ӵ���
class _XLiquidBall
{
private:
	char m_isEnable;	//�Ƿ���Ч
public:
	_XVector2 m_position;	//λ��
	float m_radius;			//�뾶,��λ��
	float m_mass;			//����,��λ��
	_XVector2 m_speed;		//�ٶ�,ֱ������ϵ���ٶ�,��λ��ÿ��
	float m_airResistance;	//��������(�ڵ��ٵ������Ӧ�ÿ��Ժ��Բ���)
	float m_adhereRate;		//ճ��ϵ��
	float m_adhereRadius;	//ճ���뾶(ԭ����������������)

	_XVector2 m_positionS;	//�ڲ�����ĵ�,��ʼ
	//_XVector2 m_positionE;	//�ڲ�����ĵ�,����
	//int m_upCrashOrder;		//һ�β���ͬʱ���������巢����ײ
	void setEnable(char isEnable)
	{
		if(isEnable == 0) m_isEnable = 0;
		else m_isEnable = 1;
	}
	int getIsEnable() const
	{
		return m_isEnable;
	}
	void init(const _XVector2& position,float radius,float mass,const _XVector2& speed,float airResistance,float adhereRate,float adhereRadius)
	{
		m_position = position;	//λ��
		m_radius = radius;			//�뾶
		m_mass = mass;			//����
		m_speed = speed;		//�ٶ�,ֱ������ϵ���ٶ�
		m_airResistance = airResistance;	//��������(�ڵ��ٵ������Ӧ�ÿ��Ժ��Բ���)
		m_adhereRate = adhereRate;		//ճ��ϵ��
		m_adhereRadius = adhereRadius;	//ճ���뾶(ԭ����������������)
	}

	_XLiquidBall()
		:m_isEnable(0)	//�Ƿ���Ч
		,m_position(0.0f,0.0f)	//λ��
		,m_radius(0.0f)			//�뾶
		,m_mass(0.0f)			//����
		,m_speed(0.0f,0.0f)		//�ٶ�,ֱ������ϵ���ٶ�
		,m_airResistance(0.0f)	//��������(�ڵ��ٵ������Ӧ�ÿ��Ժ��Բ���)
		,m_adhereRate(0.0f)		//ճ��ϵ��
		,m_adhereRadius(0.0f)	//ճ���뾶(ԭ����������������)
	{
	}
};

//��ײ����
struct _XCrashData
{
	double m_crashTime;	//��ײ������ʱ��
	float m_crashAngle;	//��ײ�����ķ�����
	char m_needFrash;	//�Ƿ���Ҫˢ������
	_XCrashData()
		:m_crashTime(0.0)
		,m_crashAngle(0.0f)
		,m_needFrash(0)
	{
	}
};

class _XLiquidBalls
{
private:
	char m_isInited;	//�Ƿ�����˳�ʼ��
	char m_isEnable;	//�Ƿ���Ч
	int crashInsideProc(double timeDelay,double upTime);	//�Ƿ����
	void crashTimerProc(double timeDelay,double upTime);
	int checkProc();//��������Ƿ�ϸ�
public:
	//�����ǹ����������
	_XLiquidBall *m_pLiquidBall;
	int m_liquidBallSum; //���ӵ�����Ŀ
	//�����ǹ�����߶�����
	_XLine *m_pLine;
	int m_lineSum;	//�߶ε�����

	int m_liquidBallDataSum;
	_XCrashData *m_pCrashData;
//	double *m_pLiquidBallData;
//	char *m_pLiquidBallFlag;	//�Ƿ���Ҫ���м���ı��
	double m_minTimer;	//��Ԫ��ײ��ʱ���Сֵ
	int m_crashOrder;
	int m_crashObj0;
	int m_crashObj1;
	int m_crashSameTimeSum;
	int m_deep;
	//_XRect m_area;	//�����˶�������
	char m_haveError;	//�Ƿ���ڴ���

	int init(int liquidBallSum,int lineSum);
	void move(int timeDelay);
	void crashProc(int timeDelay);
	int addOneBall(const _XVector2& position,float radius,float mass,_XVector2 speed,float airResistance,float adhereRate,float adhereRadius);	//����һ�����ӣ����������Ƿ�ɹ�
	int addOneLine(const _XVector2& startPosition,_XVector2 endPosition);
	_XLiquidBalls();
	void setEnable(char isEnable)
	{
		if(isEnable == 0) m_isEnable = 0;
		else m_isEnable = 1;
	}
	int getIsEnable() const
	{
		return m_isEnable;
	}
	int getBallIsEnable(int order) const
	{
		return m_pLiquidBall[order].getIsEnable();
	}
	_XVector2 getBallPosition(int order) const
	{
		return m_pLiquidBall[order].m_position;
	}
};

#endif