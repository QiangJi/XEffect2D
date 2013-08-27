#ifndef _JIA_XDATABASIC_
#define _JIA_XDATABASIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------

#include "stdio.h" 

#ifdef OS_WINDOWS
#include "windows.h"
#endif

#include "XBasicClass.h"

//����һ������ת�����洢����ʽ�������ܡ��������
//Ϊ�����ݵİ�ȫ������ʹ��AB��������ķ�ʽ�����Խϴ�Χ��ȷ��������һ�����еı���
#define DATABASIC_DEFAULT_FILE_NAME_A "databasicA.dat"
#define DATABASIC_DEFAULT_FILE_NAME_B "databasicB.dat"
#define DATABASIC_KEY_LENGTH 128

enum ANALYZING_FILE_MODE
{
    ANALYZING_BUFFERING,    //���屣��    //���ݲ���ȫ��Ӳ����ȫ��Ч
    ANALYZING_IMMEDIATELY    //��ʱ����    //���ݰ�ȫ��Ӳ�����
};

class _XDataBasic
{
private:
    int m_ID;                    //������ݵ�ID�����ڴ������ݹ���
    int m_versionsOrder;        //���ڼ�¼���ݵİ汾��ÿ�����ݵĸı䶼��ı���汾���Ա���ȷ�����ļ����Ⱥ�˳��
    int m_dataLength;            //���ݳ���
    unsigned char *m_pData;        //����
    unsigned char m_checkData;    //У���ַ�
    int m_secretKeyLength;        //��Կ����
    unsigned char *m_secretKey;    //����ʹ�õ���Կ��������Խ��м򵥵ļ��ܺ�У�飩
    char m_isInited;            //�Ƿ��ʼ��

    bool EncryptData();            //��������
    bool DeCryptData();            //��������
    unsigned char getCheckData();        //����У����
    bool checkDataIsRigth();    //��������Ƿ����У��
    char m_nowBackOrder;        //��ǰ�����ı������

    bool mergeDataBack(const char *fileName);    //������������ڲ����ã������������������߼�
    bool mergeDataBack(const char *fileNameA,const char *fileNameB);    //������������ڲ����ã������������������߼�
    bool analyzingDataBack(const char *fileName);    //������������ڲ����ã������������������߼�
    bool analyzingData(const char *fileName);    //�����ݱ��浽�ļ�

	bool m_isSaveAsynchronous;	//�Ƿ�ʹ���첽���ݱ���
	bool m_needSave;			//�����Ƿ���Ҫ����
	char *m_fileName;			//���ڱ������ݵ��ļ���
	
	ANALYZING_FILE_MODE m_saveMode;	//�ļ��Ĵ洢ģʽ

	char m_isEnd;	//�Ƿ����
	
	_XDataBasic(const _XDataBasic &temp)	//���ؿ������캯��Ϊ˽�з�ֹ����Ҫ�Ĵ���
	{
	}
public:
    _XDataBasic();
    ~_XDataBasic();
	void release();

    bool init(int ID,bool isSaveAsynchronous = false,ANALYZING_FILE_MODE saveMode = ANALYZING_BUFFERING);
    //�ڴ������ݼ��ת��
    bool mergeData(const unsigned char *pData,int dataLength);        //���ⲿ�ϲ����ݵ��ڲ�
    int analyzingData(unsigned char *pData,int dataLength);			//���ڲ����ݽ������ⲿ,�����ڲ����ݵĴ�С��-1Ϊ����
    //�ļ������ݼ��ת��
    bool mergeData(const char *fileName = NULL);            //���ļ�����ȡ����
    bool analyzingDataManager(const char *fileName = NULL);        //�����ݱ��浽�ļ���������������֮ǰ�û������öԱ���ľ���������е���
    //���������ݼ��ת��(Ŀǰ��ʱ�����ṩ���֧��)

	//�첽���ݱ����߳�
#ifdef OS_WINDOWS
	static DWORD WINAPI saveDataThread(void * pParam);
#endif
#ifdef OS_LINUX
	static void *saveDataThread(void * pParam); 
#endif 
	pthread_t m_saveDataThreadP;

	void endDataBasic();	//�˳�ʱ�ر��߳�
};


inline void _XDataBasic::endDataBasic()
{
    if(m_isInited == 0) return;
	if(m_isSaveAsynchronous)
	{
		m_isEnd = 1;
	}
}

#endif