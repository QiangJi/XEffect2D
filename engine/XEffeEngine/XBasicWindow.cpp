//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XBasicWindow.h"
#include "XSoundCommon.h"
#include "XLogBook.h"
#include "XWindowCommon.h"

namespace XEE
{
	int frameTime = 0;
	float nowFPS = 0.0f;
	//SDL_Surface *screen = NULL;
	_XVector2 mousePosition;
	_XFColor defaultBGColor = _XFColor(0.25f,0.25f,0.25f,1.0f);	//Ĭ�ϵı�����ɫ

	int windowWidth = 0;	//��ǰ���ڵĿ��
	int windowHeight = 0;	//��ǰ���ڵĸ߶�
	int sceneX = 0;			//������λ��
	int sceneY = 0;			//������λ��
	int sceneWidth = 0;		//�����Ŀ��
	int sceneHeight = 0;	//�����ĸ߶�
	float sceneSizeX = 1.0f;
	float sceneSizeY = 1.0f;
	float viewAngle3D = 45.0f;
	_XWindowData windowData;
	int audioFrequency = 44100;//48000;
	int audioChannel = 2;
	int audioFormat = MIX_DEFAULT_FORMAT;
	_XResourcePosition defaultResourcePosition = RESOURCE_LOCAL_FOLDER;
	LRESULT (*customWinProc)(HWND hwnd,UINT Msg,WPARAM wParam,LPARAM lParam,bool &end) = NULL;
	HWND wHandle = 0;
	HDC wHDC = 0;	//����������ָ��
	HGLRC wCurrentHGLRC = 0;	//��ǰ��������
	HGLRC wCopyHGLRC = 0;	//��ǰ�����ĵĸ���
	unsigned int mainThreadHandle = 0xffffffff;

	char isOpenGL2Support = 0;
	int maxTetureSize = 1024;
	char isLineSmooth = 0;

	#include <wincon.h>
	extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow ();
	//�����Լ�����Ϣ������
	//����Ϣ�������´�
	WNDPROC currentWndProc;
	static LRESULT CALLBACK winMsgProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) 
	{
		if(customWinProc != NULL) 
		{
			bool end = false;
			LRESULT ret = customWinProc(hwnd,Msg,wParam,lParam,end);
			if(end) return ret;
		}
		return CallWindowProc(currentWndProc,hwnd,Msg,wParam,lParam);
	}
	//--------------------------------------
	static void registerMyMsgProc()
	{
		//DragAcceptFiles (wHandle, TRUE);	//�Ƿ������򴰿���ק�ļ�
		//����1
		//currentWndProc = (WNDPROC)SetWindowLongPtr(wHandle, GWL_WNDPROC, (long)winMsgProc);
		//����2
		currentWndProc = (WNDPROC)GetWindowLongPtr(wHandle, GWL_WNDPROC);
		SetWindowLongPtr(wHandle, GWL_WNDPROC, (long)winMsgProc);
	}
}
bool createWindow(int width,int height,const char *windowTitle,int isFullScreen,int withFrame)
{
	if(!_XWindow.createWindow(width,height,windowTitle,isFullScreen,withFrame)) return false;
	XEE::windowWidth = width;
	XEE::windowHeight = height;
	//��ʼ��һЩ����
	XEE::wHDC = wglGetCurrentDC();
	XEE::wCurrentHGLRC = wglGetCurrentContext();
	XEE::wCopyHGLRC = wglCreateContext(XEE::wHDC);
	//wglCopyContext(XEE::wCurrentHGLRC,XEE::wCopyHGLRC,GL_ALL_ATTRIB_BITS);
	wglShareLists(XEE::wCurrentHGLRC,XEE::wCopyHGLRC);
	XEE::wHandle = WindowFromDC(XEE::wHDC);
	XEE::mainThreadHandle = GetCurrentThreadId();

	XEE::registerMyMsgProc();

	if(windowTitle != NULL) _XWindow.setWindowTitle(windowTitle);	//���ô��ڱ���

	_XSoundHandle.init();
	return true;
}

#if WITH_OBJECT_MANAGER
#include "XObjectManager.h"
#endif
#include "XControl/XControlManager.h"
#include "XSound.h"
namespace XEE
{
	_XVector2 getMousePosFromWindow(int mouseX,int mouseY)	//ϵͳ����ռ��򴰿�����ռ����ת��
	{
		float x = 0,y = 0;
		switch(XEE::windowData.rotateMode)
		{
		case WINDOW_ROTATE_MODE_0:
			switch(XEE::windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				x = mouseX * XEE::sceneSizeX + XEE::sceneX;
				y = mouseY * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				x = (XEE::windowData.w - mouseX) * XEE::sceneSizeX + XEE::sceneX;
				y = mouseY * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				x = mouseX * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseY) * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				x = (XEE::windowData.w - mouseX) * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseY) * XEE::sceneSizeY + XEE::sceneY;
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_90:
			switch(XEE::windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				x = (XEE::windowData.w - mouseY) * XEE::sceneSizeX + XEE::sceneX;
				y = mouseX * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				x = mouseY * XEE::sceneSizeX + XEE::sceneX;
				y = mouseX * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				x = (XEE::windowData.w - mouseY) * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseX) * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				x = mouseY * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseX) * XEE::sceneSizeY + XEE::sceneY;
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_180:
			switch(XEE::windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				x = (XEE::windowData.w - mouseX) * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseY) * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				x = mouseX * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseY) * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				x = (XEE::windowData.w - mouseX) * XEE::sceneSizeX + XEE::sceneX;
				y = mouseY * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				x = mouseX * XEE::sceneSizeX + XEE::sceneX;
				y = mouseY * XEE::sceneSizeY + XEE::sceneY;
				break;
			}
			break;
		case WINDOW_ROTATE_MODE_270:
			switch(XEE::windowData.turnOverMode)
			{
			case WINDOW_TURNOVER_MODE_NULL:
				x = mouseY * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseX) * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR:
				x = (XEE::windowData.w - mouseY) * XEE::sceneSizeX + XEE::sceneX;
				y = (XEE::windowData.h - mouseX) * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_TTD:
				x = mouseY * XEE::sceneSizeX + XEE::sceneX;
				y = mouseX * XEE::sceneSizeY + XEE::sceneY;
				break;
			case WINDOW_TURNOVER_MODE_LTR_TTD:
				x = (XEE::windowData.w - mouseY) * XEE::sceneSizeX + XEE::sceneX;
				y = mouseX * XEE::sceneSizeY + XEE::sceneY;
				break;
			}
			break;
		}
		return _XVector2(x,y);
	}
	void release()
	{
		_XSound::GetInstance().release();
		_XSoundHandle.close();
		_XWindow.release();
	}
	void keyProc(int key,_XKeyState keyState)
	{
		_XControlManager::GetInstance().keyProc(key,keyState);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().keyProc(key,keyState);
#endif
	}
	void mouseProc(int x,int y,_XMouseState mouseState)
	{
		switch(mouseState)
		{
		case MOUSE_MOVE:
			XEE::mousePosition = getMousePosFromWindow(x,y);
			_XControlManager::GetInstance().mouseProc(mousePosition.x,mousePosition.y,MOUSE_MOVE);
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().mouseProc(mousePosition.x,mousePosition.y,MOUSE_MOVE);
#endif
			break;
		case MOUSE_LEFT_BUTTON_DOWN:
			{
				_XVector2 tempPos;
				if(XEE::windowData.isFullScreen != 0)
					tempPos = getMousePosFromWindow(XEE_SDL_getMouseX(),XEE_SDL_getMouseY());
				else
					tempPos = getMousePosFromWindow(x,y);
				_XControlManager::GetInstance().mouseProc(tempPos.x,tempPos.y,MOUSE_LEFT_BUTTON_DOWN);
#if WITH_OBJECT_MANAGER
				_XObjectManager::GetInstance().mouseProc(tempPos.x,tempPos.y,MOUSE_LEFT_BUTTON_DOWN);
#endif
			}
			break;
		case MOUSE_LEFT_BUTTON_UP:
			{
				_XVector2 tempPos = getMousePosFromWindow(x,y);
				_XControlManager::GetInstance().mouseProc(tempPos.x,tempPos.y,MOUSE_LEFT_BUTTON_UP);
#if WITH_OBJECT_MANAGER
				_XObjectManager::GetInstance().mouseProc(tempPos.x,tempPos.y,MOUSE_LEFT_BUTTON_UP);
#endif
			}
			break;
		case MOUSE_RIGHT_BUTTON_DOWN:
			{
				_XVector2 tempPos;
				if(XEE::windowData.isFullScreen != 0)
					tempPos = getMousePosFromWindow(XEE_SDL_getMouseX(),XEE_SDL_getMouseY());
				else
					tempPos = getMousePosFromWindow(x,y);
				_XControlManager::GetInstance().mouseProc(tempPos.x,tempPos.y,MOUSE_RIGHT_BUTTON_DOWN);
			}
			break;
		case MOUSE_RIGHT_BUTTON_UP:
			{
				_XVector2 tempPos = getMousePosFromWindow(x,y);
				_XControlManager::GetInstance().mouseProc(tempPos.x,tempPos.y,MOUSE_RIGHT_BUTTON_UP);
			}
			break;
		case MOUSE_MIDDLE_BUTTON_DOWN:
			break;
		case MOUSE_MIDDLE_BUTTON_UP:
			break;
		}
	}

	void inputEvent(const _XInputEvent& inputEvent)
	{
		switch(inputEvent.type)
		{
		case EVENT_KEYBOARD:
			{
				switch(inputEvent.keyState)
				{
				case KEY_STATE_DOWN:	//���̰���
#ifdef XEE_OS_WINDOWS	//windowsϵͳ��ͨ�����뷨�������ַ�
					if(inputEvent.unicode != 0)
					{//�ַ�����
						char temp[2];
						memcpy(temp,&inputEvent.unicode,2);
						if(temp[0] == 0) _XControlManager::GetInstance().insertChar(&temp[1],1); else
						if(temp[0] < 128 && temp[1] == 0) 
						{
							if(temp[0] >= ' ' && temp[0] <= '~') _XControlManager::GetInstance().insertChar(temp,1);
						}
						else _XControlManager::GetInstance().insertChar(temp,2);
					}
#endif
					keyProc(inputEvent.keyValue,KEY_STATE_DOWN);
					break;
				case KEY_STATE_UP:	//���̵���
					keyProc(inputEvent.keyValue,KEY_STATE_UP);
					break;
				}
			}
			break;
		case EVENT_MOUSE:
			mouseProc(inputEvent.mouseX,inputEvent.mouseY,inputEvent.mouseState);
			break;
		}
	}
}//namespace XEE