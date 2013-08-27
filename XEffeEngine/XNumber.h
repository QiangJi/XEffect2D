#ifndef _JIA_XNUMBER_
#define _JIA_XNUMBER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XSprite.h"

#ifndef TEXT_MODE
#define TEXT_MODE ("0123456789+-*/%.")
#endif

#ifndef MAX_NUMBER_LENGTH
#define MAX_NUMBER_LENGTH (256)
#endif

//���ֵĶ��뷽ʽ
enum _XNumberAlignmentMode
{
	_NUMBER_ALIGNMENT_MODE_LEFT,	//������
	_NUMBER_ALIGNMENT_MODE_MIDDLE,	//���ж���
	_NUMBER_ALIGNMENT_MODE_RIGHT,	//�Ҳ����
};
//24568
//+1258
//-1258
//-1258%
//-1.25%
//+-%.0123456789
class _XNumber:public _XObjectBasic
{
private:
	int m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�

	_XSprite m_sprite;					//��ͼ
	int m_isInited;
	char m_isVisiable;

	float m_angle;			//����ĽǶ�
	float m_angleSin;	//�Ƕȵ�sinֵ
	float m_angleCos;

	_XNumberAlignmentMode m_alignmentMode;
public:
	void setAlignmentMode(_XNumberAlignmentMode mode)
	{
		if(mode != m_alignmentMode)
		{
			m_alignmentMode = mode;
			m_needUpdateData = 1;
		}
	}
	void setVisiable() {m_isVisiable = 1;}					//��������ɼ�
	void disVisiable() {m_isVisiable = 0;}						//����������ɼ�
	char getVisiable() const {return m_isVisiable;}					//��ȡ����Ƿ�ɼ���״̬ 
private:
	char *m_string;

	//���ü���ǰ���µ����ط�Χ
	int m_maxPixelWidth;	//�����������ؿ��
	int m_maxPixelHeight;	//�����������ظ߶�
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
private:
	char m_needUpdateData;	//�Ƿ���Ҫ�����ڲ�����
	void updateData();		//������Ҫ�����ڲ�����
	_XVector2 *m_textPosition;	//���ڱ���ÿ���ַ���λ��
	_XRect *m_textRect;		//���ڱ���ÿ���ַ��ļ��к���
	int m_needShowTextSum;	//��Ҫ��ʾ���ַ�������

	_XVector2 m_position;	//�����λ�ã������������õ�λ��
	_XVector2 m_setPosition;	//���屻���õ�λ�ã����λ�ÿ���������ת�ȶ��������ձ��ı�
	float m_alpha;			//͸��ͨ��
	float m_distance;		//����֮��ľ���	
	_XVector2I m_size;		//���ֵĳߴ磨���ش�С��
	_XVector2I m_layout;		//����ͼƬ�Ĳ���
	_XVector2 m_showSize;
	_XVector2 m_rotateBasicPoint;	//������ת�����ĵ�
public:
	int init(const char *fileName,	//����ͼƬ������
		_XVector2I size,			//��������ش�С
		_XVector2I layout,		//����ͼƬ�Ĳ���
		int resoursePosition = 0);		
	void draw();

	void setSize(const _XVector2& size);		//�����������ʾ��С
	void setSize(float x,float y);	//�����������ʾ��С
	_XVector2 getSize() const
	{
		return m_showSize;
	}
	void setAngle(float angle);	//����������ʾ�ĽǶ�
	float getAngle() const
	{
		return m_angle;
	}
	void setPosition(const _XVector2& position);	//����������ʾ��λ��
	void setPosition(float x,float y);	//����������ʾ��λ��
	_XVector2 getPosition() const
	{
		return m_setPosition;
	}
	void setRotateBasePoint(float x,float y);

	int setNumber(int temp);								//����������
	int setNumber(float temp,int decimalPartLength);		//���ô�С�������
	int setNumber(const char *temp);		
	//����һ�κ�����ַ���
	_XNumber& operator = (const _XNumber& temp);
	int setACopy(const _XNumber &temp);

	void setColor(float r,float g,float b,float a);	//������ɫ
	void setColor(const _XFColor& color);
	void setAlpha(float a);
	float getAlpha() const
	{
		return m_alpha;
	}
	int release();

	void setDistance(float distance)
	{
		m_distance = distance;
	}
	float getDistance() const
	{
		return m_distance;
	}
	//Ϊ��ʵ����������������Ӽ����ӿ�
	int isInRect(float x,float y);	//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);	//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	_XNumber();
	~_XNumber();
};

#include "XNumber.inl"

#endif
