//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBasicClass.h"

#ifdef XEE_OS_WINDOWS
//void close(int temp)
//{
//    //ע��������Ҫʹ��shutdown(),���Ǿ������ʹ�û���Ҫ��ϸ������20100527��
//    //shutdown()
//    closesocket(temp);
//}

//#include "winsock2.h"

int initSocket()
{
    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(1,1),&wsaData) != 0) return 0;
    if(LOBYTE(wsaData.wVersion) != 1 
           || HIBYTE(wsaData.wVersion) != 1) 
    {
        WSACleanup( );    
        return 0; 
    }
    return 1;
}
#endif