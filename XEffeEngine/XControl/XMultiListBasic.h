#ifndef _JIA_XMUTILISTBASIC_
#define _JIA_XMUTILISTBASIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.26
//--------------------------------

//����һ�������Ķ����б��ֻʵ����һЩ�����Ĺ���

#include "XSlider.h"
#include "XControlBasic.h"
#include "../XFontUnicode.h"
#include "../XSprite.h"
#include "XMouseRightButtonMenu.h"
#include "XMultiList.h"
#include "XCheck.h"

class _XMultiListBasic:public _XControlBasic
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
	
	int m_tableRowSum;				//����е�����(������Ҫ����һ����Ч��������������ĸ���)
	_XMultiListOneRow *m_tableRow;	//�е�����
	int m_tableLineSum;				//����е�����(������Ҫ����һ����Ч��������������ĸ���)
	_XMultiListOneBox *m_tableBox;	//����Ԫ�ص�����

	char m_haveSelect;				//�Ƿ���ѡ��
	int m_selectLineOrder;			//ѡ�������һ��
	_XRect m_selectRect;			//ѡ����е��׿�
	bool *m_haveSelectFlag;			//���ڱ�����е������Ƿ�ѡ��

	int m_showStartLine;			//��ʾ���Ǵӵڼ��п�ʼ��
	float m_showStartRow;			//��ʾ��ͷһ�е����ؿ��
	int m_canShowLineSum;			//�ؼ�������ʾ������

	char m_needShowMove;			//�Ƿ���Ҫ��ʾ�ƶ�������

	_XSprite m_spriteBackGround;	//�ؼ��Ĵ󱳾�
	_XSprite m_spriteSelect;		//�ؼ���ѡ��(��ʱֻ�ܵ���ѡ��)
	_XSprite m_spriteMove;			//����ƶ��Ķ�����

	friend void funMutiListBasicValueChangeV(void *pClass,int objectID);
	friend void funMutiListBasicValueChangeMoveV(void *pClass,int objectID);
	friend void funMutiListBasicValueChangeH(void *pClass,int objectID);
	friend void funMutiListBasicValueChangeMoveH(void *pClass,int objectID);
	friend void funMutiListBasicStateChange(void *pClass,int objectID);

	//������Ϊ��ʵ���������ı�������������ʱ����
	char m_mouseLeftButtonDown;		//�������Ƿ���
	int m_startX;					//��꿪ʼ�϶������
	int m_changeRowOrder;			//�϶��еı��
	_XMultiListOneRow * m_changeRow;	//�϶��е�ָ��
//	int m_rowDx;					//�϶��е���ʾƫ��λ��

	//������Ϊ��ʵ������϶��ƶ��ж�����ı���
	char m_mouseMoveDown;	//����Ƿ��а����϶�
	int m_oldLineOrder;		//��갴�����ڵ��У����ڼ�����Ҫ����ƶ�

	void (*m_funCheckStateChange)(void *,int ID);	//״̬ѡ�����仯��ʱ����õĺ���
	void (*m_funSelectChange)(void *,int ID);		//ѡ�����仯ʱ����
	void *m_pClass;				//�ص������Ĳ���

public:
	int getSelectOrder() const	//����ѡ��������к�
	{
		return m_selectLineOrder;
	}
	void setCallbackFun(void (* funSelectChange)(void *,int),void (* funCheckStateChange)(void *,int),void *pClass);
	void clearAllSelect();	//������е�ѡ��

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

	_XCheck *m_check0;		//�Ƿ�ɼ���״̬ѡ���İ�ť
	_XCheck *m_check1;		//�Ƿ�ɱ༭��״̬ѡ���İ�ť
	bool *m_check0State;		//�Ƿ�ɼ���״̬
	bool *m_check1State;		//�Ƿ�ɱ༭��״̬
public:
	bool getCheckState(int order,int lineOrder)
	{
		if(m_isInited == 0) return false;
	//	if(lineOrder < 0 || lineOrder >= min(m_tableLineSum,m_canShowLineSum)) return false;
	//	if(order == 0) return m_check0[lineOrder].getState();
	//	else return m_check1[lineOrder].getState();
		if(lineOrder < 0 || lineOrder >= m_tableLineSum) return false;
		if(order == 0) return m_check0State[lineOrder];
		else return m_check1State[lineOrder];
	}
	void setCheckState(int order,int lineOrder,bool state)
	{
		if(m_isInited == 0) return;
	//	if(lineOrder < 0 || lineOrder >= min(m_tableLineSum,m_canShowLineSum)) return;
	//	if(order == 0) m_check0[lineOrder].setState(state);
	//	else m_check1[lineOrder].setState(state);
		if(lineOrder < 0 || lineOrder >= m_tableLineSum) return;
		if(order == 0) m_check0State[lineOrder] = state;
		else m_check1State[lineOrder] = state;
		//��������б���ѡ��״̬����Ϣ
		if(lineOrder >= m_showStartLine && lineOrder < min(m_showStartLine + m_canShowLineSum,m_tableLineSum))
		{
			if(m_check0State[lineOrder]) m_check0[lineOrder - m_showStartLine].setState(true);
			else m_check0[lineOrder - m_showStartLine].setState(false);
			if(m_check1State[lineOrder]) m_check1[lineOrder - m_showStartLine].setState(true);
			else m_check1[lineOrder - m_showStartLine].setState(false);
		}
	}

	int init(const _XVector2& position,		//�ռ����ڵ�λ��
		const _XRect& Area,					//�ؼ���ʵ����ʾ����
		const _XMultiListTexture &tex,		//�ؼ�����ͼ
		const _XCheckTexture &checktex0,		//��ѡ�����ͼ
		const _XCheckTexture &checktex1,		//��ѡ�����ͼ
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

	_XMultiListBasic();
	~_XMultiListBasic();
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
	_XMultiListBasic(const _XMultiListBasic &temp);
	_XMultiListBasic& operator = (const _XMultiListBasic& temp);
};

inline int _XMultiListBasic::addALine()					//��ĩβ����һ��
{
	return setLineSum(m_tableLineSum + 1);
}

inline int _XMultiListBasic::addARow()					//��ĩβ����һ��
{
	return setRowSum(m_tableRowSum + 1);
}

inline int _XMultiListBasic::deleteSelectLine()			//ɾ��ѡȡ��һ��
{
	if(m_isInited == 0) return 0;
	if(m_haveSelect == 0) return 0;
	return deleteLine(m_selectLineOrder);
}

inline int _XMultiListBasic::moveUpLine(int order)		//��order������
{
	return moveDownLine(order - 1);
}

inline int _XMultiListBasic::moveLeftRow(int order)		//��order������
{
	return moveRightRow(order - 1);
}

inline void _XMultiListBasic::setSize(const _XVector2& size)	//���óߴ�
{
	setSize(size.x,size.y);
}

inline void _XMultiListBasic::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}

#endif