#ifndef _JIA_XSHADOWMAP_
#define _JIA_XSHADOWMAP_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.10.1
//--------------------------------
#include "XShaderGLSL.h"

//����һ������������Ӱ���࣬ʹ�õķ�����Shadowmap����������ٶȿ죬
//����ֻ�ܶ�����ƱȽ���Ч�����ڵ��Դ�����ã�����Ҫ�����Ե�������

#define SHADOW_MAP_TEXTURE_SIZE (1024)

class _XShadowMap
{
private:
	_XBool m_isInited;	//�Ƿ��ʼ��
	_XFBO m_fbo;	//��ʱ��ʹ��FBO
public:
	unsigned int getShadowMapTexture(){return m_fbo.getTexture(0);}
	_XBool init();
	void updateShadowMap();	//���ݲ���ˢ��shadowmap������
	//void draw();

	void release();

	_XShadowMap()
		:m_isInited(XFalse)
	{
	}
	~_XShadowMap()
	{
		release();
	}
};

#endif