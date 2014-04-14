#ifndef _JIA_XOBJECTMANAGER_
#define _JIA_XOBJECTMANAGER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2012.11.9
//--------------------------------
#include "XOSDefine.h"
#include "XMouseAndKeyBoardDefine.h"
#include "XBasicWindow.h"
#include "XBasicFun.h"
#include "XFont.h"
#include "XControl/XControls.h"

#include "stdio.h"
#include <memory>
#include "string.h"

/**********************************
//�༭����Ĺ��ܼ�˵��
1����ctrl + h�����Ƿ���ʾ���Դ���
2����ctrl + u�����Ƿ�ʼ�༭����
3����ctrl + p�����ı����Դ��ڵ�λ��
4����Q����ȥ�����в���
5����E����NodeLine����ı༭ת��
6����S�����ߴ�����
7����R������ת
8����M�����ƶ�
9����D����NodeLine��ɾ����ǰ ѡ�еĵ�
10����A����NodeLine�������λ������һ����
11����C�����ı���ת����
12����T�������÷�ת״̬
13����left����right����up����down�����ƶ�����
14����N��:�Ƿ������ѡ���
15����B�����Ƿ���ʾ�����ID
16����Delete�������б���ɾ��ѡ������
17����Shift����Ƭѡ���������ͬʱ�ı��������״̬
***********************************/

using namespace std;

#define OBJ_MANAGER_WITH_ID (1)

//��������͵Ķ���
enum _XObjectOptionType
{
	OBJ_OPTION_SIZE_ON,			//����Ŵ�
	OBJ_OPTION_SIZE_DOWN,		//�����С
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
#define MAX_OBJECT_SUM (4096)	//������ע����������
//�����Ҫʹ�õ�����Ϣ
struct _XObjectInfo
{
	_XVector2 m_position;	//�����λ��
	_XVector2 m_size;		//����ĳߴ�
	float m_angle;			//����ĽǶ�
//	float m_alpha;			//�����͸����
	float length;			//����NodeLine��˵����Ҫ��ʾ���ĳ���
};
enum _XObjectMouseState	//��������״̬״̬
{
	OBJ_STATE_NULL,			//��״̬
	OBJ_STATE_MOUSE_DOWN,	//��갴��״̬
	OBJ_STATE_MOUSE_MOVE,	//����϶�״̬
	OBJ_STATE_MOUSE_ON,		//�������״̬
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
	_XObjectMouseState m_objectMouseState[MAX_OBJECT_SUM];		//��������״̬

	_XVector2 m_objectMousePoint[MAX_OBJECT_SUM];		//��갴����λ��
	_XObjectType m_objectType[MAX_OBJECT_SUM];			//������͵ı��
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
	_XBool m_objCanEdit[MAX_OBJECT_SUM];		//����Ƿ���Ա༭		
	_XBool m_objBeSelect[MAX_OBJECT_SUM];		//����Ƿ�ѡ��

	int m_objLineOrder[MAX_OBJECT_SUM];		//������б���еĶ�Ӧ��ϵ
	int m_lineObjOrder[MAX_OBJECT_SUM];		//�б����������Ķ�Ӧ��ϵ
#if OBJ_MANAGER_WITH_ID
	char m_showObjID;		//�Ƿ���ʾObjID
	_XFontUnicode m_fontID[MAX_OBJECT_SUM];		//��ʾ�����ID������ǳ��������ܣ���Ҫ����
	void setShowObjID()
	{
		if(!m_isShowUI) return;
		if(m_showObjID == 0) m_showObjID = 1;
		else m_showObjID = 0;
	}
#endif
	void release(){}
	_XFontUnicode m_font;		//��ʾ���ַ�
	_XBool m_isInited;	//�Ƿ��ʼ��
	_XBool checkNeedUpdate(int order);	//��������Ϣ�Ƿ���Ҫ����
	void updateObjInfo(int order);		//���������Ϣ
	void updateObjStateInfo(int order);	//���������״̬��Ϣ
	//++++++++++++++++++++++++++++++++++++++++++++++++++
	//������Ϊ�˱༭�����������
//	_XSliderTexture m_sliderTextureH;
//	_XSlider m_sliderH;
//	_XSliderTexture m_sliderTextureV;
//	_XSlider m_sliderV;
//	_XCheckTexture m_checkTex0;
//	_XCheckTexture m_checkTex1;
//	_XMultiListTexture m_mutiListTexture;
	_XMultiListBasic m_mutiList;	//�����б��
	void upDateMultiListData();		//���¶����б���е�����
	//--------------------------------------------------
	friend void funObjectSelectChange(void *,int);
	friend void funObjectStateChange(void *,int);
	_XBool m_isShowUI;		//�Ƿ���ʾ����Ԫ��
	_XBool m_isOption;		//�Ƿ���Բ���
	_XBool m_canSelect;		//�Ƿ���������ѡ���
	void chengeCanSelectState()
	{//�ı�����Ƿ���Ա���ѡ�ı�ǩ
		if(!m_isShowUI) return;
		if(!m_canSelect) m_canSelect = XTrue;
		else m_canSelect = XFalse;
	}
	char m_editWindowsPos;	//�༭���ڵ�λ��	1	0
	void setEditWidowsPos()					//	2	3
	{
		if(!m_isShowUI) return;
		++ m_editWindowsPos;
		if(m_editWindowsPos >= 4) m_editWindowsPos = 0;
		switch(m_editWindowsPos)
		{
		case 0:m_mutiList.setPosition(XEE::sceneX + XEE::sceneWidth - 289.0f,XEE::sceneY);break;
		case 1:m_mutiList.setPosition(XEE::sceneX,XEE::sceneY);break;
		case 2:m_mutiList.setPosition(XEE::sceneX,XEE::sceneY + XEE::sceneHeight - 257.0f);break;
		case 3:m_mutiList.setPosition(XEE::sceneX + XEE::sceneWidth - 289.0f,XEE::sceneY + XEE::sceneHeight - 257.0f);break;
		}
	}
	void setShow()	
	{
		m_isShowUI = XTrue;
		m_mutiList.setVisiable();
	}	//������ʾ
	void disShow() 
	{
		m_isShowUI = XFalse;
		m_mutiList.disVisiable();
	}	//���ò���ʾ
	void setOption()
	{
		m_isOption = XTrue;
		setShow();
	}
	void disOption()
	{
		m_isOption = XFalse;
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

	_XKeyState m_ctrlKeyState;	//ctrl�����İ���״̬
public:
	_XBool init(_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
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