//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XButtonEx.h"
#include "XObjectManager.h"
#include "XControlManager.h"
#include "XResourceManager.h"

_XButtonEx::_XButtonEx()
:m_isInited(XFalse)
,m_buttonNormal(NULL)		
,m_buttonDown(NULL)	
,m_buttonOn(NULL)		
,m_buttonDisable(NULL)	
,m_funInit(NULL)
,m_funRelease(NULL)
,m_funMouseOn(NULL)
,m_funMouseDown(NULL)
,m_funMouseUp(NULL)
,m_pClass(NULL)
,m_hotKey(-1)
,m_nowButtonState(BUTTON_STATE_NORMAL)
,m_upMousePoint(0,0)
,m_pArea(NULL)
,m_pNowArea(NULL)
,m_resInfo(NULL)
{
}
_XButtonEx::~_XButtonEx()
{
	release();
}
void _XButtonEx::setTexture(const _XButtonTexture& tex)
{
	if(tex.buttonNormal == NULL) return;
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;
}
_XBool _XButtonEx::init(const _XVector2& position,		//�ؼ���λ��
		const _XVector2 *area,int pointSum,	//����������ť����Ӧ��Χ�����е㣬�Ѿ��������
		const _XButtonTexture &tex,					//��ť�ĸ�����ͼ��Ϣ
		const char *caption,float captionSize,_XVector2 captionPosition,	//��ť����ʵ�����ֵ������Ϣ
		const _XFontUnicode &font)
{
	if(m_isInited) return XFalse;
	if(area == NULL || pointSum <= 2) return XFalse;	//�ؼ�����Ҫ��һ�����ʵ���Ӧ���䣬��Ȼ����ɴ���
	if(tex.buttonNormal == NULL) return XFalse;		//��������ͨ״̬����ͼ����Ϊ�գ�����ֱ�ӷ��ش���
	if(captionSize <= 0) return XFalse;
	m_position = position;
	m_textPosition = captionPosition;
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;

	m_pArea = createArrayMem<_XVector2>(pointSum);
	if(m_pArea == NULL) return XFalse;
	m_pNowArea = createArrayMem<_XVector2>(pointSum);
	if(m_pNowArea == NULL)
	{
		XDELETE_ARRAY(m_pArea);
		return XFalse;
	}
	m_areaPointSum = pointSum;
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_pArea[i] = area[i];
		m_pNowArea[i].x = m_position.x + area[i].x * m_size.x;
		m_pNowArea[i].y = m_position.y + area[i].y * m_size.y;
	}

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //����������ж���
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	m_size.set(1.0f,1.0f);
	m_sprite.init(m_buttonNormal->texture.m_w,m_buttonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
	m_color.setColor(1.0f,1.0f,1.0f,1.0f);
	m_sprite.setColor(m_color);
	//������ж��룬���������λ����Ҫ����
	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_nowButtonState = BUTTON_STATE_NORMAL;

	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_BUTTONEX);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XButtonEx::initEx(const _XVector2& position,			//�ؼ���λ��
	const _XButtonTexture &tex,				//��ť�ĸ�����ͼ��Ϣ
	const char *caption,float captionSize,	//��ť����ʵ�����ֵ������Ϣ
	const _XFontUnicode &font)				//��ť��ʹ�õ�����
{
	if(m_isInited) return XFalse;
	if(tex.m_pArea == NULL || tex.m_areaPointSum <= 2) return XFalse;	//�ؼ�����Ҫ��һ�����ʵ���Ӧ���䣬��Ȼ����ɴ���
	if(tex.buttonNormal == NULL) return XFalse;		//��������ͨ״̬����ͼ����Ϊ�գ�����ֱ�ӷ��ش���
	if(captionSize <= 0) return XFalse;
	m_position = position;
	m_textPosition = tex.m_fontPosition;
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;

	m_pArea = createArrayMem<_XVector2>(tex.m_areaPointSum);
	if(m_pArea == NULL) return XFalse;
	m_pNowArea = createArrayMem<_XVector2>(tex.m_areaPointSum);
	if(m_pNowArea == NULL)
	{
		XDELETE_ARRAY(m_pArea);
		return XFalse;
	}
	m_areaPointSum = tex.m_areaPointSum ;
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_pArea[i] = tex.m_pArea[i];
		m_pNowArea[i].x = m_position.x + tex.m_pArea[i].x * m_size.x;
		m_pNowArea[i].y = m_position.y + tex.m_pArea[i].y * m_size.y;
	}

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //����������ж���
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	m_size.set(1.0f,1.0f);
	m_sprite.init(m_buttonNormal->texture.m_w,m_buttonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
	m_color.setColor(1.0f,1.0f,1.0f,1.0f);
	m_sprite.setColor(m_color);
	//������ж��룬���������λ����Ҫ����
	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_nowButtonState = BUTTON_STATE_NORMAL;

	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_BUTTONEX);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XButtonEx::initPlus(const char *path,				//��ť�ĸ�����ͼ��Ϣ
		const char *caption,float captionSize,	//��ť����ʵ�����ֵ������Ϣ
		const _XFontUnicode &font,_XResourcePosition resoursePosition)				//��ť��ʹ�õ�����
{
	if(m_isInited) return XFalse;
	if(path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XBUTTON_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	_XButtonTexture * tex = (_XButtonTexture *)m_resInfo->m_pointer;

	if(tex->m_pArea == NULL || tex->m_areaPointSum <= 2) return XFalse;	//�ؼ�����Ҫ��һ�����ʵ���Ӧ���䣬��Ȼ����ɴ���
	if(tex->buttonNormal == NULL) return XFalse;		//��������ͨ״̬����ͼ����Ϊ�գ�����ֱ�ӷ��ش���
	if(captionSize <= 0) return XFalse;
	m_position.set(0.0f,0.0f);
	m_textPosition = tex->m_fontPosition;
	m_buttonNormal = tex->buttonNormal;
	if(tex->buttonDown != NULL) m_buttonDown = tex->buttonDown;
	else m_buttonDown = tex->buttonNormal;
	if(tex->buttonOn != NULL) m_buttonOn = tex->buttonOn;
	else m_buttonOn = tex->buttonNormal;
	if(tex->buttonDisable != NULL) m_buttonDisable = tex->buttonDisable;
	else m_buttonDisable = tex->buttonNormal;

	m_pArea = createArrayMem<_XVector2>(tex->m_areaPointSum);
	if(m_pArea == NULL) return XFalse;
	m_pNowArea = createArrayMem<_XVector2>(tex->m_areaPointSum);
	if(m_pNowArea == NULL)
	{
		XDELETE_ARRAY(m_pArea);
		return XFalse;
	}
	m_areaPointSum = tex->m_areaPointSum ;
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_pArea[i] = tex->m_pArea[i];
		m_pNowArea[i].x = m_position.x + tex->m_pArea[i].x * m_size.x;
		m_pNowArea[i].y = m_position.y + tex->m_pArea[i].y * m_size.y;
	}

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //����������ж���
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	m_size.set(1.0f,1.0f);
	m_sprite.init(m_buttonNormal->texture.m_w,m_buttonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
	m_color.setColor(1.0f,1.0f,1.0f,1.0f);
	m_sprite.setColor(m_color);
	//������ж��룬���������λ����Ҫ����
	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_nowButtonState = BUTTON_STATE_NORMAL;

	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_BUTTONEX);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void _XButtonEx::setPosition(float x,float y)
{
	if(!m_isInited) return;
	m_position.set(x,y);
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_pNowArea[i].x = m_position.x + m_pArea[i].x * m_size.x;
		m_pNowArea[i].y = m_position.y + m_pArea[i].y * m_size.y;
	}
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_sprite.setPosition(m_position);
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
}
void _XButtonEx::setSize(float x,float y)
{
	if(!m_isInited) return;
	if(x <= 0 && y <= 0) return;
	m_size.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_sprite.setSize(m_size);
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_pNowArea[i].x = m_position.x + m_pArea[i].x * m_size.x;
		m_pNowArea[i].y = m_position.y + m_pArea[i].y * m_size.y;
	}
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
}
_XBool _XButtonEx::canGetFocus(float x,float y)
{
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return getIsInRect(_XVector2(x,y),m_pNowArea,m_areaPointSum);
}
_XBool _XButtonEx::canLostFocus(float x,float y)
{
	return XTrue;
}
_XBool _XButtonEx::mouseProc(float x,float y,_XMouseState mouseState)
{
	m_upMousePoint.set(x,y);
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�

	if(getIsInRect(m_upMousePoint,m_pNowArea,m_areaPointSum))
	{//��궯���ڷ�Χ��
		if(mouseState == MOUSE_MOVE && m_nowButtonState == BUTTON_STATE_NORMAL)
		{
			m_nowButtonState = BUTTON_STATE_ON;
			if(m_funMouseOn != NULL) m_funMouseOn(m_pClass,m_objectID);
		}else
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN && (m_nowButtonState == BUTTON_STATE_NORMAL || m_nowButtonState == BUTTON_STATE_ON))
		{
			m_nowButtonState = BUTTON_STATE_DOWN;
			if(m_funMouseDown != NULL) m_funMouseDown(m_pClass,m_objectID);
		}else
		if(mouseState == MOUSE_LEFT_BUTTON_UP && m_nowButtonState == BUTTON_STATE_DOWN)
		{
			m_nowButtonState = BUTTON_STATE_ON;
			if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
		}
	}else
	{
		if(m_nowButtonState == BUTTON_STATE_ON || m_nowButtonState == BUTTON_STATE_DOWN)
		{
			m_nowButtonState = BUTTON_STATE_NORMAL;
		}
	}
	return XTrue;
}
_XBool _XButtonEx::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�

	if(keyState == KEY_STATE_UP)
	{//��������ʱ������Ӧ
		if(m_nowButtonState == BUTTON_STATE_NORMAL)
		{//��ťֻ������ͨ״̬�²�����Ӧ��ݼ�����ֹ��ͻ
			if(keyOrder == m_hotKey || (keyOrder ==  XKEY_RETURN && m_isBeChoose != 0))
			{
				if(m_funMouseDown != NULL) m_funMouseDown(m_pClass,m_objectID);
				if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
				return XTrue;
			}
		}
	}
	return XFalse;
}
void _XButtonEx::draw()
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	if(!m_isVisiable) return;	//������ɼ�ֱ���˳�

	if(m_nowButtonState == BUTTON_STATE_NORMAL) m_sprite.draw(m_buttonNormal); else
	if(m_nowButtonState == BUTTON_STATE_DOWN) m_sprite.draw(m_buttonDown); else
	if(m_nowButtonState == BUTTON_STATE_ON) m_sprite.draw(m_buttonOn); else
	if(m_nowButtonState == BUTTON_STATE_DISABLE) m_sprite.draw(m_buttonDisable);

	//��ʾ��ť������
	m_caption.draw();
}
void _XButtonEx::setCallbackFun(void (* funInit)(void *,int),
							  void (* funRelease)(void *,int),
							  void (* funMouseOn)(void *,int),
							  void (* funMouseDown)(void *,int),
							  void (* funMouseUp)(void *,int),void *pClass)
{
	if(funInit != NULL) m_funInit = funInit;
	if(funRelease != NULL) m_funRelease = funRelease;
	if(funMouseOn != NULL) m_funMouseOn = funMouseOn;
	if(funMouseDown != NULL) m_funMouseDown = funMouseDown;
	if(funMouseUp != NULL) m_funMouseUp = funMouseUp;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
void _XButtonEx::release()
{
	if(!m_isInited) return;

	XDELETE_ARRAY(m_pArea);
	XDELETE_ARRAY(m_pNowArea);
	m_areaPointSum = 0;
	if(m_funRelease != NULL) m_funRelease(m_pClass,m_objectID);

	_XControlManager::GetInstance().decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}
_XBool _XButtonEx::setACopy(const _XButtonEx &temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited) return XFalse;
	if(m_isInited) release();
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_BUTTONEX);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}

	m_isInited = temp.m_isInited;					//��ť�Ƿ񱻳�ʼ��
	m_nowButtonState = temp.m_nowButtonState;		//��ǰ�İ�ť״̬

	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);

	if(!m_caption.setACopy(temp.m_caption))	return XFalse;		//��ť�ı���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_areaPointSum = temp.m_areaPointSum;
	m_pArea = createArrayMem<_XVector2>(m_areaPointSum);
	if(m_pArea == NULL) return XFalse;
	m_pNowArea = createArrayMem<_XVector2>(m_areaPointSum);
	if(m_pNowArea == NULL)
	{
		XDELETE_ARRAY(m_pArea);
		return XFalse;
	}
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		m_pArea[i] = temp.m_pArea[i];
		m_pNowArea[i] = temp.m_pNowArea[i];
	}

	m_textColor = temp.m_textColor;

	m_buttonNormal = temp.m_buttonNormal;			//��ͨ״̬�µİ�ť��ͼ(������Σ������ͼһ��Ҫ��)
	m_buttonDown = temp.m_buttonDown;				//����״̬�µİ�ť��ͼ
	m_buttonOn = temp.m_buttonOn;					//����״̬�µİ�ť��ͼ
	m_buttonDisable = temp.m_buttonDisable;			//��Ч״̬�µİ�ť��ͼ

	m_funInit = temp.m_funInit;
	m_funRelease = temp.m_funRelease;
	m_funMouseOn = temp.m_funMouseOn;
	m_funMouseDown = temp.m_funMouseDown;
	m_funMouseUp = temp.m_funMouseUp;
	m_pClass = temp.m_pClass;				//�ص������Ĳ���

	m_sprite.setACopy(temp.m_sprite);		//������ʾ��ͼ�ľ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_textPosition = temp.m_textPosition;	//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize = temp.m_textSize;			//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	m_upMousePoint = temp.m_upMousePoint;	//�ϴμ�¼�����λ��

	m_hotKey = temp.m_hotKey;
	return XTrue;
}
_XBool _XButtonEx::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
	//return getIsInRect(m_upMousePoint,m_pNowArea,m_areaPointSum);
}
_XVector2 _XButtonEx::getBox(int order)
{//Ѱ�����е�������Χ�ĵ�
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(!m_isInited) return ret;
	float left = m_pNowArea[0].x;
	float right = m_pNowArea[0].x;
	float top = m_pNowArea[0].y;
	float bottom = m_pNowArea[0].y;
	for(int i = 0;i < m_areaPointSum;++ i)
	{
		if(m_pNowArea[i].x < left) left = m_pNowArea[i].x;
		if(m_pNowArea[i].x > right) right = m_pNowArea[i].x;
		if(m_pNowArea[i].y < top) top = m_pNowArea[i].y;
		if(m_pNowArea[i].y > bottom) bottom = m_pNowArea[i].y;
	}
	if(order == 0) ret.set(left,top);else
	if(order == 1) ret.set(right,top);else
	if(order == 2) ret.set(right,bottom);else
	if(order == 3) ret.set(left,bottom);

	return ret;
}
