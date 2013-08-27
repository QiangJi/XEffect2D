#ifndef _JIA_XNODELINE_
#define _JIA_XNODELINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "stdlib.h"
#include "XBasicClass.h"
#include "XBasicFun.h"
#include "XObjectBasic.h"

#define NODELINE_DATA_FILE_NAME ("line/line00000.dat")

#pragma pack(push)
#pragma pack(1)
struct _XNode
{
	_XVector2 myPoint;		//������λ��
	float toNextAngle;		//����һ����ĽǶ�
	float toNextSin;		//����һ����ĽǶȵ�sinֵ
	float toNextCos;		//����һ����ĽǶȵ�cosֵ
	float toNextLength;		//����һ����ľ���

	float toPreviousAngle;	//��ǰһ����ĽǶ�
	float toPreviousSin;	//��ǰһ����ĽǶȵ�sinֵ
	float toPreviousCos;	//��ǰһ����ĽǶȵ�cosֵ
	float toPreviousLength;	//��ǰһ����ľ���
};
#pragma pack(pop)

//�����ߵ�����ģʽ
enum
{
//	_XLEAD_MODE_NO_ROTATE = 0x01,		//û�нǶȱ仯
//	_XLEAD_MODE_ROTATE = 0x02,			//�������ߵĽǶȱ仯���仯
	_XLEAD_MODE_FOREWORD = 0x04,		//˳���ƶ�����
	_XLEAD_MODE_BACKWORD = 0x08			//�����ƶ�����
};

//���������е������
struct _XNodeSpecialPoint
{
	char isEnable;			//������Ƿ���Ч
	char isEnd;				//������Ƿ��Ѿ��������ߵ��յ�
	float nowLength;		//������߹��ľ���
	_XVector2 position;		//������λ��
	float angle;			//�����ĽǶ�
	float upNodeLength;		//��һ���ڵ�����
	int upNodeOrder;		//��һ���ڵ�ı��
	_XVector2 divergePoint;	//������ƫ��λ��
	float speed;			//�������ٶ�
	_XNodeSpecialPoint()
		:isEnable(0)
		,isEnd(1)
	{
	}
};
//�ڵ������нڵ��������������
#define MAX_NODE_SUM 64
//����չ��ʱ����ʹ��
//typedef int XNodeSpecialPointHandle	//�����ڵ��е������ľ��

//ע�⣺����һЩ������Ҫ��1������������Ҫ�����㣬2�����в��ܳ��������غϵĵ㡣�������㻹û�н����

class _XSCounter;
//�ڵ��������ߵ���
class _XNodeLine:public _XObjectBasic
{
private:
	_XSCounter *m_cp;				//���ü�����

	int m_myOrder;					//���������ߵı��
	_XNode *m_node;					//�������еĽڵ�
	int m_leadMode;					//���ߵ�����ģʽ
	int m_isLoop;					//�����Ƿ�պ�0���պ� 1�պϣ�ע��պϵĵ㣬�׵��β�㲻���غϣ����򽫻�ʧȥβ��ķ���

	_XVector2 m_offsetPosition;		//�������ߵ�ƫ��λ��
	_XVector2 m_size;					//������ߵ����ű���Ҫ���ձ������У����ܹ�ʹ��ˮƽ�ʹ�ֱ�Ĳ�ͬ���������򽫻���������нǶȵ�ʧ������
	char m_isACopy;					//�Ƿ���һ������
	char m_isVisiable;

	float m_lineLength;					//���ߵ��ܳ���
	int m_nodeSum;						//���������е������ڵ������
public:
	void setVisiable() {m_isVisiable = 1;}					//��������ɼ�
	void disVisiable() {m_isVisiable = 0;}						//����������ɼ�
	char getVisiable() const {return m_isVisiable;}					//��ȡ����Ƿ�ɼ���״̬ 
	//���ڲ�����������
	void setOneNode(const _XVector2& node,int nodeOrder);	//�ı�����ĳ�����λ��
	void setOneNode(float x,float y,int nodeOrder);	//�ı�����ĳ�����λ��
	void addOneNode(const _XVector2& node);		//������ͷ�����һ����
	void addOneNode(float x,float y);	//������ͷ�����һ����
	void decreaseOneNode();				//������ͷ���޳�һ����
	void addOneNode(const _XVector2& node,int nodeOrder);		//�������м����һ����
	void addOneNode(float x,float y,int nodeOrder);		//�������м����һ����
	void decreaseOneNode(int nodeOrder);	//�������м��޳�һ����
	int saveNodeLine();					//�����������ߣ�ʹ�ú����ļ�������ʽ�����д�����
	int getOrder() const;						//��ȡ�����ŵ���Ϣ
	void setMyOrder(int temp);			//�������ߵı�ţ�ÿ�����߶���һ����ţ��ڱ�������ʱ��������������
	void clearUp();						//������еĵ�
	void setLoop();						//�������߱պ�
	int getNodeOrder(float x,float y,float distance) const	//����ָ������Ͼ���Ҫ�������ĵ�ı�ţ����û���򷵻�-1
	{
		if(distance < 0) return -1;
		_XVector2 temp(x,y);
		for(int i = 0;i < m_nodeSum;i ++)
		{
			if(temp.getLength(m_node[i].myPoint) <= distance) return i;
		}
		return -1;
	}

	int getNodeSum() const	//���ؽڵ������еĽڵ�����
	{
		return m_nodeSum;
	}
	_XVector2 getNode(int order) const	//��ȡ�ڵ�������ĳ���ڵ��λ��
	{
		if(order < 0 || order >= m_nodeSum) return _XVector2(0.0f,0.0f);
		return m_node[order].myPoint;
	}
	//int setSize(float size);			//�������ߵĳߴ�
	void setPosition(const _XVector2& position);	//�������ߵ�ƫ��λ��
	void setPosition(float x,float y);	//�������ߵ�ƫ��λ��
	_XVector2 getPosition() const
	{
		return m_offsetPosition;
	}
	_XVector2 getSize() const
	{
		return m_size;
	}
	void setSize(float x,float y)
	{
		m_size.set(x,y);
	}
	void setSize(float x)
	{
		m_size.set(x,x);
	}
	void setSize(const _XVector2& size)
	{
		m_size = size;
	}
	float getAngle() const	//�������ӿڶ��������û��ʵ�ʵ�����
	{
		return 0;
	}
	float getNodeLineLength() const	//��ȡ�ڵ����ߵ��ܳ���
	{
		return m_lineLength;
	}
	void setAngle(float angle)	//�������ӿڶ��������û��ʵ�ʵ�����
	{//ɶҲ����
	}
	//Ϊ����������ʵ������Ľӿ�
	int isInRect(float x,float y);
	_XVector2 getBox(int order);

private:
	//�������ߵ�ʹ��
	_XNodeSpecialPoint m_specialPoint;	//�����е������
	void release();		//�ͷ���Դ
public:
	int getNodeLine(int resoursePosition = 0);					//���ļ��ж�ȡ��������
	void moveSpecialPoint(int timeDelay,int isLoop);	//�ƶ������е����ֵ�
	void resetSpecialPoint();				//����������

	void setLeadMode(int mode);				//�������ߵ����ͣ�˳�����е����ߣ������������е�����
	void setSpecialPointSpeed(float speed);	//���������еĵ���ٶ�
	int getSpecialPointIsEnd() const;				//��ȡ�����еĵ��Ƿ��Ѿ��ִ��յ�
	_XVector2 getSpecialPointPosition() const;		//��ȡ�����еĵ��λ��
	float getSpecialPointAngle() const;			//��ȡ�����еĵ�ĵ�ǰ�Ƕ�

	//setACopy�������������Ŀ�����Դ���������Ŀ�걻��ǰ�ͷŵ������������⣬������Ҫ����ʹ��
	int setACopy(const _XNodeLine &temp);			//����һ������������ӵ�к�ԭ��һ������Դ��������ӵ���Լ��ĸ��ԣ���������Դ�ǲ����޸ĵ�
	_XNodeLine(const _XNodeLine &temp);				//���صĿ������캯������ȫ����������������ֻ������ֵ��ͬ����
	_XNodeLine& operator = (const _XNodeLine& temp);//���صĸ�ֵ����������ȫ����������������ֻ������ֵ��ͬ����

	_XNodeLine();
	~_XNodeLine();	//���������������ͷ���Դ
};

#include "XNodeLine.inl"

#endif