#ifndef _JIA_XRESOURCEPACK_
#define _JIA_XRESOURCEPACK_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//Ŀǰ������������������
//--------------------------------

#include "XOSDefine.h"
#include "XBasicFun.h"

#include "stdio.h"
#include <memory>
#include "string.h"

class _XTextureInfo;

using namespace std;

#define MAX_FILE_SUM (3000)			//��Դ��������ܰ�������Դ����
//#define MAX_FILE_NAME_LENGTH (256)	//�ļ����ĳ���
#define LOCK_CODE_LENGTH (32)			//��Կ�ĳ��ȣ���Կ�ֳ��ĸ����֣�ÿ������32��bytes
#define ALL_LOCK_CODE_LENGTH (128)	//��Կ�ܳ���

#pragma pack(push)
#pragma pack(1)
struct _XResourceFileStruct
{
	char fileName[MAX_FILE_NAME_LENGTH];		//�ļ�����
	int fileLength;								//�ļ�����
	int filePosition;							//�ļ�ƫ������
};
#pragma pack(pop)

class _XResourcePack
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	_XResourcePack();
	_XResourcePack(const _XResourcePack&);
	_XResourcePack &operator= (const _XResourcePack&);
	virtual ~_XResourcePack(); 
public:
	static _XResourcePack& GetInstance();
	//-------------------------------------------
	//+++++++++++++++++++++++++++++++++++++++++++
	//2011����������ڴ���Ҽ���
private:
	unsigned char *m_fileData;
	int m_fileDataLength;
	int m_fileDataPoint;
	//-------------------------------------------
public:
	_XResourceFileStruct *m_fileInfo;						//����ܴ��MAX_FILE_SUM���ļ�
	int m_fileSum;											//��Դ���е��ļ�����
	int m_normalFileSum;									//��ͨ�ļ�������
	_XBool m_haveReadedFileList;								//�Ƿ��Ѿ���ȡ������Ϣ
	_XBool m_haveReadedNormalFileList;						//�Ƿ��Ѿ���ȡ����ͨ�ļ�������Ϣ
	long m_headLength;										//��Դ�����ļ�ͷ�ĳ���
	unsigned char tempCode[LOCK_CODE_LENGTH];				//��Դ���еļ�����
	unsigned char m_lockKey[ALL_LOCK_CODE_LENGTH];			//����
	unsigned char m_hardLockKey[LOCK_CODE_LENGTH];			//��Կ����������Ӳ���ļ�����Կ

	char m_outFileName[MAX_FILE_NAME_LENGTH];				//����ļ�������
	void setOutFileName(const char *temp = NULL);			//��������ļ�������
private:
	_XBool m_isGetOptimizeInfo;
	_XBool getOptimizeInfo();
	_XBool releaseOptimizeInfo();
	_XTextureInfo *m_texInfo;
	int *m_targetOrder;	//��ͼ��Ŀ����ͼ֮��Ķ�Ӧ��ϵ
	char *m_targetName;	//��Դ�Ż�֮���Ŀ������
	int m_texInfoSum;
	int m_targetTextureSum;
public:
	int makeFileList();										//�����嵥�ļ�
	int makeResourcePack(int packMode = 0);					//�����Դ����packMode = 0ʱ��װ���ǲ���Ҫ�������ܺ�У�����Դ��
															//			��packMode = 1ʱ��װ���Ǿ������ܺ�У�����Դ
	//Ŀǰ���������������ѹ������Դ����������
	int unpackAllResource();								//������е���Դ	//����û��Ŀ¼�����Ĵ�����������ֻ�ܽ�ѹ����Ŀ¼���ļ�
	int unpackResource(const char *fileName);						//���ĳһ����Դ	//����û��Ŀ¼�����Ĵ�����������ֻ�ܽ�ѹ����Ŀ¼���ļ�
	int unpackResource(const char *fileName,unsigned char *p);	//���ĳһ����Դ��һ���ڴ�ռ�
	int getStringLength(const char *p) const;							//����ַ����ĳ���
	void setHardLockKey(const unsigned char *p);							//����Ӳ���μ�����Կ

	int checkFileCode(const char *p) const;								//����ļ��������Ƿ���ȷ
	//int xFileNameCMP(char *p1,char *p2);					//�Ƚ������ļ�·���Ƿ���ͬ

	//int getPathDeep(char *p);								//��ȡ·�����
	int readFileListFromResouce();							//��ѹ�����ж�ȡ�ļ�������Ϣ		//���������Ҫ�ع�
	int readNormalFileListFromResouce();					//��ѹ�����ж�ȡ��ͨ�ļ���������Ϣ	//���������Ҫ�ع�
	int getFileLength(const char *fileName);						//��Դ������Դ�ļ��Ĵ�С
	unsigned char *getFileData(const char *filename);		//��ȡָ���ļ����ļ�����

	int writeCheckData();	//����Դ�ļ���д��У���
	_XBool checkCheckData();	//У����Դ�ļ���У���

	void lockOrUnlockProc(unsigned char *p,int startPoint,int length) const;	//��������p���м��ܻ��߽��� startPointΪ��ʼλ�� lengthΪ���ݳ���
	void getlockKey();		//��������

	int isOptimized(const char *p) const;	//�ж�����ļ��Ƿ��Ż� 0:û�в����Ż� 1:�Ѿ������Ż�
	void release();	//�ͷ���Դ
};

inline int _XResourcePack::getStringLength(const char *p) const
{
	if(strlen(p) >= MAX_FILE_NAME_LENGTH) 
	{
		return 0;
	}else
	{
		return (int)(strlen(p));
	}
//	for(int i=0;i < MAX_FILE_NAME_LENGTH;++ i)
//	{
//		if(p[i] == '\0') return i;
//	}
//	return 0;
}
inline void _XResourcePack::setHardLockKey(const unsigned char *p)
{
	memcpy(m_hardLockKey,p,32);
}
inline _XResourcePack::~_XResourcePack()
{
	release();
}
inline _XResourcePack& _XResourcePack::GetInstance()
{
	static _XResourcePack m_instance;
	return m_instance;
}
inline void _XResourcePack::release()
{
	XDELETE_ARRAY(m_fileInfo);
	XDELETE_ARRAY(m_fileData);
	m_haveReadedFileList = XFalse;
}

#endif