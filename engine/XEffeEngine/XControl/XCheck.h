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
	_XBool m_isInited;
	void releaseTex();
public:
	_XTextureData *checkChoosed;				//ѡ��ťѡ�е���ͼ
	_XTextureData *checkDischoose;				//ѡ��ťδѡ�е���ͼ
	_XTextureData *checkDisableChoosed;			//��Ч״̬��ѡ��ťѡ�е���ͼ
	_XTextureData *checkDisableDischoose;		//��Ч״̬��ѡ��ťδѡ�е���ͼ

	_XRect m_mouseRect;			//������Ӧ��Χ
	_XVector2 m_fontPosition;	//�������ֵ�λ��

	_XCheckTexture();
	~_XCheckTexture();
	_XBool init(const char *choosed,const char *disChoose,const char *disableChoosed,const char *disableDischoose,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};
class _XRadios;
class _XMultiListBasic;
class _XDirectoryList;
class _XCheck:public _XControlBasic
{
	friend _XRadios;
	friend _XMultiListBasic;
	friend _XDirectoryList;
private:
	_XBool m_isInited;					//�������Ƿ񱻳�ʼ��
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

	_XBool m_state;				//��ѡ���ѡ��״̬
	_XRect m_mouseClickArea;	//����������Ӧ��Χ
	_XBool m_withCaption;			//�Ƿ�ӵ������
		//�����ǹ��ڹ��������Ľӿ�
	_XBool *m_pVariable;	//�����ı���

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//û����ͼ����ʽ
public:
	void setConnectVar(_XBool * p) {m_pVariable = p;}	//��������
	void disConnectVar() {m_pVariable = NULL;}			//ȡ����������

public:
	_XBool getWithCaption() const {return m_withCaption;}
	void setWithCaption(_XBool withCaption)
	{
		m_withCaption = withCaption;
	}
	_XBool init(const _XVector2 & position,	//�ؼ���λ��
		const _XRect &Area,					//�ؼ���ͼƬ����Ӧ��Χ
		const _XCheckTexture &tex,			//�ؼ�����ͼ
		const char *caption,const _XFontUnicode &font,float captionSize,	//��ʾ������������Ϣ
		const _XVector2 &textPosition);		//�����λ��
	_XBool initEx(const _XVector2 & position,	//�ؼ���λ��
		const _XCheckTexture &tex,			//�ؼ�����ͼ
		const char *caption,const _XFontUnicode &font,float captionSize);	//��ʾ������������Ϣ
	_XBool initPlus(const char * path,			//�ؼ�����ͼ
		const char *caption,const _XFontUnicode &font,
		float captionSize,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//��ʾ������������Ϣ
	_XBool initWithoutTex(const char *caption,				//����
		const _XFontUnicode &font,float captionSize,	//�����Լ��ߴ�
		const _XRect& area,_XVector2 captionPosition);	//�ռ�Ĵ�С�Լ����ֵ����λ��
protected:
	void draw();
	void drawUp(){};						//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//������궯������Ӧ����(���ض����Ƿ���ɸı�)
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);		//�Լ��̶�������Ӧ
	void insertChar(const char *ch,int len){;}
	_XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float x,float y){return XTrue;}
public:
	void setCallbackFun(void (* funInit)(void *,int),
							 void (* funRelease)(void *,int),
							 void (* funMouseOn)(void *,int),
							 void (* funMouseDown)(void *,int),
							 void (* funMouseUp)(void *,int),
							 void (*funStateChange)(void *,int),void *pClass = NULL);

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);//���ÿؼ���λ��

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y);

	void setTextColor(const _XFColor& color) 
	{
		m_textColor = color;
		m_caption.setColor(m_textColor * m_color);
	}	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a) 	//���ð�ť����ɫ
	{
		m_color.setColor(r,g,b,a);
		m_sprite.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		updateChildColor();
	}	//���ð�ť����ɫ
	void setAlpha(float a) 
	{
		m_color.setA(a);
		m_sprite.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		updateChildAlpha();
	}	//���ð�ť����ɫ

	void setText(const char *temp);				//���ø�ѡ�������
	_XBool setACopy(const _XCheck &temp);			//����һ������
	_XCheck();
	~_XCheck();
	void release();
	void disable();
	void enable();
	_XBool getState() const;
	void setState(_XBool temp);
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	virtual void justForTest() {;}
private:	//�������ظ��ƹ��캯���͸�ֵ����������ֹ������õĴ���
	_XCheck(const _XCheck &temp);
	_XCheck& operator = (const _XCheck& temp);
};

inline void _XCheck::disable()
{
	if(!m_isInited) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isEnable)m_isEnable = XFalse;
}
inline void _XCheck::enable()
{
	if(!m_isInited) return ;	//���û�г�ʼ��ֱ���˳�
	if(!m_isEnable) m_isEnable = XTrue;
}
inline _XBool _XCheck::getState() const
{
	return m_state;
}
inline void _XCheck::setState(_XBool temp)
{
	//������״̬���ۺ�ʱ���ǿ��Ըı��
//	if(!m_isVisiable) return;	//������ɼ�ֱ���˳�
//	if(!m_isEnable) return;
	if((m_state && temp) || (!m_state && !temp)) return;
	m_state = temp;
//	if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
}
#endif