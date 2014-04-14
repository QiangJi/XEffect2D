#ifndef _JIA_XGROUP_
#define _JIA_XGROUP_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XControlBasic.h"
#include "../XFontUnicode.h"
#include "XButton.h"

//Ŀǰ��δʵ�֣����ǽ�����ӵĹ���
//1������������֮�����ڵĳ�Ա״̬���ᱻ�ı�
//2�����ˣ����˵���һ�β�������ֵ��������ܿ��Ի���һֱ�������״̬
//3�����̲��������е����ݶ�����ͨ�����̲��������

enum _XGroupState
{
	STATE_NORMAL,		//����״̬
	STATE_MINISIZE,		//��С��״̬
};
#define XGROUP_STATE_BTN_SIZE (32.0f)
//����һ��Ⱥ��Ŀռ䣬�Ƚϼٵ���һ���ؼ�
class _XGroup:public _XControlBasic
{
private:
	_XBool m_isInited;
	_XVector2 m_position;	//�ؼ���λ��
	_XVector2 m_size;		//��С
	_XRect m_rect;			//�ؼ��ķ�Χ
	_XRect m_drawRect;		//��ǰ�ռ�ķ�Χ

	_XFontUnicode m_caption;
	_XVector2 m_textSize;

	_XGroupState m_state;
	_XButton m_stateBotton;

	void (*m_funStateChange)(void *,int ID);	//״̬�����ı�ʱ���õĺ���
	void *m_pClass;

	friend void funXGroupStateBtn(void *pClass,int objectID);
	void updateData()		//�����ڲ�����
	{
		if(m_state == STATE_NORMAL)
		{
			m_caption.setPosition(m_position.x + (m_rect.left + XGROUP_STATE_BTN_SIZE + 5.0f) * m_size.x,m_position.y + m_rect.top * m_size.y);
		//	m_stateBotton.setPosition(m_position.x + (m_rect.right - XGROUP_STATE_BTN_SIZE) * m_size.x,m_position.y + m_rect.top * m_size.y);
			m_stateBotton.setPosition(m_position.x,m_position.y + m_rect.top * m_size.y);
			m_drawRect.set(m_rect.left * m_size.x + m_position.x,m_rect.top * m_size.y + m_position.y,
				m_rect.right * m_size.x + m_position.x,m_rect.bottom * m_size.y + m_position.y);
		}else
		{
			m_caption.setPosition(m_position.x + (XGROUP_STATE_BTN_SIZE + 3.0f) * m_size.x,m_position.y + 3.0f * m_size.y);
		//	m_stateBotton.setPosition(m_position.x + 3.0f * m_size.x + m_caption.getMaxPixelWidth(),m_position.y + 3.0f * m_size.y);
			m_stateBotton.setPosition(m_position.x,m_position.y + 3.0f * m_size.y);
			m_drawRect.left = m_rect.left * m_size.x + m_position.x;
			m_drawRect.top = m_rect.top * m_size.y + m_position.y;
			m_drawRect.right = m_drawRect.left + (6.0f + XGROUP_STATE_BTN_SIZE) * m_size.x + m_caption.getMaxPixelWidth();
			m_drawRect.bottom = m_drawRect.top + (6.0f + XGROUP_STATE_BTN_SIZE) * m_size.x;
		}
	}
public:
	_XBool init(const _XVector2& position,
		const _XRect &rect,
		const char *caption,const _XFontUnicode &font,float captionSize);
	void setState(_XGroupState state)
	{
		if(m_state == state) return;
		m_state = state;
		if(m_state == STATE_NORMAL)
		{
			m_stateBotton.setCaptionText("-");
			//��������������
			for(int i = 0;i < m_childList.size();++ i)
			{
				m_childList[i]->setVisiable();
			}
		}else
		{
			m_stateBotton.setCaptionText("+");
			//��ʾ����������
			for(int i = 0;i < m_childList.size();++ i)
			{
				m_childList[i]->disVisiable();
			}
		}
		updateData();
		if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
	}
	_XGroupState getState(){return m_state;}	//��ȡ�ؼ���״̬
	void setCallbackFun(void (* funStateChange)(void *,int),
		void *pClass = NULL)
	{
		m_funStateChange = funStateChange;
		m_pClass = pClass;
	}
	void resetSize(_XVector2 size)
	{
		m_rect.right = size.x;
		m_rect.bottom = size.y;
		updateData();
	}
public:
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
		m_caption.setSize(m_textSize * m_size);
		m_stateBotton.setSize(m_size);
		updateData();
		updateChildSize();
	}

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a){updateChildColor();}	//do nothing
	void setAlpha(float a){updateChildAlpha();}							//do nothing

	void insertChar(const char *ch,int len){;}			//do nothing
	void setCaptionText(const char *caption)						//���ð�ť�ı��������
	{
		if(caption == NULL) return;
		m_caption.setString(caption);
	}
protected:
	void draw();
	void drawUp(){;}
	_XBool mouseProc(float x,float y,_XMouseState mouseState)
	{
	//	if(!m_isInited) return XTrue;	//���û�г�ʼ��ֱ���˳�
	//	if(!m_isVisiable) return XTrue;	//������ɼ�ֱ���˳�
	//	m_stateBotton.mouseProc(x,y,mouseState);
		return XTrue;
	}		//do nothing
	_XBool keyboardProc(int keyOrder,_XKeyState keyState)
	{
	//	if(!m_isInited) return XTrue;	//���û�г�ʼ��ֱ���˳�
	//	if(!m_isVisiable) return XTrue;	//������ɼ�ֱ���˳�
	//	m_stateBotton.keyboardProc(keyOrder,keyState);
		return XTrue;
	}		//do nothing
	_XBool canGetFocus(float x,float y){return XFalse;}	//do nothing	//�¼����Դ�͸���������XTrue������¼����ܴ�͸
	_XBool canLostFocus(float x,float y){return XTrue;}	//do nothing
public:
	void setLostFocus() {;}							//do nothing
	void disable(){m_isEnable = XFalse;}	
	void enable(){m_isEnable = XTrue;}
	void release();
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	_XBool setACopy(const _XGroup & temp);

	void setVisiable()
	{
		m_isVisiable = XTrue;
		m_stateBotton.setVisiable();
		if(m_state == STATE_NORMAL)
		{//��������������
			for(int i = 0;i < m_childList.size();++ i)
			{
				m_childList[i]->setVisiable();
			}
		}else
		{//��ʾ����������
			for(int i = 0;i < m_childList.size();++ i)
			{
				m_childList[i]->disVisiable();
			}
		}
		//updateChildVisiable();
	}
	void disVisiable()
	{
		m_isVisiable = XFalse;
		m_stateBotton.disVisiable();
		updateChildVisiable();
	}	//���ÿؼ����ɼ�

	_XGroup()
		:m_isInited(XFalse)
		,m_position(0.0f,0.0f)
		,m_size(1.0f,1.0f)
		,m_state(STATE_NORMAL)
		,m_funStateChange(NULL)
		,m_pClass(NULL)
	{
	}
	~_XGroup()
	{
		release();
	}
	virtual void justForTest() {;}
private://����Ϊ�˷�ֹ�������ظ�ֵ�����������ƹ��캯��
	_XGroup(const _XGroup &temp);
	_XGroup& operator = (const _XGroup& temp);
};

#endif