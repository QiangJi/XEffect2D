//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMusic.h"
#include "XBasicFun.h"
#include "XResourcePack.h"

_XMusic::_XMusic()
:firstAddMusicFlag(0)
{
	for(int i = 0;i < MAX_MUSIC_SUM;++ i)
	{
		m_music[i] = NULL;		//��ʼ����Ч��ԴΪNULL
	}
//	m_resoursePosition = RESOURCE_OUTSIDE;
}

_XMusic::~_XMusic()
{
	Mix_HaltMusic();
	clearUp();
}

XMusicHandle _XMusic::addMusic(const char *fileName)
{
	if(firstAddMusicFlag == 0)
	{//�����൱���ǳ�ʼ��
		m_musicVolume = Mix_Volume(0,-1);
		firstAddMusicFlag = 1;
	}
	for(int i = 0;i < MAX_MUSIC_SUM;++ i)
	{
		if(m_music[i] == NULL)
		{
		//	if(m_resoursePosition == RESOURCE_OUTSIDE)
			{
				if((m_music[i] = Mix_LoadMUS(fileName)) == NULL) 
				{
					AddLogInfoStr("Sound load error!\n");
					return -1;
				}
			}
		/*	else
			{
				char tempFileName[]="xiajia.mp3";
				memcpy(tempFileName + 7,fileName + strlen(fileName) - 3,4);
				FILE *fp;
				_XResourcePack& ResourceTemp = * _XResourcePack::GetInstance();
				if((fp = fopen(tempFileName,"wb")) == NULL)
				{
					DebugShow("Action text file error!\n");
					exit(1);
				}
				int lengthTemp = ResourceTemp.getFileLength(fileName);
				unsigned char *p = NULL;
				try
				{
					p = new unsigned char[lengthTemp + 1];
					if(p == NULL) exit(1);
				}catch(...)
				{
					exit(1);
				}
				ResourceTemp.unpackResource(fileName,p);
				fwrite(p,lengthTemp,1,fp);
				XDELETE_ARRAY(p);
				fclose(fp);
				//��Ҫע���������δ����mp3�ĸ�ʽ
				if((m_music[i] = Mix_LoadMUS(tempFileName)) == NULL) 
				{
					//DeleteFile(tempFileName);
					WCHAR wsz[64]; 
					swprintf(wsz,L"%S",tempFileName);
					DeleteFile(wsz);//ɾ����ʱ�ļ�
					DebugShow("Sound load error!\n");
					return -1;
				}
				//DeleteFile(L"xiajia.wav");
				WCHAR wsz[64]; 
				swprintf(wsz,L"%S",tempFileName);
				DeleteFile(wsz);//ɾ����ʱ�ļ�
			}*/
//			DebugShow("Loading sound:%s\n",File);
			return i;
		}
	}
	return -1;
}

void _XMusic::clearUp()
{
	for(int i = 0;i < MAX_MUSIC_SUM;++ i)
	{
		if(m_music[i] != NULL)
		{
			Mix_FreeMusic(m_music[i]);
			m_music[i] = NULL;
		}
	}
}