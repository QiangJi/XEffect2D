#ifndef _JIA_OSDEFINE_
#define _JIA_OSDEFINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
#define XEE_OS_WINDOWS        //Windowsϵͳ��
//#define XEE_OS_LINUX        //Linuxϵͳ��

//ֻ��������ӹ����޳�һЩ����Ĵ��룬�������������ط����κ����Ƶ����
#pragma warning(disable: 4244)    //conversion from 'int' to 'float', possible loss of data  
#pragma warning(disable: 4267)    //conversion from 'size_t' to 'int', possible loss of data 
#pragma warning(disable: 4100)    //unreferenced formal parameter
#pragma warning(disable: 4245)    //conversion from 'int' to 'unsigned int', signed/unsigned mismatch
#pragma warning(disable: 4127)    //conditional expression is constant
#pragma warning(disable: 4018)    //signed/unsigned mismatch
#pragma warning(disable: 4996)    //'sscanf' was declared deprecated

//�����еı��뻷������û�ж���_XBool���ͣ�����������һЩͳһ���Ĵ���
#include "windows.h"
#if 1
//typedef BOOL _XBool;
//#define XFalse (FALSE)
//#define XTrue (TRUE)
typedef int _XBool;
#define XFalse (0)
#define XTrue (1)
#else
typedef bool _XBool;
#define XFalse (false)
#define XTrue (true)
#endif

#if defined DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

#endif
