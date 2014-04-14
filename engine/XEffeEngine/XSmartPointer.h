#ifndef _JIA_XSMARTPOINTER_
#define _JIA_XSMARTPOINTER_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2013.1.15
//--------------------------------
#include "stdio.h"
#include "XBasicFun.h"
//����������
template<class T> class _XSmartP;

template<class T> class _XBackP  
{  
private:
    friend _XSmartP<T>;  
    T *m_p;			//ʵ�ʵ�ָ��
    size_t m_counter;			//���ü�����
    _XBackP(T *p) 
		:m_p(p)
		,m_counter(1)  
    {  
	//	printf("_XBackP constructor called !\n");  
    }  
public:
    ~_XBackP()  
    {  
        XDELETE(m_p);  
	//	printf( "_XBackP distructor called !\n");  
    }  
};  
  
template<class T> class _XSmartP  
{  
public:  
    // ���캯����p��ָ���Ѿ���̬������T����ָ��  
    _XSmartP(T *p) 
		:m_backP(new _XBackP<T>(p))
    {  
	//	printf("_XSmartP constructor called ! use = %d\n",m_backP->m_counter); 
    }  
    _XSmartP() 
		:m_backP(new _XBackP<T>(new T()))
    {  
	//	printf("_XSmartP constructor called ! use = %d\n",m_backP->m_counter); 
    }  
  
    // ���ƹ��캯�������Ƴ�Ա����ʹ�ü�����1  
    _XSmartP(const _XSmartP& temp) 
		:m_backP(temp.m_backP)
    {  
		++m_backP->m_counter;    
	//	printf("_XSmartP copy constructor called ! use = %d\n",m_backP->m_counter); 
    }  
    // ��ֵ������  
    _XSmartP<T>& operator=(const _XSmartP<T>&temp)
	{  
		if(this == &temp) return *this;		//��ֹ��������
		// �����Ҳ������е�ʹ�ü���  
		++temp.m_backP->m_counter;  
		// ��������������ʹ�ü�����1�����ö����ʹ�ü�������0����ɾ���ö���  
		if (--m_backP->m_counter == 0)  
		{
			XDELETE(m_backP);
		}
		m_backP = temp.m_backP;   // ����m_backPָ��  
		return *this;  
	}   
    // �����������������Ϊ0����ɾ��m_backP����  
    ~_XSmartP()  
    {  
	//	printf("_XSmartP distructor called ! use = %d\n",m_backP->m_counter); 
		if (--m_backP->m_counter == 0)  
		{
			XDELETE(m_backP);
		}
    }  
    // ��ȡ���ݳ�Ա  
    T *getPtr() const  
    {  
        return m_backP->m_p;  
    } 
//	// �޸����ݳ�Ա  
//	void setPtr(T *p) const  
//	{  
//		m_backP->m_p = p;  
//	}
    // ���ػ��޸Ķ���  
    T getVal() const  
    {  
        return *m_backP->m_p;  
    }  
    void setVal(T val)  
    {  
        *m_backP->m_p = val;  
    }  
private:  
    _XBackP<T> *m_backP;   //ָ��ʹ�ü�����m_backP 
};  

//�����ǹ�������Ķ���
template<class T> class _XSmartPArray;

template<class T> class _XBackPArray  
{  
private:
    friend _XSmartPArray<T>;  
    T *m_p;			//ʵ�ʵ�ָ��
    size_t m_counter;			//���ü�����
    _XBackPArray(T p[]) 
		:m_p(p)
		,m_counter(1)  
    {  
	//	printf("_XBackPArray constructor called !\n");  
    }  
public:
    ~_XBackPArray()  
    {  
        XDELETE_ARRAY(m_p);  
	//	printf( "_XBackPArray distructor called !\n");  
    }  
};  
  
template<class T> class _XSmartPArray  
{  
public:  
	_XSmartPArray()
		:m_backP(new _XBackPArray<T>(new T[2]))
		,m_size(2)
	{
	//	printf("_XSmartPArray constructor called ! use = %d\n",m_backP->m_counter); 
	}
    // ���캯����p��ָ���Ѿ���̬������int����ָ��  
    _XSmartPArray(T p[],int size) 
		:m_backP(new _XBackPArray<T>(p))
		,m_size(size)
    {  
	//	printf("_XSmartPArray constructor called ! use = %d\n",m_backP->m_counter); 
    }  
    _XSmartPArray(int size) 
		:m_backP(new _XBackPArray<T>(new T[size]))
		,m_size(size)
    {  
	//	printf("_XSmartPArray constructor called ! use = %d\n",m_backP->m_counter); 
    }  
    // ���ƹ��캯�������Ƴ�Ա����ʹ�ü�����1  
    _XSmartPArray(const _XSmartPArray& temp) 
		:m_backP(temp.m_backP)
		,m_size(temp.m_size)
    {  
		++m_backP->m_counter;    
	//	printf("_XSmartPArray copy constructor called ! use = %d\n",m_backP->m_counter); 
    }  
    // ��ֵ������  
    _XSmartPArray<T>& operator=(const _XSmartPArray<T>&temp)
	{  
		if(this == &temp) return *this;		//��ֹ��������
		// �����Ҳ������е�ʹ�ü���  
		++temp.m_backP->m_counter;  
		// ��������������ʹ�ü�����1�����ö����ʹ�ü�������0����ɾ���ö���  
		if (--m_backP->m_counter == 0)  
		{
			XDELETE(m_backP);
		}
		m_backP = temp.m_backP;   // ����m_backPָ��  
		m_size = temp.m_size;
		return *this;  
	}   
 	T& operator[](int order)const
	{
	//	if(order < 0 || order >= m_size) order = 0;
		return m_backP->m_p[order];
	}
 	T& operator[](int order)
	{
	//	if(order < 0 || order >= m_size) order = 0;
		return m_backP->m_p[order];
	}
    // �����������������Ϊ0����ɾ��m_backP����  
    ~_XSmartPArray()  
    {  
	//	printf("_XSmartPArray distructor called ! use = %d\n",m_backP->m_counter); 
		if (--m_backP->m_counter == 0)  
		{
			XDELETE(m_backP);
		}
    }  
    // ��ȡ���ݳ�Ա  
    T *getPtr() const  
    {  
        return m_backP->m_p;  
    } 
//	// �޸����ݳ�Ա  
//	void setPtr(T *p) const  
//	{  
//		m_backP->m_p = p;  
//	}
    // ���ػ��޸Ķ���  
    T getVal(int order) const  
    {  
		if(order < 0 || order >= m_size) order = 0;
		return m_backP->m_p[order];  
    }  
    void setVal(T val,int order)  
    {  
		if(order < 0 || order >= m_size) order = 0;
        m_backP->m_p[order] = val;  
    }  
private:  
    _XBackPArray<T> *m_backP;   //ָ��ʹ�ü�����m_backP 
	int m_size;
}; 
#endif