#ifndef _JIA_XINPUTEVENTSDL_
#define _JIA_XINPUTEVENTSDL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.25
//--------------------------------
#include "XInputEventCore.h"

//�����Ƿ���δ������¼�
extern bool SDLEventToInputEvent(_XInputEvent &event,const SDL_Event &tmpEvent);	//��SDL�������¼�ת����XEE�������¼�
inline bool getInputEventSDL(_XInputEvent &event)
{
	SDL_Event tmpEvent;		//SDL�¼����
	if(SDL_PollEvent(&tmpEvent) == 0) return false;
	//����ת���¼�
	return SDLEventToInputEvent(event,tmpEvent);
}

#endif