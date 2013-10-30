//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XSound.h"
#include "SDL.h"
#include "XBasicFun.h"
#include "XResourcePack.h"
#include "XLogBook.h"

_XSound::_XSound()
:firstAddSoundFlag(0)
,m_resoursePosition(RESOURCE_OUTSIDE)
{
	for(int i = 0;i < MAX_SOUND_SUM;++ i)
	{
		m_soundTime[i] = -1;	//��ʼ����Ч�Ĳ�����ʼʱ��Ϊ-1
		m_sound[i] = NULL;		//��ʼ����Ч��ԴΪNULL
		m_isSlowDownInit[i] = 0;
	}
}
_XSound::~_XSound()
{
	Mix_HaltChannel(-1);
	clearUp();
}

int _XSound::addSound(const char* filename,int resoursePosition)
{
	if(firstAddSoundFlag == 0)
	{//�����൱���ǳ�ʼ��
		m_soundVolume = Mix_Volume(0,-1);
		firstAddSoundFlag = 1;
	}
	if(resoursePosition != RESOURCE_OUTSIDE)
	{
		resoursePosition = RESOURCE_INSIDE;
	}
	m_resoursePosition = resoursePosition;
	for(int i = 0;i < MAX_SOUND_SUM;++ i)
	{
		if(m_sound[i] == NULL)
		{
			if(m_resoursePosition == RESOURCE_OUTSIDE)
			{
				if((m_sound[i] = Mix_LoadWAV(filename)) == NULL) 
				{
					AddLogInfoStr("Sound load error!\n");
					return -1;
				}
			}else
			{
				int lengthTemp = _XResourcePack::GetInstance().getFileLength(filename);
				unsigned char *p = NULL;
				try
				{
					p = new unsigned char[lengthTemp + 1];
					if(p == NULL) return -1;
				}catch(...)
				{
					return -1;
				}
				_XResourcePack::GetInstance().unpackResource(filename,p);
				SDL_RWops *fileData = SDL_RWFromMem(p,lengthTemp);
				if((m_sound[i] = Mix_LoadWAV_RW(fileData,1)) == NULL) 
				{
					AddLogInfoStr("Sound load error!\n");
					return -1;
				}
				XDELETE_ARRAY(p);
				//SDL_FreeRW(fileData);
			}
			return i;
		}
	}
	return -1;
}

void _XSound::clearUp()
{
	for(int i = 0;i < MAX_SOUND_SUM;++ i)
	{
		if(m_sound[i] != NULL)
		{
			Mix_FreeChunk(m_sound[i]);
			m_sound[i] = NULL;
		}
	}
}

void _XSound::clearOneSound(XSoundHandle soundHandle)
{
	if(soundHandle < 0 || soundHandle >= MAX_SOUND_SUM) return;
	if(m_sound[soundHandle] != NULL)
	{
		Mix_FreeChunk(m_sound[soundHandle]);
		m_sound[soundHandle] = NULL;
	}
}

int _XSound::play(XSoundHandle soundHandle)
{
	if(soundHandle < 0 || soundHandle >= MAX_SOUND_SUM) return 0;
    if(m_sound[soundHandle] == NULL) return 0;

	if(m_soundTime[soundHandle] == -1 || SDL_GetTicks() - m_soundTime[soundHandle] >= MIN_SOUND_DELAY)
	{
		m_soundTime[soundHandle] = SDL_GetTicks();
#ifdef DEBUG_MODE
		printf("sound play %d\n",soundHandle);
#endif
		int channel = Mix_PlayChannel(-1, m_sound[soundHandle], 0);
		Mix_Volume(channel,m_soundVolume);
		return 1;
	}
	return 0;
}

int _XSound::slowDownInit(int rate,XSoundHandle handle)
{
	if(rate <= 0 || rate >= 10) return 0;
	if(m_isSlowDownInit[handle] == 1) return 0;	//����Ѿ�����������Ҫ���·������ܲ���
	if(handle < 0 || handle >= MAX_SOUND_SUM) return 0;
	if(m_sound[handle] == NULL) return 0;
	//��¼ԭ������
	m_slowDownOldData[handle] = m_sound[handle]->abuf;
	m_slowDownOldLen[handle] = m_sound[handle]->alen;
	//�����µ�����
	m_slowDownNewLen[handle] = m_sound[handle]->alen * rate;
	m_slowDownNewData[handle] = new unsigned char[m_slowDownNewLen[handle]];
	m_slowDownRate[handle] = rate;
	//�����µ�����(��������������˫����,16bits��)
	for(int i = 0;i < m_sound[handle]->alen >> 2;++ i)
	{
		for(int j = 0;j < 4 * rate;++ j)
		{
			m_slowDownNewData[handle][i * 4 * rate + j] = m_sound[handle]->abuf[i * 4 + (j % 4)];
		}
	}
	//��¼״̬
	m_isEnableSlowDown[handle] = 0;
	m_isSlowDownInit[handle] = 1;
	return 1;
}
void _XSound::enableSlowDown(XSoundHandle handle)	//ʹ���ӳ�
{
	if(handle < 0 || handle >= MAX_SOUND_SUM) return;
	if(m_isEnableSlowDown[handle] == 0 && m_isSlowDownInit[handle] != 0)
	{
		m_sound[handle]->alen = m_slowDownNewLen[handle];
		m_sound[handle]->abuf = m_slowDownNewData[handle];
		m_isEnableSlowDown[handle] = 1;
	}
}
void _XSound::disableSlowDown(XSoundHandle handle)	//ȡ���ӳ�
{
	if(handle < 0 || handle >= MAX_SOUND_SUM) return;
	if(m_isEnableSlowDown[handle] != 0)
	{
		m_sound[handle]->alen = m_slowDownOldLen[handle];
		m_sound[handle]->abuf = m_slowDownOldData[handle];
		m_isEnableSlowDown[handle] = 0;
	}
}
void _XSound::slowDownRelease(XSoundHandle handle)	//�ͷ���Դ
{
	if(handle < 0 || handle >= MAX_SOUND_SUM) return;
	if(m_isSlowDownInit[handle] != 0)
	{
		if(m_isEnableSlowDown != 0) disableSlowDown(handle);
		XDELETE_ARRAY(m_slowDownNewData[handle]);
		m_isSlowDownInit[handle] = 0;
	}
}
int _XSound::slowDownAllInit(int rate)
{
	if(rate <= 0 || rate >= 10) return 0;
	for(int i = 0;i < MAX_SOUND_SUM;++ i)
	{
		if(m_isSlowDownInit[i] != 0)
		{
			if(m_slowDownRate[i] != rate)
			{
				slowDownRelease(i);
				slowDownInit(rate,i);
			}
		}else
		{
			slowDownInit(rate,i);
		}
	}
	return 1;
}
void _XSound::enableAllSlowDown()	//ʹ���ӳ�
{
	for(int i = 0;i < MAX_SOUND_SUM;++ i)
	{
		enableSlowDown(i);
	}
}
void _XSound::disableAllSlowDown()	//ȡ���ӳ�
{
	for(int i = 0;i < MAX_SOUND_SUM;++ i)
	{
		disableSlowDown(i);
	}
}
void _XSound::slowDownAllRelease()	//�ͷ���Դ
{
	for(int i = 0;i < MAX_SOUND_SUM;++ i)
	{
		slowDownRelease(i);
	}
}

//forTest�����������ӳ���һ����Χ����Ч
void _XSound::slowDown()
{
	unsigned char temp[2];
	unsigned char *p = new unsigned char[m_sound[0]->alen * 3];
	for(int i = 0;i < m_sound[0]->alen >> 2;++ i)
	{
		for(int j = 0;j < 12;++ j)
		{
			p[i * 12 + j] = m_sound[0]->abuf[i * 4 + (j % 4)];
		}
	}
	m_sound[0]->alen *= 3;
	free(m_sound[0]->abuf);
	m_sound[0]->abuf = p;
}
