#ifndef _JIA_XACTION_
#define _JIA_XACTION_

#include "stdlib.h"

enum _XACTION_STATE
{
	_XA_STATE_NULL,		//����״̬δ����
	_XA_STATE_ACTION,	//����������
	_XA_STATE_PAUSE,	//������ͣ
	_XA_STATE_END,		//��������
};

//����һ�����ڶ���������
class _XAction
{
private:
protected:
	_XACTION_STATE m_actionState;	//������״̬
	bool m_actionIsLoop;			//�����Ƿ�ѭ��
public:
	virtual void setLoop(bool loop)	//���ö����Ƿ�ѭ��
	{
		m_actionIsLoop = loop;	
	}
	virtual bool getIsLoop()		//��ȡ�����Ƿ�ѭ��
	{
		return m_actionIsLoop;
	}
	virtual void setStart()			//���ö�����ʼ����
	{
		if(m_actionState == _XA_STATE_END)
		{
			m_actionState = _XA_STATE_ACTION;
			resetData();
		}
	}
	virtual void setPause()			//���ö�����ͣ
	{
		if(m_actionState == _XA_STATE_ACTION)
			m_actionState = _XA_STATE_PAUSE;
	}
	virtual void setResume()		//���ö����ָ�
	{
		if(m_actionState == _XA_STATE_PAUSE)
			m_actionState = _XA_STATE_ACTION;
	}
	virtual void move(int timeStep);
	virtual void resetData() = 0;		//��ʼ������
	_XACTION_STATE getActionState()
	{
		return m_actionState;
	}
	bool getIsEnd()
	{
		if(m_actionState == _XA_STATE_END) return true;
		else return false;
	}

	_XAction()
		:m_actionState(_XA_STATE_NULL)
		,m_actionIsLoop(false)
	{
	}
};
//ʹ�ûص���������ʽ��ʾ����
class _XActionEx
{
private:
	char m_isInited;	//�Ƿ��ʼ��
	_XACTION_STATE m_actionState;	//������״̬
	bool m_actionIsLoop;			//�����Ƿ�ѭ��

	void (*m_funResetData)(void *);		//������������
	void (*m_funMove)(void *);		//����
	void (*m_funDraw)(void *);		//���
	void (*m_funAtEnd)(void *);		//��������֮�����
	bool (*m_funIsEnd)(void *);		//�ж��Ƿ�������

	void * m_pClass;
public:
	virtual void setLoop(bool loop)	//���ö����Ƿ�ѭ��
	{
		m_actionIsLoop = loop;	
	}
	virtual bool getIsLoop()		//��ȡ�����Ƿ�ѭ��
	{
		return m_actionIsLoop;
	}
	virtual void setStart()			//���ö�����ʼ����
	{
		if(m_actionState == _XA_STATE_END)
		{
			m_actionState = _XA_STATE_ACTION;
			if(m_funResetData != NULL) (*m_funResetData)(m_pClass);
		}
	}
	virtual void setPause()			//���ö�����ͣ
	{
		if(m_actionState == _XA_STATE_ACTION)
			m_actionState = _XA_STATE_PAUSE;
	}
	virtual void setResume()		//���ö����ָ�
	{
		if(m_actionState == _XA_STATE_PAUSE)
			m_actionState = _XA_STATE_ACTION;
	}
	virtual void move();
	_XACTION_STATE getActionState()
	{
		return m_actionState;
	}
	bool getIsEnd()
	{
		if(m_actionState == _XA_STATE_END) return true;
		else return false;
	}

	_XActionEx()
		:m_isInited(0)
		,m_actionState(_XA_STATE_NULL)
		,m_actionIsLoop(false)
		,m_funResetData(NULL)		//������������
		,m_funMove(NULL)		//����
		,m_funDraw(NULL)		//���
		,m_funAtEnd(NULL)		//��������֮�����
		,m_pClass(NULL)
	{
	}
	int init(bool (*funIsEnd)(void *),
		void (*funMove)(void *),		
		void (*funDraw)(void *),		
		void (*funResetData)(void *),
		void (*funAtEnd)(void *),	
		void * pClass);	
	void draw();
};
#endif