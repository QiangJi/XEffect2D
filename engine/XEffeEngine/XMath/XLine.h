#ifndef _JIA_XLINE_
#define _JIA_XLINE_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
#include "XBasicFun.h"
#include "XVector2.h"
//�߶ε���
class _XLine
{
private:
	_XVector2 m_startPosition;	//�߶ε���ʼ��
	_XVector2 m_endPosition;	//�߶ε��յ�
	_XBool m_isEnable;			//�Ƿ���Ч
public:
	double A,B,C;	//Ax + By + C = 0;
	double m_data0;	//A^2 + B^2
	double m_data1;	//sqrt(A^2 + B^2)
	double m_angle;
public:
	void set(const _XVector2 &startPosition,const _XVector2 &endPosition);
	void set(float startX,float startY,float endX,float endY);
	float getLineLength() const;	//����߶εĳ���
	float getLineAngle() const;	//����߶εĽǶ�
	_XVector2 getStartPosition() const;	//����߶ε���ʼλ��
	_XVector2 getEndPosition() const;		//����߶ε��յ�λ��
	void setStartPosition(const _XVector2& startPosition);
	void setStartPosition(float x,float y);
	void setEndPosition(const _XVector2& endPosition);
	void setEndPosition(float x,float y);
	_XBool getIsEnable() const
	{
		return m_isEnable;
	}
	void setEnable(_XBool temp)
	{
		if(temp) m_isEnable = XTrue;
		else m_isEnable = XFalse;
	}
	_XLine()
	:m_isEnable(XFalse)
	,A(0.0)
	,B(0.0)
	,C(0.0)
	,m_data0(0.0)
	,m_data1(0.0)
	,m_angle(0.0)
	,m_startPosition(0.0f,0.0f)
	,m_endPosition(1.0f,1.0f)
	{}
};
#include "XLine.inl"
#endif