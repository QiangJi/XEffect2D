#ifndef _JIA_XSLIDER_
#define _JIA_XSLIDER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XControlBasic.h"
#include "../XSprite.h"
#include "XButton.h"

enum _XSliderType
{
	_XSLIDER_TYPE_VERTICAL,		//��ֱ��
	_XSLIDER_TYPE_HORIZONTAL	//ˮƽ��
};

enum _XSliderState
{
	_XSLIDER_STATE_NORMAL,		//��ͨ״̬
	_XSLIDER_STATE_DOWN,		//��ͨ״̬
	_XSLIDER_STATE_ON,			//��ͨ״̬
	_XSLIDER_STATE_DISABLE		//��ͨ״̬
};

class _XSliderTexture
{
private:
	char m_isInited;
public:
	_XTextureData *sliderLineNormal;			//�������ߵ���ͨ״̬
	_XTextureData *sliderLineDown;				//�������ߵİ���״̬
	_XTextureData *sliderLineOn;				//�������ߵ�����״̬
	_XTextureData *sliderLineDisable;			//�������ߵ���Ч״̬
	_XTextureData *sliderButtonNormal;			//��������ť����ͨ״̬
	_XTextureData *sliderButtonDown;			//��������ť�İ���״̬
	_XTextureData *sliderButtonOn;				//��������ť������״̬
	_XTextureData *sliderButtonDisable;			//��������ť����Ч״̬

	_XSliderTexture();
	~_XSliderTexture();
	int init(const char *LNormal,const char *LDown,const char *LOn,const char *LDisable,
		const char *BNormal,const char *BDown,const char *BOn,const char *BDisable,int resoursePosition = 0);
	void release();
};
class _XMultiList;
class _XMultiListBasic;
class _XMultiText;
class _XSlider:public _XControlBasic
{
	friend _XMultiList;
	friend _XMultiListBasic;
	friend _XMultiText;
private:
	char m_isInited;
	_XSliderState m_nowSliderState;			//��������״̬

	_XSliderType m_typeVorH;						//������������

	const _XTextureData *m_sliderLineNormal;			//�������ߵ���ͨ״̬
	const _XTextureData *m_sliderLineDown;			//�������ߵİ���״̬
	const _XTextureData *m_sliderLineOn;				//�������ߵ�����״̬
	const _XTextureData *m_sliderLineDisable;			//�������ߵ���Ч״̬
	const _XTextureData *m_sliderButtonNormal;		//��������ť����ͨ״̬
	const _XTextureData *m_sliderButtonDown;			//��������ť�İ���״̬
	const _XTextureData *m_sliderButtonOn;			//��������ť������״̬
	const _XTextureData *m_sliderButtonDisable;		//��������ť����Ч״̬

	void (*m_funInit)(void *,int);		//��ʼ��
	void (*m_funRelease)(void *,int);		//��Դ�ͷ�
	void (*m_funMouseOn)(void *,int);		//�������
	void (*m_funMouseDown)(void *,int);	//��갴��
	void (*m_funMouseUp)(void *,int);		//��굯��
	void (*m_funMouseMove)(void *,int);	//����϶�����ʱ����
	void (*m_funValueChange)(void *,int);	//����������ֵ�仯
	void *m_pClass;				//�ص������Ĳ���

	_XRect m_buttonArea;		//�������а�ť����Ч��Χ,�����Χ�ǰ�ť�����ͼƬ����Ӧ��Χ����Ҫע��
	//_XRect m_nowButtonArea;		//��ǰ������Ӧ��Χ
	_XSprite m_buttonSprite;	//������ʾ��ͼ�ľ���
	_XSprite m_lineSprite;		//������ʾ��ͼ�ľ���

	_XVector2 m_upMousePoint;		//�ϴ�����ڹ�������λ��
	_XVector2 m_mouseFlyArea;		//�����ҷ�Ƿɳ�����Ӧ��Χ
	_XVector2 m_nowButtonPosition;//��ǰ���鰴ť��λ��
	float m_minValue;			//�����������ֵ
	float m_maxValue;			//����������Сֵ
	float m_nowValue;			//�������ĵ�ǰֵ
	float m_upValue;			//����������һ��ֵ(�������������Ŀ����������϶����鳬����Χ֮����Իָ����϶�֮ǰ��֪������Ŀǰ���ڼ򵥵Ŀ��ǣ������������)
	char m_dataChanged;			//����������ֵ�Ƿ��б��޸�
	float m_keyOneValue;		//����һ�ΰ����Ļ�����(ע�����ֵ����Ҫ>=0,���򽫻���ɴ���)
	void updateButtonData();	//����ʵʱ���ݵı仯���»������Ϣ
public:
	int init(const _XVector2& position,	//�ؼ����ڵ�λ��
		const _XRect& Area,				//�ؼ��������Ӧ��Χ
		const _XRect& buttonArea,		//������������Ӧ��Χ(�������ʵ����û��������)
		const _XSliderTexture &tex,_XSliderType type,float max,float min);
	void setPosition(const _XVector2& position);		//���û�������λ��
	void setPosition(float x,float y);		//���û�������λ��
	_XVector2 getPosition() const
	{
		return m_position;
	}
	void setSize(const _XVector2& size);				//���û����������ű���
	void setSize(float x,float y);				//���û����������ű���
	_XVector2 getSize() const
	{
		return m_size;
	}
	void setColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_color = color;
		m_buttonSprite.setColor(m_color);
		m_lineSprite.setColor(m_color);
	}	//�����������ɫ
	_XFColor getColor() const {return m_color;}	//��ȡ�ؼ��������ɫ
protected:
	void draw();							//��滬����
	void drawUp(){};						//do nothing
	int mouseProc(float x,float y,_XMouseState mouseState);	//������궯������Ӧ����
	int keyboardProc(int keyOrder,_XKeyState keyState);
	int canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	int canLostFocus(float x,float y);
	void setLostFocus() 
	{
		if(m_isInited == 0) return;	//���û�г�ʼ��ֱ���˳�
		if(m_isActive == 0) return;		//û�м���Ŀؼ������տ���
		if(m_isVisiable == 0) return;	//������ɼ�ֱ���˳�
		if(m_isEnable == 0) return;		//�����Ч��ֱ���˳�

		if(m_nowSliderState != _XSLIDER_STATE_DISABLE) m_nowSliderState = _XSLIDER_STATE_NORMAL;
	}	//����ʧȥ����
public:
	void setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funMouseOn)(void *,int),
		void (* funMouseDown)(void *,int),
		void (* funMouseUp)(void *,int),
		void (*funValueChange)(void *,int),
		void (*funMouseMove)(void *,int),
		void *pClass);
	void setNowValue(float temp);			//���û��鵱ǰ��ֵ
	void setRange(float max,float min);

	int setACopy(const _XSlider &temp);			//����һ������

	_XSlider();
	~_XSlider();
	void release();
	//��������
	void disable();		//ʹ�ؼ���Ч
	void enable();		//ʹ�ؼ���Ч
	float getNowValue() const;	//��ȡ���鵱ǰ��ֵ
	float getMaxValue() const; 
	float getMinValue() const;
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	int isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private:	//Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XSlider(const _XSlider &temp);
	_XSlider& operator = (const _XSlider& temp);
};

inline void _XSlider::disable()//ʹ�ؼ���Ч
{
	m_nowSliderState = _XSLIDER_STATE_DISABLE;
}

inline void _XSlider::enable()//ʹ�ؼ���Ч
{
	if(m_nowSliderState == _XSLIDER_STATE_DISABLE)
	{
		m_nowSliderState = _XSLIDER_STATE_NORMAL;
		//����Ҫ������꺯��
		mouseProc(m_upMousePoint.x,m_upMousePoint.y,_XMOUSE_MOVE);
	}
}

inline float _XSlider::getNowValue() const	//��ȡ���鵱ǰ��ֵ
{
	return m_nowValue;
}

inline float _XSlider::getMaxValue() const
{
	return m_maxValue;
}

inline float _XSlider::getMinValue() const
{
	return m_minValue;
}

inline void _XSlider::setSize(const _XVector2& size)
{
	setSize(size.x,size.y);
}

inline void _XSlider::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}
#endif