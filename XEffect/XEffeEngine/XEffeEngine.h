#ifndef _JIA_XEFFEENGINE_
#define _JIA_XEFFEENGINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//++++++++++++++++++++++++++++++++
/*˵��:
1���������ͨ��example09�ĳ������Դ�����Զ��Ż�������ע������ϵͳ����Դ����ͨ�������������Ż���
���򽫻��������ϵͳ����ͼ����ԭ����������ϵͳ��Ҫ�ϸߵ���ͼ�����ٶȣ�����ͼƬ��Դ���ܽ����Ż���
���򽫻���ʧЧ�ʣ��ۺϿ���֮��ȡ��������ϵͳ����Դ�����Ż�ƴ�ϡ�
2����Ҫע����ǣ���Դ�Ż�����Դ������loading���֮����ܱ����ã����������loading֮ǰ��ʾ����Դ��
������loading֮ʱʹ�õ���Դ��ò�Ҫ������Դ�Ż�������loading������û��ʲô���塣
3���������е���Դ����music��movie������ͨ��XResourcePack�������Դ���д���ͼ��ܣ�֮���������ֱ
�Ӵ���Դ������ȡ��Դ�ļ���
4����Ȼ����Ҳ֧����Դ���Ż�����֧Ԯ�����������ֱ��ʹ�á�
5��Ŀǰ��Դֻ��һ��loading���룬Ȼ����ʹ�ã�û�в��Թ���̬���룬֮����Ҫ���ԡ�
6��
*/
//--------------------------------

#include "glew.h"
#include <stdio.h>
#include <stdlib.h>

#include "XBasicOpenGL.h"

#include "SDL.h"
#include "SDL_image.h"

#include "XSprite.h"
#include "XFrame.h"
#include "XFont.h"
#include "XFontUnicode.h"
#include "XBasicSDL.h"
#include "XOSDefine.h"

#include "XNumber.h"
#include "XNumberEx.h"
#include "XSound.h"
#include "XMusic.h"
#include "XNodeLine.h"
#include "XBezierSpline.h"
#include "XBezierInterpolation.h"
#include "XLogBook.h"
#include "XString.h"
#include "XParticle/XParticles.h"
#include "XControl/XControls.h"
#include "XPhysics/XPhysicsBasic.h"
#include "tinyxml.h"

#include "XMovie.h"

#include "XEffeEngine.h"
#include "XBmp.h"
#include "XDataBasic.h"

#include "XTextureInformation.h"
#include "XFrameEx.h"
#include "X2DPacker.h"

#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif

extern SDL_Surface * initWindow(int width,int height,const char *windowTitle,int isFullScreen = 0,int isShowCursor = 1);	//��ʼ������

//�����������һЩ���ܺ���
extern void XSetStop(int temp = -1);	//ֹͣ 0,play,1,stop,-1,change state
extern int XGetStopState();
extern void XSetFPS(int temp);		//������Ϸ֡�ʣ�С��0Ϊ������֡��(Ŀǰ�����������û����Ӧ�е�����)
extern int XEngineIdle();		//�������ѭ����������ѭ���¼�����
#endif
