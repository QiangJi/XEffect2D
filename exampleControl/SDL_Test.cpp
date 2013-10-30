//********************************************************************************
//����һ��������������������
//program by ��ʤ��
//********************************************************************************

#include "XEffeEngine.h"
#include "XControl/XControlManager.h"

#ifdef OS_WINDOWS
//#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )	//���ؿ���̨�ı���˵��
#endif

int SDL_event(SDL_Surface *screen);	//�������¼������������磺�����¼���������¼�

_XButton Button;		//��ť
_XButton ButtonCopy;
_XButtonEx ButtonEx;	//���ⰴť
_XButtonEx ButtonExCopy;
_XCheck Check;			//��ѡ��
_XCheck CheckCopy;
_XEdit Edit;			//�����
_XEdit EditCopy;
_XMouseRightButtonMenu MouseRightButtonMenu; //�������Ҽ��˵�
_XSlider Slider;		//������
_XSlider SliderCopy;
_XRadios Radios;		//��ѡ��
_XRadios RadiosCopy;	
_XProgress Progress;	//������
_XProgress ProgressCopy;
_XMultiText MutiText;	//�����ı���ʾ
_XMultiText MutiTextCopy;
_XMultiList MutiList;	//�����б��
_XCombo Combo;			//�����˵�
_XCombo ComboCopy;

void sliderChange(void *pClass,int ID)
{
	Progress.setValue(((_XSlider *)pClass)->getNowValue());
	ProgressCopy.setValue(100.0f - ((_XSlider *)pClass)->getNowValue());
}
void cbMouseOn(void *pClass,int ID)
{
	printf("MouseOn ID:%d\n",ID);
}

void cbMouseDown(void *pClass,int ID)
{
	printf("MouseDown ID:%d\n",ID);
}

void cbMouseUp(void *pClass,int ID)
{
	printf("MouseUp ID:%d\n",ID);
}

int main(int argc, char **argv)
{
	Uint32 interval;	//���ε���֮���ʱ���			
	SDL_Surface *screen = NULL;	//���ڻ�ͼ��
	int quit = 0;

	//��������
	screen = initWindow(1280,720,"Xiajia");	
	if(screen == NULL)
	{
		printf("Window init error!\n");
		return 0;
	}
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().init();
#endif
	//��ʼ������
	_XFontUnicode FontUnicode;
	FontUnicode.init("pic/Font/FontUnicode30_T000.png",_XVector2I(30,30),_XVector2I(34,34),4);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&FontUnicode);
#endif
	//��ʼ����ť
	_XButtonTexture ButtonTexture;
	ButtonTexture.init("pic/Button/B_00.png","pic/Button/B_01.png","pic/Button/B_02.png","pic/Button/B_03.png");

	Button.init(_XVector2(0.0f,30.0f),_XRect(10.0f,7.0f,119.0f,58.0f),ButtonTexture,"xiajia",FontUnicode,0.5f,_XVector2(64.0f,32.0f));
	Button.setCallbackFun(NULL,NULL,cbMouseOn,cbMouseDown,cbMouseUp,NULL);
	Button.setSize(1.0f,1.0f);
	Button.setCaptionText("��ð�");
	Button.setHotKey(SDLK_0);

	ButtonCopy.setACopy(Button);
	ButtonCopy.setPosition(0.0f,100.0f);
	ButtonCopy.setCaptionText("��ø�ƨ");
	ButtonCopy.disable();

	_XButtonTexture ButtonTextureEx;
	ButtonTextureEx.init("pic/ButtonEx/B_00.png","pic/ButtonEx/B_01.png","pic/ButtonEx/B_02.png","pic/ButtonEx/B_03.png");
	_XVector2 ButtonExArea[6];
	ButtonExArea[0].set(38.0f,20.0f);
	ButtonExArea[1].set(13.0f,64.0f);
	ButtonExArea[2].set(38.0f,109.0f);
	ButtonExArea[3].set(91.0f,109.0f);
	ButtonExArea[4].set(116.0f,64.0f);
	ButtonExArea[5].set(91.0f,20.0f);
	ButtonEx.init(_XVector2(150.0f,30.0f),ButtonExArea,6,ButtonTextureEx,"��",1.0f,_XVector2(64.0f,64.0f),FontUnicode);
	ButtonExCopy.setACopy(ButtonEx);
	ButtonExCopy.setPosition(280.0f,30.0f);
	ButtonExCopy.setCaptionText("��");
	//��ʼ����ѡ��
	_XCheckTexture CheckTexture;
	CheckTexture.init("pic/Check/C_00.png","pic/Check/C_01.png","pic/Check/CD_00.png","pic/Check/CD_01.png");
	Check.init(_XVector2(0.0f,160.0f),_XRect(18.0f,19.0f,42.0f,43.0f),CheckTexture,"��ѡ��",FontUnicode,1.0f,_XVector2(45.0f,32.0f));
	CheckCopy.setACopy(Check);
	CheckCopy.setPosition(150.0f,160.0f);
	CheckCopy.setText("��ѡ����");
	CheckCopy.setSize(0.5f,0.5f);
	//��ʼ���Ҽ��˵�
	_XMouseRightButtonMenuTexture MouseRightButtonMenuTexture;
	MouseRightButtonMenuTexture.init("pic/RightButtonMenu/00.png","pic/RightButtonMenu/01.png","pic/RightButtonMenu/02.png","pic/RightButtonMenu/03.png");
	MouseRightButtonMenu.init(4,_XVector2(0.0f,0.0f),_XRect(8.0f,2.0f,252.0f,49.0f),MouseRightButtonMenuTexture,FontUnicode,1.0,_XVector2(133.0f,25.0f));
	MouseRightButtonMenu.setText("����(T)",0);
	MouseRightButtonMenu.setText("����(C)",1);
	MouseRightButtonMenu.setText("ճ��(V)",2);
	MouseRightButtonMenu.setText("����(S)",3);
	MouseRightButtonMenu.setHotKey(SDLK_t,0);
	MouseRightButtonMenu.setHotKey(SDLK_c,1);
	MouseRightButtonMenu.setHotKey(SDLK_v,2);
	MouseRightButtonMenu.setHotKey(SDLK_s,3);
	//MouseRightButtonMenu.setCallbackFun(NULL,NULL);
	MouseRightButtonMenu.setSize(0.5f,0.5f);
	_XControlManager::GetInstance().decreaseAObject(&MouseRightButtonMenu);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&MouseRightButtonMenu);
#endif
	//��ʼ�������
	_XEditTexture EditTexture;
	EditTexture.init("pic/Edit/E_back.png","pic/Edit/E_disable.png","pic/Edit/E_Select.png","pic/Edit/E_Insert.png","pic/Edit/E_upon.png");
	FontUnicode.setAlignmentModeX(_FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	FontUnicode.setColor(0.0f,0.0f,0.0f,1.0f);
	Edit.init(_XVector2(400.0f,50.0f),_XRect(12.0f,2.0f,509.0f,38.0f),EditTexture,"Test",FontUnicode,1.0f,&MouseRightButtonMenu);
	//Edit.disable();
	//Edit.setCallbackFun(NULL,eInputOver,&Edit);
	Edit.setString("This is a test for setString().");
	EditCopy.setACopy(Edit);
	EditCopy.setPosition(400.0f,100.0f);
	EditCopy.setSize(0.5f,0.5f);
	EditCopy.setTextColor(_XFColor(1.0f,1.0f,1.0f,1.0f));
	//��ʼ��������
	_XSliderTexture SliderTexture;
	//��ֱ������
	//SliderTexture.init("pic/Slider/BLH_00.png","pic/Slider/BLH_01.png","pic/Slider/BLH_02.png","pic/Slider/BLH_03.png",
	//"pic/Slider/BH_00.png","pic/Slider/BH_01.png","pic/Slider/BH_02.png","pic/Slider/BH_03.png");
	//Slider.init(_XVector2(50.0f,250.0f),_XRect(6.0f,11.0f,32.0f,506.0f),_XRect(5.0f,4.0f,27.0f,26.0f),SliderTexture,_XSLIDER_TYPE_VERTICAL,100.0f,0.0f);
	//ˮƽ������
	SliderTexture.init("pic/Slider/BL_00.png","pic/Slider/BL_01.png","pic/Slider/BL_02.png","pic/Slider/BL_03.png",
	"pic/Slider/B_00.png","pic/Slider/B_01.png","pic/Slider/B_02.png","pic/Slider/B_03.png");
	Slider.init(_XVector2(0.0f,250.0f),_XRect(11.0f,0.0f,506.0f,26.0f),_XRect(4.0f,5.0f,27.0f,26.0f),SliderTexture,_XSLIDER_TYPE_HORIZONTAL,100.0f,0.0f);
	Slider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,sliderChange,NULL,&Slider);
	Slider.setNowValue(55.0f);

	SliderCopy.setACopy(Slider);
	SliderCopy.setPosition(0.0f,300.0f);
	SliderCopy.setSize(0.5f,0.5f);
	//�����ǵ�ѡ��
	Radios.init(5,_XVector2(0.0f,35.0f),_XVector2(00.0f,350.0f),_XRect(18.0f,19.0f,42.0f,43.0f),&CheckTexture,FontUnicode,1.0f,_XVector2(45.0f,32.0f));
	Radios.setRadioText("����ĺܵ���",0);
	Radios.setRadioText("�ҵ��۶���Ҫ����",1);
	Radios.setRadioText("��Ҫ����",2);
	Radios.setRadioText("���Ѿ�����",3);
	Radios.setRadioText("I have Boom",4);
	Radios.setChoosed(3);

	RadiosCopy.setACopy(Radios);
	RadiosCopy.setPosition(300.0f,350.0f);
	RadiosCopy.setRadioSum(8);
	RadiosCopy.setSize(0.5f,0.5f);
	RadiosCopy.setRadioText("��������",0);
	RadiosCopy.setRadioText("������",1);
	RadiosCopy.setRadioText("����ʼ����",2);
	RadiosCopy.setRadioText("��û�仯",3);
	RadiosCopy.setRadioText("���仵��",4);
	RadiosCopy.setRadioText("������ʹ",5);
	RadiosCopy.setRadioText("��Ҫ����",6);
	RadiosCopy.setRadioText("���Ѿ�����",7);
	//�����ǽ�����
	_XNumber Number;
	Number.init("pic/Number.png",_XVector2I(15,30),_XVector2I(8,2));
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&Number);
#endif
	_XProgressTexture ProgressTexture;
	ProgressTexture.init("pic/Progress/PB_00.png","pic/Progress/PM_00.png","pic/Progress/PU_00.png");
	Progress.init(_XVector2(0.0f,550.0f),_XRect(2.0f,0.0f,509.0f,30.0f),ProgressTexture,&Number,1.0f,_XVector2(256.0f,3.0f));
	Progress.setValue(25.0f);

	ProgressCopy.setACopy(Progress);
	ProgressCopy.setPosition(0.0f,600.0f);
	ProgressCopy.setSize(0.5f,0.5f);
	ProgressCopy.setColor(_XFColor(1.0f,1.0f,1.0f,0.5f));
	//�����Ƕ����ı���ʾ
	_XSliderTexture SliderTextureH;
	SliderTextureH.init("pic/MutiText/BL_00.png","pic/MutiText/BL_01.png","pic/MutiText/BL_02.png","pic/MutiText/BL_03.png",
		"pic/MutiText/B_00.png","pic/MutiText/B_01.png","pic/MutiText/B_02.png","pic/MutiText/B_03.png");
	_XSlider SliderH;
	SliderH.init(_XVector2(200.0f,200.0f),_XRect(0.0f,0.0f,512.0f - 40.0f,32.0f),_XRect(0.0f,0.0f,32.0f,32.0f),SliderTextureH,_XSLIDER_TYPE_HORIZONTAL,100.0f,0.0f);
	_XControlManager::GetInstance().decreaseAObject(&SliderH);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&SliderH);
#endif
	_XSliderTexture SliderTextureV;
	SliderTextureV.init("pic/MutiText/BLH_00.png","pic/MutiText/BLH_01.png","pic/MutiText/BLH_02.png","pic/MutiText/BLH_03.png",
		"pic/MutiText/BH_00.png","pic/MutiText/BH_01.png","pic/MutiText/BH_02.png","pic/MutiText/BH_03.png");
	_XSlider SliderV;
	SliderV.init(_XVector2(200.0f,200.0f),_XRect(0.0f,0.0f,32.0f,512.0f - 40.0f),_XRect(0.0f,0.0f,32.0f,32.0f),SliderTextureV,_XSLIDER_TYPE_VERTICAL,100.0f,0.0f);
	_XControlManager::GetInstance().decreaseAObject(&SliderV);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&SliderV);
#endif
	char tempTest[] = "0 :jiashenghua\n1 :123456789012345678901234567890123456789\n2 :PI = 3.141592653589793\n3 :\n\n5 :don`t use Chinese\n6 :It is not supply for Chinese at now!\n7 :\n8 :\n9 :\n10:\n11:\n12:\n13:\n14:\n15:\n16:";
	_XMultiTextTexture MutiTextTexture;
	MutiTextTexture.init("pic/MutiText/E_back.png","pic/MutiText/E_disable.png","pic/MutiText/E_Select.png","pic/MutiText/E_Insert.png","pic/MutiText/E_up.png");
	MutiText.init(_XVector2(520.0f,150.0f),_XRect(4.0f,4.0f,508.0f,508.0f),MutiTextTexture,tempTest,FontUnicode,1.0f/*,MouseRightButtonMenu*/,SliderV,SliderH);
	MutiText.setSize(0.5f,0.5f);

	MutiTextCopy.setACopy(MutiText);
	MutiTextCopy.setSize(0.5f,0.5f);
	MutiTextCopy.setPosition(790.0f,150.0f);
	MutiTextCopy.setTextColor(_XFColor(1.0f,1.0f,1.0f,1.0f));
//	MutiTextCopy.disActive();
//	MutiTextCopy.disVisiable();
	//�����б��
	_XSliderTexture SliderTextureHX;
	SliderTextureHX.init("pic/MutiList/BL_00.png","pic/MutiList/BL_01.png","pic/MutiList/BL_02.png","pic/MutiList/BL_03.png",
		"pic/MutiList/B_00.png","pic/MutiList/B_01.png","pic/MutiList/B_02.png","pic/MutiList/B_03.png");
	_XSlider SliderHX;
	SliderHX.init(_XVector2(200.0f,200.0f),_XRect(0.0f,0.0f,512.0f - 40.0f,32.0f),_XRect(0.0f,0.0f,32.0f,32.0f),SliderTextureHX,_XSLIDER_TYPE_HORIZONTAL,100.0f,0.0f);
	_XControlManager::GetInstance().decreaseAObject(&SliderHX);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&SliderHX);
#endif
	_XSliderTexture SliderTextureVX;
	SliderTextureVX.init("pic/MutiList/BLH_00.png","pic/MutiList/BLH_01.png","pic/MutiList/BLH_02.png","pic/MutiList/BLH_03.png",
		"pic/MutiList/BH_00.png","pic/MutiList/BH_01.png","pic/MutiList/BH_02.png","pic/MutiList/BH_03.png");
	_XSlider SliderVX;
	SliderVX.init(_XVector2(200.0f,200.0f),_XRect(0.0f,0.0f,32.0f,512.0f - 40.0f),_XRect(0.0f,0.0f,32.0f,32.0f),SliderTextureVX,_XSLIDER_TYPE_VERTICAL,100.0f,0.0f);
	_XControlManager::GetInstance().decreaseAObject(&SliderVX);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&SliderVX);
#endif
	_XMultiListTexture MutiListTexture;
	MutiListTexture.init("pic/MutiList/E_back.png","pic/MutiList/E_disable.png","pic/MutiList/E_Select.png","pic/MutiList/E_move.png","pic/MutiList/E_Title.png","pic/MutiList/E_TitleEnd.png");
	MutiList.init(_XVector2(610.0f,450.0f),_XRect(4.0f,4.0f,508.0f,508.0f),MutiListTexture,FontUnicode,1.0f,3,24/*,MouseRightButtonMenu*/,SliderVX,SliderHX);
	char tempChar[] = "Boxxx"; 
	for(int i = 0;i < 24;i++)
	{
		for(int j = 0; j < 3;j++)
		{
			tempChar[3] = (i * 4 + j) /10 + '0';
			tempChar[4] = (i * 4 + j) %10 + '0';
			MutiList.setBoxStr(tempChar,i,j);
		}
	}
	MutiList.setTileStr("��Title00",0);
	MutiList.setRowWidth(100,0);
	MutiList.setTileStr("Titleʤ",1);
	MutiList.setRowWidth(120,1);
	MutiList.setTileStr("Title02��",2);
	MutiList.setRowWidth(150,2);
	MutiList.exportData();
	//MutiList.importData();
	MutiList.setPosition(610,450);
	MutiList.setSize(0.5,0.5);
	//��ʼ�������˵�
	_XComboTexture ComboTexture;
	ComboTexture.init("pic/Combo/C_back.png","pic/Combo/C_disable.png",
		"pic/Combo/C_rightButtomEnable.png","pic/Combo/C_rightButtomOn.png","pic/Combo/C_rightButtomDown.png","pic/Combo/C_rightButtomDisable.png",
		"pic/Combo/C_downMenuUpEnable.png","pic/Combo/C_downMenuUpOn.png","pic/Combo/C_downMenuUpDown.png","pic/Combo/C_downMenuUpDisable.png",
		"pic/Combo/C_downMenuEnable.png","pic/Combo/C_downMenuOn.png","pic/Combo/C_downMenuDown.png","pic/Combo/C_downMenuDisable.png",
		"pic/Combo/C_downMenuDownEnable.png","pic/Combo/C_downMenuDownOn.png","pic/Combo/C_downMenuDownDown.png","pic/Combo/C_downMenuDownDisable.png");
	Combo.init(ComboTexture,_XVector2(250.0f,190.0f),_XRect(8.0f,1.0f,217.0f,43.0f),_XRect(6.0f,1.0f,48.0f,43.0f),_XRect(8.0f,1.0f,261.0f,23.0f),
		_XRect(8.0f,1.0f,261.0f,39.0f),_XRect(8.0f,1.0f,261.0f,23.0f),10,5,FontUnicode,1.0f);
	Combo.setMenuStr("This input is long",0);
	Combo.setMenuStr("����!",1);
	Combo.setMenuStr("������!",2);
	Combo.setMenuStr("������!",3);
	Combo.setMenuStr("������!",4);

	ComboCopy.setACopy(Combo);
	ComboCopy.setPosition(250.0f,160.0f);
	ComboCopy.setSize(_XVector2(0.5,0.5));

	while (!quit) 
	{
		quit = SDL_event(screen);
		interval = getFrameTiming();
		
		engineMove(interval);	//����ĸ�������
		clearScreen();			//�����Ļ

		updateScreen();			//������Ļ������
		
		SDL_Delay(1);
	}
	releaseSDL();
	return 0;	
}

//�������¼������������磺�����¼���������¼�
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
			case SDLK_d:
				break;
			case SDLK_a:
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
		case SDL_MOUSEMOTION:	//����ƶ�
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