//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XPointCtrl.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

_XBool _XPointCtrl::init(const _XVector2& position,const _XFontUnicode *font)
{
	if(m_isInited) return false;
	m_position = position;
	if(font == NULL)
	{
		m_withFont = false;
	}else
	{
		m_withFont = true;
		m_font.setACopy(*font);
		m_font.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
		m_font.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_font);
#endif
	}
	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_POINTCTRL);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	updateData();

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	m_isInited = true;
	return true;
}
void _XPointCtrl::draw()
{
	if(!m_isInited) return;
	if(!m_isVisiable) return;	//������ɼ�ֱ���˳�

	switch(m_ctrlMode)
	{
	case CTRL_MODE_COMPLETE:
		//�������
		if(m_withRange)
		{
			float w = m_range.getWidth() / XPOINT_CTRL_LINE_SUM;
			float h = m_range.getHeight() / XPOINT_CTRL_LINE_SUM;
			for(int i = 1;i < XPOINT_CTRL_LINE_SUM;++ i)
			{
				drawLine(m_range.left + w * i,m_range.top + 12.0f,m_range.left + w * i,m_range.bottom - 12.0f,1,1.0f,1.0f,1.0f,0.5f,1);
				drawLine(m_range.left + 12.0f,m_range.top + h * i,m_range.right - 12.0f,m_range.top + h * i,1,1.0f,1.0f,1.0f,0.5f,1);
			}
		}
	case CTRL_MODE_NORMAL:
		//���߿�ͱ��
		if(m_withRange) 
		{
			drawRect(m_range);
			float w = m_range.getWidth() / XPOINT_CTRL_LINE_SUM;
			float h = m_range.getHeight() / XPOINT_CTRL_LINE_SUM;
			for(int i = 1;i < XPOINT_CTRL_LINE_SUM;++ i)
			{
				drawLine(m_range.left + w * i,m_range.top,m_range.left + w * i,m_range.top + 10.0f);
				drawLine(m_range.left + w * i,m_range.bottom,m_range.left + w * i,m_range.bottom - 10.0f);
				drawLine(m_range.left,m_range.top + h * i,m_range.left + 10.0f,m_range.top + h * i);
				drawLine(m_range.right,m_range.top + h * i,m_range.right - 10.0f,m_range.top + h * i);
			}
		}
	case CTRL_MODE_SIMPLE:
		break;
	}
	//����ԭ��λ��
	if(m_isDown)
	{
		drawLine(m_position.x - m_truePixelSize.x,m_position.y,m_position.x + m_truePixelSize.x,m_position.y,2,1.0f,0.0f,0.0f);
		drawLine(m_position.x,m_position.y - m_truePixelSize.y,m_position.x,m_position.y + m_truePixelSize.y,2,1.0f,0.0f,0.0f);
	}else
	{
		drawLine(m_position.x - m_truePixelSize.x,m_position.y,m_position.x + m_truePixelSize.x,m_position.y,1,1.0f,1.0f,1.0f);
		drawLine(m_position.x,m_position.y - m_truePixelSize.y,m_position.x,m_position.y + m_truePixelSize.y,1,1.0f,1.0f,1.0f);
	}
	if(m_withFont) m_font.draw();	//��ʾ������ʾ
}
_XBool _XPointCtrl::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	switch(mouseState)
	{
	case MOUSE_MOVE:
		if(m_isDown) 
		{
			setPosition(x,y);
			if(m_funDataChange != NULL) m_funDataChange(m_pClass,m_objectID);
		}
		break;
	case MOUSE_LEFT_BUTTON_DOWN:
		//��갴��
		if(_XVector2(x,y).getLengthSqure(m_position) < 100.0f) m_isDown = true;
		break;
	case MOUSE_LEFT_BUTTON_UP:
		if(m_isDown) m_isDown = false;
		break;
	}
	return XTrue;
}
_XBool _XPointCtrl::setACopy(const _XPointCtrl & temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited) return XFalse;
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_POINTCTRL);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}

	m_isInited = temp.m_isInited;
	m_position = temp.m_position;	//�ؼ���λ��
	m_size = temp.m_size;		//��С
	m_pixelSize = temp.m_pixelSize;	//���ش�С
	m_truePixelSize = temp.m_truePixelSize;	//��ʵ�����سߴ�
	if(!m_font.setACopy(temp.m_font))	 return XFalse;
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_font);
#endif
	memcpy(m_textStr,temp.m_textStr,64);		//��ʾ���ַ���

	m_withFont = temp.m_withFont;
	m_isDown = temp.m_isDown;		//�Ƿ����ʰȡ
	m_withRange = temp.m_withRange;	//��Χ�����Ƿ���Ч
	m_range = temp.m_range;			//�����ƶ��ķ�Χ
	m_withMap = temp.m_withMap;		//�Ƿ����ӳ��
	m_mapRange = temp.m_mapRange;	//ӳ��ķ�Χ
	m_mapValue = temp.m_mapValue;	//ӳ��ķ�Χ

	m_ctrlMode = temp.m_ctrlMode;	//�ؼ�ģʽ

	m_funDataChange = temp.m_funDataChange;
	m_pClass = temp.m_pClass;	

	if(temp.m_withFont)
	{
		m_font.setACopy(temp.m_font);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_font);
#endif
	}

	return XTrue;
}
void _XPointCtrl::release()
{
	_XControlManager::GetInstance().decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
}
void _XPointCtrl::updateData()
{
	m_truePixelSize = m_pixelSize * m_size;
	if(m_withRange)
	{//�����ڷ�Χ��
		if(m_position.x < m_range.left) m_position.x = m_range.left;
		if(m_position.x > m_range.right) m_position.x = m_range.right;
		if(m_position.y < m_range.top) m_position.y = m_range.top;
		if(m_position.y > m_range.bottom) m_position.y = m_range.bottom;
	}
	if(m_withMap)
	{//�������ӳ��
		m_mapValue.x = maping1D(m_position.x,m_range.left,m_range.right,m_mapRange.left,m_mapRange.right);
		m_mapValue.y = maping1D(m_position.y,m_range.top,m_range.bottom,m_mapRange.top,m_mapRange.bottom);
		if(m_withFont)
		{
			sprintf(m_textStr,"(%.0f,%.0f)",m_mapValue.x,m_mapValue.y);
			m_font.setString(m_textStr);
			m_font.setPosition(m_position);
		}
	}else
	{
		if(m_withFont)
		{
			sprintf(m_textStr,"(%.0f,%.0f)",m_position.x,m_position.y);
			m_font.setString(m_textStr);
			m_font.setPosition(m_position);
		}
	}
}