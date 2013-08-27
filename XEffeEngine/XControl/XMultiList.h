#ifndef _JIA_XMUTILIST_
#define _JIA_XMUTILIST_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//������һ�������б�ؼ�����
//ʵ�ֻ�����			x
//ʵ�����ı�����		x
//ʵ����ѡ				x
//ʵ�����ݵ��뵼��		x
//ʵ�ּ����У�������	x
//ʵ�ֲ����У�������	x
//ʵ��ɾ���У�ɾ����	x
//ʵ���ƶ��У��ƶ���	x
//ʵ��ˮƽ���������������ƶ�
//����϶��е��ƶ�		x

#include "XSlider.h"
#include "XControlBasic.h"
#include "../XFontUnicode.h"
#include "../XSprite.h"
#include "XMouseRightButtonMenu.h"

//�����Ƕ����б�ؼ�����ͼ
class _XMultiListTexture
{
private:
	char m_isInited;
public:
	_XTextureData *mutiListNormal;		//�����б����ͨ״̬
	_XTextureData *mutiListDisable;		//�����б����Ч״̬
	_XTextureData *mutiListSelect;		//�����б��б�ѡ���еı���
	_XTextureData *mutiListMove;		//�ƶ�����ʱ�Ķ����ǩ
	_XTextureData *mutiListTitle;		//�����б�ı��ⱳ��
	_XTextureData *mutiListTitleEnd;	//�����б�ı���ָ���

	_XMultiListTexture();
	~_XMultiListTexture();
	int init(const char *normal,const char *disable,const char *select,const char *move,
		const char *title,const char *titleEnd,int resoursePosition = 0);
	void release();
};

//���е�һ��Ԫ��
struct _XMultiListOneBox
{
	char isEnable;				//���Ԫ���Ƿ���Ч
	char isShow;				//���Ԫ���Ƿ���Ҫ��ʾ
	_XFontUnicode text;			//������ʾ������

	_XVector2 order;				//���Ԫ�����ڵ�λ��(Ҳ������һ�У���һ��)
	char *string;				//���Ԫ���е��ַ���
	int stringLength;			//�ַ����ĳ���
	_XMultiListOneBox *nextBox;	//��һ��Ԫ�ص�ָ��
	_XMultiListOneBox()
		:string(NULL)
		,nextBox(NULL)
	{
	}
};

//���е�һ��
struct _XMultiListOneRow
{
	char isEnable;					//��һ���Ƿ���Ч
	char isShow;					//��һ�����Ƿ���Ҫ��ʾ	0:����ʾ 1:������ʾ 2:����ʾ�ָ��
	_XFontUnicode text;				//������ʾ������

	int order;						//��һ�еı��
	int stringShowWidth;			//��һ�п�����ʾ���ַ��Ŀ��
	_XVector2 position;				//��һ�е���Կؼ����Ͻǵ�����λ��
	_XVector2 pixSize;				//����һ����λ�����سߴ�,������ش�С��û�о������ŵ�
	char *title;					//��һ�еı���
	_XMultiListOneRow *nextRow;		//��һ�е�ָ��
	_XSprite m_spriteTitleEnd;		//ÿ�н����ı�Ƿ�
	_XSprite m_spriteTitle;			//ÿ�еı���
	char needChip;					//�Ƿ���Ҫ�и0������Ҫ�и1��ǰ���и2�������и�
	float left;						//��ߵ��и�λ��
	float right;					//�ұߵ��и�λ��
	_XMultiListOneRow()
		:title(NULL)
		,nextRow(NULL)
	{
	}
};

#define MUTILIST_MIN_WIDTH 80		//�����б����С���
#define MUTILIST_MAX_ROW_SUM 256	//����ж�����
#define MUTILIST_TITLE_EXPAND_LENGTH 100	//���һ��������չ�Ŀ��

class _XMultiList:public _XControlBasic
{
private:
	char m_isInited;				//�Ƿ��ʼ��

	const _XTextureData *m_mutiListNormal;	//�����б����ͨ״̬
	const _XTextureData *m_mutiListDisable;	//�����б����Ч״̬
	const _XTextureData *m_mutiListSelect;	//�����б��б�ѡ���еı���
	const _XTextureData *m_mutiListMove;		//�ƶ�����ʱ�Ķ����ǩ
	const _XTextureData *m_mutiListTitle;		//�����б�ı��ⱳ��
	const _XTextureData *m_mutiListTitleEnd;	//�����б�ı���ָ���

	char m_needShowVSlider;			//�Ƿ���Ҫ��ʾ��ֱ������
	_XSlider m_verticalSlider;		//��ֱ������
	char m_needShowHSlider;			//�Ƿ���Ҫ��ʾˮƽ������
	_XSlider m_horizontalSlider;	//ˮƽ������
	
	int m_tableRowSum;				//����е�����
	_XMultiListOneRow *m_tableRow;	//�е�����
	int m_tableLineSum;				//����е�����
	_XMultiListOneBox *m_tableBox;	//����Ԫ�ص�����

	char m_haveSelect;				//�Ƿ���ѡ��
	int m_selectLineOrder;			//ѡ�������һ��
	int m_showStartLine;			//��ʾ���Ǵӵڼ��п�ʼ��
	int m_showStartRow;				//��ʾ�ӵڼ��п�ʼ��
	int m_canShowLineSum;			//�ؼ�������ʾ������

	char m_needShowMove;			//�Ƿ���Ҫ��ʾ�ƶ�������

	_XSprite m_spriteBackGround;	//�ؼ��Ĵ󱳾�
	_XSprite m_spriteSelect;		//�ؼ���ѡ��(��ʱֻ�ܵ���ѡ��)
	_XSprite m_spriteMove;			//����ƶ��Ķ�����

	friend void funMutiListValueChangeV(void *pClass,int objectID);
	friend void funMutiListValueChangeMoveV(void *pClass,int objectID);
	friend void funMutiListValueChangeH(void *pClass,int objectID);
	friend void funMutiListValueChangeMoveH(void *pClass,int objectID);

	//������Ϊ��ʵ���������ı�������������ʱ����
	char m_mouseLeftButtonDown;		//�������Ƿ���
	int m_startX;					//��꿪ʼ�϶������
	int m_changeRowOrder;			//�϶��еı��
	_XMultiListOneRow * m_changeRow;	//�϶��е�ָ��
//	int m_rowDx;					//�϶��е���ʾƫ��λ��

	//������Ϊ��ʵ������϶��ƶ��ж�����ı���
	char m_mouseMoveDown;	//����Ƿ��а����϶�
	int m_oldLineOrder;		//��갴�����ڵ��У����ڼ�����Ҫ����ƶ�

public:
	void setTileStr(const char *str,int order);		//����ĳ�����������
	void setBoxStr(const char *str,int line,int row);	//����ĳһ����Ԫ�������
	int setRowWidth(int temp,int order);		//��������һ�еĿ��
private:
	void updateShowChange();					//�����б��ı仯��Ϣ�����б���еı�����ʾ���
	int m_showPixWidth;							//��ʾ��������ؿ��
	int m_showPixHight;							//��ʾ��������ظ߶�
	void updateSliderState();					//���ݱ�����������»�������״̬
	void updateSelectLine();					//����ѡ���е�״̬
	void initANewRowData(_XMultiListOneRow * upRow,int i);	//��ʼ��һ���µı���
	void initANewBoxData(_XMultiListOneBox * nowBox,_XMultiListOneRow * nowRow,int i,int j);	//��ʼ��һ���б�Ԫ�ص�����

	_XFontUnicode m_caption;
	_XVector2 m_fontSize;
	_XFColor m_textColor;			//���ֵ���ɫ
	float m_nowTextWidth;			//��ǰ��������
	float m_nowTextHeight;			//��ǰ������߶�

public:
	int init(const _XVector2& position,		//�ռ����ڵ�λ��
		const _XRect& Area,					//�ؼ���ʵ����ʾ����
		const _XMultiListTexture &tex,		//�ؼ�����ͼ
		const _XFontUnicode &font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum,				//�ؼ��е�����
		//const _XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		const _XSlider &vSlider,	//��ֱ������
		const _XSlider &hSlider);	//ˮƽ������
protected:
	void draw();					//��溯��
	void drawUp(){};						//do nothing
	int mouseProc(float x,float y,_XMouseState mouseState);					//������궯������Ӧ����
	int keyboardProc(int keyOrder,_XKeyState keyState){return 1;};	//do nothing
	int canGetFocus(float x,float y);	//�����жϵ�ǰ����Ƿ���Ի�ý���
	int canLostFocus(float x,float y);
public:
	int exportData(const char *fileName = NULL);			//���ݵ���
	int importData(const char *fileName = NULL);			//���ݵ���
	int setRowSum(int rowSum);		//��������������������հף�����ɾ��	;��Ҫ������ƣ�����������;�Ĵ���������ս�����߼�����
	int setLineSum(int lineSum);	//���������������Ĳ�����հף�����ɾ��	;��Ҫ������ƣ�����������;�Ĵ���������ս�����߼�����
	int deleteLine(int order);		//ɾ��ĳһ��
	int deleteRow(int order);		//ɾ��ĳһ��
	int insertALine(int order);		//��order������һ��
	int insertARow(int order);		//��order������һ��
	int moveDownLine(int order);	//��order������
	int moveRightRow(int order);	//��order������

	void setPosition(const _XVector2& position);		//����λ�õ�ʱ���ܶ�̬�ĸı��еĿ��,���򽫻����bug
	void setPosition(float x,float y);
	_XVector2 getPosition() const {return m_position;}
	void setSize(const _XVector2& size);				//���óߴ�
	void setSize(float x,float y);			//���óߴ�
	_XVector2 getSize() const {return m_size;}
	void setTextColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_textColor = color;
		m_caption.setColor(m_textColor);
	}	//�����������ɫ
	_XFColor getTextColor() const {return m_textColor;}	//��ȡ�ؼ��������ɫ
	void setColor(const _XFColor& color) 
	{
		if(m_isInited == 0) return;
		m_color = color;
		m_spriteBackGround.setColor(m_color);
		m_spriteSelect.setColor(m_color);
		m_spriteMove.setColor(m_color);
		m_caption.setColor(m_textColor * m_color);
	}	//���ð�ť����ɫ
	_XFColor getColor() const {return m_color;}	//��ȡ�ؼ�����ɫ

	_XMultiList();
	~_XMultiList();
	void release();
private:
	void releaseTempMemory();	//�ͷŲ��������ڴ�ռ�
public:
	//���涨�������������
	int addALine();					//��ĩβ����һ��
	int addARow();					//��ĩβ����һ��
	int deleteSelectLine();			//ɾ��ѡȡ��һ��
	int moveUpLine(int order);		//��order������
	int moveLeftRow(int order);		//��order������
	//Ϊ��֧���������������ؼ��������ṩ���������ӿڵ�֧��
	int isInRect(float x,float y);		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private://Ϊ�˷�ֹ���������ɵĴ����������ظ�ֵ�������͸�ֵ���캯��
	_XMultiList(const _XMultiList &temp);
	_XMultiList& operator = (const _XMultiList& temp);
};

inline int _XMultiList::addALine()					//��ĩβ����һ��
{
	return setLineSum(m_tableLineSum + 1);
}

inline int _XMultiList::addARow()					//��ĩβ����һ��
{
	return setRowSum(m_tableRowSum + 1);
}

inline int _XMultiList::deleteSelectLine()			//ɾ��ѡȡ��һ��
{
	if(m_isInited == 0) return 0;
	if(m_haveSelect == 0) return 0;
	return deleteLine(m_selectLineOrder);
}

inline int _XMultiList::moveUpLine(int order)		//��order������
{
	return moveDownLine(order - 1);
}

inline int _XMultiList::moveLeftRow(int order)		//��order������
{
	return moveRightRow(order - 1);
}

inline void _XMultiList::setSize(const _XVector2& size)	//���óߴ�
{
	setSize(size.x,size.y);
}

inline void _XMultiList::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}

#endif