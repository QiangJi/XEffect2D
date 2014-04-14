#ifndef _JIA_XACTIONMANAGER_
#define _JIA_XACTIONMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XActionDescription.h"
#include "../XMedia/XDirectory.h"
#include <map>
//��ָ��Ŀ¼�µ�����AD�ļ�ȫ�����룬������AD���Ʊ���
//����Դ�ļ��ж�ȡ���ݻ�������⣬�����޸�
class _XActionMananger
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	_XActionMananger()
		:m_isInited(XFalse)
	{
	}
	_XActionMananger(const _XActionMananger&);
	_XActionMananger &operator= (const _XActionMananger&);
	virtual ~_XActionMananger() {release();}
public:
	static _XActionMananger& GetInstance()
	{
		static _XActionMananger m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	_XBool m_isInited;	//�Ƿ��Ѿ�����
	std::map<int,_XActionDescription *> m_actionDescriptions;	//���еĶ���������
	std::map<string,int> m_actionDesMap;
	std::vector<int> m_actionIDList;							//AD��ID�б�

	_XBool isActionDescriptionFile(const char * filename);
	//���ļ����ж�ȡ���������ļ�
	void loadActionDesFromFold(const _XDir *dir);
public:
	_XBool loadAllActionDes(const char * path = NULL);	//��ָ��Ŀ¼�µĶ���ȫ������ //�ڲ���Դ��ȡ���������
	_XBool saveAllActionDes();	//�����ж������浽Ĭ��Ŀ¼��

	_XBool pushAActionDescription(_XActionDescription * tempAD)
	{//���һ��AD������
		if(tempAD == NULL) return XFalse;
		if(getActionDes(tempAD->getName()) != NULL) return XFalse;	//ͬ��
		if(getActionDes(tempAD->getID()) != NULL) return XFalse;		//ͬID
		//����һ��
		m_actionDescriptions.insert(pair<int,_XActionDescription *>(tempAD->getID(),tempAD));
		m_actionDesMap.insert(pair<string,int>(tempAD->getName(),tempAD->getID()));
		m_actionIDList.push_back(tempAD->getID());
		return XTrue;
	}
	_XActionDescription *getActionDes(int ID)		//����ID����
	{
		if(m_actionDescriptions.count(ID) == 0) return NULL;
		return m_actionDescriptions[ID];
	}
	_XActionDescription *getActionDes(const char * name)	//�������ֵ���
	{
		if(name == NULL) return NULL;
		if(m_actionDesMap.count(name) == 0) return NULL;
		int id = m_actionDesMap[name];
		return getActionDes(id);
	}
	_XActionDescription *getActionDesByIndex(int index)	//��Ҫ��Ϊ�˱���
	{
		int sum = m_actionIDList.size();
		if(index < 0 || index >= sum) return NULL;
		return getActionDes(m_actionIDList[index]);
	}
	_XBool getIsReferenced(_XActionDescription * AD);
	int getADSum()
	{
		if(!m_isInited) return 0;
		return m_actionDesMap.size();
	}
//++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����Ƕ�AD�Ŀ��ƽӿ�(��δʵ��)
	//void play(int ID);		//���Ŷ���
	//void stop(int ID);		//ֹͣ��������
	//void pause(int ID);		//��ͣ����
	//void setShow(int ID);	//��ʾ����
	//void disShow(int ID);	//���ض���
	//_XBool getIsEnd(int ID);	//�ж϶����Ƿ񲥷����

	//void play(const char * name);
	////....����Ķ�Ӧ�ӿ�
	//void draw();
	//void move(int stepTime);
//--------------------------------------------------

	void release();	//��Դ�ͷ�	
	void deleteAActionDesNoReference(_XActionDescription * AD);
	_XBool setADName(_XActionDescription * AD,const char *name);
};
#endif