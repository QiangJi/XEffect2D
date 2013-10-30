#ifndef _JIA_XBASICSDL_
#define _JIA_XBASICSDL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "SDL_image.h"

#define WITH_OBJECT_MANAGER (0)	//�Ƿ�ʹ���������

extern int XEE_windowWidth;		//��ǰ���ڵĿ��
extern int XEE_windowHeight;	//��ǰ���ڵĸ߶�
extern int XEE_frameTime;	//��һ֡��ʱ��
extern float XEE_nowFPS;	//��ǰ��֡��

extern int XEE_audioFrequency;	//��Ƶ��ʼ����Ƶ��
extern int XEE_audioFormat;		//��Ƶ�ĸ�ʽ
extern int XEE_audioChannel;	//��Ƶ������

extern SDL_Surface * loadImage(const char *pFileName,int alphaFlag = 0,int resourcesPosition = 0);	//����ͼƬ��Դ
extern SDL_Surface * loadImageEx(const char *pFileName,int resourcesPosition = 0);	//����ͼƬ��Դ
extern int isNPOT(int width, int height);									//�ж��Ƿ����2��n�η�

extern int getFrameTiming();											//������ʾ5���֡�ʣ����ҷ������ε���֮���ʱ���
extern SDL_Surface * initSDL(int width,int height,const char *windowTitle = NULL,int isFullScreen = 0);	//��ʼ��SDL����
extern void releaseSDL();
extern void inputEvent(const SDL_Event& inputEvent);
extern void engineMove(int delay);			//�����ڲ��Ķ�������

#endif
