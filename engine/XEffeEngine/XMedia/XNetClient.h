#ifndef _JIA_XNETCLIENT_
#define _JIA_XNETCLIENT_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2011.7.8
//--------------------------------
#include "XNetCommon.h"
//ע�⣺��δ���Ƕ��̻߳�������
class _XNetClient
{
private:
	//WSADATA m_wsaData;
	SOCKET m_netSocket;	//����ͨѶ������ӿ�	

	_XCritical m_mutexThread;
	_XCritical m_mutexSend;	//�����߳���
	_XCritical m_mutexRecv;	//�����߳���
	_XCritical m_mutexBoardCast;	//�߳���

	_XConnectState m_connectState; 
	_XNetData * m_recvPacket;	//��δ��������������
	std::deque<_XNetData *> m_recvDataBuff;	//���յ�������
	std::deque<_XNetData *> m_sendDataBuff;	//���յ�������
	pthread_t m_recvThread;
	pthread_t m_sendThread;
	pthread_t m_boardcastThread;
	static DWORD WINAPI recvThread(void * pParam);	//�����߳�
	static DWORD WINAPI sendThread(void * pParam);	//�����߳�
	static DWORD WINAPI boardcastThread(void * pParam);	//�����߳�
	//++++++++++++++++++++++++++++++++++++++++++++
	//20130903���޸�
private:
	pthread_t m_connectThread;
	static DWORD WINAPI connectThread(void * pParam);	//�����߳�
	_XBool m_isClientCreated;
	int m_threadSum;			//�̵߳�����
	_XBool m_useBroadcast;
	string m_serverIP;
public:
	_XBool createClient(_XBool useBroadcast = XTrue,const char *ip = NULL,int port = 0);			//ͨ���㲥����ʽ���ӷ�����
	void (*m_connectBrokenFun)();	//����Ͽ�ʱ����õĻص�����
	void (*m_connectFun)();			//�������ӽ�����ʱ����õĺ���
	//--------------------------------------------
private:
	int m_recvPacketSize;	//���յ������ݰ��Ĵ�С
	unsigned char m_packetHeadData[PACKET_HEAD_LEN];
	_XBool getDataPacket(unsigned char *buff,int len);
	_XBool sendDataSocket(char * data,int len);

	//������Ϊ�˹㲥�Ŷ���Ľӿ�
	sockaddr_in m_serverAddr;
	int m_serverPort;
	char m_projectStr[PROJECT_STRING_LEN];	//���̵������ַ���
	_XBool m_boardcastThreadState;	//�㲥�̵߳�״̬
	//_XBool recvBoardcast();	//���ջ�Ӧ
	_XBool sendBoardcast();	//�㲥����
	_XBool m_isExit;
public:
	_XBool connectToServer(const char *ip,int port = NET_SERVER_PORT);	//���ӷ�����,ͨ��������IP��port���ӷ�����
	_XBool connectToServer();			//�Լ������㲥���ӷ�����
	_XBool getIsConnect() const
	{
		return m_connectState == CONNECT_STATE_CONNECT;
	}
	_XBool setProjectStr(const char * str)
	{
		if(str == NULL) return XFalse;
		if(strlen(str) >= PROJECT_STRING_LEN) return XFalse;
		strcpy(m_projectStr,str);
		return XTrue;
	}
	_XBool sendData(_XNetData *data)
	{
		if(data == NULL) return XFalse;
		m_mutexSend.Lock();
		m_sendDataBuff.push_back(data);
		m_mutexSend.Unlock();
		return XTrue;
	}
	void clearSendData(_XNetDataType type)	//�ӷ���������ȡ�����еķ�������
	{
		m_mutexSend.Lock();
		std::deque<_XNetData *>::iterator it = m_sendDataBuff.begin();
		for(;it != m_sendDataBuff.end();it ++)
		{
			if((* it)->type == type) (* it)->isEnable = XFalse;
		}
		m_mutexSend.Unlock();
	}
	_XNetData *getData()
	{
		if(m_recvDataBuff.size() <= 0) return NULL;
		m_mutexRecv.Lock();
		if(m_recvDataBuff.front()->isEnable)
		{//���ݰ�����
			_XNetData * ret = m_recvDataBuff.front();
			m_recvDataBuff.pop_front();
			m_mutexRecv.Unlock();
			return ret;
		}else
		{
			m_mutexRecv.Unlock();
			return NULL;
		}
	}
	_XConnectState getConnectState() const {return m_connectState;}
	void release();
	_XNetClient()
		:m_connectState(CONNECT_STATE_DISCONNECT)
		,m_netSocket(-1)
		,m_boardcastThreadState(XFalse)
		,m_isExit(XFalse)
		,m_connectBrokenFun(NULL)
		,m_connectFun(NULL)
		,m_isClientCreated(XFalse)
		,m_threadSum(0)
	{
		m_recvDataBuff.clear();
		setProjectStr(DEFAULT_PROJECT_STR);
	}
	~_XNetClient(){release();}
};
#endif