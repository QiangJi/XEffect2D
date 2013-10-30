//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XProgress.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XProgressTexture::_XProgressTexture()
:m_isInited(0)
,progressBackGround(NULL)			//�������ı�����ͼ
,progressMove(NULL)				//���������ƶ��仯����ͼ
,progressUpon(NULL)				//���������ϲ�����ֻ��������Ч
{
}

_XProgressTexture::~_XProgressTexture()
{
	release();
}

int _XProgressTexture::init(const char *backgound,const char *move,const char *upon,int resoursePosition)
{
	if(m_isInited != 0) return 0;
	if(move == NULL) return 0;	//���������ٱ���Ҫ���м��ƶ�����ͼ
	int ret = 1;
	if(backgound != NULL && 
		(progressBackGround = createATextureData(backgound,resoursePosition)) == NULL) ret = 0;
	if(move != NULL && ret != 0 && 
		(progressMove = createATextureData(move,resoursePosition)) == NULL) ret = 0;
	if(upon != NULL && ret != 0 &&
		(progressUpon = createATextureData(upon,resoursePosition)) == NULL) ret = 0;

	if(ret == 0)
	{
		XDELETE(progressBackGround);
		XDELETE(progressMove);
		XDELETE(progressUpon);
		return 0;
	}
	m_isInited = 1;
	return 1;
}

void _XProgressTexture::release()
{
	if(m_isInited == 0) return;
	XDELETE(progressBackGround);
	XDELETE(progressMove);
	XDELETE(progressUpon);
	m_isInited = 0;
}

int _XProgress::init(const _XVector2& position,//�ؼ����ڵ�λ��
		const _XRect &Area,	//����������ķ�Χ
		const _XProgressTexture &tex,	//�ؼ�����ͼ
		_XNumber *font,float captionSize,const _XVector2& textPosition,	//�ؼ�������
		int mode)
{
	if(m_isInited != 0) return 0;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return 0;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	if(tex.progressMove == NULL) return 0;//��������ͨ״̬����ͼ����Ϊ�գ�����ֱ�ӷ��ش���
	if(captionSize < 0) return 0;
	m_mouseRect = Area;
	m_position = position;

	if(mode != 0) m_mode = 1;
	else m_mode = 0;

	m_progressBackGround = tex.progressBackGround;		
	m_progressMove = tex.progressMove;				
	m_progressUpon = tex.progressUpon;		

	m_nowValue = 0.0f;
	if(font == NULL) m_isShowFont = 0;
	else
	{
		m_caption.setACopy(* font);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
		m_isShowFont = 1;
	}
	m_size.set(1.0f,1.0f);
	if(m_mode == 0)
	{
		m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
			m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	}else
	{
		m_nowMouseRect.set(m_position.x - m_mouseRect.top * m_size.y,m_position.y + m_mouseRect.left * m_size.x,
			m_position.x - m_mouseRect.bottom * m_size.y,m_position.y + m_mouseRect.right * m_size.x);
	}

	//��ʼ��������ʾ����Ϣ
	m_caption.setNumber("0%");
	m_caption.setAlignmentMode(_NUMBER_ALIGNMENT_MODE_MIDDLE);
	m_textPosition = textPosition;
	if(m_mode != 0)
	{
		m_caption.setAngle(90);
		m_caption.setPosition(m_position.x - m_textPosition.y * m_size.y,m_position.y + m_textPosition.x * m_size.x);
	}else m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);

	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	//��ʼ����ʾ�ľ���
	m_spriteBackground.init(m_progressBackGround->texture.m_w,m_progressBackGround->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackground);
#endif
	m_spriteBackground.setPosition(m_position);
	m_spriteBackground.setSize(m_size);
	m_spriteBackground.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteMove.init(m_progressMove->texture.m_w,m_progressMove->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteMove);
#endif
	m_spriteMove.setPosition(m_position);
	m_spriteMove.setSize(m_size);
	m_spriteMove.setIsTransformCenter(POINT_LEFT_TOP);
	m_spriteMove.setClipRect(m_mouseRect.left,m_mouseRect.top,m_mouseRect.left + 1.0f,m_mouseRect.bottom);

	m_spriteUpon.init(m_progressUpon->texture.m_w,m_progressUpon->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteUpon);
#endif
	m_spriteUpon.setPosition(m_position);
	m_spriteUpon.setSize(m_size);
	m_spriteUpon.setIsTransformCenter(POINT_LEFT_TOP);
	if(m_mode != 0)
	{
		m_caption.setAngle(90);
		m_spriteBackground.setAngle(90);
		m_spriteMove.setAngle(90);
		m_spriteUpon.setAngle(90);
	}

	m_isVisiable = 1;
	m_isEnable = 1;
	m_isActive = 1;
	//ע������ؼ�
	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_PROGRESS);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif

	m_isInited = 1;
	return 1;
}

void _XProgress::setSize(float x,float y)
{
	if(x <= 0 && y <= 0) return;
	m_size.set(x,y);
	m_spriteBackground.setSize(m_size);
	m_spriteMove.setSize(m_size);
	m_spriteUpon.setSize(m_size);
	if(m_mode == 0)
	{
		m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
		m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
			m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	}else
	{
		m_caption.setPosition(m_position.x - m_textPosition.y * m_size.y,m_position.y + m_textPosition.x * m_size.x);
		m_nowMouseRect.set(m_position.x - m_mouseRect.top * m_size.y,m_position.y + m_mouseRect.left * m_size.x,
			m_position.x - m_mouseRect.bottom * m_size.y,m_position.y + m_mouseRect.right * m_size.x);
	}
	m_caption.setSize(m_size);
}

void _XProgress::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_spriteBackground.setPosition(m_position);
	m_spriteMove.setPosition(m_position);
	m_spriteUpon.setPosition(m_position);
	if(m_mode == 0)
	{
		m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
		m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
			m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	}else
	{
		m_caption.setPosition(m_position.x - m_textPosition.y * m_size.y,m_position.y + m_textPosition.x * m_size.x);
		m_nowMouseRect.set(m_position.x - m_mouseRect.top * m_size.y,m_position.y + m_mouseRect.left * m_size.x,
			m_position.x - m_mouseRect.bottom * m_size.y,m_position.y + m_mouseRect.right * m_size.x);
	}
}

void _XProgress::draw()
{
	if(m_isInited == 0) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isVisiable == 0) return;	//������ɼ�ֱ���˳�
	if(m_progressBackGround != NULL) m_spriteBackground.draw(m_progressBackGround);
	if(m_progressMove != NULL) m_spriteMove.draw(m_progressMove);
	if(m_isShowFont != 0) m_caption.draw();	//��ʾ����
	if(m_progressUpon != NULL) m_spriteUpon.draw(m_progressUpon);
}

void _XProgress::setValue(float temp)
{
	if(m_isInited == 0) return;
	if(temp < 0.0f) temp = 0.0f;
	if(temp > 100.0f) temp = 100.0f;
	int tempI = temp;
	if(tempI == (int)m_nowValue) return;
	m_nowValue = temp;
	char tempC[20] = "100%";
	sprintf(tempC,"%d%%",tempI);
	m_caption.setNumber(tempC);

	m_spriteMove.setClipRect(m_mouseRect.left,m_mouseRect.top,m_mouseRect.left + m_mouseRect.getWidth() * m_nowValue/100.0f,m_mouseRect.bottom);
}

_XProgress::_XProgress()
:m_isInited(0)
,m_progressBackGround(NULL)		
,m_progressMove(NULL)			
,m_progressUpon(NULL)		
,m_nowValue(0.0f)
,m_isShowFont(1)
,m_isACopy(0)
{
}

_XProgress::~_XProgress()
{
	release();
}

void _XProgress::release()
{
	if(m_isInited == 0) return;
	_XControlManager::GetInstance().decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	m_isInited = 0;
}

int _XProgress::setACopy(const _XProgress &temp)		//����һ������
{
	if(&temp == this) return 0;
	if(temp.m_isInited == 0) return 0;
	if(_XControlBasic::setACopy(temp) == 0) return 0;

	m_nowValue = temp.m_nowValue;					//�������ĵ�ǰֵ
	if(m_isInited == 0)
	{//���û�о�����ʼ���ͽ��и����Ļ�������Ҫע��ؼ�
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_PROGRESS);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}
	m_isInited = temp.m_isInited;					//�������Ƿ񱻳�ʼ��
	m_mode = temp.m_mode;					//�������ĵ�ǰֵ
	if(temp.m_isShowFont != 0)
	{
		if(m_caption.setACopy(temp.m_caption) == 0)	return 0;				//�������ı���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	}

	m_progressBackGround = temp.m_progressBackGround;	//�������ı�����ͼ
	m_progressMove = temp.m_progressMove;			//���������ƶ��仯����ͼ
	m_progressUpon = temp.m_progressUpon;			//���������ϲ�����ֻ��������Ч

	m_spriteBackground.setACopy(temp.m_spriteBackground);	//������ʾ�����ľ���
	m_spriteMove.setACopy(temp.m_spriteMove);			//������ʾ��ͼ�ľ���
	//m_movePosition = temp.m_movePosition;			//����������ڱ�����λ��
	m_spriteUpon.setACopy(temp.m_spriteUpon);			//������ʾ�ϲ����ֵľ���
	//m_uponPosition = temp.m_uponPosition;			//��������ڱ�����λ��
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackground);
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteMove);
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteUpon);
#endif
	m_textPosition = temp.m_textPosition;			//������ʾ��λ��
	m_textSize = temp.m_textSize;				//���ֵĳߴ�
	m_textColor = temp.m_textColor;				//���ֵĳߴ�

	m_isShowFont = temp.m_isShowFont;				//�Ƿ���ʾ��������
	m_isACopy = 1;
	return 1;
}

int _XProgress::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}

_XVector2 _XProgress::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;
	if(order == 0) ret.set(m_nowMouseRect.left,m_nowMouseRect.top);else
	if(order == 1) ret.set(m_nowMouseRect.right,m_nowMouseRect.top);else
	if(order == 2) ret.set(m_nowMouseRect.right,m_nowMouseRect.bottom);else
	if(order == 3) ret.set(m_nowMouseRect.left,m_nowMouseRect.bottom);
	return ret;
}
