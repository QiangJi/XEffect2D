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
	SLIDER_TYPE_VERTICAL,		//��ֱ��
	SLIDER_TYPE_HORIZONTAL	//ˮƽ��
};

enum _XSliderState
{
	SLIDER_STATE_NORMAL,		//��ͨ״̬
	SLIDER_STATE_DOWN,			//��갴��
	SLIDER_STATE_ON,			//�������
	SLIDER_STATE_DISABLE		//��Ч״̬
};

class _XSliderTexture
{
private:
	_XBool m_isInited;
	void releaseTex();
public:
	_XTextureData *sliderLineNormal;			//�������ߵ���ͨ״̬
	_XTextureData *sliderLineDown;				//�������ߵİ���״̬
	_XTextureData *sliderLineOn;				//�������ߵ�����״̬
	_XTextureData *sliderLineDisable;			//�������ߵ���Ч״̬
	_XTextureData *sliderButtonNormal;			//��������ť����ͨ״̬
	_XTextureData *sliderButtonDown;			//��������ť�İ���״̬
	_XTextureData *sliderButtonOn;				//��������ť������״̬
	_XTextureData *sliderButtonDisable;			//��������ť����Ч״̬

	_XRect m_mouseRect;			//������Ӧ��Χ
	_XRect m_mouseRectButton;			//������Ӧ��Χ
	_XVector2 m_fontPosition;	//�������ֵ�λ��

	_XSliderTexture();
	~_XSliderTexture();
	_XBool init(const char *LNormal,const char *LDown,const char *LOn,const char *LDisable,
		const char *BNormal,const char *BDown,const char *BOn,const char *BDisable,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};
class _XMultiList;
class _XMultiListBasic;
class _XMultiText;
class _XDirectoryList;
class _XSliderEx;
class _XSlider:public _XControlBasic
{
	friend _XMultiList;
	friend _XMultiListBasic;
	friend _XMultiText;
	friend _XDirectoryList;
	friend _XSliderEx;
private:
	_XBool m_isInited;
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

	_XBool m_needFont;						//�Ƿ���Ҫ����
	_XFontUnicode m_caption;				//����
	char m_fontString[MAX_FILE_NAME_LENGTH];//��ʽ���ַ���
	_XVector2 m_textPosition;				//���ֵ����λ��
	_XVector2 m_textSize;					//���ֵĳߴ�
	_XFColor m_textColor;					//���ֵ���ɫ
	//�����ǹ��ڹ��������Ľӿ�
	float *m_pVariable;	//�����ı���
public:
	void setConnectVar(float * p) {m_pVariable = p;}	//��������
	void disConnectVar() {m_pVariable = NULL;}			//ȡ����������
private:
	_XVector2 m_upMousePoint;		//�ϴ�����ڹ�������λ��
	_XVector2 m_mouseFlyArea;		//�����ҷ�Ƿɳ�����Ӧ��Χ
	_XVector2 m_nowButtonPosition;//��ǰ���鰴ť��λ��
	float m_minValue;			//�����������ֵ
	float m_maxValue;			//����������Сֵ
	float m_nowValue;			//�������ĵ�ǰֵ
	float m_upValue;			//����������һ��ֵ(�������������Ŀ����������϶����鳬����Χ֮����Իָ����϶�֮ǰ��֪������Ŀǰ���ڼ򵥵Ŀ��ǣ������������)
	_XBool m_dataChanged;			//����������ֵ�Ƿ��б��޸�
	float m_keyOneValue;		//����һ�ΰ����Ļ�����(ע�����ֵ����Ҫ>=0,���򽫻���ɴ���)
	void updateButtonData();	//����ʵʱ���ݵı仯���»������Ϣ

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//û����ͼ����ʽ

	void setTexture(const _XSliderTexture &tex);
public:
	_XBool init(const _XVector2& position,	//�ؼ����ڵ�λ��
		const _XRect& Area,				//�ؼ��������Ӧ��Χ
		const _XRect& buttonArea,		//������������Ӧ��Χ(�������ʵ����û��������)
		const _XSliderTexture &tex,float max = 100.0f,float min = 0.0f,_XSliderType type = SLIDER_TYPE_HORIZONTAL);
	_XBool initEx(const _XVector2& position,	//�ؼ����ڵ�λ��
		const _XSliderTexture &tex,float max = 100.0f,float min = 0.0f,_XSliderType type = SLIDER_TYPE_HORIZONTAL);
	_XBool initPlus(const char * path,float max = 100.0f,float min = 0.0f,_XSliderType type = SLIDER_TYPE_HORIZONTAL,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	//int initWithoutTex(const _XRect& area,const _XRect& buttonArea,_XSliderType type,float max,float min,const _XVector2 &fontPosition);
	_XBool initWithoutTex(const _XRect& area,float max = 100.0f,float min = 0.0f,
		_XSliderType type = SLIDER_TYPE_HORIZONTAL,const _XVector2 &fontPosition = _XVector2(0.0f,0.0f));

	_XBool setFont(const char *caption,const _XFontUnicode &font,float captionSize,const _XVector2 &fontPosition);
	_XBool setFontEx(const char *caption,const _XFontUnicode &font,float captionSize);

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
//	using _XControlBasic::getPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);		//���û�������λ��

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
//	using _XControlBasic::getSize;	//���⸲�ǵ�����
	void setSize(float x,float y);				//���û����������ű���

	using _XObjectBasic::setColor;	//���⸲�ǵ�����
//	using _XControlBasic::getColor;	//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a)
	{
		if(!m_isInited) return;
		m_color.setColor(r,g,b,a);
		m_buttonSprite.setColor(m_color);
		m_lineSprite.setColor(m_color);
		updateChildColor();
	}
	void setAlpha(float a)
	{
		if(!m_isInited) return;
		m_color.setA(a);
		m_buttonSprite.setColor(m_color);
		m_lineSprite.setColor(m_color);
		updateChildAlpha();
	}
protected:
	void draw();							//��滬����
	void drawUp(){};						//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);
	void insertChar(const char *ch,int len){;}
	_XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float x,float y){return XTrue;}
	void setLostFocus() 
	{
		if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
		if(!m_isActive) return;		//û�м���Ŀؼ������տ���
		if(!m_isVisiable) return;	//������ɼ�ֱ���˳�
		if(!m_isEnable) return;		//�����Ч��ֱ���˳�

		if(m_nowSliderState != SLIDER_STATE_DISABLE) m_nowSliderState = SLIDER_STATE_NORMAL;
	}	//����ʧȥ����
public:
	void setCallbackFun(void (* funInit)(void *,int),
		void (* funRelease)(void *,int),
		void (* funMouseOn)(void *,int),
		void (* funMouseDown)(void *,int),
		void (* funMouseUp)(void *,int),
		void (*funValueChange)(void *,int),
		void (*funMouseMove)(void *,int),
		void *pClass = NULL);
	void setNowValue(float temp);			//���û��鵱ǰ��ֵ
	void setRange(float max,float min);

	_XBool setACopy(const _XSlider &temp);			//����һ������

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
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	virtual void justForTest() {;}
private:	//Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XSlider(const _XSlider &temp);
	_XSlider& operator = (const _XSlider& temp);
};
inline void _XSlider::disable()//ʹ�ؼ���Ч
{
	m_nowSliderState = SLIDER_STATE_DISABLE;
}
inline void _XSlider::enable()//ʹ�ؼ���Ч
{
	if(m_nowSliderState == SLIDER_STATE_DISABLE)
	{
		m_nowSliderState = SLIDER_STATE_NORMAL;
		//����Ҫ������꺯��
		mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
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
#endif