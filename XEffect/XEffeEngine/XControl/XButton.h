#ifndef _JIA_XBUTTON_
#define _JIA_XBUTTON_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XControlBasic.h"
#include "../XFontUnicode.h"
#include "../XSprite.h"

//����һ����ť���࣬������Ӧ��ť�¼���

enum _XButtonState
{
	_XBUTTON_STATE_NORMAL,		//��ť״̬��ͨ״̬
	_XBUTTON_STATE_DOWN,		//��ť���µ�״̬
	_XBUTTON_STATE_ON,			//��ť��괦��������״̬
	_XBUTTON_STATE_DISABLE		//��ť��Ч��״̬
};

//�ؼ��¼�����Ӧʹ�ûص������ķ�ʽ
//�����¼�	��ʱ�Ȳ���
//ע���¼�	��ʱ�Ȳ���
//�����¼�
//�����¼�
//�����¼�
class _XButtonTexture
{
private:
	char m_isInited;
	//_XSCounter *m_cp;		//���ü�����,��δ���
public:
	_XTextureData *buttonNormal;			//��ͨ״̬�µİ�ť��ͼ
	_XTextureData *buttonDown;				//����״̬�µİ�ť��ͼ
	_XTextureData *buttonOn;				//����״̬�µİ�ť��ͼ
	_XTextureData *buttonDisable;			//��Ч״̬�µİ�ť��ͼ

	_XButtonTexture();
	~_XButtonTexture();
	int init(const char *normal,const char *down,const char *on,const char *disable,int resoursePosition = 0);
	void release();
};

//���ڵ����⣺����û����ʾ����
class _XCombo;
class _XMouseRightButtonMenu;
class _XButton:public _XControlBasic
{
	friend _XCombo;
	friend _XMouseRightButtonMenu;
private:
	char m_isInited;					//��ť�Ƿ񱻳�ʼ��
	_XButtonState m_nowButtonState;		//��ǰ�İ�ť״̬

	_XFontUnicode m_caption;			//��ť�ı���

	const _XTextureData *m_buttonNormal;			//��ͨ״̬�µİ�ť��ͼ(������Σ������ͼһ��Ҫ��)
	const _XTextureData *m_buttonDown;			//����״̬�µİ�ť��ͼ
	const _XTextureData *m_buttonOn;				//����״̬�µİ�ť��ͼ
	const _XTextureData *m_buttonDisable;			//��Ч״̬�µİ�ť��ͼ

	void (*m_funInit)(void *,int ID);
	void (*m_funRelease)(void *,int ID);
	void (*m_funMouseOn)(void *,int ID);
	void (*m_funMouseDown)(void *,int ID);
	void (*m_funMouseUp)(void *,int ID);
	void *m_pClass;				//�ص������Ĳ���

	_XSprite m_sprite;	//������ʾ��ͼ�ľ���
	_XVector2 m_textPosition;	//������ʾ��λ�ã�������ڿؼ���λ��������
	_XVector2 m_textSize;		//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	_XFColor m_textColor;		//���ֵ���ɫ
	_XVector2 m_upMousePoint;	//�ϴμ�¼�����λ��

	int m_hotKey;
public:
	//��Ҫע���������������λ�ã����ſؼ������Ŵ���һЩbug����Ҫʵ��ʹ����΢������������̬Ч�����Ժ���Ҫ�Ľ�
	int init(const _XVector2& position,	//�ؼ����ڵ�λ��
		const _XRect& Area,				//�ؼ������Ӧ�����䣬ʹ�õ��������ͼƬ���Ͻǵ�����
		const _XButtonTexture &tex,		//�ؼ�����ͼ��Ϣ
		const char *caption,const _XFontUnicode &font,float captionSize,_XVector2 textPosition);	//�ؼ�����������Ϣ

	void setPosition(const _XVector2& position);		//���ð�ť��λ��
	void setPosition(float x,float y);
	_XVector2 getPosition() const
	{
		return m_position;
	}
	void setSize(const _XVector2& size);				//���ð�ť�����ű���
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
		m_sprite.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
	}	//���ð�ť����ɫ
	_XFColor getColor() const {return m_color;}	//��ȡ�ؼ�����ɫ
protected:
	void draw();								//��水ť
	void drawUp(){};							//do nothing
	int mouseProc(float x,float y,_XMouseState mouseState);		//������궯������Ӧ����
	int keyboardProc(int keyOrder,_XKeyState keyState);			//�����Ƿ񴥷���������
	int canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	int canLostFocus(float x,float y);	//Ӧ���ǿ�����ʱʧȥ�����
	void setLostFocus() 
	{
		if(m_isInited == 0) return;	//���û�г�ʼ��ֱ���˳�
		if(m_isActive == 0) return;		//û�м���Ŀؼ������տ���
		if(m_isVisiable == 0) return;	//������ɼ�ֱ���˳�
		if(m_isEnable == 0) return;		//�����Ч��ֱ���˳�

		if(m_nowButtonState != _XBUTTON_STATE_DISABLE) m_nowButtonState = _XBUTTON_STATE_NORMAL;
	}	//����ʧȥ����
public:
	void setCallbackFun(void (* funInit)(void *,int),void (* funRelease)(void *,int),void (* funMouseOn)(void *,int),void (* funMouseDown)(void *,int),void (* funMouseUp)(void *,int),void *pClass);
	void setTexture(const _XButtonTexture& tex);

	int setACopy(const _XButton &temp);

	_XButton();
	~_XButton();
	void release();
	//������Ϊ������Ч�ʶ��������������
	void setCaptionPosition(const _XVector2& textPosition);			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
	void setCaptionPosition(float x,float y);			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
	void setCaptionSize(const _XVector2& size);						//���ð�ť�ı���ĳߴ�
	void setCaptionSize(float x,float y);						//���ð�ť�ı���ĳߴ�
	void setCaptionText(const char *caption);						//���ð�ť�ı��������
	void setHotKey(int hotKey);	//���ð������ȼ�
	int getHotKey() const;			//��ȡ��ǰ�������ȼ���Ϣ
	void setState(_XButtonState temp);		//���ð�ť��״̬
	_XButtonState getState() const; 
	void disable();
	void enable();
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	int isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	_XButton(const _XButton &temp);
	_XButton& operator = (const _XButton& temp);
};

inline void _XButton::setCaptionPosition(const _XVector2& textPosition)			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
{
	setCaptionPosition(textPosition.x,textPosition.y);
}

inline void _XButton::setCaptionPosition(float x,float y)			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
{
	m_textPosition.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
}

inline void _XButton::setCaptionSize(const _XVector2& size)						//���ð�ť�ı���ĳߴ�
{
	setCaptionSize(size.x,size.y);
}

inline void _XButton::setCaptionSize(float x,float y)						//���ð�ť�ı���ĳߴ�
{
	if(x < 0 || y < 0) return;
	m_textSize.set(x,y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
}

inline void _XButton::setHotKey(int hotKey)	//���ð������ȼ�
{
	m_hotKey = hotKey;
}

inline void _XButton::setCaptionText(const char *caption)						//���ð�ť�ı��������
{
	if(caption != NULL) m_caption.setString(caption);
}

inline int _XButton::getHotKey() const			//��ȡ��ǰ�������ȼ���Ϣ
{
	return m_hotKey;
}

inline void _XButton::setState(_XButtonState temp)		//���ð�ť��״̬
{
	m_nowButtonState = temp;
}
 
inline _XButtonState _XButton::getState() const
{
	if(m_isEnable == 0) return _XBUTTON_STATE_DISABLE;
	return m_nowButtonState;
}

inline void _XButton::disable()
{
	m_isEnable = 0;
	m_nowButtonState = _XBUTTON_STATE_DISABLE;
}

inline void _XButton::enable()
{
	if(m_nowButtonState == _XBUTTON_STATE_DISABLE)
	{
		m_isEnable = 1;
		m_nowButtonState = _XBUTTON_STATE_NORMAL;
		mouseProc(m_upMousePoint.x,m_upMousePoint.y,_XMOUSE_MOVE);
	}
}

inline void _XButton::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}

inline void _XButton::setSize(const _XVector2& size)			//���ð�ť�����ű���
{
	setSize(size.x,size.y);
}

#endif