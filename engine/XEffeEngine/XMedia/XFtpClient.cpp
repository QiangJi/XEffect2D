//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XFtpClient.h"
#include "XNetCommon.h"

_XBool _XFtpClient::connectServer(const char * serverIP,int port)	//���������������
{
	if(serverIP == NULL) return XFalse;
	if(m_isConnect) return XFalse;		//��ֹ�ظ�����
	
	if(WSAStartup(MAKEWORD(2,2),&m_wsaData) != 0) printf("Socket init error\n");
	m_cmdsocket = socket(AF_INET,SOCK_STREAM,0); //�����׽���
	sockaddr_in serveraddr;
	serveraddr.sin_addr.s_addr = inet_addr(serverIP);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(port);
	if(connect(m_cmdsocket,(const sockaddr *)&serveraddr,sizeof(sockaddr))==SOCKET_ERROR)
	{
		printf("connect error!\n");
		closesocket(m_cmdsocket);
		return XFalse;
	}
	//���շ�������
	if(!recvRetCode()) return XFalse;
	if(m_retCode != 220) return XFalse;	//�������ݲ���
	m_isConnect = XTrue;
	return XTrue;
}
//�����Ƕ�ftp��װ��һЩ����
_XBool _XFtpClient::setPort(SOCKET *listenSock)	//����ͨѶ�˿�
{
	if(!m_isConnect) return XFalse;
	//��ȡ�����ip
	//char hname[128];
	//gethostname(hname,sizeof(hname));
	//hostent *hent = gethostbyname(hname);
	//sprintf(hname,"%d.%d.%d.%d",(unsigned char)(hent->h_addr_list[0][0]),
	//	(unsigned char)(hent->h_addr_list[0][1]),
	//	(unsigned char)(hent->h_addr_list[0][2]),
	//	(unsigned char)(hent->h_addr_list[0][3]));
	//std::string dip = hname;
	std::string dip = getMyIP();
	std::string::size_type pos = 0;
	sockaddr_in localAddr;
	int sizeAddr = sizeof(sockaddr);
	UINT dataport;
	std::string str;
	*listenSock = socket(AF_INET,SOCK_STREAM,0);
	//localAddr.sin_addr.s_addr = inet_addr(hname);
	localAddr.sin_addr.s_addr = inet_addr(dip.c_str());
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(0);
	if(bind(*listenSock,(const sockaddr*)& localAddr,sizeAddr) == SOCKET_ERROR)
	{
		printf("bind error:%d\n",GetLastError());
		return XFalse;
	}
	listen(*listenSock,5);
	getsockname(*listenSock,(struct sockaddr*)&localAddr,&sizeAddr);
	dataport = ntohs(localAddr.sin_port);	
	while((pos = dip.find(".", pos)) != std::string::npos)
		dip.replace(pos,1, ",");

	sprintf(m_commandBuff,"PORT %s,%d,%d\r\n",dip.c_str(),dataport >> 8,dataport%256);
	if(!sendCommand(m_commandBuff)) return XFalse;

	if(m_retCode != 200) return XFalse;	//�������ݲ���
	return XTrue;
}
_XBool _XFtpClient::sendFileList()	//��ȡ�ļ��б�
{//����ļ��б�����ݲ�û�д��ݳ���
	if(!m_isConnect) return XFalse;
	SOCKET listensocket;
	if(!setPort(&listensocket)) return XFalse;
	if(!sendCommand("LIST\r\n")) return XFalse;
	if(m_retCode != 125) return XFalse;	//�������ݲ���
	//���濪ʼ��������
	sockaddr_in clientaddr;
	int sizeaddr = sizeof(sockaddr);
	SOCKET clientsocket = accept(listensocket,(struct sockaddr*)&clientaddr,&sizeaddr);
	closesocket(listensocket);
	char buf[RECV_BUFF_SIZE + 1];
	int ret = 0;
	while(1)
	{
		ret = recv(clientsocket,buf,RECV_BUFF_SIZE,0);
		if(ret > 0)
		{
			buf[ret]='\0';
			printf("%s",buf);
		}else
		{//���ݷ������
			break;
		}
	}
	//�鿴�����Ƿ��Ѿ��������
	if(!recvRetCode()) return XFalse;
	if(m_retCode != 226) return XFalse;
	closesocket(clientsocket);
	return XTrue;
}
_XBool _XFtpClient::sendDownloadFile(const char *filename)	//����ָ���ļ�
{
	if(!m_isConnect) return XFalse;
	if(filename == NULL) return XFalse;
	SOCKET listensocket;
	if(!setPort(&listensocket)) return XFalse;
	strcpy(m_commandBuff,"RETR ");
	strcat(m_commandBuff,filename);
	strcat(m_commandBuff,"\r\n");
	if(!sendCommand(m_commandBuff)) return XFalse;
	//���濪ʼ�����ļ�����
	sockaddr_in clientaddr;
	int sizeaddr = sizeof(sockaddr);
	SOCKET clientsocket = accept(listensocket,(struct sockaddr*)&clientaddr,&sizeaddr);
	closesocket(listensocket);
	std::string str = std::string(".") + "//" + filename;
	ofstream file;
	file.open(str.c_str(),ios::binary);
	int ret = 0;
	char buf[RECV_BUFF_SIZE];
	while(1)
	{
		ret = recv(clientsocket,buf,RECV_BUFF_SIZE,0);
		if(ret > 0) file.write(buf,ret);
		else break;	//���ݽ������
	}
	file.close();
	closesocket(clientsocket);
	if(!recvRetCode()) return XFalse;
	if(m_retCode != 226) return XFalse;
	return XTrue;
}
_XBool _XFtpClient::sendUpdateFile(const char *filename)	//�ϴ��ļ�
{
	if(!m_isConnect) return XFalse;
	if(filename == NULL) return XFalse;
	SOCKET listensocket;
	if(!setPort(&listensocket)) return XFalse;
	strcpy(m_commandBuff,"STOR ");
	strcat(m_commandBuff,filename);
	strcat(m_commandBuff,"\r\n");
	if(!sendCommand(m_commandBuff)) return XFalse;
	//���濪ʼ�ϴ�����
	sockaddr_in clientaddr;
	int sizeaddr = sizeof(sockaddr);
	SOCKET clientsocket = accept(listensocket,(struct sockaddr*)&clientaddr,&sizeaddr);
	closesocket(listensocket);
	std::string str = std::string(".") + "//" + filename;
	ifstream file;
	file.open(str.c_str(),ios::binary);
	//filebuf *fb = file.rdbuf();
	file.seekg(0, std::ios::end);//�������ļ��ĳ���
	int length = file.tellg(); 
	file.seekg(0, std::ios::beg);
	int left = length;
	char buf[RECV_BUFF_SIZE];
	while(left > 0)
	{//�����ļ�����ֱ���������
		file.read(buf,RECV_BUFF_SIZE<left?RECV_BUFF_SIZE:left);
		send(clientsocket,buf,RECV_BUFF_SIZE<left?RECV_BUFF_SIZE:left,0);
		left -= RECV_BUFF_SIZE;
		//printf(".");
		Sleep(1);
	}
	//printf("\n");
	file.close();
	closesocket(clientsocket);
	if(!recvRetCode()) return XFalse;
	if(m_retCode != 226) return XFalse;
	return XTrue;
}
_XBool _XFtpClient::sendUpdateFile(const char *inFilename,const char *outFilename)
{
	if(!m_isConnect) return XFalse;
	if(inFilename == NULL) return XFalse;
	if(outFilename == NULL) return XFalse;
	SOCKET listensocket;
	if(!setPort(&listensocket)) return XFalse;
	strcpy(m_commandBuff,"STOR ");
	strcat(m_commandBuff,outFilename);
	strcat(m_commandBuff,"\r\n");
	if(!sendCommand(m_commandBuff)) return XFalse;
	//���濪ʼ�ϴ�����
	sockaddr_in clientaddr;
	int sizeaddr = sizeof(sockaddr);
	SOCKET clientsocket = accept(listensocket,(struct sockaddr*)&clientaddr,&sizeaddr);
	closesocket(listensocket);
	std::string str = std::string(".") + "//" + inFilename;
	ifstream file;
	file.open(str.c_str(),ios::binary);
	//filebuf *fb = file.rdbuf();
	file.seekg(0, std::ios::end);//�������ļ��ĳ���
	int length = file.tellg(); 
	file.seekg(0, std::ios::beg);
	int left = length;
	char buf[RECV_BUFF_SIZE];
	while(left > 0)
	{//�����ļ�����ֱ���������
		file.read(buf,RECV_BUFF_SIZE<left?RECV_BUFF_SIZE:left);
		send(clientsocket,buf,RECV_BUFF_SIZE<left?RECV_BUFF_SIZE:left,0);
		left -= RECV_BUFF_SIZE;
		//printf(".");
		Sleep(1);
	}
	//printf("\n");
	file.close();
	closesocket(clientsocket);
	if(!recvRetCode()) return XFalse;
	if(m_retCode != 226) return XFalse;
	return XTrue;
}