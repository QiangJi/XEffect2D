//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XGroup.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
#include "XResourceManager.h"

void funXGroupStateBtn(void *pClass,int objectID)
{
	_XGroup * pPar = (_XGroup *)pClass;
	if(pPar->m_state == STATE_NORMAL) pPar->setState(STATE_MINISIZE);
	else pPar->setState(STATE_NORMAL);
}

_XBool _XGroup::init(const _XVector2& position,
	const _XRect &rect,
	const char *caption,const _XFontUnicode &font,float captionSize)
{
	if(m_isInited) return XFalse;	//防止重复初始化

	m_position = position;
	m_rect = rect;
	m_textSize.set(captionSize);
	m_size.set(1.0f,1.0f);
	m_caption.setACopy(font);
	m_caption.setSize(captionSize);
	m_caption.setString(caption);
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT);
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);

	m_state = STATE_NORMAL;
	if(!m_stateBotton.initWithoutTex("-",font,captionSize,_XRect(0.0f,0.0f,XGROUP_STATE_BTN_SIZE,XGROUP_STATE_BTN_SIZE),
		_XVector2(XGROUP_STATE_BTN_SIZE * 0.5f,XGROUP_STATE_BTN_SIZE * 0.5f))) return false;
	m_stateBotton.setCallbackFun(NULL,NULL,NULL,funXGroupStateBtn,NULL,this);
	updateData();

#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
	_XObjectManager::GetInstance().decreaseAObject(&m_stateBotton);
#endif
	//_XControlManager::GetInstance().decreaseAObject(&m_stateBotton);
	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_GROUP);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	m_isInited = XTrue;
	return XTrue;
}
void _XGroup::draw()
{
	if(!m_isInited) return;
	if(!m_isVisiable) return ;	//如果没有初始化直接退出

	if(m_state == STATE_NORMAL)
	{
		int h = m_caption.getMaxPixelHeight();
		int w = m_caption.getMaxPixelWidth();

		drawLine(m_drawRect.left,m_drawRect.top + (h >> 1),m_drawRect.left,m_drawRect.bottom,1);
		drawLine(m_drawRect.right,m_drawRect.top + (h >> 1),m_drawRect.right,m_drawRect.bottom,1);
		drawLine(m_drawRect.left,m_drawRect.bottom,m_drawRect.right,m_drawRect.bottom,1);

		//drawLine(m_drawRect.left,m_drawRect.top + (h >> 1),m_drawRect.left + 20.0f * m_size.x ,m_drawRect.top + (h >> 1),1);
		if(m_drawRect.getWidth() > w + XGROUP_STATE_BTN_SIZE + 10.0f)
			drawLine(m_drawRect.left + (XGROUP_STATE_BTN_SIZE + 10.0f) * m_size.x + w,m_drawRect.top + (h >> 1),m_drawRect.right,m_drawRect.top + (h >> 1),1);
	}else
	{
		drawLine(m_drawRect.left,m_drawRect.top,m_drawRect.left,m_drawRect.bottom,1);
		drawLine(m_drawRect.right,m_drawRect.top,m_drawRect.right,m_drawRect.bottom,1);
		drawLine(m_drawRect.left,m_drawRect.bottom,m_drawRect.right,m_drawRect.bottom,1);
		drawLine(m_drawRect.left,m_drawRect.top,m_drawRect.right,m_drawRect.top,1);
	}
	m_caption.draw();
	m_stateBotton.draw();
}
_XBool _XGroup::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
_XVector2 _XGroup::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;
	if(m_state == STATE_NORMAL)
	{
		if(order == 0) ret.set(m_drawRect.left,m_drawRect.top);else
		if(order == 1) ret.set(m_drawRect.right,m_drawRect.top);else
		if(order == 2) ret.set(m_drawRect.right,m_drawRect.bottom);else
		if(order == 3) ret.set(m_drawRect.left,m_drawRect.bottom);
	}else
	{
		if(order == 0) ret.set(m_drawRect.left,m_drawRect.top);else
		if(order == 1) ret.set(m_drawRect.right,m_drawRect.top);else
		if(order == 2) ret.set(m_drawRect.right,m_drawRect.bottom);else
		if(order == 3) ret.set(m_drawRect.left,m_drawRect.bottom);
	}

	return ret;
}
void _XGroup::release()
{
	_XControlManager::GetInstance().decreaseAObject(this);	//注销这个物件
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
}
_XBool _XGroup::setACopy(const _XGroup & temp)
{
	if(& temp == this) return XTrue;	//防止自身赋值
	if(!temp.m_isInited) return XFalse;
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_GROUP);	//在物件管理器中注册当前物件
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}

	m_isInited = temp.m_isInited;
	m_position = temp.m_position;	//控件的位置
	m_size = temp.m_size;		//大小
	m_rect = temp.m_rect;			//控件的范围
	m_funStateChange = temp.m_funStateChange;	//状态发生改变时调用的函数
	m_pClass = temp.m_pClass;

	m_caption.setACopy(temp.m_caption);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_textSize = temp.m_textSize;

	return XTrue;
}