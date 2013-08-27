#ifndef _JIA_XBEZIERSPLINE_
#define _JIA_XBEZIERSPLINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XBasicClass.h"

class _XBezierSpline
{
private:
	_XVector2 m_p0;		//3�α����������еĵ�һ����
	_XVector2 m_p1;
	_XVector2 m_p2;
	_XVector2 m_p3;
public:
	_XVector2 getBezierSplineValue(float temp);	//temp����(0,1)

	//���������߿����Զ��ƶ�ȡֵ
	_XVector2 m_nowValue;			//��ǰ�ı�����ȡֵ
	float m_nowPosition;		//�����е�ĵ�ǰλ��
	float m_speed;				//�����е���ƶ��ٶ�
	int m_isEnd;				//�Ƿ��Ѿ��ƶ���ĩβ
	void move(int timeDelay);	//�ƶ������еĵ�
	_XBezierSpline();

	void init(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3);
	void init(float p0x,float p0y,float p1x,float p1y,float p2x,float p2y,float p3x,float p3y);
	void reset();				//���������ƶ���
};

inline void _XBezierSpline::init(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3)
{
	m_p0 = p0;
	m_p1 = p1;
	m_p2 = p2;
	m_p3 = p3;
}

inline void _XBezierSpline::init(float p0x,float p0y,float p1x,float p1y,float p2x,float p2y,float p3x,float p3y)
{
	m_p0.set(p0x,p0y);
	m_p1.set(p1x,p1y);
	m_p2.set(p2x,p2y);
	m_p3.set(p3x,p3y);
}

inline void _XBezierSpline::reset()				//���������ƶ���
{
	m_nowPosition = 0;
	m_isEnd = 0;
	m_nowValue = m_p0;
}

#endif