#ifndef _JIA_XCONTROLBASIC_
#define _JIA_XCONTROLBASIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "../XBasicClass.h"
#include "../XMouseAndKeyBoardDefine.h"
#include "../XObjectBasic.h" 

//˵����������Ԫ��Ŀ����Ϊ���ÿؼ����������Է��ʿؼ��Ļ�ͼ��������Ϣ��������
//���ǿؼ���ʵ�屻����֮�󣬶���ĳ���Ա�����Լ�������Щ��������ֹ�ظ���ͼ����
//������Ϣ����������Ԫ�������������ṹ��Ƶ���϶Ⱥ͸��Ӷȣ�������Ҫ˼����
//�õĽ����ʽ
class _XControlManager;
//�ؼ���Ļ���
class _XControlBasic:_XObjectBasic
{
	friend _XControlManager;
protected:
	_XRect m_mouseRect;		//�ؼ��������Ӧ��Χ
	_XRect m_nowMouseRect;	//��ǰ������Ӧ��Χ�������Χ�ܵ�λ�úʹ�С�ı仯���仯
	_XVector2 m_size;		//�ؼ��Ĵ�С
	_XVector2 m_position;	//�ؼ���λ��
	_XFColor m_color;		//�ؼ�����ɫ

	char m_isEnable;		//�ؼ��Ƿ���Ч����Ч��������Կ���������ȴ��ʾΪ���ܲ�����ģʽ
	char m_isVisiable;		//�ؼ��Ƿ�ɼ����ɼ�������Ż���ʾ����
	char m_isActive;		//�ؼ��Ƿ��ڼ���״̬�������������ܽ��տ����ź�,ͬʱ���������屻����
	char m_isBeChoose;		//�ؼ��Ƿ�ѡ�У����ռ��̲�����������table��ѡ��(��ʱ��δʵ��)������ͬʱֻ��һ������ܳ�Ϊ����
	int m_objectID;			//�ؼ���ID
public:
	_XRect getMouseRect() const {return m_mouseRect;}
	float getMouseRectWidth() const {return m_mouseRect.getWidth();}
	float getMouseRectHeight() const {return m_mouseRect.getHeight();}
	int getControlID() const {return m_objectID;}	//��ȡ�ؼ���ID
	void setActive(){m_isActive = 1;}
	void disActive(){m_isActive = 0;}		//���ÿؼ������տ�����Ϣ
	char getActive() const {return m_isActive;}
	void setVisiable(){m_isVisiable = 1;}
	void disVisiable(){m_isVisiable = 0;}	//���ÿؼ����ɼ�
	char getVisiable() const {return m_isVisiable;}
	int setACopy(const _XControlBasic & temp);
protected:	//���Ϊ����ĺ������������ⲿ����
	virtual void draw() = 0;
	virtual void drawUp() = 0;	//���������һ������Ķ������ؼ����������棬�ײ��Ư���㣬Ư����Ҳ�����ϲ��У������Ҽ��˵��������˵���
	virtual int mouseProc(float x,float y,_XMouseState mouseState) = 0;	//�����Ӧ����,������������ķ���ֵ�����壬�����ռ���Ķ����в�ͳһ���з��ؿؼ��Ƿ���״̬����ģ��ȵ�
	virtual int keyboardProc(int keyOrder,_XKeyState keyState) = 0;		//������Ӧ����

	virtual int canGetFocus(float x,float y) = 0;	//�Ƿ���Ի�ý���
	virtual int canLostFocus(float x,float y) = 0;	//��ǰ�ؼ��Ƿ����ʧȥ���㣬���ڴ�������������
	virtual void setLostFocus() {m_isBeChoose = 0;}	//����ʧȥ����
public:
	virtual void setPosition(const _XVector2& position) = 0;
	virtual void setPosition(float x,float y) = 0;
	virtual _XVector2 getPosition() const = 0;

	virtual void setSize(const _XVector2& size) = 0;
	virtual void setSize(float x,float y) = 0;
	virtual _XVector2 getSize() const = 0;

	float getAngle() const{return 0;}				//��ȡ����ĽǶ�
	void setAngle(float angle) {}					//��������ĽǶ�

	_XControlBasic();
	virtual ~_XControlBasic() {};
private:	//Ϊ�˷�ֹ������õĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XControlBasic(const _XControlBasic &temp);
	_XControlBasic& operator = (const _XControlBasic& temp);
};

/*
//��Ҫʵ�ֵĿؼ���������һЩ
1����ť		Button			x
2�����������	Edit		x	(Ŀǰ�����ܽ���������������)
3��������	Progress		x
4��������	Slider			x
5�������б�� MutiList		x
6�������б�� MutiText		x
7����ѡ��ť	Radio			x
8����ѡ��ť Check			x
9�������˵�	Combo			x
10��Ⱥ���	Group
11��ҳ�棨��ǩ��ť�� Tab
12�����������	MutiEdit		(Ŀǰ�����ܽ���������������)
13���ؼ�������� ����ͳһ�������пռ�ֱ�ӵ�ID�Լ����ڻ����ϵ
*/

#endif