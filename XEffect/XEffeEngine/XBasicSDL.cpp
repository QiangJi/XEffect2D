//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBasicSDL.h"
#include "math.h"
#include "SDL_mixer.h"
#include "XBasicFun.h"
#include "XResourcePack.h"
#include "XLogBook.h"

SDL_Surface * loadImage(const char *pFileName,int alphaFlag,int resourcesPosition)
{
	SDL_Surface *temp;
	SDL_Surface *temp_back = NULL;
	if(resourcesPosition == 0)
	{
		temp_back = IMG_Load(pFileName);
	}else
	{//�����ڲ���Դ�Ķ�ȡ
		//д������
		int length = _XResourcePack::GetInstance().getFileLength(pFileName);
		if(length < 0) return NULL;
		unsigned char *p = NULL;
		try
		{
			p = new unsigned char[length + 1];
			if(p == NULL) return NULL;
		}catch(...)
		{
			return NULL;
		}
		if(_XResourcePack::GetInstance().unpackResource(pFileName,p) != 1)
		{
			//printf("LoadIMG Error!\n");
			AddLogInfoStr("LoadIMG Error!\n");
			return NULL;
		}
		SDL_RWops *fileData = SDL_RWFromMem(p,length);
		temp_back = IMG_Load_RW(fileData,1);
		//temp_back = IMG_Load(rw,1);
		XDELETE_ARRAY(p);
		//SDL_FreeRW(fileData);
	}
    if(temp_back == NULL)
    {
		AddLogInfoNull("%s file load error!\n",pFileName);
    	return NULL;
    }

	if(alphaFlag != 0)
	{
    	SDL_SetAlpha(temp_back, SDL_RLEACCEL | SDL_SRCALPHA, 255);
	}
    temp = NULL;
    if(0 == alphaFlag)
    {
    	temp = SDL_DisplayFormat(temp_back);
    }else
    {
    	temp = SDL_DisplayFormatAlpha(temp_back);
    }
    if(NULL == temp)
    {
		AddLogInfoNull("%s file change error!\n",pFileName);
    	return NULL;
    }
    SDL_FreeSurface(temp_back);
	temp_back = NULL;

    return temp;
}

SDL_Surface * loadImageEx(const char *pFileName,int resourcesPosition)
{
	SDL_Surface *temp_back = NULL;
	if(resourcesPosition == 0)
	{
		temp_back = IMG_Load(pFileName);
	}else
	{//�����ڲ���Դ�Ķ�ȡ
		//д������
		int length = _XResourcePack::GetInstance().getFileLength(pFileName);
		if(length < 0) return NULL;
		unsigned char *p = NULL;
		try
		{
			p = new unsigned char[length + 1];
			if(p == NULL) return NULL;
		}catch(...)
		{
			return NULL;
		}
		//printf("%d\n",length);
		if(_XResourcePack::GetInstance().unpackResource(pFileName,p) != 1)
		{
			//printf("LoadIMG Error!\n");
			AddLogInfoStr("LoadIMG Error!\n");
			return NULL;
		}
		SDL_RWops *fileData = SDL_RWFromMem(p,length);
		if(fileData == NULL)
		{
			XDELETE_ARRAY(p);
			return NULL;
		}else
		{
			temp_back = IMG_Load_RW(fileData,1);
			//temp_back = IMG_Load(rw,1);
			XDELETE_ARRAY(p);
			//SDL_FreeRW(fileData);
		}
	}
    if(temp_back == NULL)
    {
		AddLogInfoNull("%s file load error!\n",pFileName);
    	return NULL;
    }
    return temp_back;
}


int isNPOT(int width, int height)
{
	if((int)powf(2.0f,ceilf(logf((float)width)/logf(2.0f))) != width) return 1;
	if((int)powf(2.0f,ceilf(logf((float)height)/logf(2.0f))) != height) return 1;
	else return 0;
}

int XEE_frameTime = 0;
float XEE_nowFPS = 0.0f;

int getFrameTiming()	//����֡�ʣ�����ʾÿ5���֡��
{
	static Uint32 current = 0,last = 0,five = 0,nframes = 0;

	current = SDL_GetTicks();
	++nframes;

	if(current - five > 5000) 
	{//��֡����ʾ����
		XEE_nowFPS = nframes/5.0f;
//#ifdef DEBUG_MODE
		//cout<<nframes<<" frames in 5 seconds = "<<nframes/5.0f<<" FPS"<<endl;
		printf("%u frames in 5 seconds = %.1f FPS\n",nframes,XEE_nowFPS);
//#endif
		nframes = 0;
		five = current;
	}
	
	XEE_frameTime = current - last;
	last = current;
	
	return XEE_frameTime;	//�������ε���֮���ʱ���
}


int XEE_windowWidth = 0;		//��ǰ���ڵĿ��
int XEE_windowHeight = 0;	//��ǰ���ڵĸ߶�
int XEE_audioFrequency = 48000;
int XEE_audioChannel = 2;
int XEE_audioFormat = MIX_DEFAULT_FORMAT;
SDL_Surface * initSDL(int width,int height,const char *windowTitle,int isFullScreen)
{
	SDL_Surface *screen;
	const SDL_VideoInfo* info = NULL;	//��ʾ�豸��Ϣ
	int bpp = 0;			//����ɫ��
	int flags = 0;			//ĳ�����

	// ----- SDL init --------------- 
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) 
	//if(SDL_Init(SDL_INIT_VIDEO) < 0) 
	{//��ʼ��SDL
		fprintf(stderr,"Video initialization failed: %s\n",SDL_GetError());
		AddLogInfoNull("Video initialization failed: %s\n",SDL_GetError());
		return NULL;
	}
	atexit(SDL_Quit);				//�˳�ʱ�˳�SDL
	
	info = SDL_GetVideoInfo();		//��ô����豸��Ϣ
	bpp = info->vfmt->BitsPerPixel;	//�����ʾɫ��
	
	// ----- OpenGL attribute setting via SDL --------------- 
	//SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 4);		//���ú�ɫλ��
	//SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 4);		//������ɫλ��
	//SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 4);		//������ɫλ��
	//SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 4);		//������ɫλ��
	//bpp = 16;
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	//����˫����
	
	if(isFullScreen != 0)
	{
		flags = SDL_OPENGL | SDL_FULLSCREEN;			//����Ϊȫ��
	}else
	{
		flags = SDL_OPENGL;								//���ڱ�־��
	}
	if((screen = SDL_SetVideoMode(width, height, bpp, flags)) == 0) 
	{//��ʼ��������Ϣ
		fprintf(stderr, "Video mode set failed: %s\n", SDL_GetError());
		AddLogInfoNull("Video mode set failed: %s\n", SDL_GetError());
        return NULL;
	}
	XEE_windowWidth = width;
	XEE_windowHeight = height;

	if(windowTitle != NULL) SDL_WM_SetCaption(windowTitle, NULL);	//���ô��ڱ���

//��ʼ�������豸(Music Sound ��Movie�࣬��߶��������豸���ڳ�ͻ������Ҫ��ռ)
//	XEE_audioFrequency = MIX_DEFAULT_FREQUENCY;
	XEE_audioFrequency = 44100;	
    XEE_audioFormat = MIX_DEFAULT_FORMAT;
    XEE_audioChannel = 2;
    const int TMP_CHUNK_SIZE = 1024;// + 192000 * 3 /2;

    Mix_OpenAudio(XEE_audioFrequency,XEE_audioFormat,XEE_audioChannel,TMP_CHUNK_SIZE);
	Mix_AllocateChannels(128);
	return screen;
}

void releaseSDL()
{
	Mix_CloseAudio();
	//SDL_CloseAudio();
	SDL_Quit();
}

#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif
#include "XControl/XControlManager.h"

void inputEvent(const SDL_Event& inputEvent)
{
	switch(inputEvent.type)
	{
	case SDL_KEYDOWN:	//���̰���
		_XControlManager::GetInstance().keyProc((int)(inputEvent.key.keysym.sym),_XKEY_STATE_DOWN);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().keyProc((int)(inputEvent.key.keysym.sym),_XKEY_STATE_DOWN);
#endif
		break;
	case SDL_KEYUP:	//���̵���
		_XControlManager::GetInstance().keyProc((int)(inputEvent.key.keysym.sym),_XKEY_STATE_UP);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().keyProc((int)(inputEvent.key.keysym.sym),_XKEY_STATE_UP);
#endif
		break;
	case SDL_MOUSEMOTION:
		_XControlManager::GetInstance().mouseProc(inputEvent.button.x,inputEvent.button.y,_XMOUSE_MOVE);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().mouseProc(inputEvent.button.x,inputEvent.button.y,_XMOUSE_MOVE);
#endif
		break;
	case SDL_MOUSEBUTTONDOWN:
		if(inputEvent.button.button == 1)
		{//���
			_XControlManager::GetInstance().mouseProc(inputEvent.button.x,inputEvent.button.y,_XMOUSE_LEFT_BUTTON_DOWN);
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().mouseProc(inputEvent.button.x,inputEvent.button.y,_XMOUSE_LEFT_BUTTON_DOWN);
#endif
		}else
		if(inputEvent.button.button == 2)
		{//�Ҽ�
			_XControlManager::GetInstance().mouseProc(inputEvent.button.x,inputEvent.button.y,_XMOUSE_RIGHT_BUTTON_DOWN);
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if(inputEvent.button.button == 1)
		{//���
			_XControlManager::GetInstance().mouseProc(inputEvent.button.x,inputEvent.button.y,_XMOUSE_LEFT_BUTTON_UP);
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().mouseProc(inputEvent.button.x,inputEvent.button.y,_XMOUSE_LEFT_BUTTON_UP);
#endif
		}else
		if(inputEvent.button.button == 3)
		{//�Ҽ�
			_XControlManager::GetInstance().mouseProc(inputEvent.button.x,inputEvent.button.y,_XMOUSE_RIGHT_BUTTON_UP);
		}
		break;
	}
}

void engineMove(int delay)
{
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().move(delay);
#endif
#ifdef GET_ALL_PIXEL
			XEE_AllPixel = 0;
#endif

}