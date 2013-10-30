#ifndef _JIA_XSOUND_
#define _JIA_XSOUND_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include <SDL_mixer.h>
#include "XBasicFun.h"
//�޸Ŀ��Դ���Դ��������ȡ��Դ

#define MAX_SOUND_SUM 128	//�����Ч����
#define MIN_SOUND_DELAY 50	//��ͬ��Ч���ŵ���Сʱ��������λΪ����

typedef int XSoundHandle;

class _XSound
{
private:
	int m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
private:
	Mix_Chunk *m_sound[MAX_SOUND_SUM];
	int m_soundTime[MAX_SOUND_SUM];						//�����Ա���ڿ�����Ч���ŵļ������ֹͬһ����Ч�ڶ�ʱ���ڲ��ϵز���
	int m_soundVolume;
	int firstAddSoundFlag;

	char m_isSlowDownInit[MAX_SOUND_SUM];
	char m_isEnableSlowDown[MAX_SOUND_SUM];
	unsigned char *m_slowDownOldData[MAX_SOUND_SUM];
	int m_slowDownOldLen[MAX_SOUND_SUM];
	unsigned char *m_slowDownNewData[MAX_SOUND_SUM];
	int m_slowDownNewLen[MAX_SOUND_SUM];
	int m_slowDownRate[MAX_SOUND_SUM];
public:
	XSoundHandle addSound(const char* filename,int resoursePosition = 0);					//����һ����Ч�����������Ч�ľ�������ڲ���
	void clearUp();										//��������������������Դ
	void clearOneSound(XSoundHandle soundHandle);		//�����ָ������Ч��Դ
	int play(XSoundHandle soundHandle);
	void setSoundVolume(int volume);					//���õ�ǰ��Ч���ŵ�����
	void setAllVolume(int volume);						//���������������ŵ�����
	_XSound();
	~_XSound();

	//�����ǹ��������ӳ��ķ���
	int slowDownInit(int rate,XSoundHandle handle);
	void enableSlowDown(XSoundHandle handle);	//ʹ���ӳ�
	void disableSlowDown(XSoundHandle handle);	//ȡ���ӳ�
	void slowDownRelease(XSoundHandle handle);	//�ͷ���Դ

	int slowDownAllInit(int rate);
	void enableAllSlowDown();	//ʹ���ӳ�
	void disableAllSlowDown();	//ȡ���ӳ�
	void slowDownAllRelease();	//�ͷ���Դ
	//forTest�����������ӳ���һ����Χ����Ч
	void slowDown();
};

inline void _XSound::setSoundVolume(int volume)
{
	if(volume < 0) volume = 0;
	if(volume > 128) volume = 128;
	m_soundVolume = volume;
}

inline void _XSound::setAllVolume(int volume)
{
	if(volume < 0) volume = 0;
	if(volume > 128) volume = 128;
	m_soundVolume = volume;
	Mix_Volume(-1,m_soundVolume);
}
#endif