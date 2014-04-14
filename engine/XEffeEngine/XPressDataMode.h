#ifndef _JIA_PRESSDATAMODE_
#define _JIA_PRESSDATAMODE_
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.10
//--------------------------------
//����һ��ѹ������ģ�ͣ����ǵ�ѹ��ʱ���ݿ�ʼ�仯�����ͷ�ʱ���ݿ�ʼ�ָ�
//���Է�Ϊ����ѹ��ģ�ͺ�˫��ѹ��ģ��
#include "XOSDefine.h"

enum _XPressState
{
//	PRESS_STATE_LEFT,	//����ѹ��	
//	PRESS_STATE_RIGHT,	//����ѹ��
	PRESS_STATE_PRESS,	//���ڵ���ѹ��ģ��
	PRESS_STATE_NORMAL,	//�ͷ�
};
class _XPressDataModeS	//����ѹ��ģ��
{
private:
	float m_maxDeformData;		//����������
	float m_normalData;			//��ԭ����
	float m_nowDeformData;		//��ǰ�ı�������
	float m_nowData;			//��ǰ������
	_XPressState m_pressState;	//ѹ��״̬
	//����ѹ���Ǳ����ָܻ���
	//_XBool m_canRecover;		//�Ƿ��Զ��ָ�
	int m_sensitivity;			//������	
	int m_nowPressTimer;		//��ǰ�ļ�ʱ��

	float m_startChangeData;	//���α仯������
	float m_minChangeData;	//��С�ı仯����
	float m_maxChangeData;	//���ı仯����
	//_XBool m_isRecover;		//�����Ƿ���ȫ�ָ�
	float m_dataFlag;		//���ݱ任�ķ���
public:
	void init(float normalValue,float maxDeformValue,int sensitivity);
	void setParam(float startChange,float minChange,float maxChange)
	{
		m_startChangeData = startChange;
		m_minChangeData = minChange;
		m_maxChangeData = maxChange;
	}
	void setPress();
	void setNormal(){m_pressState = PRESS_STATE_NORMAL;}
	void move(int stepTime);
	float getNowData() const {return m_nowData;}
	_XBool isPress() const 
	{
		if(m_pressState != PRESS_STATE_NORMAL) return XTrue;
		else return XFalse;
	}
	//�������������δ������Ȳ���
	void setNowData(float data)
	{//��Χ���
		if(m_normalData > m_maxDeformData)
		{
			if(data < m_maxDeformData) data = m_maxDeformData; 
			if(data > m_normalData) data = m_normalData; 
		}else
		{
			if(data > m_maxDeformData) data = m_maxDeformData; 
			if(data < m_normalData) data = m_normalData; 
		}
		m_nowData = data;
	}

};
class _XPressDataModeD	//˫��ѹ��ģ��
{
private:
	float m_minDeformData;		//��С��������
	float m_maxDeformData;		//����������
	float m_normalData;			//��ԭ����
	float m_nowDeformData;		//��ǰ�ı�������
	float m_nowData;			//��ǰ������
	_XPressState m_pressMinState;	//ѹ��״̬
	_XPressState m_pressMaxState;	//ѹ��״̬
	//����ѹ���Ǳ����ָܻ���
	char m_canRecover;		//�Ƿ��Զ��ָ�0:���ظ���1:�����ָ���2�����ָ�
	int m_sensitivity;			//������	
	int m_nowPressTimer;		//��ǰ�ļ�ʱ��

	float m_startChangeData;	//���α仯������
	float m_minChangeData;	//��С�ı仯����
	float m_maxChangeData;	//���ı仯����
	_XBool m_isRecover;		//�����Ƿ���ȫ�ָ�
public:
	void init(float normalValue,float maxDeformValue,float minDeformValue,int sensitivity,char canRecover = 1);
	void setParam(float startChange,float minChange,float maxChange)
	{
		m_startChangeData = startChange;
		m_minChangeData = minChange;
		m_maxChangeData = maxChange;
	}
	void setPressMax();
	void setPressMin();
	void setNormalMin(){m_pressMinState = PRESS_STATE_NORMAL;}
	void setNormalMax(){m_pressMaxState = PRESS_STATE_NORMAL;}
	void move(int stepTime);
	float getNowData() const {return m_nowData;}
	_XBool isPress() const 
	{
		if(m_pressMinState != PRESS_STATE_NORMAL || m_pressMaxState != PRESS_STATE_NORMAL) return XTrue;
		else return XFalse;
	}
	//�������������δ������Ȳ���
	void setNowData(float data)
	{
		if(data < m_minDeformData) data = m_minDeformData; 
		if(data > m_maxDeformData) data = m_maxDeformData; 
		m_nowData = data;
		m_isRecover = XFalse;
	}
};
#endif