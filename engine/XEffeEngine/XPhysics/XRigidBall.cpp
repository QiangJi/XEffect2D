//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.12.10
//--------------------------------
#include "XRigidBall.h"

float gravity = 9.80f;
//float gravity = 0.00f;
#define CRASH_SPEED_DOWN (1.0f)

int _XLiquidBalls::init(int liquidBallSum,int lineSum)
{
	if(m_isInited != 0) return 0;
	if(liquidBallSum <= 0) return 0;
	if(lineSum <= 0) return 0;

	m_liquidBallSum = liquidBallSum;
	m_pRigidBall = createArrayMem<_XRigidBall>(m_liquidBallSum);
	if(m_pRigidBall == NULL) return 0;

	m_lineSum = lineSum;
	m_pLine = createArrayMem<_XLine>(m_lineSum);
	if(m_pLine == NULL) 
	{
		XDELETE_ARRAY(m_pRigidBall);
		return 0;
	}

	//m_area.set(50.0f,50.0f,1000.0f,700.0f);
	//m_liquidBallDataSum = ((1 + m_liquidBallSum + m_lineSum - 1) * (m_liquidBallSum + m_lineSum - 1)) >> 1;
	//�߶���Ҫ��ײ���ж�Ϊ�������壬�߶α����Լ��߶������˵�
	m_liquidBallDataSum = (((3 * m_lineSum) + (m_liquidBallSum - 1 + 3 * m_lineSum)) * m_liquidBallSum) >> 1;
//	m_pLiquidBallData = createArrayMem<double>(m_liquidBallDataSum);
//	if(m_pLiquidBallData == NULL) return 0;
//	m_pLiquidBallFlag = createArrayMem<char>(m_liquidBallDataSum);
//	if(m_pLiquidBallFlag == NULL) return 0;

	m_pCrashData = createArrayMem<_XCrashData>(m_liquidBallDataSum);
	if(m_pCrashData == NULL)
	{
		XDELETE_ARRAY(m_pRigidBall);
		XDELETE_ARRAY(m_pLine);
		return 0;
	}
	m_haveError = 0;
	m_isInited = 1;

	return 1;
}

void _XLiquidBalls::move(int timeDelay)
{
	if(m_isInited == 0) return;
	if(m_isEnable == 0) return;
	if(m_haveError != 0) return;
	crashProc(timeDelay);
}

int _XLiquidBalls::checkProc()//��������Ƿ�ϸ�
{
	//������ȷ�Լ��
	for(int i = 0;i < m_liquidBallSum - 1;++ i)
	{
		if(m_pRigidBall[i].getIsEnable() != 0)
		{
			for(int j = i + 1;j < m_liquidBallSum;++ j)
			{
				if(m_pRigidBall[j].getIsEnable() != 0)
				{
					if((m_pRigidBall[i].m_position.x - m_pRigidBall[j].m_position.x) * (m_pRigidBall[i].m_position.x - m_pRigidBall[j].m_position.x) +
						(m_pRigidBall[i].m_position.y - m_pRigidBall[j].m_position.y) * (m_pRigidBall[i].m_position.y - m_pRigidBall[j].m_position.y) <
						(m_pRigidBall[i].m_radius + m_pRigidBall[j].m_radius - 2.0f) * (m_pRigidBall[i].m_radius + m_pRigidBall[j].m_radius - 2.0f) - 1.0f)
					{
						m_haveError = 1;
//						printf("haha:%d,%d--------\n",i,j);
						return 0;
					}
				}
			}
		}
	}
	for(int i = 0;i < m_liquidBallSum;++ i)
	{
		if(m_pRigidBall[i].getIsEnable() != 0)
		{
			if(m_pRigidBall[i].m_position.y >= 700.0f - m_pRigidBall[i].m_radius + 3.0f
				|| m_pRigidBall[i].m_position.y <= 50.0f + m_pRigidBall[i].m_radius - 3.0f
				|| m_pRigidBall[i].m_position.x >= 1000.0f - m_pRigidBall[i].m_radius + 3.0f
				|| m_pRigidBall[i].m_position.x <= 50.0f + m_pRigidBall[i].m_radius - 3.0f)
			{
				m_haveError = 1;
//				printf("Out:%d,+++++++++\n",i);
				return 0;
			}
			if((m_pRigidBall[i].m_position.y <= 500.0f && m_pRigidBall[i].m_position.y >= 300.0f && 
				m_pRigidBall[i].m_position.x >= 300.0f - m_pRigidBall[i].m_radius + 1.0f && m_pRigidBall[i].m_position.x <= 500.0f + m_pRigidBall[i].m_radius - 1.0f)
				|| (m_pRigidBall[i].m_position.x <= 500.0f && m_pRigidBall[i].m_position.x >= 300.0f && 
				m_pRigidBall[i].m_position.y >= 300.0f - m_pRigidBall[i].m_radius + 1.0f && m_pRigidBall[i].m_position.y <= 500.0f + m_pRigidBall[i].m_radius - 1.0f)
				/*m_pRigidBall[i].m_position.y <= 500.0f + m_pRigidBall[i].m_radius * 0.707f - 3.0f
				&& m_pRigidBall[i].m_position.y >= 300.0f - m_pRigidBall[i].m_radius * 0.707f + 3.0f
				&& m_pRigidBall[i].m_position.x <= 500.0f + m_pRigidBall[i].m_radius * 0.707f - 3.0f
				&& m_pRigidBall[i].m_position.x >= 300.0f - m_pRigidBall[i].m_radius * 0.707f + 3.0f*/)
			{
				m_haveError = 1;
//				printf("Into:%d,+++++++++\n",i);
				return 0;
			}
			if( (m_pRigidBall[i].m_position.y - 300.0f) * (m_pRigidBall[i].m_position.y - 300.0f) + (m_pRigidBall[i].m_position.x - 300.0f) * (m_pRigidBall[i].m_position.x - 300.0f) < (m_pRigidBall[i].m_radius - 3.0f)*(m_pRigidBall[i].m_radius - 3.0f) ||
				(m_pRigidBall[i].m_position.y - 300.0f) * (m_pRigidBall[i].m_position.y - 300.0f) + (m_pRigidBall[i].m_position.x - 500.0f) * (m_pRigidBall[i].m_position.x - 500.0f) < (m_pRigidBall[i].m_radius - 3.0f)*(m_pRigidBall[i].m_radius - 3.0f) ||
				(m_pRigidBall[i].m_position.y - 500.0f) * (m_pRigidBall[i].m_position.y - 500.0f) + (m_pRigidBall[i].m_position.x - 300.0f) * (m_pRigidBall[i].m_position.x - 300.0f) < (m_pRigidBall[i].m_radius - 3.0f)*(m_pRigidBall[i].m_radius - 3.0f) ||
				(m_pRigidBall[i].m_position.y - 500.0f) * (m_pRigidBall[i].m_position.y - 500.0f) + (m_pRigidBall[i].m_position.x - 500.0f) * (m_pRigidBall[i].m_position.x - 500.0f) < (m_pRigidBall[i].m_radius - 3.0f)*(m_pRigidBall[i].m_radius - 3.0f))
			{
				m_haveError = 1;
//				printf("HitPoint:%d,+++++++++\n",i);
				return 0;
			}
		}
	}
	return 1;
}

//��ʱ����������ǽ�����ײ
void _XLiquidBalls::crashProc(int timeDelay)
{
	if(m_isInited == 0) return;
	if(m_isEnable == 0) return;
	for(int i = 0;i < m_liquidBallDataSum;++ i)
	{
		m_pCrashData[i].m_needFrash = 1;
		//m_pCrashData[i].m_needFrash = 1;	//��ʼ�����е��໥�����������ݶ���Ҫ����
	}
	//for(int i = 0;i < m_liquidBallSum;++ i)
	//{
	//	m_pRigidBall[i].m_upCrashOrder = -1;
	//}
	m_deep = 0;
	crashInsideProc(timeDelay,0.0f);
	//������ȷ�Լ��
	checkProc();
}

void _XLiquidBalls::crashTimerProc(double timeDelay,double upTime)
{
	m_crashObj0 = 0;	//�����ʱ����ڷ�������ײ��������Ϊ������ײ����������ı��
	m_crashObj1 = 0;
	m_minTimer = -1.0f;
	m_crashSameTimeSum = 0;
	m_crashOrder = 0;
	int order = 0;
	for(int i = 0;i < m_liquidBallSum;++ i)
	{
		if(m_pRigidBall[i].getIsEnable() != 0)
		{//��Ч
			for(int j = i + 1;j < m_liquidBallSum + m_lineSum;++ j)
			{
				if(j < m_liquidBallSum)
				{//���������ײ
					if(m_pRigidBall[j].getIsEnable() != 0)
					{
						if(m_pCrashData[order].m_needFrash != 0)
						//if(1)
						{
							m_pCrashData[order].m_crashTime = getCrashTimerB2B(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),	//����ٶ���Ҫ����0.0001
								m_pRigidBall[j].m_positionS,m_pRigidBall[j].m_speed,_XVector2(0.0f,gravity),
								m_pRigidBall[i].m_radius + m_pRigidBall[j].m_radius,timeDelay);
							m_pCrashData[order].m_needFrash = 0;
							if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
							{//��Ԫͬʱ��ײ������
								m_crashSameTimeSum++;
							}
							if(m_pCrashData[order].m_crashTime >= 0)
							{//�ж��Ƿ�Ϊ��Сֵ
								//������ʱ�Ȳ������Ż���֮���ڽ����ⲿ�ֹ���
								m_pCrashData[order].m_crashAngle = m_pRigidBall[i].m_positionS.getAngle(m_pRigidBall[j].m_positionS);
								if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
								{
									m_minTimer = m_pCrashData[order].m_crashTime;	//��Сֵ
									m_crashObj0 = i;
									m_crashObj1 = j;
									m_crashOrder = order;
								}
							}
						}else
						{
							if(m_pCrashData[order].m_crashTime >= 0)
							{
								m_pCrashData[order].m_crashTime -= upTime;
								if(m_pCrashData[order].m_crashTime < 0) m_pCrashData[order].m_crashTime = 0;
								if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
								{//��Ԫͬʱ��ײ������
									m_crashSameTimeSum++;
								}
								if(m_pCrashData[order].m_crashTime >= 0)
								{//�ж��Ƿ�Ϊ��Сֵ
									if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
									{
										m_minTimer = m_pCrashData[order].m_crashTime;	//��Сֵ
										m_crashObj0 = i;
										m_crashObj1 = j;
										m_crashOrder = order;
									}
								}
							}
						}
					}else
					{
						m_pCrashData[order].m_crashTime = -1.0; 
					}
					++ order;
				}else
				{//���߶ε���ײ
					if(m_pLine[j - m_liquidBallSum].getIsEnable() != 0)
					{
						//if(m_pCrashData[order].m_needFrash != 0)
						if(1)
						{//��Ҫ������ײ�ж�
							//double tempTime;
							//�߶α���
							m_pCrashData[order].m_crashTime = getCrashTimerB2L(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),
								m_pLine[j - m_liquidBallSum],m_pRigidBall[i].m_radius,timeDelay);
							//m_pCrashData[order].m_crashTime = getCrashTimerB2L(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),
							//	m_pLine[j - m_liquidBallSum].getStartPosition(),m_pLine[j - m_liquidBallSum].getEndPosition(),m_pRigidBall[i].m_radius,timeDelay);

							//tempTime = getCrashTimerB2L(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),
							//	&(m_pLine[j - m_liquidBallSum]),m_pRigidBall[i].m_radius,timeDelay);
							//if(m_pCrashData[order].m_needFrash == 0 && tempTime >= 0 && m_pCrashData[order].m_crashTime < 0)
							//{
							//	printf("Error!\n");
							//}
							//m_pCrashData[order].m_crashTime = tempTime;

							m_pCrashData[order].m_needFrash = 0;
							if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
							{//��Ԫͬʱ��ײ������
								m_crashSameTimeSum++;
							}
							if(m_pCrashData[order].m_crashTime >= 0 )
							{//�ж��Ƿ�Ϊ��Сֵ
								m_pCrashData[order].m_crashAngle = m_pLine[j - m_liquidBallSum].getLineAngle();
								if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
								{
									m_minTimer = m_pCrashData[order].m_crashTime;	//��Сֵ
									m_crashObj0 = i;
									m_crashObj1 = m_liquidBallSum + (j - m_liquidBallSum) * 3 + 0;
									m_crashOrder = order;
								}
							}
							++order;
							//�߶ζ˵�
							m_pCrashData[order].m_crashTime = getCrashTimerB2P(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),
								m_pLine[j - m_liquidBallSum].getStartPosition(),m_pRigidBall[i].m_radius,timeDelay);

							//tempTime = getCrashTimerB2P(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),
							//	m_pLine[j - m_liquidBallSum].getStartPosition(),m_pRigidBall[i].m_radius,timeDelay);
							//if(m_pCrashData[order].m_needFrash == 0 && tempTime >= 0 && m_pCrashData[order].m_crashTime < 0)
							//{
							//	printf("Error!\n");
							//}
							//m_pCrashData[order].m_crashTime = tempTime;

							m_pCrashData[order - 1].m_needFrash = 0;
							if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
							{//��Ԫͬʱ��ײ������
								m_crashSameTimeSum++;
							}
							if(m_pCrashData[order].m_crashTime >= 0 )
							{//�ж��Ƿ�Ϊ��Сֵ
								m_pCrashData[order].m_crashAngle = m_pRigidBall[i].m_positionS.getAngle(m_pLine[j - m_liquidBallSum].getStartPosition());
								if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
								{
									m_minTimer = m_pCrashData[order].m_crashTime;	//��Сֵ
									m_crashObj0 = i;
									m_crashObj1 = m_liquidBallSum + (j - m_liquidBallSum) * 3 + 1;
									m_crashOrder = order;
								}
							}
							++order;
							//�߶ζ˵�
							m_pCrashData[order].m_crashTime = getCrashTimerB2P(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),
								m_pLine[j - m_liquidBallSum].getEndPosition(),m_pRigidBall[i].m_radius,timeDelay);

							//tempTime = getCrashTimerB2P(m_pRigidBall[i].m_positionS,m_pRigidBall[i].m_speed,_XVector2(0.0f,gravity),
							//	m_pLine[j - m_liquidBallSum].getEndPosition(),m_pRigidBall[i].m_radius,timeDelay);
							//if(m_pCrashData[order].m_needFrash == 0 && tempTime >= 0 && m_pCrashData[order].m_crashTime < 0)
							//{
							//	printf("Error!\n");
							//}
							//m_pCrashData[order].m_crashTime = tempTime;

							m_pCrashData[order - 2].m_needFrash = 0;
							if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
							{//��Ԫͬʱ��ײ������
								m_crashSameTimeSum++;
							}
							if(m_pCrashData[order].m_crashTime >= 0 )
							{//�ж��Ƿ�Ϊ��Сֵ
								m_pCrashData[order].m_crashAngle = m_pRigidBall[i].m_positionS.getAngle(m_pLine[j - m_liquidBallSum].getEndPosition());
								if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
								{
									m_minTimer = m_pCrashData[order].m_crashTime;	//��Сֵ
									m_crashObj0 = i;
									m_crashObj1 = m_liquidBallSum + (j - m_liquidBallSum) * 3 + 2;
									m_crashOrder = order;
								}
							}
							++order;
						}else
						{	//�߶α���
							if(m_pCrashData[order].m_crashTime >= 0)
							{
								m_pCrashData[order].m_crashTime -= upTime;
								if(m_pCrashData[order].m_crashTime < 0) m_pCrashData[order].m_crashTime = 0;
								if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
								{//��Ԫͬʱ��ײ������
									m_crashSameTimeSum++;
								}
								if(m_pCrashData[order].m_crashTime >= 0 )
								{//�ж��Ƿ�Ϊ��Сֵ
									if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
									{
										m_minTimer = m_pCrashData[order].m_crashTime;	//��Сֵ
										m_crashObj0 = i;
										m_crashObj1 = m_liquidBallSum + (j - m_liquidBallSum) * 3 + 0;
										m_crashOrder = order;
									}
								}
							}
							++order;
							//�߶ζ˵�
							if(m_pCrashData[order].m_crashTime >= 0)
							{
								m_pCrashData[order].m_crashTime -= upTime;
								if(m_pCrashData[order].m_crashTime < 0) m_pCrashData[order].m_crashTime = 0;
								if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
								{//��Ԫͬʱ��ײ������
									m_crashSameTimeSum++;
								}
								if(m_pCrashData[order].m_crashTime >= 0 )
								{//�ж��Ƿ�Ϊ��Сֵ
									if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
									{
										m_minTimer = m_pCrashData[order].m_crashTime;	//��Сֵ
										m_crashObj0 = i;
										m_crashObj1 = m_liquidBallSum + (j - m_liquidBallSum) * 3 + 1;
										m_crashOrder = order;
									}
								}
							}
							++order;
							//�߶ζ˵�
							if(m_pCrashData[order].m_crashTime >= 0)
							{
								m_pCrashData[order].m_crashTime -= upTime;
								if(m_pCrashData[order].m_crashTime < 0) m_pCrashData[order].m_crashTime = 0;
								if(m_pCrashData[order].m_crashTime >= 0 && m_minTimer > 0 && abs(m_pCrashData[order].m_crashTime - m_minTimer) <= 0.0001f)
								{//��Ԫͬʱ��ײ������
									m_crashSameTimeSum++;
								}
								if(m_pCrashData[order].m_crashTime >= 0 )
								{//�ж��Ƿ�Ϊ��Сֵ
									if(m_pCrashData[order].m_crashTime < m_minTimer || m_minTimer < 0)
									{
										m_minTimer = m_pCrashData[order].m_crashTime;	//��Сֵ
										m_crashObj0 = i;
										m_crashObj1 = m_liquidBallSum + (j - m_liquidBallSum) * 3 + 2;
										m_crashOrder = order;
									}
								}
							}
							++order;
						}
					}else
					{
						m_pCrashData[order].m_crashTime = -1.0;	//�߶α���
						++order;
						m_pCrashData[order].m_crashTime = -1.0;	//�߶ζ˵�
						++order;
						m_pCrashData[order].m_crashTime = -1.0;	//�߶ζ˵�
						++order;
					}
				}
			}
		}else
		{//��Ч
			for(int j = i + 1;j < m_liquidBallSum + m_lineSum;++ j)
			{
				if(j < m_liquidBallSum)
				{
					m_pCrashData[order].m_crashTime = -1.0; 
					++ order;
				}else
				{
					m_pCrashData[order].m_crashTime = -1.0;	//�߶α���
					++order;
					m_pCrashData[order].m_crashTime = -1.0;	//�߶ζ˵�
					++order;
					m_pCrashData[order].m_crashTime = -1.0;	//�߶ζ˵�
					++order;
				}
			}
		}
	}
	if(order > m_liquidBallDataSum)
	{
		m_haveError = 1;
//		printf("Error!\n");
	}
}

int _XLiquidBalls::crashInsideProc(double timeDelay,double upTime)
{
	m_deep ++;
	if(m_deep >= 512) 
	{
		m_haveError = 1;
//		printf("Deep Too large!\n");
		return 1;	//��ֹ������ȹ���
	}
	//��ʼ��һЩ��Ϣ
	//�������еĵ�
	for(int i = 0;i < m_liquidBallSum;++ i)
	{
		if(m_pRigidBall[i].getIsEnable() != 0)
		{
			m_pRigidBall[i].m_positionS = m_pRigidBall[i].m_position;
		}
	}
	//������ײʱ��
	crashTimerProc(timeDelay,upTime);

	//�������״̬����
	if(m_minTimer >= 0)
	{
//		printf("Crash in same time:%d\n",m_crashSameTimeSum);
	//	if(m_crashSameTimeSum > 1)
	//	{
	//		m_haveError = 1;
	//		printf("same time Crash too more!\n");
	//	}
//		printf("dx:%f,dy:%f,px:%f,py:%f\n",m_pRigidBall[m_crashObj0].m_speed.x,m_pRigidBall[m_crashObj0].m_speed.y,
//			m_pRigidBall[m_crashObj0].m_position.x,m_pRigidBall[m_crashObj0].m_position.y);
		for(int i = 0;i < m_liquidBallSum;++ i)
		{
			if(m_pRigidBall[i].getIsEnable() != 0)
			{
				m_pRigidBall[i].m_positionS.set(m_pRigidBall[i].m_position.x + m_pRigidBall[i].m_speed.x * m_minTimer * 0.001f,
					m_pRigidBall[i].m_position.y + m_pRigidBall[i].m_speed.y * m_minTimer * 0.001f + 0.5f * gravity * (m_minTimer * 0.001f) * (m_minTimer * 0.001f));
				m_pRigidBall[i].m_position = m_pRigidBall[i].m_positionS;
				//�����������
				//��������
				{
					m_pRigidBall[i].m_speed.y += gravity * m_minTimer * 0.001f;
				}
			}
		}
		//���������ײ����
		{//��Ҫ���ٶȵı仯����
			//������j,i,��صĶ���Ҫ���±�ǣ�����(�ⲿ���Ż������������߼���û�����⣬����������Ǹ����������������غϻ��߼���Ҫ�غϵ��������ڽ��������ɵ����غ������Ҫ��һ������)
			int tempBasic = 0;
			if(m_crashObj1 < m_liquidBallSum)
			{//�����˶��������ײ
				for(int i = 0;i <= m_crashObj1;++ i)
				{
					if(m_pRigidBall[i].getIsEnable() != 0)
					{//��Ч
						for(int j = i + 1;j < m_liquidBallSum + m_lineSum;++ j)
						{
							if(i == m_crashObj0 || j == m_crashObj1 
								|| i == m_crashObj1 || j == m_crashObj0) m_pCrashData[tempBasic].m_needFrash = 1;
							if(j >= m_liquidBallSum) tempBasic += 3;
							else tempBasic ++;
						}
					}else
					{
						tempBasic ++;
					}
				}
			}else
			{//�˶�������̶��߶ε���ײ
				for(int i = 0;i < m_liquidBallSum;++ i)
				{
					if(m_pRigidBall[i].getIsEnable() != 0)
					{//��Ч
						for(int j = i + 1;j < m_liquidBallSum + m_lineSum;++ j)
						{
							if(i == m_crashObj0 || j == m_crashObj0) m_pCrashData[tempBasic].m_needFrash = 1;
							if(j >= m_liquidBallSum) tempBasic += 3;
							else tempBasic ++;
						}
					}else
					{
						tempBasic ++;
					}
				}
			}
			//{
			//	printf("%d,%d:%d\n",m_crashObj0,m_crashObj1,m_liquidBallDataSum);
			//	tempBasic = 0;
			//	for(int i = 0;i < m_liquidBallSum;++ i)
			//	{
			//		for(int j = 0;j < i;j++)
			//		{
			//			printf(" ");
			//		}
			//		for(int j = i + 1;j < m_liquidBallSum + m_lineSum;++ j)
			//		{
			//			if(j >= m_liquidBallSum) 
			//			{
			//				printf("%d",m_pCrashData[tempBasic].m_needFrash);
			//				tempBasic ++;
			//				printf("%d",m_pCrashData[tempBasic].m_needFrash);
			//				tempBasic ++;
			//				printf("%d",m_pCrashData[tempBasic].m_needFrash);
			//				tempBasic ++;
			//			}else
			//			{
			//				printf("%d",m_pCrashData[tempBasic].m_needFrash);
			//				tempBasic ++;
			//			}
			//		}
			//		printf("\n");
			//	}
			//	m_haveError = 1;
			//}
			//m_pRigidBall[m_crashObj0].m_upCrashOrder = m_crashObj1;	//����������巢������ײ
			//���潫��������������
			if(m_crashObj1 < m_liquidBallSum)
			{//Ŀ��Ϊһ�����
				//����ٶ�
				float tempAngle = m_pCrashData[m_crashOrder].m_crashAngle;//��ײ�Ƕ�
				float tempSin = sin(tempAngle);
				float tempCos = cos(tempAngle);
				float speedx1 = m_pRigidBall[m_crashObj0].m_speed.x;
				float speedy1 = m_pRigidBall[m_crashObj0].m_speed.y;
				float dx1 = speedx1 * tempCos + speedy1 * tempSin;
				float dy1 = speedy1 * tempCos - speedx1 * tempSin;
				float speedx2 = m_pRigidBall[m_crashObj1].m_speed.x;
				float speedy2 = m_pRigidBall[m_crashObj1].m_speed.y;
				float dx2 = speedx2 * tempCos + speedy2 * tempSin;
				float dy2 = speedy2 * tempCos - speedx2 * tempSin;
				//����Գ�����ĺ�ֵ
				float Rx1 = (dx1 * (m_pRigidBall[m_crashObj0].m_mass - m_pRigidBall[m_crashObj1].m_mass) + 2.0f * m_pRigidBall[m_crashObj1].m_mass * dx2) / (m_pRigidBall[m_crashObj0].m_mass + m_pRigidBall[m_crashObj1].m_mass);
				float Rx2 = (dx2 * (m_pRigidBall[m_crashObj1].m_mass - m_pRigidBall[m_crashObj0].m_mass) + 2.0f * m_pRigidBall[m_crashObj0].m_mass * dx1) / (m_pRigidBall[m_crashObj1].m_mass + m_pRigidBall[m_crashObj0].m_mass);
				dx1 = Rx1;
				dx2 = Rx2;
				//�ٴν�������ϵ��ת
				tempSin = sin(-tempAngle);
				tempCos = cos(-tempAngle);
				speedx1 = dx1 * tempCos + dy1 * tempSin;
				speedy1 = dy1 * tempCos - dx1 * tempSin;
				speedx2 = dx2 * tempCos + dy2 * tempSin;
				speedy2 = dy2 * tempCos - dx2 * tempSin;
				//��ԭֵ
				m_pRigidBall[m_crashObj0].m_speed.x = speedx1 * CRASH_SPEED_DOWN;
				m_pRigidBall[m_crashObj0].m_speed.y = speedy1 * CRASH_SPEED_DOWN;
				m_pRigidBall[m_crashObj1].m_speed.x = speedx2 * CRASH_SPEED_DOWN;
				m_pRigidBall[m_crashObj1].m_speed.y = speedy2 * CRASH_SPEED_DOWN;
				//��ӡ������
//				printf("Crash!D:%d,%d,%d,A:%3.2f\n",m_deep,m_crashObj0,m_crashObj1,tempAngle /PI * 180.0f);
			}else
			{//Ŀ��Ϊ�߶�
				//��ӡ������
//				printf("Crash!D:%d,%d,%d -->\n",m_deep,m_crashObj0,m_crashObj1,m_pRigidBall[m_crashObj0].m_speed.x,m_pRigidBall[m_crashObj0].m_speed.y);
				if((m_crashObj1 - m_liquidBallSum) % 3 == 0)
				{//�߶α���
					//float tempAngle = m_pLine[(m_crashObj1 - m_liquidBallSum) / 3].getLineAngle();//��ײ�Ƕ�
					//float tempSin = sin(tempAngle);
					//float tempCos = cos(tempAngle);
					//float speedx1 = m_pRigidBall[m_crashObj0].m_speed.x;
					//float speedy1 = m_pRigidBall[m_crashObj0].m_speed.y;
					//float dx1 = speedx1 * tempCos + speedy1 * tempSin;
					//float dy1 = -speedy1 * tempCos + speedx1 * tempSin;
					////�ٴν�������ϵ��ת
					//tempSin = sin(-tempAngle);
					//tempCos = cos(-tempAngle);
					//speedx1 = dx1 * tempCos + dy1 * tempSin;
					//speedy1 = dy1 * tempCos - dx1 * tempSin;
					////��ԭֵ
					//m_pRigidBall[m_crashObj0].m_speed.x = speedx1 * CRASH_SPEED_DOWN;
					//m_pRigidBall[m_crashObj0].m_speed.y = speedy1 * CRASH_SPEED_DOWN;
					//������㷨�����Ż�
					float tempAngle = m_pCrashData[m_crashOrder].m_crashAngle;//��ײ�Ƕ�
					float tempSin = sin(2.0f * tempAngle);
					float tempCos = cos(2.0f * tempAngle);
					float speedx1 = m_pRigidBall[m_crashObj0].m_speed.y * tempSin + m_pRigidBall[m_crashObj0].m_speed.x * tempCos;
					float speedy1 = -m_pRigidBall[m_crashObj0].m_speed.y * tempCos + m_pRigidBall[m_crashObj0].m_speed.x * tempSin;
					//��ԭֵ
					m_pRigidBall[m_crashObj0].m_speed.x = speedx1 * CRASH_SPEED_DOWN;
					m_pRigidBall[m_crashObj0].m_speed.y = speedy1 * CRASH_SPEED_DOWN;
				}else
				if((m_crashObj1 - m_liquidBallSum) % 3 == 1)
				{//�߶����
					//float tempAngle = m_pRigidBall[m_crashObj0].m_positionS.getAngle(m_pLine[(m_crashObj1 - m_liquidBallSum) / 3].getStartPosition());//��ײ�Ƕ�
					//float tempSin = sin(tempAngle);
					//float tempCos = cos(tempAngle);
					//float speedx1 = m_pRigidBall[m_crashObj0].m_speed.x;
					//float speedy1 = m_pRigidBall[m_crashObj0].m_speed.y;
					//float dx1 = -speedx1 * tempCos - speedy1 * tempSin;
					//float dy1 = speedy1 * tempCos - speedx1 * tempSin;
					////�ٴν�������ϵ��ת
					//tempSin = sin(-tempAngle);
					//tempCos = cos(-tempAngle);
					//speedx1 = dx1 * tempCos + dy1 * tempSin;
					//speedy1 = dy1 * tempCos - dx1 * tempSin;
					////��ԭֵ
					//m_pRigidBall[m_crashObj0].m_speed.x = speedx1 * CRASH_SPEED_DOWN;
					//m_pRigidBall[m_crashObj0].m_speed.y = speedy1 * CRASH_SPEED_DOWN;
					//��������㷨���л���
					float tempAngle = m_pCrashData[m_crashOrder].m_crashAngle;//��ײ�Ƕ�
					float tempSin = sin(2.0f * tempAngle);
					float tempCos = cos(2.0f * tempAngle);
					float speedx1 = -m_pRigidBall[m_crashObj0].m_speed.y * tempSin - m_pRigidBall[m_crashObj0].m_speed.x * tempCos;
					float speedy1 = m_pRigidBall[m_crashObj0].m_speed.y * tempCos - m_pRigidBall[m_crashObj0].m_speed.x * tempSin;
					m_pRigidBall[m_crashObj0].m_speed.x = speedx1 * CRASH_SPEED_DOWN;
					m_pRigidBall[m_crashObj0].m_speed.y = speedy1 * CRASH_SPEED_DOWN;
				}else
				if((m_crashObj1 - m_liquidBallSum) % 3 == 2)
				{//�߶��յ�
					//float tempAngle = m_pRigidBall[m_crashObj0].m_positionS.getAngle(m_pLine[(m_crashObj1 - m_liquidBallSum) / 3].getEndPosition());//��ײ�Ƕ�
					//float tempSin = sin(tempAngle);
					//float tempCos = cos(tempAngle);
					//float speedx1 = m_pRigidBall[m_crashObj0].m_speed.x;
					//float speedy1 = m_pRigidBall[m_crashObj0].m_speed.y;
					//float dx1 = -speedx1 * tempCos - speedy1 * tempSin;
					//float dy1 = speedy1 * tempCos - speedx1 * tempSin;
					////�ٴν�������ϵ��ת
					//tempSin = sin(-tempAngle);
					//tempCos = cos(-tempAngle);
					//speedx1 = dx1 * tempCos + dy1 * tempSin;
					//speedy1 = dy1 * tempCos - dx1 * tempSin;
					////��ԭֵ
					//m_pRigidBall[m_crashObj0].m_speed.x = speedx1 * CRASH_SPEED_DOWN;
					//m_pRigidBall[m_crashObj0].m_speed.y = speedy1 * CRASH_SPEED_DOWN;
					//��������㷨���л���
					float tempAngle = m_pCrashData[m_crashOrder].m_crashAngle;//��ײ�Ƕ�
					float tempSin = sin(2.0f * tempAngle);
					float tempCos = cos(2.0f * tempAngle);
					float speedx1 = -m_pRigidBall[m_crashObj0].m_speed.y * tempSin - m_pRigidBall[m_crashObj0].m_speed.x * tempCos;
					float speedy1 = m_pRigidBall[m_crashObj0].m_speed.y * tempCos - m_pRigidBall[m_crashObj0].m_speed.x * tempSin;
					m_pRigidBall[m_crashObj0].m_speed.x = speedx1 * CRASH_SPEED_DOWN;
					m_pRigidBall[m_crashObj0].m_speed.y = speedy1 * CRASH_SPEED_DOWN;
				}
			}
		}
//		printf("Crash time:%f,dx:%f,dy:%f,px:%f,py:%f,r:%f\n",m_minTimer,m_pRigidBall[m_crashObj0].m_speed.x,m_pRigidBall[m_crashObj0].m_speed.y,
//			m_pRigidBall[m_crashObj0].m_position.x,m_pRigidBall[m_crashObj0].m_position.y,m_pRigidBall[m_crashObj0].m_radius);
		if(timeDelay - m_minTimer <= 0) return 1;
		return crashInsideProc(timeDelay - m_minTimer,m_minTimer);
	}else
	{
		for(int i = 0;i < m_liquidBallSum;++ i)
		{
			
			if(m_pRigidBall[i].getIsEnable() != 0)
			{
				m_pRigidBall[i].m_positionS.set(m_pRigidBall[i].m_position.x + m_pRigidBall[i].m_speed.x * timeDelay * 0.001f,
					m_pRigidBall[i].m_position.y + m_pRigidBall[i].m_speed.y * timeDelay * 0.001f + 0.5f * gravity * (timeDelay * 0.001f) * (timeDelay * 0.001f));
				m_pRigidBall[i].m_position = m_pRigidBall[i].m_positionS;
				//�����������
				//��������
				{
					m_pRigidBall[i].m_speed.y += gravity * timeDelay * 0.001f;
				}
			}
		}
		//if(checkProc() != 1)
		//{//���������ݷ������⣬����������ݻ�ԭ
		//	for(int i = 0;i < m_liquidBallSum;++ i)
		//	{
		//		if(m_pRigidBall[i].getIsEnable() != 0)
		//		{
		//			//�����������
		//			{
		//				//float dx = abs(m_pRigidBall[i].m_speed.x * timeDelay * 0.001f);
		//				//float tempDx = m_pRigidBall[i].m_speed.x * m_pRigidBall[i].m_speed.x - abs(2.0f * m_pRigidBall[i].m_airResistance * m_pRigidBall[i].m_speed.x * dx) / (m_pRigidBall[i].m_mass * 0.001f);
		//				//if(tempDx <= 0) 
		//				//{
		//				//	m_pRigidBall[i].m_speed.x = 0.0f;
		//				//}else
		//				//{
		//				//	if(m_pRigidBall[i].m_speed.x < 0) m_pRigidBall[i].m_speed.x = -sqrt(tempDx);
		//				//	else m_pRigidBall[i].m_speed.x = sqrt(tempDx);
		//				//}
		//				//float dy = abs(m_pRigidBall[i].m_speed.y * timeDelay * 0.001f);
		//				//float tempDy = m_pRigidBall[i].m_speed.y * m_pRigidBall[i].m_speed.y - abs(2.0f * m_pRigidBall[i].m_airResistance * m_pRigidBall[i].m_speed.y * dy) / (m_pRigidBall[i].m_mass * 0.001f);
		//				//if(tempDy < 0) m_pRigidBall[i].m_speed.y = 0.0f;
		//				//else
		//				//{
		//				//	if(m_pRigidBall[i].m_speed.y < 0) m_pRigidBall[i].m_speed.y = -sqrt(tempDy);
		//				//	else m_pRigidBall[i].m_speed.y = sqrt(tempDy);
		//				//}
		//			}
		//			//��������
		//			{
		//				m_pRigidBall[i].m_speed.y -= gravity * timeDelay * 0.001f;
		//				//float dy = abs(m_pRigidBall[i].m_speed.y * timeDelay * 0.001f);
		//				//float tempDy = m_pRigidBall[i].m_speed.y * m_pRigidBall[i].m_speed.y - 2.0f * gravity * dy;
		//				//if(tempDy < 0) m_pRigidBall[i].m_speed.y = 0.0f;
		//				//else
		//				//{
		//				//	if(m_pRigidBall[i].m_speed.y < 0) m_pRigidBall[i].m_speed.y = -sqrt(tempDy);
		//				//	else m_pRigidBall[i].m_speed.y = sqrt(tempDy);
		//				//}
		//			}
		//			m_pRigidBall[i].m_positionS.set(m_pRigidBall[i].m_position.x - m_pRigidBall[i].m_speed.x * timeDelay * 0.001f,
		//				m_pRigidBall[i].m_position.y - m_pRigidBall[i].m_speed.y * timeDelay * 0.001f - 0.5f * gravity * (timeDelay * 0.001f) * (timeDelay * 0.001f));
		//			m_pRigidBall[i].m_position = m_pRigidBall[i].m_positionS;
		//		}
		//	}
		//	//������ʾ�������
		//	float m_minTimer = -1.0f;
		//	crashTimerProc(timeDelay,upTime);
		//}
		return 1;	//��ɼ���
	}
}

int _XLiquidBalls::addOneBall(const _XVector2& position,float radius,float mass,_XVector2 speed,float airResistance,float adhereRate,float adhereRadius)	//����һ�����ӣ����������Ƿ�ɹ�
{
	if(m_isInited == 0) return 0;
	if(m_isEnable == 0) return 0;
	if(radius < 4.0f) radius = 4.0f;
	for(int i = 0;i < m_liquidBallSum;++ i)
	{
		if(m_pRigidBall[i].getIsEnable() == 0)
		{
			m_pRigidBall[i].init(position,radius,mass,speed,airResistance,adhereRate,adhereRadius);
			m_pRigidBall[i].setEnable(1);
			return 1;
		}
	}
	return 0;
}

int _XLiquidBalls::addOneLine(const _XVector2& startPosition,_XVector2 endPosition)
{
	if(m_isInited == 0) return 0;
	if(m_isEnable == 0) return 0;
	for(int i = 0;i < m_lineSum;++ i)
	{
		if(m_pLine[i].getIsEnable() == 0)
		{
			m_pLine[i].set(startPosition,endPosition);
			m_pLine[i].setEnable(1);
			return 1;
		}
	}
	return 0;
}

_XLiquidBalls::_XLiquidBalls()
:m_isEnable(0)
,m_isInited(0)
,m_liquidBallSum(0)
//,m_pRigidBall(NULL)
//,m_pLiquidBallData(NULL)
,m_pCrashData(NULL)
{
}
