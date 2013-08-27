#ifndef _JIA_XPASSWARDPAD_
#define _JIA_XPASSWARDPAD_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------

#include "XButton.h"
//����һ�������������ֻ��������޸�������࣬Ŀǰ�����δʵ�֣�֮����ʱ����ʵ��

#define MAX_PASSWARD_LENGTH (14)	//�����������볤��
#define MAX_ENTER_TRY (3)			//�������������Դ���
#define MIN_PASSWARD_LENGTH (4)		//��С���������볤��

class _XPasswardPadTexture
{
private:
	char m_isInited;
public:
	_XButtonTexture m_bottomTex;		//��������ͼ

	_XTextureData *backGround;				//��������ı�����ͼ

	_XPasswardPadTexture();
	~_XPasswardPadTexture();
	int init(const char *normal,const char *down,const char *on,const char *disable,const char *BG,int resoursePosition = 0);
	void release();
};

//�����������Ҫ��һЩ����
struct _XPasswardPadData
{
	_XVector2 bottonOffsetPosition;	//��ť����ƫ������
	_XVector2 bottonEffectArea;		//��ť��Ӧ������
	_XVector2 bottonDistance;		//��ť֮��ľ���
	_XVector2 bottonTextPosition;	//��ť�����ֵ�λ��
	_XVector2 captionPosition;		//�����ֵ�λ��
};
//Ŀǰ����ؼ��в����ƣ����ܴӿؼ��Ļ�����������������Ҫ����
class _XPasswardPad
{
private:
	char m_isInited;	//�Ƿ��ʼ��

	int m_mode;			//0:���������ģʽ 1:�����޸ĵ�ģʽ
	_XVector2 m_position;
	_XPasswardPadData m_passwardPadData;	//�����������Ե�һЩ����

	int m_stage;		//��������Ľ׶��Ա����
	int m_couter;
	char m_oldPassward[MIN_PASSWARD_LENGTH + 1];	//�ϵ�����
	char m_newPassward[MAX_PASSWARD_LENGTH + 1];	//�µ�����
	char m_enterPassward[MAX_PASSWARD_LENGTH + 1];	//���������
	char m_enterPasswardLength;
	char m_showString[MAX_PASSWARD_LENGTH + 1];		//������ʾ���ַ���

	_XFontUnicode m_caption;			//��ʾ���ֵ�����
	_XSprite m_sprite;		//��ʾ������ͼ
	_XButton m_button[12];	//12��С���̵İ���

	const _XPasswardPadTexture *m_texture;	//�ؼ�����ͼ
	//_XButtonTexture m_buttonTexture;	//��ť�ؼ�����ͼ

	bool m_isVisiable;	//�Ƿ���ʾ

	friend void btnFunc00(void *pClass,int ID);
	friend void btnFunc01(void *pClass,int ID);
	friend void btnFunc02(void *pClass,int ID);
	friend void btnFunc03(void *pClass,int ID);
	friend void btnFunc04(void *pClass,int ID);
	friend void btnFunc05(void *pClass,int ID);
	friend void btnFunc06(void *pClass,int ID);
	friend void btnFunc07(void *pClass,int ID);
	friend void btnFunc08(void *pClass,int ID);
	friend void btnFunc09(void *pClass,int ID);
	friend void btnFunc10(void *pClass,int ID);
	friend void btnFunc11(void *pClass,int ID);

	void addChar(char temp);
	void btn11Proc();
	void btn10Proc();

public:
	int init(const _XVector2& position,_XPasswardPadTexture *tex,const _XFontUnicode& font,const _XPasswardPadData& data);
	void draw();
	int getIsEnd();		//��������Ľ�� ��������ģʽ��0����δ���� 1������������ȷ 2������������� 3���û������˳�
										//�����޸�ģʽ��0����δ���� 1�������޸ĳɹ� 2����������������� 3���û������˳�
//	void mouseProc(float x,float y,_XMouseState mouseState);
	void setPassward(const char *p);	//������Ҫ�������ȷ����
	void getPassward(char *p) const;	//��ȡ�µ�����
	void setStart(int mode);	//������������忪ʼ����
	void setDisShow();

	void setPosition(const _XVector2& position);

	_XPasswardPad();
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XPasswardPad(const _XPasswardPad &temp);
	_XPasswardPad& operator = (const _XPasswardPad& temp);
};

inline int _XPasswardPad::getIsEnd()
{
	return m_stage;
}

inline void _XPasswardPad::setDisShow()
{
	m_isVisiable = false;
}

inline void _XPasswardPad::getPassward(char *p) const	//��ȡ�µ�����
{
	if(p == NULL) return;
	if(strlen(m_newPassward) < MIN_PASSWARD_LENGTH) return;
	strcpy(p,m_newPassward);
}

inline void _XPasswardPad::setPassward(const char *p)	//������Ҫ�������ȷ����
{
	if(p == NULL) return;	//��������Ƿ�
	if(strlen(p) <= MIN_PASSWARD_LENGTH || strlen(p) > MAX_PASSWARD_LENGTH) return;	//���볤�Ȳ��Ϸ�
	strcpy(m_oldPassward,p);
}
#endif