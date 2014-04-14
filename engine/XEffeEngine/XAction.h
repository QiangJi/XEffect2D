#ifndef _JIA_XACTION_
#define _JIA_XACTION_
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.1
//--------------------------------
//Environment="PATH=%PATH%;..\dll\libfreetype;"	//��Ҫ����dll·��
#include "XBasicFun.h"
#include "stdlib.h"
#include "string.h"

enum _XActionState
{
	ACTION_STATE_NULL,		//����״̬δ����
	ACTION_STATE_ACTION,	//����������
	ACTION_STATE_PAUSE,	//������ͣ
	ACTION_STATE_END,		//��������
};

//ʹ�ûص���������ʽ��ʾ����
#define MAX_ACTION_NAME_LENGTH (256)
enum _XActionType
{
	ACTION_TYPE_COMPLEX,	//���и��ϵĶ���,�ɺܶ�ԭ�Ӷ������������̵Ķ���
	ACTION_TYPE_ATOMIC,		//ԭ�ӵĲ���(��)ϸ�ֵĶ���
};
class _XActionEx
{
private:
	_XResourcePosition m_resoursePosition;	//��Դ���ڵ�λ��
	char m_isInited;	//�Ƿ��ʼ��
	_XActionType m_type;
	_XActionState m_actionState;	//������״̬
	bool m_actionIsLoop;			//�����Ƿ�ѭ��
	char m_actionName[MAX_ACTION_NAME_LENGTH];				//���������ƣ����ڰ�������

	void (*m_funResetData)(void *);			//������������
	void (*m_funMove)(int stepTime,void *);	//����
	void (*m_funDraw)(void *);				//���
	void (*m_funAtEnd)(void *);				//��������֮�����
	bool (*m_funIsEnd)(void *);				//�ж��Ƿ�������
	//������Դ����
	int (*m_funInit)(int,void*);	//��Դ��ʼ��
	void (*m_funRelease)(void*);	//��Դ�ͷź���

	void * m_pClass;
public:
	virtual bool isName(const char *name)
	{
		if(name == NULL || m_actionName == NULL) return false;
		if(strcmp(name,m_actionName) == 0) return true;
		return false;
	}
	void setStop();	//��ֹ��������δʵ��
	virtual void setLoop(bool isLoop) {m_actionIsLoop = isLoop;}	//���ö����Ƿ�ѭ��
	virtual bool getIsLoop() {return m_actionIsLoop;}		//��ȡ�����Ƿ�ѭ��
	virtual void setStart();			//���ö�����ʼ����
	virtual void setPause()			//���ö�����ͣ
	{
		if(m_actionState == ACTION_STATE_ACTION)
			m_actionState = ACTION_STATE_PAUSE;
	}
	virtual void setResume()		//���ö����ָ�
	{
		if(m_actionState == ACTION_STATE_PAUSE)
			m_actionState = ACTION_STATE_ACTION;
	}
	virtual void move(int stepTime = 1);
	_XActionState getActionState()
	{
		return m_actionState;
	}
	bool getIsEnd()
	{
		if(m_actionState == ACTION_STATE_END) return true;
		else return false;
	}

	_XActionEx()
		:m_isInited(0)
		,m_actionState(ACTION_STATE_NULL)
		,m_actionIsLoop(false)
		,m_funResetData(NULL)		//������������
		,m_funMove(NULL)		//����
		,m_funDraw(NULL)		//���
		,m_funAtEnd(NULL)		//��������֮�����
		,m_funInit(NULL)		//��ʼ����Դ����
		,m_funRelease(NULL)		//��Դ�ͷ�
		,m_pClass(NULL)
		,m_actionSum(0)
		,m_pAction(NULL)
		,m_type(ACTION_TYPE_ATOMIC)
	{
		m_actionName[0] = '\0';
	}
	int init(_XResourcePosition resoursePosition,
		const char *actionName,
		bool (*funIsEnd)(void *),
		void (*funMove)(int,void *),		
		void (*funDraw)(void *),		
		void (*funResetData)(void *),
		void (*funAtEnd)(void *) = NULL,	
		int (*funInit)(int,void *) = NULL,
		void (*funRelease)(void *) = NULL,
		void * pClass = NULL);	
	void draw();

	~_XActionEx()
	{
		release();
	}
	void release()
	{
		if(m_isInited == 0) return;
		if(m_funRelease != NULL) (*m_funRelease)(m_pClass);
		if(m_type == ACTION_TYPE_COMPLEX) XDELETE_ARRAY(m_pAction);
		m_isInited = 0;
	}
//�����Ǹ��϶����Ķ���
private:
	_XActionEx **m_pAction;	//�Ӷ���
	int m_actionSum;		//�Ӷ�����������
	int m_nowActionIndex;
public:
	int init(_XResourcePosition resoursePosition,const char *actionName,int actionSum,...);	//���϶����ĳ�ʼ��
};

//�����������
typedef int _XActionHandle;	//�����ľ��
struct _XActionData
{
	_XActionHandle actionHandle;	//�����ı��
	char actionIsEnable;	//�����Ƿ���Ч
	_XActionEx *pAction;	//������ָ��

	_XActionData()
		:actionIsEnable(0)
		,pAction(NULL)
	{
	}
};
#define MAX_ACTION_SUM (512)	//�������ͬʱ���ڵĶ�������
class _XActionManager
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	_XActionManager()
		:m_nowActionHandle(0)
		,m_actionSum(0)
	{
		m_pActions = createArrayMem<_XActionData>(MAX_ACTION_SUM);
	}
	_XActionManager(const _XActionManager&);
	_XActionManager &operator= (const _XActionManager&);
	virtual ~_XActionManager(){;} 
public:
	static _XActionManager& GetInstance()
	{
		static _XActionManager m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	int m_nowActionHandle;		//��ǰ�Ķ������
	_XActionData *m_pActions;	//���ж����ı�
	int m_actionSum;	//��ǰӵ�еĶ���������
public:
	_XActionHandle pushAction(_XActionEx *action);	//���������һ�����������ض����ľ����-1��ʾ�����Ѿ�����

	_XActionHandle getActionHandle(const char *actionName);	//ͨ�����ֻ�ȡ�������
	_XActionHandle getActionHandle(const _XActionEx *action);	//ͨ�����ֻ�ȡ�������
	int popAction(const char *actionName);				//�ӱ���ɾ��һ������
	int popAction(_XActionEx *action);
	int popAction(_XActionHandle actionHandle);	
	int releaseAction(const char *actionName);	//�ӱ���ɾ��һ���������ͷŵ���Ӧ����Դ
	int releaseAction(_XActionEx *action);
	int releaseAction(_XActionHandle actionHandle);
	int setStartAction(const char *actionName);			//���ÿ�ʼ����һ������
	int setStartAction(_XActionHandle actionHandle);	//���ÿ�ʼ����һ������
	_XActionEx *getPAction(const char *actionName);
	_XActionEx *getPAction(_XActionHandle actionHandle);

	bool getIsActionEnd(const char *actionName);		//��ȡ��������Ƿ񲥷����
	bool getIsActionEnd(_XActionHandle actionHandle);	//��ȡ��������Ƿ񲥷����

	void move(int stepTime)		//�������ƽ����еĶ���
	{
		for(int i = 0;i < MAX_ACTION_SUM;++ i)
		{
			if(m_pActions[i].actionIsEnable != 0) m_pActions[i].pAction->move(stepTime);	//�������Ҫ˳��
		}
	}
	void draw()					//������������еĶ���
	{
		for(int i = 0;i < MAX_ACTION_SUM;++ i)
		{
			if(m_pActions[i].actionIsEnable != 0) m_pActions[i].pAction->draw();	//draw��˳�����⣬���������Ҫ����һ��ӳ��
		}
	}
	//����Ľӿ���δʵ��
	void setAllActionStop();	//ֹͣ���еĶ���
	void setAllActionPause();	//��ͣ���еĶ���
	void setAllActionResume();	//�ָ����еĶ���
	void popAllAction();		//�������еĶ���
};
#endif