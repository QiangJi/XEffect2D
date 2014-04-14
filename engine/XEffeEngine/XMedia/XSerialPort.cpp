#include "XSerialPort.h"
#include <tchar.h>
//++++++++++++++++++++++++++++++++
//Author:	  ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2013.4.9
//--------------------------------
// ��ʹ�ö���ʱ
#define TIMEOUT_READ_INTERVAL			0xFFFFFFFF
#define TIMEOUT_READ_TOTAL_MULTIPLIER	0
#define TIMEOUT_READ_TOTAL_CONSTANT		1000
// д��ʱΪ��
#define TIMEOUT_WRITE_TOTAL_MULTIPLIER	0
#define TIMEOUT_WRITE_TOTAL_CONSTANT	5000
// �Ƽ�������/�����������ע�⣺ʵ��ֵ��ϵͳ���ã�
#define BUFFER_INPUT_RECOMMEND	10000
#define BUFFER_OUTPUT_RECOMMEND	10000
// �첽��ȡ/д�����ʱ�ȴ��¼��ĳ�ʱʱ��
#define TIMEOUT_READCOMM_EVENT	4000
#define TIMEOUT_WRITECOMM_EVENT	2000
// һЩͨѶЭ��ʹ�õĺ�
#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

// �򿪴���
_XBool _XSerialPort::open(int nPort,int nBaud,int nParity,_XSerialPortMode mode)
{
    if(m_bOpened) return XTrue;	//��ֹ�ظ���

    TCHAR szPort[50];
    TCHAR szComParams[50];
    DCB dcb;
	m_mode = mode;

	wsprintf(szPort,_T("COM%d"),nPort);
	// API�������ļ���Windows�н������豸�����ļ��Դ�
	m_hIDComDev = CreateFile(
					  szPort,
					  GENERIC_READ | GENERIC_WRITE,
					  0,
					  NULL,
					  OPEN_EXISTING,
					  FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	// �첽��д
					  NULL);

	if(m_hIDComDev == NULL) return XFalse;

	memset(&m_overlappedRead,0,sizeof(OVERLAPPED));
	memset(&m_overlappedWrite,0,sizeof(OVERLAPPED));

	//���ó�ʱ
	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout		= TIMEOUT_READ_INTERVAL;
	CommTimeOuts.ReadTotalTimeoutMultiplier = TIMEOUT_READ_TOTAL_MULTIPLIER;
	CommTimeOuts.ReadTotalTimeoutConstant	= TIMEOUT_READ_TOTAL_CONSTANT;
	CommTimeOuts.WriteTotalTimeoutMultiplier= TIMEOUT_WRITE_TOTAL_MULTIPLIER;
	CommTimeOuts.WriteTotalTimeoutConstant	= TIMEOUT_WRITE_TOTAL_CONSTANT;

	SetCommTimeouts(m_hIDComDev,&CommTimeOuts);

	wsprintf(szComParams,_T("COM%d:%d,n,8,1" ),nPort,nBaud);
	//�����첽��ȡ/д������¼�
	m_overlappedRead.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	m_overlappedWrite.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	//��ȡ/���ô����豸����
	dcb.DCBlength = sizeof ( DCB );
	GetCommState ( m_hIDComDev, &dcb );
	dcb.BaudRate = nBaud;
	dcb.ByteSize = 8;			// 8 Bits ����
	dcb.StopBits = ONESTOPBIT;	// һλֹͣλ
	dcb.Parity = nParity;				//ȱʡ��У��
//	unsigned char ucSet;
//	ucSet = (unsigned char)((FC_RTSCTS & FC_DTRDSR) != 0);
//	ucSet = (unsigned char)((FC_RTSCTS & FC_RTSCTS) != 0);
//	ucSet = (unsigned char)((FC_RTSCTS & FC_XONXOFF) != 0);

	if(!SetCommState(m_hIDComDev,&dcb) ||
		!SetupComm(m_hIDComDev,BUFFER_INPUT_RECOMMEND,BUFFER_OUTPUT_RECOMMEND) ||
		m_overlappedRead.hEvent == NULL ||
		m_overlappedWrite.hEvent == NULL) 
	{
		//DWORD dwError = GetLastError();
		if(m_overlappedRead.hEvent != NULL) CloseHandle(m_overlappedRead.hEvent);
		if(m_overlappedWrite.hEvent != NULL) CloseHandle(m_overlappedWrite.hEvent);

		CloseHandle(m_hIDComDev);
		return XFalse;
	}
	if(m_mode == SP_MODE_AUTO) //�Զ�ģʽ��ʱ�����߳�
	{
		m_threadState = STATE_BEFORE_START;
		if(CreateThread(0,0,recvThread,this,0,&m_recvThread) == 0)
		{//�߳̽���ʧ��
			printf("Server command thread create error!\n");
			return XFalse;
		}
	}

    m_bOpened = XTrue;
    return m_bOpened;
}
DWORD WINAPI _XSerialPort::recvThread(void * pParam)
{
	_XSerialPort * pPar = (_XSerialPort *)pParam;
	DWORD dwEvtMask = 0,dwError,nBytesRead;
	pPar->m_threadState = STATE_START;
	char buff[1024];
	COMSTAT cs;
	if(!SetCommMask(pPar->m_hIDComDev,EV_RXCHAR)) printf("Error!\n");
	while(1)
	{
		if(pPar->m_threadState == STATE_SET_TO_END) break;
		if(WaitCommEvent(pPar->m_hIDComDev, &dwEvtMask,NULL))
		{
			ClearCommError(pPar->m_hIDComDev,&dwError,&cs);
			if((dwEvtMask & EV_RXCHAR) && cs.cbInQue)
			{
				if(ReadFile(pPar->m_hIDComDev,buff,cs.cbInQue,&nBytesRead,&pPar->m_overlappedRead))
				{
					if(pPar->m_funRecv != NULL) 
						pPar->m_funRecv(pPar->m_pClass,(unsigned char *)buff,nBytesRead);
				}
			//	PurgeComm(pPar->m_hIDComDev,PURGE_RXCLEAR);
			}
		}
		Sleep(1);
	}
	pPar->m_threadState = STATE_END;
	return 0;
}
// �رմ���
_XBool _XSerialPort::close()
{
	if(!m_bOpened || m_hIDComDev == NULL) return XTrue;
	if(m_overlappedRead.hEvent != NULL) CloseHandle(m_overlappedRead.hEvent);
	if(m_overlappedWrite.hEvent != NULL) CloseHandle(m_overlappedWrite.hEvent);
	if(m_mode == SP_MODE_AUTO) //�Զ�ģʽ��ʱ�����߳�
	{
		m_threadState = STATE_SET_TO_END;
		SetCommMask(m_hIDComDev,0);
		while(1)
		{//�ȴ��߳̽���
			if(m_threadState == STATE_END) break;
			Sleep(1);
		}
	}
	CloseHandle(m_hIDComDev);
	m_bOpened = XFalse;
	m_hIDComDev = NULL;

	return XTrue;
}
// �򴮿�д�����ݣ����ڲ�ʹ��
//_XBool _XSerialPort::writeCommByte(unsigned char ucByte)
//{
//	_XBool bWriteStat;
//	DWORD dwBytesWritten;
//
//	bWriteStat = WriteFile(m_hIDComDev,(LPSTR)&ucByte,1,&dwBytesWritten,&m_overlappedWrite);
//
//	// ��ѯ�첽д���Ƿ���ɣ�δ��������ȴ�
//	if(!bWriteStat && (GetLastError() == ERROR_IO_PENDING))
//	{
//		if(WaitForSingleObject(m_overlappedWrite.hEvent,TIMEOUT_WRITECOMM_EVENT))
//		{
//			dwBytesWritten = 0;
//		}else 
//		{
//			GetOverlappedResult(m_hIDComDev,&m_overlappedWrite,&dwBytesWritten,XFalse);
//			m_overlappedWrite.Offset += dwBytesWritten;
//		}
//	}
//
//	return XTrue;
//}
// �򴮿ڷ�������
int _XSerialPort::sendData(const unsigned char *buffer,int size)
{
	if(!m_bOpened || m_hIDComDev == NULL) return 0;

	//DWORD dwBytesWritten = 0;
	//for(int i = 0;i < size;++ i) 
	//{
	//	writeCommByte(buffer[i]);
	//	dwBytesWritten++;
	//}

	//return (int)dwBytesWritten;
	_XBool bWriteStat;
	DWORD dwBytesWritten;

	bWriteStat = WriteFile(m_hIDComDev,buffer,size,&dwBytesWritten,&m_overlappedWrite);
	// ��ѯ�첽д���Ƿ���ɣ�δ��������ȴ�
	if(!bWriteStat && (GetLastError() == ERROR_IO_PENDING))
	{
		if(WaitForSingleObject(m_overlappedWrite.hEvent,TIMEOUT_WRITECOMM_EVENT))
		{
			dwBytesWritten = 0;
		}else 
		{
			GetOverlappedResult(m_hIDComDev,&m_overlappedWrite,&dwBytesWritten,XFalse);
			m_overlappedWrite.Offset += dwBytesWritten;
		}
	}
	return dwBytesWritten;
}
// ��ѯ���ܻ��������Ƿ�������(ֻ��ѯ,����ȡ)
int _XSerialPort::readDataWaiting()
{
	if(!m_bOpened || m_hIDComDev == NULL) return 0;

	DWORD dwErrorFlags;
	COMSTAT ComStat;

	ClearCommError(m_hIDComDev,&dwErrorFlags,&ComStat);

	return (int)ComStat.cbInQue;
}
//��ȡ���Դ��ڵ�����
int _XSerialPort::readData(void *buffer,int limit)
{
    if(!m_bOpened || m_hIDComDev == NULL) return 0;

    BOOL bReadStatus;
    DWORD dwBytesRead, dwErrorFlags;
    COMSTAT ComStat;
	// ��ȡ֮ǰ�����������Ϣ
    ClearCommError(m_hIDComDev,&dwErrorFlags,&ComStat);
    if(!ComStat.cbInQue) return 0;

    dwBytesRead = (DWORD)ComStat.cbInQue;

    if(limit < (int)dwBytesRead) dwBytesRead = (DWORD)limit;

    bReadStatus = ReadFile(m_hIDComDev,buffer,dwBytesRead,&dwBytesRead,&m_overlappedRead);
	// ��ѯ�첽��ȡ�Ƿ���ɣ�δ��������ȴ�
    if(!bReadStatus)
	{
        if(GetLastError() == ERROR_IO_PENDING)
		{
            WaitForSingleObject(m_overlappedRead.hEvent,TIMEOUT_READCOMM_EVENT);
            return (int)dwBytesRead;
        }
        return 0;
    }
    return (int)dwBytesRead;
}