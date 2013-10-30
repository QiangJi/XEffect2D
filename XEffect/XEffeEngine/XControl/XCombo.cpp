//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XCombo.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

void funComboMenuState(void *pClass,int ID)
{
	_XCombo *pPar = (_XCombo *) pClass;
	if(pPar->m_isDrawDownMenu == 0)
	{
		pPar->m_isDrawDownMenu = 1;
		pPar->m_buttom[1].disable();
		if(pPar->m_menuSum > pPar->m_menuDrawSum) pPar->m_buttom[2].enable();
		else pPar->m_buttom[2].disable();
		pPar->m_menuStartDrawOrder = 0;
		pPar->updateString();
	}else pPar->m_isDrawDownMenu = 0;
//	pPar->m_mouseButtonDownIsUseable = 1;
}

void funComboMenuStart(void *pClass,int ID)
{
	_XCombo *pPar = (_XCombo *) pClass;
	if(pPar->m_menuStartDrawOrder > 0)
	{
		pPar->m_menuStartDrawOrder --;
		pPar->updateString();
		if(pPar->m_menuStartDrawOrder + pPar->m_menuDrawSum < pPar->m_menuSum) pPar->m_buttom[2].enable();
		if(pPar->m_menuStartDrawOrder == 0) pPar->m_buttom[1].disable();
	}
// 	pPar->m_mouseButtonDownIsUseable = 1;
}

void funComboMenuEnd(void *pClass,int ID)
{
	_XCombo *pPar = (_XCombo *) pClass;
	if(pPar->m_menuStartDrawOrder + pPar->m_menuDrawSum < pPar->m_menuSum)
	{
		pPar->m_menuStartDrawOrder ++;
		pPar->updateString();
		if(pPar->m_menuStartDrawOrder > 0) pPar->m_buttom[1].enable();
		if(pPar->m_menuStartDrawOrder + pPar->m_menuDrawSum >= pPar->m_menuSum) pPar->m_buttom[2].disable();
	}
// 	pPar->m_mouseButtonDownIsUseable = 1;
}

void funComboMenu(void *pClass,int ID)
{
	_XCombo *pPar = (_XCombo *) pClass;
	for(int i = 0;i < pPar->m_menuDrawSum;++ i)
	{
		if(pPar->m_buttom[i + 3].getControlID() == ID)
		{
			pPar->m_isDrawDownMenu = 0;
			if(pPar->m_nowChooseOrder != i + pPar->m_menuStartDrawOrder)
			{
				pPar->m_nowChooseOrder = i + pPar->m_menuStartDrawOrder;
				pPar->updateString();
				if(pPar->m_funDataChange != NULL) pPar->m_funDataChange(pPar->m_pClass,pPar->m_objectID);
			}
			break;
		}
	}
// 	pPar->m_mouseButtonDownIsUseable = 1;
}

_XComboTexture::_XComboTexture()
:m_isInited(0)
,comboInputNormal(NULL)				//�����˵���������ͨ״̬
,comboInputDisable(NULL)			//��Ч״̬�µİ�ť��ͼ
{
}

_XComboTexture::~_XComboTexture()
{
	release();
}

int _XComboTexture::init(const char *inputNormal,const char *inputDisable,
		 const char *downButtonNormal,const char *downButtonOn,const char *downButtonDown,const char *downButtonDisable,
		 const char *downMenuUpNormal,const char *downMenuUpOn,const char *downMenuUpDown,const char *downMenuUpDisable,
		 const char *downMenuNormal,const char *downMenuOn,const char *downMenuDown,const char *downMenuDisable,
		 const char *downMenuDownNormal,const char *downMenuDownOn,const char *downMenuDownDown,const char *downMenuDownDisable,
		 int resoursePosition)
{
	if(m_isInited != 0) return 0;	//��ֹ�ظ���ʼ��
	if(inputNormal == NULL || inputDisable == NULL ||
		downButtonNormal == NULL || downButtonOn == NULL || downButtonDown == NULL || downButtonDisable == NULL ||
		downMenuUpNormal == NULL || downMenuUpOn == NULL || downMenuUpDown == NULL || downMenuUpDisable == NULL ||
		downMenuNormal == NULL || downMenuOn == NULL || downMenuDown == NULL || downMenuDisable == NULL ||
		downMenuDownNormal == NULL || downMenuDownOn == NULL || downMenuDownDown == NULL || downMenuDownDisable == NULL)
	{//��ͼ�����ݲ���Ϊ��
		return 0;
	}
	int ret = 1;
	//���������������е���ͼ��Դ
	if((comboInputNormal = createATextureData(inputNormal,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(comboInputDisable = createATextureData(inputDisable,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 && 
		downButtonTex.init(downButtonNormal,downButtonDown,downButtonOn,downButtonDisable,resoursePosition) == 0)
	{
		ret = 0;
	}
	if(ret != 0 && 
		downMenuUpButtonTex.init(downMenuUpNormal,downMenuUpDown,downMenuUpOn,downMenuUpDisable,resoursePosition) == 0)
	{
		ret = 0;
	}
	if(ret != 0 && 
		downMenuButtonTex.init(downMenuNormal,downMenuDown,downMenuOn,downMenuDisable,resoursePosition) == 0)
	{
		ret = 0;
	}
	if(ret != 0 && 
		downMenuDownButtonTex.init(downMenuDownNormal,downMenuDownDown,downMenuDownOn,downMenuDownDisable,resoursePosition) == 0)
	{
		ret = 0;
	}
	if(ret == 0)
	{
		XDELETE(comboInputNormal);
		XDELETE(comboInputDisable);
		
		downButtonTex.release();
		downMenuUpButtonTex.release();
		downMenuButtonTex.release();
		downMenuDownButtonTex.release();
		return 0;
	}

	m_isInited = 1;
	return 1;
}

void _XComboTexture::release()
{
	if(m_isInited == 0) return;
	XDELETE(comboInputNormal);
	XDELETE(comboInputDisable);
	
	downButtonTex.release();
	downMenuUpButtonTex.release();
	downMenuButtonTex.release();
	downMenuDownButtonTex.release();
	m_isInited = 0;
}

_XCombo::_XCombo()
:m_isInited(0)
,m_comboInputNormal(NULL)			//�����˵���������ͨ״̬
,m_comboInputDisable(NULL)			//��Ч״̬�µİ�ť��ͼ
,m_funDataChange(NULL)
,m_pClass(NULL)
,m_menuData(NULL)
,m_buttom(NULL)
{
}

_XCombo::~_XCombo()
{
	release();
}

void _XCombo::release()
{
	if(m_isInited == 0) return;
	XDELETE_ARRAY(m_buttom);
	XDELETE_ARRAY(m_menuData);
	_XControlManager::GetInstance().decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	m_isInited = 0;
}

int _XCombo::init(const _XComboTexture &tex,	//�ؼ�����ͼ
		const _XVector2& position,		//�ؼ���λ��
		const _XRect& inputArea,		//��������Ч��Χ
		const _XRect& downButtonArea,	//������ť����Ӧ��Χ
		const _XRect& downMenuUpArea,	//�Ϸ�ҳ����Ӧ��Χ
		const _XRect& downMenuArea,		//ѡ�������Ӧ��Χ
		const _XRect& downMenuDownArea,	//�·�ҳ����Ӧ��Χ
		int menuSum,					//�����˵��е�ѡ�������
		int drawMenuSum,				//�����˵�����ʾ�Ĳ˵��������
		const _XFontUnicode &font,		//��ʾ����ʹ�õ�����
		float fontSize)				//����Ĵ�С
{
	if(m_isInited != 0) return 0;				//��ֹ�ظ���ʼ��
	if(tex.comboInputNormal == NULL) return 0;	//����������ͼΪ�գ�������ͼ������Ϊ�գ��򷵻�ʧ��
	if(menuSum <= 0) return 0;					//�˵��в���û�в˵���
	if(drawMenuSum <= 0 || drawMenuSum > menuSum) return 0;	
	if(fontSize <= 0) return 0;
	if(inputArea.getHeight() <= 0 || inputArea.getWidth() <= 0) return 0;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	m_position = position;

	//������ͼ
	m_comboInputNormal = tex.comboInputNormal;					//�����˵���������ͨ״̬
	m_comboInputDisable = tex.comboInputDisable;				//��Ч״̬�µİ�ť��ͼ

	//������ֵ
	m_size.set(1.0f,1.0f);
	m_menuSum = menuSum;
	m_menuDrawSum = drawMenuSum;
	m_menuStartDrawOrder = 0;
	m_inputArea = inputArea;
	m_downButtonArea = downButtonArea;
	m_downMenuUpArea = downMenuUpArea;
	m_downMenuArea = downMenuArea;
	m_downMenuDownArea = downMenuDownArea;

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(_FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(_FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	m_caption.setString(" ");
	m_caption.setPosition(m_position.x + (m_inputArea.left + COMBO_LEFT_DISTANSE) * m_size.x,m_position.y + (m_inputArea.top + m_inputArea.getHeight() * 0.5f + COMBO_TOP_DISTANSE) * m_size.y);
	m_textSize.set(fontSize,fontSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	//������ť
	try
	{
		m_buttom = new _XButton[m_menuDrawSum + 3];
		if(m_buttom == NULL)return 0;
	}catch(...)
	{
		return 0;
	}
	//�����ʼ�����а�ť
	//_XButtonTexture tempButtonTex;
	//tempButtonTex.set(m_comboDownButtonNormal,m_comboDownButtonDown,m_comboDownButtonOn,m_comboDownButtonDisable);
	if(m_buttom[0].init(_XVector2(m_position.x + (m_inputArea.right - m_downButtonArea.left) * m_size.x,m_position.y),
		m_downButtonArea,tex.downButtonTex," ",font,m_textSize.x,_XVector2(0,0)) == 0) return 0;		//�����˵���ߵ������˵��İ�ť
	m_buttom[0].setSize(m_size);
	m_buttom[0].setCallbackFun(NULL,NULL,NULL,funComboMenuState,NULL,this);
	//tempButtonTex.set(m_comboDownMenuUpNormal,m_comboDownMenuUpDown,m_comboDownMenuUpOn,m_comboDownMenuUpDisable);
	if(m_buttom[1].init(_XVector2(m_position.x,m_position.y + (m_inputArea.bottom - m_downMenuUpArea.top) * m_size.y),
		m_downMenuUpArea,tex.downMenuUpButtonTex," ",font,m_textSize.x,_XVector2(0,0)) == 0) return 0;		//�����˵����˵�����İ�ť
	m_buttom[1].setSize(m_size);
	m_buttom[1].setCallbackFun(NULL,NULL,NULL,funComboMenuStart,NULL,this);
	//tempButtonTex.set(m_comboDownMenuDownNormal,m_comboDownMenuDownDown,m_comboDownMenuDownOn,m_comboDownMenuDownDisable);
	_XVector2 tempPosition;
	tempPosition.set(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
		+ m_downMenuArea.getHeight() * m_menuDrawSum - m_downMenuDownArea.top) * m_size.y);
	if(m_buttom[2].init(tempPosition,m_downMenuDownArea,tex.downMenuDownButtonTex," ",font,m_textSize.x,_XVector2(0,0)) == 0) return 0;		//�����˵����˵�������İ�ť
	m_buttom[2].setSize(m_size);
	m_buttom[2].setCallbackFun(NULL,NULL,NULL,funComboMenuEnd,NULL,this);

	//tempButtonTex.set(m_comboDownMenuNormal,m_comboDownMenuDown,m_comboDownMenuOn,m_comboDownMenuDisable);
	for(int i = 0;i < m_menuDrawSum;++ i)
	{
		tempPosition.set(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
			+ m_downMenuArea.getHeight() * i - m_downMenuArea.top) * m_size.y);
		if(m_buttom[3 + i].init(tempPosition,m_downMenuArea,tex.downMenuButtonTex," ",font,m_textSize.x,
			_XVector2(m_downMenuArea.left + m_downMenuArea.getWidth() * 0.5f,m_downMenuArea.top + m_downMenuArea.getHeight() * 0.5f)) == 0) return 0;		//�����˵����˵�������İ�ť
		m_buttom[3 + i].setSize(m_size);
		m_buttom[3 + i].setCallbackFun(NULL,NULL,NULL,funComboMenu,NULL,this);
	}
	//�ӿؼ���������ע����Щ��ť
	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		_XControlManager::GetInstance().decreaseAObject(&(m_buttom[i]));	//ע��������
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_buttom[i]);
#endif
	}
	m_allArea.set(m_position.x + m_inputArea.left * m_size.x,m_position.y + m_inputArea.top * m_size.y,
		m_position.x + m_downMenuArea.right * m_size.x,
		m_position.y + (m_inputArea.top + m_inputArea.getHeight() + m_downMenuUpArea.getHeight() 
		+ m_downMenuArea.getHeight() * m_menuDrawSum + m_downMenuDownArea.getHeight()) * m_size.y);

	//����˵��п�����ʾ����������
	m_menuTextWidth = (downMenuArea.getWidth() - downButtonArea.getWidth() - COMBO_LEFT_DISTANSE) * m_size.x/ (m_caption.getTextSize().x * m_caption.getSize().x * 0.5f);	//�����Ҫ����
	if(m_menuTextWidth < 3) return 0;

	m_sprite.init(m_comboInputNormal->texture.m_w,m_comboInputNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	//���ݴ洢
	try
	{
		m_menuData = new char[COMBO_MAX_MENU_LENGTH];
		if(m_menuData == NULL) return 0;		//�ڴ����ʧ�� 
	}catch(...)
	{
		return 0;
	}
	m_menuData[0] = '\0';
	char tempMenu[] = "MenuXXXX\n";
	for(int i = 0;i < m_menuSum;++ i)
	{//��ʼ�����еĲ˵��������Ϊ�ո�
		tempMenu[4] = (i /1000) % 10 + '0';
		tempMenu[5] = (i /100) % 10 + '0';
		tempMenu[6] = (i /10) % 10 + '0';
		tempMenu[7] = i % 10 + '0';
		strcat(m_menuData,tempMenu);
	}
	m_isDrawDownMenu = 0;
	m_nowChooseOrder = 0;

	m_isVisiable = 1;
	m_isEnable = 1;
	m_isActive = 1;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_COMBO);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = 1;

	updateString();
	return 1;
}

void _XCombo::draw()
{
	if(m_isInited == 0) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isVisiable == 0) return;	//������ɼ�ֱ���˳�
	m_buttom[0].draw();
	if(m_isEnable != 0)
	{
		m_sprite.draw(m_comboInputNormal);
	}else
	{
		m_sprite.draw(m_comboInputDisable);
	}
	m_caption.draw();
}

void _XCombo::drawUp()
{
	if(m_isInited == 0) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isVisiable == 0) return;	//������ɼ�ֱ���˳�
	if(m_isDrawDownMenu != 0)
	{
		m_buttom[1].draw();
		for(int i = 0;i < m_menuDrawSum;++ i)
		{
			m_buttom[i + 3].draw();
		}
		m_buttom[2].draw();
	}
}

int _XCombo::canGetFocus(float x,float y)
{
	if(m_isInited == 0) return 0;	//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 0;		//û�м���Ŀؼ������տ���
	if(m_isVisiable == 0) return 0;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}

int _XCombo::canLostFocus(float x,float y)
{
	if(m_isInited == 0) return 1;	//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 1;		//û�м���Ŀؼ������տ���
	if(m_isVisiable == 0) return 1;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 1;		//�����Ч��ֱ���˳�

	if(m_isDrawDownMenu != 0) return 0;
	return 1;
}

int _XCombo::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(m_isInited == 0) return 0;	//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 0;		//û�м���Ŀؼ������տ���
	if(m_isVisiable == 0) return 0;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�
	
	if(mouseState == _XMOUSE_LEFT_BUTTON_DOWN)
	{//�������Ч�������ڰ��������������ص������˵�
	//	m_mouseButtonDownIsUseable = 0;
		if(!m_allArea.isInRect(x,y)) m_isDrawDownMenu = 0;
	}
	m_buttom[0].mouseProc(x,y,mouseState);	//������ť������¼�������Ӧ
	if(m_isDrawDownMenu != 0)
	{
		for(int i = 0;i < m_menuDrawSum + 2;++ i)
		{
			m_buttom[i + 1].mouseProc(x,y,mouseState);
		}
	}
//	if(mouseState == _XMOUSE_STATE_DOWN && m_mouseButtonDownIsUseable == 0)
//	{
//		m_isDrawDownMenu = 0;
//	}
	return 1;
}

int _XCombo::setMenuStr(const char *str,int order)
{
	if(m_isInited == 0) return 0;	//���û�г�ʼ��ֱ���˳�
	if(order < 0 || order >= m_menuSum) return 0;
	if(str == NULL) return 0;
	if(strlen(str) >= COMBO_MAX_MENU_LENGTH) return 0;
	char temp1[COMBO_MAX_MENU_LENGTH];
	strcpy(temp1,str);
	//�������ַ����Ƿ����Ҫ��
	for(int i = 0;i < COMBO_MAX_MENU_LENGTH;++ i)
	{
		if(temp1[i] == '\0') break;
		if(temp1[i] == '\n')
		{
			temp1[i] = '\0';
			break;
		}
	}

	int len = strlen(m_menuData);
	int addLen = strlen(temp1);
	char temp[COMBO_MAX_MENU_LENGTH];
	if(order == 0)
	{//��һ��Ԫ��
		int i;
		for(i = 0;i < len;++i)
		{
			if(m_menuData[i] == '\n')
			{
				strcpy(temp,m_menuData + i);
				break;
			}
		}
		if(i >= len) return 0;
		if(len + addLen - i>= COMBO_MAX_MENU_LENGTH) return 0;
		strcpy(m_menuData,temp1);
		strcat(m_menuData,temp);
		updateString();
	}else
	{//�����Ԫ��
		int start = 0;
		int end = 0;
		int i = 0;
		int j = 0;
		for(i = 0;i < len;++ i)
		{
			if(m_menuData[i] == '\n')
			{
				j++;
				if(j == order) start = i;
				if(j == order + 1)
				{
					end = i;
					break;
				}
			}
		}
		if(start == 0 && end == 0) return 0;	//û���ҵ����ʵ�λ��
		if(len + addLen - (end - start) >= COMBO_MAX_MENU_LENGTH) return 0;
		strcpy(temp,m_menuData + end);
		m_menuData[start + 1] = '\0';
		strcat(m_menuData,temp1);
		strcat(m_menuData,temp);
		updateString();
	}
	return 1;
}

void _XCombo::updateString()
{
	if(m_isInited == 0) return;	//���û�г�ʼ��ֱ���˳�
	char temp[COMBO_MAX_MENU_LENGTH];
	int start = 0;
	int order = 0;
	int len = strlen(m_menuData);
	for(int i = 0;i < len;++ i)
	{
		if(m_menuData[i] == '\n')
		{
			if(order >= m_menuStartDrawOrder && order < m_menuStartDrawOrder + m_menuDrawSum)
			{
				memcpy(temp,m_menuData + start,i - start);
				temp[i - start] = '\0';
				if(i - start >= m_menuTextWidth)
				{//���������ʾ���ȣ�����ʾ"..."
					temp[m_menuTextWidth] = '\0';
					temp[m_menuTextWidth - 1] = '.';
					temp[m_menuTextWidth - 2] = '.';
				}
				m_buttom[order - m_menuStartDrawOrder + 3].setCaptionText(temp);
				if(order == m_nowChooseOrder)
				{
					m_caption.setString(temp);
				}
			}else
			if(order == m_nowChooseOrder)
			{
				memcpy(temp,m_menuData + start,i - start);
				temp[i - start] = '\0';
				if(i - start >= m_menuTextWidth)
				{//���������ʾ���ȣ�����ʾ"..."
					temp[m_menuTextWidth] = '\0';
					temp[m_menuTextWidth - 1] = '.';
					temp[m_menuTextWidth - 2] = '.';
				}
				m_caption.setString(temp);
			}
			start = i + 1;
			order ++;
		}
	}
}

void _XCombo::setSize(float x,float y)			//���ÿؼ��ĳߴ�
{
	if(x <= 0 && y <= 0) return;	//�ߴ��ֵ�������
	if(m_isInited == 0) return;				//���û�г�ʼ������ֱ�ӷ���	
	m_size.set(x,y);
	m_caption.setPosition(m_position.x + (m_inputArea.left + COMBO_LEFT_DISTANSE) * m_size.x,m_position.y + (m_inputArea.top + m_inputArea.getHeight() * 0.5f + COMBO_TOP_DISTANSE) * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_buttom[0].setPosition(m_position.x + (m_inputArea.right - m_downButtonArea.left) * m_size.x,m_position.y);
	m_buttom[0].setSize(m_size);
	m_buttom[1].setPosition(m_position.x,m_position.y + (m_inputArea.bottom - m_downMenuUpArea.top) * m_size.y);
	m_buttom[1].setSize(m_size);
	m_buttom[2].setPosition(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
		+ m_downMenuArea.getHeight() * m_menuDrawSum - m_downMenuDownArea.top) * m_size.y);
	m_buttom[2].setSize(m_size);
	m_allArea.set(m_position.x + m_inputArea.left * m_size.x,m_position.y + m_inputArea.top * m_size.y,
		m_position.x + m_downMenuArea.right * m_size.x,
		m_position.y + (m_inputArea.top + m_inputArea.getHeight() + m_downMenuUpArea.getHeight() 
		+ m_downMenuArea.getHeight() * m_menuDrawSum + m_downMenuDownArea.getHeight()) * m_size.y);

	for(int i = 0;i < m_menuDrawSum;++ i)
	{
		m_buttom[3 + i].setPosition(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
			+ m_downMenuArea.getHeight() * i - m_downMenuArea.top) * m_size.y);
		m_buttom[3 + i].setSize(m_size);
	}
	m_sprite.setSize(m_size);
}

void _XCombo::setPosition(float x,float y)	//���ÿռ��λ��
{
	if(m_isInited == 0) return;				//���û�г�ʼ������ֱ�ӷ���
	m_position.set(x,y);
	m_caption.setPosition(m_position.x + (m_inputArea.left + COMBO_LEFT_DISTANSE) * m_size.x,m_position.y + (m_inputArea.top + m_inputArea.getHeight() * 0.5f + COMBO_TOP_DISTANSE) * m_size.y);
	m_buttom[0].setPosition(m_position.x + (m_inputArea.right - m_downButtonArea.left) * m_size.x,m_position.y);
	m_buttom[1].setPosition(m_position.x,m_position.y + (m_inputArea.bottom - m_downMenuUpArea.top) * m_size.y);
	m_buttom[2].setPosition(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
		+ m_downMenuArea.getHeight() * m_menuDrawSum - m_downMenuDownArea.top) * m_size.y);
	m_allArea.set(m_position.x + m_inputArea.left * m_size.x,m_position.y + m_inputArea.top * m_size.y,
		m_position.x + m_downMenuArea.right * m_size.x,
		m_position.y + (m_inputArea.top + m_inputArea.getHeight() + m_downMenuUpArea.getHeight() 
		+ m_downMenuArea.getHeight() * m_menuDrawSum + m_downMenuDownArea.getHeight()) * m_size.y);

	for(int i = 0;i < m_menuDrawSum;++ i)
	{
		m_buttom[3 + i].setPosition(m_position.x,m_position.y + (m_inputArea.bottom + m_downMenuUpArea.getHeight() 
			+ m_downMenuArea.getHeight() * i - m_downMenuArea.top) * m_size.y);
	}
	m_sprite.setPosition(m_position);
}

int _XCombo::setACopy(const _XCombo &temp)
{
	if(& temp == this) return 1;	//��ֹ����ֵ
	if(temp.m_isInited == 0) return 0;
	if(m_isInited != 0) release();	//������Ҫ�ͷ�ԭ�е���Դ
	if(_XControlBasic::setACopy(temp) == 0) return 0;
	if(m_isInited == 0)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_COMBO);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}
	m_isInited = temp.m_isInited;

	m_comboInputNormal = temp.m_comboInputNormal;					//�����˵���������ͨ״̬
	m_comboInputDisable = temp.m_comboInputDisable;					//��Ч״̬�µİ�ť��ͼ

	m_menuSum = temp.m_menuSum;							//�����˵��е��ܲ˵�������
	m_menuDrawSum = temp.m_menuDrawSum;					//�����˵�����ʾ�Ĳ˵�������
	m_menuStartDrawOrder = temp.m_menuStartDrawOrder;	//�����˵��п�ʼ��ʾ�ĵ�һ���˵���ı��
	m_menuTextWidth = temp.m_menuTextWidth;				//�����˵�����ʾ�����ֵ����ֿ��

	m_isDrawDownMenu = temp.m_isDrawDownMenu;			//�Ƿ���ʾ�����˵�
	m_nowChooseOrder = temp.m_nowChooseOrder;			//��ǰѡ��Ĳ˵���ı��

	m_caption.setACopy(temp.m_caption);			//���ֵ�����
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_sprite.setACopy(temp.m_sprite);		//������ʾ��ͼ�ľ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_textSize = temp.m_textSize;			//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	m_textColor = temp.m_textColor;			//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����

	try
	{
		m_menuData = new char[COMBO_MAX_MENU_LENGTH];//�����˵��ľ�������
		if(m_menuData == NULL) return 0;		//�ڴ����ʧ�� 	
	}catch(...)
	{
		return 0;
	}
	strcpy(m_menuData,temp.m_menuData);
	try
	{
		m_buttom = new _XButton[m_menuDrawSum + 3];	//�����˵��еİ�ť��3����ťΪ���ܰ�ť������Ϊ�˵���ť
		if(m_buttom == NULL) return 0;
	}catch(...)
	{
		return 0;
	}

	for(int i = 0;i < m_menuDrawSum + 3;++ i)
	{
		m_buttom[i].setACopy(temp.m_buttom[i]);	//�������ƵĻ����ص��������������
		if(i >= 3) m_buttom[i].setCallbackFun(NULL,NULL,NULL,funComboMenu,NULL,this);
		_XControlManager::GetInstance().decreaseAObject(&(m_buttom[i]));	//�ӿؼ���������ע����Щ��ť
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_buttom[i]);
#endif
	}
	//Ϊ�˱��������������ע��ص�����
	m_buttom[0].setCallbackFun(NULL,NULL,NULL,funComboMenuState,NULL,this);
	m_buttom[1].setCallbackFun(NULL,NULL,NULL,funComboMenuStart,NULL,this);
	m_buttom[2].setCallbackFun(NULL,NULL,NULL,funComboMenuEnd,NULL,this);

	m_funDataChange = temp.m_funDataChange;
	m_pClass = temp.m_pClass;				//�ص������Ĳ���

	m_inputArea = temp.m_inputArea;			//��������Ӧ��Χ
	m_downButtonArea = temp.m_downButtonArea;	//������ť����Ӧ��Χ
	m_downMenuUpArea = temp.m_downMenuUpArea;		//�Ϸ�ҳ����Ӧ��Χ
	m_downMenuArea = temp.m_downMenuArea;		//ѡ�������Ӧ��Χ
	m_downMenuDownArea = temp.m_downMenuDownArea;	//�·�ҳ����Ӧ��Χ
	m_allArea = temp.m_allArea;
	return 1;
}

int _XCombo::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}

_XVector2 _XCombo::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;
	if(m_isDrawDownMenu != 0)
	{//�Ƿ���ʾ�����˵�
		if(order == 0) ret.set(m_allArea.left,m_allArea.top);else
		if(order == 1) ret.set(m_allArea.right,m_allArea.top);else
		if(order == 2) ret.set(m_allArea.right,m_allArea.bottom);else
		if(order == 3) ret.set(m_allArea.left,m_allArea.bottom);
	}else
	{//
		if(order == 0) ret.set(m_allArea.left,m_allArea.top);else
		if(order == 1) ret.set(m_allArea.right,m_allArea.top);else
		if(order == 2) ret.set(m_allArea.right,m_position.y + m_inputArea.bottom * m_size.y);else
		if(order == 3) ret.set(m_allArea.left,m_position.y + m_inputArea.bottom * m_size.y);
	}
	return ret;
}
