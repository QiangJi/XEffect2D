#ifndef _JIA_XMOUSEANDKEYBOARDDEFINE_
#define _JIA_XMOUSEANDKEYBOARDDEFINE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2012.12.10
//--------------------------------
//�����ǹ������ͼ��̵�һЩ������Ķ���
//enum _XMouseState
//{
//	_XMOUSE_STATE_MOVE,		//����ƶ�
//	_XMOUSE_STATE_DOWN,		//��갴��
//	_XMOUSE_STATE_UP		//��굯��
//};
//enum _XMouseKeyOrder
//{
//	_XMOUSE_KEY_ORDER_LEFT,		//������
//	_XMOUSE_KEY_ORDER_MIDDLE,	//����м�
//	_XMOUSE_KEY_ORDER_RIGHT,	//����Ҽ�
//	_XMOUSE_KEY_ORDER_NULL		//û�а���
//};
enum _XKeyState
{
	_XKEY_STATE_DOWN,		//��������
	_XKEY_STATE_UP			//��������
};
enum _XMouseState
{
	_XMOUSE_MOVE,				//����ƶ�
	_XMOUSE_LEFT_BUTTON_DOWN,	//����������
	_XMOUSE_LEFT_BUTTON_UP,		//����������
	_XMOUSE_RIGHT_BUTTON_DOWN,	//����Ҽ�����
	_XMOUSE_RIGHT_BUTTON_UP,	//����Ҽ�����
	_XMOUSE_MIDDLE_BUTTON_DOWN,	//����м�����
	_XMOUSE_MIDDLE_BUTTON_UP,	//����м�����
};
#endif