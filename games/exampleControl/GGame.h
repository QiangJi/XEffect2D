#ifndef _JIA_GGAME_
#define _JIA_GGAME_
#include "XEffeEngine.h"
#include "XControl/XControlManager.h"
#include "XResourcePack.h"

class _GGame
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	_GGame(){}
	_GGame(const _GGame&);
	_GGame &operator= (const _GGame&);
	virtual ~_GGame(){}
public:
	static _GGame& GetInstance()
	{
		static _GGame m_instance;
		return m_instance;
	}
	//-------------------------------------------
public:
	bool init();
	void move(int stepTime);
	void draw();
	void input(const _XInputEvent &input);
	void release(){;}

	_XFontUnicode FontUnicode;
	_XButton Button;		//��ť
	_XButton ButtonCopy;
	_XButtonEx ButtonEx;	//���ⰴť
	_XButtonEx ButtonExCopy;
	_XCheckTexture CheckTexture;
	_XCheck Check;			//��ѡ��
	_XCheck CheckCopy;
	_XMouseRightButtonMenu MouseRightButtonMenu; //�������Ҽ��˵�
	_XEdit Edit;			//�����
	_XEdit EditCopy;
	_XSlider Slider;		//������
	_XSlider SliderCopy;
	_XRadios Radios;		//��ѡ��
	_XRadios RadiosCopy;	
	_XNumber Number;
	_XProgress Progress;	//������
	_XProgress ProgressCopy;
	_XMultiText MutiText;	//�����ı���ʾ
	_XMultiText MutiTextCopy;
	_XMultiList MutiList;	//�����б��
	_XCombo Combo;			//�����˵�
	_XCombo ComboCopy;
	_XSliderEx tmpSldEx;
	_XPasswordPad passwordPad;
};

#endif