#ifndef _JIA_XBUTTONEX_
#define _JIA_XBUTTONEX_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.12
//--------------------------------
//����һ����XButton���ǿ��������֧�ָ�����״��button
#include "XButton.h"

class _XButtonEx:public _XControlBasic
{
private:
	char m_isInited;					//��ť�Ƿ񱻳�ʼ��
	_XButtonState m_nowButtonState;		//��ǰ�İ�ť״̬

	_XFontUnicode m_caption;			//��ť�ı���

	const _XTextureData *m_buttonNormal;		//��ͨ״̬�µİ�ť��ͼ(������Σ������ͼһ��Ҫ��)
	const _XTextureData *m_buttonDown;			//����״̬�µİ�ť��ͼ
	const _XTextureData *m_buttonOn;			//����״̬�µİ�ť��ͼ
	const _XTextureData *m_buttonDisable;		//��Ч״̬�µİ�ť��ͼ

	void (*m_funInit)(void *,int ID);			//�ؼ���ʼ����ʱ����õĺ���
	void (*m_funRelease)(void *,int ID);		//�ؼ���Դ�ͷŵ�ʱ����õĺ���
	void (*m_funMouseOn)(void *,int ID);		//���������ʱ����õĺ���
	void (*m_funMouseDown)(void *,int ID);		//��갴�µ�ʱ����õĺ���
	void (*m_funMouseUp)(void *,int ID);		//��굯���ʱ�����õĺ���
	void *m_pClass;				//�ص������Ĳ���

	_XSprite m_sprite;			//������ʾ��ͼ�ľ���
	_XVector2 m_textPosition;	//������ʾ��λ�ã�������ڿؼ���λ��������
	_XVector2 m_textSize;		//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	_XFColor m_textColor;		//���ֵ���ɫ
	_XVector2 m_upMousePoint;	//�ϴμ�¼�����λ��
	_XVector2 *m_pArea;			//������״�ĵ������
	_XVector2 *m_pNowArea;		//������״�ĵ������
	int m_areaPointSum;			//������״�ĵ������

	int m_hotKey;
public:
	//��Ҫע���������������λ�ã����ſؼ������Ŵ���һЩbug����Ҫʵ��ʹ����΢������������̬Ч�����Ժ���Ҫ�Ľ�
	int init(const _XVector2& position,			//�ؼ���λ��
		const _XVector2 *area,int pointSum,		//����������ť����Ӧ��Χ�����е㣬�Ѿ��������
		const _XButtonTexture &tex,				//��ť�ĸ�����ͼ��Ϣ
		const char *caption,float captionSize,_XVector2 captionPosition,	//��ť����ʵ�����ֵ������Ϣ
		const _XFontUnicode &font);				//��ť��ʹ�õ�����

	void setPosition(const _XVector2& position);		//���ð�ť��λ��
	void setPosition(float x,float y);
	_XVector2 getPosition() const {return m_position;}

	void setSize(const _XVector2& size);				//���ð�ť�����ű���
	void setSize(float x,float y);
	_XVector2 getSize() const {return m_size;}

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
	int canLostFocus(float x,float y);
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

	int setACopy(const _XButtonEx &temp);

	_XButtonEx();
	~_XButtonEx();
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
	void release();	//��Դ�ͷź���
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	int isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	_XButtonEx(const _XButtonEx &temp);
	_XButtonEx& operator = (const _XButtonEx& temp);
};

inline void _XButtonEx::setCaptionPosition(const _XVector2& textPosition)			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
{
	setCaptionPosition(textPosition.x,textPosition.y);
}

inline void _XButtonEx::setCaptionPosition(float x,float y)			//���ð�ť�ı����λ�ã�����ڰ������Ͻ�
{
	m_textPosition.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
}

inline void _XButtonEx::setCaptionSize(const _XVector2& size)						//���ð�ť�ı���ĳߴ�
{
	setCaptionSize(size.x,size.y);
}

inline void _XButtonEx::setCaptionSize(float x,float y)						//���ð�ť�ı���ĳߴ�
{
	if(x < 0 || y < 0) return;
	m_textSize.set(x,y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
}

inline void _XButtonEx::setHotKey(int hotKey)	//���ð������ȼ�
{
	m_hotKey = hotKey;
}

inline void _XButtonEx::setCaptionText(const char *caption)						//���ð�ť�ı��������
{
	if(caption != NULL)
	{
		m_caption.setString(caption);
	}
}

inline int _XButtonEx::getHotKey() const			//��ȡ��ǰ�������ȼ���Ϣ
{
	return m_hotKey;
}

inline void _XButtonEx::setState(_XButtonState temp)		//���ð�ť��״̬
{
	m_nowButtonState = temp;
}
 
inline _XButtonState _XButtonEx::getState() const
{
	if(m_isEnable == 0) return _XBUTTON_STATE_DISABLE;
	return m_nowButtonState;
}

inline void _XButtonEx::disable()
{
	m_isEnable = 0;
	m_nowButtonState = _XBUTTON_STATE_DISABLE;
}

inline void _XButtonEx::enable()
{
	if(m_nowButtonState == _XBUTTON_STATE_DISABLE)
	{
		m_isEnable = 1;
		m_nowButtonState = _XBUTTON_STATE_NORMAL;
		mouseProc(m_upMousePoint.x,m_upMousePoint.y,_XMOUSE_MOVE);
	}
}

inline void _XButtonEx::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}

inline void _XButtonEx::setSize(const _XVector2& size)			//���ð�ť�����ű���
{
	setSize(size.x,size.y);
}

#endif