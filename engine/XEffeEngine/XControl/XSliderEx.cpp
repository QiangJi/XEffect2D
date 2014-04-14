//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XEffeEngine.h"
#include"XSliderEx.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
#include "XResourceManager.h"

void sliderExBtnProc(void *pClass,int id)
{
	_XSliderEx *p = (_XSliderEx *)pClass;
	if(p->m_secondarySld.getVisiable()) p->m_secondarySld.disVisiable();
	else 
	{//������Ҫ���÷�Χ
		p->m_secondarySld.setVisiable();
		float sum = (p->m_mainSld.getMaxValue() - p->m_mainSld.getMinValue()) * 0.02;
		float v = p->m_mainSld.getNowValue();
		float min = v - sum * 0.5f;
		float max = v + sum * 0.5f;
		if(min < p->m_mainSld.getMinValue())
		{
			min = p->m_mainSld.getMinValue();
			max = min + sum;
		}else
		if(max > p->m_mainSld.getMaxValue())
		{
			max = p->m_mainSld.getMaxValue();
			min = max - sum;
		}
		p->m_secondarySld.setRange(max,min);
		p->m_secondarySld.setNowValue(v);
	}
}
void sliderExSldProc(void *pClass,int id)
{
	_XSliderEx *p = (_XSliderEx *)pClass;
	if(id == p->m_secondarySld.getControlID()) p->m_mainSld.setNowValue(p->m_secondarySld.getNowValue()); else
	if(id == p->m_mainSld.getControlID())
	{
		if(p->m_secondarySld.getVisiable()) p->m_secondarySld.disVisiable();
		if(p->m_funMouseDown != NULL) p->m_funMouseDown(p->m_pClass,p->getControlID());
	}
}
void sliderExSldOnProc(void *pClass,int id)
{
	_XSliderEx *p = (_XSliderEx *)pClass;
	if(p->m_funMouseOn != NULL) p->m_funMouseOn(p->m_pClass,p->getControlID());
}
void sliderExSldUpProc(void *pClass,int id)
{
	_XSliderEx *p = (_XSliderEx *)pClass;
	if(p->m_funMouseUp != NULL) p->m_funMouseUp(p->m_pClass,p->getControlID());
}
void sliderExSldMoveProc(void *pClass,int id)
{
	_XSliderEx *p = (_XSliderEx *)pClass;
	if(p->m_funMouseDown != NULL) p->m_funMouseDown(p->m_pClass,p->getControlID());
}
void sliderExSldChangeProc(void *pClass,int id)
{
	_XSliderEx *p = (_XSliderEx *)pClass;
	if(p->m_funValueChange != NULL) p->m_funValueChange(p->m_pClass,p->getControlID());
}
_XBool _XSliderEx::initWithoutTex(const _XRect& area,float max,float min,
		_XSliderType type,const _XVector2 &fontPosition)
{
	if(m_isInited) return XFalse;	//��ֹ�ظ���ʼ��

	if(!m_mainSld.initWithoutTex(area,max,min,type,fontPosition)) return false;
	if(m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//ˮƽģʽ
		m_secondarySld.initWithoutTex(_XRect(0.0f,0.0f,area.getWidth(),area.getHeight()),max,min,type);
		m_chooseBtn.initWithoutTex("+",XEE::systemFont,1.0f,_XRect(0.0f,0.0f,area.getHeight(),area.getHeight()),
			_XVector2(area.getHeight() * 0.5f,area.getHeight() * 0.5f));
	}else
	{//��ֱģʽ
		m_secondarySld.initWithoutTex(_XRect(0.0f,0.0f,area.getWidth(),area.getHeight()),max,min,type);
		m_chooseBtn.initWithoutTex("+",XEE::systemFont,1.0f,_XRect(0.0f,0.0f,area.getWidth(),area.getWidth()),
			_XVector2(area.getWidth() * 0.5f,area.getWidth() * 0.5f));
	}
	m_mainSld.setCallbackFun(NULL,NULL,sliderExSldOnProc,sliderExSldProc,sliderExSldUpProc,sliderExSldChangeProc,sliderExSldMoveProc,this);
	m_secondarySld.disVisiable();
	m_secondarySld.setCallbackFun(NULL,NULL,NULL,NULL,NULL,sliderExSldProc,sliderExSldProc,this);
	m_chooseBtn.disVisiable();
	m_chooseBtn.setCallbackFun(NULL,NULL,NULL,NULL,sliderExBtnProc,this);
	if(m_funInit != NULL) m_funInit(m_pClass,getControlID());

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_mainSld);
	_XObjectManager::GetInstance().decreaseAObject(&m_secondarySld);
	_XObjectManager::GetInstance().decreaseAObject(&m_chooseBtn);
#endif

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_SLIDER);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif

	m_isInited = XTrue;
	setPosition(0.0f,0.0f);
	return XTrue;
}
_XBool _XSliderEx::setACopy(const _XSliderEx &temp)			//����һ������
{//
	if(&temp == this) return XFalse;
	if(!temp.m_isInited) return XFalse;
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_SLIDER);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}

	m_isInited = temp.m_isInited;
	m_timer = temp.m_timer;
	m_funInit = temp.m_funInit;	//��ʼ��
	m_funRelease = temp.m_funRelease;	//��Դ�ͷ�
	m_funMouseOn = temp.m_funMouseOn;	//�������
	m_funMouseDown = temp.m_funMouseDown;//��갴��
	m_funMouseUp = temp.m_funMouseUp;	//��굯��
	m_funMouseMove = temp.m_funMouseMove;//����϶�����ʱ����
	m_funValueChange = temp.m_funValueChange;//����������ֵ�仯
	m_pClass = temp.m_pClass;				//�ص������Ĳ���
//	m_needDrawBtn = temp.m_needDrawBtn;
//	m_needDrawSld = temp.m_needDrawSld;
	if(!m_mainSld.setACopy(temp.m_mainSld)) return XFalse;		
	if(!m_secondarySld.setACopy(temp.m_secondarySld)) return XFalse;		
	if(!m_chooseBtn.setACopy(temp.m_chooseBtn)) return XFalse;		
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_mainSld);
	_XObjectManager::GetInstance().decreaseAObject(&m_secondarySld);
	_XObjectManager::GetInstance().decreaseAObject(&m_chooseBtn);
#endif
	return XTrue;
}
_XBool _XSliderEx::canGetFocus(float x,float y)
{
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
_XBool _XSliderEx::mouseProc(float x,float y,_XMouseState mouseState)	//������궯������Ӧ����
{//��δʵ��
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�

	m_mainSld.mouseProc(x,y,mouseState);
	m_secondarySld.mouseProc(x,y,mouseState);
	m_chooseBtn.mouseProc(x,y,mouseState);
	return XTrue;
}
void _XSliderEx::update(int stepTime)
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	//������״̬�жϣ�������Ӧ������update�����жϣ���������ԭ��������пؼ���û��update����������Ŀǰ��draw��������жϣ�֮����Ҫ�����������
	if(m_timer < 1000)
	{
		m_timer += XEE::frameTime;
		if(m_timer >= 1000)
		{
			m_secondarySld.disVisiable();
			m_chooseBtn.disVisiable();
		}
	}
	//if(m_mainSld.getVisiable() && m_mainSld.getActive() && m_mainSld.isInRect(x,y))
	if(m_mainSld.getVisiable() && m_mainSld.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
	{
		m_timer = 0;
		m_chooseBtn.setVisiable();
	}
	if(m_chooseBtn.getVisiable() && m_chooseBtn.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
	{
		m_timer = 0;
	}
	if(m_secondarySld.getVisiable() && m_secondarySld.isInRect(XEE::mousePosition.x,XEE::mousePosition.y))
	{
		m_timer = 0;
	}

}
void _XSliderEx::draw()
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	if(!m_isVisiable) return;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return;		//�����Ч��ֱ���˳�

	m_mainSld.draw();
	m_secondarySld.draw();
	m_chooseBtn.draw();
}//��滬����
void _XSliderEx::setSize(float x,float y)
{
	if(x <= 0 && y <= 0) return;
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_size.set(x,y);

	m_mainSld.setSize(x,y);
	m_secondarySld.setSize(x,y);
	m_chooseBtn.setSize(x,y);
	if(m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//ˮƽģʽ
		m_chooseBtn.setPosition(m_mainSld.getBox(1).x + 5.0f * m_size.x,m_mainSld.getBox(1).y);
		m_secondarySld.setPosition(m_mainSld.getBox(3).x,m_mainSld.getBox(3).y + 5.0f * m_size.y);
	}else
	{
		m_chooseBtn.setPosition(m_mainSld.getBox(3).x,m_mainSld.getBox(3).y + 5.0f * m_size.y);
		m_secondarySld.setPosition(m_mainSld.getBox(0).x - 5.0f * m_size.y - m_secondarySld.getMouseRectWidth(),m_mainSld.getBox(0).y);
	}
	m_nowMouseRect = m_mainSld.getMouseRect();
	updateChildSize();
}
void _XSliderEx::setPosition(float x,float y)
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_position.set(x,y);

	m_mainSld.setPosition(x,y);
	if(m_mainSld.m_typeVorH == SLIDER_TYPE_HORIZONTAL)
	{//ˮƽģʽ
		m_chooseBtn.setPosition(m_mainSld.getBox(1).x + 5.0f * m_size.x,m_mainSld.getBox(1).y);
		m_secondarySld.setPosition(m_mainSld.getBox(3).x,m_mainSld.getBox(3).y + 5.0f * m_size.y);
	}else
	{
		m_chooseBtn.setPosition(m_mainSld.getBox(3).x,m_mainSld.getBox(3).y + 5.0f * m_size.y);
		m_secondarySld.setPosition(m_mainSld.getBox(0).x - 5.0f * m_size.y - m_secondarySld.getMouseRectWidth(),m_mainSld.getBox(0).y);
	}
	m_nowMouseRect = m_mainSld.getMouseRect();
	updateChildPos();
}