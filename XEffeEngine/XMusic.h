#ifndef _JIA_XMUSIC_
#define _JIA_XMUSIC_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
#include "SDL_mixer.h"
#include "XLogBook.h"

//Ŀǰ���񲢲�֧�ִ��ڴ�������MP3��ʽ���鿴sdl_mixer�е�Mix_LoadMUS_RW˵���к�����˼��ֻ֧��Ogg��MikMod
#define MAX_MUSIC_SUM 128	//��������

typedef int XMusicHandle;

class _XMusic
{
//public:
//	int m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
private:
	Mix_Music *m_music[MAX_MUSIC_SUM];
	int m_musicVolume;
	int firstAddMusicFlag;
public:
	XMusicHandle addMusic(const char *fileName);
	void clearUp();											//��������������������Դ
	void clearOneMusic(XMusicHandle musicHandle);			//�����ָ������Ч��Դ
	int playMusic(XMusicHandle musicHandle,int loop = 0);
	void setMusicVolume(int volume);						//���õ�ǰ��Ч���ŵ�����(0 - 128)
	void fadeOutMusic(int ms);
	int fadeInMusic(XMusicHandle musicHandle,int loop = 0,int ms = 0);
	_XMusic();
	~_XMusic();

	int isEnd();
};

inline int _XMusic::fadeInMusic(XMusicHandle musicHandle,int loop,int ms)
{
	if(musicHandle < 0 || musicHandle >= MAX_MUSIC_SUM) return 0;
	if(Mix_FadeInMusic(m_music[musicHandle],loop,ms) == -1) return 0;
	return 1;
}

inline void _XMusic::fadeOutMusic(int ms)
{
	Mix_FadeOutMusic(ms);
}

inline void _XMusic::clearOneMusic(XMusicHandle musicHandle)
{
	if(musicHandle < 0 || musicHandle >= MAX_MUSIC_SUM) return;
	if(m_music[musicHandle] != NULL)
	{
		Mix_FreeMusic(m_music[musicHandle]);
		m_music[musicHandle] = NULL;
	}
}

inline int _XMusic::playMusic(XMusicHandle musicHandle,int loop)
{
	if(musicHandle < 0 || musicHandle >= MAX_MUSIC_SUM) return 0;
	if(Mix_PlayMusic(m_music[musicHandle],loop) == -1)
	{
		AddLogInfoStr("Sound play error!\n");
		return 0;
	}
	return 1;
}

inline void _XMusic::setMusicVolume(int volume)
{
	if(volume < 0) volume = 0;
	if(volume > 128) volume = 128;
	m_musicVolume = volume;
	Mix_VolumeMusic(m_musicVolume);
}

inline int _XMusic::isEnd()
{
	if(Mix_PlayingMusic() == 1)
	{
		return 0;
	}else
	{
		return 1;
	}
}

#endif