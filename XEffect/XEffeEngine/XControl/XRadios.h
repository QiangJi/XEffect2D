#ifndef _JIA_XRADIOS_
#define _JIA_XRADIOS_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//����һ����������࣬������MFC��������ġ�
//1����ѡ�����Զ������Ҫ�ڳ�ʼ����ʱ���趨��һ�鵥ѡ���еĵ�ѡ������
//2�������ĵ�ѡ��ʵ�����Ǿ��ǵ����ĸ�ѡ��ʵ������û�������

//�ؼ��ļ��ʽ��Ҫ�޸�

//������ƶ������ţ�ͳһʹ���������ռ��������ƶ�������
#include "XCheck.h"

typedef _XCheckTexture _XRadiosTexture;

class _XRadios:public _XControlBasic
{
private:
	char m_isInited;	//�ռ�ʱ���Ѿ���ʼ��

	int m_radioSum;		//ѡ�������
	int m_nowChoose;	//��ǰ��ѡ����ı��
	_XCheck *m_radio;	//���е�ѡ���ָ��
	_XVector2 *m_checkPosition;	//��ѡ������λ��
	_XVector2 m_distance;	//���õ�ÿ����ѡ��֮��ľ���

	void (*m_funStateChange)(void *,int);			//�ؼ�״̬�ı�ʱ����
	void *m_pClass;				//�ص������Ĳ���

	const _XCheckTexture *m_texture;

	_XFontUnicode m_caption;
	float m_captionSize;
	_XFColor m_textColor;	//��ʾ���������ɫ
	_XVector2 m_textPosition;
public:
	int init(int radioSum,			//ѡ�������
		const _XVector2& distance,	//ÿ����ѡ��֮��ľ���
		const _XVector2& position,	//�ؼ���λ��
		const _XRect &Area,			//ѡ��ͼ��������Ӧ��Χ
		const _XRadiosTexture* tex,const _XFontUnicode &font,float captionSize,
		const _XVector2& textPosition);		//��ѡ���ʼ��
protected:
	void draw();
	void drawUp(){};						//do nothing
	int mouseProc(float x,float y,_XMouseState mouseState);	//������궯������Ӧ����
	int keyboardProc(int keyOrder,_XKeyState keyState);		//�����ڼ���״̬��ʱ�򣬿���ͨ����������4�������ı�ѡ���ֵ
	int canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	int canLostFocus(float x,float y);
public:
	void disable();
	void enable();
	void setChoosed(int temp);									//���õ�ǰѡ��ĵ�ѡ��
	void setRadioPosition(const _XVector2& position,int order);			//���õ�ѡ����ĳһ���λ��(����ʹ���������)
	void setRadioPosition(float x,float y,int order);			//���õ�ѡ����ĳһ���λ��(����ʹ���������)
	
	void setDistance(const _XVector2& distance)
	{
		if(m_isInited == 0) return;
		m_distance = distance;
		//��������
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_checkPosition[i].set(m_distance.x * i,m_distance.y * i);
			m_radio[i].setPosition(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y);
		}
	}
	void setSize(const _XVector2& size);				//�������ű���
	void setSize(float x,float y);				//�������ű���
	_XVector2 getSize() const
	{
		return m_size;
	}
	void setPosition(const _XVector2& position);		//����λ��
	void setPosition(float x,float y);		//����λ��
	_XVector2 getPosition() const
	{
		return m_position;
	}
	void setTextColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_textColor = color;
		m_caption.setColor(m_textColor * m_color);
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].setTextColor(m_textColor * m_color);
		}
	}	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ
	void setColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_color = color;
		m_caption.setColor(m_textColor * m_color);
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].setColor(m_color);
		}
	}	//���ð�ť����ɫ
	_XFColor getColor() const {return m_color;}	//��ȡ�ؼ�����ɫ

	int setACopy(const _XRadios &temp);			//����һ������
	int setRadioSum(int radioSum);

	_XRadios();
	~_XRadios();
    //��������������
	void release();	//�ͷŷ������Դ
	void setCallbackFun(void (* funStateChange)(void *,int),void *pClass);
	int getNowChoose();	//���ص�ǰ��ѡ����ѡ�����ı��
	void setRadioText(const char *temp,int order);	//���õ�ѡ����ĳһ�������
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	int isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private:	//Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XRadios(const _XRadios &temp);
	_XRadios& operator = (const _XRadios& temp);
};

inline void _XRadios::setCallbackFun(void (* funStateChange)(void *,int),void *pClass)
{
	if(funStateChange != NULL) m_funStateChange = funStateChange;
	m_pClass = pClass;
}

inline int _XRadios::getNowChoose()	//���ص�ǰ��ѡ����ѡ�����ı��
{
	return m_nowChoose;
}

inline void _XRadios::setRadioText(const char *temp,int order)	//���õ�ѡ����ĳһ�������
{
	if(m_isInited == 0) return ;	//���û�г�ʼ��ֱ���˳�
	if(temp == NULL) return;
	if(order < 0 || order >= m_radioSum) return;
	m_radio[order].setText(temp);
}

inline void _XRadios::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}

inline void _XRadios::setSize(const _XVector2& size)
{
	setSize(size.x,size.y);
}

inline void _XRadios::setRadioPosition(const _XVector2& position,int order)
{
	setRadioPosition(position.x,position.y,order);
}
#endif