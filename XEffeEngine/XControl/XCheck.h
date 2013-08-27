#ifndef _JIA_XCHECK_
#define _JIA_XCHECK_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//����һ������ʵ�ָ�ѡ�����

#include "XControlBasic.h"
#include "../XFontUnicode.h"
#include "../XSprite.h"

#define XCHECK_LEFT_ADD_LENGTH 15	//���ַ��������������س�����Ϊ��Ӧ��Χ��Ϊ��ʹ���Ƹ��Ӻ���

class _XCheckTexture
{
private:
	char m_isInited;
public:
	_XTextureData *checkChoosed;				//ѡ��ťѡ�е���ͼ
	_XTextureData *checkDischoose;				//ѡ��ťδѡ�е���ͼ
	_XTextureData *checkDisableChoosed;			//��Ч״̬��ѡ��ťѡ�е���ͼ
	_XTextureData *checkDisableDischoose;		//��Ч״̬��ѡ��ťδѡ�е���ͼ

	_XCheckTexture();
	~_XCheckTexture();
	int init(const char *choosed,const char *disChoose,const char *disableChoosed,const char *disableDischoose,int resoursePosition = 0);
	void release();
};
class _XRadios;
class _XMultiListBasic;
class _XCheck:public _XControlBasic
{
	friend _XRadios;
	friend _XMultiListBasic;
private:
	char m_isInited;					//�������Ƿ񱻳�ʼ��
	_XFontUnicode m_caption;			//�������ı���

	const _XTextureData *m_checkChoosed;			//ѡ��ťѡ�е���ͼ
	const _XTextureData *m_checkDischoose;		//ѡ��ťδѡ�е���ͼ
	const _XTextureData *m_checkDisableChoosed;	//��Ч״̬��ѡ��ťѡ�е���ͼ
	const _XTextureData *m_checkDisableDischoose;	//��Ч״̬��ѡ��ťδѡ�е���ͼ

	void (*m_funInit)(void *,int ID);				//�ؼ���ʼ����ʱ����ã������Ŀǰû��ʵ����Ч��
	void (*m_funRelease)(void *,int ID);				//�ؼ�ע����ʱ����ã������Ŀǰû��ʵ����Ч��
	void (*m_funMouseOn)(void *,int ID);				//�������ʱ���ã������Ŀǰû��ʵ����Ч��
	void (*m_funMouseDown)(void *,int ID);			//��갴��ʱ���ã������Ŀǰû��ʵ����Ч��		
	void (*m_funMouseUp)(void *,int ID);				//��굯��ʱ����
	void (*m_funStateChange)(void *,int ID);			//�ؼ�״̬�ı�ʱ����
	void *m_pClass;				//�ص������Ĳ���

	_XSprite m_sprite;			//������ʾ��ͼ�ľ���
	_XVector2 m_textPosition;		//���ֵ����λ��
	_XVector2 m_textSize;			//���ֵĳߴ�
	_XFColor m_textColor;		//���ֵ���ɫ

	bool m_state;				//��ѡ���ѡ��״̬
	_XRect m_mouseClickArea;	//����������Ӧ��Χ
	char m_withCaption;			//�Ƿ�ӵ������
public:
	char getWithCaption() const {return m_withCaption;}
	void setWithCaption(char withCaption)
	{
		if(withCaption != 0) withCaption = 1;
		m_withCaption = withCaption;
	}
	int init(const _XVector2 & position,	//�ؼ���λ��
		const _XRect &Area,					//�ؼ���ͼƬ����Ӧ��Χ
		const _XCheckTexture &tex,			//�ؼ�����ͼ
		const char *caption,const _XFontUnicode &font,float captionSize,	//��ʾ������������Ϣ
		const _XVector2 &textPosition);		//�����λ��
protected:
	void draw();
	void drawUp(){};						//do nothing
	int mouseProc(float x,float y,_XMouseState mouseState);	//������궯������Ӧ����(���ض����Ƿ���ɸı�)
	int keyboardProc(int keyOrder,_XKeyState keyState);		//�Լ��̶�������Ӧ
	int canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	int canLostFocus(float x,float y);
public:
	void setCallbackFun(void (* funInit)(void *,int),
							 void (* funRelease)(void *,int),
							 void (* funMouseOn)(void *,int),
							 void (* funMouseDown)(void *,int),
							 void (* funMouseUp)(void *,int),
							 void (*funStateChange)(void *,int),void *pClass);

	void setPosition(const _XVector2 & position);		//���ÿؼ���λ��
	void setPosition(float x,float y);
	_XVector2 getPosition() const
	{
		return m_position;
	}
	void setSize(const _XVector2 &size);				//���ÿؼ������ű���
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
	void setText(const char *temp);				//���ø�ѡ�������
	int setACopy(const _XCheck &temp);			//����һ������
	_XCheck();
	~_XCheck();
	void release();
	void disable();
	void enable();
	bool getState() const;
	void setState(bool temp);
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	int isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private:	//�������ظ��ƹ��캯���͸�ֵ����������ֹ������õĴ���
	_XCheck(const _XCheck &temp);
	_XCheck& operator = (const _XCheck& temp);
};

inline void _XCheck::disable()
{
	if(m_isInited == 0) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isEnable != 0)m_isEnable = 0;
}

inline void _XCheck::enable()
{
	if(m_isInited == 0) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isEnable == 0) m_isEnable = 1;
}

inline bool _XCheck::getState() const
{
	return m_state;
}

inline void _XCheck::setState(bool temp)
{
	if(m_isVisiable == 0) return;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return;
	if(m_state == temp) return;
	m_state = temp;
//	if(m_funStateChange != NULL) (* m_funStateChange)(m_pClass,m_objectID);
}

inline void _XCheck::setPosition(const _XVector2 &position)
{
	setPosition(position.x,position.y);
}

inline void _XCheck::setSize(const _XVector2 &size)
{
	setSize(size.x,size.y);
}
#endif