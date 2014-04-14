//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XShareMem.h"

#ifdef XEE_OS_WINDOWS
bool _XShareMem::init(const char *name,int size)
{
	if(m_isInited) return false;
	if(size <= 0) return false;
	m_size = size;

	m_hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		size,                // maximum object size (low-order DWORD)
		name);                 // name of mapping object
	if(m_hMapFile == NULL) return false;

	m_pAddress = (unsigned char *)MapViewOfFile(m_hMapFile, //ӳ���ڴ��ַ��ָ��
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		size);
	if(m_pAddress == NULL)
	{//ӳ��ʧ��
		CloseHandle(m_hMapFile);
		return false;
	}

	m_isInited = true;
	return true;
}
void _XShareMem::release()
{
	if(!m_isInited) return;
	UnmapViewOfFile(m_pAddress);
	CloseHandle(m_hMapFile);
	m_isInited = false;
}
#endif

#ifdef XEE_OS_LINUX
bool _XShareMem::init(const char *name,int size)
{
	if(m_isInited) return false;
	if(size <= 0) return false;
	m_size = size;
	void *shmaddr = NULL;
	if((m_shmid = shmget(IPC_PRIVATE,size,0666 | IPC_CREAT)) < 0)				//���������ڴ�
//	if((m_prsem = sem_open(name,O_CREAT,0666,0)) == SEM_FAILED) return false;	//�ź������ڿ��ƶ�д	//��
//	//if((m_prsem = sem_open(name,O_CREAT,0666,1)) == SEM_FAILED) return false;	//�ź������ڿ��ƶ�д	//д
	if((shmaddr = shmat(m_shmid,NULL,0)) == (void *)-1) return false;			//ӳ���ڴ��ַ
	m_pAddress = (unsigned char *)shmaddr;

	m_isInited = true;
	return true;
}
void _XShareMem::release()
{
	if(!m_isInited) return;
	shmctl(m_shmid,IPC_RMID,NULL);	//ɾ�������ڴ�
	m_isInited = false;
}
#endif