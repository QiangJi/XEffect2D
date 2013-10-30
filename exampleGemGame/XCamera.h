#ifndef _JIA_XCAMERA_
#define _JIA_XCAMERA_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2011.12.10
//--------------------------------
#include "XEffeEngine.h"
#include "videoInput.h"
#pragma comment(lib, "videoInput.lib")

#pragma comment(linker, "/NODEFAULTLIB:atlthunk.lib")
#include <atlbase.h>

#if (_ATL_VER < 0x0700)
namespace ATL
{
  inline void * __stdcall __AllocStdCallThunk()
  {
  return ::HeapAlloc(::GetProcessHeap(), 0, sizeof(_stdcallthunk));
  }

  inline void __stdcall __FreeStdCallThunk(void *p)
  {
  ::HeapFree(::GetProcessHeap(), 0, p);
  }
};
#endif //(_ATL_VER < 0x0700)

class _XCamera
{
private:
	char m_isInited;	//�Ƿ��ʼ��

	videoInput m_VI;		//����ͷ���豸ָ��
	int m_deviceSum;		//����ͷ�豸������
	int m_cameraWidth;		//����ͷ�Ŀ��
	int m_cameraHight;		//����ͷ�ĸ߶�
	int m_cameraTexW;		//����ͷ��ͼ���
	int m_cameraTexH;		//����ͷ��ͼ�߶�
	int m_buffSize;			//����ͷ���ݵĴ�С
	int m_deviceOrder;		//����ͷ�豸�ı��
	unsigned char *m_frameDataBuff;	//����ͷÿ֡������
	unsigned char *m_texDataBuff;	//��ͼ������
	int m_px;
	int m_py;
	GLuint m_cameraTex;		//����ͷ��ͼ��ָ��

	char m_isWork;	//�����豸�Ƿ���
public:
	_XSprite m_cameraSprite;	//���������ͼ����ͼλ���Լ�������Կ����޸������Ա�����

	int init(int width,int hight,int deviceOrder = 0,int FPS = 30);
	void release();
	int upDateFrame();
	void draw();
	void getData(unsigned char * p) const
	{
		if(m_isInited == 0) return;
		memcpy(p,m_frameDataBuff,m_buffSize);
	}
	int getBuffSize() const
	{
		return m_buffSize;
	}
	void setWork()
	{
		m_isWork = 1;
	}
	void setStop()
	{
		m_isWork = 0;
	}

	_XCamera()
	:m_isInited(0)
	,m_frameDataBuff(NULL)
	,m_texDataBuff(NULL)
	,m_isWork(1)
	{
	}
};

#endif