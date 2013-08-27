#ifndef _JIA_XCIRCLEGROUP_
#define _JIA_XCIRCLEGROUP_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
/****************************************
˵����
������ݽṹʹ�ù̶����ڴ��С
�������ڴ�ռ�
�Ƚ��ȳ�������˳��
�������������ۻ��������
���̰߳�ȫ
�Ż���
����������ַ����������������Ƶĸ��Ӷ���������Ҫ���ؿ��ǣ�
1��һ����������Դ��һ���������Դ�����ַ�ʽ������ʹ�û������������̰߳�ȫ
2�������������Դ������������Դ���������Ҫʹ�û��������ܱ�֤���̰߳�ȫ
���Խ����
1��һ����������Դ��һ���������Դ�����������ݣ����ַ�ʽ��ʹ�û������ǰ�ȫ��
2�������������Դ������������Դ���������Ҫʹ�û��������ܱ�֤���̰߳�ȫ
****************************************/
#include "XBasicClass.h"
#include "XOSDefine.h"
#ifdef OS_LINUX
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pthread.h>
#endif

template<class Type> class _XCircleGroup
{
private:
    char m_isInited;    //�Ƿ��ʼ��
    int m_insertPoint;    //��ǰ�Ĳ����
    int m_popPoint;        //��ǰȡ����
    int m_buffSize;        //����buff�Ĵ�С
    
    struct _XElement    //Ԫ�صĽṹ
    {
        bool isEnable;
        Type elementData;
    };
    _XElement *m_pElement;    //Ԫ�ض��е�ָ��
	pthread_mutex_t m_mutex;  
    int m_mode;            //ӵ�´���ģʽ��0��ӵ���������ϵ����� 1��ӵ�µȴ�
private:
	_XCircleGroup(const _XCircleGroup & temp)	//���ؿ������캯����ֹ����Ҫ�Ĵ���
	{
	}
public:
    _XCircleGroup()        //���캯��
    :m_isInited(0)
    ,m_insertPoint(0)    //��ǰ�Ĳ����
    ,m_popPoint(0)        //��ǰȡ����
    ,m_buffSize(0)        //����buff�Ĵ�С
    ,m_pElement(NULL)    //Ԫ�ض��е�ָ��
    ,m_mode(0)
    {
    }
	int init(int buffsize,int mode = 0)    //��ʼ�����������ڴ�ռ�
	{
		if(m_isInited != 0) return 0;    //����Ѿ���ʼ���������ظ���ʼ��
		if(buffsize <= 0) return 0;        //0�ߴ��ǲ��е�
		m_insertPoint = 0;                //��ǰ�Ĳ����
		m_popPoint = 0;                    //��ǰȡ����
		m_buffSize = buffsize;            //����buff�Ĵ�С
		m_pElement = NULL;                //Ԫ�ض��е�ָ��
		m_mode = mode;
		try
		{
			m_pElement = new _XElement[m_buffSize];
			if(m_pElement == NULL) return 0;    //�ڴ����ʧ��
		}catch(...)
		{
			return 0;    //�ڴ����ʧ��
		}
		//��ʼ����������
		for(int i = 0;i < m_buffSize;++ i)
		{
			m_pElement[i].isEnable = 0;
		}

		m_isInited = 1;
		return 1;
	}
	int insertOneElement(Type &element)    //����һ��Ԫ��
	{
		if(m_isInited == 0) return 0;
		pthread_mutex_lock(m_mutex);
		if(m_pElement[m_insertPoint].isEnable == 0)
		{//�����ݿ���ȡ������ȡ�����ݣ�������Ӧ�Ĵ���
			m_pElement[m_insertPoint].elementData = element;    //ע��������ʹ�õ��Ǹ�ֵ��������������ڲ���Ա����ָ������ǻ���ִ���
			m_pElement[m_insertPoint].isEnable = 1;
			++ m_insertPoint;
			if(m_insertPoint >= m_buffSize) m_insertPoint = 0;
			pthread_mutex_unlock(m_mutex);
			return 1;
		}else
		{
	//        printf("buff is full!");
			if(m_mode == 0)
			{
	//            printf(" some data will be demp!\n");
				//��buff�������ݶ���
				m_pElement[m_insertPoint].elementData = element;    //ע��������ʹ�õ��Ǹ�ֵ��������������ڲ���Ա����ָ������ǻ���ִ���
				m_pElement[m_insertPoint].isEnable = 1;
				++ m_insertPoint;
				if(m_insertPoint >= m_buffSize) m_insertPoint = 0;
				++ m_popPoint;
				if(m_popPoint >= m_buffSize) m_popPoint = 0;
			}
	//        printf("\n");
			pthread_mutex_unlock(m_mutex);
			return 2;
		}
	}
	int popOneElement(Type &element)    //ȡ��һ��Ԫ��
	{
		if(m_isInited == 0) return 0;
		pthread_mutex_lock(m_mutex);
		if(m_pElement[m_popPoint].isEnable != 0)
		{//�����ݿ���ȡ������ȡ�����ݣ�������Ӧ�Ĵ���
			element = m_pElement[m_popPoint].elementData;    //ע��������ʹ�õ��Ǹ�ֵ��������������ڲ���Ա����ָ������ǻ���ִ���
			m_pElement[m_popPoint].isEnable = 0;
			++ m_popPoint;
			if(m_popPoint >= m_buffSize) m_popPoint = 0;
			pthread_mutex_unlock(m_mutex);
			return 1;
		}
		pthread_mutex_unlock(m_mutex);
		return 0;
	}
    int isEmpty() const   //�Ƿ�Ϊ��
    {
        if(m_isInited == 0) return 1;
        if(m_pElement[m_popPoint].isEnable == 0) return 1;    //û�п���ȡ����������
        else return 0;
    }
	int isFull() const   //�Ƿ�Ϊ��
	{
		if(m_isInited == 0) return 0;
		if(m_pElement[m_insertPoint].isEnable != 0) return 1;    //û�п��Բ���Ŀռ���
		else return 0;
	}
};
#endif

/********************************************
//for test

#include "stdafx.h"
#include "windows.h"
#include "stdlib.h"
#include "math.h"
#include "XCircleGroup.h"

struct _data
{
    int data1[1024];
    _data& operator=(_data d) 
    {
        if ( &d == this ) return *this;
        for(int i = 0;i < 1024;++ i)
        {
            data1[i] = d.data1[i];
            Sleep(1);
        }
        return *this;
    }
};
_XCircleGroup <_data> tempBuff;

DWORD WINAPI thread_d(LPVOID hParameter)
{
    int data = 0;
    _data tempData;
    while(1)
    {
        //data = rand();
        //if(data < 0) data = -data;
        ++ data;
        for(int i = 0;i < 1024;++ i)
        {
            tempData.data1[i] = data;
        }
        if(tempBuff.insertOneElement(tempData) != 1)
        {
            -- data;
        }
        Sleep(1);
    }
    return 0;
}
DWORD WINAPI thread_e(LPVOID hParameter)
{
    int data = 0;
    _data tempData;
    while(1)
    {
        //data = rand();
        //if(data < 0) data = -data;
        ++ data;
        for(int i = 0;i < 1024;++ i)
        {
            tempData.data1[i] = data;
        }
        if(tempBuff.insertOneElement(tempData) != 1)
        {
            -- data;
        }
        Sleep(1);
    }
    return 0;
}
DWORD WINAPI thread_f(LPVOID hParameter)
{
    int data = 0;
    _data tempData;
    while(1)
    {
        //data = rand();
        //if(data < 0) data = -data;
        ++ data;
        for(int i = 0;i < 1024;++ i)
        {
            tempData.data1[i] = data;
        }
        if(tempBuff.insertOneElement(tempData) != 1)
        {
            -- data;
        }
        Sleep(1);
    }
    return 0;
}

//���Խ��������Ҫ���̺߳͸����߳��ж����������ܱ�֤���������ķ�Χ����ת��
int main(int avg,char* arg)
{
    HANDLE temp;
    DWORD dw;
    tempBuff.init(100,1);
    temp = CreateThread(NULL,NULL,thread_d,NULL,0,&dw);
    temp = CreateThread(NULL,NULL,thread_e,NULL,0,&dw);
    temp = CreateThread(NULL,NULL,thread_f,NULL,0,&dw);
    _data data;
    int oldData = 0;
    while(1)
    {
        if(tempBuff.popOneElement(&data) == 1)
        {
            for(int i = 1;i < 1024;++ i)
            {
                if(data.data1[0] != data.data1[i])
                {
                    printf("error!\n");
                }
            }
            oldData = data.data1[0];
        }
        Sleep(1);
    }
    return 1;
}

*********************************************/
