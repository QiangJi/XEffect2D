#ifndef _JIA_XBASICWINDOW_
#define _JIA_XBASICWINDOW_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XBasicFun.h"
#include "XCommonDefine.h"
#include "XInputEventCommon.h"

class _XFColor;
namespace XEE
{ 
extern int windowWidth;		//��ǰ���ڵĿ��
extern int windowHeight;	//��ǰ���ڵĸ߶�
extern int sceneX;			//������λ��
extern int sceneY;			//������λ��
extern int sceneWidth;		//�����Ŀ��
extern int sceneHeight;		//�����ĸ߶�
extern float sceneSizeX;		//���ڵ����ű���
extern float sceneSizeY;		//���ڵ����ű���
extern float viewAngle3D;		//3D�۲�ĽǶ�
extern _XWindowData windowData;	//���ڵ��������
extern _XResourcePosition defaultResourcePosition;

extern int frameTime;	//��һ֡��ʱ��
extern float nowFPS;	//��ǰ��֡��

extern int audioFrequency;	//��Ƶ��ʼ����Ƶ��
extern int audioFormat;		//��Ƶ�ĸ�ʽ
extern int audioChannel;	//��Ƶ������
extern _XVector2 mousePosition;	//��ǰ�����λ��

extern void release();
extern void inputEvent(const _XInputEvent& inputEvent);
extern void keyProc(int key,_XKeyState keyState);
extern void mouseProc(int x,int y,_XMouseState mouseState);
extern LRESULT (*customWinProc)(HWND hwnd,UINT Msg,WPARAM wParam,LPARAM lParam,bool &end);	//�Զ����ϵͳ��Ϣ������
extern HWND wHandle;
extern HDC wHDC;	//����������ָ��
extern HGLRC wCurrentHGLRC;	//��ǰ��������
extern HGLRC wCopyHGLRC;	//��ǰ�����ĵĸ���
extern unsigned int mainThreadHandle;	//���̵߳�ID

extern char isOpenGL2Support;	//OpenGL�Ƿ�֧��2.0�Ƿ�֧��GLSL
extern int maxTetureSize;		//�Կ�֧�ֵ������ͼ�ߴ�
extern char isLineSmooth;		//�Ƿ�ʹ�û��ߵ�ƽ������
extern _XFColor defaultBGColor;	//Ĭ�ϵı�����ɫ

}

extern bool createWindow(int width,int height,const char *windowTitle = NULL,int isFullScreen = 0,int withFrame = 1);	//��ʼ��SDL����

#endif