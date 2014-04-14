#ifndef _JIA_XACTIONDESCRIPTION_
#define _JIA_XACTIONDESCRIPTION_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "../XObjectBasic.h"
#include "../XNumberEx.h"
//#include "../XBasicFun.h"
#include <vector>
#include "XActionObjectManager.h"
#define ACTION_DES_PATH ("ResourcePack/ActionDescription/")
enum _XActionType
{//����������
	ACTION_TYPE_POSITION,	//λ�õı仯
	ACTION_TYPE_POSITION_X,	//λ�õı仯
	ACTION_TYPE_POSITION_Y,	//λ�õı仯
	ACTION_TYPE_COLOR,		//��ɫ�ı仯
	ACTION_TYPE_R,
	ACTION_TYPE_G,
	ACTION_TYPE_B,
	ACTION_TYPE_ALPHA,		//͸���ȵı仯
	ACTION_TYPE_ANGLE,		//�Ƕȱ仯
	ACTION_TYPE_SIZE,		//�ߴ�ı仯
	ACTION_TYPE_SIZE_EX,	//�ߴ�ı仯	//x��y�ȱ�������
	ACTION_TYPE_SIZE_X,		//�ߴ�ı仯	
	ACTION_TYPE_SIZE_Y,		//�ߴ�ı仯
	ACTION_TYPE_STAY,		//ͣ��״̬
	ACTION_TYPE_MAX,
};
extern string actionTypeStr[];
struct _XActionState
{//������ֵ�״̬
	_XBool isEnable;
	_XVector2 position;
	_XFColor color;
	float angle;	//[0-360]
	_XVector2 size;
	_XActionState()
		:angle(0.0f)
		,isEnable(XFalse)
	{
	}
};
//״̬����
enum _XActionStateType
{
	ACTION_STATE_TYPE_POS_X,
	ACTION_STATE_TYPE_POS_Y,
	ACTION_STATE_TYPE_COLOR_R,
	ACTION_STATE_TYPE_COLOR_G,
	ACTION_STATE_TYPE_COLOR_B,
	ACTION_STATE_TYPE_COLOR_A,
	ACTION_STATE_TYPE_ANGLE,
	ACTION_STATE_TYPE_SIZE_X,
	ACTION_STATE_TYPE_SIZE_Y,
	ACTION_STATE_TYPE_MAX,
};
struct _XActionStateDescription
{
	int time;	//״̬������ʱ��㣬���ֻ��AD_TYPE_STATE_LINEʱ��Ч
	_XBool isValidPosX;
	_XBool isValidPosY;
	_XBool isValidColorR;
	_XBool isValidColorG;
	_XBool isValidColorB;
	_XBool isValidColorA;
	_XBool isValidAngle;
	_XBool isValidSizeX;
	_XBool isValidSizeY;

	_XVector2 position;
	_XFColor color;
	float angle;	//[0-360]
	_XVector2 size;
	_XActionStateDescription()
		:angle(0.0f)
	{}
};
typedef _XActionStateDescription _XActionStateDes;
//����һ���Զ�����������
class _XActionCore
{
public:
	_XBool m_isEnable;
	_XActionType m_actionType;
	//std::vector<_XMoveData *> m_moveData;
	_XMoveData *m_moveData;
	int m_moveDataSum;
	_XObjectBasic* m_object;	//�����Ķ���
	float m_startTime;				//������ʼʱ��,����������ʱ��
	float m_endTime;
	float m_timeLine;				//ʱ����
	_XActionState m_stayState;		//��Ҫ���ֵ�״̬
	//_XBool m_isStart;			//�Ƿ�ʼ
	_XBool m_isActionStart;	//������ʼ�����ݿ�ʼ�仯
	_XBool m_isEnd;
	_XBool set(float startTime,float endTime,_XActionType type,_XMoveData *md = NULL,_XActionState *state = NULL);
	void move(int delay);
	_XBool getIsEnd() const {return m_isEnd;}
	void setEnd(){m_isEnd = XTrue;}		//���ý���
	//_XBool getIsLoop();
	void setStart();	//���ÿ�ʼ
	_XActionCore()
		:m_isEnable(XFalse)
		,m_isEnd(XTrue)
		,m_startTime(0.0f)
		,m_timeLine(0.0f)
		//,m_isStart(XFalse)
		,m_moveData(NULL)
		,m_moveDataSum(0)
	{
	}
	~_XActionCore()
	{
		release();
	}
	void release();
};
//AD�жԶ�������������
//enum _XActionDesType
//{
//	AD_TYPE_AC,				//ͨ��ActionCore�ķ�ʽ��������
//	AD_TYPE_STATE_TABLE,	//ͨ��״̬��ķ�ʽ��������
//};
class _XActionMananger;
class _XActionDescription
{
	friend _XActionMananger;
private:
	int m_ID;
	_XBool m_isEnable;
	std::vector<_XActionDescription *> m_otherAction;	//��������ɲ���
	std::vector<int> m_otherActionID;	//������ڸ�����ʼ��
	_XObjectBasic* m_object;			//�����Ķ���
	std::vector<_XActionCore *> m_actionCore;	//����Ԫ
	char m_actionName[MAX_FILE_NAME_LENGTH];			//����������
	float m_startTime;				//������ʼʱ��
	float m_endTime;				//��������ʱ��(ò��û�б�Ҫ),����û����
	float m_timeLine;
	_XBool m_isEnd;
	//_XBool m_isStart;	//�Ƿ�ʼ
	_XBool m_isActionStart;	//������ʼ�����ݿ�ʼ�仯
	//_XActionDesType m_actionDesType;
	//+++++++++++++++++++++++++++++++++++++++++++++
	//�����Ƕ�AD_TYPE_STATE_LINEģʽ����������δʵ�֣�
	//˵�����ģʽ�£�AD�д��state���б�ʱ��֡��⵱ǰ����������state֮�䣬
	//Ȼ��ͨ�����Բ�ֵ(����չΪ��������ֵ)�ķ�ʽ���㵱ǰ��״̬
private:
	std::vector<_XActionStateDes *> m_stateTable;	//״̬�б�,��ʱ������
	int m_minTime;
	int m_maxTime;
	float getASValue(int time,_XActionStateType type);
public:
	int getMinTime() {return m_minTime;}	//�����С��ʱ���
	int getMaxTime() {return m_maxTime;}	//�������ʱ���
	int getActionStateDesSum() {return m_stateTable.size();}
	_XActionStateDes* getPreviousASD(int time,_XActionStateType type);	//��õ�ǰʱ������һ��״̬����
	_XActionStateDes* getNextASD(int time,_XActionStateType type);		//��õ�ǰʱ������һ��״̬����
	_XActionStateDes* getASD(int index)
	{
		if(index < 0 || index >= m_stateTable.size()) return NULL;
		return m_stateTable[index];
	}
	_XActionState getActionState(int time);		//��ȡָ��ʱ���״̬(ע������㷨��Ч���д��Ż�)
	_XBool insertASD(_XActionStateDes* ASD);		//����һ��״̬����(��ʱ������)
	_XBool popASD(const _XActionStateDes* ASD);	//����һ��״̬����,���ͷ���Դ
	//_XBool checkStateTable();						//���״̬�б�ĺ�����,ȱ���������յ������������յ�Ĵ���
	//---------------------------------------------
public:
	float getEndTime() const {return m_endTime;}
	float getStartTime() const {return m_startTime;}
	float getNowTime() const {return m_timeLine;}
	_XObjectBasic * getObject() {return m_object;}
	int getOtherActionSum(){return m_otherAction.size();}
	_XActionDescription *getOtherAD(int index)
	{
		if(index < 0 || index >= m_otherAction.size()) return NULL;
		return m_otherAction[index];
	}
	int getActionCoreSum(){return m_actionCore.size();}
	_XActionCore * getActionCore(int index)
	{
		if(!m_isEnable) return NULL;
		if(index < 0 || index >= m_actionCore.size()) return NULL;
		return m_actionCore[index];
	}

public:
	int getID() const{return m_ID;}
	const char * getName() const{return m_actionName;}
	_XBool setName(const char * name)
	{//(��δʵ��)
		if(name == NULL) return XFalse;
		if(strlen(name) >= MAX_FILE_NAME_LENGTH) return XFalse;
		if(strcmp(name,m_actionName) == 0) return XTrue;	//������ͬ�����ظ�����
		//if(_XActionMananger::GetInstance().getActionDes(name) != NULL) return XFalse; //����
		//������Ҫ�������(��δʵ��)
		strcpy(m_actionName,name);
		return XTrue;
	}
	void move(int delay);
	void draw();
	void setStart();
	_XBool getIsEnd() const {return m_isEnd;}	//�Ƿ����
	void setLoop();							//��δ���
	_XBool set(float startTime,float endTime,_XObjectBasic *obj)
	{
		if(m_isEnable) return XFalse;
		m_startTime = startTime;
		m_endTime = endTime;
		m_object = obj;
		m_otherAction.clear();
		m_actionCore.clear();

		m_isEnable = XTrue;
		return XTrue;
	}
	int getADChildIndex(_XActionDescription * p);	//����ƶ���AD�ڵ�ǰAD���б��еı��
	void setObject(_XObjectBasic *obj);
	_XBool loadAction(const char * filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//���涯�����ļ�,��׺".acd"
	_XBool saveAction(const char *filename = NULL);	//���ļ���ȡ����,��׺".acd"
	_XBool pushAActionCore(_XActionCore * p);
	_XBool popAActionCore(_XActionCore * p);	//��������ɾ��һ��Ԫ��
	_XBool replaceAAction(int index,_XActionCore * p);
	_XBool popAActionCore(int index);
	_XBool getIsChildActionDes(_XActionDescription * p);
	_XBool pushAActionDescription(_XActionDescription * p);
	_XBool popAActionDescription(int index);
	_XActionDescription();
	~_XActionDescription()
	{
		release();
	}
	void release();
};

#endif