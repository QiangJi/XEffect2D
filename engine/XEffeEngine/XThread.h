#ifndef _JIA_XTHREAD_
#define _JIA_XTHREAD_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:        2011.4.9
//--------------------------------
//����һ��ͳһ���߳̽��й�����࣬Ŀ�����������е��߳��ܱ�ͳһ����
//��������δ���

#include "windows.h"
#include <vector>

//���̵߳�״̬
enum _XThreadState
{
	STATE_BEFORE_START,		//�߳���δ��ʼ
	STATE_START,			//�߳��Ѿ�����
	STATE_SET_TO_END,		//�̱߳�����Ϊ����
	STATE_END,				//�߳̽���
};
inline void waitThreadEnd(_XThreadState &state)
{
	if(state != STATE_START) return;
	state = STATE_SET_TO_END;
	while(state != STATE_END)
	{
		Sleep(1);
	}
}

typedef DWORD pthread_t;    

struct _XThreadInfo
{
	_XThreadState state;	//�̵߳�״̬
	HANDLE h;				//�̵߳ľ��
	void (* fun)(void *);	//�߳��е��õĺ���
	void *p;				//�̺߳����Ĳ���
	_XThreadInfo()
		:state(STATE_BEFORE_START)
		,h(0)
		,fun(NULL)
		,p(NULL)
	{}
};
//���ڶ��̵߳ĵ��û��ƣ�������������⣬��Ҫ����
//���̻߳��ƻ᲻ͣ�Ľ����̣߳���ʱ�����У����ᵼ��������buffԽ��Խ��
class _XThread
{
protected:
    _XThread()
	{}
    _XThread(const _XThread&);
	_XThread &operator= (const _XThread&);
    virtual ~_XThread(){release();}
public:
    static _XThread& GetInstance()
	{
		static _XThread m_instance;
		return m_instance;
	}
private:
	std::vector<_XThreadInfo *> m_threadBuff;	//�̵߳Ķ���
	void release();//�ͷ���Դ
public:
	//id,����̵߳ı�ţ�fun�̵߳��õĺ�����p�̴߳��ݵĲ���
	bool createAThread(int *id,void (* fun)(void *),void *p);			//����һ���߳�
	bool closeAThread(int id)			//�ر�һ���߳�
	{
		if(id < 0 || id >= m_threadBuff.size() || m_threadBuff[id]->state != STATE_START) return false;	
		if(TerminateThread(m_threadBuff[id]->h,0))
		{//�߳̽���
			m_threadBuff[id]->state = STATE_END;
			return true;
		}else
		{//�߳�
			return false;
		}
	}
	bool closeAllThread()			//�ر������߳�
	{
		return true;
	}
	_XThreadState getThreadState();	//��ȡһ���̵߳�״̬
	bool suspendAThread();			//����һ���߳�
	bool resumeAThread();			//�ָ�һ���߳�
	bool suspendAllThread();		//���������߳�
	bool resumeAllThread();			//�ָ������߳�

	void update();	//�����̵߳�״̬
};

#endif