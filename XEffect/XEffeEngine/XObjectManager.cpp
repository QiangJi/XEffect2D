//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XObjectManager.h"
#include "XSprite.h"
#include "XFrameEx.h"
#include "XBasicSDL.h"
#include "XNodeLine.h"
#include "./XControl/XControlManager.h"

void funObjectSelectChange(void * pClass,int objectID)
{
	printf("select change! %d\n",((_XObjectManager *)pClass)->m_mutiList.getSelectOrder());
}

void funObjectStateChange(void * pClass,int objectID)
{
	printf("check change!\n");
	_XObjectManager * par = (_XObjectManager *)pClass;
	//�������״̬�ĸı�
	int tempChooseOBJSum = par->getNowChooseObjectSum();
	for(int i = 0;i < tempChooseOBJSum;++ i)
	{
		if(par->m_lineObjOrder[i] >= 0)
		{
			if(par->m_mutiList.getCheckState(1,i)) par->m_objCanEdit[par->m_lineObjOrder[i]] = 1;
			else 
			{
				par->m_objCanEdit[par->m_lineObjOrder[i]] = 0;
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
:m_isInited(0)
,m_isShowUI(1)
,m_isOption(1)
{
	//put single object into auto_ptr object 
	//m_auto_ptr = auto_ptr<_XObjectManager>(this);
	for(int i = 0;i < MAX_OBJECT_SUM;++ i)
	{
		m_objectState[i] = OBJ_STATE_NULL;		//���״̬��Ч
		m_pObject[i] = NULL;		//��ʼ��ָ����Ч
		m_objectType[i] = OBJ_NULL;	//��Ч���������
		m_objectKeyOption[i] = OBJ_OPTION_NULL;
		m_objectSetState[i] = OBJ_OPTION_STATE_NULL;
		m_objLineOrder[i] = -1;
		m_lineObjOrder[i] = -1;
		m_objCanEdit[i] = 1;	//��ʼ�����е�������ǿ��Ա༭��
	}
	m_nowObjectSum = 0;
	m_nowMouseOnObjectSum = 0;
	//�����ǹ��ڰ������������ĳ�ʼ��������
	m_keyMaxTime = 500;
	m_keyMinTime = 10;
	for(int i = 0;i < 4;++ i)
	{
		m_keyState[i] = _XKEY_STATE_UP;
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
//	if(m_isInited == 0) return -1;
	if(m_nowObjectSum >= MAX_OBJECT_SUM) return -1;	//����Ѿ����ˣ�ע��ʧ��
	if(type == OBJ_NULL) return -1;	//��Ч���������ע��
	if(object == NULL) return -1; 
	int ret = findObjectID(object);
	if(ret >= 0) return ret;	//�������Ѿ�ע�ᣬ���ֹ�ظ�ע��
	//���濪ʼע�ᶯ��
	m_objectState[m_nowObjectSum] = OBJ_STATE_NULL;		//Ĭ��״̬
	m_objectUpState[m_nowObjectSum] = OBJ_STATE_NULL;		//Ĭ��״̬
	m_pObject[m_nowObjectSum] = object;		//��ʼ��ָ����Ч
	m_objectType[m_nowObjectSum] = type;	//��Ч���������
	m_objectMousePoint[m_nowObjectSum].set(0.0f,0.0f);
	m_objectKeyOption[m_nowObjectSum] = OBJ_OPTION_NULL;
	m_objectSetState[m_nowObjectSum] = OBJ_OPTION_STATE_NULL;
	
	m_objectEditParm[m_nowObjectSum] = -1;
	
	++ m_nowObjectSum;
	//printf("ObjectSum:%d\n",m_nowObjectSum);
	return m_nowObjectSum - 1;
}

void _XObjectManager::decreaseAObject(int objectID)
{
//	if(m_isInited == 0) return;
	if(objectID < 0 || objectID >= m_nowObjectSum) return;	//��������Ƿ�
	if(m_nowObjectSum <= 0) return;	//���������û�������ֱ�ӷ���
	//�������г�ȡ���ע�ᣬ��������������
	if(objectID == m_nowObjectSum - 1)
	{//�Ѿ������һ������
		m_objectState[objectID] = OBJ_STATE_NULL;		//Ĭ��״̬
		m_objectUpState[objectID] = OBJ_STATE_NULL;
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
			m_objectState[i] = m_objectState[i + 1];	//Ĭ��״̬
			m_objectUpState[i] = m_objectUpState[i + 1];
			m_pObject[i] = m_pObject[i + 1];			//��ʼ��ָ����Ч
			m_objectType[i] = m_objectType[i + 1];		//��Ч���������
			m_objectMousePoint[i] = m_objectMousePoint[i + 1];
			m_objectKeyOption[i] = m_objectKeyOption[i + 1];
			m_objectSetState[i] = m_objectSetState[i + 1];
		}
		m_objectState[m_nowObjectSum - 1] = OBJ_STATE_NULL;			//Ĭ��״̬
		m_objectUpState[m_nowObjectSum - 1] = OBJ_STATE_NULL;			//Ĭ��״̬
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
	if(m_isInited == 0) return;	//���û�г�ʼ���Ļ�ֱ�ӷ���
	if(m_nowObjectSum == 0) return;	//���û�����ֱ�ӷ���
	if(keyID == SDLK_RCTRL && keyState == _XKEY_STATE_UP)
	{
		if(m_isOption == 0) setOption();
		else disOption();
	}
	if(m_isOption == 0) return;
	switch(keyID)
	{
	case SDLK_LCTRL:	//�Ƿ���ʾUI����
		if(keyState == _XKEY_STATE_DOWN)
		{
		}else
		{
			if(m_isShowUI == 0) setShow();
			else disShow();
		}
		break;
	case SDLK_q:	//ȥ�����в���
		for(int i = 0;i < m_nowObjectSum;++ i)
		{
			m_objectKeyOption[i] = OBJ_OPTION_NULL;
		}
		break;
	case SDLK_e:	//nodeline�������༭ת̨
		if(keyState == _XKEY_STATE_DOWN)
		{
		}else
		{
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objectType[i] == OBJ_NODELINE && m_objectState[i] == OBJ_STATE_BE_CHOOSE && m_objCanEdit[i] != 0)
				{
					if(m_objectKeyOption[i] != OBJ_OPTION_EDIT)
					{
						m_objectKeyOption[i] = OBJ_OPTION_EDIT;
						printf("NodeLine Length:%f\n",((_XNodeLine *)m_pObject[i])->getNodeLineLength());
					}else
					{
						m_objectKeyOption[i] = OBJ_OPTION_NULL;
					}
				}
			}
		}
		break;
	case SDLK_s:	//����
		if(keyState == _XKEY_STATE_DOWN)
		{//����
		}else
		{//����,��ʱ�������б�ѡ�е�����������Ų���
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objectState[i] == OBJ_STATE_BE_CHOOSE && m_objCanEdit[i] != 0)
				{
					if(m_objectType[i] == OBJ_NODELINE && m_objectKeyOption[i] == OBJ_OPTION_EDIT)
					{//���ﱣ�����ߵ��ļ���
						((_XNodeLine *)m_pObject[i])->saveNodeLine();
					}else
					{
						m_objectKeyOption[i] = OBJ_OPTION_SIZE;
					}
				}
			}
		}
		break;
	case SDLK_r:	//��ת
		if(keyState == _XKEY_STATE_DOWN)
		{//����
		}else
		{//����
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objectState[i] == OBJ_STATE_BE_CHOOSE && m_objCanEdit[i] != 0)
				{
					m_objectKeyOption[i] = OBJ_OPTION_ROTATE;
				}
			}
		}
		break;
	case SDLK_m:	//�ƶ�
		if(keyState == _XKEY_STATE_DOWN)
		{//����
		}else
		{//����
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objectState[i] == OBJ_STATE_BE_CHOOSE && m_objCanEdit[i] != 0)
				{
					m_objectKeyOption[i] = OBJ_OPTION_POSITION;
				}
			}
		}
		break;
	case SDLK_d:
		if(keyState == _XKEY_STATE_DOWN)
		{//����
		}else
		{//����
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objectState[i] == OBJ_STATE_BE_CHOOSE && m_objectKeyOption[i] == OBJ_OPTION_EDIT && m_objCanEdit[i] != 0)
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
	case SDLK_a://�����������ڵ�ǰλ������һ����
		if(keyState == _XKEY_STATE_DOWN)
		{//����
		}else
		{//����
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objectState[i] == OBJ_STATE_BE_CHOOSE && m_objectKeyOption[i] == OBJ_OPTION_EDIT && m_objCanEdit[i] != 0)
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
	case SDLK_c:	//�ı���ת��׼��
		if(keyState == _XKEY_STATE_DOWN)
		{//����
		}else
		{//����
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objectState[i] == OBJ_STATE_BE_CHOOSE && m_objCanEdit[i] != 0)
				{
					m_objectKeyOption[i] = OBJ_OPTION_ROTATEMODE;
				}
			}
		}
		break;
	case SDLK_t:	//���÷�ת״̬
		if(keyState == _XKEY_STATE_DOWN)
		{//����
		}else
		{//����
			for(int i = 0;i < m_nowObjectSum;++ i)
			{
				if(m_objectState[i] == OBJ_STATE_BE_CHOOSE && m_objCanEdit[i] != 0)
				{
					m_objectKeyOption[i] = OBJ_OPTION_TURNOVERMODE;
				}
			}
		}
		break;
	case SDLK_LEFT:
		if(keyState == _XKEY_STATE_DOWN)
		{//����
			m_keyState[0] = _XKEY_STATE_DOWN;
			m_keyTimer[0] = 0;
			m_keyNowTime[0] = m_keyMaxTime;
		}else
		{//������������������Ӧ�Ĳ���
			m_keyState[0] = _XKEY_STATE_UP;
		}
		break;
	case SDLK_RIGHT:
		if(keyState == _XKEY_STATE_DOWN)
		{//����
			m_keyState[1] = _XKEY_STATE_DOWN;
			m_keyTimer[1] = 0;
			m_keyNowTime[1] = m_keyMaxTime;
		}else
		{//����
			m_keyState[1] = _XKEY_STATE_UP;
		}
		break;
	case SDLK_UP:
		if(keyState == _XKEY_STATE_DOWN)
		{//����
			m_keyState[2] = _XKEY_STATE_DOWN;
			m_keyTimer[2] = 0;
			m_keyNowTime[2] = m_keyMaxTime;
		}else
		{//����
			m_keyState[2] = _XKEY_STATE_UP;
		}
		break;
	case SDLK_DOWN:
		if(keyState == _XKEY_STATE_DOWN)
		{//����
			m_keyState[3] = _XKEY_STATE_DOWN;
			m_keyTimer[3] = 0;
			m_keyNowTime[3] = m_keyMaxTime;
		}else
		{//����
			m_keyState[3] = _XKEY_STATE_UP;
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
				if(mode == _TURN_OVER_MODE_NULL) temp->setLeft2Right(); else
				if(mode == _TURN_OVER_MODE_LEFT_TO_RIGHT) temp->setUp2Down();
				else temp->resetLeftRightUpDown();
			}
			break;
		//case OBJ_FRAMEEX:
		//	{
		//		_XFrameEx * temp = (_XFrameEx *)m_pObject[order];
		//		_XTurnOverMode mode = temp->getTurnOverMode();
		//		if(mode == _TURN_OVER_MODE_NULL) temp->setLeft2Right(); else
		//		if(mode == _TURN_OVER_MODE_LEFT_TO_RIGHT) temp->setUp2Down();
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
	if(m_isInited == 0) return;	//���û�г�ʼ���Ļ�ֱ�ӷ���
	if(m_isOption == 0) return;
	m_nowMouseOnObjectSum = 0;
	bool stateChangeFlag = false;	//ѡ���״̬�Ƿ����˸ı�
	if(m_isShowUI != 0 && m_mutiList.isInRect(x,y) != 0) return;	//��ʾ�ռ��ʱ����Ϣ�������´���

	switch(eventType)
	{
	case _XMOUSE_LEFT_BUTTON_DOWN:	//������µĶ���
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
					{//���������ϱ������������Ӧ
						m_objectUpState[i] = m_objectState[i];
						m_objectState[i] = OBJ_STATE_MOUSE_DOWN;
						m_objectMousePoint[i] = temp->getPosition();
						m_objectMousePoint[i].x -= x;
						m_objectMousePoint[i].y -= y;
						if(m_objectType[i] == OBJ_NODELINE)
						{
							int tempOrder = ((_XNodeLine *)m_pObject[i])->getNodeOrder(x,y,5);
							if(tempOrder >= 0 && tempOrder != m_objectEditParm[i])
							{
								m_objectEditParm[i] = tempOrder;
							}
						}
					}
				}
				break;
			case OBJ_NODELINE:
				{
					_XObjectBasic * temp = (_XObjectBasic *)m_pObject[i];
					if(temp->isInRect(x,y) != 0)
					{//���������ϱ������������Ӧ
						if(m_objectKeyOption[i] == OBJ_OPTION_EDIT)
						{
							_XNodeLine *tempNodeLine = (_XNodeLine *)m_pObject[i];
							int tempOrder = -1;
							if(tempNodeLine->getSize().x != 0 && tempNodeLine->getSize().y != 0)
							{//��ֹ�������
								tempOrder = tempNodeLine->getNodeOrder((x - tempNodeLine->getPosition().x) / tempNodeLine->getSize().x,
									(y - tempNodeLine->getPosition().y) / tempNodeLine->getSize().y,5);
								if(tempOrder >= 0)// && tempOrder != m_objectEditParm[i])
								{
									m_objectEditParm[i] = tempOrder;

									m_objectUpState[i] = m_objectState[i];
									m_objectState[i] = OBJ_STATE_MOUSE_DOWN;
									m_objectMousePoint[i] = tempNodeLine->getNode(tempOrder);
									m_objectMousePoint[i].x -= x / tempNodeLine->getSize().x;
									m_objectMousePoint[i].y -= y / tempNodeLine->getSize().y;
								}
							}
						}else
						{
							m_objectUpState[i] = m_objectState[i];
							m_objectState[i] = OBJ_STATE_MOUSE_DOWN;
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
	case _XMOUSE_LEFT_BUTTON_UP:		//���������
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
				//	_XObjectBasic * temp = (_XObjectBasic *)m_pObject[i];
				//	if(temp->isInRect(x,y) != 0)
					if(m_objectState[i] == OBJ_STATE_MOUSE_DOWN)	//ֻҪ��굯�𣬲����Ƿ��������λ�ö�����Ӧ�ж�
					{//���������ϱ������������Ӧ
						if(m_objectUpState[i] == OBJ_STATE_NULL || m_objectUpState[i] == OBJ_STATE_MOUSE_ON) 
						{
							m_objectState[i] = OBJ_STATE_BE_CHOOSE;
						}else 
						{//ȡ��ѡ��Ҳ��ȡ�����ļ��̶���
							if(m_objectType[i] == OBJ_NODELINE)
							{
								if(m_objectKeyOption[i] != OBJ_OPTION_EDIT)
								{
									m_objectState[i] = OBJ_STATE_NULL;
									m_objectKeyOption[i] = OBJ_OPTION_NULL;
								}else
								{
									m_objectState[i] = OBJ_STATE_BE_CHOOSE;
								}
							}else
							{
								m_objectState[i] = OBJ_STATE_NULL;
								m_objectKeyOption[i] = OBJ_OPTION_NULL;
							}
						}
						stateChangeFlag = true;
					}else
					if(m_objectState[i] == OBJ_STATE_MOUSE_MOVE)
					{//�������϶�״̬����
						m_objectState[i] = OBJ_STATE_BE_CHOOSE;
						stateChangeFlag = true;
					}
				}
				break;
			default:
				break;
			}
		}
		break;
	case _XMOUSE_MOVE:		//����ƶ��¼�
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
				if(m_objectState[i] == OBJ_STATE_NULL)	//�����¼����ж�
				{
					_XObjectBasic * temp = (_XObjectBasic *)m_pObject[i];
					if(temp->isInRect(x,y) != 0)
					{
						m_objectState[i] = OBJ_STATE_MOUSE_ON;	//���������¼�
						m_nowMouseOnObjectSum ++;
					}
				}else
				if(m_objectState[i] == OBJ_STATE_MOUSE_ON)	//����Ѿ���������״̬���������ж��Ƕ�����״̬����
				{
					_XObjectBasic * temp = (_XObjectBasic *)m_pObject[i];
					if(temp->isInRect(x,y) == 0) m_objectState[i] = OBJ_STATE_NULL;	//��������״̬����
				}else
				if((m_objectState[i] == OBJ_STATE_MOUSE_DOWN || m_objectState[i] == OBJ_STATE_MOUSE_MOVE) && m_objCanEdit[i] != 0)	//���״̬�Ļ����������ø�������ƶ�
				{
					if(m_objectType[i] != OBJ_NODELINE)
					{
						_XObjectBasic * temp = (_XObjectBasic *)m_pObject[i];
						temp->setPosition(m_objectMousePoint[i].x + x,m_objectMousePoint[i].y + y);
					}else
					{
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
					m_objectState[i] = OBJ_STATE_MOUSE_MOVE;	//����϶�״̬
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
		m_mutiList.clearAllSelect();
		m_mutiList.setLineSum(0);
		char tempStr[256] = "";
		int lineOrder = 0;
		for(int i = 0;i < m_nowObjectSum;++ i)
		{
			if(m_objectState[i] != OBJ_STATE_NULL)
			{
				m_mutiList.addALine();
				sprintf(tempStr,"%d",i);
				m_mutiList.setBoxStr(tempStr,lineOrder,0);
				sprintf(tempStr,"x:%.0f,y:%.0f",m_objInfo[i].m_position.x,m_objInfo[i].m_position.y);
				m_mutiList.setBoxStr(tempStr,lineOrder,1);
				sprintf(tempStr,"x:%.2f,y:%.2f",m_objInfo[i].m_size.x,m_objInfo[i].m_size.y);
				m_mutiList.setBoxStr(tempStr,lineOrder,2);
				sprintf(tempStr,"%.2f",m_objInfo[i].m_angle);
				m_mutiList.setBoxStr(tempStr,lineOrder,3);
				m_objLineOrder[i] = lineOrder;
				m_lineObjOrder[lineOrder] = i;
				//�����������Ŀɱ༭״̬
				if(m_objCanEdit[i] == 0) m_mutiList.setCheckState(1,lineOrder,false);
				else m_mutiList.setCheckState(1,lineOrder,true); 

				if(((_XObjectBasic *)m_pObject[i])->getVisiable()) m_mutiList.setCheckState(0,lineOrder,true);
				else m_mutiList.setCheckState(0,lineOrder,false); 
				lineOrder ++;
			}else
			{
				m_objLineOrder[i] = -1;
			}
		}
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
//	char tempStr[100];
//	if(m_objectType[order] == OBJ_NODELINE)
//	{
//		m_objInfo[order].length = ((_XNodeLine *)m_pObject[order])->getNodeLineLength();
//		sprintf(tempStr," x:%.0f,y:%.0f\n len:%.1f\n size:x %.2f y %.2f",m_objInfo[order].m_position.x,m_objInfo[order].m_position.y,
//			m_objInfo[order].length,m_objInfo[order].m_size.x,m_objInfo[order].m_size.y);
//	}else
//	if(m_objectType[order] == OBJ_CONTROL)
//	{
//		sprintf(tempStr," x:%.0f,y:%.0f\n size:x %.2f y %.2f",m_objInfo[order].m_position.x,m_objInfo[order].m_position.y,
//			m_objInfo[order].m_size.x,m_objInfo[order].m_size.y);
//	}else
//	{
//		sprintf(tempStr," x:%.0f,y:%.0f\n angle:%.1f\n size:x %.2f y %.2f",m_objInfo[order].m_position.x,m_objInfo[order].m_position.y,
//			m_objInfo[order].m_angle,m_objInfo[order].m_size.x,m_objInfo[order].m_size.y);
//	}
//	m_font[order].setString(tempStr);
//
//	_XVector2 fontPosition = m_objInfo[order].m_position;
//
//	if(fontPosition.x < 0.0f) fontPosition.x = 0.0f;	//��ֹ������ʾ������Ļ��Χ����������û���ж����ޣ�ֻ�ж�������
//	if(fontPosition.x >= XEE_windowWidth - m_font[order].getMaxPixelWidth()) fontPosition.x = XEE_windowWidth - m_font[order].getMaxPixelWidth();
//	if(fontPosition.y < 0.0f) fontPosition.y = 0.0f;
//	if(fontPosition.y >= XEE_windowHeight - m_font[order].getMaxPixelHeight()) fontPosition.y = XEE_windowHeight - m_font[order].getMaxPixelHeight();
//	m_font[order].setPosition(fontPosition);

	if(m_objLineOrder[order] < 0) return;
	char tempStr[256] = "";
	sprintf(tempStr,"%d",order);
	m_mutiList.setBoxStr(tempStr,m_objLineOrder[order],0);
	sprintf(tempStr,"x:%.0f,y:%.0f",m_objInfo[order].m_position.x,m_objInfo[order].m_position.y);
	m_mutiList.setBoxStr(tempStr,m_objLineOrder[order],1);
	sprintf(tempStr,"x:%.2f,y:%.2f",m_objInfo[order].m_size.x,m_objInfo[order].m_size.y);
	m_mutiList.setBoxStr(tempStr,m_objLineOrder[order],2);
	sprintf(tempStr,"%.2f",m_objInfo[order].m_angle);
	m_mutiList.setBoxStr(tempStr,m_objLineOrder[order],3);
}

bool _XObjectManager::checkNeedUpdate(int order)
{
	_XObjectBasic * temp = (_XObjectBasic *)m_pObject[order];
	if(m_objInfo[order].m_position.x != temp->getPosition().x || m_objInfo[order].m_position.y != temp->getPosition().y) return true;
	if(m_objInfo[order].m_size.x != temp->getSize().x || m_objInfo[order].m_size.y != temp->getSize().y) return true;
	if(m_objInfo[order].m_angle != temp->getAngle()) return true;
	if(m_objectType[order] == OBJ_NODELINE && m_objInfo[order].length != ((_XNodeLine *)m_pObject[order])->getNodeLineLength()) return true;
	return false;
}

void _XObjectManager::draw()	//�����������̫��ʱ�����������ִ��Ч�ʽϵ�
{//���
	if(m_isInited == 0) return;	//���û�г�ʼ���Ļ�ֱ�ӷ���
	if(m_isOption == 0) return;
	for(int i = 0;i < m_nowObjectSum;++ i)
	{
		if(m_objectState[i] != OBJ_STATE_NULL)	//�ǲ���ͨ״̬
		{//�����������ѡ������ʾ�����Ϣ
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
					if(m_objectState[i] == OBJ_STATE_BE_CHOOSE || m_objectState[i] == OBJ_STATE_MOUSE_MOVE)
					{//��ͨѡ��״̬
						if(m_objCanEdit[i] != 0)
						{
							drawLine(temp->getBox(0),temp->getBox(1));
							drawLine(temp->getBox(1),temp->getBox(2));
							drawLine(temp->getBox(2),temp->getBox(3));
							drawLine(temp->getBox(3),temp->getBox(0));
						}else
						{
							drawLine(temp->getBox(0),temp->getBox(1),1,0,0,0);
							drawLine(temp->getBox(1),temp->getBox(2),1,0,0,0);
							drawLine(temp->getBox(2),temp->getBox(3),1,0,0,0);
							drawLine(temp->getBox(3),temp->getBox(0),1,0,0,0);
						}
						//������ʾ�����λ��
						//m_font[i].draw();
					}else
					if(m_objectState[i] == OBJ_STATE_MOUSE_DOWN)
					{//��갴��״̬
						drawLine(temp->getBox(0),temp->getBox(1),1,1.0f,0.0f,0.0f,1.0f);
						drawLine(temp->getBox(1),temp->getBox(2),1,1.0f,0.0f,0.0f,1.0f);
						drawLine(temp->getBox(2),temp->getBox(3),1,1.0f,0.0f,0.0f,1.0f);
						drawLine(temp->getBox(3),temp->getBox(0),1,1.0f,0.0f,0.0f,1.0f);
					}else
					if(m_objectState[i] == OBJ_STATE_MOUSE_ON)
					{//�������״̬
						drawLine(temp->getBox(0),temp->getBox(1),1,1.0f,1.0f,0.0f,1.0f);
						drawLine(temp->getBox(1),temp->getBox(2),1,1.0f,1.0f,0.0f,1.0f);
						drawLine(temp->getBox(2),temp->getBox(3),1,1.0f,1.0f,0.0f,1.0f);
						drawLine(temp->getBox(3),temp->getBox(0),1,1.0f,1.0f,0.0f,1.0f);
					}
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
					if(m_objectState[i]  == OBJ_STATE_BE_CHOOSE || m_objectState[i]  == OBJ_STATE_MOUSE_MOVE)
					{//�༭״̬
						if(m_objectKeyOption[i] == OBJ_OPTION_EDIT)
						{
							drawLine(temp->getBox(0),temp->getBox(1));
							drawLine(temp->getBox(1),temp->getBox(2));
							drawLine(temp->getBox(2),temp->getBox(3));
							drawLine(temp->getBox(3),temp->getBox(0));
						}
						//m_font[i].draw();
					}
				}
				break;
			}
		}
	}
}

void _XObjectManager::move(int delay)
{
	if(m_isInited == 0) return;
	if(m_isOption == 0) return;
	if(m_keyState[0] == _XKEY_STATE_DOWN)
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
	if(m_keyState[1] == _XKEY_STATE_DOWN)
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
	if(m_keyState[2] == _XKEY_STATE_DOWN)
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
	if(m_keyState[3] == _XKEY_STATE_DOWN)
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

int _XObjectManager::init(int resoursePosition)
{
	if(m_isInited != 0) return 1;
	if(resoursePosition != 0) resoursePosition = 1;

	//��ʼ������
	if(m_font.init("pic/ObjectManager/Font/FontUnicode30_T000.png",_XVector2I(30,30),_XVector2I(34,34),4,resoursePosition) == 0) return 0;
	decreaseAObject(&(m_font));
	//��ʼ��һ���б��
	if(m_sliderTextureH.init("pic/ObjectManager/MutiList/BL_00.png",
		"pic/ObjectManager/MutiList/BL_01.png",
		"pic/ObjectManager/MutiList/BL_02.png",
		"pic/ObjectManager/MutiList/BL_03.png",
		"pic/ObjectManager/MutiList/B_00.png",
		"pic/ObjectManager/MutiList/B_01.png",
		"pic/ObjectManager/MutiList/B_02.png",
		"pic/ObjectManager/MutiList/B_03.png",resoursePosition) == 0) return 0;
	if(m_sliderH.init(_XVector2(200.0f,200.0f),_XRect(0.0f,0.0f,512.0f - 40.0f,32.0f),_XRect(0.0f,0.0f,32.0f,32.0f),
		m_sliderTextureH,_XSLIDER_TYPE_HORIZONTAL,100.0f,0.0f) == 0) return 0;
	_XControlManager::GetInstance().decreaseAObject(&m_sliderH);
	decreaseAObject(&m_sliderH);

	if(m_sliderTextureV.init("pic/ObjectManager/MutiList/BLH_00.png",
		"pic/ObjectManager/MutiList/BLH_01.png",
		"pic/ObjectManager/MutiList/BLH_02.png",
		"pic/ObjectManager/MutiList/BLH_03.png",
		"pic/ObjectManager/MutiList/BH_00.png",
		"pic/ObjectManager/MutiList/BH_01.png",
		"pic/ObjectManager/MutiList/BH_02.png",
		"pic/ObjectManager/MutiList/BH_03.png",resoursePosition) == 0) return 0;
	if(m_sliderV.init(_XVector2(200.0f,200.0f),_XRect(0.0f,0.0f,32.0f,512.0f - 40.0f),_XRect(0.0f,0.0f,32.0f,32.0f),
		m_sliderTextureV,_XSLIDER_TYPE_VERTICAL,100.0f,0.0f) == 0) return 0;
	_XControlManager::GetInstance().decreaseAObject(&m_sliderV);
	decreaseAObject(&m_sliderV);

	if(m_checkTex0.init("pic/ObjectManager/MutiList/C_00.png",
		"pic/ObjectManager/MutiList/C_01.png",
		"pic/ObjectManager/MutiList/C_02.png",
		"pic/ObjectManager/MutiList/C_03.png",resoursePosition) == 0) return 0;
	if(m_checkTex1.init("pic/ObjectManager/MutiList/C_10.png",
		"pic/ObjectManager/MutiList/C_11.png",
		"pic/ObjectManager/MutiList/C_12.png",
		"pic/ObjectManager/MutiList/C_13.png",resoursePosition) == 0) return 0;

	if(m_mutiListTexture.init("pic/ObjectManager/MutiList/E_back.png",
		"pic/ObjectManager/MutiList/E_disable.png",
		"pic/ObjectManager/MutiList/E_Select.png",
		"pic/ObjectManager/MutiList/E_move.png",
		"pic/ObjectManager/MutiList/E_Title.png",
		"pic/ObjectManager/MutiList/E_TitleEnd.png",resoursePosition) == 0) return 0;

	if(m_mutiList.init(_XVector2(XEE_windowWidth - 289.0f,0.0f),_XRect(70.0f,4.0f,574.0f,508.0f),m_mutiListTexture,m_checkTex0,m_checkTex1,m_font,1.0f,4,0,m_sliderV,m_sliderH) == 0) return 0;
	decreaseAObject(&m_mutiList);
	m_mutiList.setSize(0.5f,0.5f);
	m_mutiList.setTileStr("OBJ ID",0);
	m_mutiList.setRowWidth(100,0);
	m_mutiList.setTileStr("Position",1);
	m_mutiList.setRowWidth(120,1);
	m_mutiList.setTileStr("Size",2);
	m_mutiList.setRowWidth(150,2);
	m_mutiList.setTileStr("Angle",3);
	m_mutiList.setRowWidth(150,3);

	m_mutiList.setCallbackFun(funObjectSelectChange,funObjectStateChange,this);
	m_isInited = 1;
	return 1;
}

_XObjectManager::~_XObjectManager()
{
	//printf("����������Ѿ�����!\n");
	m_isInited = 0;
}

void _XObjectManager::getNowChooseObjectID(int *ID) const
{
	if(m_isInited == 0) return;
	if(ID == NULL) return;
	int ret = 0;
	for(int i = 0;i < m_nowObjectSum;++ i)
	{
		if(m_objectState[i] == OBJ_STATE_BE_CHOOSE) 
		{
			ID[ret] = i;
			++ ret;
		}
	}
}

int _XObjectManager::findObjectID(const void * object) const
{
//	if(m_isInited == 0) return -1;
	for(int i = 0;i < m_nowObjectSum;++ i)
	{
		if(m_pObject[i] == object) return i;
	}
	return -1;
}
