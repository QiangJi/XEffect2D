#ifndef _JIA_XEDIT_
#define _JIA_XEDIT_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//����һ�������������
//��Ҫ֧�ֵĹ��ܵ�
//1�������ı�����
//2���ı��м����
//3�����ѡ���ı�(�����������ѡ��״̬�ᷴɫ�������ʹ��CPUͼ�����������Ҫʹ��������ɫ��������ʵ��)
//4���ı��滻�����С����ơ�ɾ��
//5�����̿��Ƽ���λ�õĸı�
//6�������ʽ���ı�����

//�ؼ���״̬
//1����Ч״̬
//2��������ʾ״̬
//3������״̬
//4�����ѡ��״̬
//5������Ҽ��ĵ����˵����˵����ݣ����С����ơ�ճ������������Ҫʵ��һ���Ҽ��˵�����

//ע�⣺����ֻ֧��Ӣ������

//С��������	x
//���̲���		x
//������		x
//���Ƭѡ		x
//�Ҽ��˵�		x
//������ճ��	x
//����������ʾ��Χ�Ĵ���	x
//shift + ���������Ƭѡ	x
//�������Լ������Ҽ��˵�	x
//ctrl+c��ctrl+v�Ĳ���		x
//����ΧƬѡ				x
//shift + home				x
//shift + end				x
//����ģʽ					x
//���˫��ȫѡ

#include "XControlBasic.h"
#include "../XFontUnicode.h"
#include "../XSprite.h"
#include "XMouseRightButtonMenu.h"

class _XEditTexture
{
private:
	_XBool m_isInited;
	void releaseTex();
public:
	_XTextureData *editNormal;			//�������ͨ״̬
	_XTextureData *editDisable;			//�������Ч״̬
	_XTextureData *editSelect;			//�����Ƭѡ��ɫ
	_XTextureData *editInsert;			//����������
	_XTextureData *editUpon;			//�ϲ����ɫ

	_XRect m_mouseRect;			//������Ӧ��Χ

	_XEditTexture();
	~_XEditTexture();
	_XBool init(const char *normal,const char *disable,const char *select,const char *insert,
		const char *upon = NULL,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};

#define MAX_INPUT_STRING_LENGTH 256	//������������ַ����ĳ���
#define TEXT_EDGE_WIDTH (10)		//��������ı߽���

class _XDirectoryList;
class _XEdit:public _XControlBasic
{
	friend _XDirectoryList;
private:
	_XBool m_isInited;	//�Ƿ��ʼ��
	_XMouseRightButtonMenu *m_mouseRightButtonMenu;	//����Ҽ��˵�

	_XFontUnicode m_caption;			//��ʾ��ǰ������ַ���
//	_XFontUnicode m_selectText;			//��ʾ��ѡ�е��ַ���(Ŀǰ���û�б�ʹ�õ�����������)

	char *m_nowString;		//��ǰ������ַ���
	char *m_tempNowString;	//����ַ�����Ҫ���ڸ���ʹ��
//	char *m_insertString;	//������ַ���
//	char *m_selectString;	//ѡȡ���ַ���
	int m_nowInsertPoint;	//��ǰѡ�������λ��,�����ַ���������λ��
	int m_nowStringLength;	//��ǰ�����ַ����ĳ���
	_XBool m_haveSelect;		//�Ƿ����ַ�������ѡ��
	_XBool m_selectMouseDown;	//����Ƿ���׼��Ƭѡ
	int m_selectStart;		//ѡ�����ʼλ��
	int m_selectEnd;		//ѡ��Ľ���λ��

	const _XTextureData *m_editNormal;		//�������ͨ״̬
	const _XTextureData *m_editDisable;		//�������Ч״̬
	const _XTextureData *m_editSelect;		//�����Ƭѡ��ɫ
	const _XTextureData *m_editInsert;		//����������
	const _XTextureData *m_editUpon;

	_XSprite m_spriteBackGround;	//������ʾ�����ı�����ͼ
	_XSprite m_spriteSelect;		//������ʾѡ�����ݵı�����ɫ
	_XSprite m_spriteInsert;		//������ʾ�������
	_XVector2 m_textPosition;			//������ʾ��λ�ã�������ڿؼ���λ��������
	_XVector2 m_textSize;				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	_XFColor m_textColor;		//���ֵ���ɫ
	float m_nowTextWidth;		//��ǰ����Ŀ�ȣ������ֵ������ĳߴ�ı�ʱ��Ҫ����
	float m_nowTextHeight;		//��ǰ����ĸ߶�

	void (*m_funInputChenge)(void *,int);		//�������ݷ����ı��ʱ�����
	void (*m_funInputOver)(void *,int);		//ȷ���������֮���ʱ�����
	void *m_pClass;

	int m_timer;	//���ǲ��������˸ʱʹ�õ�ʱ���ǣ����ʱ��ʹ�ò�׼ȷ�ļ�ʱ��ʽ
	//���������ڰ����жϵ��м����
	char m_keyShiftState;		//shift������״̬	0����1����
	char m_keyCapsLockState;	//CapsLock������״̬	1��Ч��0��Ч
	char m_keyNumLockState;		//NumLock������״̬	1��Ч��0��Ч
	char m_keyCtrlState;		//Ctrl������״̬ 0����1����
	
	int m_canShowLength;		//������п�����ʾ���ַ����ĳ���
	int m_nowShowStart;			//��ǰ���������ʾ���ַ�������ʼλ��
	char *m_nowShowString;		//��ǰ��ʾ���ַ��� 
	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//û����ͼ����ʽ
	_XRect m_selectRect;	//ѡ��ķ�Χ
	int keyJudgement(int keyOrder);	//�����ǰ����жϴ���ĺ��������ڲ�����
public:
	void upDataShowStr();	//������ʾ���Ⱥ͹���λ�ø�����ʾ������
	void upDateInsertShowPosition();	//����ʵ��������¹����ʾ��λ��
	void changeInsertPoint(int sum);	//�ƶ����
	void upDataSelectShow();			//����Ƭѡ����ʾ��Ϣ
	void mouseRightMenuProc();			//�Ҽ��˵��Ĵ���
	void deleteSelectStr();			//ɾ��ѡ�е��ַ������ƶ����

public:
	_XBool init(const _XVector2& position,		//�ؼ���λ��
		const _XRect& Area,					//�ؼ��������Ӧ����
		const _XEditTexture &tex,			//�ؼ�����ͼ
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		const _XFontUnicode &font,			//�ؼ�������
		float strSize,	//�ؼ���������Ϣ
		_XMouseRightButtonMenu * mouseMenu = NULL);		//�ؼ����Ҽ��˵�
	_XBool initEx(const _XVector2& position,		//�ؼ���λ��
		const _XEditTexture &tex,			//�ؼ�����ͼ
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		const _XFontUnicode &font,			//�ؼ�������
		float strSize,	//�ؼ���������Ϣ
		_XMouseRightButtonMenu * mouseMenu = NULL);		//�ؼ����Ҽ��˵�
	_XBool initPlus(const char * path,			//�ؼ�����ͼ
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		const _XFontUnicode &font,			//�ؼ�������
		float strSize,	//�ؼ���������Ϣ
		_XMouseRightButtonMenu * mouseMenu = NULL,//�ؼ����Ҽ��˵�
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	
	_XBool initWithoutTex(const _XRect& area,
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		const _XFontUnicode &font,			//�ؼ�������
		float strSize,	//�ؼ���������Ϣ
		_XMouseRightButtonMenu * mouseMenu = NULL);
protected:
	void update(int stepTime);
	void draw();					//��ͼ����
	void drawUp();				//���С�˵�
	_XBool mouseProc(float x,float y,_XMouseState mouseState);					//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);							//�����Ƿ������������
	void insertChar(const char * ch,int len);
	_XBool canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	_XBool canLostFocus(float x,float y);
	virtual void setLostFocus() 
	{
		if(m_haveSelect) m_haveSelect = XFalse;
		m_isBeChoose = XFalse;
	}	
public:
	void release();					//��Դ�ͷź���
	void setString(const char *str);		//����������ַ���

	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y);		//���ÿؼ���λ��

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y);				//���ÿؼ������ű���

	void setTextColor(const _XFColor& color) 
	{
		m_textColor = color;
		m_caption.setColor(m_textColor * m_color);
	}	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	void setColor(float r,float g,float b,float a) 
	{
		m_color.setColor(r,g,b,a);
		m_spriteBackGround.setColor(m_color);
		m_spriteSelect.setColor(m_color);
		m_spriteInsert.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		updateChildColor();
	}	//���ð�ť����ɫ
	void setAlpha(float a) 
	{
		m_color.setA(a);
		m_spriteBackGround.setColor(m_color);
		m_spriteSelect.setColor(m_color);
		m_spriteInsert.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
		updateChildAlpha();
	}	//���ð�ť����ɫ
	
	_XBool setACopy(const _XEdit &temp);				//����Ϊһ���������ò�����Դ		

	_XEdit();
	~_XEdit();
	//��������������
	void disable();					//ʹ��Ч
	void enable();					//ʹ��
	char *getString() const;				//����������ַ���
	int getSelectLength() const;				//���ѡ���ĳ���
	int getSelectHead() const;				//���ѡȡ��ͷ
	int getSelectEnd() const;				//���ѡȡ��β
	void setCallbackFun(void (* funInputChenge)(void *,int),void (* funInputOver)(void *,int),void *pClass = NULL);		//���ûص�����
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	virtual void justForTest() {;}
private:	//Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ���캯���͸�ֵ������
	_XEdit(const _XEdit &temp);
	_XEdit& operator = (const _XEdit& temp);
};
inline void _XEdit::disable()					//ʹ��Ч
{
	m_isEnable = XFalse;
	if(m_mouseRightButtonMenu != NULL) m_mouseRightButtonMenu->disVisiable(); //ȡ���Ҽ��˵�����ʾ
}
inline void _XEdit::enable()					//ʹ��
{
	m_isEnable = XTrue;
}
inline  char *_XEdit::getString() const				//����������ַ���
{
	return m_nowString;
}
inline int _XEdit::getSelectLength() const				//���ѡ���ĳ���
{
	if(m_selectEnd < m_selectStart) return m_selectStart - m_selectEnd;
	else return m_selectEnd - m_selectStart;
}
inline int _XEdit::getSelectHead() const				//���ѡȡ��ͷ
{
	if(m_selectEnd < m_selectStart)return  m_selectEnd;
	else return m_selectStart;
}
inline int _XEdit::getSelectEnd() const				//���ѡȡ��β
{
	if(m_selectEnd < m_selectStart) return m_selectStart;
	else return m_selectEnd;
}
inline void _XEdit::setCallbackFun(void (* funInputChenge)(void *,int),void (* funInputOver)(void *,int),void *pClass)		//���ûص�����
{
	if(funInputChenge != NULL) m_funInputChenge = funInputChenge;
	if(funInputOver != NULL) m_funInputOver = funInputOver;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
#endif