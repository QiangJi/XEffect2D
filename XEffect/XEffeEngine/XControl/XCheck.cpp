//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XCheck.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XCheckTexture::_XCheckTexture()
:m_isInited(0)
,checkChoosed(NULL)			//ѡ��ťѡ�е���ͼ		
,checkDischoose(NULL)			//ѡ��ťδѡ�е���ͼ		
,checkDisableChoosed(NULL)		//��Ч״̬��ѡ��ťѡ�е���ͼ		
,checkDisableDischoose(NULL)	//��Ч״̬��ѡ��ťδѡ�е���ͼ	
{
}

_XCheckTexture::~_XCheckTexture()
{
	release();
}

int _XCheckTexture::init(const char *choosed,const char *disChoose,const char *disableChoosed,const char *disableDischoose,int resoursePosition)
{
	if(m_isInited != 0) return 0;
	//ע����������״̬����ͼ������Ҫ�У����򽫲��ܳ�ʼ��
	if(choosed == NULL || disChoose == NULL || disableChoosed == NULL || disableDischoose == NULL) return 0;
	int ret = 1;
	
	if((checkChoosed = createATextureData(choosed,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(checkDischoose = createATextureData(disChoose,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(checkDisableChoosed = createATextureData(disableChoosed,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(checkDisableDischoose = createATextureData(disableDischoose,resoursePosition)) == NULL) ret = 0;
	
	if(ret == 0)
	{
		XDELETE(checkChoosed);
		XDELETE(checkDischoose);
		XDELETE(checkDisableChoosed);
		XDELETE(checkDisableDischoose);
		return 0;
	}

	m_isInited = 1;
	return 1;
}

void _XCheckTexture::release()
{
	if(m_isInited == 0) return;
	XDELETE(checkChoosed);
	XDELETE(checkDischoose);
	XDELETE(checkDisableChoosed);
	XDELETE(checkDisableDischoose);
	m_isInited = 0;
}

_XCheck::_XCheck()
:m_isInited(0)					//�������Ƿ񱻳�ʼ��
,m_checkChoosed(NULL)			//ѡ��ťѡ�е���ͼ
,m_checkDischoose(NULL)		//ѡ��ťδѡ�е���ͼ
,m_checkDisableChoosed(NULL)	//��Ч״̬��ѡ��ťѡ�е���ͼ
,m_checkDisableDischoose(NULL)	//��Ч״̬��ѡ��ťδѡ�е���ͼ
,m_funInit(NULL)				//�ؼ���ʼ����ʱ����ã������Ŀǰû��ʵ����Ч��
,m_funRelease(NULL)				//�ؼ�ע����ʱ����ã������Ŀǰû��ʵ����Ч��
,m_funMouseOn(NULL)				//�������ʱ����
,m_funMouseDown(NULL)			//��갴��ʱ����		
,m_funMouseUp(NULL)				//��굯��ʱ����
,m_funStateChange(NULL)			//�ؼ�״̬�ı�ʱ����
,m_pClass(NULL)
,m_state(false)
,m_withCaption(1)
{
}

_XCheck::~_XCheck()
{
	release();
}

void _XCheck::release()
{
	_XControlManager::GetInstance().decreaseAObject(this);	//ע��������
	if(m_funRelease != NULL) (* m_funRelease)(m_pClass,m_objectID);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
}

//ע�⣺����Ĭ���ĸ�״̬����ͼ�ߴ�һ�£����򽫻�������벻��������
int _XCheck::init(const _XVector2 & position,	//�ؼ���λ��
		const _XRect &Area,					//�ؼ���ͼƬ����Ӧ��Χ
		const _XCheckTexture &tex,			//�ؼ�����ͼ
		const char *caption,const _XFontUnicode &font,float captionSize,	//��ʾ������������Ϣ
		const _XVector2 &textPosition)		//�����λ��
{
	if(m_isInited != 0) return 0;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return 0;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	m_position = position;
	m_mouseRect = Area;

	if(tex.checkChoosed == NULL || tex.checkDisableChoosed == NULL 
		|| tex.checkDisableDischoose == NULL || tex.checkDischoose == NULL) return 0;
	m_checkChoosed = tex.checkChoosed;			//ѡ��ťѡ�е���ͼ
	m_checkDischoose = tex.checkDischoose;		//ѡ��ťδѡ�е���ͼ
	m_checkDisableChoosed = tex.checkDisableChoosed;	//��Ч״̬��ѡ��ťѡ�е���ͼ
	m_checkDisableDischoose = tex.checkDisableDischoose;	//��Ч״̬��ѡ��ťδѡ�е���ͼ

	m_caption.setACopy(font);			//�������ı���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(_FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(_FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_size.set(1.0f,1.0f);
	m_textPosition = textPosition;		//���ֵ����λ��

	m_sprite.init(m_checkChoosed->texture.m_w,m_checkChoosed->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(captionSize < 0) return 0;
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_state = false;				//��ѡ���ѡ��״̬
	if(m_funInit != NULL) (* m_funInit)(m_pClass,m_objectID);

	m_isVisiable = 1;
	m_isEnable = 1;
	m_isActive = 1;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_CHECK);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = 1;
	return 1;
}

void _XCheck::draw()
{
	if(m_isInited == 0) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isVisiable == 0) return;	//������ɼ�ֱ���˳�
	if(m_state)
	{
		if(m_isEnable != 0)m_sprite.draw(m_checkChoosed);
		else m_sprite.draw(m_checkDisableChoosed);
	}else
	{
		if(m_isEnable != 0)m_sprite.draw(m_checkDischoose);
		else m_sprite.draw(m_checkDisableDischoose);
	}
	if(m_withCaption != 0) m_caption.draw();	//�����Ҫ�������������
}

void _XCheck::setPosition(float x,float y)
{
	if(m_isInited == 0) return;	//���û�г�ʼ��ֱ���˳�
	m_position.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_sprite.setPosition(m_position);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
}

void _XCheck::setSize(float x,float y)
{
	if(x <= 0 && y <= 0) return;
	if(m_isInited == 0) return;	//���û�г�ʼ��ֱ���˳�
	m_size.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_sprite.setSize(m_size);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
}

void _XCheck::setText(const char *temp)
{
	if(temp == NULL) return;
	m_caption.setString(temp);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
}

int _XCheck::canGetFocus(float x,float y)
{
	if(m_isInited == 0) return 0;	//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 0;		//û�м���Ŀؼ������տ���
	if(m_isVisiable == 0) return 0;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�
	if(m_withCaption != 0)
	{
		if(m_mouseClickArea.isInRect(x,y) || m_nowMouseRect.isInRect(x,y)) return 1;
	}else
	{
		if(m_nowMouseRect.isInRect(x,y)) return 1;
	}
	return 0;
}

int _XCheck::canLostFocus(float x,float y)
{
	return 1;
}

int _XCheck::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(m_isInited == 0) return 0;	//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 0;		//û�м���Ŀؼ������տ���
	if(m_isVisiable == 0) return 0;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�

	if(mouseState == _XMOUSE_LEFT_BUTTON_UP)
	{
		if(m_withCaption != 0)
		{
			if(m_mouseClickArea.isInRect(x,y) || m_nowMouseRect.isInRect(x,y))
			{//������Ч��ִ����ز���
				m_state = !m_state;
				if(m_funMouseUp != NULL) (* m_funMouseUp)(m_pClass,m_objectID);
				if(m_funStateChange != NULL) (* m_funStateChange)(m_pClass,m_objectID);
				return 1;
			}
		}else
		{
			if(m_nowMouseRect.isInRect(x,y))
			{//������Ч��ִ����ز���
				m_state = !m_state;
				if(m_funMouseUp != NULL) (* m_funMouseUp)(m_pClass,m_objectID);
				if(m_funStateChange != NULL) (* m_funStateChange)(m_pClass,m_objectID);
				return 1;
			}
		}
	}
	return 0;
}

int _XCheck::keyboardProc(int keyOrder,_XKeyState keyState)
{//�س����߿ո���Ըı�����ؼ���ѡ��״̬
	if(m_isInited == 0) return 0;	//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 0;		//û�м���Ŀؼ������տ���
	if(m_isVisiable == 0) return 0;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�

	if(keyState == _XKEY_STATE_UP)
	{//��������ʱ������Ӧ
		if((keyOrder == SDLK_RETURN || keyOrder == SDLK_SPACE) && m_isBeChoose != 0)
		{//���¿ո�����߻س�������Ч
			m_state = !m_state;
			if(m_funStateChange != NULL)(* m_funStateChange)(m_pClass,m_objectID);
		}
	}
	return 1;
}

void _XCheck::setCallbackFun(void (* funInit)(void *,int),
							 void (* funRelease)(void *,int),
							 void (* funMouseOn)(void *,int),
							 void (* funMouseDown)(void *,int),
							 void (* funMouseUp)(void *,int),
							 void (*funStateChange)(void *,int),void *pClass)
{//ʵ����������Բ������жϣ��������������ע��
	if(funInit != NULL) m_funInit = funInit;				//�ؼ���ʼ����ʱ����ã������Ŀǰû��ʵ����Ч��
	if(funRelease != NULL) m_funRelease = funRelease;				//�ؼ�ע����ʱ����ã������Ŀǰû��ʵ����Ч��
	if(funMouseOn != NULL) m_funMouseOn = funMouseOn;				//�������ʱ����
	if(funMouseDown != NULL) m_funMouseDown = funMouseDown;			//��갴��ʱ����		
	if(funMouseUp != NULL) m_funMouseUp = funMouseUp;				//��굯��ʱ����
	if(funStateChange != NULL) m_funStateChange = funStateChange;			//�ؼ�״̬�ı�ʱ����
	m_pClass = pClass;
}

int _XCheck::setACopy(const _XCheck &temp)
{
	if(& temp == this) return 1;	//��ֹ����ֵ
	if(temp.m_isInited == 0) return 0;
	if(_XControlBasic::setACopy(temp) == 0) return 0;

	if(m_isInited == 0)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_CHECK);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}

	m_isInited = temp.m_isInited;					//�������Ƿ񱻳�ʼ��
	if(m_caption.setACopy(temp.m_caption) == 0)	return 0;		//�������ı���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif

	m_checkChoosed = temp.m_checkChoosed;			//ѡ��ťѡ�е���ͼ
	m_checkDischoose = temp.m_checkDischoose;		//ѡ��ťδѡ�е���ͼ
	m_checkDisableChoosed = temp.m_checkDisableChoosed;	//��Ч״̬��ѡ��ťѡ�е���ͼ
	m_checkDisableDischoose = temp.m_checkDisableDischoose;	//��Ч״̬��ѡ��ťδѡ�е���ͼ

	m_funInit = temp.m_funInit;				//�ؼ���ʼ����ʱ����ã������Ŀǰû��ʵ����Ч��
	m_funRelease = temp.m_funRelease;				//�ؼ�ע����ʱ����ã������Ŀǰû��ʵ����Ч��
	m_funMouseOn = temp.m_funMouseOn;				//�������ʱ���ã������Ŀǰû��ʵ����Ч��
	m_funMouseDown = temp.m_funMouseDown;			//��갴��ʱ���ã������Ŀǰû��ʵ����Ч��		
	m_funMouseUp = temp.m_funMouseUp;				//��굯��ʱ����
	m_funStateChange = temp.m_funStateChange;			//�ؼ�״̬�ı�ʱ����
	m_pClass = temp.m_pClass;

	m_withCaption = temp.m_withCaption;
	m_sprite.setACopy(temp.m_sprite);			//������ʾ��ͼ�ľ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_textPosition = temp.m_textPosition;		//���ֵ����λ��
	m_textSize = temp.m_textSize;			//���ֵĳߴ�

	m_state = temp.m_state;				//��ѡ���ѡ��״̬
	m_textColor = temp.m_textColor;				//��ѡ���ѡ��״̬
	m_mouseClickArea = temp.m_mouseClickArea;	//���������Ӧ��Χ

	return 1;
}

int _XCheck::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}

_XVector2 _XCheck::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;
	float left = 0.0f;
	float right = 0.0f;
	float top = 0.0f;
	float bottom = 0.0f;
	if(m_withCaption != 0)
	{
		if(m_nowMouseRect.left <= m_mouseClickArea.left) left = m_nowMouseRect.left;
		else left = m_mouseClickArea.left;
		if(m_nowMouseRect.top <= m_mouseClickArea.top) top = m_nowMouseRect.top;
		else top = m_mouseClickArea.top;
		if(m_nowMouseRect.right <= m_mouseClickArea.right) right = m_mouseClickArea.right;
		else right = m_nowMouseRect.right;
		if(m_nowMouseRect.bottom <= m_mouseClickArea.bottom) bottom = m_mouseClickArea.bottom;
		else bottom = m_nowMouseRect.bottom;
	}else
	{
		left = m_nowMouseRect.left;
		top = m_nowMouseRect.top;
		right = m_mouseClickArea.right;
		bottom = m_mouseClickArea.bottom;
	}

	if(order == 0) ret.set(left,top);else
	if(order == 1) ret.set(right,top);else
	if(order == 2) ret.set(right,bottom);else
	if(order == 3) ret.set(left,bottom);

	return ret;
}
