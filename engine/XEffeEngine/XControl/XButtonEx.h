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
	_XBool m_isInited;					//��ť�Ƿ񱻳�ʼ��
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
	_XResourceInfo *m_resInfo;
public:
	//��Ҫע���������������λ�ã����ſؼ������Ŵ���һЩbug����Ҫʵ��ʹ����΢������������̬Ч�����Ժ���Ҫ�Ľ�
	_XBool init(const _XVector2& position,			//�ؼ���λ��
		const _XVector2 *area,int pointSum,		//����������ť����Ӧ��Χ�����е㣬�Լ��������
		const _XButtonTexture &tex,				//��ť�ĸ�����ͼ��Ϣ
		const char *caption,float captionSize,_XVector2 captionPosition,	//��ť����ʵ�����ֵ������Ϣ
		const _XFontUnicode &font);				//��ť��ʹ�õ�����
	_XBool initEx(const _XVector2& position,			//�ؼ���λ��
		const _XButtonTexture &tex,				//��ť�ĸ�����ͼ��Ϣ
		const char *caption,float captionSize,	//��ť����ʵ�����ֵ������Ϣ
		const _XFontUnicode &font);				//��ť��ʹ�õ�����
	_XBool initPlus(const char *path,				//��ť�ĸ�����ͼ��Ϣ
		const char *caption,float captionSize,	//��ť����ʵ�����ֵ������Ϣ
		const _XFontUnicode &font,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);				//��ť��ʹ�õ�����

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;	//���⸲�ǵ�����
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color) 
	{
		m_textColor = color;
		m_caption.setColor(m_textColor * m_color);
	}	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a) 
	{
		m_color.setColor(r,g,b,a);
		m_sprite.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
	}	//���ð�ť����ɫ
	void setAlpha(float a) 
	{
		m_color.setA(a);
		m_sprite.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
	}	//���ð�ť����ɫ
protected:
	void draw();								//��水ť
	void drawUp(){};							//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);		//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);			//�����Ƿ񴥷���������
	void insertChar(const char *ch,int len){;}
	_XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float x,float y);
	void setLostFocus() 
	{
		if(m_isInited == 0) return;	//���û�г�ʼ��ֱ���˳�
		if(m_isActive == 0) return;		//û�м���Ŀؼ������տ���
		if(m_isVisiable == 0) return;	//������ɼ�ֱ���˳�
		if(m_isEnable == 0) return;		//�����Ч��ֱ���˳�

		if(m_nowButtonState != BUTTON_STATE_DISABLE) m_nowButtonState = BUTTON_STATE_NORMAL;
	}	//����ʧȥ����
public:
	void setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funMouseOn)(void *,int),
		void (* funMouseDown)(void *,int),
		void (* funMouseUp)(void *,int),
		void *pClass = NULL);
	void setTexture(const _XButtonTexture& tex);

	_XBool setACopy(const _XButtonEx &temp);

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
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	virtual void justForTest() {}
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
	if(m_isEnable == 0) return BUTTON_STATE_DISABLE;
	return m_nowButtonState;
}

inline void _XButtonEx::disable()
{
	m_isEnable = 0;
	m_nowButtonState = BUTTON_STATE_DISABLE;
}

inline void _XButtonEx::enable()
{
	if(m_nowButtonState == BUTTON_STATE_DISABLE)
	{
		m_isEnable = 1;
		m_nowButtonState = BUTTON_STATE_NORMAL;
		mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	}
}

#endif