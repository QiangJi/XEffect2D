#ifndef _JIA_XMODBUSPROTOCOL_
#define _JIA_XMODBUSPROTOCOL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.9.25
//--------------------------------
//����һ�����ڴ���ͨѶ��Э���ʵ��

extern unsigned short CRC16(unsigned char *p,unsigned short len);

//Э��Ĺ���ģʽ
enum _XModBusWorkMode
{
	MODE_ASCII,		//16���ƹ���ģʽ
	MODE_RTU,		//2���ƹ���ģʽ
};
enum _XModBusType
{
	TYPE_MASTER,	//���豸
	TYPE_SLAVE,		//���豸
};

#include <deque>
#include "XBasicClass.h"
#include "XMedia/XSerialPort.h"

struct _XModBusData
{
	int delayTime;		//���յ����ݵ��ӳ�ʱ��
	unsigned char * data;
	int dataLen;
	_XModBusData()
		:data(NULL)
		,dataLen(0)
	{
	}
};
enum _XModeBusCMD
{
	CMD_READ_COIL_VOLUME = 1,
	CMD_READ_INPUT = 2,
	CMD_READ_HOLD_REGISTER = 3,
	CMD_READ_INPUT_REGISTER = 4,
	CMD_SET_ONE_COIL = 5,
	CMD_SET_ONE_REGISTER = 6,
	CMD_SET_COILS = 15,
	CMD_SET_REGISTERS = 16,
};
enum _XModeBusErrorCode
{
	ERROR_READ_COIL_VOLUME = 0x81,
	ERROR_READ_INPUT = 0x82,
	ERROR_READ_HOLD_REGISTER = 0x83,
	ERROR_READ_INPUT_REGISTER = 0x84,
	ERROR_SET_ONE_COIL = 0x85,
	ERROR_SET_ONE_REGISTER = 0x86,
	ERROR_SET_COILS = 0x8f,
	ERROR_SET_REGISTERS = 0x90,
};
#define MODBUS_HEAD_LEN (8)
struct _XModbusState
{
	int nPort;		//ͨѶ���ڵĶ˿ں�
	int nBaud;		//ͨѶƵ��
	int nParity;	//��żУ��
	unsigned char deviceID;			//�������
	_XModBusWorkMode workMode;		//����ģʽ��16���ƻ���2����
	_XModBusType workType;			//�������ͣ��������ߴӻ�
	unsigned char *coilsBuff;		//״̬�Ĵ���
	int coilsBuffSize;				//�ɱ������������bitΪ��λ
	int coilsOffset;				//״̬��ƫ��,ƫ��֮ǰ�Ĳ�ʹ��
	unsigned short *hRegisterBuff;	//���ּĴ���
	int hRegisterBuffSize;			
	int hRegisterOffset;			
	unsigned short *iRegisterBuff;	//����Ĵ���
	int iRegisterBuffSize;
	int iRegisterOffset;
	unsigned char *inputBuff;		//����״̬�Ĵ���
	int inputBuffSize;
	int inputOffset;
	int outTime;					//��ʱʱ�䵥λ��ms
	int maxResendTimes;				//����ط�����
	_XModbusState()
		:nPort(0)
		,nBaud(19200)
		,nParity(0)
		,workMode(MODE_RTU)
		,workType(TYPE_MASTER)
		,coilsBuff(NULL)
		,coilsBuffSize(0)
		,hRegisterBuff(NULL)
		,hRegisterBuffSize(0)
		,iRegisterBuff(NULL)
		,iRegisterBuffSize(0)
		,inputBuff(NULL)
		,inputBuffSize(0)
		,outTime(300)
		,maxResendTimes(3)
		,deviceID(1)
		,coilsOffset(0)
		,hRegisterOffset(0)
		,iRegisterOffset(0)
		,inputOffset(0)
	{
	}
};
class _XModBusProtocol
{
private:
	_XBool m_isInited;
	std::deque<_XModBusData> m_recvData;	//�յ�������
	std::deque<_XModBusData> m_sendData;	//���͵�����

	_XSerialPort m_serialPort;
	//static DWORD WINAPI recvThread(void * pParam);		//�����߳�
	static DWORD WINAPI sendThread(void * pParam);		//�����߳�

	//int m_tempDataOffset;
	//int m_tempDataLen;
	//unsigned char *m_tempDataBuff;
	//unsigned char m_modbusHead[MODBUS_HEAD_LEN];
	//int m_modbusNeedHeadLen;
	//int m_headLen;
	//void recvDataProc(unsigned char * data,int len);		//��������
	_XBool m_needRecv;	//�Ƿ�ȴ��ظ�
	int m_delayTime;	//�ȴ��ظ���ʱ��

	_XCritical m_sendMutex;
	_XCritical m_recvMutex;
//	_XThreadState m_recvThreadState;
	_XThreadState m_sendThreadState;

	_XModbusState m_modbusState;
	_XModBusData m_nowSendData;	//��ǰ���͵����ݰ�
	void sendNowData();			//���͵�ǰ��Ҫ���͵�����
	int m_sendTime;				//���ʹ���
	_XBool m_connectState;		//����״̬���������״̬ΪXFalse�Ļ������˳��շ��߳�
	_XBool pushAData(_XModBusData &data);			//���Ͷ���������һ������
	void answerProc(_XModBusData &data);			//����ָ��������ش�����
	void (*m_callBackFun)(const _XModBusData & data,void *p);	//����һ���ص������������յ�����ʱ���øûص�����
	void * m_pClass;
public:
	void setCallBackFun(void (*p)(const _XModBusData &,void *),void *pC)
	{
		m_callBackFun = p;
		m_pClass = pC;
	}

	_XBool openDevice(_XModbusState &modbusState);	//˵����mode��type�Ĺ�����δʵ��
	int getSendBuffSize();							//��ȡ���Ͷ��еĳ���
	_XBool pushData(_XModBusData &data)				//���Ͷ���������һ������
	{
		if(m_modbusState.workType == TYPE_MASTER)
			return pushAData(data);
		return XFalse;
	}
	int getRecvBuffSize();							//��ȡ���ܶ��еĳ���
	_XModBusData popData();							//�ӽ��ܶ�����ȡ��һ������

	void release();
	_XModBusProtocol()
		:m_isInited(XFalse)
	//	,m_tempDataBuff(NULL)
	{
	}
	~_XModBusProtocol(){release();}
	friend void recvCallback(void *pClass,unsigned char * data,int len);
};
inline int _XModBusProtocol::getRecvBuffSize()
{
	m_recvMutex.Lock();
	int ret = m_recvData.size();
	m_recvMutex.Unlock();
	return ret;
}
inline int _XModBusProtocol::getSendBuffSize()
{
	m_sendMutex.Lock();
	int ret = m_sendData.size();
	m_sendMutex.Unlock();
	return ret;
}
//����Ľӿ���δ���
extern int getModbusCMDDataSum(const _XModBusData &CMD);	//�������н�����������
extern bool getModbusCMDAddress(const _XModBusData &CMD,unsigned int &addr);
extern bool getModbusCMDData(const _XModBusData &CMD,unsigned int &data);

#endif