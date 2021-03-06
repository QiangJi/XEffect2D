#ifndef _JIA_XACTION_
#define _JIA_XACTION_
//++++++++++++++++++++++++++++++++
//Author:	  贾胜华(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.1
//--------------------------------
#include "XBasicFun.h"
#include "stdlib.h"
#include "string.h"

enum _XActionSimpleState
{
	ACTION_STATE_NULL,		//动作状态未定义
	ACTION_STATE_ACTION,	//动作进行中
	ACTION_STATE_PAUSE,	//动作暂停
	ACTION_STATE_END,		//动作结束
};

//使用回调函数的形式显示动作
#define MAX_ACTION_NAME_LENGTH (256)
enum _XActionSimpleType
{
	ACTION_TYPE_COMPLEX,	//串行复合的动作,由很多原子动作串起来过程的动作
	ACTION_TYPE_ATOMIC,		//原子的不可(必)细分的动作
};
class _XActionSimpleEx
{
private:
	_XResourcePosition m_resoursePosition;	//资源所在的位置
	char m_isInited;	//是否初始化
	_XActionSimpleType m_type;
	_XActionSimpleState m_actionState;	//动作的状态
	_XBool m_actionIsLoop;			//动作是否循环
	char m_actionName[MAX_ACTION_NAME_LENGTH];				//动作的名称，用于帮助记忆

	void (*m_funResetData)(void *);			//重新设置数据
	void (*m_funMove)(int stepTime,void *);	//步进
	void (*m_funDraw)(void *);				//描绘
	void (*m_funAtEnd)(void *);				//动作结束之后调用
	_XBool (*m_funIsEnd)(void *);				//判断是否动作结束
	//进行资源管理
	int (*m_funInit)(int,void*);	//资源初始化
	void (*m_funRelease)(void*);	//资源释放函数

	void * m_pClass;
public:
	virtual _XBool isName(const char *name)
	{
		if(name == NULL || m_actionName == NULL) return XFalse;
		if(strcmp(name,m_actionName) == 0) return XTrue;
		return XFalse;
	}
	void setStop();	//终止动作，尚未实现
	virtual void setLoop(_XBool isLoop) {m_actionIsLoop = isLoop;}	//设置动作是否循环
	virtual _XBool getIsLoop() {return m_actionIsLoop;}		//获取动作是否循环
	virtual void setStart();			//设置动作开始进行
	virtual void setPause()			//设置动作暂停
	{
		if(m_actionState == ACTION_STATE_ACTION)
			m_actionState = ACTION_STATE_PAUSE;
	}
	virtual void setResume()		//设置动作恢复
	{
		if(m_actionState == ACTION_STATE_PAUSE)
			m_actionState = ACTION_STATE_ACTION;
	}
	virtual void move(int stepTime = 1);
	_XActionSimpleState getActionState()
	{
		return m_actionState;
	}
	_XBool getIsEnd()
	{
		if(m_actionState == ACTION_STATE_END) return XTrue;
		else return XFalse;
	}

	_XActionSimpleEx()
		:m_isInited(0)
		,m_actionState(ACTION_STATE_NULL)
		,m_actionIsLoop(XFalse)
		,m_funResetData(NULL)		//重新设置数据
		,m_funMove(NULL)		//步进
		,m_funDraw(NULL)		//描绘
		,m_funAtEnd(NULL)		//动作结束之后调用
		,m_funInit(NULL)		//初始化资源加载
		,m_funRelease(NULL)		//资源释放
		,m_pClass(NULL)
		,m_actionSum(0)
		,m_pAction(NULL)
		,m_type(ACTION_TYPE_ATOMIC)
	{
		m_actionName[0] = '\0';
	}
	int init(_XResourcePosition resoursePosition,
		const char *actionName,
		_XBool (*funIsEnd)(void *),
		void (*funMove)(int,void *),		
		void (*funDraw)(void *),		
		void (*funResetData)(void *),
		void (*funAtEnd)(void *) = NULL,	
		int (*funInit)(int,void *) = NULL,
		void (*funRelease)(void *) = NULL,
		void * pClass = NULL);	
	void draw();

	~_XActionSimpleEx()
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
//下面是复合动作的定义
private:
	_XActionSimpleEx **m_pAction;	//子动作
	int m_actionSum;		//子动作的数据量
	int m_nowActionIndex;
public:
	int init(_XResourcePosition resoursePosition,const char *actionName,int actionSum,...);	//复合动作的初始化
};

//动作管理的类
typedef int _XActionSimpleHandle;	//动作的句柄
struct _XActionSimpleData
{
	_XActionSimpleHandle actionHandle;	//动作的编号
	char actionIsEnable;	//动作是否有效
	_XActionSimpleEx *pAction;	//动作的指针

	_XActionSimpleData()
		:actionIsEnable(0)
		,pAction(NULL)
	{
	}
};
#define MAX_ACTION_SUM (512)	//最多允许同时存在的动作数量
class _XActionSimpleManager
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//下面需要将其设计为Singleton模式
protected:
	_XActionSimpleManager()
		:m_nowActionHandle(0)
		,m_actionSum(0)
	{
		m_pActions = createArrayMem<_XActionSimpleData>(MAX_ACTION_SUM);
	}
	_XActionSimpleManager(const _XActionSimpleManager&);
	_XActionSimpleManager &operator= (const _XActionSimpleManager&);
	virtual ~_XActionSimpleManager(){;} 
public:
	static _XActionSimpleManager& GetInstance()
	{
		static _XActionSimpleManager m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	int m_nowActionHandle;		//当前的动作句柄
	_XActionSimpleData *m_pActions;	//所有动作的表
	int m_actionSum;	//当前拥有的动作的数量
public:
	_XActionSimpleHandle pushAction(_XActionSimpleEx *action);	//向表中推入一个动作，返回动作的句柄，-1表示动作已经存在

	_XActionSimpleHandle getActionHandle(const char *actionName);	//通过名字获取动作句柄
	_XActionSimpleHandle getActionHandle(const _XActionSimpleEx *action);	//通过名字获取动作句柄
	int popAction(const char *actionName);				//从表中删除一个动作
	int popAction(_XActionSimpleEx *action);
	int popAction(_XActionSimpleHandle actionHandle);	
	int releaseAction(const char *actionName);	//从表中删除一个动作并释放掉相应的资源
	int releaseAction(_XActionSimpleEx *action);
	int releaseAction(_XActionSimpleHandle actionHandle);
	int setStartAction(const char *actionName);			//设置开始播放一个动作
	int setStartAction(_XActionSimpleHandle actionHandle);	//设置开始播放一个动作
	_XActionSimpleEx *getPAction(const char *actionName);
	_XActionSimpleEx *getPAction(_XActionSimpleHandle actionHandle);

	_XBool getIsActionEnd(const char *actionName);		//获取这个动作是否播放完成
	_XBool getIsActionEnd(_XActionSimpleHandle actionHandle);	//获取这个动作是否播放完成

	void move(int stepTime)		//遍历并推进所有的动作
	{
		for(int i = 0;i < MAX_ACTION_SUM;++ i)
		{
			if(m_pActions[i].actionIsEnable != 0) m_pActions[i].pAction->move(stepTime);	//这个不需要顺序
		}
	}
	void draw()					//遍历并描绘所有的动作
	{
		for(int i = 0;i < MAX_ACTION_SUM;++ i)
		{
			if(m_pActions[i].actionIsEnable != 0) m_pActions[i].pAction->draw();	//draw的顺序问题，这里估计需要进行一个映射
		}
	}
	//下面的接口尚未实现
	void setAllActionStop();	//停止所有的动作
	void setAllActionPause();	//暂停所有的动作
	void setAllActionResume();	//恢复所有的动作
	void popAllAction();		//弹出所有的动作
};

#endif