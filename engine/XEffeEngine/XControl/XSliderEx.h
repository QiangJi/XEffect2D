#ifndef _JIA_XSLIDEREX_
#define _JIA_XSLIDEREX_
//������ɲ��ֲ���
//����һ����΢�����ص�slider���࣬����һ�������ࡣ
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.6
//--------------------------------

#include "XSlider.h"

class _XSliderEx:public _XControlBasic
{
private:
	_XBool m_isInited;
	_XSlider m_mainSld;			//����Ļ�����
	_XSlider m_secondarySld;	//�����Ļ�����
	_XButton m_chooseBtn;		//�л������������İ�ť

	int m_timer;	//��ʱ���������жϸ��ְ���״̬�Ƿ���Ҫ��ʾ

	void (*m_funInit)(void *,int);		//��ʼ��
	void (*m_funRelease)(void *,int);		//��Դ�ͷ�
	void (*m_funMouseOn)(void *,int);		//�������
	void (*m_funMouseDown)(void *,int);	//��갴��
	void (*m_funMouseUp)(void *,int);		//��굯��
	void (*m_funMouseMove)(void *,int);	//����϶�����ʱ����
	void (*m_funValueChange)(void *,int);	//����������ֵ�仯
	void *m_pClass;				//�ص������Ĳ���

	friend void sliderExBtnProc(void *pClass,int id);
	friend void sliderExSldProc(void *pClass,int id);
	friend void sliderExSldOnProc(void *pClass,int id);
	friend void sliderExSldUpProc(void *pClass,int id);
	friend void sliderExSldMoveProc(void *pClass,int id);
	friend void sliderExSldChangeProc(void *pClass,int id);
protected:
	void update(int stepTime);
	void draw();//��滬����
	void drawUp(){;}						//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);
	void insertChar(const char *ch,int len){;}
	_XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float x,float y){return XTrue;}
	void setLostFocus();	//����ʧȥ����
public:
	_XBool initWithoutTex(const _XRect& area,float max = 100.0f,float min = 0.0f,
		_XSliderType type = SLIDER_TYPE_HORIZONTAL,const _XVector2 &fontPosition = _XVector2(0.0f,0.0f));
	_XSliderEx()
		:m_isInited(XFalse)
		,m_timer(0)
		,m_funInit(NULL)		//��ʼ��
		,m_funRelease(NULL)		//��Դ�ͷ�
		,m_funMouseOn(NULL)		//�������
		,m_funMouseDown(NULL)	//��갴��
		,m_funMouseUp(NULL)		//��굯��
		,m_funMouseMove(NULL)	//����϶�����ʱ����
		,m_funValueChange(NULL)	//����������ֵ�仯
		,m_pClass(NULL)				//�ص������Ĳ���
	{}
	virtual ~_XSliderEx()
	{release();}

	void release();
	//������һЩ��������
	_XBool setFont(const char *caption,const _XFontUnicode &font,float captionSize,const _XVector2 &fontPosition);
	_XBool setFontEx(const char *caption,const _XFontUnicode &font,float captionSize);

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y);

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a);//�����������ɫ
	void setAlpha(float a);

	_XBool setACopy(const _XSliderEx &temp);			//����һ������

	//��������
	void setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funMouseOn)(void *,int),
		void (* funMouseDown)(void *,int),
		void (* funMouseUp)(void *,int),
		void (*funValueChange)(void *,int),
		void (*funMouseMove)(void *,int),
		void *pClass = NULL);
	void setNowValue(float temp);			//���û��鵱ǰ��ֵ
	void setRange(float max,float min);

	void disable();		//ʹ�ؼ���Ч
	void enable();		//ʹ�ؼ���Ч
	void setVisiable();
	void disVisiable();
	float getNowValue() const;	//��ȡ���鵱ǰ��ֵ
	float getMaxValue() const; 
	float getMinValue() const;
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y); //��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);		//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	virtual void justForTest() {;}
private:	//Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XSliderEx(const _XSliderEx &temp);
	_XSliderEx& operator = (const _XSliderEx& temp);
};
inline void _XSliderEx::disable()//ʹ�ؼ���Ч
{
	m_mainSld.disable();
	m_secondarySld.disable();
	m_chooseBtn.disable();
}
inline void _XSliderEx::enable()//ʹ�ؼ���Ч
{
	m_mainSld.enable();
	m_secondarySld.enable();
	m_chooseBtn.enable();
}
inline float _XSliderEx::getNowValue() const	//��ȡ���鵱ǰ��ֵ
{
	return m_mainSld.getNowValue();
}
inline float _XSliderEx::getMaxValue() const
{
	return m_mainSld.getMaxValue();
}
inline float _XSliderEx::getMinValue() const
{
	return m_mainSld.getMinValue();
}
inline void _XSliderEx::setCallbackFun(void (* funInit)(void *,int),
	void (* funRelease)(void *,int),
	void (* funMouseOn)(void *,int),
	void (* funMouseDown)(void *,int),
	void (* funMouseUp)(void *,int),
	void (* funValueChange)(void *,int),
	void (* funMouseMove)(void *,int),
	void *pClass)
{
	m_funInit = funInit;
	m_funRelease = funRelease;
	m_funMouseOn = funMouseOn;
	m_funMouseDown = funMouseDown;		//��Ч
	m_funMouseUp = funMouseUp;
	m_funValueChange = funValueChange;
	m_funMouseMove = funMouseMove;
	m_pClass = pClass;
}
inline void _XSliderEx::setNowValue(float temp)
{
	m_mainSld.setNowValue(temp);
}
inline void _XSliderEx::setRange(float max,float min)
{
	m_mainSld.setRange(max,min);
}
inline _XBool _XSliderEx::isInRect(float x,float y) //��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
inline _XVector2 _XSliderEx::getBox(int order)		//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(!m_isInited) return ret;
	return m_mainSld.getBox(order);
	return ret;
}
inline void _XSliderEx::setColor(float r,float g,float b,float a)
{
	if(!m_isInited) return;
	m_color.setColor(r,g,b,a);
	m_mainSld.setColor(m_color);
	m_secondarySld.setColor(m_color);
	m_chooseBtn.setColor(m_color);
	updateChildColor();
}//�����������ɫ
inline void _XSliderEx::setAlpha(float a)
{
	if(!m_isInited) return;
	m_color.setA(a);
	m_mainSld.setColor(m_color);
	m_secondarySld.setColor(m_color);
	m_chooseBtn.setColor(m_color);
	updateChildAlpha();
}
inline void _XSliderEx::release()
{
	if(!m_isInited) return;
	if(m_funRelease != NULL) m_funRelease(m_pClass,getControlID());
	m_isInited = false;
}
//������һЩ��������
inline _XBool _XSliderEx::setFont(const char *caption,const _XFontUnicode &font,float captionSize,const _XVector2 &fontPosition)
{
	return m_mainSld.setFont(caption,font,captionSize,fontPosition);
}
inline _XBool _XSliderEx::setFontEx(const char *caption,const _XFontUnicode &font,float captionSize)
{
	return m_mainSld.setFontEx(caption,font,captionSize);
}
inline _XBool _XSliderEx::keyboardProc(int keyOrder,_XKeyState keyState)
{
	return m_mainSld.keyboardProc(keyOrder,keyState);
}
inline void _XSliderEx::setLostFocus() 
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return;		//�����Ч��ֱ���˳�

	m_mainSld.setLostFocus();
	m_secondarySld.setLostFocus();
	m_chooseBtn.setLostFocus();
}
inline void _XSliderEx::setVisiable()
{
	m_isVisiable = XTrue;
	m_mainSld.setVisiable();
	//m_secondarySld.setVisiable();
	//m_chooseBtn.setVisiable();
	updateChildVisiable();
}
inline void _XSliderEx::disVisiable()
{
	m_isVisiable = XFalse;
	m_mainSld.disVisiable();
	m_secondarySld.disVisiable();
	m_chooseBtn.disVisiable();
	updateChildVisiable();
}
#endif