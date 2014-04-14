#ifndef _JIA_SERIALPORT_
#define _JIA_SERIALPORT_
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.9
//--------------------------------
#include <windows.h>
#include "XOSDefine.h"
#include "XBasicClass.h"
//�������ݵĽ���ģʽ
enum _XSerialPortMode
{
	SP_MODE_MANUAL,	//��ѯģʽ
	SP_MODE_AUTO,	//�¼�ģʽ
};

class _XSerialPort
{
public:
	_XSerialPort()
		:m_hIDComDev(NULL)
		,m_bOpened(XFalse)
		,m_mode(SP_MODE_MANUAL)
		,m_funRecv(NULL)
		,m_pClass(NULL)
	{
		memset(&m_overlappedRead,0,sizeof(OVERLAPPED));
		memset(&m_overlappedWrite,0,sizeof(OVERLAPPED));
	}
	~_XSerialPort(){close();}

	_XBool open(int nPort = 1,int nBaud = 9600,int nParity = 0,_XSerialPortMode mode = SP_MODE_MANUAL);	//�򿪴����豸
	_XBool close();//�ر��豸

	int readData(void *buff, int limit);
	int sendData(const unsigned char *,int size);
	int readDataWaiting();	// ��ѯ���������Ƿ���δ��ȡ������

	_XBool isOpened() const {return m_bOpened;}
	void setCallBackFun(void (* funRecv)(void *,unsigned char *,int),void *pClass)
	{
		m_funRecv = funRecv;
		m_pClass = pClass;
	}
protected:
	//_XBool writeCommByte(unsigned char);	// �ڲ�ʵ�֣��򴮿�д����

	HANDLE m_hIDComDev;
	OVERLAPPED m_overlappedRead;
	OVERLAPPED m_overlappedWrite;
	_XBool m_bOpened;
	_XSerialPortMode m_mode;

	_XThreadState m_threadState;
	pthread_t m_recvThread;
	static DWORD WINAPI recvThread(void * pParam);		//�����߳�

	void (*m_funRecv)(void *,unsigned char *,int);	//�յ����ݵĻص�����
	void *m_pClass;
};

#endif