//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XControlBasic.h"

_XControlBasic::_XControlBasic()
:m_mouseRect(0.0f,0.0f,1.0f,1.0f)		//�ؼ��������Ӧ��Χ
,m_size(1.0f,1.0f)		//�ؼ��Ĵ�С
,m_position(0.0f,0.0f)	//�ؼ���λ��
,m_color(1.0f,1.0f,1.0f,1.0f)		//�ؼ�����ɫ
,m_isEnable(0)		//�ؼ��Ƿ���Ч����Ч����������������������
,m_isVisiable(0)		//�ؼ��Ƿ�ɼ����ɼ�����������������������
,m_isActive(0)		//�ؼ��Ƿ��ڼ���״̬�������������ܽ��տ����ź�
,m_isBeChoose(0)
{
	static int controlOrder = 0;
	controlOrder ++;
	m_objectID = controlOrder;
}

int _XControlBasic::setACopy(const _XControlBasic & temp)
{
	m_mouseRect = temp.m_mouseRect;		//�ؼ��������Ӧ��Χ
	m_size = temp.m_size;		//�ؼ��Ĵ�С
	m_position = temp.m_position;	//�ؼ���λ��
	m_color = temp.m_color;		//�ؼ�����ɫ

	m_isEnable = temp.m_isEnable;		//�ؼ��Ƿ���Ч����Ч����������������������
	m_isVisiable = temp.m_isVisiable;		//�ؼ��Ƿ�ɼ����ɼ�����������������������
	m_isActive = temp.m_isActive;		//�ؼ��Ƿ��ڼ���״̬�������������ܽ��տ����ź�
	m_isBeChoose = temp.m_isBeChoose;
	return 1;
}