//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XMouseRightButtonMenu.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XMouseRightButtonMenu::_XMouseRightButtonMenu()
:m_isInited(0)
,m_menu(NULL)				//��ť��ָ��
,m_menuSum(0)				//��ť������
,m_nowChoose(-1)			//��ǰѡ���ֵ��-1ʲôҲû��ѡ��//��ʹ�ü���ѡ��˵�ʱ���ֵ����Ч
,m_lastChoose(-1)			//����ѡ���ֵ
,m_funChooseChange(NULL)	//�ı�ѡֵʱ����
,m_funChooseOver(NULL)		//����ȷ��ѡֵʱ����
{
}

_XMouseRightButtonMenu::~_XMouseRightButtonMenu()
{
	release();
}

inline void _XMouseRightButtonMenu::release()	//�ͷ���Դ
{
	if(m_isInited == 0) return;
	XDELETE_ARRAY(m_menu);
	_XControlManager::GetInstance().decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	m_isInited = 0;
}

int _XMouseRightButtonMenu::init(int menuSum,	//�˵��е��������
		const _XVector2& position,	//�˵���λ��
		const _XRect& Area,	//�˵���������Ӧ��Χ
		const _XMouseRightButtonMenuTexture &tex,	//�˵�����ͼ
		const _XFontUnicode &font,float captionSize, const _XVector2& textPosition)		//�˵�������
{
	if(m_isInited != 0) return 0;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return 0;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	if(tex.buttonNormal == NULL || tex.buttonOn == NULL) return 0;//�⼸����ͼ�Ǳ���Ҫ�е�
	if(menuSum <= 0) return 0;	//û�в˵���ĳ�ʼ����ʧ�ܵ�
	m_position = position;
	m_mouseRect = Area;

	m_size.set(1.0f,1.0f);
	m_menuSum = menuSum;
	m_allArea.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_size.y);
	m_upMousePoint.set(m_position.x + (m_mouseRect.left + m_mouseRect.getWidth() * 0.5f) * m_size.x,
		m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * 0.5f) * m_size.y);	//Ĭ�ϳ�ʼλ��Ϊ��һ����ť�����м�

	//���ø����˵���
	try
	{
		m_menu = new _XButton[m_menuSum];
		if(m_menu == NULL) return 0;	//�ڴ����ʧ����ֱ�ӷ���ʧ��
	}catch(...)
	{
		return 0;
	}	

	for(int i = 0;i < m_menuSum;++ i)
	{
		if(m_menu[i].init(_XVector2(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_size.y),m_mouseRect,tex," ",font,captionSize,textPosition) == 0)
		{
			XDELETE_ARRAY(m_menu);
			return 0;
		}
		m_menu[i].setSize(m_size);
	//����Щ����������������ע����
		_XControlManager::GetInstance().decreaseAObject(&(m_menu[i]));	//ע��������
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_menu[i]);
#endif
	}
	m_nowChoose = -1;
	m_lastChoose = -1;		//����ѡ���ֵ

	m_isVisiable = 0;
	m_isEnable = 1;
	m_isActive = 1;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MOUSERIGHTBUTTONMENU);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = 1;
	return 1;
}

void _XMouseRightButtonMenu::draw()
{
	if(m_isInited == 0) return;		//û�г�ʼ��ʱ����ʾ
	if(m_isVisiable == 0) return;	//���ɼ�ʱ����ʾ
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].draw();
	}
}

void _XMouseRightButtonMenu::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_allArea.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_size.y);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setPosition(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_size.y);
	}
	m_upMousePoint.set(m_position.x + (m_mouseRect.left + m_mouseRect.getWidth() * 0.5f) * m_size.x,
		m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * 0.5f) * m_size.y);	//Ĭ�ϳ�ʼλ��Ϊ��һ����ť�����м�
	m_nowChoose = -1;
}

void _XMouseRightButtonMenu::setSize(float x,float y)
{
	if(x <= 0 && y <= 0) return;
	m_size.set(x,y);
	m_allArea.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * m_menuSum) * m_size.y);
	for(int i = 0;i < m_menuSum;++ i)
	{
		m_menu[i].setPosition(m_position.x,m_position.y + m_mouseRect.getHeight() * i * m_size.y);
		m_menu[i].setSize(m_size);
	}
	m_upMousePoint.set(m_position.x + (m_mouseRect.left + m_mouseRect.getWidth() * 0.5f) * m_size.x,
		m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * 0.5f) * m_size.y);	//Ĭ�ϳ�ʼλ��Ϊ��һ����ť�����м�
	m_nowChoose = -1;
}

int _XMouseRightButtonMenu::canGetFocus(float x,float y)
{
	if(m_isInited == 0) return 0;	//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 0;		//û�м���Ŀؼ������տ���
	if(m_isVisiable == 0) return 0;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}

int _XMouseRightButtonMenu::canLostFocus(float x,float y)
{
	return 1;
}

int _XMouseRightButtonMenu::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(m_isInited == 0) return 0;	//���û�г�ʼ��ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�

	if(m_isVisiable == 0)
	{//û����ʾ��ʱ����Ҫͨ���������Ҽ�������Ƿ񵯳��˵�
		if(mouseState == _XMOUSE_RIGHT_BUTTON_UP)
		{//�Ҽ���������ʾ�˵�
			m_isVisiable = 1;
			setPosition(x,y);
		}
	}else
	{//��ʾʱ�԰�����������Ӧ
		for(int i = 0;i < m_menuSum;++ i)
		{
			m_menu[i].mouseProc(x,y,mouseState);
		}
		if(m_allArea.isInRect(x,y))
		{
			if(mouseState == _XMOUSE_LEFT_BUTTON_DOWN || mouseState == _XMOUSE_RIGHT_BUTTON_DOWN)
			{
				m_lastChoose = (y - m_position.y - m_mouseRect.top * m_size.y) / (m_mouseRect.getHeight() * m_size.y);
				if(m_funChooseOver != NULL) (* m_funChooseOver)(m_pClass,m_objectID);
				//�ָ����а�����״̬Ϊ��ͨ״̬
				for(int i = 0;i < m_menuSum;++ i)
				{
					m_menu[i].setState(_XBUTTON_STATE_NORMAL);
				}
				m_isVisiable = 0;
			}
		//	m_upMousePoint.set(x,y);
			//m_upMousePoint.y = (int)((y - m_objRect.top) / m_objRect.getHeight() * m_size.y) * m_objRect.getHeight() * m_size.y + m_objRect.getHeight() * m_size.y * 0.5;
		}else
		{//������Ĳ���
			if(mouseState == _XMOUSE_LEFT_BUTTON_DOWN || mouseState == _XMOUSE_RIGHT_BUTTON_DOWN)
			{
				m_lastChoose = -1;
				if(m_funChooseOver != NULL) (* m_funChooseOver)(m_pClass,m_objectID);
				m_isVisiable = 0;
			}
		}
	}
	return 1;
}

int _XMouseRightButtonMenu::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(m_isInited == 0) return 0;		//���û�г�ʼ��ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�
	if(m_isVisiable == 0) return 0;	//���̲���ֻ������ʾ������²�����ʾ
	if(keyState == _XKEY_STATE_UP)
	{
		if(keyOrder == SDLK_UP)
		{//���ϼ�
			-- m_nowChoose;
			if(m_nowChoose < 0) m_nowChoose = m_menuSum -1;
			m_upMousePoint.y = m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * (m_nowChoose + 0.5f)) * m_size.y;

		//	m_upMousePoint.y -= m_objRect.getHeight() * m_size.y;
		//	if(m_upMousePoint.y < m_allArea.top)
		//	{
		//		m_upMousePoint.y += m_allArea.getHeight();
		//	}
			mouseProc(m_upMousePoint.x,m_upMousePoint.y,_XMOUSE_MOVE);
		}else
		if(keyOrder == SDLK_DOWN)
		{//���¼�
			++ m_nowChoose;
			if(m_nowChoose >= m_menuSum) m_nowChoose = 0;
			m_upMousePoint.y = m_position.y + (m_mouseRect.top + m_mouseRect.getHeight() * (m_nowChoose + 0.5f)) * m_size.y;

		//	m_upMousePoint.y += m_objRect.getHeight() * m_size.y;
		//	if(m_upMousePoint.y > m_allArea.bottom)
		//	{
		//		m_upMousePoint.y -= m_allArea.getHeight();
		//	}
			mouseProc(m_upMousePoint.x,m_upMousePoint.y,_XMOUSE_MOVE);
		}else
		if(keyOrder ==  SDLK_RETURN)
		{//�س���
			for(int i = 0;i < m_menuSum;++ i)
			{
				if(m_menu[i].getState() == _XBUTTON_STATE_ON)
				{
					m_lastChoose = i;
					if(m_funChooseOver != NULL) (* m_funChooseOver)(m_pClass,m_objectID);
					m_isVisiable = 0;
					return 1;
				}
			}
		}else
		{
			for(int i = 0;i < m_menuSum;++ i)
			{
				if(keyOrder == m_menu[i].getHotKey() && m_menu[i].getState() != _XBUTTON_STATE_DISABLE)
				{//���������˵��Ŀ�ݼ���ͬ�����Ҳ˵���״̬������Ч
					m_lastChoose = i;
					if(m_funChooseOver != NULL) (* m_funChooseOver)(m_pClass,m_objectID);
					m_isVisiable = 0;
					return 1;
				}
			}
		}
	}
	return 1;
}

void _XMouseRightButtonMenu::setNowChoose(int temp)
{
}

int _XMouseRightButtonMenu::setACopy(const _XMouseRightButtonMenu &temp)
{
	if(& temp == this) return 1;	//��ֹ����ֵ
	if(temp.m_isInited == 0) return 0;	//���Ŀ��û�г�ʼ����ֱ�ӷ���
	if(m_isInited != 0)	release();//��������Ѿ���ʼ��,�ͷŵ���Ӧ����Դ
	if(_XControlBasic::setACopy(temp) == 0) return 0;
	if(m_isInited == 0)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MOUSERIGHTBUTTONMENU);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}
	m_isInited = temp.m_isInited;

	//���ø����˵���
	m_menuSum = temp.m_menuSum;		//��ť������
	try
	{
		m_menu = new _XButton[m_menuSum];
		if(m_menu == NULL) return 0;	//�ڴ����ʧ����ֱ�ӷ���ʧ��
	}catch(...)
	{
		return 0;
	}
	for(int i = 0;i < m_menuSum;++ i)
	{
		if(m_menu[i].setACopy(temp.m_menu[i]) == 0) return 0;
		//����Щ�ؼ��ӿؼ���������ע����
		_XControlManager::GetInstance().decreaseAObject(&(m_menu[i]));	//ע��������
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_menu[i]);
#endif
	}

	m_nowChoose = temp.m_nowChoose;		//��ǰѡ���ֵ��-1ʲôҲû��ѡ��//��ʹ�ü���ѡ��˵�ʱ���ֵ����Ч
	m_lastChoose = temp.m_lastChoose;		//����ѡ���ֵ

	m_funChooseChange = temp.m_funChooseChange;	//�ı�ѡֵʱ����
	m_funChooseOver = temp.m_funChooseOver;		//����ȷ��ѡֵʱ����

	m_allArea = temp.m_allArea;		//��궯������Ӧ����
	m_upMousePoint = temp.m_upMousePoint;	//�ϴμ�¼������ڷ�Χ�ڵ�λ��

	return 1;
}

int _XMouseRightButtonMenu::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}

_XVector2 _XMouseRightButtonMenu::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;
	if(order == 0) ret.set(m_allArea.left,m_allArea.top);else
	if(order == 1) ret.set(m_allArea.right,m_allArea.top);else
	if(order == 2) ret.set(m_allArea.right,m_allArea.bottom);else
	if(order == 3) ret.set(m_allArea.left,m_allArea.bottom);
	return ret;
}