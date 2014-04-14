#ifndef _JIA_XBEZIERSPLINE_
#define _JIA_XBEZIERSPLINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "XBasicClass.h"

//3�α���������
class _XBezierSpline
{
private:
	_XVector2 m_pS;		//3�α����������е����
	_XVector2 m_pSP;		//���ƫ�Ƶ�
	_XVector2 m_pEP;		//�յ�ƫ�Ƶ�
	_XVector2 m_pE;		//�յ�

	_XBool m_isEnd;				//�Ƿ��Ѿ��ƶ���ĩβ
	float m_speed;				//�����е���ƶ��ٶ�
	_XVector2 m_nowValue;		//��ǰ�ı�����ȡֵ
	float m_nowAngle;			//��ǰ�ĽǶ�
	float m_nowPosition;		//�����е�ĵ�ǰλ��
public:
	_XVector2 getBezierSplineValue(float temp);	//temp����(0,1)
	float getBezierSplineAngle(float temp);		//Ŀǰ�ķ����Ƚ�ԭʼ������Ľ�����ѧ��ʽ�Ƶ�	
	_XBool getIsEnd() const {return m_isEnd;}
	float getSpeed() const {return m_speed;}
	void setSpeed(float speed) {m_speed = speed;}
	_XVector2 getNowValue() const {return m_nowValue;}
	float getNowAngle() const {return m_nowAngle;}
	float getNowPosition() const {return m_nowPosition;}

	//���������߿����Զ��ƶ�ȡֵ
	void move(int timeDelay);	//�ƶ������еĵ�
	_XBezierSpline();
	void draw();

	void init(const _XVector2& pStart,const _XVector2& pSP,const _XVector2& pEP,const _XVector2& pEnd);
	void init(float pSx,float pSy,float pSPx,float pSPy,float pEPx,float pEPy,float pEx,float pEy);
	void reset();				//���������ƶ���
};
inline void _XBezierSpline::init(const _XVector2& pStart,const _XVector2& pSP,const _XVector2& pEP,const _XVector2& pEnd)
{
	m_pS = pStart;
	m_pSP = pSP;
	m_pEP = pEP;
	m_pE = pEnd;
}
inline void _XBezierSpline::init(float pSx,float pSy,float pSPx,float pSPy,float pEPx,float pEPy,float pEx,float pEy)
{
	m_pS.set(pSx,pSy);
	m_pSP.set(pSPx,pSPy);
	m_pEP.set(pEPx,pEPy);
	m_pE.set(pEx,pEy);
}
inline void _XBezierSpline::reset()				//���������ƶ���
{
	m_nowPosition = 0.0f;
	m_isEnd = XFalse;
	m_nowValue = m_pS;
}

//2�α���������
class _XBezierSpline2
{
private:
	_XVector2 m_pS;		//�����������е����
	_XVector2 m_pP;	//ƫ�Ƶ�
	_XVector2 m_pE;		//�յ�

	_XBool m_isEnd;				//�Ƿ��Ѿ��ƶ���ĩβ
	float m_speed;				//�����е���ƶ��ٶ�
	_XVector2 m_nowValue;		//��ǰ�ı�����ȡֵ
	float m_nowAngle;			//��ǰ�ĽǶ�
	float m_nowPosition;		//�����е�ĵ�ǰλ��
public:
	_XVector2 getBezierSplineValue(float temp);	//temp����(0,1)
	float getBezierSplineAngle(float temp);		//Ŀǰ�ķ����Ƚ�ԭʼ������Ľ�����ѧ��ʽ�Ƶ�	
	_XBool getIsEnd() const {return m_isEnd;}
	float getSpeed() const {return m_speed;}
	void setSpeed(float speed) {m_speed = speed;}
	_XVector2 getNowValue() const {return m_nowValue;}
	float getNowAngle() const {return m_nowAngle;}
	float getNowPosition() const {return m_nowPosition;}

	//���������߿����Զ��ƶ�ȡֵ
	void move(int timeDelay);	//�ƶ������еĵ�
	_XBezierSpline2();
	void draw();

	void init(const _XVector2& pStart,const _XVector2& pP,const _XVector2& pEnd);
	void init(float pSx,float pSy,float pPx,float pPy,float pEx,float pEy);
	void reset();				//���������ƶ���
};
inline void _XBezierSpline2::init(const _XVector2& pStart,const _XVector2& pP,const _XVector2& pEnd)
{
	m_pS = pStart;
	m_pP = pP;
	m_pE = pEnd;
}
inline void _XBezierSpline2::init(float pSx,float pSy,float pPx,float pPy,float pEx,float pEy)
{
	m_pS.set(pSx,pSy);
	m_pP.set(pPx,pPy);
	m_pE.set(pEx,pEy);
}
inline void _XBezierSpline2::reset()				//���������ƶ���
{
	m_nowPosition = 0.0f;
	m_isEnd = XFalse;
	m_nowValue = m_pS;
}

#endif