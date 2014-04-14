#ifndef _JIA_XSOUND_
#define _JIA_XSOUND_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XSoundCommon.h"
#include "XBasicFun.h"
#include "XResourceManager.h"
//�޸Ŀ��Դ���Դ��������ȡ��Դ
#include <vector>
//#define MAX_SOUND_SUM 128	//�����Ч����
#define MIN_SOUND_DELAY 50	//��ͬ��Ч���ŵ���Сʱ��������λΪ����

struct _XSoundInfo
{
	void * sound;	//Mix_Chunk
	_XResourceInfo *resInfo;
	int soundTime;						//�����Ա���ڿ�����Ч���ŵļ������ֹͬһ����Ч�ڶ�ʱ���ڲ��ϵز���

	_XBool isSlowDownInit;
	_XBool isEnableSlowDown;
	unsigned char *slowDownOldData;
	int slowDownOldLen;
	unsigned char *slowDownNewData;
	int slowDownNewLen;
	int slowDownRate;
	int lateChannel;	//��¼������ŵ�ǰ��Ч���������Ա��ڿ��Կ��������Ч��ֹͣ��

	_XSoundInfo()
		:sound(NULL)
		,resInfo(NULL)
		,isSlowDownInit(XFalse)
		,isEnableSlowDown()
		,slowDownOldData(NULL)
		,slowDownNewData(NULL)
		,soundTime(-1)
		,lateChannel(-1)
	{}
};

typedef int XSoundHandle;
//������Ϊ����ϵͳ
class _XSound
{
	//+++++++++++++++++++++++++++++++++++++++++++
	//������Ҫ�������ΪSingletonģʽ
protected:
	_XSound();
	_XSound(const _XSound&);
	_XSound &operator= (const _XSound&);
	virtual ~_XSound();
public:
	static _XSound& GetInstance()
	{
		static _XSound m_instance;
		return m_instance;
	}
	//-------------------------------------------
private:
	_XResourcePosition m_resoursePosition;	//��Դλ��
private:
	std::vector<_XSoundInfo *> m_sound;

	int m_soundVolume;
	_XBool m_firstAddSoundFlag;
public:
	XSoundHandle addSound(const char* filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);					//����һ����Ч�����������Ч�ľ�������ڲ���
	void clearUp();											//��������������������Դ
	void clearOneSound(XSoundHandle soundHandle);			//�����ָ������Ч��Դ
	_XBool play(XSoundHandle soundHandle,int loops = 0);	//-1Ϊ����ѭ��
	void setSoundVolume(int volume);						//���õ�ǰ��Ч���ŵ�����
	void setAllVolume(int volume);							//���������������ŵ�����

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//�����ǹ��������ӳ��ķ�����ֻ��SDL_Mixer����С
	_XBool slowDownInit(int rate,XSoundHandle handle);
	void enableSlowDown(XSoundHandle handle);	//ʹ���ӳ�
	void disableSlowDown(XSoundHandle handle);	//ȡ���ӳ�
	void slowDownRelease(XSoundHandle handle);	//�ͷ���Դ
	_XBool slowDownAllInit(int rate);
	void enableAllSlowDown();	//ʹ���ӳ�
	void disableAllSlowDown();	//ȡ���ӳ�
	void slowDownAllRelease();	//�ͷ���Դ
	//-------------------------------------------------------

	void pause(XSoundHandle s)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return;
		_XSoundHandle.pauseSound(m_sound[s]->lateChannel);
	}
	void resume(XSoundHandle s)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return;
		_XSoundHandle.resumeSound(m_sound[s]->lateChannel);
	}
	_XBool isPause(XSoundHandle s)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return XFalse;
		return _XSoundHandle.isSoundPause(m_sound[s]->lateChannel);
	}
	void stop(XSoundHandle s)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return;
		_XSoundHandle.soundFadeOut(m_sound[s]->lateChannel,100);
	}
	_XBool isEnd(XSoundHandle s)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return XTrue;
		return !_XSoundHandle.isSoundPlaying(m_sound[s]->lateChannel);
	}
	_XBool fadeInSound(XSoundHandle s,int loops,int ms);
	void fadeOutSound(XSoundHandle s,int ms)
	{
		if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL) return;
		_XSoundHandle.soundFadeOut(m_sound[s]->lateChannel,ms);
	}
	void release()	//ע����������಻���Զ�������������������ͷź���������һ��Ҫ�ǵ����������ͷź������Զ����õĻ���Ҫ�����ظ�����
	{
		_XSoundHandle.haltSound();//�ر����е�channel
		clearUp();
	}
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
	_XSoundHandle.setVolume(-1,m_soundVolume);
}
#endif