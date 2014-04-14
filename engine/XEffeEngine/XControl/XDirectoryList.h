#ifndef _JIA_XDIRECTORYLIST_
#define _JIA_XDIRECTORYLIST_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.5
//--------------------------------
#include "XSlider.h"
#include "XEdit.h"
#include "XCheck.h"
#include "XControlBasic.h"
#include "../XFontUnicode.h"
#include "XButton.h"
#include "XMedia/XDirectory.h"
//�������ڽ����У���δ��������
//����һ����ʵĿ¼��Ϣ�Ŀؼ����࣬�������һ�������һ����ť��2���϶�����1��չʾ�����
class _XDirListTexture
{
private:
	_XBool m_isInited;
public:
	_XTextureData *dirListNormal;			//��ͨ״̬
	_XTextureData *dirListDisable;			//��Ч״̬

	_XRect m_mouseRect;			//������Ӧ��Χ

	_XDirListTexture();
	~_XDirListTexture();
	_XBool init(const char *normal,const char *disable,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initEx(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	void release();
};
//Ŀ¼�б���һ����Ϣ
class _XDirListOneLine
{
public:
	_XFontUnicode m_font;
	char *m_string;		//�ַ�����
	_XCheck *m_check;
	_XBool m_needCheck;
	_XBool m_isEnable;		//�Ƿ���Ч

	//_XVector2 m_pos;		//��ǰ�����ڵ�λ��
	_XFile *m_file;			//ָ���ļ���Ϣ��ָ��
	_XDirListOneLine()
		:m_isEnable(XFalse)
		,m_needCheck(XFalse)
		,m_check(NULL)
		,m_string(NULL)
		,m_file(NULL)
	{
	}
	~_XDirListOneLine()
	{
		release();
	}
	void release()
	{
		if(!m_isEnable) return;
		XDELETE_ARRAY(m_string);
		XDELETE(m_check);
		m_isEnable = XFalse;
	}
};
#ifndef DEFAULT_SLIDER_WIDTH
#define DEFAULT_SLIDER_WIDTH (32)		//Ĭ�ϵĻ��������
#endif	//DEFAULT_SLIDER_WIDTH
#ifndef DEFAULT_DIRLIST_BT_SIZE
#define DEFAULT_DIRLIST_BT_SIZE (40)
#endif	//DEFAULT_DIRLIST_BT_SIZE
#ifndef DEFAULT_DIRLIST_CK_SIZE
#define DEFAULT_DIRLIST_CK_SIZE (28)
#endif	//DEFAULT_DIRLIST_CK_SIZE
class _XDirectoryList:public _XControlBasic
{
private:
	_XBool m_isInited;	//�Ƿ��ʼ��

	_XDirectory m_dir;	//���ڽ���·��

	_XSprite m_spriteBackGround;
	const _XTextureData *m_dirListNormal;
	const _XTextureData *m_dirListDisable;

	_XBool m_needShowVSlider;			//�Ƿ���Ҫ��ʾ��ֱ������
	_XSlider m_verticalSlider;		//��ֱ������
	_XBool m_needShowHSlider;			//�Ƿ���Ҫ��ʾˮƽ������
	_XSlider m_horizontalSlider;	//ˮƽ������
	_XButton m_button;	//��ť
	_XEdit m_edit;
	_XCheck m_check;

	_XBool m_haveSelect;				//�Ƿ���ѡ��
	int m_selectLineOrder;			//ѡ�������һ��
	int m_showStartLine;			//��ʾ���Ǵӵڼ��п�ʼ��
	int m_canShowLineSum;			//�ؼ�������ʾ������
	
	int m_nowLineSum;				//��ǰһ��ӵ�ж�����
	int m_mouseTime;				//��������ʱ���ж��Ƿ�˫��

	int m_maxLineWidth;				//��ǰ������п�[����]
	int m_nowLineLeft;				//��ǰ�ļ��[����]
	int m_canShowMaxLineWidth;		//������ʾ������п�[����]

	std::vector<_XDirListOneLine *> m_lineData;	//ÿһ�е�����
	int m_showPixWidth;				//��ʾ��������ؿ��
	int m_showPixHight;				//��ʾ��������ظ߶�

	_XFontUnicode m_caption;
	_XVector2 m_fontSize;
	_XFColor m_textColor;			//���ֵ���ɫ
	float m_nowTextWidth;			//��ǰ��������
	float m_nowTextHeight;			//��ǰ������߶�
	void furlFolder(int index,_XBool flag = XTrue);		//��£ĳ���ļ���,�ڿؼ��ص���flag = XTrue;
	void updateHSliderState();	//����ˮƽ����״̬
	void updateShowPosition();	//������ʾ��״̬

	//�������ڲ��ؼ��Ļص�����
	friend void funDirListButton(void *pClass,int objectID);
	friend void funDirListValueChangeV(void *pClass,int objectID);
	friend void funDirListValueChangeH(void *pClass,int objectID);
	friend void funDirListCheck(void *pClass,int objectID);

	_XResourceInfo *m_resInfo;
	_XBool m_withoutTex;	//û����ͼ����ʽ

	_XBool m_canChangePath;
public:
	_XBool getCanChangePath() const {return m_canChangePath;}
	void setCanChangePath(_XBool flag)
	{
		if((flag && m_canChangePath) || (!flag && !m_canChangePath)) return;
		m_canChangePath = flag;
		if(flag) m_edit.enable();
		else m_edit.disable();
	}
public:
	_XBool init(const _XVector2& position,
		const _XRect& Area,	
		_XDirListTexture & tex,
		_XFontUnicode &font,
		float fontSize,
		const _XCheck &check,
		const _XButton &button,
		const _XEdit &edit,
		const _XSlider &vSlider,	//��ֱ������
		const _XSlider &hSlider);
	_XBool initEx(const _XVector2& position,
		_XDirListTexture & tex,
		_XFontUnicode &font,
		float fontSize,
		const _XCheck &check,
		const _XButton &button,
		const _XEdit &edit,
		const _XSlider &vSlider,	//��ֱ������
		const _XSlider &hSlider);
	_XBool initPlus(const char * path,
		_XFontUnicode &font,
		float fontSize,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);
	_XBool initWithoutTex(const _XRect& area,	
		_XFontUnicode &font,
		float fontSize);	//��δʵ��
	const char * getSelectFileName() const	//��ȡȫ·��
	{
		if(m_haveSelect) return m_lineData[m_selectLineOrder]->m_file->allPath;
		else return NULL;
	}
	int getSelectLineOrder() const
	{
		if(m_haveSelect) return m_selectLineOrder;
		else return -1;
	}
protected:
	void draw();
	void drawUp(){;}	//do nothing
	_XBool mouseProc(float x,float y,_XMouseState mouseState);					//������궯������Ӧ����
	_XBool keyboardProc(int keyOrder,_XKeyState keyState);
	void insertChar(const char * ch,int len) {;}
	_XBool canGetFocus(float x,float y)//�����жϵ�ǰ����Ƿ���Ի�ý���
	{
		if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
		if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
		if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
		if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
		return isInRect(x,y);
	}
	_XBool canLostFocus(float x,float y){return XTrue;}
public:
	using _XObjectBasic::setPosition;	//���⸲�ǵ�����
	void setPosition(float x,float y)
	{
		m_position.set(x,y);
		updateShowPosition();
		if(!m_withoutTex) m_spriteBackGround.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight()) * m_size);
		m_button.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),0.0f) * m_size);
		m_verticalSlider.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()) * m_size);
		m_horizontalSlider.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()) * m_size);
		m_edit.setPosition(m_position);
		updateChildPos();
	}

	using _XObjectBasic::setSize;		//���⸲�ǵ�����
	void setSize(float x,float y)
	{//���ϽǶ���
		m_size.set(x,y);
		updateShowPosition();
		if(!m_withoutTex) 
		{
			m_spriteBackGround.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight()) * m_size);
			m_spriteBackGround.setSize(m_size);
		}
		m_button.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),0.0f) * m_size);
		m_button.setSize(m_size);
		m_verticalSlider.setPosition(m_position + _XVector2(m_mouseRect.getWidth(),m_edit.getMouseRect().getHeight()) * m_size);
		m_verticalSlider.setSize(m_size);
		m_horizontalSlider.setPosition(m_position + _XVector2(0.0f,m_edit.getMouseRect().getHeight() + m_mouseRect.getHeight()) * m_size);
		m_horizontalSlider.setSize(m_size);
		m_edit.setPosition(m_position);
		m_edit.setSize(m_size);
		updateChildSize();
	}			//���óߴ�

	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	_XBool isInRect(float x,float y);						//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);						//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	using _XObjectBasic::setColor;		//���⸲�ǵ�����
	virtual void setColor(float r,float g,float b,float a){updateChildColor();}

	virtual void setAlpha(float a){updateChildAlpha();}
	virtual void justForTest() {;}

	void release();
	_XDirectoryList()
		:m_isInited(0)
		,m_resInfo(NULL)
		,m_withoutTex(XFalse)
		,m_canChangePath(XTrue)
	{
	}
	~_XDirectoryList()
	{
		release();
	}
};

#endif