#ifndef _XSPRING_
#define _XSPRING_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.12.10
//--------------------------------
#include "XPhysicsBasic.h"

class _XSpring:public _XBasicPhysic2DObject
{
private:
	float m_normalLength;	//���ɵ���Ȼ��չ����
	float m_minLength;		//����ѹ������С���ȣ�������С���ȵ�ѹ���ͳ���һ������3��
	float m_maxLength;		//������չ����󳤶ȣ�����������Ƚ��᲻����չ���߶��ѣ�������������ö�����
	float m_springRatio;	//���ɵĵ���ϵ��
	float m_nowLength;		//���ɵĵ�ǰ����

	float m_nowForce;		//���ɵ�ǰ������
public:
	int init();					//��ʼ�����ɵ�����
	int move(int timeDelay);	//���ɵ��˶�

	_XSpring()
	{
	}
};

#endif