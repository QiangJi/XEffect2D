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
	char m_isInited;
public:
	_XTextureData *editNormal;			//�������ͨ״̬
	_XTextureData *editDisable;			//�������Ч״̬
	_XTextureData *editSelect;			//�����Ƭѡ��ɫ
	_XTextureData *editInsert;			//����������
	_XTextureData *editUpon;			//�ϲ����ɫ

	_XEditTexture();
	~_XEditTexture();
	int init(const char *normal,const char *disable,const char *select,const char *insert,const char *upon = NULL,int resoursePosition = 0);
	void release();
};

#define MAX_INPUT_STRING_LENGTH 256	//������������ַ����ĳ���
#define TEXT_EDGE_WIDTH (10)		//��������ı߽���

class _XEdit:public _XControlBasic
{
private:
	char m_isInited;	//�Ƿ��ʼ��
	_XMouseRightButtonMenu *m_mouseRightButtonMenu;	//����Ҽ��˵�

	_XFontUnicode m_caption;			//��ʾ��ǰ������ַ���
//	_XFontUnicode m_selectText;			//��ʾ��ѡ�е��ַ���(Ŀǰ���û�б�ʹ�õ�����������)

	char *m_nowString;		//��ǰ������ַ���
	char *m_tempNowString;	//����ַ�����Ҫ���ڸ���ʹ��
//	char *m_insertString;	//������ַ���
//	char *m_selectString;	//ѡȡ���ַ���
	int m_nowInsertPoint;	//��ǰѡ�������λ��,�����ַ���������λ��
	int m_nowStringLength;	//��ǰ�����ַ����ĳ���
	char m_haveSelect;		//�Ƿ����ַ�������ѡ��
	char m_selectMouseDown;	//����Ƿ���׼��Ƭѡ
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

	char m_timer;	//���ǲ��������˸ʱʹ�õ�ʱ���ǣ����ʱ��ʹ�ò�׼ȷ�ļ�ʱ��ʽ
	//���������ڰ����жϵ��м����
	char m_keyShiftState;		//shift������״̬	0����1����
	char m_keyCapsLockState;	//CapsLock������״̬	1��Ч��0��Ч
	char m_keyNumLockState;		//NumLock������״̬	1��Ч��0��Ч
	char m_keyCtrlState;		//Ctrl������״̬ 0����1����
	
	int m_canShowLength;		//������п�����ʾ���ַ����ĳ���
	int m_nowShowStart;			//��ǰ���������ʾ���ַ�������ʼλ��
	char *m_nowShowString;		//��ǰ��ʾ���ַ��� 

public:
	int keyJudgement(int keyOrder);	//�����ǰ����жϴ���ĺ��������ڲ�����
	void upDataShowStr();	//������ʾ���Ⱥ͹���λ�ø�����ʾ������
	void upDateInsertShowPosition();	//����ʵ��������¹����ʾ��λ��
	void changeInsertPoint(int sum);	//�ƶ����
	void upDataSelectShow();			//����Ƭѡ����ʾ��Ϣ
	void mouseRightMenuProc();			//�Ҽ��˵��Ĵ���
	void deleteSelectStr();			//ɾ��ѡ�е��ַ������ƶ����

public:
	int init(const _XVector2& position,		//�ؼ���λ��
		const _XRect& Area,					//�ؼ��������Ӧ����
		const _XEditTexture &tex,			//�ؼ�����ͼ
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		const _XFontUnicode &font,			//�ؼ�������
		float strSize,	//�ؼ���������Ϣ
		_XMouseRightButtonMenu * mouseMenu = NULL);		//�ؼ����Ҽ��˵�
protected:
	void draw();					//��ͼ����
	void drawUp();				//���С�˵�
	int mouseProc(float x,float y,_XMouseState mouseState);					//������궯������Ӧ����
	int keyboardProc(int keyOrder,_XKeyState keyState);							//�����Ƿ������������
	int canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	int canLostFocus(float x,float y);
public:
	void release();					//��Դ�ͷź���
	void setString(const char *str);		//����������ַ���

	void setPosition(const _XVector2& position);		//���ÿؼ���λ��
	void setPosition(float x,float y);		//���ÿؼ���λ��
	_XVector2 getPosition() const {return m_position;}
	void setSize(const _XVector2& size);				//���ÿؼ������ű���
	void setSize(float x,float y);				//���ÿؼ������ű���
	_XVector2 getSize() const {return m_size;}
	void setTextColor(const _XFColor& color) 
	{
		m_textColor = color;
		m_caption.setColor(m_textColor * m_color);
	}	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ
	void setColor(const _XFColor& color) 
	{
		m_color = color;
		m_spriteBackGround.setColor(m_color);
		m_spriteSelect.setColor(m_color);
		m_spriteInsert.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
	}	//���ð�ť����ɫ
	_XFColor getColor() const {return m_color;}	//��ȡ�ؼ�����ɫ
	
	int setACopy(const _XEdit &temp);				//����Ϊһ���������ò�����Դ		

	_XEdit();
	~_XEdit();
	//��������������
	void disable();					//ʹ��Ч
	void enable();					//ʹ��
	char *getString() const;				//����������ַ���
	int getSelectLength() const;				//���ѡ���ĳ���
	int getSelectHead() const;				//���ѡȡ��ͷ
	int getSelectEnd() const;				//���ѡȡ��β
	void setCallbackFun(void (* funInputChenge)(void *,int),void (* funInputOver)(void *,int),void *pClass);		//���ûص�����
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	int isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private:	//Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ���캯���͸�ֵ������
	_XEdit(const _XEdit &temp);
	_XEdit& operator = (const _XEdit& temp);
};

inline void _XEdit::disable()					//ʹ��Ч
{
	m_isEnable = 0;
	if(m_mouseRightButtonMenu != NULL) m_mouseRightButtonMenu->disVisiable(); //ȡ���Ҽ��˵�����ʾ
}

inline void _XEdit::enable()					//ʹ��
{
	m_isEnable = 1;
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
	m_pClass = pClass;
}

#endif