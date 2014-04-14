#ifndef _JIA_XPOINTCTRL_
#define _JIA_XPOINTCTRL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XControlBasic.h"
#include "../XFontUnicode.h"

//����һ������ԭ�ؼ��㣬������Ӧ����λ�ò���
#define XPOINT_CTRL_LINE_SUM (10)
class _XPointCtrl:public _XControlBasic
{
private:
	_XBool m_isInited;		//�Ƿ��ʼ��
	_XVector2 m_position;	//�ؼ���λ��
	_XVector2 m_size;		//��С
	_XVector2 m_pixelSize;	//���ش�С
	_XVector2 m_truePixelSize;	//��ʵ�����سߴ�
	_XFontUnicode m_font;
	char m_textStr[64];		//��ʾ���ַ���

	bool m_withFont;
	bool m_isDown;		//�Ƿ����ʰȡ
	void updateData();

	void (*m_funDataChange)(void *,int ID);	//���ݷ����仯��ʱ��ŵ��õĻص�����
	void *m_pClass;	

	bool m_withRange;	//��Χ�����Ƿ���Ч
	_XRect m_range;		//�����ƶ��ķ�Χ
	bool m_withMap;		//�Ƿ����ӳ��
	_XRect m_mapRange;	//ӳ��ķ�Χ
	_XVector2 m_mapValue;
	_XCtrlMode m_ctrlMode;		//�ؼ���ģʽ
public:
	void setCtrlMode(_XCtrlMode mode){m_ctrlMode = mode;}
	void setRange(float l,float t,float r,float b)
	{
		m_withRange = true;
		m_range.set(l,t,r,b);
		updateData();
	}
	void disRange(){m_withRange = false;}
	_XVector2 getValue() 
	{
		if(m_withMap) return m_mapValue;
		return m_position;
	}
	void setMapRange(float l,float t,float r,float b)
	{
		m_withMap = true;
		m_mapRange.set(l,t,r,b);
	}
	void disMapRange(){m_withMap = false;}
public:
	_XBool init(const _XVector2& position,const _XFontUnicode *font = NULL);
	void draw();
	void drawUp(){;}
	void setCallbackFun(void (* funDataChange)(void *,int),void *pClass = NULL)
	{
		m_funDataChange = funDataChange;
		m_pClass = pClass;
	}

	_XBool mouseProc(float x,float y,_XMouseState mouseState);
	_XBool keyboardProc(int keyOrder,_XKeyState keyState){return XTrue;}		//do nothing
	_XBool canGetFocus(float x,float y)
	{
		if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
		if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
		if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
		if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
		return isInRect(x,y);
	}	//do nothing	//�¼����Դ�͸���������XTrue������¼����ܴ�͸
	_XBool canLostFocus(float x,float y){return XTrue;}	//do nothing

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y)
	{
		m_position.set(x,y);
		updateData();
		updateChildPos();
	}

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y) 
	{
		m_size.set(x,y);
		updateData();
		updateChildSize();
	}

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a){updateChildColor();}	//do nothing
	void setAlpha(float a){updateChildAlpha();}							//do nothing

	void insertChar(const char *ch,int len){;}			//do nothing

	void setLostFocus() {;}							//do nothing
	void disable(){m_isEnable = 0;}	
	void enable(){m_isEnable = 1;}
	void release();

	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	_XBool setACopy(const _XPointCtrl & temp);

	virtual void justForTest() {;}

	_XPointCtrl()
		:m_isInited(false)
		,m_position(0.0f,0.0f)
		,m_size(1.0f,1.0f)
		,m_withFont(false)
		,m_pixelSize(10.0f,10.0f)
		,m_isDown(false)
		,m_range(0.0f,0.0f,100.0f,100.0f)
		,m_withRange(false)
		,m_mapRange(0.0f,0.0f,100.0f,100.0f)
		,m_withMap(false)
		,m_mapValue(0.0f,0.0f)
		,m_funDataChange(NULL)
		,m_pClass(NULL)
		,m_ctrlMode(CTRL_MODE_SIMPLE)
	{
	}
	~_XPointCtrl()
	{
		release();
	}
};

inline _XBool _XPointCtrl::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
inline _XVector2 _XPointCtrl::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;

	if(order == 0) ret.set(m_position.x - m_truePixelSize.x,m_position.y - m_truePixelSize.y);else
	if(order == 1) ret.set(m_position.x + m_truePixelSize.x,m_position.y - m_truePixelSize.y);else
	if(order == 2) ret.set(m_position.x + m_truePixelSize.x,m_position.y + m_truePixelSize.y);else
	if(order == 3) ret.set(m_position.x - m_truePixelSize.x,m_position.y + m_truePixelSize.y);

	return ret;
}
#endif