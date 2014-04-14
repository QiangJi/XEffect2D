//inline �����Ķ���
inline _XObjectType _XObjectManager::getObjectType(int objectID) const
{
//	if(!m_isInited) return OBJ_NULL;
	if(objectID < 0 || objectID >= m_nowObjectSum) return OBJ_NULL;
	return m_objectType[objectID];
}

inline void *_XObjectManager::getObject(int objectID) const
{
//	if(!m_isInited) return NULL;
	if(objectID < 0 || objectID >= m_nowObjectSum) return NULL;
	return m_pObject[objectID];
}

inline void _XObjectManager::decreaseAObject(const void * object)
{
//	if(!m_isInited) return;
	decreaseAObject(findObjectID(object));
}

inline int _XObjectManager::getNowObjectSum() const
{
//	if(!m_isInited) return 0;
	return m_nowObjectSum;
}

inline int _XObjectManager::getNowMouseOnObjectSum() const
{
//	if(!m_isInited) return 0;
	return m_nowMouseOnObjectSum;
}

inline int _XObjectManager::getNowChooseObjectSum() const
{
//	if(!m_isInited) return 0;
	int ret = 0;
	for(int i = 0;i < m_nowObjectSum;++ i)
	{
		if(m_objBeSelect[i]) ++ ret;
	}
	return ret;
}
