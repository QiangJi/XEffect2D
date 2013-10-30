#ifndef _JIA_XFONTBASIC_
#define _JIA_XFONTBASIC_

#include "XSprite.h"

//���������Ļ����࣬������������������̳ж���

enum _XFontAlignmentModeX
{
	_FONT_ALIGNMENT_MODE_X_LEFT,	//������
	_FONT_ALIGNMENT_MODE_X_MIDDLE,	//���ж���
	_FONT_ALIGNMENT_MODE_X_RIGHT,	//�Ҳ����
};
enum _XFontAlignmentModeY
{
	_FONT_ALIGNMENT_MODE_Y_UP,		//�϶���
	_FONT_ALIGNMENT_MODE_Y_MIDDLE,	//���ж���
	_FONT_ALIGNMENT_MODE_Y_DOWN,	//�¶���
};

#define STRING_MAX_SIZE 2048	//�ַ�������󳤶�

class _XFontBasic:public _XObjectBasic
{
//private:
protected:
	char m_isInited;		//�Ƿ��Ѿ���ʼ��
	int m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
	_XSprite m_sprite;		//��ͼ
	char *m_string;			//��Ҫ��ʾ���ַ���
	int m_lineSum;			//��ʾ�ַ���������
	int m_needShowTextSum;		//��Ҫ��ʾ���ַ�������

	char m_needUpdateData;	//�Ƿ���Ҫ�����ڲ�����

	_XFontAlignmentModeX m_alignmentModeX;	//�����ˮƽ���뷽ʽ
	_XFontAlignmentModeY m_alignmentModeY;	//����Ĵ�ֱ���뷽ʽ
public:
	void setAlignmentModeX(_XFontAlignmentModeX mode)
	{
		if(mode != m_alignmentModeX)
		{
			m_alignmentModeX = mode;
			m_needUpdateData = 1;
		}
	}
	void setAlignmentModeY(_XFontAlignmentModeY mode)
	{
		if(mode != m_alignmentModeY)
		{
			m_alignmentModeY = mode;
			m_needUpdateData = 1;
		}
	}
protected:
	float m_angle;			//����ĽǶ�
	float m_angleSin;		//����Ƕȵ�����	����������������Ϊ�˽�ʡ����ʱ��
	float m_angleCos;		//����Ƕȵ�����

	float m_distance;		//�ַ�֮��ľ���	
	float m_distanceY;		//��֮��ľ���

	_XVector2I m_position;		//�����λ��
	_XVector2I m_setPosition;	//���屻���õ�λ�ã����λ�ÿ���������ת�ȶ��������ձ��ı�
	_XVector2 m_rotateBasicPoint;	//������ת�����ĵ�

	_XVector2I m_size;		//���ֵĳߴ磨���ش�С��
	_XVector2 m_showSize;	//��ʾ����������Ŵ�С
public:

	void setDistance(float distance);			//����������֮��ľ���
	void setDistance(float x,float y);			//����������֮��ľ���
	void setAngle(float angle);	//����������ʾ�ĽǶ�
	float getAngle() const
	{
		return m_angle;
	}

	void setSize(const _XVector2& size);	//�����������ʾ��С
	void setSize(float x,float y);	//�����������ʾ��С
	void setPosition(const _XVector2& position);	//����������ʾ��λ��
	//void setPosition(int x,int y);			//����������ʾ��λ��
	void setPosition(float x,float y);			//����������ʾ��λ��
	void setRotateBasePoint(float x,float y);

	_XVector2I getTextSize() const
	{
		return m_size;
	}
//	_XVector2 getShowSize() const
//	{
//		return m_showSize;
//	}
	_XVector2 getSize() const
	{
		return m_showSize;
	}
//	_XVector2I getPosition() const
//	{
//		return m_position;
//	}
	_XVector2 getPosition() const
	{
		return _XVector2(m_position.x,m_position.y);
	}
protected:
	//���ü���ǰ���µ����ط�Χ
	int m_maxPixelWidth;	//�����������ؿ��
	int m_maxPixelHeight;	//�����������ظ߶�

	virtual void updateData() = 0;	//����һ�����麯������ʵ�ֲο���������
public:
	int getMaxPixelWidth()
	{
		if(m_needUpdateData != 0)
		{
			updateData();
			m_needUpdateData = 0;
		}
		return m_maxPixelWidth;
	}
	int getMaxPixelHeight()
	{
		if(m_needUpdateData != 0)
		{
			updateData();
			m_needUpdateData = 0;
		}
		return m_maxPixelHeight;
	}

public:
	//���ظ�ֵ������
	_XFontBasic& operator = (const _XFontBasic& temp);
	int setACopy(const _XFontBasic &temp);

protected:
	_XFontBasic()
		:m_needUpdateData(0)
		,m_alignmentModeX(_FONT_ALIGNMENT_MODE_X_LEFT)
		,m_alignmentModeY(_FONT_ALIGNMENT_MODE_Y_UP)
		,m_distance(0.0f)
		,m_distanceY(0.0f)
		,m_angle(0.0f)
		,m_angleSin(0.0f)
		,m_angleCos(1.0f)
		,m_position(0.0f,0.0f)
		,m_setPosition(0.0f,0.0f)
		,m_rotateBasicPoint(0.0f,0.0f)
		,m_showSize(1.0f,1.0f)
		,m_isInited(0)
		,m_maxPixelWidth(0)
		,m_maxPixelHeight(0)
		,m_lineSum(0)
		,m_needShowTextSum(0)
	{
		m_string = new char[STRING_MAX_SIZE];
		m_string[0] = '\0';
	}
	//���ؿ������캯��
	_XFontBasic(const _XFontBasic& temp);
public:
	int setString(const char *p);	//������ʾ���ַ���

	virtual ~_XFontBasic()	//������麯����Ϊ�˷�ֹͨ������ָ��ɾ�����������ʱ��ɵ��ڴ�й¶
	{
		XDELETE_ARRAY(m_string);
	}
	
	int isInRect(float x,float y);	//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);	//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
};

inline int _XFontBasic::setString(const char *p)	//������ʾ���ַ���
{
	if(p == NULL) return 0;
	if(strlen(p) >= STRING_MAX_SIZE) return 0;
	strcpy(m_string,p);
	m_needUpdateData = 1;
	return 1;
}

inline void _XFontBasic::setPosition(const _XVector2& position)	//����������ʾ��λ��
{
	setPosition(position.x,position.y);
}

inline void _XFontBasic::setPosition(float x,float y)			//����������ʾ��λ��
{
	m_setPosition.set(x,y);
	_XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos 
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set((int)(tempPosition.x),(int)(tempPosition.y));

	m_needUpdateData = 1;
}
inline void _XFontBasic::setDistance(float distance)			//����������֮��ľ���
{
	m_distance = distance;
	m_needUpdateData = 1;
}

inline void _XFontBasic::setDistance(float x,float y)
{
	m_distance = x;
	m_distanceY = y;
	m_needUpdateData = 1;
}

inline void _XFontBasic::setAngle(float angle)	//����������ʾ�ĽǶ�
{
	m_angle = angle;
	m_sprite.setAngle(angle);
	m_angleSin = sin(angle * ANGLE_TO_RADIAN);
	m_angleCos = cos(angle * ANGLE_TO_RADIAN);
	_XVector2 tempPosition;
	tempPosition.x = m_setPosition.x - (m_rotateBasicPoint.x * m_angleCos
		- m_rotateBasicPoint.y * m_angleSin);
	tempPosition.y = m_setPosition.y - (m_rotateBasicPoint.x * m_angleSin 
		+ m_rotateBasicPoint.y * m_angleCos);
	m_position.set((int)(tempPosition.x),(int)(tempPosition.y));

	m_needUpdateData = 1;
}

inline void _XFontBasic::setSize(const _XVector2& size)	//�����������ʾ��С
{
	setSize(size.x,size.y);
}

inline void _XFontBasic::setSize(float x,float y)	//�����������ʾ��С
{
	m_showSize.set(x,y);
	m_sprite.setSize(x,y);
	m_needUpdateData = 1;
}

inline void _XFontBasic::setRotateBasePoint(float x,float y)
{
	m_rotateBasicPoint.set(x,y);
	setPosition(m_setPosition);
}


#endif