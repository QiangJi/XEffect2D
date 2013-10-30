//********************************************************************************
//������һ������Ŀ�ܣ���ܰ������ڵĽ������������¼��Ĵ������Լ�ͼ����Ⱦ����
//program by ��ʤ��
//********************************************************************************

#include "XEffeEngine.h"

#ifdef OS_WINDOWS
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	//���ؿ���̨�ı���˵��
#endif

//�������¼������������磺�����¼���������¼�
int SDL_event(SDL_Surface *screen);

int main(int argc, char **argv)
{
	Uint32 interval;	//���ε���֮���ʱ���			
	SDL_Surface *screen = NULL;	//���ڻ�ͼ��
	int quit = 0;
	//��������
	screen = initWindow(1024,768,"Xiajia");	
	if(screen == NULL)
	{
		printf("Window init error!\n");
		return 0;
	}
	while (!quit) 
	{
		quit = SDL_event(screen);
		interval = getFrameTiming();
		
		engineMove(interval);	//����ĸ�������
		clearScreen();	//�����Ļ

		updateScreen();	//������Ļ������
		
		SDL_Delay(1);
	}

	releaseSDL();
	return 0;	
}

int SDL_event(SDL_Surface *screen)
{
	int flag =0;
	SDL_Event event;		//SDL�¼����

	while(SDL_PollEvent(&event)) 
	{
		switch(event.type)
		{
		case SDL_QUIT:
			flag = 1;
			break;
		case SDL_KEYUP:
			break;
		case SDL_KEYDOWN:

			switch(event.key.keysym.sym )
			{
			case SDLK_s:
				break;
			case SDLK_LALT:
				break;
			case SDLK_RALT:
				break;
			case SDLK_F1:
				SDL_WM_ToggleFullScreen(screen);
				break;
			case SDLK_RIGHT:
				break;
			case SDLK_LEFT:
				break;
			case SDLK_UP:
				break;
			case SDLK_DOWN:
				break;
			}
			break;
		case SDL_MOUSEMOTION:
			break;
		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == 1)
			{//���
			}else
			if(event.button.button == 3)
			{//�Ҽ�
			}
			break;
		case SDL_MOUSEBUTTONUP:
			if(event.button.button == 1)
			{//���
			}else
			if(event.button.button == 3)
			{//�Ҽ�
			}
			break;
		}
		inputEvent(event);
	}
	return flag;
}