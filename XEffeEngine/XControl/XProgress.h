#ifndef _JIA_XPROGRESS_
#define _JIA_XPROGRESS_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//����һ�����ڽ��������࣬������ʹ��������ͼ�Ľ⹹�����Ǳ����㣬�ƶ�������ֻ��߹ⷴ��㣬�����ƶ����Ǳ���Ҫ�еġ�

#include "XControlBasic.h"
#include "../XNumber.h"
#include "../XSprite.h"

class _XProgressTexture
{
private:
	char m_isInited;
public:
	_XTextureData *progressBackGround;			//�������ı�����ͼ
	_XTextureData *progressMove;				//���������ƶ��仯����ͼ
	_XTextureData *progressUpon;				//���������ϲ�����ֻ��������Ч

	_XProgressTexture();
	~_XProgressTexture();
	int init(const char *backgound,const char *move,const char *upon,int resoursePosition = 0);
	void release();
};

//����������ֵ�趨��ΧΪ0.0 �� 100.0
class _XProgress:public _XControlBasic
{
private:
	float m_nowValue;					//�������ĵ�ǰֵ

	char m_isInited;					//�������Ƿ񱻳�ʼ��
	char m_mode;	//0��ͨģʽ 1����ģʽ
	_XNumber m_caption;					//�������ı���

	const _XTextureData *m_progressBackGround;	//�������ı�����ͼ
	const _XTextureData *m_progressMove;			//���������ƶ��仯����ͼ
	const _XTextureData *m_progressUpon;			//���������ϲ�����ֻ��������Ч

	_XSprite m_spriteBackground;	//������ʾ�����ľ���
	_XSprite m_spriteMove;			//������ʾ��ͼ�ľ���
//	_XVector2 m_movePosition;			//����������ڱ�����λ��
	_XSprite m_spriteUpon;			//������ʾ�ϲ����ֵľ���
//	_XVector2 m_uponPosition;			//��������ڱ�����λ��
	_XVector2 m_textPosition;			//������ʾ��λ��
	_XVector2 m_textSize;				//���ֵĳߴ�
	_XFColor m_textColor;		//���ֵ���ɫ

	char m_isShowFont;				//�Ƿ���ʾ��������
	char m_isACopy;			//���ʵ���Ƿ���һ������

public:
	int init(const _XVector2& position,//�ؼ����ڵ�λ��
		const _XRect &Area,	//����������ķ�Χ
		const _XProgressTexture &tex,	//�ؼ�����ͼ
		_XNumber* font,float captionSize,const _XVector2& textPosition,	//�ؼ�������
		int mode = 0);
	void setPosition(const _XVector2& position);
	void setPosition(float x,float y);
	_XVector2 getPosition() const
	{
		return m_position;
	}

	void setSize(const _XVector2& size);
	void setSize(float x,float y);
	_XVector2 getSize() const
	{
		return m_size;
	}
	void setTextColor(const _XFColor& color) 
	{
		m_textColor = color;
		m_caption.setColor(m_textColor * m_color);
	}	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ
	void setColor(const _XFColor& color) 
	{
		m_color = color;
		m_spriteBackground.setColor(m_color);
		m_spriteMove.setColor(m_color);
		m_spriteUpon.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
	}	//���ð�ť����ɫ
	_XFColor getColor() const {return m_color;}	//��ȡ�ؼ�����ɫ

protected:
	void draw();
	void drawUp(){};						//do nothing
	int mouseProc(float x,float y,_XMouseState mouseState){return 1;}	//do nothing
	int keyboardProc(int keyOrder,_XKeyState keyState){return 1;}	//do nothing
	int canGetFocus(float x,float y){return 0;}	//�����жϵ�ǰ����Ƿ���Ի�ý���
	int canLostFocus(float x,float y){return 1;};
public:
	void setValue(float temp);
	float getValue() const;

	int setACopy(const _XProgress &temp);		//����һ������
	_XProgress();
	~_XProgress();
	void release();
	//��������������
	void enable();
	void disable();
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	int isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XProgress(const _XProgress &temp);
	_XProgress& operator = (const _XProgress& temp);
};

inline float _XProgress::getValue() const
{
	return m_nowValue;
}

inline void _XProgress::enable()
{
	m_isEnable = 1;
}

inline void _XProgress::disable()
{
	m_isEnable = 0;
}

inline void _XProgress::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}

inline void _XProgress::setSize(const _XVector2& size)
{
	setSize(size.x,size.y);
}
#endif