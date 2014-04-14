//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.6.3
//--------------------------------
#ifndef _JIA_XTERRAIN_
#define _JIA_XTERRAIN_

#include "XBasicFun.h"
#include "XTextureInformation.h"
#include "XCam.h"
#include "XBasic3D.h"
#include "XShaderGLSL.h"

//�޸Ľ���
//1��������VBO
//2������������ģʽ

class _XWaterFlow;
//����һ������ͼ����
class _XTerrain
{
	friend _XWaterFlow;
private:
	_XBool m_isInited;				//�Ƿ�����˳�ʼ��
	float *m_terrainData;	//��ͼ����
	const float *m_newH;			//���µ�������ʱ������µĸ߶�����
	_XTextureData m_texture;		//���ε���ͼ����
	_XBool m_withTexture;				//�Ƿ�����ͼ
	int m_width;					//��ͼ�ĳߴ�
	int m_height;	

	_XVector3 m_position;	//���ε�λ��
	_XVector3 m_size;		//���εĳߴ�
	_XVector3 m_angle;		//���εļҶ�
	//float m_distance;
	float getHeight(float x,float z) const
	{
		if(x < 0 || x >= m_width) return 0.0f;
		if(z < 0 || z >= m_height) return 0.0f;
		return m_terrainData[(int)(z * m_width + x)];
	}
	_XVector2 getTexturePosition(float x,float z) const
	{
		if(x < 0) x = 0;
		if(x >= m_width) x = m_width;
		if(z < 0) z = 0;
		if(z >= m_height) z = m_height;
		//return _XVector2(x,z) / m_texture.textureSize;
		return _XVector2(x / m_width,z / m_height);
	}
	int m_allPointSum;		//�ܵĶ�����
	int m_allFaceSum;		//�ܵ�����
	float m_coordOffsetX;	//��ͼ�Ĳ���
	float m_coordOffsetY;

	_XVBO m_vbo;
public:
	float *m_v;
	float *m_n;
	float *m_t;
	float *m_c;
private:
	_XVector3 *m_faceN;		//��ķ���
	//_XVector3 *m_pointN;	//��ķ���
	unsigned int *m_index;	//����

	bool m_withTexGen;	//�Ƿ�ʹ�ý�������
	float m_reflectRate;	//��յķ���ϵ��
	int m_faceSum;		//�����������
	_XBool m_withColor;	//�Ƿ�ʹ�ò����ɫ

	_XShaderGLSL *m_pShader;	//shader��ָ��
	void releaseMemory()
	{
		if(m_pSubject == NULL)
		{
			XDELETE_ARRAY(m_v);
			XDELETE_ARRAY(m_n);
			XDELETE_ARRAY(m_faceN);
			//XDELETE_ARRAY(m_pointN);
			XDELETE_ARRAY(m_t);
			XDELETE_ARRAY(m_c);
			XDELETE_ARRAY(m_index);
		}else
		{
			if(!m_subjectV) XDELETE_ARRAY(m_v);
			if(!m_subjectN) XDELETE_ARRAY(m_n);
			if(!m_subjectC) XDELETE_ARRAY(m_c);
			if(!m_subjectT) XDELETE_ARRAY(m_t);
			if(!m_subjectI) XDELETE_ARRAY(m_index);
			m_pSubject = NULL;
		}
	}
public:
	float getReflectRate(){return m_reflectRate;}
	void setShader(_XShaderGLSL *p){m_pShader = p;}
public:
	_XBool init(const char *mapFilename,const char * texFilename,int w,int h,
		_XBool withColor = false,	//�Ƿ�ʹ����ɫ
		_XResourcePosition resourcePosition = RESOURCE_SYSTEM_DEFINE);	//���ļ��ж�ȡ��������
	_XBool init(const float *mapData,const char * texFilename,int w,int h,
		_XBool withColor = false,	//�Ƿ�ʹ����ɫ
		_XResourcePosition resourcePosition = RESOURCE_SYSTEM_DEFINE);	//ֱ�Ӵ������ж�ȡ����
	void draw(_XBool withTex = XTrue);
	void drawNormal(int step = 1);	//��淨��
	void updateTexture(const unsigned char * data)
	{
		if(data == NULL) return;
		if(!m_isInited) return;
		if(!m_withTexture) return;
		m_texture.texture.updateTexture(data);
	}
	void setTexture(unsigned int tex)	//����������ڲ���
	{
		if(!m_isInited) return;
		if(!m_withTexture) return;
		m_texture.texture.m_texture = tex;
	}
	void setTexture(const _XTextureData &tex)
	{
		if(!m_isInited) return;
		if(!m_withTexture) return;
		m_texture.setACopy(tex);
	}
private:
	void updateNormal(const float *heights);	//���·��ߵ���ֵ
public:
	//flags ���ڱ����Щ���Ƿ���Ҫ������
	//pT �����ṩ���ݣ���pT��ΪNULLʱ����ʾ���ݲ����㣬ֱ�Ӵ�pT�ж�ȡ
	_XBool updateHeight(const float *heights,const bool *flags = NULL);	//flags ���ڱ�����Ƿ���Ч
	_XBool updateByHeightAndNormal(const float *heights,const float *normal);
	_XBool updateByFlag(const bool *flags,const float * w);
	_XBool updateVBO(bool v,bool n = false,bool c = false,bool t = false,bool i = false);
	_XBool updateByStep(int step);	//ʹ�ò���step�������ͼ����ʾ��Ĭ�������step = 1;

	void setPosition(float x,float y,float z) {m_position.set(x,y,z);}
	void setPosition(const _XVector3 &pos) {m_position = pos;}
	void setSize(float x,float y,float z) {m_size.set(x,y,z);}
	void setSize(const _XVector3 &size) {m_size = size;}
	void setAngle(float x,float y,float z) {m_angle.set(x,y,z);}
	void setAngle(const _XVector3 &angle) {m_angle = angle;}

	void setWithTexGen(bool flag){m_withTexGen = flag;}
	void release()
	{
		if(!m_isInited) return;
		XDELETE_ARRAY(m_terrainData);
		XDELETE_ARRAY(m_tempFitTerrain);
		m_vbo.release();
		releaseMemory();
		m_isInited = XFalse;
	}
	void setReflectRate(float rate)
	{
		if(rate < 0.0f) rate = 0.0f;
		if(rate > 1.0f) rate = 1.0f;
		m_reflectRate = rate;
	}
	_XVector3 myNormalize(float x,float z) 
	{//�����ض��Ż��Ĺ�һ������
		float d = 1.0f / sqrt(x * x + 1.0f + z * z);
		return _XVector3(x * d,d,z * d);
	}
	bool getMousePoint(_XVector2 &outPoint);	//[��ʰȡ]���ݵ�ǰ����λ�û�ȡĿǰ�����3D������ʰȡ��λ�ã����û��ʰȡ�������򷵻�false
	bool getMousePointEx(const _XVector3 &n,const _XVector3 &f,_XVector2 &outPoint);	//[��ʰȡ]
	_XVector3 getMyNormal(int x,bool flag)	//���㷨��
	{//�����ض��Ż������ߺ���
		//_XVector3 w,v;
		if(flag) return myNormalize(m_newH[x] - m_newH[x + 1],m_newH[x] - m_newH[x + m_width]);	//�ϰ����
		else return myNormalize(m_newH[x + m_width] - m_newH[x + 1 + m_width],m_newH[x + 1] - m_newH[x + 1 + m_width]);	//�°����
	}
private:
	_XTerrain *m_pSubject;	//����Ŀ��
	bool m_subjectV;		//�����ı�־
	bool m_subjectN;
	bool m_subjectC;
	bool m_subjectT;
	bool m_subjectI;
public:
	//t:������Ŀ��
	//w,n,c,t:ӵ�е�����
	//cv,cn,cc,ct:����������
	bool setSubject(_XTerrain *T,bool cv,bool cn,bool cc,bool ct,bool ci)	//����Ϊt�Ĵ���������û�����ԣ�����ȫ����t��
	{//����û�н����ϸ������У��(���֮����Ҫ�ϸ����߼���������)
		if(T == NULL) return false;
		if(m_pSubject != NULL) return false;
		m_vbo.setAsSubject(T->m_vbo,XTrue,XTrue,m_withColor,XTrue,XTrue,cv,cn,cc,ct,ci);

		m_subjectV = cv;
		m_subjectN = cn;
		m_subjectC = cc;
		m_subjectT = ct;
		m_subjectI = ci;
		if(m_subjectV)
		{
			XDELETE_ARRAY(m_v);
			m_v = T->m_v;
		}
		if(m_subjectN)
		{
			XDELETE_ARRAY(m_n);
			m_n = T->m_n;
		}
		if(m_subjectC)
		{
			XDELETE_ARRAY(m_c);
			m_c = T->m_c;
		}
		if(m_subjectT)
		{
			XDELETE_ARRAY(m_t);
			m_t = T->m_t;
		}
		if(m_subjectI)
		{
			XDELETE_ARRAY(m_index);
			m_index = T->m_index;
		}
		m_pSubject = T;
		return true;
	}

	_XTerrain()
		:m_isInited(XFalse)
		,m_terrainData(NULL)
		,m_position(0.0f,0.0f,0.0f)
		,m_size(1.0f,1.0f,1.0f)
		,m_angle(0.0f,0.0f,0.0f)
		,m_v(NULL)
		,m_n(NULL)
		,m_faceN(NULL)
		,m_c(NULL)
		,m_withColor(false)
		//,m_pointN(NULL)
		,m_t(NULL)
		//,m_distance(1.0f)	//������Ƭ�ĳߴ�
		,m_withTexGen(false)
		,m_reflectRate(0.5f)
		,m_faceSum(0)
		,m_pShader(NULL)
		,m_index(NULL)
		,m_pSubject(NULL)
		,m_tempFitTerrain(NULL)
		,m_subjectV(false)
		,m_subjectN(false)
		,m_subjectC(false)
		,m_subjectT(false)
	{
	}
	~_XTerrain()
	{
		release();
	}
private:
	float *m_tempFitTerrain;
	int m_oInX;
	int m_oInY;
public:
	void myFitTerrain(const float *inData,int inX,int inY,float *outData);	//���������Ż���
};
//���������δ��������
extern void fitTerrain(const float *inData,int inX,int inY,float *outData,int outX,int outY);//������Ӧ,������ߴ�ĵ��ΰ���3�α�������ֵ����Ҫ�ĵ���
#endif
