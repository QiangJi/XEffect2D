#ifndef _JIA_XSOCKETEX_
#define _JIA_XSOCKETEX_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.2.25
//--------------------------------
//����һ����ǿ�͵Ļ���socket���������ӵ��࣬���������������������������֧�ֶ�ͻ�������
//���ʹ�����Ӻ�ͨѶ������ƣ����Ӳ㸺�𱣳�����������Լ���������״̬
//ͨѶ�㸺��ͨѶ�Լ�����ͨѶ״̬��ͨѶ�㻹��������ͼ���ģ�顣
#include "XOSDefine.h"
#include <string>
#include <vector>
#include <deque>
#include "XCritical.h"
#include "XThread.h"
#include "XMedia/XNetCommon.h"
//1����δ��ɶ����ݽ��м���
//2�����Ʋ�����Ψһ�ԣ�Ҳ���ǿ����������Ƿ������������ݾ���Ӧ����ȷ��
//3���������������ͻ��˵�����û�ж�ԭ�пͻ��˵����ݽ��д����������ۻ���Դ���ģ���Ҫ����Ӧ�ĵ��ߴ�����ơ�

//��������״̬
enum _XSocketConnectState
{
	CON_STATE_NULL,		//��δ��������
	CON_STATE_NORMAL,	//��������
	CON_STATE_UNSTABLE,	//���Ӳ��ȶ������ݶϿ�
	CON_STATE_ERROR,	//�������ӶϿ�
};
enum _XSocketNetRole
{
	NET_NULL,		//��δ����
	NET_SERVER,		//���������
	NET_CLIENT,		//����ͻ���
};
enum _XSocketMode
{
	SOCKET_LAN,		//������
	SOCKET_WAN,		//������
};
enum _XSocketWorkMode
{
	SOCKET_WORK_MODE_IP,	//ͨ��IP�ķ�ʽ��������
	SOCKET_WORK_MODE_NAME,	//ͨ�����������Ƶķ�ʽ��������
};
enum _XSocketSysDataType
{//ϵͳ���ݵ�����
	SYS_DATA_TYPE_SETNAME,		//��������
	SYS_DATA_TYPE_SETID,		//����ID
	SYS_DATA_TYPE_UPDATEINIFO,	//������Ϣ
	SYS_DATA_TYPE_HEARTBEAT,	//�����ź�
};
enum _XSocketExDataType
{
	SOCKET_DATA_TYPE_SYSTEM,	//ϵͳ����
	SOCKET_DATA_TYPE_CUSTOM,	//��������
};
#define SOCKET_PACKET_HEAD_SIZE (16)	//���ݰ��İ�ͷ��С
#define DEFAULT_CLIENT_NAME ("xiajia_1981@163.com")
#pragma pack(push)
#pragma pack(1)
struct _XSocketDataPack
{//���ݰ��Ľṹ
	int fromID;	//��������
	int toID;	//������ȥ
	_XSocketExDataType dataType;	//��������
	int dataLen;	//���ݳ���
	unsigned char * data;	//������
	_XSocketDataPack()
		:data(NULL)
		,dataType(SOCKET_DATA_TYPE_CUSTOM)
	{}
};
#pragma pack(pop)
enum _XSocketClientState
{
	CLIENT_STATE_ONLINE,	//����
	CLIENT_STATE_OFFLINE,	//����
};
#define SOCKET_INFO_HEAD_SIZE (16)	//�û���Ϣͷ��С
struct _XSocketUserInfo
{
	int userID;				//�û�ID���ɷ��������䣩
	int userSocket;			//�û��׽���
	_XSocketClientState userState;			//�û�״̬
	int userNameLen;
	char *userName;			//�û�����
	_XSocketUserInfo()
		:userName(NULL)
		,userNameLen(0)
	{}
};
enum _XSocketClinetState
{//�ͻ��˵�״̬
	CLINET_STATE_UNCONNECT,	//��δ����
	CLINET_STATE_CONNECT,	//������
	CLINET_STATE_DISCONNECT,//���ӶϿ�
};
struct _XSocketRecvPacket
{
	int recvDataLen;				//���յ������ݰ��ĳ���
	unsigned char headData[SOCKET_PACKET_HEAD_SIZE];		//���ݰ��İ�ͷ����
	_XSocketDataPack *dataPacket;	//���ݰ�������
	_XSocketRecvPacket()
		:recvDataLen(0)
		,dataPacket(NULL)
	{}
};
struct _XSocketUserData
{
	bool isEnable;	//�Ƿ���Ч
	int userID;		//�û�ID
	int userSocket;	//�û��������׽���
	_XSocketClinetState state;
	int heartbeatTimer;	//������ʱ��

	_XSocketRecvPacket recvPacket; //���յ����ݰ�
	_XThreadState recvThreadState;

	_XSocketUserData()
		:isEnable(false)
		,state(CLINET_STATE_UNCONNECT)
		,heartbeatTimer(0)
		,recvThreadState(STATE_BEFORE_START)
	{}
};

class _XSocketEx
{
private:
	bool m_isInited;	//�Ƿ��ʼ��
	SOCKET m_netSocket;	//�����������ӵ��׽���
	_XSocketNetRole m_socketRole;		//���������߿ͻ���
	int m_timer;	//��ʱ��

	//�ͻ��˵����
	_XSocketConnectState m_conState;	//��������״̬
	_XSocketRecvPacket m_recvPacket;	//���յ����ݰ�
	//�����������
	int m_nowAddClientID;	//��ǰ����Ŀͻ��˵�ID
	void addAClient(int clientSocket);	//�µĿͻ��˼���
	int m_myID;		//����ı��
	int m_IDIndex;	//���ڽ���ID

	_XCritical m_recvMutex;				//���յ��߳���
	std::deque<_XSocketDataPack *> m_recvDeque;		//���ն���
	_XCritical m_sendMutex;				//���͵��߳���
	std::deque<_XSocketDataPack *> m_sendDeque;		//���Ͷ���
	_XCritical m_userInfoMutex;			//�û���Ϣ���߳���
	std::vector<_XSocketUserInfo *> m_usersInfo;	//�û���Ϣ����
	//_XCritical m_userDataMutex;			//�û����ݵ��߳���
	std::vector<_XSocketUserData *> m_usersData;	//�û����ݶ���

	_XSocketWorkMode m_workMode;

	sockaddr_in m_serverAddr;	//��������ַ
	std::string m_serverName;	//������������
	std::string m_serverIP;		//��������IP
	int m_serverPort;		//�������Ķ˿ں�
	std::string m_myName;	//�Լ�������

	bool getDataPacket(_XSocketRecvPacket &packet,unsigned char *buff,int len);	//�����ݽ��������������ݰ�

	_XThreadState m_connectThreadState;	//�����̵߳�״̬
	_XThreadState m_recvThreadState;	//�����̵߳�״̬
	_XThreadState m_sendThreadState;	//�����̵߳�״̬
	_XThreadState m_acceptThreadState;	//�����̵߳�״̬
	HANDLE m_acceptThreadHandle;
	_XThreadState m_boardcastThreadState;	//�㲥�̵߳�״̬
#ifdef XEE_OS_WINDOWS
    static DWORD WINAPI connectThread(void * pParam);	//�����߳� 
	static DWORD WINAPI recvThread(void * pParam);		//�����߳� 
	static DWORD WINAPI sendThread(void * pParam);		//�����߳� 
	static DWORD WINAPI acceptThread(void * pParam);	//�����߳� 
	static DWORD WINAPI recvThreadX(void * pParam);		//��������Ϊ�ͻ��˿����Ľ����߳� 
	static DWORD WINAPI boardcastThread(void * pParam);		//�㲥�߳� 
#endif
	bool connectToNet();	//���ӵ�����
	//������һЩ˽�еķ���
	bool connectToServer();	//���ӵ�������
	bool createServer();	//����������
	void sendClientInfo();	//�����пͻ��˷���Ŀǰ������״̬
	void systemDataproc(const _XSocketDataPack *data);
	void lockData(unsigned char *data,int len);		//��������
	void unlockData(unsigned char *data,int len);	//��������
	bool sendAData(_XSocketDataPack * data)
	{//ע����̻߳���,����Ĭ��Ϊ�����Ѿ���������
		if(data == NULL) return false;
		if(data->toID == m_myID)	//�Լ����͸��Լ�����Ϣֱ�Ӷ���
		{
			XDELETE_ARRAY(data->data);
			XDELETE(data);
			return false;
		}
		m_sendMutex.Lock();
		m_sendDeque.push_back(data);
		m_sendMutex.Unlock();
		return true;
	}
	void requestRename();	//����������͸�������
	void sendHeartbeat();	//���������ź�
	bool m_clientStateChange;	//�ͻ��˵�״̬�Ƿ����仯
	void setOffline(int id);//����ĳ���ͻ�������
	void heartbeatProc(int stepTime);//����ͻ��˵�����������Ӧ�Ĵ���
	bool sendBoardcast();	//�㲥����
public:
	bool createNetWorkByName(const char *serverName,int port)		//ͨ�����������ƽ�������
	{
		if(m_isInited) return false;	//�ظ�����
		if(serverName == NULL) return false;

		m_workMode = SOCKET_WORK_MODE_NAME;
		m_serverName = serverName;
		m_serverPort = port;
		m_conState = CON_STATE_NULL;
		//m_socketRole = NET_NULL;
		//���潨�������߳�
		m_connectThreadState = STATE_BEFORE_START;
		if(CreateThread(0,0,connectThread,this,0,NULL) == 0) return false;	//�����߳̽���ʧ��
		//���潨�����ݴ����߳�(��δ���)

		m_isInited = true;
		return true;
	}
	bool createNetWorkByIP(const char *serverIP,int port)		//ͨ��������IP��������
	{//��δ���
		if(m_isInited) return false;	//�ظ�����
		if(serverIP == NULL) return false;

		m_workMode = SOCKET_WORK_MODE_IP;
		m_serverIP = serverIP;
		m_serverPort = port;
		m_conState = CON_STATE_NULL;
		//m_socketRole = NET_NULL;
		//���潨�������߳�
		m_connectThreadState = STATE_BEFORE_START;
		if(CreateThread(0,0,connectThread,this,0,NULL) == 0) return false;	//�����߳̽���ʧ��
		//���潨�����ݴ����߳�(��δ���)

		m_isInited = true;
		return true;
	}
	//����
	bool sendAData(int toID,unsigned char * data,int len);	//��ָ���û�������Ϣ
	bool sendToAll(unsigned char * data,int len)
	{
		if(m_socketRole == NET_CLIENT) return sendAData(-1,data,len); else
		if(m_socketRole == NET_SERVER)
		{//����Ƿ������������пͻ��˷�������
		//	m_userDataMutex.Lock();
			for(int i = 0;i < m_usersData.size();++ i)
			{
				if(m_usersData[i]->state == CLINET_STATE_CONNECT) 
					sendAData(m_usersData[i]->userID,data,len);
			}
		//	m_userDataMutex.Unlock();
		}
		return true;
	}			//��ǰ���ߵ������û�������Ϣ(��δʵ��)
	//����
	_XSocketDataPack *popAData()
	{
		m_recvMutex.Lock();
		if(m_recvDeque.size() <= 0)
		{
			m_recvMutex.Unlock();
			return NULL;
		}
		_XSocketDataPack *ret = m_recvDeque[0];
		m_recvDeque.pop_front();
		m_recvMutex.Unlock();
		return ret;
	}
	void deleteAData(_XSocketDataPack * p)
	{
		XDELETE_ARRAY(p->data);
		XDELETE_ARRAY(p);
	}

	void release();
	_XSocketEx()
		:m_isInited(false)
		,m_socketRole(NET_NULL)
		,m_conState(CON_STATE_NULL)
		,m_workMode(SOCKET_WORK_MODE_IP)
		,m_netSocket(INVALID_SOCKET)
		,m_nowAddClientID(-1)
		,m_IDIndex(0)
		,m_myID(0)
		,m_myName(DEFAULT_CLIENT_NAME)
		,m_timer(0)
		,m_clientStateChange(false)
		,m_connectThreadState(STATE_BEFORE_START)	//�����̵߳�״̬
		,m_recvThreadState(STATE_BEFORE_START)	//�����̵߳�״̬
		,m_sendThreadState(STATE_BEFORE_START)	//�����̵߳�״̬
		,m_acceptThreadState(STATE_BEFORE_START)	//�����̵߳�״̬
		,m_boardcastThreadState(STATE_BEFORE_START)	//�㲥�̵߳�״̬
		,m_acceptThreadHandle(0)
	{}
	~_XSocketEx() {release();}

	//������һЩ��ɢ�ķ�������
	int getClientSum() {return m_usersInfo.size();}
	const _XSocketUserInfo * getUserInfo(int index)
	{
		_XSocketUserInfo * temp = NULL;
		m_userInfoMutex.Lock();
		if(index < 0 || index >= m_usersInfo.size())
		{
			m_userInfoMutex.Unlock();
			return NULL;
		}
		temp = m_usersInfo[index];
		m_userInfoMutex.Unlock();
		return temp;
	}
	void setAsClient()
	{ 
		if(m_isInited) return;
		m_socketRole = NET_CLIENT;
	}
	void setAsServer()
	{ 
		if(m_isInited) return;
		m_socketRole = NET_SERVER;
	}
	_XSocketNetRole getRole() const {return m_socketRole;}
	int getMyID() const {return m_myID;}
	int getMySocket() const {return m_netSocket;}
	bool setMyName(const std::string &myName);
	std::string getMyName(){return m_myName;}
	_XSocketConnectState getConState() const {return m_conState;}
};
#endif