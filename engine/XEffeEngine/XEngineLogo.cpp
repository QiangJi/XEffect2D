//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XEngineLogo.h"
#include "XResourcePack.h"

int _XEngineLogo::init(_XVector2 position,_XResourcePosition resoursePosition,int mode)
{
	if(m_isInited != 0) return 0;	//重复初始化

	m_resoursePosition = resoursePosition;

	if(mode != 0) m_mode = 1;
	else m_mode = 0;
	setPosition(position);

	if(m_back.init("ResourcePack/pic/Back.png",m_resoursePosition) == 0) return 0;
//	m_back.setPosition(m_position.x - 160,m_position.y - 96);	//居中
//	m_back.setPosition(0,0);	//全窗口
	m_back.setIsTransformCenter(POINT_LEFT_TOP);
	if(m_logoBack.init("ResourcePack/pic/logo/Back.png",m_resoursePosition) == 0) return 0;
	m_logoBack.setPosition(m_position);
	if(m_logoLight.init("ResourcePack/pic/logo/Light.png",m_resoursePosition) == 0) return 0;
	m_logoLight.setPosition(m_position);
	if(m_logoMiddle.init("ResourcePack/pic/logo/Middle.png",m_resoursePosition) == 0) return 0;
	m_logoMiddle.setPosition(m_position);
	if(m_logoTextX.init("ResourcePack/pic/logo/X.png",m_resoursePosition) == 0) return 0;
	m_logoTextX.setPosition(m_position);
	if(m_logoTextE.init("ResourcePack/pic/logo/E.png",m_resoursePosition) == 0) return 0;
	m_logoTextE.setPosition(m_position.x + 63,m_position.y + 1);
	m_logoTextE.setAlpha(0);
	if(m_logoTextRE.init("ResourcePack/pic/logo/RE.png",resoursePosition) == 0) return 0;
	m_logoTextRE.setPosition(m_position.x + 63,m_position.y + 1);
	m_logoTextRE.setAlpha(0);
	m_logoTextEC.setACopy(m_logoTextE);
	m_logoTextEC.setPosition(m_position.x + 121,m_position.y + 1);

	if(m_mode != 0)
	{
	//	m_back.setAngle(90);
		m_logoBack.setAngle(90);
		m_logoBack.setPosition(m_position.x + 91 + 21,m_position.y - 64 - 34 + 35);
		m_logoLight.setAngle(90);
		m_logoLight.setPosition(m_position.x + 91 + 21,m_position.y - 64 + 1);
		m_logoMiddle.setAngle(90);
		m_logoMiddle.setPosition(m_position.x,m_position.y + 35);
		m_logoTextX.setAngle(90);
		m_logoTextX.setPosition(m_position.x + 91 + 5,m_position.y - 64 - 34 + 35);
		m_logoTextE.setAngle(90);
		m_logoTextE.setPosition(m_position.x + 90 + 5,m_position.y + 1 - 34 + 35);
		m_logoTextRE.setAngle(90);
		m_logoTextRE.setPosition(m_position.x + 90 + 5,m_position.y + 1 - 34 + 35);
		m_logoTextEC.setAngle(90);
		m_logoTextEC.setPosition(m_position.x + 90 + 5,m_position.y + 59 - 34 + 35);
	}

	m_logoStage = -2;
	m_xAlpha = 0;
	m_xSize = 10;
	m_dxLight = 0;
	m_eAlpha = 0;
	m_logoTextX.setAlpha(m_eAlpha);
	m_logoTextX.setSize(_XVector2(m_xSize,m_xSize));
	for(int i = 0;i < 8;i++)
	{
		m_stageFlag[i] = 0;
	}

	m_isInited = 1;
	return 1;
}

void _XEngineLogo::reset()
{
	m_logoStage = -1;
	for(int i = 0;i < 8;i++)
	{
		m_stageFlag[i] = 0;
	}
}

void _XEngineLogo::move(int timeDelay)
{
	if(m_isInited == 0) return;
	if(m_logoStage == -1)
	{//初始化所有数据
		m_xAlpha = 0.0f;
		m_xSize = 10.0f;
		m_dxLight = 0.0f;
		m_eAlpha = 0.0f;
		m_logoTextX.setAlpha(m_xAlpha);
		m_logoTextX.setSize(_XVector2(m_xSize,m_xSize));
		m_logoTextE.setAlpha(m_eAlpha);
		m_logoTextE.setSize(_XVector2(1.0f,1.0f));
		m_logoStage = 0;
		m_logoTextE.setAlpha(0.0f);
		m_logoTextRE.setAlpha(0.0f);
//		for(int i = 0;i < 30;++ i)
//		{
//			m_logoTextECopy[i].setAlpha(0);
//		}
		m_logoTextEC.setAlpha(0.0f);
		m_back.setAlpha(1.0f);
		m_logoBack.setAlpha(1.0f);
		m_logoLight.setAlpha(1.0f);
		m_logoMiddle.setAlpha(1.0f);
	}else
	if(m_logoStage == 0)
	{//X出现
		if(m_xAlpha < 1.0f)
		{
			m_xAlpha += 0.005f * timeDelay;
		}else
		{
			m_xAlpha = 1.0f;
		}
		if(m_xSize > 1.0f)
		{
			m_xSize *= powf(0.98f,timeDelay);
		}else
		{
			m_xSize = 1.0f;
		}
		if(m_xSize <= 1.0f && m_xAlpha >= 1.0f)
		{//进入第二个阶段
			m_logoStage = 1;
			m_dxLight = 20.0f;
		}
		m_logoTextX.setAlpha(m_xAlpha);
		m_logoTextX.setSize(_XVector2(m_xSize,m_xSize));
	}
	if(m_logoStage >= 1 && m_logoStage < 5)
	{//E出现
		if(m_eAlpha < 1.5f)
		{
			m_eAlpha += 0.0005f * timeDelay;
			if(m_eAlpha <= 1.0f)
			{
				m_logoTextE.setAlpha(m_eAlpha);
			}
			if(m_eAlpha >= 0.5f)
			{
				m_logoTextEC.setAlpha(m_eAlpha - 0.5f);
			}
		}
		if(m_logoStage == 1 && m_eAlpha > 1.0f)
		{
			m_eAlpha = 1.0f;
			m_logoStage = 2;
		}
		//扫光
		if(m_dxLight < 180.0f)
		{
			m_dxLight += 0.1f * timeDelay;
			if(m_mode != 0)
			{
				m_logoLight.setPosition(m_position.x + 91.0f + 21.0f,m_position.y - 64.0f + m_dxLight + 1.0f);
			}else
			{
				m_logoLight.setPosition(m_position.x + m_dxLight,m_position.y);
			}
		}
	}
	if(m_logoStage == 2)
	{//第一个E翻身
		m_xSize += PI * 0.001f * timeDelay;
		if(m_xSize >= PI_HALF)
		{
			m_xSize = PI_HALF;
			m_logoStage = 3;
			m_logoTextRE.setAlpha(1.0f);
			m_logoTextRE.setSize(_XVector2(0.0f,1.0f));
			m_logoTextRE.setColor(1.0f,1.0f,-1.0f,-1.0f);
		}
		if(m_mode != 0)
		{
			m_logoTextE.setSize(_XVector2(1.0f,cos(m_xSize)));
		}else
		{
			m_logoTextE.setSize(_XVector2(cos(m_xSize),1.0f));
		}
	}else
	if(m_logoStage == 3)
	{//第一个E翻身
		m_xSize -= PI * 0.001f * timeDelay;
		if(m_xSize <= 0.0f)
		{
			m_xSize = 0.0f;			
			m_logoStage = 4;
		}
		if(m_mode != 0)
		{
			m_logoTextRE.setSize(_XVector2(1.0f,cos(m_xSize)));
		}else
		{
			m_logoTextRE.setSize(_XVector2(cos(m_xSize),1.0f));
		}
			m_logoTextRE.setColor(1.0f - cos(m_xSize),1.0f - cos(m_xSize),-1.0f,-1.0f);
	}else
	if(m_logoStage == 4)
	{
		m_xSize += timeDelay;
		if(m_xSize >= 1000.0f)
		{
			m_xSize = 1000.0f;
			m_logoStage = 5;
			m_eAlpha = 1.0f;
		}
	}else
	if(m_logoStage == 5)
	{//整体淡出
		m_eAlpha -= 0.002f * timeDelay;
		if(m_eAlpha <= 0.0f)
		{
			m_eAlpha = 0.0f;
			m_logoStage = -2;
		}
		m_logoTextRE.setAlpha(m_eAlpha);
		m_logoTextEC.setAlpha(m_eAlpha);
		m_logoTextX.setAlpha(m_eAlpha);
//		m_logoTextECopy[29].setAlpha(m_eAlpha);
		m_back.setAlpha(m_eAlpha);
		m_logoBack.setAlpha(0.0f);
		m_logoLight.setAlpha(0.0f);
		m_logoMiddle.setAlpha(0.0f);
	}
}

void _XEngineLogo::draw()
{
	if(m_isInited == 0) return;	
	XEE::clearScreen();

	if(XEE::windowData.windowType == WINDOW_TYPE_3D) 
		XEE::begin2DDraw();
	m_back.draw();
	m_logoBack.draw();
	m_logoLight.draw();
	m_logoMiddle.draw();
	m_logoTextX.draw();
	m_logoTextE.draw();
	m_logoTextRE.draw();
	m_logoTextEC.draw();

	//	for(int i = 0;i < 30;i++)
//	{
//		m_logoTextECopy[i].draw();
//	}
	XEE::updateScreen();
	mySleep(1);
}

_XEngineLogo::_XEngineLogo()
:m_isInited(0)
,m_resoursePosition(RESOURCE_LOCAL_FOLDER)
{
}
_XEngineLogo::~_XEngineLogo()
{
	release();
}
int _XEngineLogo::release()
{
	if(m_isInited == 0) return 1;
	m_back.release();
	m_logoBack.release();
	m_logoLight.release();
	m_logoMiddle.release();
	m_logoTextX.release();
	m_logoTextE.release();
	m_logoTextRE.release();
	m_logoTextEC.release();
	m_isInited = 0;
	return 1;
}
