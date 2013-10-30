#ifndef _JIA_XOBJECTMANAGER_
#define _JIA_XOBJECTMANAGER_

//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2012.11.9
//--------------------------------

#include "XOSDefine.h"
#include "XMouseAndKeyBoardDefine.h"
#include "XBasicSDL.h"
#include "XBasicFun.h"
#include "XFont.h"
#include "XControl/XControls.h"

#include "stdio.h"
#include <memory>
#include "string.h"

using namespace std;

//��������͵Ķ���
enum _XObjectType
{
	OBJ_NULL,			//��Ч�����
	OBJ_SPRITE,			//��������
	OBJ_FRAME,			//����֡�����
	OBJ_FRAMEEX,		//����֡�����
	OBJ_NUMBER,			//�����ַ��������
	OBJ_FONTUNICODE,	//��ʾ�ַ������
	OBJ_FONTX,			//��ʾ�ַ������
	OBJ_NODELINE,		//�ڵ����ߵ����
	OBJ_CONTROL,		//�ؼ������
};

enum _XObjectOptionType
{
	OBJ_OPTION_SIZE_ON,			//����Ŵ�
	OBJ_OPTION_SIZE_DOWN,			//�����С
	OBJ_OPTION_X_ON,			//�������
	OBJ_OPTION_X_DOWN,			//�������
	OBJ_OPTION_Y_ON,			//�������
	OBJ_OPTION_Y_DOWN,			//�������
	OBJ_OPTION_ROTATE_ON,		//��ת�Ƕ�����
	OBJ_OPTION_ROTATE_DOWN,		//��ת�Ƕȼ�С
	OBJ_OPTION_ROTATE_MODE_CHANGE,	//��תģʽ�仯
	OBJ_OPTION_TURNOVER_MODE_CHANGE,//��תģʽ�仯
};	

enum _XObjectOption
{
	OBJ_OPTION_NULL,			//�޶���
	OBJ_OPTION_POSITION,		//λ�ñ仯
	OBJ_OPTION_SIZE,			//�ߴ�仯
	OBJ_OPTION_ROTATE,			//��ת
	OBJ_OPTION_ROTATEMODE,		//�ı���תģʽ
	OBJ_OPTION_TURNOVERMODE,	//�ı䷭תģʽ

	OBJ_OPTION_EDIT,			//������ڱ༭״̬
};

enum _XObjectOptionState	//�����ǰ���õ�״̬
{
	OBJ_OPTION_STATE_NULL,					//û�������κ�״̬
	OBJ_OPTION_STATE_CAN_NOT_CHOOSE,		//����ѡ��״̬
	OBJ_OPTION_STATE_NO_MOUSE_PROC,			//������������
	OBJ_OPTION_STATE_NO_KEY_PROC,			//����������̲���
};

#define MAX_OBJECT_SUM (8192)	//������ע����������

//�����Ҫʹ�õ�����Ϣ
struct _XObjectInfo
{
	_XVector2 m_position;	//�����λ��
	_XVector2 m_size;		//����ĳߴ�
	float m_angle;			//����ĽǶ�
//	float m_alpha;			//�����͸����
	float length;			//����NodeLine��˵����Ҫ��ʾ���ĳ���
};

enum _XObjectState	//�����״̬
{
	OBJ_STATE_NULL,			//��״̬
	OBJ_STATE_BE_CHOOSE,	//ѡ��״̬
	OBJ_STATE_MOUSE_DOWN,	//��갴��״̬
	OBJ_STATE_MOUSE_MOVE,	//����϶�״̬
	OBJ_STATE_MOUSE_ON,		//�������״̬
//	OBJ_STATE_EDIT,			//�༭״̬
};

//�������һ������ϵͳ��������Ӧ����ֻ��һ��ʵ��
class _XObjectManager
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//���ֵ���ģʽ����Ʒ�ʽ��֮ǰ��Ҫ��
protected:
	_XObjectManager();
	_XObjectManager(const _XObjectManager&);
	_XObjectManager &operator= (const _XObjectManager&);
	virtual ~_XObjectManager(); 
public:
	static _XObjectManager& GetInstance();
	//-------------------------------------------
private:
	void *m_pObject[MAX_OBJECT_SUM];	//�����ָ��
	_XObjectState m_objectState[MAX_OBJECT_SUM];	//�����״̬��Ϣ 0:��״̬ 1����ѡ��״̬ 2����갴��״̬ 3���������״̬
	_XObjectState m_objectUpState[MAX_OBJECT_SUM];	//�������һ��״̬��Ϣ
	_XVector2 m_objectMousePoint[MAX_OBJECT_SUM];		//��갴����λ��
	_XObjectType m_objectType[MAX_OBJECT_SUM];	//������͵ı��
	int m_nowObjectSum;					//��ǰ��ע����������
	int m_nowMouseOnObjectSum;			//��ǰ�������on״̬�µ��������
	_XObjectOption m_objectKeyOption[MAX_OBJECT_SUM];	//�����ǰ�������¶�Ӧ�Ĺ��� 0����Ч��1���ƶ���2�����ţ�3����ת��4���ı���תģʽ��5�����÷�ת״̬
	//�������������Ҫ��һ���ӿ�������
	_XObjectOptionState m_objectSetState[MAX_OBJECT_SUM];	//�����ǰ�����õ�״̬��0��û������״̬��1������Ϊ����ѡ��״̬��2������Ϊ������������״̬��3������Ϊ�����ܼ��̲���״̬

	int m_objectEditParm[MAX_OBJECT_SUM];	//������б༭��ʱ��ĸ�������������NodeLine�������ѡ�еĵ�ı��
	_XVector2 m_nowMousePosition;			//��굱ǰ��λ��

	void objectKeyOption(int order,_XObjectOptionType optionType);	//����Ӧ��������в�����OptionType���Ƕ���

	//��Ҫһ�������֧��������ʾ�����Ϣ
	_XObjectInfo m_objInfo[MAX_OBJECT_SUM];	//�������Ϣ
	//��������������û�����б��Ĳ������Ӧ�������������
	char m_objCanEdit[MAX_OBJECT_SUM];		//����Ƿ���Ա༭
	int m_objLineOrder[MAX_OBJECT_SUM];		//������б���еĶ�Ӧ��ϵ
	int m_lineObjOrder[MAX_OBJECT_SUM];		//�б����������Ķ�Ӧ��ϵ

	//_XFontUnicode m_font[MAX_OBJECT_SUM];		//��ʾ���ַ�
	_XFontUnicode m_font;		//��ʾ���ַ�
	char m_isInited;	//�Ƿ��ʼ��
	bool checkNeedUpdate(int order);	//��������Ϣ�Ƿ���Ҫ����
	void updateObjInfo(int order);		//���������Ϣ
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ�˱༭�����������
	_XSliderTexture m_sliderTextureH;
	_XSlider m_sliderH;
	_XSliderTexture m_sliderTextureV;
	_XSlider m_sliderV;
	_XCheckTexture m_checkTex0;
	_XCheckTexture m_checkTex1;
	_XMultiListTexture m_mutiListTexture;
	_XMultiListBasic m_mutiList;	//�����б��
	//--------------------------------------------------
	friend void funObjectSelectChange(void *,int);
	friend void funObjectStateChange(void *,int);
	char m_isShowUI;		//�Ƿ���ʾ����Ԫ��
	char m_isOption;		//�Ƿ���Բ���
	void setShow() 
	{
		m_isShowUI = 1;
		m_mutiList.setVisiable();
	}	//������ʾ
	void disShow() 
	{
		m_isShowUI = 0;
		m_mutiList.disVisiable();
	}	//���ò���ʾ
	void setOption()
	{
		m_isOption = 1;
		setShow();
	}
	void disOption()
	{
		m_isOption = 0;
		disShow();
		//ȡ�������еĶ���
		for(int i = 0;i < m_nowObjectSum;++ i)
		{
			m_objectKeyOption[i] = OBJ_OPTION_NULL;
		}
	}
public:
	//������Ϊ��ʵ�ְ��������������������
	_XKeyState m_keyState[4];	//��Ӧ�����������ĸ�����
	int m_keyTimer[4];	//�ĸ������ļ�ʱ
	int m_keyNowTime[4];	//�ĸ������ĵ�ǰʱ����
	int m_keyMaxTime;	//���������Ӧʱ��
	int m_keyMinTime;	//�����������Ӧʱ��
public:
	int init(int resoursePosition = 0);
	int addAObject(void * object,_XObjectType type);			//ע��һ�����,����ע���ID��ʵ���������б�ţ�-1Ϊע��ʧ��
	void decreaseAObject(int objectID);							//ȥ��һ�������ע��
	void decreaseAObject(const void * object);					//ȥ��һ�������ע��
	int findObjectID(const void * object) const;		//��ȡָ�������ID -1��ʾ�����û��ע��
	_XObjectType getObjectType(int objectID) const;	//��ȡ���������
	void *getObject(int objectID) const;				//��ȡ�����ָ��

	void mouseProc(int x,int y,_XMouseState eventType);	//���������¼�����Ӧ
	void keyProc(int keyID,_XKeyState keyState);		//keyState:0���£�1����
	int getNowObjectSum() const;				//��ȡ��ǰע����������
	int getNowMouseOnObjectSum() const;			//��ȡ��ǰ�������On״̬�µ��������
	int getNowChooseObjectSum() const;			//��ȡ��ǰ��ѡ�е��������
	void getNowChooseObjectID(int *ID) const;	//��ȡ��ǰ��ѡ������������ID����

	void draw();			//��������һЩ��Ϣ
	void move(int delay);	//����
};

#include "XObjectManager.inl"

#endif