//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XNodeLine.h"
#include "XBasicFun.h"
#include "XBasicSDL.h"
#include "stdio.h"
#include <math.h>
#include "XResourcePack.h"
#include "XObjectManager.h"
#include "XBasicOpenGL.h"

_XNodeLine::_XNodeLine()
:m_node(NULL)
,m_isACopy(0)
,m_myOrder(0)
,m_leadMode(_XLEAD_MODE_FOREWORD)	//���ߵ�����ģʽ
,m_lineLength(0)					//���ߵ��ܳ���
//,m_nodeSum(0)						//���������е������ڵ������
,m_offsetPosition(0,0)
,m_size(1.0f,1.0f)
,m_isLoop(0)
,m_isVisiable(1)
{
	try
	{
		m_cp = new _XSCounter;
		m_node = new _XNode[MAX_NODE_SUM];
		if(m_node == NULL) printf("Memory error!\n");
	}catch(...)
	{
		m_node = NULL;
		printf("Memory error!\n");
	}
	m_specialPoint.isEnable = 0;
	clearUp();
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_NODELINE);
#endif
}

_XNodeLine::~_XNodeLine()
{
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XDELETE_ARRAY(m_node);
		XDELETE(m_cp);
	}
	release();
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
}

void _XNodeLine::addOneNode(float x,float y)		//������ͷ�����һ����
{
	if(m_isACopy != 0) return;//�������ܽ����������
	int prevNodeNumber;
	if(m_nodeSum >= MAX_NODE_SUM || m_node == NULL) return;	//�����¼�ĵ��Ѿ�������ֱ�ӷ���
	m_node[m_nodeSum].myPoint.set(x,y);		//��¼��ǰ�ĵ����Ϣ
	m_node[m_nodeSum].toNextAngle = 0;
	m_node[m_nodeSum].toNextCos = 0;
	m_node[m_nodeSum].toNextSin = 0;
	m_node[m_nodeSum].toNextLength = 0;
	if(m_nodeSum > 0)
	{//��д��һ��������������Ϣ
		prevNodeNumber = m_nodeSum - 1;
		m_node[prevNodeNumber].toNextAngle = m_node[prevNodeNumber].myPoint.getAngle(m_node[m_nodeSum].myPoint);
		m_node[prevNodeNumber].toNextCos = cos(m_node[prevNodeNumber].toNextAngle);
		m_node[prevNodeNumber].toNextSin = sin(m_node[prevNodeNumber].toNextAngle);
		m_node[prevNodeNumber].toNextLength = m_node[prevNodeNumber].myPoint.getLength(m_node[m_nodeSum].myPoint);

		//��д��ǰ��������Ϣ
		m_node[m_nodeSum].toPreviousAngle = m_node[prevNodeNumber].toNextAngle + PI;
		if(m_node[m_nodeSum].toPreviousAngle >= PI2) m_node[m_nodeSum].toPreviousAngle -= PI2;
		m_node[m_nodeSum].toPreviousCos = cos(m_node[m_nodeSum].toPreviousAngle);
		m_node[m_nodeSum].toPreviousSin = sin(m_node[m_nodeSum].toPreviousAngle);
		m_node[m_nodeSum].toPreviousLength = m_node[prevNodeNumber].toNextLength;
		m_lineLength += m_node[m_nodeSum].toPreviousLength;
		
		if(m_isLoop == 0)
		{
			//���һ����ķ�����ǰһ����ķ��򱣳�һ��
			m_node[m_nodeSum].toNextAngle = m_node[prevNodeNumber].toNextAngle;
			m_node[m_nodeSum].toNextCos = m_node[prevNodeNumber].toNextCos;
			m_node[m_nodeSum].toNextSin = m_node[prevNodeNumber].toNextSin;
			if(m_nodeSum == 1)
			{//ͷ�ĵ㱣�ֺ�һ����ķ���
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
		}
	}
	++ m_nodeSum;
	if(m_isLoop != 0) setLoop();	//�������߱պ�
}

void _XNodeLine::setOneNode(float x,float y,int nodeOrder)
{
	if(m_isACopy != 0) return;//�������ܽ����������
	if(m_node == NULL) return;
	if(nodeOrder == 0)
	{//ͷ
		m_lineLength -= m_node[0].toNextLength;
		m_node[0].myPoint.set(x,y);
		m_node[0].toNextAngle = m_node[0].myPoint.getAngle(m_node[1].myPoint);
		m_node[0].toNextCos = cos(m_node[0].toNextAngle);
		m_node[0].toNextSin = sin(m_node[0].toNextAngle);
		m_node[0].toNextLength = m_node[0].myPoint.getLength(m_node[1].myPoint);

		m_lineLength += m_node[0].toNextLength;
		m_node[1].toPreviousAngle = m_node[0].toNextAngle + PI;
		if(m_node[1].toPreviousAngle >= PI2) m_node[1].toPreviousAngle -= PI2;
		m_node[1].toPreviousCos = cos(m_node[1].toPreviousAngle);
		m_node[1].toPreviousSin = sin(m_node[1].toPreviousAngle);
		m_node[1].toPreviousLength = m_node[0].toNextLength;
		if(m_isLoop == 0)
		{
			if(m_nodeSum > 1)
			{//ͷ�ĵ㱣�ֺ�һ����ķ���
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(m_nodeSum == 2)
			{
				m_node[1].toNextAngle = m_node[0].toNextAngle;
				m_node[1].toNextCos = m_node[0].toNextCos;
				m_node[1].toNextSin = m_node[0].toNextSin;
			}
		}
		if(m_isLoop != 0) setLoop();
	}else
	if(nodeOrder == m_nodeSum - 1)
	{//β
		m_lineLength -= m_node[nodeOrder].toPreviousLength;
		m_node[nodeOrder].myPoint.set(x,y);		//��¼��ǰ�ĵ����Ϣ
		m_node[nodeOrder].toNextAngle = 0;
		m_node[nodeOrder].toNextCos = 0;
		m_node[nodeOrder].toNextSin = 0;
		m_node[nodeOrder].toNextLength = 0;
		if(m_nodeSum > 1)
		{//��д��һ��������������Ϣ
			m_node[nodeOrder - 1].toNextAngle = m_node[nodeOrder - 1].myPoint.getAngle(m_node[nodeOrder].myPoint);
			m_node[nodeOrder - 1].toNextCos = cos(m_node[nodeOrder - 1].toNextAngle);
			m_node[nodeOrder - 1].toNextSin = sin(m_node[nodeOrder - 1].toNextAngle);
			m_node[nodeOrder - 1].toNextLength = m_node[nodeOrder - 1].myPoint.getLength(m_node[nodeOrder].myPoint);

			//��д��ǰ��������Ϣ
			m_node[nodeOrder].toPreviousAngle = m_node[nodeOrder - 1].toNextAngle + PI;
			if(m_node[nodeOrder].toPreviousAngle >= PI2) m_node[nodeOrder].toPreviousAngle -= PI2;
			m_node[nodeOrder].toPreviousCos = cos(m_node[nodeOrder].toPreviousAngle);
			m_node[nodeOrder].toPreviousSin = sin(m_node[nodeOrder].toPreviousAngle);
			m_node[nodeOrder].toPreviousLength = m_node[nodeOrder - 1].toNextLength;
			m_lineLength += m_node[nodeOrder].toPreviousLength;
			//���һ����ķ�����ǰһ����ķ��򱣳�һ��
			if(m_isLoop == 0)
			{
				m_node[nodeOrder].toNextAngle = m_node[nodeOrder - 1].toNextAngle;
				m_node[nodeOrder].toNextCos = m_node[nodeOrder - 1].toNextCos;
				m_node[nodeOrder].toNextSin = m_node[nodeOrder - 1].toNextSin;
				if(m_nodeSum == 2)
				{//ͷ�ĵ㱣�ֺ�һ����ķ���
					m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
					m_node[0].toPreviousCos = m_node[1].toPreviousCos;
					m_node[0].toPreviousSin = m_node[1].toPreviousSin;
				}
			}
		}
		if(m_isLoop != 0) setLoop();	//�������߱պ�
	}else
	{//�м�
		m_lineLength -= m_node[nodeOrder].toPreviousLength;
		m_lineLength -= m_node[nodeOrder].toNextLength;

		m_node[nodeOrder].myPoint.set(x,y);
		m_node[nodeOrder].toNextAngle = m_node[nodeOrder].myPoint.getAngle(m_node[nodeOrder + 1].myPoint);
		m_node[nodeOrder].toNextCos = cos(m_node[nodeOrder].toNextAngle);
		m_node[nodeOrder].toNextSin = sin(m_node[nodeOrder].toNextAngle);
		m_node[nodeOrder].toNextLength = m_node[nodeOrder].myPoint.getLength(m_node[nodeOrder + 1].myPoint);

		m_lineLength += m_node[nodeOrder].toNextLength;

		m_node[nodeOrder + 1].toPreviousAngle = m_node[nodeOrder].toNextAngle + PI;
		if(m_node[nodeOrder + 1].toPreviousAngle >= PI2) m_node[nodeOrder + 1].toPreviousAngle -= PI2;
		m_node[nodeOrder + 1].toPreviousCos = cos(m_node[nodeOrder + 1].toPreviousAngle);
		m_node[nodeOrder + 1].toPreviousSin = sin(m_node[nodeOrder + 1].toPreviousAngle);
		m_node[nodeOrder + 1].toPreviousLength = m_node[nodeOrder].toNextLength;

		m_node[nodeOrder].toPreviousAngle = m_node[nodeOrder].myPoint.getAngle(m_node[nodeOrder - 1].myPoint);
		m_node[nodeOrder].toPreviousCos = cos(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousSin = sin(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousLength = m_node[nodeOrder].myPoint.getLength(m_node[nodeOrder - 1].myPoint);
		m_lineLength += m_node[nodeOrder].toPreviousLength;

		m_node[nodeOrder - 1].toNextAngle = m_node[nodeOrder].toPreviousAngle + PI;
		if(m_node[nodeOrder - 1].toNextAngle >= PI2) m_node[nodeOrder - 1].toNextAngle -= PI2;
		m_node[nodeOrder - 1].toNextCos = cos(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextSin = sin(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextLength = m_node[nodeOrder - 1].myPoint.getLength(m_node[nodeOrder].myPoint);
		if(m_isLoop == 0)
		{
			if(nodeOrder == 1)
			{//ͷ�ĵ㱣�ֺ�һ����ķ���
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(nodeOrder == m_nodeSum - 2 && m_isLoop == 0)
			{//���һ����Ҫ����ǰһ����ķ���
				m_node[nodeOrder + 1].toNextAngle = m_node[nodeOrder].toNextAngle;
				m_node[nodeOrder + 1].toNextCos = m_node[nodeOrder].toNextCos;
				m_node[nodeOrder + 1].toNextSin = m_node[nodeOrder].toNextSin;
			}
		}
	}
}

void _XNodeLine::decreaseOneNode()
{
	if(m_isACopy != 0) return;//�������ܽ����������
	if(m_nodeSum <= 0 || m_node == NULL) return;
	m_lineLength -= m_node[m_nodeSum - 1].toPreviousLength;
	m_node[m_nodeSum - 1].myPoint = _XVector2(0,0);
//	m_node[m_nodeSum - 1].toNextAngle = 0;
//	m_node[m_nodeSum - 1].toNextCos = 0;
//	m_node[m_nodeSum - 1].toNextSin = 0;
	m_node[m_nodeSum - 1].toNextLength = 0;
	m_node[m_nodeSum - 1].toPreviousAngle = 0;
	m_node[m_nodeSum - 1].toPreviousCos = 0;
	m_node[m_nodeSum - 1].toPreviousSin = 0;
	m_node[m_nodeSum - 1].toPreviousLength = 0;
	-- m_nodeSum;
	if(m_isLoop != 0) setLoop();	//�������߱պ�
}

void _XNodeLine::addOneNode(float x,float y,int nodeOrder)		//�������м����һ����
{
	if(m_isACopy != 0) return;//�������ܽ����������
	if(m_nodeSum >= MAX_NODE_SUM || m_node == NULL) return;	//�����¼�ĵ��Ѿ�������ֱ�ӷ���
	if(nodeOrder == 0)
	{//ͷ
		for(int i = m_nodeSum;i > 0;-- i)
		{
			m_node[i] = m_node[i - 1];
		}
		m_node[0].myPoint.set(x,y);
		m_node[0].toNextAngle = m_node[0].myPoint.getAngle(m_node[1].myPoint);
		m_node[0].toNextCos = cos(m_node[0].toNextAngle);
		m_node[0].toNextSin = sin(m_node[0].toNextAngle);
		m_node[0].toNextLength = m_node[0].myPoint.getLength(m_node[1].myPoint);
		++ m_nodeSum;
		//m_lineLength += m_node[1].toPreviousLength;	//����Ӧ�������߼������
		m_lineLength += m_node[0].toNextLength;
		m_node[1].toPreviousAngle = m_node[0].toNextAngle + PI;
		if(m_node[1].toPreviousAngle >= PI2) m_node[1].toPreviousAngle -= PI2;
		m_node[1].toPreviousCos = cos(m_node[1].toPreviousAngle);
		m_node[1].toPreviousSin = sin(m_node[1].toPreviousAngle);
		m_node[1].toPreviousLength = m_node[0].toNextLength;
		if(m_isLoop == 0)
		{
			if(m_nodeSum > 1)
			{//ͷ����㱣��ǰһ����ķ���
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(m_nodeSum == 2)
			{
				m_node[1].toNextAngle = m_node[0].toNextAngle;
				m_node[1].toNextCos = m_node[0].toNextCos;
				m_node[1].toNextSin = m_node[0].toNextSin;
			}
		}else
		{
			setLoop();
		}
	}else
	if(nodeOrder == m_nodeSum)
	{//β
		addOneNode(x,y);
	}else
	{//�м�
		for(int i = m_nodeSum;i > nodeOrder;-- i)
		{
			m_node[i] = m_node[i - 1];
		}
		m_lineLength -= m_node[nodeOrder].toPreviousLength;
		m_node[nodeOrder].myPoint.set(x,y);
		m_node[nodeOrder].toNextAngle = m_node[nodeOrder].myPoint.getAngle(m_node[nodeOrder + 1].myPoint);
		m_node[nodeOrder].toNextCos = cos(m_node[nodeOrder].toNextAngle);
		m_node[nodeOrder].toNextSin = sin(m_node[nodeOrder].toNextAngle);
		m_node[nodeOrder].toNextLength = m_node[nodeOrder].myPoint.getLength(m_node[nodeOrder + 1].myPoint);
		m_lineLength += m_node[nodeOrder].toNextLength;

		m_node[nodeOrder + 1].toPreviousAngle = m_node[nodeOrder].toNextAngle + PI;
		if(m_node[nodeOrder + 1].toPreviousAngle >= PI2) m_node[nodeOrder + 1].toPreviousAngle -= PI2;
		m_node[nodeOrder + 1].toPreviousCos = cos(m_node[nodeOrder + 1].toPreviousAngle);
		m_node[nodeOrder + 1].toPreviousSin = sin(m_node[nodeOrder + 1].toPreviousAngle);
		m_node[nodeOrder + 1].toPreviousLength = m_node[nodeOrder].toNextLength;

		m_node[nodeOrder].toPreviousAngle = m_node[nodeOrder].myPoint.getAngle(m_node[nodeOrder - 1].myPoint);
		m_node[nodeOrder].toPreviousCos = cos(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousSin = sin(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousLength = m_node[nodeOrder].myPoint.getLength(m_node[nodeOrder - 1].myPoint);
		m_lineLength += m_node[nodeOrder].toPreviousLength;

		m_node[nodeOrder - 1].toNextAngle = m_node[nodeOrder].toPreviousAngle + PI;
		if(m_node[nodeOrder - 1].toNextAngle >= PI2) m_node[nodeOrder - 1].toNextAngle -= PI2;
		m_node[nodeOrder - 1].toNextCos = cos(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextSin = sin(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextLength = m_node[nodeOrder - 1].myPoint.getLength(m_node[nodeOrder].myPoint);
		++ m_nodeSum;
		if(m_isLoop == 0)
		{
			if(nodeOrder == 1)
			{//ͷ����㱣��ǰһ����ķ���
				m_node[0].toPreviousAngle = m_node[1].toPreviousAngle;
				m_node[0].toPreviousCos = m_node[1].toPreviousCos;
				m_node[0].toPreviousSin = m_node[1].toPreviousSin;
			}
			if(nodeOrder == m_nodeSum - 2)
			{//���һ������ǰһ���㱣��һ��
				m_node[m_nodeSum - 1].toNextAngle = m_node[nodeOrder].toNextAngle;
				m_node[m_nodeSum - 1].toNextCos = m_node[nodeOrder].toNextCos;
				m_node[m_nodeSum - 1].toNextSin = m_node[nodeOrder].toNextSin;
			}
		}else
		{
			setLoop();	//�������߱պ�
		}
	}
}

void _XNodeLine::decreaseOneNode(int nodeOrder)
{
	//������ݵĺ�����
	if(m_isACopy != 0) return;//�������ܽ����������
	if(nodeOrder <= 0 || nodeOrder >= m_nodeSum) return;
	if(nodeOrder == 0)
	{//ͷ
		for(int i = 0;i < m_nodeSum - 1;++ i)
		{
			m_node[i] = m_node[i + 1];
		}
		m_lineLength -= m_node[0].toPreviousLength;
	//	m_node[0].toPreviousAngle = 0;
	//	m_node[0].toPreviousCos = 0;
	//	m_node[0].toPreviousSin = 0;
		m_node[0].toPreviousLength = 0;
		//���һ����Ϊ���к�decreaseOneNode�����еļ����������ӵ�
		m_lineLength += m_node[m_nodeSum - 1].toPreviousLength;
		decreaseOneNode();
	}else
	if(nodeOrder == m_nodeSum - 1)
	{//β
		decreaseOneNode();
	}else
	{//�м�
		m_lineLength -= m_node[nodeOrder].toPreviousLength;
		m_lineLength -= m_node[nodeOrder + 1].toPreviousLength;
		for(int i = nodeOrder;i < m_nodeSum - 1;++ i)
		{
			m_node[i] = m_node[i + 1];
		}
		m_node[nodeOrder - 1].toNextAngle = m_node[nodeOrder - 1].myPoint.getAngle(m_node[nodeOrder].myPoint);
		m_node[nodeOrder - 1].toNextCos = cos(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextSin = sin(m_node[nodeOrder - 1].toNextAngle);
		m_node[nodeOrder - 1].toNextLength = m_node[nodeOrder - 1].myPoint.getLength(m_node[nodeOrder].myPoint);
		m_lineLength += m_node[nodeOrder - 1].toNextLength;
		m_node[nodeOrder].toPreviousAngle = m_node[nodeOrder - 1].toNextAngle + PI;
		if(m_node[nodeOrder].toPreviousAngle >= PI2) m_node[nodeOrder].toPreviousAngle -= PI2;
		m_node[nodeOrder].toPreviousCos = cos(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousSin = sin(m_node[nodeOrder].toPreviousAngle);
		m_node[nodeOrder].toPreviousLength = m_node[nodeOrder - 1].toNextLength;
		//���һ����Ϊ���к�decreaseOneNode�����еļ����������ӵ�
		m_lineLength += m_node[m_nodeSum - 1].toPreviousLength;
		decreaseOneNode();
	}
}

void _XNodeLine::clearUp()
{//ֻ��Ҫ�����һ�������Ϣ����
	if(m_isACopy != 0) return;//�������ܽ����������
	m_node[0].myPoint = _XVector2(0,0);
	m_node[0].toNextAngle = 0;
	m_node[0].toNextCos = 0;
	m_node[0].toNextSin = 0;
	m_node[0].toNextLength = 0;
	m_node[0].toPreviousAngle = 0;
	m_node[0].toPreviousCos = 0;
	m_node[0].toPreviousSin = 0;
	m_node[0].toPreviousLength = 0;
	m_nodeSum = 0;
	m_lineLength = 0;
	m_isLoop = 0;
}

int _XNodeLine::saveNodeLine()
{
	if(m_isACopy != 0) return 0;//�������ܽ����������
	if(m_nodeSum <= 1) return 0;
	FILE *fp;
	char fileName[] = NODELINE_DATA_FILE_NAME;
	if(m_myOrder >= 0) fileName[13] = m_myOrder % 10 + '0';
	if(m_myOrder >= 10) fileName[12] = (m_myOrder/10) % 10 + '0';
	if(m_myOrder >= 100) fileName[11] = (m_myOrder/100) % 10 + '0';
	if(m_myOrder >= 1000) fileName[10] = (m_myOrder/1000) % 10 + '0';
	if(m_myOrder >= 10000) fileName[9] = (m_myOrder/10000) % 10 + '0';
	//���ļ�
	if((fp = fopen(fileName,"wb")) == NULL)
	{
		printf("Line file open error!\n");
		return 0;
	}
	//д������
	fwrite(&m_nodeSum,sizeof(int),1,fp);			//�ڵ�����
	fwrite(&m_lineLength,sizeof(float),1,fp);		//�����߳���
	//����д��ڵ���Ϣ
	for(int i = 0;i < m_nodeSum;++ i)
	{
		fwrite(&m_node[i],sizeof(_XNode),1,fp);	
	}
	//д�����֮��ر��ļ�
	fclose(fp);
	return 1;
}

int _XNodeLine::getNodeLine(int resoursePosition)
{
	if(m_isACopy != 0) return 0;//�������ܽ����������
	char fileName[] = NODELINE_DATA_FILE_NAME;
	if(m_myOrder >= 0) fileName[13] = m_myOrder % 10 + '0';
	if(m_myOrder >= 10) fileName[12] = (m_myOrder/10) % 10 + '0';
	if(m_myOrder >= 100) fileName[11] = (m_myOrder/100) % 10 + '0';
	if(m_myOrder >= 1000) fileName[10] = (m_myOrder/1000) % 10 + '0';
	if(m_myOrder >= 10000) fileName[9] = (m_myOrder/10000) % 10 + '0';
	if(resoursePosition == RESOURCE_OUTSIDE)
	{
		FILE *fp;
		//���ļ�
		if((fp = fopen(fileName,"rb")) == NULL)
		{
			printf("Line file open error!\n");
			return 0;
		}
		//��ȡ����
		fread(&m_nodeSum,sizeof(int),1,fp);		//�ڵ�����
		fread(&m_lineLength,sizeof(float),1,fp);		//�����߳���
		//���ζ�ȡ�ڵ���Ϣ
		for(int i = 0;i < m_nodeSum;++ i)
		{
			fread(&m_node[i],sizeof(_XNode),1,fp);	
		}
		//��ȡ���֮��ر��ļ�
		fclose(fp);
	}else
	{
		//����Դ�ļ�����ȡ��Դ
		int length = _XResourcePack::GetInstance().getFileLength(fileName);
		if(length < 0) return NULL;
		unsigned char *p = NULL;
		try
		{
			p = new unsigned char[length + 1];
			if(p == NULL) return 0;
		}catch(...)
		{
			return  0;
		}
		if(_XResourcePack::GetInstance().unpackResource(fileName,p) != 1)
		{
			printf("Load NodeLine Data Error!\n");
			return 0;
		}
		//����ȡ�������ͷŵ�ָ����λ��
		int offset = 0;
		memcpy(&m_nodeSum,p + offset,sizeof(int));
		offset += sizeof(int);
		memcpy(&m_lineLength,p + offset,sizeof(float));
		offset += sizeof(float);
		//����д��ڵ���Ϣ
		for(int i = 0;i < m_nodeSum;++ i)
		{
			memcpy(&m_node[i],p + offset,sizeof(_XNode));
			offset += sizeof(_XNode);
		}
		XDELETE_ARRAY(p);
	}
	return 1;
}
//ע��������Ҫ��2����
void _XNodeLine::moveSpecialPoint(int timeDelay,int isLoop)
{
	if(m_nodeSum <= 1) return;
	if(isLoop != 0 && m_isLoop == 0)
	{
		setLoop();
	}
	if(m_specialPoint.isEnable != 0 && m_specialPoint.isEnd == 0)
	{
		m_specialPoint.nowLength += timeDelay * m_specialPoint.speed;
		if((m_leadMode & _XLEAD_MODE_FOREWORD) != 0)
		{//˳��
			while(1)
			{
				if(m_specialPoint.nowLength > m_specialPoint.upNodeLength + m_node[m_specialPoint.upNodeOrder].toNextLength)
				{//�Ѿ�Խ������һ����
					if(isLoop == 0)
					{//��ѭ��
						if(m_specialPoint.upNodeOrder + 1 == m_nodeSum - 1)
						{//�Ѿ������յ�
							m_specialPoint.isEnd = 1;
							m_specialPoint.angle = m_node[m_nodeSum - 2].toNextAngle;
							m_specialPoint.nowLength = m_lineLength;
							m_specialPoint.position = m_node[m_nodeSum - 1].myPoint;
							m_specialPoint.upNodeOrder = m_nodeSum - 2;
							m_specialPoint.upNodeLength = m_lineLength - m_node[m_nodeSum - 1].toPreviousLength;
							break;
						}else
						{
							++ m_specialPoint.upNodeOrder;
							m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toPreviousLength;
						}
					}else
					{//ѭ��
						if(m_specialPoint.upNodeOrder == m_nodeSum - 1)
						{//���µ������
							m_specialPoint.isEnd = 1;
							m_specialPoint.angle = m_node[0].toNextAngle;
							m_specialPoint.nowLength = m_lineLength + m_node[m_nodeSum - 1].toNextLength;
							m_specialPoint.position = m_node[0].myPoint;
							m_specialPoint.upNodeOrder = m_nodeSum - 1;
							m_specialPoint.upNodeLength = m_lineLength;
							break;
						}else
						{
							++ m_specialPoint.upNodeOrder;
							m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toPreviousLength;
						}
					}
				}else
				{//û��Խ������һ����
					if(m_specialPoint.upNodeOrder == m_nodeSum - 1)
					{
						m_specialPoint.position.x = m_node[m_specialPoint.upNodeOrder].myPoint.x 
							+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toNextCos;
						m_specialPoint.position.y = m_node[m_specialPoint.upNodeOrder].myPoint.y 
							+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toNextSin;
						if(m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle <= PI
							&& m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle >= -PI)
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
								* (m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle);
						}else
						if(m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle > PI)
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
								* (PI2 - (m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle));
						}else
						if(m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle < -PI)
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
								* (PI2 + (m_node[0].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle));
						}
					}else
					{
						m_specialPoint.position.x = m_node[m_specialPoint.upNodeOrder].myPoint.x 
							+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toNextCos;
						m_specialPoint.position.y = m_node[m_specialPoint.upNodeOrder].myPoint.y 
							+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toNextSin;
						if(m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle <= PI
							&& m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle >= -PI)
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
								* (m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle);
						}else
						if(m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle > PI)
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
								* (m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle - PI2);
						}else
						if(m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle < -PI)
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toNextAngle 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toNextLength 
								* (PI2 + (m_node[m_specialPoint.upNodeOrder + 1].toNextAngle - m_node[m_specialPoint.upNodeOrder].toNextAngle));
						}	
					}
					break;
				}
			}
		}else
		{//����
			while(1)
			{
				if(m_specialPoint.nowLength > m_specialPoint.upNodeLength + m_node[m_specialPoint.upNodeOrder].toPreviousLength)
				{//�Լ�Խ������һ����
					if(isLoop == 0)
					{//��ѭ��
						if(m_specialPoint.upNodeOrder == 1)
						{//�Ѿ������յ�
							m_specialPoint.isEnd = 1;
							m_specialPoint.angle = m_node[1].toPreviousAngle;
							m_specialPoint.nowLength = m_lineLength;
							m_specialPoint.position = m_node[0].myPoint;
							m_specialPoint.upNodeOrder = 1;
							m_specialPoint.upNodeLength = m_lineLength - m_node[0].toNextLength;
							break;
						}else
						{
							-- m_specialPoint.upNodeOrder;
							m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toNextLength;
						}
					}else
					{
						if(m_specialPoint.upNodeOrder == 0)
						{//�Ѿ������յ�
							m_specialPoint.isEnd = 1;
							m_specialPoint.angle = m_node[m_nodeSum - 1].toPreviousAngle;
							m_specialPoint.nowLength = m_lineLength + m_node[0].toPreviousLength;
							m_specialPoint.position = m_node[m_nodeSum - 1].myPoint;
							m_specialPoint.upNodeOrder = 0;
							m_specialPoint.upNodeLength = m_lineLength;
							break;
						}else
						{
							-- m_specialPoint.upNodeOrder;
							m_specialPoint.upNodeLength += m_node[m_specialPoint.upNodeOrder].toNextLength;
						}
					}
				}else
				{//û��Խ������һ����
					if(m_specialPoint.upNodeOrder == 0)
					{
						m_specialPoint.position.x = m_node[m_specialPoint.upNodeOrder].myPoint.x 
							+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toPreviousCos;
						m_specialPoint.position.y = m_node[m_specialPoint.upNodeOrder].myPoint.y 
							+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toPreviousSin;
						if(m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle <= PI
							&& m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle >= -PI)
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
								* (m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle);
						}else
						if(m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle > PI)
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
								* ((m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle) - PI2);
						}else
						if(m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle < -PI)
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
								* (PI2 + (m_node[m_nodeSum - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle));
						}
					}else
					{
						m_specialPoint.position.x = m_node[m_specialPoint.upNodeOrder].myPoint.x 
							+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toPreviousCos;
						m_specialPoint.position.y = m_node[m_specialPoint.upNodeOrder].myPoint.y 
							+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength) * m_node[m_specialPoint.upNodeOrder].toPreviousSin;
						if(m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle <= PI
							&& m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle >=  -PI)
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
								* (m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle);
						}else
						if(m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle > PI)
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
								* ((m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle) - PI2);
						}else
						if(m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle < -PI)
						{
							m_specialPoint.angle = m_node[m_specialPoint.upNodeOrder].toPreviousAngle 
								+ (m_specialPoint.nowLength - m_specialPoint.upNodeLength)/m_node[m_specialPoint.upNodeOrder].toPreviousLength 
								* (PI2 + (m_node[m_specialPoint.upNodeOrder - 1].toPreviousAngle - m_node[m_specialPoint.upNodeOrder].toPreviousAngle));
						}					
					}
					break;
				}
			}
		}
	}
}

void _XNodeLine::resetSpecialPoint()
{
	if(m_nodeSum <= 1) return;
	if((m_leadMode & _XLEAD_MODE_FOREWORD) != 0)
	{//˳��
		m_specialPoint.isEnable = 1;
		m_specialPoint.isEnd = 0;
		m_specialPoint.nowLength = 0;
		m_specialPoint.position = _XVector2(m_node[0].myPoint.x,m_node[0].myPoint.y);
		m_specialPoint.angle = m_node[0].toNextAngle;
		m_specialPoint.upNodeLength = 0;
		m_specialPoint.upNodeOrder = 0;
	}else
	{//����
		m_specialPoint.isEnable = 1;
		m_specialPoint.isEnd = 0;
		m_specialPoint.nowLength = 0;
		m_specialPoint.position = _XVector2(m_node[m_nodeSum - 1].myPoint.x,m_node[m_nodeSum - 1].myPoint.y);
		m_specialPoint.angle = m_node[m_nodeSum - 1].toPreviousAngle;
		m_specialPoint.upNodeLength = 0;
		m_specialPoint.upNodeOrder = m_nodeSum - 1;
	}
}

void _XNodeLine::setLoop()
{
	if(m_isACopy != 0) return;//�������ܽ����������
	if(m_nodeSum < 2) return;	//С���������ǲ���ѭ����
	//����һ��������һ������������
	m_node[m_nodeSum - 1].toNextAngle = m_node[m_nodeSum - 1].myPoint.getAngle(m_node[0].myPoint);
	m_node[m_nodeSum - 1].toNextCos = cos(m_node[m_nodeSum - 1].toNextAngle);
	m_node[m_nodeSum - 1].toNextSin = sin(m_node[m_nodeSum - 1].toNextAngle);
	m_node[m_nodeSum - 1].toNextLength = m_node[m_nodeSum - 1].myPoint.getLength(m_node[0].myPoint);
	if(m_isLoop != 0)
	{//���������ԭ�����Ǳպϵģ�������Ҫ��ȥԭ���ĳ���
		m_lineLength -= m_node[0].toPreviousLength;
	}
	m_node[0].toPreviousAngle = m_node[m_nodeSum - 1].toNextAngle + PI;
	if(m_node[0].toPreviousAngle >= PI2) m_node[0].toPreviousAngle -= PI2;
	m_node[0].toPreviousCos = cos(m_node[0].toPreviousAngle);
	m_node[0].toPreviousSin = sin(m_node[0].toPreviousAngle);
	m_node[0].toPreviousLength = m_node[m_nodeSum - 1].toNextLength;

	m_lineLength += m_node[0].toPreviousLength; //�����µĳ���
	m_isLoop = 1;
}

int _XNodeLine::setACopy(const _XNodeLine &temp)
{
	if(this == &temp) return 0;		//��ֹ��������
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//�ͷ��������Դ
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XDELETE_ARRAY(m_node);
		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;

//	if(m_isACopy == 0)
//	{//��������Ǹ�������Ҫ�ͷ���Դ
//		XDELETE_ARRAY(m_node);
//		m_isACopy = 1;
//	}
	//��������
	m_isVisiable = temp.m_isVisiable;					//���������ߵı��
	m_myOrder = temp.m_myOrder;					//���������ߵı��
	m_node = temp.m_node;						//�������еĽڵ�
	m_leadMode = temp.m_leadMode;				//���ߵ�����ģʽ
	m_isLoop = temp.m_isLoop;					//�����Ƿ�պ�0���պ� 1�պϣ�ע��պϵĵ㣬�׵��β�㲻���غϣ����򽫻�ʧȥβ��ķ���

	m_offsetPosition = temp.m_offsetPosition;	//�������ߵ�ƫ��λ��
	m_size = temp.m_size;						//������ߵ����ű���Ҫ���ձ������У����ܹ�ʹ��ˮƽ�ʹ�ֱ�Ĳ�ͬ���������򽫻���������нǶȵ�ʧ������
//	m_isACopy = temp.m_isACopy;					//�Ƿ���һ������

	m_lineLength = temp.m_lineLength;			//���ߵ��ܳ���
	m_nodeSum = temp.m_nodeSum;					//���������е������ڵ������
	m_specialPoint = temp.m_specialPoint;		//�����ϵ��ƶ��ĵ�
	return 1;
}

_XNodeLine::_XNodeLine(const _XNodeLine &temp)
{
	//m_node = new _XNode[MAX_NODE_SUM];
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	m_cp = temp.m_cp;

	//��������
	m_isVisiable = temp.m_isVisiable;					//���������ߵı��
	m_myOrder = temp.m_myOrder;					//���������ߵı��
	m_leadMode = temp.m_leadMode;				//���ߵ�����ģʽ
	m_isLoop = temp.m_isLoop;					//�����Ƿ�պ�0���պ� 1�պϣ�ע��պϵĵ㣬�׵��β�㲻���غϣ����򽫻�ʧȥβ��ķ���

	m_offsetPosition = temp.m_offsetPosition;	//�������ߵ�ƫ��λ��
	m_size = temp.m_size;						//������ߵ����ű���Ҫ���ձ������У����ܹ�ʹ��ˮƽ�ʹ�ֱ�Ĳ�ͬ���������򽫻���������нǶȵ�ʧ������

	m_lineLength = temp.m_lineLength;			//���ߵ��ܳ���
	m_nodeSum = temp.m_nodeSum;					//���������е������ڵ������
	m_specialPoint = temp.m_specialPoint;		//�����ϵ��ƶ��ĵ�
	m_node = temp.m_node;						//�������еĽڵ�
//	for(int i = 0;i < m_nodeSum;++ i)
//	{
//		m_node[i] = temp.m_node[i];
//	}
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_NODELINE);
#endif

	m_isACopy = 0;
}

_XNodeLine& _XNodeLine::operator = (const _XNodeLine& temp)
{
	if(&temp == this) return *this;
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//�ͷ��������Դ
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XDELETE_ARRAY(m_node);
		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;

//	if(m_node == NULL || m_isACopy != 0)
//	{//��Դ�Ѿ����ͷţ�������Դ�������Լ������������·����ڴ�ռ�
//		m_node = new _XNode[MAX_NODE_SUM];
//	}
	m_isVisiable = temp.m_isVisiable;					//���������ߵı��
	m_myOrder = temp.m_myOrder;					//���������ߵı��
	m_leadMode = temp.m_leadMode;				//���ߵ�����ģʽ
	m_isLoop = temp.m_isLoop;					//�����Ƿ�պ�0���պ� 1�պϣ�ע��պϵĵ㣬�׵��β�㲻���غϣ����򽫻�ʧȥβ��ķ���

	m_offsetPosition = temp.m_offsetPosition;	//�������ߵ�ƫ��λ��
	m_size = temp.m_size;						//������ߵ����ű���Ҫ���ձ������У����ܹ�ʹ��ˮƽ�ʹ�ֱ�Ĳ�ͬ���������򽫻���������нǶȵ�ʧ������

	m_lineLength = temp.m_lineLength;			//���ߵ��ܳ���
	m_nodeSum = temp.m_nodeSum;					//���������е������ڵ������
	m_specialPoint = temp.m_specialPoint;		//�����ϵ��ƶ��ĵ�
	m_node = temp.m_node;						//�������еĽڵ�
//	for(int i = 0;i < m_nodeSum;++ i)
//	{
//		m_node[i] = temp.m_node[i];
//	}
	m_isACopy = 0;

	return *this;
}

int _XNodeLine::isInRect(float x,float y)
{
//	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
	_XVector2 point(x,y);
	_XVector2 pointS,pointE;
	if(m_nodeSum == 1)
	{//ֻ��һ����Ľڵ����ߣ��ж������ľ���
		if(point.getLength(_XVector2(m_node[0].myPoint.x * m_size.x + m_offsetPosition.x,
			m_node[0].myPoint.y * m_size.y + m_offsetPosition.y)) <= 5) return 1;
		else return 0;
	}else
	if(m_nodeSum >= 1)
	{
		for(int i = 0;i < m_nodeSum - 1;++ i)
		{
			pointS.set(m_node[i].myPoint.x * m_size.x + m_offsetPosition.x,
				m_node[i].myPoint.y * m_size.y + m_offsetPosition.y);
			pointE.set(m_node[i + 1].myPoint.x * m_size.x + m_offsetPosition.x,
				m_node[i + 1].myPoint.y * m_size.y + m_offsetPosition.y);
			if(minDistancePointToLine(point,pointS,pointE) <= 5) return 1;
		}
	}
	return 0;
}

_XVector2 _XNodeLine::getBox(int order)
{//������Ҫ���ذ�Χ��
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	float left = 0.0f;
	float right = 0.0f;
	float top = 0.0f;
	float bottom = 0.0f;
	if(m_nodeSum > 0)
	{//����ڵ��������нڵ㣬���������ڵ�İ�Χ��
		left = m_node[0].myPoint.x;
		right = m_node[0].myPoint.x;
		top = m_node[0].myPoint.y;
		bottom = m_node[0].myPoint.y;
		//���濪ʼ�������еĵ㣬Ѱ�Ұ�Χ����
		for(int i = 1;i < m_nodeSum;++ i)
		{
			if(m_node[i].myPoint.x > right) right = m_node[i].myPoint.x;
			if(m_node[i].myPoint.x < left) left = m_node[i].myPoint.x;
			if(m_node[i].myPoint.y > bottom) bottom = m_node[i].myPoint.y;
			if(m_node[i].myPoint.y < top) top = m_node[i].myPoint.y;
		}
	}
	//����԰�Χ���ӽ�����չ����ֹ������ߵ��ߵ�����
	left -= 5;
	right += 5;
	top -= 5;
	bottom += 5;
	if(order == 0) ret.set(left * m_size.x + m_offsetPosition.x,	top * m_size.y + m_offsetPosition.y);else
	if(order == 1) ret.set(right * m_size.x + m_offsetPosition.x,	top * m_size.y + m_offsetPosition.y);else
	if(order == 2) ret.set(right * m_size.x + m_offsetPosition.x,	bottom * m_size.y + m_offsetPosition.y);else
	if(order == 3) ret.set(left * m_size.x + m_offsetPosition.x,	bottom * m_size.y + m_offsetPosition.y);
	return ret;
}