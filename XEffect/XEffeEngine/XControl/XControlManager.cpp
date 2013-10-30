#include "XControlManager.h"
#include "XButton.h"
#include "XCheck.h"
#include "XEdit.h"
#include "XMouseRightButtonMenu.h"
#include "XSlider.h"
#include "XRadios.h"
#include "XMultiText.h"
#include "XMultiList.h"
#include "XCombo.h"

_XControlManager::_XControlManager()
:m_objectSum(0)
,m_isAutoDraw(1)
,m_focusOrder(-1)
{
	for(int i = 0;i < MAX_CTRL_OBJ_SUM;++ i)
	{
		m_pObject[i].pObject = NULL;
		m_pObject[i].pFunction = NULL;
		m_objectType[i] = CTRL_OBJ_NULL;
		m_objectDrawOrderMap[i] = 0;
	}
}

_XControlManager::~_XControlManager()
{
}

_XControlManager& _XControlManager::GetInstance()
{
	static _XControlManager m_instance;
	return m_instance;
}

int _XControlManager::addAObject(void * object,_XCtrlObjType type)
{
	if(m_objectSum >= MAX_CTRL_OBJ_SUM) return 0;		//����������Ѿ�����������
	if(findObjectID(object) >= 0) return 0;				//��ֹ����ظ�ע��
	m_pObject[m_objectSum].pObject = object;
	m_objectType[m_objectSum] = type;
	m_objectDrawOrderMap[m_objectSum] = m_objectSum;
	++ m_objectSum;
//	printf("add one ctrl obj:%d\n",m_objectSum - 1);
	return 1;
}

int _XControlManager::addAObject(void (* function)())			//ע��һ�����,����ע���ID��ʵ���������б�ţ�-1Ϊע��ʧ��
{
	if(m_objectSum >= MAX_CTRL_OBJ_SUM) return 0;		//����������Ѿ�����������
	if(findObjectID(function) >= 0) return 0;				//��ֹ����ظ�ע��
	m_pObject[m_objectSum].pFunction = function;
	m_objectType[m_objectSum] = CTRL_OBJ_FUNCTION;
	m_objectDrawOrderMap[m_objectSum] = m_objectSum;
	++ m_objectSum;
//	printf("add one ctrl obj:%d\n",m_objectSum - 1);
	return 1;
}

void _XControlManager::decreaseAObject(int objectID)
{
	if(objectID < 0 || objectID >= m_objectSum) return;	//��������Ƿ�
	if(objectID == m_objectSum - 1)
	{//ɾ���������һ��Ԫ��
		m_pObject[objectID].pFunction = NULL;
		m_pObject[objectID].pObject = NULL;
		m_objectType[objectID] = CTRL_OBJ_NULL;
		for(int i = 0;i < m_objectSum;++ i)
		{
			if(m_objectDrawOrderMap[i] > m_objectDrawOrderMap[objectID]) --m_objectDrawOrderMap[i];
		}
		m_objectDrawOrderMap[objectID] = 0;
	}else
	{
		for(int i = 0;i < m_objectSum;++ i)
		{
			if(m_objectDrawOrderMap[i] > m_objectDrawOrderMap[objectID]) --m_objectDrawOrderMap[i];
		}
		m_objectDrawOrderMap[objectID] = 0;
		for(int i = objectID;i < m_objectSum - 1;++ i)
		{
			m_pObject[i].pFunction = m_pObject[i + 1].pFunction;
			m_pObject[i].pObject = m_pObject[i + 1].pObject;
			m_objectType[i] = m_objectType[i + 1];
			m_objectDrawOrderMap[i] = m_objectDrawOrderMap[i + 1];
		}
		m_pObject[m_objectSum - 1].pFunction = NULL;
		m_pObject[m_objectSum - 1].pObject = NULL;
		m_objectType[m_objectSum - 1] = CTRL_OBJ_NULL;
		m_objectDrawOrderMap[m_objectSum - 1] = 0;
	}
	-- m_objectSum;
	if(m_focusOrder == objectID) m_focusOrder = -1;	//���������ɾ���Ļ���������Ҫ���ý���
//	printf("add one ctrl obj:%d\n",m_objectSum - 1);
}

int _XControlManager::findObjectID(const void * object) const
{
	for(int i = 0;i < m_objectSum;++ i)
	{
		if(m_pObject[i].pObject == object) return i;
	}
	return -1;
}

int _XControlManager::findObjectID(void (* function)()) const
{
	for(int i = 0;i < m_objectSum;++ i)
	{
		if(m_pObject[i].pFunction == function) return i;
	}
	return -1;
}

void _XControlManager::mouseProc(int x,int y,_XMouseState mouseState)
{
	if(m_focusOrder < 0 || m_focusOrder >= m_objectSum)
	{//��ǰû�н���ؼ�
		for(int i = 0;i < m_objectSum;++ i)
		{
			if(m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_FUNCTION && m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_NULL) 
			{
				if(((_XControlBasic *)(m_pObject[m_objectDrawOrderMap[i]].pObject))->canGetFocus(x,y) != 0) 
				{
					m_focusOrder = i;
					break;
				}
			}
		}
	}else
	{
		if(((_XControlBasic *)(m_pObject[m_objectDrawOrderMap[m_focusOrder]].pObject))->canLostFocus(x,y) != 0)
		{//�����ǰ�������ʧȥ���㣬��������Դ�������ִ����
			int tempFocus = -1;
			for(int i = 0;i < m_objectSum;++ i)
			{
				if(m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_FUNCTION && m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_NULL) 
				{
					if(((_XControlBasic *)(m_pObject[m_objectDrawOrderMap[i]].pObject))->canGetFocus(x,y) != 0) 
					{
						tempFocus = i;
						break;
					}
				}
			}
			if(tempFocus >= 0 && tempFocus != m_focusOrder) 
			{
				if(m_objectType[m_objectDrawOrderMap[m_focusOrder]] != CTRL_OBJ_FUNCTION && m_objectType[m_objectDrawOrderMap[m_focusOrder]] != CTRL_OBJ_NULL) 
				{
					((_XControlBasic *)(m_pObject[m_objectDrawOrderMap[m_focusOrder]].pObject))->setLostFocus();
				}
				m_focusOrder = tempFocus;//�л�����
			}
		}
		if(m_objectType[m_objectDrawOrderMap[m_focusOrder]] != CTRL_OBJ_FUNCTION && m_objectType[m_objectDrawOrderMap[m_focusOrder]] != CTRL_OBJ_NULL) 
		{
			((_XControlBasic *)(m_pObject[m_objectDrawOrderMap[m_focusOrder]].pObject))->mouseProc(x,y,mouseState);
		}
	}
	//for(int i = 0;i < m_objectSum;++ i)
	//{
	//	if(m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_FUNCTION && m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_NULL) 
	//	{
	//		((_XControlBasic *)(m_pObject[m_objectDrawOrderMap[i]].pObject))->mouseProc(x,y,mouseState);
	//	//	if(((_XControlBasic *)(m_pObject[m_objectDrawOrderMap[i]].pObject))->isInRect(x,y) != 0) break;	//��Ϣ�������´��ݣ�����ڷ�Χ�����ܴ�͸��ǰ���
	//	}
	//}
}

void _XControlManager::keyProc(int keyOrder,_XKeyState keyState)
{
	if(m_focusOrder < 0 || m_focusOrder >= m_objectSum) return;
	if(m_objectType[m_objectDrawOrderMap[m_focusOrder]] != CTRL_OBJ_FUNCTION && m_objectType[m_objectDrawOrderMap[m_focusOrder]] != CTRL_OBJ_NULL) 
			((_XControlBasic *)(m_pObject[m_objectDrawOrderMap[m_focusOrder]].pObject))->keyboardProc(keyOrder,keyState);
	//for(int i = 0;i < m_objectSum;++ i)
	//{
	//	if(m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_FUNCTION && m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_NULL) 
	//		((_XControlBasic *)(m_pObject[m_objectDrawOrderMap[i]].pObject))->keyboardProc(keyOrder,keyState);
	//}
}

void _XControlManager::draw(const void * obj)	//�ֶ����ĳ������
{
	if(m_isAutoDraw != 0) return;
	if(findObjectID(obj) < 0) return;
	((_XControlBasic *)obj)->draw();
	((_XControlBasic *)obj)->drawUp();
}

void _XControlManager::draw()
{
	if(m_isAutoDraw == 0) return;
	if(m_objectSum <= 0) return;
	for(int i = m_objectSum - 1;i >= 0;-- i)
	{
		if(m_objectType[m_objectDrawOrderMap[i]] == CTRL_OBJ_FUNCTION) (* (m_pObject[m_objectDrawOrderMap[i]].pFunction))();
		else 
		if(m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_NULL) ((_XControlBasic *)(m_pObject[m_objectDrawOrderMap[i]].pObject))->draw();
	}
	for(int i = m_objectSum - 1;i >= 0;-- i)
	{
		if(m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_FUNCTION && m_objectType[m_objectDrawOrderMap[i]] != CTRL_OBJ_NULL) 
			((_XControlBasic *)(m_pObject[m_objectDrawOrderMap[i]].pObject))->drawUp();
	}
}

void _XControlManager::moveAndInsert(int objectID,int insertObjectID)
{
	if(objectID < 0 || objectID >= m_objectSum) return;	//���ݷǷ�
	if(insertObjectID < 0 || insertObjectID >= m_objectSum) return;	//���ݷǷ�
	if(objectID == insertObjectID) return;	//����Ĳ���
	if(m_objectDrawOrderMap[objectID] > m_objectDrawOrderMap[insertObjectID])
	{//�²�����
		for(int i = 0;i < m_objectSum;++ i)
		{
			if(m_objectDrawOrderMap[i] < m_objectDrawOrderMap[objectID] && 
				m_objectDrawOrderMap[i] >= m_objectDrawOrderMap[insertObjectID]) ++m_objectDrawOrderMap[i];
		}
		m_objectDrawOrderMap[objectID] = m_objectDrawOrderMap[insertObjectID] - 1;
	}else
	{//�ϲ�����
		for(int i = 0;i < m_objectSum;++ i)
		{
			if(m_objectDrawOrderMap[i] > m_objectDrawOrderMap[objectID] && 
				m_objectDrawOrderMap[i] <= m_objectDrawOrderMap[insertObjectID]) --m_objectDrawOrderMap[i];
		}
		m_objectDrawOrderMap[objectID] = m_objectDrawOrderMap[insertObjectID] + 1;
	}
}