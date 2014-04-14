#ifndef _JIA_XCAMERABASIC_
#define _JIA_XCAMERABASIC_
#include "XEffeEngine.h"
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.9
//--------------------------------
//����һ������ͷ��Ƶ������ȡ�Ļ��࣬�������е���Ƶ���ݲɼ����඼�̳��������

#define SUPPORT_FOR_HIK (1)	//�Ƿ�֧�ֺ���������ͷ

enum _XCameraType
{
	CAMERA_TYPE_NORMAL,	//��ͨ����ͷ
#if SUPPORT_FOR_HIK	
	CAMERA_TYPE_HIK,		//����������ͷ
#endif
	CAMERA_TYPE_AVER,		//Բ�յ�����ͷ
};
//���������ز���
struct _XCameraData
{
	int w;				//���ؿ�
	int h;				//���ظ�
	int fps;			//֡��
	int deviceOrder;	//�豸���
	_XCameraType cameraType;	//����ͷ���ͺ�

	void setNormal(int width,int height,int order = 0,int setfps = 30)
	{
		w = width;
		h = height;
		deviceOrder = order;
		fps = setfps;
		cameraType = CAMERA_TYPE_NORMAL;
	}
#if SUPPORT_FOR_HIK
	char ipAddress[16];	//��������ͷ����ַ
	char userName[64];	//�û���
	char pwd[16];		//����
	int port;			//�˿ں�

	void setHIK(int width,int height,const char *add,const char *usern,
		const char * passward,int p = 8000)
	{
		if(add == NULL || usern == NULL || passward == NULL) return;
		w = width;
		h = height;
		strcpy(ipAddress,add);
		strcpy(userName,usern);
		strcpy(pwd,passward);
		port = p;
		cameraType = CAMERA_TYPE_HIK;
	}
#endif
};

class _XCameraBasic
{
private:
protected:
	_XBool m_isInited;		//�Ƿ���г�ʼ��

	int m_cameraWidth;		//����������ؿ�
	int m_cameraHeight;		//����������ظ�
	int m_cameraTexWidth;	//��ͼ�����ؿ�
	int m_cameraTexHeight;	//��ͼ�����ظ߶�
	int m_buffSize;			//��������buff�ĳߴ�

	_XBool m_isWork;			//�����豸�Ĺ���״̬
public:
	_XTexture m_cameraTex;	//��ͼ
	int getWidth() const {return m_cameraWidth;}
	int getHeight() const {return m_cameraHeight;}
	int getTexWidth() const {return m_cameraTexWidth;}
	int getTexHeight() const {return m_cameraTexHeight;}
	GLuint getCameraTex() const {return m_cameraTex.m_texture;}
	_XSprite m_cameraSprite;//���ڻ�ͼ�ľ���

	void setWork() {m_isWork = XTrue;}
	void setStop() {m_isWork = XFalse;}
	_XBool getWorkState() const {return m_isWork;}

	virtual _XBool init(_XCameraData &data) = 0;	//��ʼ�������豸
	virtual void release() = 0;					//�ͷ��豸�����ͷ���Դ
	virtual _XBool upDateFrame() = 0;				//����֡����

	virtual int getBuffSize() const{return m_buffSize;}	//���������ݵ�buff�ߴ�
	virtual void getData(unsigned char * p) const = 0;	//������������
	virtual unsigned char * getDataP() const = 0;				//��ȡ���ݵ�ָ��

	virtual void draw()	//�����������
	{
		if(!m_isInited) return;
		m_cameraSprite.draw(&m_cameraTex.m_texture);
	}

	_XCameraBasic()
		:m_isInited(XFalse)
		,m_isWork(XTrue)
	{}
	virtual ~_XCameraBasic(){}
};

#endif