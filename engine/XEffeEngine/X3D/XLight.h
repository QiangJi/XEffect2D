#ifndef _JIA_XLIGHT_
#define _JIA_XLIGHT_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
//����һ��3D�����еĵƹ�
#include "XBasic3D.h"

class _XLight
{
private:
	_XVector4 m_ambient;	//�Ƶ���ɫ
	_XVector4 m_diffuse;	//ɢ�����ɫ��
	_XVector4 m_position;	//�ƹ��λ��
	_XVector4 m_specular;	//Ŀǰ�в�֪������Ǹ�ɶ��
	int m_index;			//�ƹ�ı�� [1 - n]

	//��Ƶķ���
	_XVector3 m_lookAtPosition;		//���Ŀ����λ��
	_XVector3 m_direction;			//��Ƶķ���
	_XMatrix4x4 m_projectMatrix;	//��Ƶ��������
	_XMatrix4x4 m_viewMatrix;		//��ƵĹ۲����
	_XVector3 m_angle;
	float m_len;
	_XBool m_needCalculate;			//�Ƿ���Ҫ����
public:
	_XMatrix4x4 getProjectMatrix() {return m_projectMatrix;}
	_XMatrix4x4 getViewMatrix() {return m_viewMatrix;}
	_XLight();
	void calculate();
	void setLength(float len) 
	{
		m_len = len;
		m_needCalculate = XTrue;
	}
	void setAngle(float x,float y,float z)
	{
		m_angle.set(x,y,z);
		m_needCalculate = XTrue;
	}
	void setIndex(int index) {m_index = index;}
	void setPosition(float x,float y,float z,float w)
	{
		m_position.set(x,y,z,w);
	}
	void setPosition(const _XVector4 &p) {m_position = p;}
	_XVector3 getPosition() const {return _XVector3(m_position.x,m_position.y,m_position.z);}
	void setAmblient(float x,float y,float z,float w)
	{
		m_ambient.set(x,y,z,w);
	}
	void setAmblient(const _XVector4 &a) {m_ambient = a;}
	void setDiffuse(float x,float y,float z,float w)
	{
		m_diffuse.set(x,y,z,w);
	}
	void setDiffuse(const _XVector4 &d) {m_diffuse = d;}
	void setSpecular(float x,float y,float z,float w)
	{
		m_specular.set(x,y,z,w);
	}
	void setSpecular(const _XVector4 &s) {m_specular = s;}
	void useLight();	//ʹ�õƹ�
	void disLight();
	void disAllLight()
	{
		glDisable(GL_LIGHTING);
	}
};

#endif //_JIA_XLIGHT_