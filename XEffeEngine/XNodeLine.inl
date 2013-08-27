//inline ����
inline void _XNodeLine::setOneNode(const _XVector2& node,int nodeOrder)
{
	setOneNode(node.x,node.y,nodeOrder);
}

inline void _XNodeLine::addOneNode(const _XVector2& node)
{
	addOneNode(node.x,node.y);
}

inline void _XNodeLine::addOneNode(const _XVector2& node,int nodeOrder)
{
	addOneNode(node.x,node.y,nodeOrder);
}

inline int _XNodeLine::getOrder() const
{
	return m_myOrder;
}

inline void _XNodeLine::setMyOrder(int temp)
{
	if(temp < 0) return;
	if(temp >= 100000) return;
	m_myOrder = temp;
}

inline void _XNodeLine::setLeadMode(int mode)
{
	if(mode == _XLEAD_MODE_FOREWORD)
	{
		m_leadMode = _XLEAD_MODE_FOREWORD;
	}else
	if(mode == _XLEAD_MODE_BACKWORD)
	{
		m_leadMode = _XLEAD_MODE_BACKWORD;
	}
}

inline void _XNodeLine::setSpecialPointSpeed(float speed)	//�����������ߵ��ٶ�
{
	m_specialPoint.speed = speed;
}

inline int _XNodeLine::getSpecialPointIsEnd() const
{
	return m_specialPoint.isEnd;
}

inline _XVector2 _XNodeLine::getSpecialPointPosition() const		//��ȡ�����еĵ��λ��
{
	return _XVector2(m_specialPoint.position.x * m_size.x + m_offsetPosition.x,m_specialPoint.position.y * m_size.y + m_offsetPosition.y);
}

inline float _XNodeLine::getSpecialPointAngle() const
{
	return m_specialPoint.angle;
}

//inline int _XNodeLine::setSize(float size)			//�������ߵĳߴ�
//{
//	if(size <= 0) return 0;
//	m_size = size;
//	return 1;
//}

inline void _XNodeLine::setPosition(const _XVector2& position)	//�������ߵ�ƫ��λ��
{
	m_offsetPosition = position;
}

inline void _XNodeLine::setPosition(float x,float y)	//�������ߵ�ƫ��λ��
{
	m_offsetPosition.set(x,y);
}

inline void _XNodeLine::release()		//�ͷ���Դ
{
//	if(m_isACopy == 0)
//	{
//		XDELETE_ARRAY(m_node);
//	}
	m_nodeSum = 0;
}