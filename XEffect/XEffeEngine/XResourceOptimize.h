#ifndef _JIA_XRESOURCEOPTIMIZE_
#define _JIA_XRESOURCEOPTIMIZE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//#include "XTextureInformation.h"

#define PNG_FILE_LIST_NAME "pngFileList.txt"
#define PNG_INFORMATION_FILE_NAME "textureInformation.txt"

//��Դ�Ż��ʹ��ͳһ����֮������Ż������һ��ͼ�����Ż��������Ҫ���洦��
#define MAX_PIXELS_OFFSET (0)	//ͼƬ�Ż�ʱ�����ͼƬ���ص��ݲ�
extern int ResourceOptimize();
extern void releaseTempFile();	//�������ʱ�ļ�
extern void clearTempFile();	//�������ʱ�ļ�
extern int pixelCompare(const unsigned int *p0,const unsigned int *p1,int maxPixelsOffset);

#endif