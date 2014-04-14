#ifndef _JIA_XMUTITEXT_
#define _JIA_XMUTITEXT_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//����һ��������ʾ�����ı����࣬�����мƻ�����༭���ܣ�����̫���Ӿ�û���������ĳ�����

#include "XEdit.h"
#include "XSlider.h"
//sliderĿ¼��Ϊ:/SliderH��/SliderV
typedef _XEditTexture _XMultiTextTexture;

//���е��ַ���
struct _XMultiTextLineString
{
	_XBool isEnable;				//��һ���Ƿ�Ҫ��ʾ���Ƿ���Ч
	char *showString;			//��һ����������Ҫ��ʾ���ַ���
//	int showStart;				//�����ַ�����ʾ���ֵ�ͷ�ڵ�ǰ�е�ƫ��
//	int showLength;				//��ʾ�ַ����ĳ���
	_XFontUnicode nowText;		//������ʾ��һ���ַ���������
	int lineHeadorder;			//��һ���ַ���ȫ���е�ͷλ��
	int lineEndOrder;			//��һ���ַ���ȫ���е�βλ��
	//int lineLength;				//��һ���ַ����ĳ���
	//char isEnterEnd;			//�Ƿ���Ϊ���ж�������ʾ1����������Ϊ��ʾ�ַ����ĳ������ƶ�����0
	_XSprite spriteSelect;		//�ַ�����ʾѡ�񱳾���ɫ�ľ���
	_XBool haveAllSelect;			//�������Ƿ�汻����ѡ��

	int lineWidthPixel;			//�е����ؿ��

	_XMultiTextLineString()
		:showString(NULL)
	{
	}
};

#define MAX_LINE_STRING_LENGTH (256)	//���п�����ʾ���ַ�������󳤶�
#define MAX_LINE_SUM (256)				//����������������ַ���
#define MAX_STRING_LENGTH (65536)		//�������������ַ����ĳ���
#ifndef DEFAULT_SLIDER_WIDTH
#define DEFAULT_SLIDER_WIDTH (32)		//Ĭ�ϵĻ��������
#endif //DEFAULT_SLIDER_WIDTH

class _XMultiText:public _XControlBasic
{
private:
	_XBool m_isInited;				//�Ƿ��ʼ��

	_XBool m_needShowVSlider;			//�Ƿ���Ҫ��ʾ��ֱ������
	_XSlider m_verticalSlider;		//��ֱ������
	_XBool m_needShowHSlider;			//�Ƿ���Ҫ��ʾˮƽ������
	_XSlider m_horizontalSlider;	//ˮƽ������

	char *m_nowString;				//��ǰ������ַ���������

	const _XTextureData *m_mutiEditNormal;	//�������ͨ״̬
	const _XTextureData *m_mutiEditDisable;	//�������Ч״̬
	const _XTextureData *m_mutiEditSelect;	//�����Ƭѡ��ɫ
	const _XTextureData *m_mutiEditUpon;	//�����Ƭѡ��ɫ

	_XSprite m_spriteBackGround;	//������ʾ�����ı�����ͼ
	//_XVector2 m_textPosition;			//������ʾ��λ�ã�������ڿؼ���λ��������
	_XVector2 m_textSize;				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	_XFColor m_textColor;			//���ֵ���ɫ
	float m_textWidth;	//����Ŀ��
	float m_textHeight;	//����ĸ߶�

	_XBool m_haveSelect;				//�Ƿ����ַ�������ѡ��
	int m_nowSelectLineOrder;		//��ǰ�ĵ��б�ѡ�������һ��

	_XMultiTextLineString *m_lineStr;
	int m_nowStartLine;			//��ǰ��ʾ�ĵ�һ�����ܵĵڼ���
	int m_nowShowLineSum;		//��ǰ������ʾ������
//	int m_nowShowLineWidth;		//��ǰһ�п�����ʾ�Ŀ��
	int m_nowMaxLineSum;		//��ǰ�ַ�����ʵ�ʵ�����
//	int m_nowMaxLineLength;		//��ǰ�ַ�����ʵ�ʵ��п�
	void updateLineStr();		//���µ�ǰ������ı���ÿһ����

	//������Ϊ��ʵ�������ƶ���������±���
	int m_nowStartPixels;			//��ʼ������λ��
	int m_nowMaxLineWidthPixels;	//��ǰ�������ؿ��
	int m_MaxLineWidthPixels;		//�ܵ�������ؿ��

//	void (*m_funInputChenge)(void *,int);		//�������ݷ����ı��ʱ�����
//	void (*m_funInputOver)(void *,int);		//ȷ���������֮���ʱ�����
//	void *m_pClass;

	char m_timer;	//���ǲ��������˸ʱʹ�õ�ʱ����

	friend void funMutiTextValueChangeV(void *pClass,int objectID);
	friend void funMutiTextValueChangeMoveV(void *pClass,int objectID);
	friend void funMutiTextValueChangeH(void *pClass,int objectID);
	friend void funMutiTextValueChangeMoveH(void *pClass,int objectID);

	void setStartLine(int temp);	//���ô�����ʾ��ͷ�е��к�
	void checkStr();							//����ַ����ĵ��п���Ƿ��������,���������Ҫ�����ڽ��ַ�������Ĭ�ϵĿ�Ȳ��Ƴ���Ҫ����ʽ,�Ա����Ժ���ַ�������

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//û����ͼ����ʽ

	void (*m_funSelect)(void *,int ID);	//������ѡʱ����
	void *m_pClass;
public:
	_XBool init(const _XVector2& position,	//�ؼ����ڵ�λ��
		const _XRect& Area,	//�ؼ��ķ�Χ
		const _XMultiTextTexture& tex,	//�ؼ�����ͼ
		const char *str,const _XFontUnicode& font, float strSize,		//�ؼ������弰�����Ϣ
		//const _XVector2& strPosition,
		//const _XMouseRightButtonMenu &mouseMenu,
		const _XSlider &vSlider,const _XSlider &hSlider);	//�ؼ����������������ؼ�
	_XBool initEx(const _XVector2& position,	//�ؼ����ڵ�λ��
		const _XMultiTextTexture& tex,	//�ؼ�����ͼ
		const char *str,const _XFontUnicode& font, float strSize,		//�ؼ������弰�����Ϣ
		const _XSlider &vSlider,const _XSlider &hSlider);	//�ؼ����������������ؼ�
	_XBool initPlus(const char *path,
		const char *str,const _XFontUnicode& font, float strSize,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(const _XRect& area,
		const char *str,const _XFontUnicode& font, float strSize);

	void setSelectCallBackFun(void (* funSelect)(void *,int),void *pClass)
	{
		m_funSelect = funSelect;
		m_pClass = pClass;
	}
protected:
	void draw();
	void drawUp(){};						//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);					//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState){return XFalse;}			//�����Ƿ������������
	void insertChar(const char *ch,int len){;}
	_XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float x,float y){return XTrue;}
public:
	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);	//�ı�ռ��λ��

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y);		//�ı�ؼ��ĳߴ�

	void setTextColor(const _XFColor& color) 
	{
		if(!m_isInited) return;
		m_textColor = color;
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			m_lineStr[i].nowText.setColor(m_textColor);
		}
	}	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a) 
	{
		if(!m_isInited) return;
		m_color.setColor(r,g,b,a);
		m_spriteBackGround.setColor(m_color);
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			m_lineStr[i].nowText.setColor(m_textColor * m_color);
		}
		m_verticalSlider.setColor(m_color);
		m_horizontalSlider.setColor(m_color);
	}	//���ð�ť����ɫ
	void setAlpha(float a) 
	{
		if(!m_isInited) return;
		m_color.setA(a);
		m_spriteBackGround.setColor(m_color);
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			m_lineStr[i].nowText.setColor(m_textColor * m_color);
		}
		m_verticalSlider.setColor(m_color);
		m_horizontalSlider.setColor(m_color);
	}	//���ð�ť����ɫ

	void setString(const char *str);			//�ı���ʾ���ַ���
	void addString(const char *str);			//���ַ���������ַ���
	void addALine(const char *str);			//���ַ���������ַ���

	_XBool setACopy(const _XMultiText &temp);
	
	_XBool exportData(const char *fileName = NULL);		//���ݵ���
	_XBool importData(const char *fileName = NULL);		//���ݵ���
	_XBool deleteSelectLine();						//ɾ����ǰѡ���һ��
	_XBool moveUpSelectLine();						//��ѡ���һ������
	_XBool moveUpLine(int order);					//��ĳһ������
	_XBool moveDownSelectLine();					//��ѡ���һ������
	_XBool moveDownLine(int order);				//��ĳһ������
	_XBool insertALine(const char *str,int lineOrder);	//���ı���ָ���в���һ��

	_XMultiText();
	~_XMultiText();
	//��������������
	void enable();		//ʹ����Ч
	void disable();		//ʹ����Ч
	void release();
	int getSelectLine() const;			//���ص�ǰ���ѡ�������һ�У�-1λû��ѡ��
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	virtual void justForTest() {}
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XMultiText(const _XMultiText &temp);
	_XMultiText& operator = (const _XMultiText& temp);
};

inline void _XMultiText::enable()		//ʹ����Ч
{
	m_isEnable = XTrue;
	m_verticalSlider.enable();
	m_horizontalSlider.enable();
}
inline void _XMultiText::disable()		//ʹ����Ч
{
	m_isEnable = XFalse;
	m_verticalSlider.disable();
	m_horizontalSlider.disable();
}
inline int _XMultiText::getSelectLine() const			//���ص�ǰ���ѡ�������һ�У�-1λû��ѡ��
{
	if(!m_isInited) return -1;
	if(!m_haveSelect) return -1;
	else return m_nowSelectLineOrder;
}

#endif