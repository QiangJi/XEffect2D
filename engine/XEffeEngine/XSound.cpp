//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XSound.h"
//#include "SDL.h"
#include "XBasicFun.h"
#include "XBasicWindow.h"
#include "XResourcePack.h"
#include "XLogBook.h"
#include "XTimer.h"
void channelFinished(int channel)
{//当某个通道的音效播放完成的时候会调用这个函数
	printf("%d finished!\n",channel);
}
void effectFunc(int chan,void *stream,int len,void *udata)
{//下面在这里进行转码(现在这里假设实现一个慢播放的功能):会crash，还不知道怎么弄呢。
	SDL_AudioCVT  wav_cvt;
	int ret = SDL_BuildAudioCVT(&wav_cvt,
		XEE::audioFormat,XEE::audioChannel,XEE::audioFrequency,
		XEE::audioFormat,XEE::audioChannel,XEE::audioFrequency * 2);
	if(ret == -1) printf("Couldn't build converter!\n");
	wav_cvt.buf = (Uint8 *)malloc(len * wav_cvt.len_mult);
	if(wav_cvt.buf == NULL) return;
	wav_cvt.len = len;
	memcpy(wav_cvt.buf,stream,wav_cvt.len);
	SDL_ConvertAudio(&wav_cvt);
	SDL_MixAudio((Uint8 *)stream,wav_cvt.buf, wav_cvt.len_cvt,128);
	free(wav_cvt.buf);
}
_XSound::_XSound()
:m_firstAddSoundFlag(XFalse)
,m_resoursePosition(RESOURCE_LOCAL_FOLDER)
{
	m_sound.clear();
//	Mix_ChannelFinished(channelFinished);
}
_XSound::~_XSound()
{
	clearUp();
}
XSoundHandle _XSound::addSound(const char* filename,_XResourcePosition resoursePosition)
{
	if(!m_firstAddSoundFlag)
	{//这里相当于是初始化
		m_soundVolume = _XSoundHandle.getVolume(0);
		m_firstAddSoundFlag = XTrue;
	}
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	m_resoursePosition = resoursePosition;
	_XSoundInfo *temp = createMem<_XSoundInfo>();
	if(temp == NULL) return -1;
	temp->resInfo = _XResourceManager::GetInstance().loadResource(filename,RESOURCE_TYPE_SOUND,m_resoursePosition);
	if(temp->resInfo == NULL)
	{
		XDELETE(temp);
		return -1;
	}
	temp->sound = ((_XResourceSound *)temp->resInfo->m_pointer)->m_handle;
	m_sound.push_back(temp);
	return m_sound.size() - 1;
}
void _XSound::clearUp()
{//资源被资源管理器管理之后，自身不能释放资源
	for(int i = 0;i < m_sound.size();++ i)
	{
		if(m_sound[i] != NULL && m_sound[i]->resInfo != NULL)
		{
			_XResourceManager::GetInstance().releaseResource(m_sound[i]->resInfo);
			m_sound[i]->resInfo = NULL;
			XDELETE(m_sound[i]);
		}
	}
	m_sound.clear();
}
void _XSound::clearOneSound(XSoundHandle soundHandle)
{
	if(soundHandle < 0 || soundHandle >= m_sound.size()
		|| m_sound[soundHandle] == NULL) return;
	_XSoundInfo *temp = m_sound[soundHandle];
	if(temp->resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(temp->resInfo);	//尚未实现
		temp->resInfo = NULL;
		XDELETE(m_sound[soundHandle]);
	}
}
_XBool _XSound::play(XSoundHandle soundHandle,int loops)
{
	if(soundHandle < 0 || soundHandle >= m_sound.size()
		|| m_sound[soundHandle] == NULL || m_sound[soundHandle]->sound == NULL) return XFalse;
	_XSoundInfo *temp = m_sound[soundHandle];
	if(temp->soundTime == -1 || getCurrentTicks() - temp->soundTime >= MIN_SOUND_DELAY)
	{
		temp->soundTime = getCurrentTicks();
#ifdef DEBUG_MODE
		printf("sound play %d\n",soundHandle);
#endif
		int channel = _XSoundHandle.playSound(temp->sound,loops);
		_XSoundHandle.setVolume(channel,m_soundVolume);
		temp->lateChannel = channel;
	//	Mix_RegisterEffect(channel,effectFunc,NULL,this);
		return XTrue;
	}
	return XFalse;
}
_XBool _XSound::slowDownInit(int rate,XSoundHandle handle)
{
	if(rate <= 0 || rate >= 10) return XFalse;
	if(handle < 0 || handle >= m_sound.size()
		|| m_sound[handle] == NULL) return XFalse;
	_XSoundInfo *temp = m_sound[handle];

	if(temp->isSlowDownInit) return XFalse;	//如果已经放慢过，需要重新放慢才能播放
	if(temp->sound == NULL) return XFalse;
	//记录原有数据
	temp->slowDownOldData = _XSoundHandle.getData(temp->sound);
	temp->slowDownOldLen = _XSoundHandle.getDataLen(temp->sound);
	//产生新的数据
	temp->slowDownNewLen = temp->slowDownOldLen * rate;
	temp->slowDownNewData = createArrayMem<unsigned char>(temp->slowDownNewLen);
	temp->slowDownRate = rate;
	//产生新的数据(这个数据是针对于双声道,16bits的)
	for(int i = 0;i < temp->slowDownOldLen >> 2;++ i)
	{
		for(int j = 0;j < 4 * rate;++ j)
		{
			temp->slowDownNewData[i * 4 * rate + j] = temp->slowDownOldData[i * 4 + (j % 4)];
		}
	}
	//记录状态
	temp->isEnableSlowDown = XFalse;
	temp->isSlowDownInit = XTrue;
	return XTrue;
}
void _XSound::enableSlowDown(XSoundHandle handle)	//使能延长
{
	if(handle < 0 || handle >= m_sound.size()
		|| m_sound[handle] == NULL) return;
	_XSoundInfo *temp = m_sound[handle];
	if(!temp->isEnableSlowDown && temp->isSlowDownInit)
	{
		_XSoundHandle.setData(temp->sound,temp->slowDownNewData);
		_XSoundHandle.setDataLen(temp->sound,temp->slowDownNewLen);
		temp->isEnableSlowDown = XTrue;
	}
}
void _XSound::disableSlowDown(XSoundHandle handle)	//取消延长
{
	if(handle < 0 || handle >= m_sound.size()
		|| m_sound[handle] == NULL) return;
	_XSoundInfo *temp = m_sound[handle];
	if(temp->isEnableSlowDown)
	{
		_XSoundHandle.setData(temp->sound,temp->slowDownOldData);
		_XSoundHandle.setDataLen(temp->sound,temp->slowDownOldLen);
		temp->isEnableSlowDown = XFalse;
	}
}
void _XSound::slowDownRelease(XSoundHandle handle)	//释放资源
{
	if(handle < 0 || handle >= m_sound.size()
		|| m_sound[handle] == NULL) return;
	_XSoundInfo *temp = m_sound[handle];
	if(temp->isSlowDownInit)
	{
		if(temp->isEnableSlowDown) disableSlowDown(handle);
		XDELETE_ARRAY(temp->slowDownNewData);
		temp->isSlowDownInit = XFalse;
	}
}
_XBool _XSound::slowDownAllInit(int rate)
{
	if(rate <= 0 || rate >= 10) return XFalse;
	for(int i = 0;i < m_sound.size();++ i)
	{
		if(m_sound[i] == NULL) continue;
		if(m_sound[i]->isSlowDownInit)
		{
			if(m_sound[i]->slowDownRate != rate)
			{
				slowDownRelease(i);
				slowDownInit(rate,i);
			}
		}else
		{
			slowDownInit(rate,i);
		}
	}
	return XTrue;
}
void _XSound::enableAllSlowDown()	//使能延长
{
	for(int i = 0;i < m_sound.size();++ i)
	{
		enableSlowDown(i);
	}
}
void _XSound::disableAllSlowDown()	//取消延长
{
	for(int i = 0;i < m_sound.size();++ i)
	{
		disableSlowDown(i);
	}
}
void _XSound::slowDownAllRelease()	//释放资源
{
	for(int i = 0;i < m_sound.size();++ i)
	{
		slowDownRelease(i);
	}
}
_XBool _XSound::fadeInSound(XSoundHandle s,int loops,int ms)
{
	if(s < 0 || s >= m_sound.size() || m_sound[s] == NULL
		|| m_sound[s]->sound == NULL) return XFalse;

	if(m_sound[s]->soundTime == -1 || getCurrentTicks() - m_sound[s]->soundTime >= MIN_SOUND_DELAY)
	{
		m_sound[s]->soundTime = getCurrentTicks();
#ifdef DEBUG_MODE
		printf("sound play %d\n",s);
#endif
		int channel = _XSoundHandle.soundFadeIn(m_sound[s]->sound,loops,ms);
		_XSoundHandle.setVolume(channel,m_soundVolume);
		m_sound[s]->lateChannel = channel;
	//	Mix_RegisterEffect(channel,effectFunc,NULL,this);
		return XTrue;
	}
	return XFalse;
}