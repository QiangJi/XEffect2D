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
	char m_isInited;
public:
	_XTextureData *comboInputNormal;			//�����˵���������ͨ״̬
	_XTextureData *comboInputDisable;			//��Ч״̬�µİ�ť��ͼ
	_XButtonTexture downButtonTex;
	_XButtonTexture downMenuUpButtonTex;
	_XButtonTexture downMenuButtonTex;
	_XButtonTexture downMenuDownButtonTex;

	_XComboTexture();
	~_XComboTexture();
	int init(const char *inputNormal,const char *inputDisable,
		const char *downButtonNormal,const char *downButtonOn,const char *downButtonDown,const char *downButtonDisable,
		const char *downMenuUpNormal,const char *downMenuUpOn,const char *downMenuUpDown,const char *downMenuUpDisable,
		const char *downMenuNormal,const char *downMenuOn,const char *downMenuDown,const char *downMenuDisable,
		const char *downMenuDownNormal,const char *downMenuDownOn,const char *downMenuDownDown,const char *downMenuDownDisable,
		int resoursePosition = 0);
	void release();
};

#define COMBO_MAX_MENU_LENGTH 512
#define COMBO_LEFT_DISTANSE 10
#define COMBO_TOP_DISTANSE 5

class _XCombo:public _XControlBasic
{
private:
	char m_isInited;

	const _XTextureData *m_comboInputNormal;			//�����˵���������ͨ״̬
	const _XTextureData *m_comboInputDisable;			//��Ч״̬�µİ�ť��ͼ

	int m_nowMenuSum;			//��ǰ���е�ѡ������(��δʵ�֣���δ��Ч)
	int m_menuSum;				//�����˵��е��ܲ˵�������
	int m_menuDrawSum;			//�����˵�����ʾ�Ĳ˵�������
	int m_menuStartDrawOrder;	//�����˵��п�ʼ��ʾ�ĵ�һ���˵���ı��
	int m_menuTextWidth;		//�����˵�����ʾ�����ֵ����ֿ��

	char m_isDrawDownMenu;		//�Ƿ���ʾ�����˵�
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
public:
	int init(const _XComboTexture &tex,	//�ؼ�����ͼ
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
protected:			
	void draw();
	void drawUp();
	int mouseProc(float x,float y,_XMouseState mouseState);	//������궯������Ӧ����
	int keyboardProc(int keyOrder,_XKeyState keyState){return 1;};	//do nothing
	int canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	int canLostFocus(float x,float y);
public:
	int setMenuStr(const char *str,int order);			//����ĳ�������˵�������
	void setShowMenuSum(int sum);	//��̬�ı���ʾ��ѡ������(��δʵ��)

	void setSize(const _XVector2& size);			//���ÿؼ��ĳߴ�
	void setSize(float x,float y);			//���ÿؼ��ĳߴ�
	_XVector2 getSize() const {return m_size;}
	void setPosition(const _XVector2& position);	//���ÿռ��λ��
	void setPosition(float x,float y);		//���ÿռ��λ��
	_XVector2 getPosition() const {return m_position;}
	void setTextColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_textColor = color;
		m_caption.setColor(m_textColor * m_color);
		for(int i = 0;i < m_menuDrawSum + 3;++ i)
		{
			m_buttom[i].setTextColor(m_textColor);
		}
	}	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ
	void setColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_color = color;
		m_sprite.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		for(int i = 0;i < m_menuDrawSum + 3;++ i)
		{
			m_buttom[i].setColor(m_color);
		}
	}	//���ð�ť����ɫ
	_XFColor getColor() const {return m_color;}	//��ȡ�ؼ�����ɫ
	//����ӵ���Լ�����Ϊ,������ĸ��ʹ��ͬ������Դ,���ĸ����Դ���ı�,��������ԴҲ����Ӧ�ı��ı�
	int setACopy(const _XCombo &temp);	//����һ������
	void release();						//�ͷ���Դ
	_XCombo();
	~_XCombo();

	//��������������
	void setCallbackFun(void (* funDataChange)(void *,int),void *pClass);
	void disable();					//ʹ��Ч
	void enable();					//ʹ��
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	int isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XCombo(const _XCombo &temp);
	_XCombo& operator = (const _XCombo& temp);
};

inline void _XCombo::setCallbackFun(void (* funDataChange)(void *,int),void *pClass)
{
	if(funDataChange != NULL) m_funDataChange = funDataChange;
	m_pClass = pClass;
}

inline void _XCombo::disable()					//ʹ��Ч
{
	m_isEnable = 0;
}

inline void _XCombo::enable()					//ʹ��
{
	m_isEnable = 1;
}

inline void _XCombo::setSize(const _XVector2& size)			//���ÿؼ��ĳߴ�
{
	setSize(size.x,size.y);
}

inline void _XCombo::setPosition(const _XVector2& position)	//���ÿռ��λ��
{
	setPosition(position.x,position.y);
}
#endif