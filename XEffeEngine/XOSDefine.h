#ifndef _JIA_OSDEFINE_
#define _JIA_OSDEFINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------

//#define DEBUG_MODE	//������Ҫ��������ʽ�汾��ͬ��ģ�鶼��Ҫ������ͷ�ļ�
//#define GET_ALL_PIXEL		//ͳ���������ͼ��������ڱ����Ϸ����ͼЧ��
#define OS_WINDOWS        //Windowsϵͳ��
//#define OS_LINUX        //Linuxϵͳ��

#if defined DLL_EXPORT
#define DECLDIR __declspec(dllexport)
#else
#define DECLDIR __declspec(dllimport)
#endif

#endif
