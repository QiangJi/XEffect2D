#ifndef _JIA_XFIREWORKPARTICLES_
#define _JIA_XFIREWORKPARTICLES_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XEchoParticles.h"

class _XFireWorkParticles
{
private:
	char m_isInited;		//�Ƿ��ʼ��
	char m_isEnd;			//�Ƿ����

	_XVector2 m_position;		//λ��
	_XVector2 m_initSpeed;	//��ʼ�ٶ�
	float m_decreaseSpeed;	//�ٶ�˥����
	float m_gravity;		//�������ٶ�
	int m_atomSum;			//���ӵ�����

	_XEchoParticles *m_atom;//ԭ��
	const _XTexture *m_texture;		//���ӵ���ͼ
//	int m_w;	//��ͼ�Ŀ�
//	int m_h;	//��ͼ�ĸ�

public:
	int init(const _XVector2& position,const _XVector2& initSpeed,float decreaseSpeed,float gravity,int atomSum,const _XTexture *texture);					//��ʼ��
	void reset();				//���ö�����ʼ
	void move(int timeDelay);	//�ı�
	void draw() const;				//���
	void release();
	int getIsEnd()
	{
		if(m_isEnd != 0) return 1;
		return 0;
	}
	//�������ӷ�����ʹ�õ���ͼ(ע�������ͼ��Ҫ���ʼ��ʱ����ͼʹ����ͬ�ĳߴ�)
	void setTexture(const _XTexture *texture)
	{
		if(m_isInited == 0) return;
		if(texture == NULL || !glIsTexture(texture->m_texture)) return;
		m_texture = texture;
		for(int i = 0;i < m_atomSum;++ i)
		{
			m_atom[i].setTexture(m_texture);
		}
	}
	//�������ӷ�������λ��
	void setPosition(const _XVector2& position)
	{
		m_position = position;
	}

	_XFireWorkParticles();		//���캯��
};

#endif