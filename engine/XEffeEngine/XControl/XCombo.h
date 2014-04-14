#ifndef _JIA_XCOMBO_
#define _JIA_XCOMBO_
#include "XButton.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//�����˵��������ͼ
class _XComboTexture
{
private:
	_XBool m_isInited;
	void releaseTex();
public:
	_XTextureData *comboInputNormal;			//�����˵���������ͨ״̬
	_XTextureData *comboInputDisable;			//��Ч״̬�µİ�ť��ͼ
	_XButtonTexture downButtonTex;
	_XButtonTexture downMenuUpButtonTex;
	_XButtonTexture downMenuButtonTex;
	_XButtonTexture downMenuDownButtonTex;

	_XRect m_mouseRect;

	_XComboTexture();
	~_XComboTexture();
	_XBool init(const char *inputNormal,const char *inputDisable,
		const char *downButtonNormal,const char *downButtonOn,const char *downButtonDown,const char *downButtonDisable,
		const char *downMenuUpNormal,const char *downMenuUpOn,const char *downMenuUpDown,const char *downMenuUpDisable,
		const char *downMenuNormal,const char *downMenuOn,const char *downMenuDown,const char *downMenuDisable,
		const char *downMenuDownNormal,const char *downMenuDownOn,const char *downMenuDownDown,const char *downMenuDownDisable,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};

#define COMBO_MAX_MENU_LENGTH (512)
#define COMBO_LEFT_DISTANSE (10)
#define COMBO_TOP_DISTANSE (2)
#define DEFAULT_COMBO_BT_SIZE (36)
#define DEFAULT_COMBO_UD_HEIGHT (24)
#define DEFAULT_COMBO_MN_HEIGHT (36)

class _XCombo:public _XControlBasic
{
private:
	_XBool m_isInited;

	const _XTextureData *m_comboInputNormal;			//�����˵���������ͨ״̬
	const _XTextureData *m_comboInputDisable;			//��Ч״̬�µİ�ť��ͼ

	int m_nowMenuSum;			//��ǰ���е�ѡ������(��δʵ�֣���δ��Ч)
	int m_menuSum;				//�����˵��е��ܲ˵�������
	int m_menuDrawSum;			//�����˵�����ʾ�Ĳ˵�������
	int m_menuStartDrawOrder;	//�����˵��п�ʼ��ʾ�ĵ�һ���˵���ı��
	int m_menuTextWidth;		//�����˵�����ʾ�����ֵ����ֿ��

	_XBool m_isDrawDownMenu;		//�Ƿ���ʾ�����˵�
	int m_nowChooseOrder;		//��ǰѡ��Ĳ˵���ı��

	_XSprite m_sprite;			//������ʾ��ͼ�ľ���
	_XFontUnicode m_caption;	//���ֵ�����
	_XVector2 m_textSize;		//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	_XFColor m_textColor;		//���ֵ���ɫ
		
	char *m_menuData;			//�����˵��ľ�������
	_XButton *m_buttom;			//�����˵��еİ�ť��3����ťΪ���ܰ�ť������Ϊ�˵���ť

	void updateString();				//�����ַ���������Ĳ˵�����

	void (*m_funDataChange)(void *,int ID);
	void *m_pClass;				//�ص������Ĳ���

	friend void funComboMenuState(void *pClass,int ID);
	friend void funComboMenuStart(void *pClass,int ID);
	friend void funComboMenuEnd(void *pClass,int ID);
	friend void funComboMenu(void *pClass,int ID);

	_XRect m_inputArea;			//��������Ӧ��Χ
	_XRect m_downButtonArea;	//������ť����Ӧ��Χ
	_XRect m_downMenuUpArea;		//�Ϸ�ҳ����Ӧ��Χ
	_XRect m_downMenuArea;		//ѡ�������Ӧ��Χ
	_XRect m_downMenuDownArea;	//�·�ҳ����Ӧ��Χ
	_XRect m_allArea;			//�����ռ����Ӧ��Χ

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//û����ͼ����ʽ
public:
	_XBool init(const _XComboTexture &tex,	//�ؼ�����ͼ
		const _XVector2& position,		//�ؼ���λ��
		const _XRect& inputArea,		//��������Ч��Χ
		const _XRect& downButtonArea,	//������ť����Ӧ��Χ
		const _XRect& downMenuUpArea,	//�Ϸ�ҳ����Ӧ��Χ
		const _XRect& downMenuArea,		//ѡ�������Ӧ��Χ
		const _XRect& downMenuDownArea,	//�·�ҳ����Ӧ��Χ
		int menuSum,					//�����˵��е�ѡ�������
		int drawMenuSum,				//�����˵�����ʾ�Ĳ˵��������
		const _XFontUnicode &font,		//��ʾ����ʹ�õ�����
		float fontSize);				//����Ĵ�С
	_XBool initEx(const _XComboTexture &tex,	//�ؼ�����ͼ
		const _XVector2& position,		//�ؼ���λ��
		int menuSum,					//�����˵��е�ѡ�������
		int drawMenuSum,				//�����˵�����ʾ�Ĳ˵��������
		const _XFontUnicode &font,		//��ʾ����ʹ�õ�����
		float fontSize);				//����Ĵ�С
	_XBool initPlus(const char * path,
		int menuSum,					//�����˵��е�ѡ�������
		int drawMenuSum,				//�����˵�����ʾ�Ĳ˵��������
		const _XFontUnicode &font,		//��ʾ����ʹ�õ�����
		float fontSize,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);				//����Ĵ�С
	//int initWithoutTex(const _XRect& inputArea,		//��������Ч��Χ
	//	const _XRect& downButtonArea,	//������ť����Ӧ��Χ
	//	const _XRect& downMenuUpArea,	//�Ϸ�ҳ����Ӧ��Χ
	//	const _XRect& downMenuArea,		//ѡ�������Ӧ��Χ
	//	const _XRect& downMenuDownArea,	//�·�ҳ����Ӧ��Χ
	//	int menuSum,					//�����˵��е�ѡ�������
	//	int drawMenuSum,				//�����˵�����ʾ�Ĳ˵��������
	//	const _XFontUnicode &font,		//��ʾ����ʹ�õ�����
	//	float fontSize);
	_XBool initWithoutTex(int inputLen,
		int menuSum,					//�����˵��е�ѡ�������
		int drawMenuSum,				//�����˵�����ʾ�Ĳ˵��������
		const _XFontUnicode &font,		//��ʾ����ʹ�õ�����
		float fontSize);
protected:			
	void draw();
	void drawUp();
	_XBool mouseProc(float x,float y,_XMouseState mouseState);	//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState){return 1;};	//do nothing
	void insertChar(const char *ch,int len){;}
	_XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float x,float y);
public:
	int getNowChooseOrder() {return m_nowChooseOrder;}	//��ȡ��ǰѡ����
	_XBool setNowChooseOrder(int index)	//���õ�ǰѡ����
	{
		if(index < 0 || index >= m_menuSum) return XFalse;
		if(m_nowChooseOrder == index) return XTrue;
		m_nowChooseOrder = index;
		updateString();
		return XTrue;
	}
	_XBool setMenuStr(const char *str,int order);			//����ĳ�������˵�������
	void setShowMenuSum(int sum);	//��̬�ı���ʾ��ѡ������(��δʵ��)

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y);			//���ÿؼ��ĳߴ�

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);		//���ÿռ��λ��

	void setTextColor(const _XFColor& color) 
	{
		if(!m_isInited) return;
		m_textColor = color;
		m_caption.setColor(m_textColor * m_color);
		for(int i = 0;i < m_menuDrawSum + 3;++ i)
		{
			m_buttom[i].setTextColor(m_textColor);
		}
	}	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a) 	//���ð�ť����ɫ
	{
		if(!m_isInited) return;
		m_color.setColor(r,g,b,a);
		m_sprite.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		for(int i = 0;i < m_menuDrawSum + 3;++ i)
		{
			m_buttom[i].setColor(m_color);
		}
	}	//���ð�ť����ɫ
	void setAlpha(float a) 
	{
		if(!m_isInited) return;
		m_color.setA(a);
		m_sprite.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		for(int i = 0;i < m_menuDrawSum + 3;++ i)
		{
			m_buttom[i].setColor(m_color);
		}
	}	//���ð�ť����ɫ

	//����ӵ���Լ�����Ϊ,������ĸ��ʹ��ͬ������Դ,���ĸ����Դ���ı�,��������ԴҲ����Ӧ�ı��ı�
	_XBool setACopy(const _XCombo &temp);	//����һ������
	void release();						//�ͷ���Դ
	_XCombo();
	~_XCombo();

	//��������������
	void setCallbackFun(void (* funDataChange)(void *,int),void *pClass = NULL);
	void disable();					//ʹ��Ч
	void enable();					//ʹ��
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	virtual void justForTest() {}
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XCombo(const _XCombo &temp);
	_XCombo& operator = (const _XCombo& temp);
};

inline void _XCombo::setCallbackFun(void (* funDataChange)(void *,int),void *pClass)
{
	if(funDataChange != NULL) m_funDataChange = funDataChange;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
inline void _XCombo::disable()					//ʹ��Ч
{
	m_isEnable = XFalse;
}
inline void _XCombo::enable()					//ʹ��
{
	m_isEnable = XTrue;
}
#endif