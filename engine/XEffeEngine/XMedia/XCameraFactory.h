#ifndef _JIA_XCAMERAFACTORY
#define _JIA_XCAMERAFACTORY
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.3.15
//--------------------------------
#include "XCameraBasic.h"
class _XCameraFactory
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	_XCameraFactory(){}
	_XCameraFactory(const _XCameraFactory&);
	_XCameraFactory &operator= (const _XCameraFactory&);
	virtual ~_XCameraFactory(){} 
public:
	static _XCameraFactory& GetInstance()
	{
		static _XCameraFactory m_instance;
		return m_instance;
	}
	//-------------------------------------------
public:
	template<typename T>
	T * create(_XCameraData & data)
	{
		T *pCamera = NULL;
		pCamera = createMem<T>();
		if(pCamera != NULL) 
		{
			if(!pCamera->init(data)) XDELETE(pCamera);//��ʼ��ʧ��
		}
		return pCamera;
	}
};

#endif //_JIA_XCAMERAFACTORY