//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XRadios.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XRadios::_XRadios()
:m_isInited(0)			//�ռ�ʱ���Ѿ���ʼ��
,m_radioSum(1)			//ѡ�������
,m_nowChoose(0)			//��ǰ��ѡ����ı��
,m_radio(NULL)			//���е�ѡ���ָ��
,m_checkPosition(NULL)
,m_funStateChange(NULL)
{
}

_XRadios::~_XRadios()
{
	release();
}

void _XRadios::release()	//�ͷŷ������Դ
{
	if(m_isInited == 0) return;
	XDELETE_ARRAY(m_radio);
	XDELETE_ARRAY(m_checkPosition);
	_XControlManager::GetInstance().decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	m_isInited = 0;
}

int _XRadios::init(int radioSum,			//ѡ�������
		const _XVector2& distance,
		const _XVector2& position,	//�ؼ���λ��
		const _XRect &Area,			//ѡ��ͼ��������Ӧ��Χ
		const _XRadiosTexture *tex,const _XFontUnicode &font,float captionSize,
		const _XVector2& textPosition)	//��ѡ���ʼ��
{
	if(m_isInited != 0) return 0;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return 0;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	if(radioSum < 1) return 0;	//����û��ѡ�����Ҫ��һ��ѡ��
	if(captionSize <= 0) return 0;
	m_mouseRect = Area;
	m_position = position;
	m_distance = distance;

	if(tex->checkChoosed == NULL || tex->checkDisableChoosed == NULL 
		|| tex->checkDisableDischoose == NULL || tex->checkDischoose == NULL) return 0;//�����ͼȱ�٣�Ҳֱ�ӷ���ʧ��
	//�����ڴ�ռ�
	m_radioSum = radioSum;
	try
	{
		m_radio = new _XCheck[m_radioSum];
		if(m_radio == NULL) return 0;//����ڴ����ʧ�ܣ�ֱ�ӷ��ش���
	}catch(...)
	{
		return 0;
	}
	try
	{
		m_checkPosition = new _XVector2[m_radioSum];
		if(m_checkPosition == NULL)
		{
			XDELETE_ARRAY(m_radio);
			return 0;
		}
	}catch(...)
	{
		XDELETE_ARRAY(m_radio);
		return 0;
	}
	m_size.set(1.0f,1.0f);

	m_texture = tex;
	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(_FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(_FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ
	m_captionSize = captionSize;
	m_textPosition = textPosition;

#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif
	//��ʼ�����е�ѡ��
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_checkPosition[i].set(m_distance.x * i,m_distance.y * i);
		if(m_radio[i].init(_XVector2(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y),
			m_mouseRect,* m_texture," ",m_caption,m_captionSize,m_textPosition) == 0)
		{
			XDELETE_ARRAY(m_radio);
			XDELETE_ARRAY(m_checkPosition);
			return 0;
		}
		m_radio[i].setState(false);
	//�������������ע������Щ���
		_XControlManager::GetInstance().decreaseAObject(&(m_radio[i]));
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_radio[i]));
#endif
	}

#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif
	m_nowChoose = 0;
	m_radio[0].setState(true);

	m_isVisiable = 1;
	m_isEnable = 1;
	m_isActive = 1;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_RADIOS);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = 1;
	return 1;
}

void _XRadios::disable()
{
	if(m_isInited == 0) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isEnable != 0)
	{
		m_isEnable = 0;
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].disable();
		}
	}
}

void _XRadios::enable()
{
	if(m_isInited == 0) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isEnable == 0)
	{
		m_isEnable = 1;
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].enable();
		}
	}
}

void _XRadios::draw()
{	
	if(m_isInited == 0) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isVisiable == 0) return;	//������ɼ�ֱ���˳�
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].draw();
	}
}

void _XRadios::setChoosed(int temp)
{
	if(temp < 0 || temp >= m_radioSum) return;
	if(temp == m_nowChoose) return;
	m_nowChoose = temp;
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setState(false);
	}
	m_radio[m_nowChoose].setState(true);
	if(m_funStateChange != NULL) (* m_funStateChange)(m_pClass,m_objectID);
}


void _XRadios::setRadioPosition(float x,float y,int order)
{
	if(m_isInited == 0) return ;	//���û�г�ʼ��ֱ���˳�
	if(order < 0 || order >= m_radioSum) return;
	m_checkPosition[order].set(x,y);
	m_radio[order].setPosition(m_position.x + x * m_size.x,m_position.y + y * m_size.y);
}

int _XRadios::canGetFocus(float x,float y)
{
	if(m_isInited == 0) return 0;	//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 0;		//û�м���Ŀؼ������տ���
	if(m_isVisiable == 0) return 0;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}

int _XRadios::canLostFocus(float x,float y)
{
	return 1;
}

int _XRadios::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(m_isInited == 0) return 0;	//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 0;		//û�м���Ŀؼ������տ���
	if(m_isVisiable == 0) return 0;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�

	int tempOrder = -1;	//��¼��������ѡ��ı��
	for(int i = 0;i < m_radioSum;++ i)
	{
		if(m_radio[i].mouseProc(x,y,mouseState) != 0)
		{//����Ĭ��ֻ����һ��ѡ�����
			if(m_nowChoose != i) tempOrder = i;
			else m_radio[m_nowChoose].setState(true);
		}
	}
	if(tempOrder != -1)
	{
		//printf("NowChoose:%d,%d\n",tempOrder,m_objectID);
		m_nowChoose = tempOrder;
		for(int i = 0;i < m_radioSum;++ i)
		{
			m_radio[i].setState(false);
		}
		m_radio[m_nowChoose].setState(true);
		if(m_funStateChange != NULL) (* m_funStateChange)(m_pClass,m_objectID);
	}
	return 1;
}

int _XRadios::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(m_isInited == 0) return 0;	//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 0;		//û�м���Ŀؼ������տ���
	if(m_isVisiable == 0) return 0;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�

	if(keyState == _XKEY_STATE_UP && m_isBeChoose != 0)
	{//��������ʱ������Ӧ
		if(keyOrder == SDLK_LEFT || keyOrder == SDLK_UP)
		{//������������
			--m_nowChoose;
			if(m_nowChoose < 0) m_nowChoose = m_radioSum - 1;
			for(int i = 0;i < m_radioSum;++ i)
			{
				m_radio[i].setState(false);
			}
			m_radio[m_nowChoose].setState(true);
			if(m_funStateChange != NULL) (* m_funStateChange)(m_pClass,m_objectID);
		}else
		if(keyOrder == SDLK_RIGHT || keyOrder == SDLK_DOWN)
		{//������������
			++m_nowChoose;
			if(m_nowChoose >= m_radioSum) m_nowChoose = 0;
			for(int i = 0;i < m_radioSum;++ i)
			{
				m_radio[i].setState(false);
			}
			m_radio[m_nowChoose].setState(true);
			if(m_funStateChange != NULL) (* m_funStateChange)(m_pClass,m_objectID);
		}
	}
	return 1;
}

void _XRadios::setSize(float x,float y)
{
	if(m_isInited == 0) return;	//���û�г�ʼ��ֱ���˳�
	if(x <= 0 && y <= 0) return;
	m_size.set(x,y);
	//�ƶ�����ѡ��ؼ������λ��
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setPosition(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y);
		m_radio[i].setSize(m_size);
	}
}

void _XRadios::setPosition(float x,float y)
{
	if(m_isInited == 0) return;	//���û�г�ʼ��ֱ���˳�
	m_position.set(x,y);
	//�ƶ�����ѡ��ؼ������λ��
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_radio[i].setPosition(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y);
	}
}

int _XRadios::setRadioSum(int radioSum)
{
	if(m_isInited == 0) return 0;	//���û�г�ʼ��ֱ���˳�
	if(radioSum <= 0) return 0;
#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif
	XDELETE_ARRAY(m_radio);
	XDELETE_ARRAY(m_checkPosition);
#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif
	m_radioSum = radioSum;
	try
	{
		m_radio = new _XCheck[m_radioSum];
		if(m_radio == NULL) return 0;//����ڴ����ʧ�ܣ�ֱ�ӷ��ش���
	}catch(...)
	{
		return 0;
	}
#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif
	try
	{
		m_checkPosition = new _XVector2[m_radioSum];
		if(m_checkPosition == NULL)
		{
			XDELETE_ARRAY(m_radio);
			return 0;
		}
	}catch(...)
	{
		XDELETE_ARRAY(m_radio);
		return 0;
	}
	//��ʼ�����е�ѡ��
	for(int i = 0;i < m_radioSum;++ i)
	{
		m_checkPosition[i].set(m_distance.x * i,m_distance.y * i);
		if(m_radio[i].init(_XVector2(m_position.x + m_checkPosition[i].x * m_size.x,m_position.y + m_checkPosition[i].y * m_size.y),
			m_mouseRect,* m_texture," ",m_caption,m_captionSize,m_textPosition) == 0)
		{
			XDELETE_ARRAY(m_radio);
			XDELETE_ARRAY(m_checkPosition);
			return 0;
		}
		//�ڿؼ���������ע������Щ���
		_XControlManager::GetInstance().decreaseAObject(&(m_radio[i]));	
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_radio[i]));
#endif
		m_radio[i].setState(false);
	}
#if WITH_OBJECT_MANAGER
	//printf("Now obj sum:%d\n",_XObjectManager::GetInstance().getNowObjectSum());
#endif

	m_nowChoose = 0;
	m_radio[0].setState(true);
	return 1;
}

int _XRadios::setACopy(const _XRadios &temp)
{
	if(&temp == this) return 0;
	if(temp.m_isInited == 0) return 0;
	if(m_isInited != 0) release();
	if(_XControlBasic::setACopy(temp) == 0) return 0;
	if(m_isInited == 0)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_RADIOS);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}

	m_isInited = temp.m_isInited;
	m_radioSum = temp.m_radioSum;	//ѡ�������
	m_nowChoose = temp.m_nowChoose;	//��ǰ��ѡ����ı��
	m_distance = temp.m_distance;	//��ǰ��ѡ����ı��

	m_texture = temp.m_texture;
	m_caption.setACopy(temp.m_caption);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_captionSize = temp.m_captionSize;
	m_textColor = temp.m_textColor;
	m_textPosition = temp.m_textPosition;

	try
	{
		m_radio = new _XCheck[m_radioSum];
		if(m_radio == NULL) return 0;//����ڴ����ʧ�ܣ�ֱ�ӷ��ش���
	}catch(...)
	{
		return 0;
	}
	try
	{
		m_checkPosition = new _XVector2[m_radioSum];
		if(m_checkPosition == NULL)
		{
			XDELETE_ARRAY(m_radio);
			return 0;
		}
	}catch(...)
	{
		XDELETE_ARRAY(m_radio);
		return 0;
	}
	for(int i = 0;i < m_radioSum;++i)
	{
		if(m_radio[i].setACopy(temp.m_radio[i]) == 0) return 0;
		m_checkPosition[i] = temp.m_checkPosition[i];
		//�ڿؼ���������ע������Щ���
		_XControlManager::GetInstance().decreaseAObject(&(m_radio[i]));	
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_radio[i]));
#endif
	}

	m_funStateChange = temp.m_funStateChange;			//�ؼ�״̬�ı�ʱ����
	m_pClass = temp.m_pClass;
	return 1;
}

int _XRadios::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}

_XVector2 _XRadios::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	_XVector2 temp;
	if(m_isInited == 0) return ret;
	//�������еİ�ť���ҵ�����Χ�Ŀ�ÿ�ζ��������᲻��̫��Ч��
	if(order == 0)
	{
		ret = m_radio[0].getBox(0);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(0);
			if(temp.x < ret.x) ret.x = temp.x;
			if(temp.y < ret.y) ret.y = temp.y;
		}
	}else
	if(order == 1)
	{
		ret = m_radio[0].getBox(1);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(1);
			if(temp.x > ret.x) ret.x = temp.x;
			if(temp.y < ret.y) ret.y = temp.y;
		}
	}else
	if(order == 2)
	{
		ret = m_radio[0].getBox(2);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(2);
			if(temp.x > ret.x) ret.x = temp.x;
			if(temp.y > ret.y) ret.y = temp.y;
		}
	}else
	if(order == 3)
	{
		ret = m_radio[0].getBox(3);
		for(int i = 0;i < m_radioSum;++i)
		{
			temp = m_radio[i].getBox(3);
			if(temp.x < ret.x) ret.x = temp.x;
			if(temp.y > ret.y) ret.y = temp.y;
		}
	}
	return ret;
}