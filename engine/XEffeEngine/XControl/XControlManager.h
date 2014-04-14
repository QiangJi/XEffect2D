#ifndef _JIA_XCONTROLMANAGER_
#define _JIA_XCONTROLMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.10
//--------------------------------
#include "stdlib.h"
#include "XControlBasic.h"

#define MAX_CTRL_OBJ_SUM (512)	//�ؼ�������������ע����ܿؼ�����

enum _XCtrlObjType
{
	CTRL_OBJ_NULL,		//��Ч�����
	CTRL_OBJ_BUTTON,	//��ť���
	CTRL_OBJ_BUTTONEX,	//��ť���
	CTRL_OBJ_CHECK,		//��ѡ������
	CTRL_OBJ_EDIT,		//���������
	CTRL_OBJ_MOUSERIGHTBUTTONMENU,	//����Ҽ��˵������
	CTRL_OBJ_SLIDER,	//�����������
	CTRL_OBJ_RADIOS,	//��ѡ������
	CTRL_OBJ_PROGRESS,	//�����������
	CTRL_OBJ_MUTITEXT,	//�����ı������
	CTRL_OBJ_MUTILIST,	//�����б��
	CTRL_OBJ_COMBO,		//�����б������
	CTRL_OBJ_DIRECTORYLIST,	//·���б��
	CTRL_OBJ_GROUP,		//Ⱥ���
	CTRL_OBJ_POINTCTRL,		//��ԭ
	CTRL_OBJ_LINECTRL,		//��ԭ
	CTRL_OBJ_SLIDEREX,	//�����������
	CTRL_OBJ_FUNCTION,	//��ͼ����
};
struct _XCtrlObjetP
{
	void *pObject;
	void (*pFunction)(void);
};
//�������Ҫʹ�õ�����ϵͳ
class _XControlManager
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//���ֵ���ģʽ����Ʒ�ʽ��֮ǰ��Ҫ��
protected:
	_XControlManager();
	_XControlManager(const _XControlManager&);
	_XControlManager &operator= (const _XControlManager&);
	virtual ~_XControlManager(); 
public:
	static _XControlManager& GetInstance();
	//-------------------------------------------
private:
	_XCtrlObjetP m_pObject[MAX_CTRL_OBJ_SUM];				//���ע�������
	_XCtrlObjType m_objectType[MAX_CTRL_OBJ_SUM];	//���������
	int m_objectSum;	//���������
	int m_objectDrawOrderMap[MAX_CTRL_OBJ_SUM];		//������˳���ӳ���
	char m_isAutoDraw;	//�Ƿ��Լ�����ť�Ļ���
	int m_focusOrder;	//��ǰ��ý���Ŀؼ��ı�ţ�ֻ�л�ý���Ŀؼ����ܶ��������¼�������Ӧ

public:
	int getObjSum() {return m_objectSum;}
	int addAObject(void * object,_XCtrlObjType type);			//ע��һ�����,����ע���ID��ʵ���������б�ţ�-1Ϊע��ʧ��
	int addAObject(void (* function)());			//ע��һ�����,����ע���ID��ʵ���������б�ţ�-1Ϊע��ʧ��
	void decreaseAObject(int objectID);							//ȥ��һ�������ע��
	void decreaseAObject(const void * object);					//ȥ��һ�������ע��
	void decreaseAObject(void (* function)());					//ȥ��һ�������ע��
	int findObjectID(const void * object) const;		//��ȡָ�������ID -1��ʾ�����û��ע��
	int findObjectID(void (* function)()) const;		//��ȡָ�������ID -1��ʾ�����û��ע��
	_XCtrlObjType getObjectType(int objectID) const;	//��ȡ���������
	void *getObject(int objectID) const;				//��ȡ�����ָ��
	//Ϊ�˴ﵽ��ͼ˳���޸��Լ���Ϣ����˳���޸ĵ�Ŀ�ģ������ṩһ�麯�����ڵ����ؼ���ע��˳����δ�������ԣ�
	void moveAndInsert(int objectID,int insertObjectID);	//�����objectID���뵽insertID��λ�ã���������֮������ȫ��˳���ƶ�
	void changeTowObject(int object1,int object2);				//��object1��object2����������Ե�
	void setIsAutoDraw(_XBool isAutoDraw)
	{
		if(isAutoDraw) m_isAutoDraw = 1;
		else m_isAutoDraw = 0;
	}
public:
	void mouseProc(int x,int y,_XMouseState mouseState);		//����¼��ĺ���
	void keyProc(int keyOrder,_XKeyState keyState);			//�����¼��ĺ���
	void insertChar(const char *ch,int len);
	void draw();
	void update(int stepTime);

	void draw(const void * obj);	//�ֶ����ĳ������
};

inline void _XControlManager::decreaseAObject(const void * object)
{
	decreaseAObject(findObjectID(object));
}

inline void _XControlManager::decreaseAObject(void (* function)())
{
	decreaseAObject(findObjectID(function));
}

inline _XCtrlObjType _XControlManager::getObjectType(int objectID) const
{
	if(objectID < 0 || objectID >= m_objectSum) return CTRL_OBJ_NULL;	//��������Ƿ�
	return m_objectType[objectID];
}

inline void *_XControlManager::getObject(int objectID) const
{
	if(objectID < 0 || objectID >= m_objectSum) return NULL;	//��������Ƿ�
	return m_pObject[objectID].pObject;
}

inline void _XControlManager::changeTowObject(int object1,int object2)				//��object1��object2����������Ե�
{
	if(object1 < 0 || object1 >= m_objectSum) return;	//���ݷǷ�
	if(object2 < 0 || object2 >= m_objectSum) return;	//���ݷǷ�
	if(object1 == object2) return;	//����Ĳ���
	xChangeTwoSum(m_objectDrawOrderMap[object1],m_objectDrawOrderMap[object2]);
}

#endif