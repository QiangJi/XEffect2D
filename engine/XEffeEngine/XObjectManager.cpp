//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XObjectManager.h"
#include "XSprite.h"
#include "XFrameEx.h"
#include "XBasicWindow.h"
#include "XNodeLine.h"
#include "./XControl/XControlManager.h"

void funObjectSelectChange(void * pClass,int objectID)
{
//	printf("select change! %d\n",((_XObjectManager *)pClass)->m_mutiList.getSelectOrder());
}

void funObjectStateChange(void * pClass,int objectID)
{
	//printf("check change!\n");
	_XObjectManager * par = (_XObjectManager *)pClass;
	//�������״̬�ĸı�
	int tempChooseOBJSum = par->getNowChooseObjectSum();
	for(int i = 0;i < tempChooseOBJSum;++ i)
	{
		if(par->m_lineObjOrder[i] >= 0)
		{
			if(par->m_mutiList.getCheckState(1,i)) par->m_objCanEdit[par->m_lineObjOrder[i]] = XTrue;
			else 
			{
				par->m_objCanEdit[par->m_lineObjOrder[i]] = XFalse;
				if(par->m_objectKeyOption[par->m_lineObjOrder[i]] != OBJ_OPTION_NULL) 
					par->m_objectKeyOption[par->m_lineObjOrder[i]] = OBJ_OPTION_NULL;	//���óɲ��ɱ༭״̬
			}

			if(par->m_mutiList.getCheckState(0,i)) ((_XObjectBasic *)(par->m_pObject[par->m_lineObjOrder[i]]))->setVisiable();
			else ((_XObjectBasic *)(par->m_pObject[par->m_lineObjOrder[i]]))->disVisiable();
		}else
		{
			break;
		}
	}
}

_XObjectManager::_XObjectManager()
:m_isInited(XFalse)
,m_isShowUI(XTrue)
,m_isOption(XTrue)
,m_editWindowsPos(0)
,m_canSelect(XTrue)		//Ĭ��״̬���������ѡ�����
#if OBJ_MANAGER_WITH_ID
,m_showObjID(0)
#endif
,m_ctrlKeyState(KEY_STATE_UP)
{
	//put single object into auto_ptr object 
	//m_auto_ptr = auto_ptr<_XObjectManager>(this);
	for(int i = 0;i < MAX_OBJECT_SUM;++ i)
	{
		m_objectMouseState[i] = OBJ_STATE_NULL;		//���״̬��Ч
		//m_objectUpMouseState[i] = OBJ_STATE_NULL;		//���״̬��Ч

		m_pObject[i] = NULL;		//��ʼ��ָ����Ч
		m_objectType[i] = OBJ_NULL;	//��Ч���������
		m_objectKeyOption[i] = OBJ_OPTION_NULL;
		m_objectSetState[i] = OBJ_OPTION_STATE_NULL;
		m_objLineOrder[i] = -1;
		m_lineObjOrder[i] = -1;

		m_objCanEdit[i] = XFalse;	//��ʼ�����е�������ǲ��ɱ༭��
		m_objBeSelect[i] = XFalse;	//��ʼ���������ѡ��
	}
	m_nowObjectSum = 0;
	m_nowMouseOnObjectSum = 0;
	//�����ǹ��ڰ������������ĳ�ʼ��������
	m_keyMaxTime = 500;
	m_keyMinTime = 10;
	for(int i = 0;i < 4;++ i)
	{
		m_keyState[i] = KEY_STATE_UP;
		m_keyTimer[i] = 0;
		m_keyNowTime[i] = m_keyMaxTime;
	}
}

_XObjectManager& _XObjectManager::GetInstance()
{
	static _XObjectManager m_instance;
	return m_instance;
}

int _XObjectManager::addAObject(void * object,_XObjectType type)
{
//	if(!m_isInited) return -1;
	if(m_nowObjectSum >= MAX_OBJECT_SUM) return -1;	//����Ѿ����ˣ�ע��ʧ��
	if(type == OBJ_NULL) return -1;	//��Ч���������ע��
	if(object == NULL) return -1; 
	int ret = findObjectID(object);
	if(ret >= 0) return ret;	//�������Ѿ�ע�ᣬ���ֹ�ظ�ע��
	//���濪ʼע�ᶯ��
	m_objectMouseState[m_nowObjectSum] = OBJ_STATE_NULL;		//Ĭ��״̬
	//m_objectUpMouseState[m_nowObjectSum] = OBJ_STATE_NULL;		//Ĭ��״̬

	m_pObject[m_nowObjectSum] = object;		//��ʼ��ָ����Ч
	m_objectType[m_nowObjectSum] = type;	//��Ч���������
	m_objectMousePoint[m_nowObjectSum].set(0.0f,0.0f);
	m_objectKeyOption[m_nowObjectSum] = OBJ_OPTION_NULL;
	m_objectSetState[m_nowObjectSum] = OBJ_OPTION_STATE_NULL;
	
	m_objectEditParm[m_nowObjectSum] = -1;

	//��ʼ�������״̬
	m_objCanEdit[m_nowObjectSum] = XFalse;	//���ɱ༭
	m_objBeSelect[m_nowObjectSum] = XFalse;	//����ѡ��
	
	++ m_nowObjectSum;
	//printf("ObjectSum:%d\n",m_nowObjectSum);
	return m_nowObjectSum - 1;
}

void _XObjectManager::decreaseAObject(int objectID)
{
//	if(!m_isInited) return;
	if(objectID < 0 || objectID >= m_nowObjectSum) return;	//��������Ƿ�
	if(m_nowObjectSum <= 0) return;	//���������û�������ֱ�ӷ���
	//�������г�ȡ���ע�ᣬ��������������
	if(objectID == m_nowObjectSum - 1)
	{//�Ѿ������һ������
		m_objectMouseState[objectID] = OBJ_STATE_NULL;		//Ĭ��״̬
		//m_objectUpMouseState[objectID] = OBJ_STATE_NULL;
		m_pObject[objectID] = NULL;		//��ʼ��ָ����Ч
		m_objectType[objectID] = OBJ_NULL;	//��Ч���������
		m_objectMousePoint[objectID].set(0.0f,0.0f);
		m_objectKeyOption[objectID] = OBJ_OPTION_NULL;
		m_objectSetState[objectID] = OBJ_OPTION_STATE_NULL;

		-- m_nowObjectSum;
	}else
	{//�������һ������
		for(int i = objectID;i < m_nowObjectSum;++ i)
		{
			m_objectMouseState[i] = m_objectMouseState[i + 1];	//Ĭ��״̬
			//m_objectUpMouseState[i] = m_objectUpMouseState[i + 1];
			m_pObject[i] = m_pObject[i + 1];			//��ʼ��ָ����Ч
			m_objectType[i] = m_objectType[i + 1];		//��Ч���������
			m_objectMousePoint[i] = m_objectMousePoint[i + 1];
			m_objectKeyOption[i] = m_objectKeyOption[i + 1];
			m_objectSetState[i] = m_objectSetState[i + 1];
		}
		m_objectMouseState[m_nowObjectSum - 1] = OBJ_STATE_NULL;			//Ĭ��״̬
		//m_objectUpMouseState[m_nowObjectSum - 1] = OBJ_STATE_NULL;			//Ĭ��״̬
		m_pObject[m_nowObjectSum - 1] = NULL;			//��ʼ��ָ����Ч
		m_objectType[m_nowObjectSum - 1] = OBJ_NULL;	//��Ч���������
		m_objectMousePoint[m_nowObjectSum - 1].set(0.0f,0.0f);
		m_objectKeyOption[m_nowObjectSum - 1] = OBJ_OPTION_NULL;
		m_objectSetState[m_nowObjectSum - 1] = OBJ_OPTION_STATE_NULL;
		-- m_nowObjectSum;
	}
	//printf("ObjectSum:%d\n",m_nowObjectSum);
}

void _XObjectManager::keyProc(int keyID,_XKeyState keyState)
{
	if(!m_isInited) return;	//���û�г�ʼ���Ļ�ֱ�ӷ���
	if(m_nowObjectSum == 0) return;	//���û�����ֱ�ӷ���
	if(keyID == XKEY_LCTRL || keyID == XKEY_RCTRL) m_ctrlKeyState = keyState;	//ctrl���ܼ���״̬

	if(m_ctrlKeyState == KEY_STATE_DOWN && keyID == XKEY_U && keyState == KEY_STATE_UP)
	{
		if(!m_isOption) setOption();
		else disOption();
	}

	if(!m_isOption) return;
	switch(keyID)
	{
	case XKEY_H:	//�Ƿ���ʾUI����
		if(keyState == KEY_STATE_UP)
		{
			if(m_ctrlKeyState == KEY_STATE_DOWN)
			{
				if(!m_isShowUI) setShow();
				else disShow();
			}
		}
		break;
	case XKEY_P:	//�������Դ��ڵ�λ��
		if(keyState == KEY_STATE_UP)
		{
			if(m_ctrlKeyState == KEY_STATE_DOWN) setEditWidowsPos();
		}
		break;
	case XKEY_DELETE:	//ɾ�����Դ�����ѡ�����
		if(keyState == KEY_STATE_UP)
		{//�����������,Ŀǰ�в�ʵ��
			for(int i = 0;i < m_mutiList.getTableLineSum();++ i)
			{
				if(m_mutiList.getHaveSelectState(i))
				{
					m_objBeSelect[m_lineObjOrder[i]] = XFalse;	//ȡ��ѡ��
				}
			}
			upDateMultiListData();	//���¶����б�������
		}
		break;
	case XKEY_N:		//�Ƿ����ʹ������ѡ���
		if(keyState == KEY_STATE_UP) chengeCanSelectState();
		break;
#if OBJ_MANAGER_WITH_ID
	case XKEY_B:		//�Ƿ���ʾ�����ID
		if(keyState == KEY_STATE_UP) setShowObjID();
		break;
#endif
	case XKEY_Q:	//ȥ�����в���
		if(keyState == KEY_STATE_UP)
		{
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				m_objectKeyOption[i] = OBJ_OPTION_NULL;
				updateObjStateInfo(i);
			}
		}
		break;
	case XKEY_E:	//nodeline�������༭״̬
		if(keyState == KEY_STATE_UP)
		{//����༭״̬���л�
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objectType[i] == OBJ_NODELINE && m_objBeSelect[i] && m_objCanEdit[i])
				{
					if(m_objectKeyOption[i] != OBJ_OPTION_EDIT)
					{
						m_objectKeyOption[i] = OBJ_OPTION_EDIT;
						printf("NodeLine Length:%f\n",((_XNodeLine *)m_pObject[i])->getNodeLineLength());
						updateObjStateInfo(i);
					}else
					{
						m_objectKeyOption[i] = OBJ_OPTION_NULL;
						updateObjStateInfo(i);
					}
				}
			}
		}
		break;
	case XKEY_S:	//����
		if(keyState == KEY_STATE_UP)
		{//����,��ʱ�������б�ѡ�е�����������Ų���
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objBeSelect[i] && m_objCanEdit[i])
				{//��ѡ���пɱ༭
					if(m_objectType[i] == OBJ_NODELINE && m_objectKeyOption[i] == OBJ_OPTION_EDIT)
					{//���ﱣ�����ߵ��ļ���
						((_XNodeLine *)m_pObject[i])->saveNodeLine();
					}else
					{
						m_objectKeyOption[i] = OBJ_OPTION_SIZE;
						updateObjStateInfo(i);
					}
				}
			}
		}
		break;
	case XKEY_R:	//��ת
		if(keyState == KEY_STATE_UP)
		{//����
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objBeSelect[i] && m_objCanEdit[i])
				{//��ѡ���ҿɱ༭
					m_objectKeyOption[i] = OBJ_OPTION_ROTATE;
					updateObjStateInfo(i);
				}
			}
		}
		break;
	case XKEY_M:	//�ƶ�
		if(keyState == KEY_STATE_UP)
		{//����
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objBeSelect[i] && m_objCanEdit[i])
				{//��ѡ���ҿɱ༭
					m_objectKeyOption[i] = OBJ_OPTION_POSITION;
					updateObjStateInfo(i);
				}
			}
		}
		break;
	case XKEY_D:
		if(keyState == KEY_STATE_UP)
		{//����
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objectKeyOption[i] == OBJ_OPTION_EDIT && m_objCanEdit[i] && m_objBeSelect[i])
				{//����������ɾ����ǰ�ĵ�
					_XNodeLine * temp = (_XNodeLine *)m_pObject[i];
					if(temp->getNodeSum() > 2 && m_objectEditParm[i] >= 0 && m_objectEditParm[i] < temp->getNodeSum()) //����Ҫ����2�������ɾ����
					{
						temp->decreaseOneNode(m_objectEditParm[i]);
						if(m_objectEditParm[i] >= temp->getNodeSum()) m_objectEditParm[i] = temp->getNodeSum() - 1;
					}
				}
			}
		}
		break;
	case XKEY_A://�����������ڵ�ǰλ������һ����
		if(keyState == KEY_STATE_UP)
		{//����
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objectKeyOption[i] == OBJ_OPTION_EDIT && m_objCanEdit[i] && m_objBeSelect[i])
				{//����������ɾ����ǰ�ĵ�
					_XNodeLine * temp = (_XNodeLine *)m_pObject[i];
					if(m_objectEditParm[i] >= 0 && m_objectEditParm[i] < temp->getNodeSum() && temp->getSize().x != 0.0f && temp->getSize().y != 0.0f) //����Ҫ����2�������ɾ����
					{
						temp->addOneNode((m_nowMousePosition.x - temp->getPosition().x) / temp->getSize().x,
							(m_nowMousePosition.y - temp->getPosition().y) / temp->getSize().y,
							m_objectEditParm[i]);
					}
				}
			}
		}
		break;
	case XKEY_C:	//�ı���ת��׼��
		if(keyState == KEY_STATE_UP)
		{//����
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objBeSelect[i] && m_objCanEdit[i])
				{//��ѡ���ҿɱ༭
					m_objectKeyOption[i] = OBJ_OPTION_ROTATEMODE;
					updateObjStateInfo(i);
				}
			}
		}
		break;
	case XKEY_T:	//���÷�ת״̬
		if(keyState == KEY_STATE_UP)
		{//����
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objBeSelect[i] && m_objCanEdit[i])
				{//��ѡ���ҿɱ༭
					m_objectKeyOption[i] = OBJ_OPTION_TURNOVERMODE;
					updateObjStateInfo(i);
				}
			}
		}
		break;
	case XKEY_LEFT:
		if(keyState == KEY_STATE_DOWN)
		{//����
			m_keyState[0] = KEY_STATE_DOWN;
			m_keyTimer[0] = 0;
			m_keyNowTime[0] = m_keyMaxTime;
		}else
		{//������������������Ӧ�Ĳ���
			m_keyState[0] = KEY_STATE_UP;
		}
		break;
	case XKEY_RIGHT:
		if(keyState == KEY_STATE_DOWN)
		{//����
			m_keyState[1] = KEY_STATE_DOWN;
			m_keyTimer[1] = 0;
			m_keyNowTime[1] = m_keyMaxTime;
		}else
		{//����
			m_keyState[1] = KEY_STATE_UP;
		}
		break;
	case XKEY_UP:
		if(keyState == KEY_STATE_DOWN)
		{//����
			m_keyState[2] = KEY_STATE_DOWN;
			m_keyTimer[2] = 0;
			m_keyNowTime[2] = m_keyMaxTime;
		}else
		{//����
			m_keyState[2] = KEY_STATE_UP;
		}
		break;
	case XKEY_DOWN:
		if(keyState == KEY_STATE_DOWN)
		{//����
			m_keyState[3] = KEY_STATE_DOWN;
			m_keyTimer[3] = 0;
			m_keyNowTime[3] = m_keyMaxTime;
		}else
		{//����
			m_keyState[3] = KEY_STATE_UP;
		}
		break;
	}
}

void _XObjectManager::objectKeyOption(int order,_XObjectOptionType optionType)
{
	if(order < 0 || order >= m_nowObjectSum) return;
	switch(optionType)
	{
	case OBJ_OPTION_SIZE_DOWN:	//��С
		switch(m_objectType[order])
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				_XObjectBasic * temp = (_XObjectBasic *)m_pObject[order];
				_XVector2 size = temp->getSize();
				if(size.x - 0.01f >= 0) size.x -= 0.01f;
				else size.x = 0.0f;
				if(size.y - 0.01f >= 0) size.y -= 0.01f;
				else size.y = 0.0f;
				temp->setSize(size);
			}
			break;
		}
		break;
	case OBJ_OPTION_SIZE_ON:	//�Ŵ�
		switch(m_objectType[order])
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				_XObjectBasic * temp = (_XObjectBasic *)m_pObject[order];
				_XVector2 size = temp->getSize();
				size += 0.01f;
				temp->setSize(size);
			}
			break;
		}
		break;
	case OBJ_OPTION_ROTATE_DOWN:	//�Ƕ���С
		switch(m_objectType[order])
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
			{
				_XObjectBasic * temp = (_XObjectBasic *)m_pObject[order];
				float angle = temp->getAngle();
				angle -= 0.5f;
				if(angle < 0) angle += 360.0f;
				temp->setAngle(angle);
			}
			break;
		}
		break;
	case OBJ_OPTION_ROTATE_ON:	//�Ƕȱ��
		switch(m_objectType[order])
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
			{
				_XObjectBasic * temp = (_XObjectBasic *)m_pObject[order];
				float angle = temp->getAngle();
				angle += 0.5f;
				if(angle > 360.0f) angle -= 360.0f;
				temp->setAngle(angle);
			}
			break;
		}
		break;
	case OBJ_OPTION_ROTATE_MODE_CHANGE:	//��תģʽ�ı仯
		switch(m_objectType[order])
		{
		case OBJ_SPRITE:
			{
				_XSprite * temp = (_XSprite *)m_pObject[order];
				_XTransformMode mode = temp->getTransformMode();
				if(mode == POINT_LEFT_TOP) temp->setIsTransformCenter(POINT_CENTER);
				else temp->setIsTransformCenter(POINT_LEFT_TOP);
			}
			break;
		//case OBJ_FRAMEEX:
		//	{
		//		_XFrameEx * temp = (_XFrameEx *)m_pObject[order];
		//		_XTransformMode mode = temp->getTransformMode();
		//		if(mode == POINT_LEFT_TOP) temp->setIsTransformCenter(POINT_CENTER);
		//		else temp->setIsTransformCenter(POINT_LEFT_TOP);
		//	}
		//	break;
		}
		break;
	case OBJ_OPTION_TURNOVER_MODE_CHANGE:	//��תģʽ�ı仯
		switch(m_objectType[order])
		{
		case OBJ_SPRITE:
			{
				_XSprite * temp = (_XSprite *)m_pObject[order];
				_XTurnOverMode mode = temp->getTurnOverMode();
				if(mode == TURN_OVER_MODE_NULL) temp->setLeft2Right(); else
				if(mode == TURN_OVER_MODE_LEFT_TO_RIGHT) temp->setUp2Down();
				else temp->resetLeftRightUpDown();
			}
			break;
		//case OBJ_FRAMEEX:
		//	{
		//		_XFrameEx * temp = (_XFrameEx *)m_pObject[order];
		//		_XTurnOverMode mode = temp->getTurnOverMode();
		//		if(mode == TURN_OVER_MODE_NULL) temp->setLeft2Right(); else
		//		if(mode == TURN_OVER_MODE_LEFT_TO_RIGHT) temp->setUp2Down();
		//		else temp->resetLeftRightUpDown();
		//	}
		//	break;
		}
		break;
	case OBJ_OPTION_X_ON:	//�ƶ�
		switch(m_objectType[order])
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				_XObjectBasic * temp = (_XObjectBasic *)m_pObject[order];
				_XVector2 position = temp->getPosition();
				position.x += 1.0f;
				temp->setPosition(position);
			}
			break;
		}
		break;
	case OBJ_OPTION_X_DOWN:	//�ƶ�
		switch(m_objectType[order])
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				_XObjectBasic * temp = (_XObjectBasic *)m_pObject[order];
				_XVector2 position = temp->getPosition();
				position.x -= 1.0f;
				temp->setPosition(position);
			}
			break;
		}
		break;
	case OBJ_OPTION_Y_ON:	//�ƶ�
		switch(m_objectType[order])
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				_XObjectBasic * temp = (_XObjectBasic *)m_pObject[order];
				_XVector2 position = temp->getPosition();
				position.y += 1.0f;
				temp->setPosition(position);
			}
			break;
		}
		break;
	case OBJ_OPTION_Y_DOWN:	//�ƶ�
		switch(m_objectType[order])
		{
		case OBJ_SPRITE:
		case OBJ_FRAMEEX:
		case OBJ_NUMBER:
		case OBJ_FONTUNICODE:
		case OBJ_FONTX:
		case OBJ_NODELINE:
		case OBJ_CONTROL:
			{
				_XObjectBasic * temp = (_XObjectBasic *)m_pObject[order];
				_XVector2 position = temp->getPosition();
				position.y -= 1.0f;
				temp->setPosition(position);
			}
			break;
		}
		break;
	}
}

void _XObjectManager::mouseProc(int x,int y,_XMouseState eventType)
{//�����Ӧ����
	if(!m_isInited) return;	//���û�г�ʼ���Ļ�ֱ�ӷ���
	if(!m_isOption) return;
	m_nowMouseOnObjectSum = 0;
	_XBool stateChangeFlag = XFalse;	//ѡ���״̬�Ƿ����˸ı�
	if(m_isShowUI && m_mutiList.isInRect(x,y) != 0) return;	//��ʾ�ռ��ʱ����Ϣ�������´���

	switch(eventType)
	{
	case MOUSE_LEFT_BUTTON_DOWN:	//������µĶ���
		//printf("Mouse Left Botton Down:%d,%d\n",x,y);
		for(int i = 0;i < m_nowObjectSum;++ i)
		{//�������ζ�ÿ��������������Ӧ
			switch(m_objectType[i])
			{
			case OBJ_NULL:
				break;
			case OBJ_SPRITE:
			case OBJ_FRAMEEX:
			case OBJ_NUMBER:
			case OBJ_FONTUNICODE:
			case OBJ_FONTX:
			case OBJ_CONTROL:
				{
					_XObjectBasic * temp = (_XObjectBasic *)m_pObject[i];
					if(temp->isInRect(x,y) != 0)
					{//�����갴��
						m_objectMouseState[i] = OBJ_STATE_MOUSE_DOWN;	//���崦�ڰ���״̬

						m_objectMousePoint[i] = temp->getPosition();
						m_objectMousePoint[i].x -= x;
						m_objectMousePoint[i].y -= y;
					}
				}
				break;
			case OBJ_NODELINE:
				{
					_XObjectBasic * temp = (_XObjectBasic *)m_pObject[i];
					if(temp->isInRect(x,y) != 0)
					{//���������ϱ������������Ӧ
						if(m_objectKeyOption[i] == OBJ_OPTION_EDIT)
						{//���ڱ༭״̬
							_XNodeLine *tempNodeLine = (_XNodeLine *)m_pObject[i];
							int tempOrder = -1;
							if(tempNodeLine->getSize().x != 0 && tempNodeLine->getSize().y != 0)
							{//��ֹ�������
								tempOrder = tempNodeLine->getNodeOrder((x - tempNodeLine->getPosition().x) / tempNodeLine->getSize().x,
									(y - tempNodeLine->getPosition().y) / tempNodeLine->getSize().y,5);
								if(tempOrder >= 0)// && tempOrder != m_objectEditParm[i])
								{
									m_objectEditParm[i] = tempOrder;

									m_objectMouseState[i] = OBJ_STATE_MOUSE_DOWN;
									m_objectMousePoint[i] = tempNodeLine->getNode(tempOrder);
									m_objectMousePoint[i].x -= x / tempNodeLine->getSize().x;
									m_objectMousePoint[i].y -= y / tempNodeLine->getSize().y;
								}
							}
						}else
						{//�Ǳ༭״̬
							m_objectMouseState[i] = OBJ_STATE_MOUSE_DOWN;
							m_objectMousePoint[i] = temp->getPosition();
							m_objectMousePoint[i].x -= x;
							m_objectMousePoint[i].y -= y;
						}
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case MOUSE_LEFT_BUTTON_UP:		//���������
		//printf("Mouse Left Botton Up:%d,%d\n",x,y);
		for(int i = 0;i < m_nowObjectSum;++ i)
		{//�������ζ�ÿ��������������Ӧ
			switch(m_objectType[i])
			{
			case OBJ_NULL:
				break;
			case OBJ_SPRITE:
			case OBJ_FRAMEEX:
			case OBJ_NUMBER:
			case OBJ_FONTUNICODE:
			case OBJ_FONTX:
			case OBJ_NODELINE:
			case OBJ_CONTROL:
				{
					if(m_objectMouseState[i] == OBJ_STATE_MOUSE_DOWN)	//ֻҪ��굯�𣬲����Ƿ��������λ�ö�����Ӧ�ж�
					{//���������ϱ������������Ӧ
						if(m_objectType[i] == OBJ_NODELINE)
						{
							if(m_objectKeyOption[i] != OBJ_OPTION_EDIT)
							{//�Ǳ༭״̬
								if(m_canSelect) 
								{
									if(!m_objBeSelect[i]) m_objBeSelect[i] = XTrue;
									else 
									{
										m_objBeSelect[i] = XFalse;
										m_objectKeyOption[i] = OBJ_OPTION_NULL;
									}
								}
								m_objectMouseState[i] = OBJ_STATE_NULL;
							}else
							{//�༭״̬
								m_objectMouseState[i] = OBJ_STATE_NULL;	//���û�ж���
							}
						}else
						{
							if(m_canSelect) 
							{
								if(!m_objBeSelect[i]) m_objBeSelect[i] = XTrue;
								else 
								{
									m_objBeSelect[i] = XFalse;
									m_objectKeyOption[i] = OBJ_OPTION_NULL;
								}
							}
							m_objectMouseState[i] = OBJ_STATE_NULL;
						}
						stateChangeFlag = XTrue;
					}else
					if(m_objectMouseState[i] == OBJ_STATE_MOUSE_MOVE)
					{//�������϶�״̬����
						stateChangeFlag = XTrue;
						m_objectMouseState[i] = OBJ_STATE_NULL;
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case MOUSE_MOVE:		//����ƶ��¼�
		m_nowMousePosition.set(x,y);
		for(int i = 0;i < m_nowObjectSum;++ i)
		{//�������ζ�ÿ��������������Ӧ
			switch(m_objectType[i])
			{
			case OBJ_NULL:
				break;
			case OBJ_SPRITE:
			case OBJ_FRAMEEX:
			case OBJ_NUMBER:
			case OBJ_FONTUNICODE:
			case OBJ_FONTX:
			case OBJ_NODELINE:
			case OBJ_CONTROL:
			{
				if(m_objectMouseState[i] == OBJ_STATE_NULL)	//�����¼����ж�
				{//���֮ǰ�������״̬
					_XObjectBasic * temp = (_XObjectBasic *)m_pObject[i];
					if(temp->isInRect(x,y) != 0)
					{//������Ϊ�������״̬
						m_objectMouseState[i] = OBJ_STATE_MOUSE_ON;	//���������¼�
						m_nowMouseOnObjectSum ++;
					}
				}else
				if(m_objectMouseState[i] == OBJ_STATE_MOUSE_ON)	//����Ѿ���������״̬���������ж��Ƕ�����״̬����
				{//����Ѿ���������״̬���������ж��Ƿ��˳�����״̬
					_XObjectBasic * temp = (_XObjectBasic *)m_pObject[i];
					if(temp->isInRect(x,y) == 0) m_objectMouseState[i] = OBJ_STATE_NULL;	//��������״̬����
				}else
				if((m_objectMouseState[i] == OBJ_STATE_MOUSE_DOWN || m_objectMouseState[i] == OBJ_STATE_MOUSE_MOVE) && m_objCanEdit[i])	//���״̬�Ļ����������ø�������ƶ�
				{//����Ѿ����ڰ���״̬������ק״̬�����ҿ��Ա༭
					if(m_objectType[i] != OBJ_NODELINE)
					{//�ı������λ��
						_XObjectBasic * temp = (_XObjectBasic *)m_pObject[i];
						temp->setPosition(m_objectMousePoint[i].x + x,m_objectMousePoint[i].y + y);
					}else
					{//���������߿����Ǹı������ߵ�λ�û����Ǹı���������ĳ�����λ��
						if(m_objectKeyOption[i] == OBJ_OPTION_EDIT)
						{//�ı�NodeLine�еĵ��λ��
							((_XNodeLine *)m_pObject[i])->setOneNode(m_objectMousePoint[i].x + x / ((_XNodeLine *)m_pObject[i])->getSize().x,
								m_objectMousePoint[i].y + y / ((_XNodeLine *)m_pObject[i])->getSize().y,m_objectEditParm[i]);
						}else
						{
							_XObjectBasic * temp = (_XObjectBasic *)m_pObject[i];
							temp->setPosition(m_objectMousePoint[i].x + x,m_objectMousePoint[i].y + y);
						}
					}
					m_objectMouseState[i] = OBJ_STATE_MOUSE_MOVE;	//����϶�״̬
				}
			}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	if(stateChangeFlag)
	{//�����ѡ���״̬�����˸ı�Ļ���������Ҫ�����б��
		upDateMultiListData();
	}
}
void _XObjectManager::upDateMultiListData()
{
	m_mutiList.clearAllSelect();
	m_mutiList.setLineSum(0);
	char tempStr[256] = "";
	int lineOrder = 0;
	for(int i = 0;i < m_nowObjectSum;++ i)
	{
		if(m_objBeSelect[i])
		{
			m_mutiList.addALine();
			sprintf(tempStr,"%d",i);
			m_mutiList.setBoxStr(tempStr,lineOrder,0);
			sprintf(tempStr,"%.0f,%.0f",m_objInfo[i].m_position.x,m_objInfo[i].m_position.y);
			m_mutiList.setBoxStr(tempStr,lineOrder,3);
			sprintf(tempStr,"%.2f,%.2f",m_objInfo[i].m_size.x,m_objInfo[i].m_size.y);
			m_mutiList.setBoxStr(tempStr,lineOrder,4);
			sprintf(tempStr,"%.2f",m_objInfo[i].m_angle);
			m_mutiList.setBoxStr(tempStr,lineOrder,5);
			m_objLineOrder[i] = lineOrder;
			m_lineObjOrder[lineOrder] = i;
			updateObjStateInfo(i);	//����Ҫ��ʼ���˶�Ӧ��ϵ֮����ܸ���״̬
			//switch(m_objectKeyOption[i])
			//{
			//case OBJ_OPTION_NULL:m_mutiList.setBoxStr("Null",lineOrder,1);break;
			//case OBJ_OPTION_POSITION:m_mutiList.setBoxStr("Position",lineOrder,1);break;
			//case OBJ_OPTION_SIZE:m_mutiList.setBoxStr("Size",lineOrder,1);break;
			//case OBJ_OPTION_ROTATE:m_mutiList.setBoxStr("Rotate",lineOrder,1);break;
			//case OBJ_OPTION_ROTATEMODE:m_mutiList.setBoxStr("RotateMode",lineOrder,1);break;
			//case OBJ_OPTION_TURNOVERMODE:m_mutiList.setBoxStr("TurnOverMode",lineOrder,1);break;
			//}
			//�����������Ŀɱ༭״̬
			if(!m_objCanEdit[i]) m_mutiList.setCheckState(1,lineOrder,XFalse);
			else m_mutiList.setCheckState(1,lineOrder,XTrue); 

			if(((_XObjectBasic *)m_pObject[i])->getVisiable()) m_mutiList.setCheckState(0,lineOrder,XTrue);
			else m_mutiList.setCheckState(0,lineOrder,XFalse); 
			lineOrder ++;
		}else
		{
			m_objLineOrder[i] = -1;
		}
	}
}
void _XObjectManager::updateObjStateInfo(int order)
{
	switch(m_objectType[order])
	{
	case OBJ_NULL:m_mutiList.setBoxStr("Null",m_objLineOrder[order],1);break;
	case OBJ_SPRITE:m_mutiList.setBoxStr("Sprite",m_objLineOrder[order],1);break;
	case OBJ_FRAME:m_mutiList.setBoxStr("Frame",m_objLineOrder[order],1);break;
	case OBJ_FRAMEEX:m_mutiList.setBoxStr("FrameEx",m_objLineOrder[order],1);break;
	case OBJ_NUMBER:m_mutiList.setBoxStr("Number",m_objLineOrder[order],1);break;
	case OBJ_FONTUNICODE:m_mutiList.setBoxStr("FontUnicode",m_objLineOrder[order],1);break;
	case OBJ_FONTX:m_mutiList.setBoxStr("Fontx",m_objLineOrder[order],1);break;
	case OBJ_NODELINE:m_mutiList.setBoxStr("NodeLine",m_objLineOrder[order],1);break;
	case OBJ_CONTROL:m_mutiList.setBoxStr("Control",m_objLineOrder[order],1);break;
	}
	switch(m_objectKeyOption[order])
	{
	case OBJ_OPTION_NULL:m_mutiList.setBoxStr("Null",m_objLineOrder[order],2);break;
	case OBJ_OPTION_POSITION:m_mutiList.setBoxStr("Position",m_objLineOrder[order],2);break;
	case OBJ_OPTION_SIZE:m_mutiList.setBoxStr("Size",m_objLineOrder[order],2);break;
	case OBJ_OPTION_ROTATE:m_mutiList.setBoxStr("Rotate",m_objLineOrder[order],2);break;
	case OBJ_OPTION_ROTATEMODE:m_mutiList.setBoxStr("RotateMode",m_objLineOrder[order],2);break;
	case OBJ_OPTION_TURNOVERMODE:m_mutiList.setBoxStr("TurnOverMode",m_objLineOrder[order],2);break;
	}
}
void _XObjectManager::updateObjInfo(int order)
{
	//printf("Update Object Info -> %d.\n",order);
	_XObjectBasic * temp = (_XObjectBasic *)m_pObject[order];
	m_objInfo[order].m_position = temp->getPosition();
	m_objInfo[order].m_size = temp->getSize();
	m_objInfo[order].m_angle = temp->getAngle();
//	m_objInfo[order].m_alpha = temp->getAngle();

	char tempStr[256] = "";
#if OBJ_MANAGER_WITH_ID
	sprintf(tempStr,"%d",order);
	m_fontID[order].setString(tempStr);
	_XVector2 fontPosition = m_objInfo[order].m_position;
	if(fontPosition.x < 0.0f) fontPosition.x = 0.0f;	//��ֹ������ʾ������Ļ��Χ����������û���ж����ޣ�ֻ�ж�������
	if(fontPosition.x >= XEE::windowWidth - m_fontID[order].getMaxPixelWidth()) fontPosition.x = XEE::windowWidth - m_fontID[order].getMaxPixelWidth();
	if(fontPosition.y < 0.0f) fontPosition.y = 0.0f;
	if(fontPosition.y >= XEE::windowHeight - m_fontID[order].getMaxPixelHeight()) fontPosition.y = XEE::windowHeight - m_fontID[order].getMaxPixelHeight();
	m_fontID[order].setPosition(fontPosition);
#endif
	if(m_objLineOrder[order] < 0) return;
	sprintf(tempStr,"%d",order);
	m_mutiList.setBoxStr(tempStr,m_objLineOrder[order],0);
	updateObjStateInfo(order);
	sprintf(tempStr,"%.0f,%.0f",m_objInfo[order].m_position.x,m_objInfo[order].m_position.y);
	m_mutiList.setBoxStr(tempStr,m_objLineOrder[order],3);
	sprintf(tempStr,"%.2f,%.2f",m_objInfo[order].m_size.x,m_objInfo[order].m_size.y);
	m_mutiList.setBoxStr(tempStr,m_objLineOrder[order],4);
	sprintf(tempStr,"%.2f",m_objInfo[order].m_angle);
	m_mutiList.setBoxStr(tempStr,m_objLineOrder[order],5);
}

_XBool _XObjectManager::checkNeedUpdate(int order)
{
	_XObjectBasic * temp = (_XObjectBasic *)m_pObject[order];
	if(m_objInfo[order].m_position.x != temp->getPosition().x || m_objInfo[order].m_position.y != temp->getPosition().y) return XTrue;
	if(m_objInfo[order].m_size.x != temp->getSize().x || m_objInfo[order].m_size.y != temp->getSize().y) return XTrue;
	if(m_objInfo[order].m_angle != temp->getAngle()) return XTrue;
	if(m_objectType[order] == OBJ_NODELINE && m_objInfo[order].length != ((_XNodeLine *)m_pObject[order])->getNodeLineLength()) return XTrue;
	return XFalse;
}

void _XObjectManager::draw()	//�����������̫��ʱ�����������ִ��Ч�ʽϵ�
{//��ʾ����ĵ�ǰ״̬
	if(!m_isInited) return;	//���û�г�ʼ���Ļ�ֱ�ӷ���
	if(!m_isOption) return;

	for(int i = 0;i < m_nowObjectSum;++ i)
	{
		if(m_objectMouseState[i] != OBJ_STATE_NULL || m_objBeSelect[i])
		{//��ѡ��״̬�������״̬��
			switch(m_objectType[i])
			{
			case OBJ_NULL:
				break;
			case OBJ_SPRITE:
			case OBJ_FRAMEEX:
			case OBJ_NUMBER:
			case OBJ_FONTUNICODE:
			case OBJ_FONTX:
			case OBJ_CONTROL:
				{
					if(checkNeedUpdate(i)) updateObjInfo(i);
					_XObjectBasic * temp = (_XObjectBasic *)m_pObject[i];
					//�������λ�������ʮ��
					drawLine(m_objInfo[i].m_position.x - 5,m_objInfo[i].m_position.y,m_objInfo[i].m_position.x + 5,m_objInfo[i].m_position.y,
						1,1.0f,0.0f,0.0f,1.0f);
					drawLine(m_objInfo[i].m_position.x,m_objInfo[i].m_position.y - 5,m_objInfo[i].m_position.x,m_objInfo[i].m_position.y + 5,
						2,1.0f,0.0f,0.0f,1.0f);
					//ʹ��������Χ���
					if(m_objBeSelect[i])
					{//����ѡ��״̬
						if(m_objCanEdit[i])
						{//�ɱ༭Ϊ��ɫ
							if(m_mutiList.getHaveSelectState(m_objLineOrder[i])) drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3),1,0.0f,0.0f,1.0f);	//��ѡ�е��������ɫ
							else drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3));
						}else
						{//���ɱ༭Ϊ��ɫ
							drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3),1,0.0f,0.0f,0.0f);
						}
					}else
					if(m_objectMouseState[i] == OBJ_STATE_MOUSE_DOWN)
					{//��갴��״̬
						drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3),1,1.0f,0.0f,0.0f);
					}else
					if(m_objectMouseState[i] == OBJ_STATE_MOUSE_ON)
					{//�������״̬
						drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3),1,1.0f,1.0f,0.0f);
					}
#if OBJ_MANAGER_WITH_ID
					if(m_showObjID != 0) m_fontID[i].draw();
#endif
				}
				break;
			case OBJ_NODELINE:
				{
					if(checkNeedUpdate(i)) updateObjInfo(i);
					_XObjectBasic * temp = (_XObjectBasic *)m_pObject[i];
					//�������λ�������ʮ��
					drawLine(m_objInfo[i].m_position.x - 5,m_objInfo[i].m_position.y,m_objInfo[i].m_position.x + 5,m_objInfo[i].m_position.y,
						2,1.0f,0.0f,0.0f,1.0f);
					drawLine(m_objInfo[i].m_position.x,m_objInfo[i].m_position.y - 5,m_objInfo[i].m_position.x,m_objInfo[i].m_position.y + 5,
						2,1.0f,0.0f,0.0f,1.0f);
					if(m_objectType[i] == OBJ_NODELINE)
					{//������Ҫ��ʾ�ڵ�������Ϣ
						_XNodeLine * tempNodeLine = (_XNodeLine *)m_pObject[i];
						for(int j = 0;j < tempNodeLine->getNodeSum();++ j)
						{
							//��������
							if(j == 0)
							{//��һ��������ɫ
								drawLine(tempNodeLine->getNode(j).x * tempNodeLine->getSize().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getSize().y + tempNodeLine->getPosition().y - 2,
									tempNodeLine->getNode(j).x * tempNodeLine->getSize().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getSize().y + tempNodeLine->getPosition().y + 2,
									4,0.0f,1.0f,0.0f);
							}else
							if(j == m_objectEditParm[i])
							{//��ѡ�еĵ�ʹ�ð�ɫ
								drawLine(tempNodeLine->getNode(j).x * tempNodeLine->getSize().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getSize().y + tempNodeLine->getPosition().y - 2,
									tempNodeLine->getNode(j).x * tempNodeLine->getSize().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getSize().y + tempNodeLine->getPosition().y + 2,
									4);
							}else
							{//�������ú�ɫ
								drawLine(tempNodeLine->getNode(j).x * tempNodeLine->getSize().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getSize().y + tempNodeLine->getPosition().y - 2,
									tempNodeLine->getNode(j).x * tempNodeLine->getSize().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getSize().y + tempNodeLine->getPosition().y + 2,
									4,1.0f,0.0f,0.0f);
							}
							if(j < tempNodeLine->getNodeSum() - 1)
							{
								drawLine(tempNodeLine->getNode(j).x * tempNodeLine->getSize().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j).y * tempNodeLine->getSize().y + tempNodeLine->getPosition().y ,
									tempNodeLine->getNode(j + 1).x * tempNodeLine->getSize().x + tempNodeLine->getPosition().x,
									tempNodeLine->getNode(j + 1).y * tempNodeLine->getSize().y + tempNodeLine->getPosition().y);
							}
						}
						drawLine(tempNodeLine->getSpecialPointPosition().x,tempNodeLine->getSpecialPointPosition().y - 2,
							tempNodeLine->getSpecialPointPosition().x,tempNodeLine->getSpecialPointPosition().y + 2,
							4,1.0f,1.0f,0.0f);
					}
					if(m_objBeSelect[i] || m_objectMouseState[i]  == OBJ_STATE_MOUSE_MOVE)
					{//�༭״̬
						if(m_objectKeyOption[i] == OBJ_OPTION_EDIT)
						{
							drawBox(temp->getBox(0),temp->getBox(1),temp->getBox(2),temp->getBox(3));
						}
#if OBJ_MANAGER_WITH_ID
						if(m_showObjID != 0) m_fontID[i].draw();
#endif
					}
				}
				break;
			}
		}
	}
}

void _XObjectManager::move(int delay)
{
	if(!m_isInited) return;
	if(!m_isOption) return;
	if(m_keyState[0] == KEY_STATE_DOWN)
	{//�������
		m_keyTimer[0] -= delay;
		if(m_keyTimer[0] <= 0)
		{//����ʱ�䵽
			m_keyNowTime[0] = (m_keyNowTime[0] >> 1);
			if(m_keyNowTime[0] < m_keyMinTime) m_keyNowTime[0] = m_keyMinTime;
			m_keyTimer[0] = m_keyNowTime[0];
			//������Ӧһ�ΰ�������
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				switch(m_objectKeyOption[i])
				{
				case OBJ_OPTION_SIZE:	//��С
					objectKeyOption(i,OBJ_OPTION_SIZE_DOWN);
					break;
				case OBJ_OPTION_ROTATE:	
					objectKeyOption(i,OBJ_OPTION_ROTATE_DOWN);
					break;
				case OBJ_OPTION_ROTATEMODE:	
					objectKeyOption(i,OBJ_OPTION_ROTATE_MODE_CHANGE);
					break;
				case OBJ_OPTION_TURNOVERMODE:	
					objectKeyOption(i,OBJ_OPTION_TURNOVER_MODE_CHANGE);
					break;
				case OBJ_OPTION_POSITION:	
					objectKeyOption(i,OBJ_OPTION_X_DOWN);
					break;
				}
			}
		}
	}
	if(m_keyState[1] == KEY_STATE_DOWN)
	{//�Ҽ�������
		m_keyTimer[1] -= delay;
		if(m_keyTimer[1] <= 0)
		{//����ʱ�䵽
			m_keyNowTime[1] = (m_keyNowTime[1] >> 1);
			if(m_keyNowTime[1] < m_keyMinTime) m_keyNowTime[1] = m_keyMinTime;
			m_keyTimer[1] = m_keyNowTime[1];
			//������Ӧһ�ΰ�������
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				switch(m_objectKeyOption[i])
				{
				case OBJ_OPTION_SIZE:	//�Ŵ�
					objectKeyOption(i,OBJ_OPTION_SIZE_ON);
					break;
				case OBJ_OPTION_ROTATE:	
					objectKeyOption(i,OBJ_OPTION_ROTATE_ON);
					break;
				case OBJ_OPTION_POSITION:	
					objectKeyOption(i,OBJ_OPTION_X_ON);
					break;
				}
			}
		}
	}
	if(m_keyState[2] == KEY_STATE_DOWN)
	{//�ϼ�������
		m_keyTimer[2] -= delay;
		if(m_keyTimer[2] <= 0)
		{//����ʱ�䵽
			m_keyNowTime[2] = (m_keyNowTime[2] >> 1);
			if(m_keyNowTime[2] < m_keyMinTime) m_keyNowTime[2] = m_keyMinTime;
			m_keyTimer[2] = m_keyNowTime[2];
			//������Ӧһ�ΰ�������
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				switch(m_objectKeyOption[i])
				{
				case OBJ_OPTION_POSITION:	
					objectKeyOption(i,OBJ_OPTION_Y_DOWN);
					break;
				}
			}
		}
	}
	if(m_keyState[3] == KEY_STATE_DOWN)
	{//�¼�������
		m_keyTimer[3] -= delay;
		if(m_keyTimer[3] <= 0)
		{//����ʱ�䵽
			m_keyNowTime[3] = (m_keyNowTime[3] >> 1);
			if(m_keyNowTime[3] < m_keyMinTime) m_keyNowTime[3] = m_keyMinTime;
			m_keyTimer[3] = m_keyNowTime[3];
			//������Ӧһ�ΰ�������
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				switch(m_objectKeyOption[i])
				{
				case OBJ_OPTION_POSITION:	
					objectKeyOption(i,OBJ_OPTION_Y_ON);
					break;
				}
			}
		}
	}
}
int _XObjectManager::init(_XResourcePosition resoursePosition)
{
	if(m_isInited) return XTrue;
	//��ʼ������
#if WITH_COMMON_RESOURCE_FOLDER
	if(m_font.initEx("../../Common/ResourcePack/pic/ObjectManager/Font",resoursePosition) == 0) return XFalse;
#else
	if(m_font.initEx("ResourcePack/pic/ObjectManager/Font",resoursePosition) == 0) return XFalse;
#endif
	decreaseAObject(&(m_font));
#if OBJ_MANAGER_WITH_ID
	for(int i = 0;i < MAX_OBJECT_SUM;++ i)
	{
		m_fontID[i].setACopy(m_font);
		decreaseAObject(&(m_fontID[i]));
		m_fontID[i].setMaxStrLen(5);
	}
#endif

	//��ʼ��һ���б��
	//if(m_sliderTextureH.initEx("ResourcePack/pic/ObjectManager/MutiList/SliderH",resoursePosition) == 0) return 0;
	//if(m_sliderH.initEx(_XVector2(200.0f,200.0f),m_sliderTextureH,SLIDER_TYPE_HORIZONTAL,100.0f,0.0f) == 0) return 0;
	//_XControlManager::GetInstance().decreaseAObject(&m_sliderH);
	//decreaseAObject(&m_sliderH);

	//if(m_sliderTextureV.initEx("ResourcePack/pic/ObjectManager/MutiList/SliderV",resoursePosition) == 0) return 0;
	//if(m_sliderV.initEx(_XVector2(200.0f,200.0f),m_sliderTextureV,SLIDER_TYPE_VERTICAL,100.0f,0.0f) == 0) return 0;
	//_XControlManager::GetInstance().decreaseAObject(&m_sliderV);
	//decreaseAObject(&m_sliderV);

	//if(m_checkTex0.initEx("ResourcePack/pic/ObjectManager/MutiList/checkC",resoursePosition) == 0) return 0;
	//if(m_checkTex1.initEx("ResourcePack/pic/ObjectManager/MutiList/checkE",resoursePosition) == 0) return 0;
	//if(m_mutiListTexture.initEx("ResourcePack/pic/ObjectManager/MutiList",resoursePosition) == 0) return 0;

	//if(m_mutiList.initEx(_XVector2(XEE::sceneX + XEE::sceneWidth - 289.0f,XEE::sceneY),m_mutiListTexture,m_checkTex0,m_checkTex1,m_font,1.0f,5,0,m_sliderV,m_sliderH) == 0) return 0;
#if WITH_COMMON_RESOURCE_FOLDER
	if(m_mutiList.initPlus("../../Common/ResourcePack/pic/ObjectManager/MutiList",m_font,1.0f,5,0,resoursePosition) == 0) return XFalse;
#else
	if(m_mutiList.initPlus("ResourcePack/pic/ObjectManager/MutiList",m_font,1.0f,5,0,resoursePosition) == 0) return XFalse;
#endif
	//if(m_mutiList.initWithoutTex(_XRect(64,0,500,500),m_font,1.0f,5,0) == 0) return 0;
	m_mutiList.setPosition(XEE::sceneX + XEE::sceneWidth - 289.0f,XEE::sceneY);
	decreaseAObject(&m_mutiList);
	m_mutiList.setSize(0.5f,0.5f);
	m_mutiList.setTileStr("ID",0);
	m_mutiList.setRowWidth(80,0);
	m_mutiList.setTileStr("Type",1);
	m_mutiList.setRowWidth(100,1);
	m_mutiList.setTileStr("State",2);
	m_mutiList.setRowWidth(100,2);
	m_mutiList.setTileStr("Position",3);
	m_mutiList.setRowWidth(120,3);
	m_mutiList.setTileStr("Size",4);
	m_mutiList.setRowWidth(150,4);
	m_mutiList.setTileStr("Angle",5);
	m_mutiList.setRowWidth(150,5);

	m_mutiList.setCallbackFun(funObjectSelectChange,funObjectStateChange,this);

	m_ctrlKeyState = KEY_STATE_UP;
	m_isInited = XTrue;
	return XTrue;
}
_XObjectManager::~_XObjectManager()
{
	//printf("����������Ѿ�����!\n");
	m_isInited = XFalse;
	release();
}
void _XObjectManager::getNowChooseObjectID(int *ID) const
{
	if(!m_isInited) return;
	if(ID == NULL) return;
	int ret = 0;
	for(int i = 0;i < m_nowObjectSum;++ i)
	{
		if(m_objBeSelect[i]) 
		{
			ID[ret] = i;
			++ ret;
		}
	}
}

int _XObjectManager::findObjectID(const void * object) const
{
//	if(!m_isInited) return -1;
	for(int i = 0;i < m_nowObjectSum;++ i)
	{
		if(m_pObject[i] == object) return i;
	}
	return -1;
}
