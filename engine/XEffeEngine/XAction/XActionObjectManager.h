#ifndef _JIA_ACTIONOBJECTMANAGER_
#define _JIA_ACTIONOBJECTMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
//��action�е���Դ���й������
#include "XBasicFun.h"
#include "XObjectBasic.h"

//enum _XActionObjectType
//{
//	ACTION_OBJ_TYPE_NULL,		//δ����
//	ACTION_OBJ_TYPE_SPRITE,		//����
//	ACTION_OBJ_TYPE_FRAMEEX,	//����֡Ex
//};
#define MAX_ACTION_OBJECT_DES_LEN (1024)
//action�����������
struct _XActionObjectDescription
{
	_XBool m_isEnable;			//�Ƿ���Ч
	int m_objectID;				//�����ID
	char *m_objectFilename;		//������ļ��� MAX_FILE_NAME_LENGTH
	_XObjectType m_objectType;
	void * m_paramData;			//����Ĳ������ݣ���ͬ������������ݲ�ͬ����������Ҫ���ⶨ��

	_XActionObjectDescription()
		:m_objectFilename(NULL)
		,m_objectType(OBJ_NULL)
		,m_paramData(NULL)
		,m_isEnable(XFalse)
	{
	}
	~_XActionObjectDescription()
	{
		release();
	}
	void release()
	{
		if(!m_isEnable) return;
		XDELETE_ARRAY(m_objectFilename);
		XDELETE(m_paramData);
		m_isEnable = XFalse;
	}
	char * getStrFromData();
	int getDataFromStr(const char * str);
	_XBool getDataFromFile(FILE *fp);
	//�������ü�����
	//��Դ�ͷ�
	//��

	_XObjectBasic * createAObject();	//�����������ɾ�������
};
#define MAX_ACTION_OBJECT_SUM (1024)	//�����������
class _XActionObjectManager
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	_XActionObjectManager()
		:m_objectSum(0)
	{
		m_pObject = createArrayMem<_XObjectBasic *>(MAX_ACTION_OBJECT_SUM);
		m_pObjDes = createArrayMem<_XActionObjectDescription *>(MAX_ACTION_OBJECT_SUM);
	}
	_XActionObjectManager(const _XActionObjectManager&);
	_XActionObjectManager &operator= (const _XActionObjectManager&);
	virtual ~_XActionObjectManager()
	{
		release();
	}
public:
	static _XActionObjectManager& GetInstance()
	{
		static _XActionObjectManager m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	_XObjectBasic ** m_pObject;	//�����ָ�����
	_XActionObjectDescription ** m_pObjDes;	//���������
	int m_objectSum;						//���������

	void release()
	{//��δʵ��
		for(int i = 0;i < m_objectSum;++ i)
		{
			XDELETE(m_pObject[i]);
			XDELETE(m_pObjDes[i]);
		}
		m_objectSum = 0;
		XDELETE_ARRAY(m_pObject);
		XDELETE_ARRAY(m_pObjDes);
	}
public:
	void releaseAllObject()
	{//�ͷŵ����е������Դ
		for(int i = 0;i < m_objectSum;++ i)
		{
			XDELETE(m_pObject[i]);
			XDELETE(m_pObjDes[i]);
		}
		m_objectSum = 0;
	}
	_XActionObjectDescription * getObjDes(const _XObjectBasic * pObj)
	{//�������ָ���ȡ�����������Ϣ
		if(pObj == NULL) return NULL;
		for(int i = 0;i < m_objectSum;++ i)
		{
			if(m_pObject[i] == pObj) return m_pObjDes[i];
		}
		return NULL;
	}
	int addAObject(_XObjectBasic * pObj,_XActionObjectDescription *pDes)
	{
		if(m_objectSum >= MAX_ACTION_OBJECT_SUM) return 0;
		if(pObj == NULL || pDes == NULL) return 0;
		if(getObjDes(pObj) != NULL) return 0;	//��ֹ�ظ�����
		m_pObject[m_objectSum] = pObj;
		m_pObjDes[m_objectSum] = pDes;
		++ m_objectSum;
		return 1;
	}
	_XBool decreaseAObject(_XObjectBasic * pObj)
	{
		if(pObj == NULL) return XFalse;
		for(int i = 0;i < m_objectSum;++ i)
		{
			if(m_pObject[i] == pObj)
			{
				XDELETE(m_pObject[i]);
				XDELETE(m_pObjDes[i]);
				for(int j = i;j < m_objectSum - 1;++ j)
				{
					m_pObject[j] = m_pObject[j + 1];
					m_pObjDes[j] = m_pObjDes[j + 1];
				}
				-- m_objectSum;
				return XTrue;
			}
		}
		return XFalse;
	}
};

#endif //_JIA_ACTIONOBJECTMANAGER_
