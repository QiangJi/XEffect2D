#include "XFontBasic.h"

int _XFontBasic::setACopy(const _XFontBasic &temp)
{
	if(this == &temp) return 0;				//��ֹ��������
	m_needUpdateData = temp.m_needUpdateData;
	m_alignmentModeX = temp.m_alignmentModeX;
	m_alignmentModeY = temp.m_alignmentModeY;

	m_maxPixelWidth = temp.m_maxPixelWidth;
	m_maxPixelHeight = temp.m_maxPixelHeight;

	m_sprite.setACopy(temp.m_sprite);		//��ͼ
	strcpy(m_string,temp.m_string);			//��Ҫ��ʵ���ַ���
	m_position = temp.m_position;			//�����λ��
	m_setPosition = temp.m_setPosition;		//�����λ��
	m_rotateBasicPoint = temp.m_rotateBasicPoint;	//�����λ��
	m_angle = temp.m_angle;					//����ĽǶ�
	m_angleSin = temp.m_angleSin;			//����ĽǶ�
	m_angleCos = temp.m_angleCos;			//����ĽǶ�
	m_distance = temp.m_distance;			//����֮��ľ���	
	m_distanceY = temp.m_distanceY;			//����֮��ľ���	
	m_size = temp.m_size;					//���ֵĳߴ磨���ش�С��
	m_showSize = temp.m_showSize;
	m_isInited = temp.m_isInited;			//�Ƿ��Ѿ���ʼ��

	m_lineSum = temp.m_lineSum;
	m_needShowTextSum = temp.m_needShowTextSum;
	return 1;
}

_XFontBasic& _XFontBasic::operator = (const _XFontBasic& temp)
{
	if(this == &temp) return * this;		//��ֹ��������
	m_needUpdateData = temp.m_needUpdateData;
	m_alignmentModeX = temp.m_alignmentModeX;
	m_alignmentModeY = temp.m_alignmentModeY;

	m_maxPixelWidth = temp.m_maxPixelWidth;
	m_maxPixelHeight = temp.m_maxPixelHeight;

	m_sprite.setACopy(temp.m_sprite);		//��ͼ
	strcpy(m_string,temp.m_string);		//��Ҫ��ʵ���ַ���
	m_position = temp.m_position;	//�����λ��
	m_setPosition = temp.m_setPosition;	//�����λ��
	m_rotateBasicPoint = temp.m_rotateBasicPoint;	//�����λ��
	m_angle = temp.m_angle;			//����ĽǶ�
	m_angleSin = temp.m_angleSin;			//����ĽǶ�
	m_angleCos = temp.m_angleCos;			//����ĽǶ�
	m_distance = temp.m_distance;		//����֮��ľ���	
	m_distanceY = temp.m_distanceY;		//����֮��ľ���	
	m_size = temp.m_size;		//���ֵĳߴ磨���ش�С��
	m_showSize = temp.m_showSize;
	m_isInited = temp.m_isInited;			//�Ƿ��Ѿ���ʼ��

	m_lineSum = temp.m_lineSum;
	m_needShowTextSum = temp.m_needShowTextSum;
	return * this;
}

_XFontBasic::_XFontBasic(const _XFontBasic& temp)
{
	if(this == &temp) return;		//��ֹ��������

	m_string = new char[STRING_MAX_SIZE];
	m_string[0] = '\0';

	m_needUpdateData = temp.m_needUpdateData;
	m_alignmentModeX = temp.m_alignmentModeX;
	m_alignmentModeY = temp.m_alignmentModeY;

	m_maxPixelWidth = temp.m_maxPixelWidth;
	m_maxPixelHeight = temp.m_maxPixelHeight;

	m_sprite.setACopy(temp.m_sprite);		//��ͼ
	strcpy(m_string,temp.m_string);		//��Ҫ��ʵ���ַ���
	m_position = temp.m_position;	//�����λ��
	m_setPosition = temp.m_setPosition;	//�����λ��
	m_rotateBasicPoint = temp.m_rotateBasicPoint;	//�����λ��
	m_angle = temp.m_angle;			//����ĽǶ�
	m_angleSin = temp.m_angleSin;			//����ĽǶ�
	m_angleCos = temp.m_angleCos;			//����ĽǶ�
	m_distance = temp.m_distance;		//����֮��ľ���	
	m_distanceY = temp.m_distanceY;		//����֮��ľ���	
	m_size = temp.m_size;		//���ֵĳߴ磨���ش�С��
	m_showSize = temp.m_showSize;
	m_isInited = temp.m_isInited;			//�Ƿ��Ѿ���ʼ��

	m_lineSum = temp.m_lineSum;
	m_needShowTextSum = temp.m_needShowTextSum;
}

int _XFontBasic::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
//������������Ѿ����ص�bug�������λ�û��߽Ƕȵ������Ϣ�ı䵫����û�е���updateData(),��ʱ�����������õ�ֵ��ʱ�����ǲ���ȷ��
_XVector2 _XFontBasic::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_needUpdateData != 0)
	{
		updateData();
		m_needUpdateData = 0;
	}
	if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_LEFT)
	{
		if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
		{
			if(order == 0) ret = m_setPosition; else
			if(order == 1) ret.set(m_setPosition.x + m_maxPixelWidth * m_angleCos,
				m_setPosition.y + m_maxPixelWidth * m_angleSin); else
			if(order == 2) ret.set(m_setPosition.x + m_maxPixelWidth * m_angleCos - m_maxPixelHeight * m_angleSin,
				m_setPosition.y + m_maxPixelWidth * m_angleSin + m_maxPixelHeight * m_angleCos); else
			if(order == 3) ret.set(m_setPosition.x - m_maxPixelHeight * m_angleSin,
				m_setPosition.y + m_maxPixelHeight * m_angleCos);
		}else
		if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
		{
			if(order == 0) ret.set(m_setPosition.x + m_maxPixelHeight * 0.5f * m_angleSin,
				m_setPosition.y - m_maxPixelHeight * 0.5f * m_angleCos); else
			if(order == 1) ret.set(m_setPosition.x + m_maxPixelWidth * m_angleCos + m_maxPixelHeight * 0.5f * m_angleSin,
				m_setPosition.y + m_maxPixelWidth * m_angleSin - m_maxPixelHeight * 0.5f * m_angleCos); else
			if(order == 2) ret.set(m_setPosition.x + m_maxPixelWidth * m_angleCos - m_maxPixelHeight * 0.5f * m_angleSin,
				m_setPosition.y + m_maxPixelWidth * m_angleSin + m_maxPixelHeight * 0.5f * m_angleCos); else
			if(order == 3) ret.set(m_setPosition.x - m_maxPixelHeight * 0.5f * m_angleSin,
				m_setPosition.y + m_maxPixelHeight * 0.5 * m_angleCos);
		}else
		if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
		{
			if(order == 0) ret.set(m_setPosition.x + m_maxPixelHeight * m_angleSin,
				m_setPosition.y - m_maxPixelHeight * m_angleCos); else
			if(order == 1) ret.set(m_setPosition.x + m_maxPixelWidth * m_angleCos + m_maxPixelHeight * m_angleSin,
				m_setPosition.y + m_maxPixelWidth * m_angleSin - m_maxPixelHeight * m_angleCos); else
			if(order == 2) ret.set(m_setPosition.x + m_maxPixelWidth * m_angleCos,
				m_setPosition.y + m_maxPixelWidth * m_angleSin); else
			if(order == 3) ret.set(m_setPosition.x,
				m_setPosition.y);
		}
	}else
	if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_MIDDLE)
	{
		if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
		{
			if(order == 0) ret.set(m_setPosition.x - m_maxPixelWidth * 0.5f * m_angleCos,
				m_setPosition.y - m_maxPixelWidth * 0.5f * m_angleSin); else
			if(order == 1) ret.set(m_setPosition.x + m_maxPixelWidth * 0.5f * m_angleCos,
				m_setPosition.y + m_maxPixelWidth * 0.5f * m_angleSin); else
			if(order == 2) ret.set(m_setPosition.x + m_maxPixelWidth * 0.5f * m_angleCos - m_maxPixelHeight * m_angleSin,
				m_setPosition.y + m_maxPixelWidth * 0.5f * m_angleSin + m_maxPixelHeight * m_angleCos); else
			if(order == 3) ret.set(m_setPosition.x - m_maxPixelWidth * 0.5f * m_angleCos - m_maxPixelHeight * m_angleSin,
				m_setPosition.y - m_maxPixelWidth * 0.5f * m_angleSin + m_maxPixelHeight * m_angleCos);
		}else
		if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
		{
			if(order == 0) ret.set(m_setPosition.x - m_maxPixelWidth * 0.5f * m_angleCos + m_maxPixelHeight * 0.5f * m_angleSin,
				m_setPosition.y - m_maxPixelWidth * 0.5f * m_angleSin - m_maxPixelHeight * 0.5f * m_angleCos); else
			if(order == 1) ret.set(m_setPosition.x + m_maxPixelWidth * 0.5f * m_angleCos + m_maxPixelHeight * 0.5f * m_angleSin,
				m_setPosition.y + m_maxPixelWidth * 0.5f * m_angleSin - m_maxPixelHeight * 0.5f * m_angleCos); else
			if(order == 2) ret.set(m_setPosition.x + m_maxPixelWidth * 0.5f * m_angleCos - m_maxPixelHeight * 0.5f * m_angleSin,
				m_setPosition.y + m_maxPixelWidth * 0.5f * m_angleSin + m_maxPixelHeight * 0.5f * m_angleCos); else
			if(order == 3) ret.set(m_setPosition.x - m_maxPixelWidth * 0.5f * m_angleCos - m_maxPixelHeight * 0.5f * m_angleSin,
				m_setPosition.y - m_maxPixelWidth * 0.5f * m_angleSin + m_maxPixelHeight * 0.5f * m_angleCos);
		}else
		if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
		{
			if(order == 0) ret.set(m_setPosition.x - m_maxPixelWidth * 0.5f * m_angleCos + m_maxPixelHeight * m_angleSin,
				m_setPosition.y - m_maxPixelWidth * 0.5f * m_angleSin - m_maxPixelHeight * m_angleCos); else
			if(order == 1) ret.set(m_setPosition.x + m_maxPixelWidth * 0.5f * m_angleCos + m_maxPixelHeight * m_angleSin,
				m_setPosition.y + m_maxPixelWidth * 0.5f * m_angleSin - m_maxPixelHeight * m_angleCos); else
			if(order == 2) ret.set(m_setPosition.x + m_maxPixelWidth * 0.5f * m_angleCos,
				m_setPosition.y + m_maxPixelWidth * 0.5f * m_angleSin); else
			if(order == 3) ret.set(m_setPosition.x - m_maxPixelWidth * 0.5f * m_angleCos,
				m_setPosition.y - m_maxPixelWidth * 0.5f * m_angleSin);
		}
	}else
	if(m_alignmentModeX == _FONT_ALIGNMENT_MODE_X_RIGHT)
	{
		if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_UP)
		{
			if(order == 0) ret.set(m_setPosition.x - m_maxPixelWidth * m_angleCos,
				m_setPosition.y - m_maxPixelWidth * m_angleSin); else
			if(order == 1) ret.set(m_setPosition.x,m_setPosition.y); else
			if(order == 2) ret.set(m_setPosition.x - m_maxPixelHeight * m_angleSin,
				m_setPosition.y + m_maxPixelHeight * m_angleCos); else
			if(order == 3) ret.set(m_setPosition.x - m_maxPixelWidth * m_angleCos - m_maxPixelHeight * m_angleSin,
				m_setPosition.y - m_maxPixelWidth * m_angleSin + m_maxPixelHeight * m_angleCos);
		}else
		if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_MIDDLE)
		{
			if(order == 0) ret.set(m_setPosition.x - m_maxPixelWidth * m_angleCos + m_maxPixelHeight * 0.5f * m_angleSin,
				m_setPosition.y - m_maxPixelWidth * m_angleSin - m_maxPixelHeight * 0.5f * m_angleCos); else
			if(order == 1) ret.set(m_setPosition.x + m_maxPixelHeight * 0.5f * m_angleSin,
				m_setPosition.y - m_maxPixelHeight * 0.5f * m_angleCos); else
			if(order == 2) ret.set(m_setPosition.x - m_maxPixelHeight * 0.5f * m_angleSin,
				m_setPosition.y + m_maxPixelHeight * 0.5f * m_angleCos); else
			if(order == 3) ret.set(m_setPosition.x - m_maxPixelWidth * m_angleCos - m_maxPixelHeight * 0.5f * m_angleSin,
				m_setPosition.y - m_maxPixelWidth * m_angleSin + m_maxPixelHeight * 0.5f * m_angleCos);
		}else
		if(m_alignmentModeY == _FONT_ALIGNMENT_MODE_Y_DOWN)
		{
			if(order == 0) ret.set(m_setPosition.x - m_maxPixelWidth * m_angleCos + m_maxPixelHeight * m_angleSin,
				m_setPosition.y - m_maxPixelWidth * m_angleSin - m_maxPixelHeight * m_angleCos); else
			if(order == 1) ret.set(m_setPosition.x + m_maxPixelHeight * m_angleSin,
				m_setPosition.y - m_maxPixelHeight * m_angleCos); else
			if(order == 2) ret.set(m_setPosition.x,
				m_setPosition.y); else
			if(order == 3) ret.set(m_setPosition.x - m_maxPixelWidth * m_angleCos,
				m_setPosition.y - m_maxPixelWidth * m_angleSin);
		}
	}
	return ret;
}
