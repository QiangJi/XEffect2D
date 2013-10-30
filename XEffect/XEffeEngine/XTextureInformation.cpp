//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XTextureInformation.h"
#include "XResourcePack.h"
#include "XResourceOptimize.h"
//#include "png.h"
#include "XSavePNG.h"

#if TEX_INFO_DEBUG
int texInfoSum = 0;
#endif

_XAllTexture::_XAllTexture()
:m_isInited(0)
,m_isEnable(0)
,m_textureSum(0)
,m_targetTextureSum(0)
,m_texInfo(NULL)
{
}

_XAllTexture::~_XAllTexture()
{
	release();
}

_XAllTexture& _XAllTexture::GetInstance()
{
	static _XAllTexture m_instance;
	return m_instance;
}

int _XAllTexture::init(int resoursePosition)
{
	if(m_isInited != 0) return 1;

	//������Դ��λ��
	if(resoursePosition != RESOURCE_OUTSIDE)
	{
		resoursePosition = RESOURCE_INSIDE;
	}
	m_resoursePosition = resoursePosition;
	
	if(m_resoursePosition == RESOURCE_INSIDE)
	{//�����Դ�Ѿ����
		int lengthTemp = _XResourcePack::GetInstance().getFileLength(PNG_INFORMATION_FILE_NAME);
		if(lengthTemp <= 0) return 0;
		unsigned char *p = NULL;
		try
		{
			p = new unsigned char[lengthTemp + 1];
			if(p == NULL) return 0;
		}catch(...)
		{
			return 0;
		}
		//����Դ������ȡ����Դ�ļ�������
		_XResourcePack::GetInstance().unpackResource(PNG_INFORMATION_FILE_NAME,p);
		*(p + lengthTemp) = '\0';
		//����һ������
		int offset = 0;
		sscanf((char *)(p + offset),"%d,",&m_textureSum);
		offset += getCharPosition((char *)(p + offset),',') + 1;
		sscanf((char *)(p + offset),"%d;",&m_targetTextureSum);
		offset += getCharPosition((char *)(p + offset),';') + 1;
		if(m_textureSum <= 0 || m_targetTextureSum <= 0)
		{
			XDELETE_ARRAY(p);
			m_isEnable = 0;
			return 0;
		}
		//�����ڴ�ռ�
		try
		{
			m_pTexture = new _XTexture[m_targetTextureSum];
			if(m_pTexture == NULL)
			{
				XDELETE_ARRAY(p);
				m_isEnable = 0;
				return 0;
			}
		}catch(...)
		{
			XDELETE_ARRAY(p);
			m_isEnable = 0;
			return 0;
		}
		try
		{
			m_texInfo = new _XTextureInfo[m_textureSum];
			if(m_texInfo == NULL)
			{
				XDELETE_ARRAY(p);
				XDELETE_ARRAY(m_pTexture);
				m_isEnable = 0;
				return 0;
			}
		}catch(...)
		{
			XDELETE_ARRAY(p);
			XDELETE_ARRAY(m_pTexture);
			m_isEnable = 0;
			return 0;
		}
		//������ȡ�������Դ�������Ҫ����Դ����
		char tempChar = ' ';
		int tempFlag = 0;
		char *targetTextureFlag = NULL;
		char targetFileName[MAX_FILE_NAME_LENGTH] = "";
		try
		{
			targetTextureFlag = new char[m_targetTextureSum];
			if(targetTextureFlag == NULL) 
			{
				XDELETE_ARRAY(p);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				m_isEnable = 0;
				return 0;
			}
		}catch(...)
		{
			XDELETE_ARRAY(p);
			XDELETE_ARRAY(m_pTexture);
			XDELETE_ARRAY(m_texInfo);
			m_isEnable = 0;
			return 0;
		}
		for(int i = 0;i < m_targetTextureSum;++ i)
		{//������е���Դ��δ����
			targetTextureFlag[i] = 0;
		}
		for(int i = 0;i < m_textureSum;++ i)
		{//������һ���س�λ
			sscanf((char *)(p + offset),"%c",&tempChar);
			offset ++;//��ȡ�ı��Ļس�λ
			sscanf((char *)(p + offset),"%c",&tempChar);
			offset ++;//��ȡ�ı��Ļس�λ

			tempFlag = 0;
			for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
			{
				sscanf((char *)(p + offset),"%c",&tempChar);
				offset ++;
				if(tempChar == ':')
				{
					m_texInfo[i].textureName[j] = '\0';
					tempFlag = 1;
					break;
				}else
				{
					m_texInfo[i].textureName[j] = tempChar;
				}
			}
			if(tempFlag == 0)
			{//�ļ����ݷ�����������ֱ�ӷ���
				m_isEnable = 0;
				XDELETE_ARRAY(p);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				XDELETE_ARRAY(targetTextureFlag);
				return 0;
			}
			tempFlag = 0;
			for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
			{
				sscanf((char *)(p + offset),"%c",&tempChar);
				offset ++;
				if(tempChar == ':')
				{
					targetFileName[j] = '\0';
					tempFlag = 1;
					break;
				}else
				{
					targetFileName[j] = tempChar;
				}
			}
			if(tempFlag == 0)
			{//�ļ����ݷ�����������ֱ�ӷ���
				m_isEnable = 0;
				XDELETE_ARRAY(p);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				XDELETE_ARRAY(targetTextureFlag);
				return 0;
			}
			sscanf((char *)(p + offset),"%d:",&tempFlag);
			offset += getCharPosition((char *)(p + offset),':') + 1;
			if(tempFlag < 0 || tempFlag >= m_targetTextureSum)
			{//�ļ����ݷ�����������ֱ�ӷ���
				m_isEnable = 0;
				XDELETE_ARRAY(p);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				XDELETE_ARRAY(targetTextureFlag);
				return 0;
			}
			if(targetTextureFlag[tempFlag] == 0)
			{//������ֵ�ǰ��Դ��δ���룬������������Դ
				if(m_pTexture[tempFlag].load(targetFileName,m_resoursePosition) == 0)
				{
					m_isEnable = 0;
					XDELETE_ARRAY(p);
					XDELETE_ARRAY(m_pTexture);
					XDELETE_ARRAY(m_texInfo);
					XDELETE_ARRAY(targetTextureFlag);
					return 0;
				}
				targetTextureFlag[tempFlag] = 1;
			}
			m_texInfo[i].texture = m_pTexture[tempFlag];
			//��ȡʣ�µ���Ϣ
			sscanf((char *)(p + offset),"%f,",&m_texInfo[i].textureOffset.x);
			offset += getCharPosition((char *)(p + offset),',') + 1;
			sscanf((char *)(p + offset),"%f:",&m_texInfo[i].textureOffset.y);
			offset += getCharPosition((char *)(p + offset),':') + 1;

			sscanf((char *)(p + offset),"%f,",&m_texInfo[i].textureMove.x);
			offset += getCharPosition((char *)(p + offset),',') + 1;
			sscanf((char *)(p + offset),"%f,",&m_texInfo[i].textureMove.y);
			offset += getCharPosition((char *)(p + offset),',') + 1;
			sscanf((char *)(p + offset),"%f,",&m_texInfo[i].textureMove2.x);
			offset += getCharPosition((char *)(p + offset),',') + 1;
			sscanf((char *)(p + offset),"%f:",&m_texInfo[i].textureMove2.y);
			offset += getCharPosition((char *)(p + offset),':') + 1;
			sscanf((char *)(p + offset),"%f,",&m_texInfo[i].textureSize.x);
			offset += getCharPosition((char *)(p + offset),',') + 1;
			sscanf((char *)(p + offset),"%f;",&m_texInfo[i].textureSize.y);
			offset += getCharPosition((char *)(p + offset),';') + 1;
		}
		XDELETE_ARRAY(targetTextureFlag);
		//�������ݶ�ȡ���
		XDELETE_ARRAY(p);
	}else
	{
		FILE *fp;
		//���ļ�
		if((fp = fopen(PNG_INFORMATION_FILE_NAME,"r")) == NULL)
		{
			m_isEnable = 0;
			return 0;
		}
		//��ȡ�ļ�����
		fscanf(fp,"%d,",&m_textureSum);
		fscanf(fp,"%d;",&m_targetTextureSum);
		if(m_textureSum <= 0 || m_targetTextureSum <= 0)
		{
			fclose(fp);
			m_isEnable = 0;
			return 0;
		}
		//�����ڴ�ռ�
		try
		{
			m_pTexture = new _XTexture[m_targetTextureSum];
			if(m_pTexture == NULL)
			{
				fclose(fp);
				m_isEnable = 0;
				return 0;
			}
		}catch(...)
		{
			fclose(fp);
			m_isEnable = 0;
			return 0;
		}

		try
		{
			m_texInfo = new _XTextureInfo[m_textureSum];
			if(m_texInfo == NULL)
			{
				fclose(fp);
				XDELETE_ARRAY(m_pTexture);
				m_isEnable = 0;
				return 0;
			}
		}catch(...)
		{
			fclose(fp);
			XDELETE_ARRAY(m_pTexture);
			m_isEnable = 0;
			return 0;
		}
		//������ȡ�������Դ�������Ҫ����Դ����
		char tempChar;
		int tempFlag;
		char *targetTextureFlag = NULL;
		char targetFileName[MAX_FILE_NAME_LENGTH] = "";
		try
		{
			targetTextureFlag = new char[m_targetTextureSum];
			if(targetTextureFlag == NULL) 
			{
				fclose(fp);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				m_isEnable = 0;
				return 0;
			}
		}catch(...)
		{
			fclose(fp);
			XDELETE_ARRAY(m_pTexture);
			XDELETE_ARRAY(m_texInfo);
			m_isEnable = 0;
			return 0;
		}
		for(int i = 0;i < m_targetTextureSum;++ i)
		{
			targetTextureFlag[i] = 0;
		}
		for(int i = 0;i < m_textureSum;++ i)
		{
			//������һ���س�λ
			fscanf(fp,"%c",&tempChar);

			tempFlag = 0;
			for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
			{
				fscanf(fp,"%c",&tempChar);
				if(tempChar == ':')
				{
					m_texInfo[i].textureName[j] = '\0';
					tempFlag = 1;
					break;
				}else
				{
					m_texInfo[i].textureName[j] = tempChar;
				}
			}
			if(tempFlag == 0)
			{//�ļ����ݷ�����������ֱ�ӷ���
				fclose(fp);
				m_isEnable = 0;
				XDELETE_ARRAY(targetTextureFlag);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				return 0;
			}
			tempFlag = 0;
			for(int j = 0;j < MAX_FILE_NAME_LENGTH;++ j)
			{
				fscanf(fp,"%c",&tempChar);
				if(tempChar == ':')
				{
					targetFileName[j] = '\0';
					tempFlag = 1;
					break;
				}else
				{
					targetFileName[j] = tempChar;
				}
			}
			if(tempFlag == 0)
			{//�ļ����ݷ�����������ֱ�ӷ���
				fclose(fp);
				m_isEnable = 0;
				XDELETE_ARRAY(targetTextureFlag);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				return 0;
			}
			fscanf(fp,"%d:",&tempFlag);
			if(tempFlag < 0 || tempFlag >= m_targetTextureSum)
			{//�ļ����ݷ�����������ֱ�ӷ���
				fclose(fp);
				m_isEnable = 0;
				XDELETE_ARRAY(targetTextureFlag);
				XDELETE_ARRAY(m_pTexture);
				XDELETE_ARRAY(m_texInfo);
				return 0;
			}
			if(tempFlag >= m_targetTextureSum) return 0;
			if(targetTextureFlag[tempFlag] == 0)
			{
				if(m_pTexture[tempFlag].load(targetFileName,m_resoursePosition) == 0)
				{
					fclose(fp);
					m_isEnable = 0;
					XDELETE_ARRAY(targetTextureFlag);
					XDELETE_ARRAY(m_pTexture);
					XDELETE_ARRAY(m_texInfo);
					return 0;
				}
				targetTextureFlag[tempFlag] = 1;
			}
			m_texInfo[i].texture = m_pTexture[tempFlag];
			//��ȡʣ�µ���Ϣ
			fscanf(fp,"%f,",&m_texInfo[i].textureOffset.x);
			fscanf(fp,"%f:",&m_texInfo[i].textureOffset.y);
			fscanf(fp,"%f,",&m_texInfo[i].textureMove.x);
			fscanf(fp,"%f,",&m_texInfo[i].textureMove.y);
			fscanf(fp,"%f,",&m_texInfo[i].textureMove2.x);
			fscanf(fp,"%f:",&m_texInfo[i].textureMove2.y);
			fscanf(fp,"%f,",&m_texInfo[i].textureSize.x);
			fscanf(fp,"%f;",&m_texInfo[i].textureSize.y);
		}
		XDELETE_ARRAY(targetTextureFlag);
		//�������ݶ�ȡ���
		fclose(fp);
	}
	m_isEnable = 1;

	m_isInited = 1;
	return 1;
}

int _XAllTexture::getTexture(const char *temp,_XTextureInfo &texInfo)
{
	if(m_isInited == 0) return 0;
	if(m_isEnable == 0) return 0;
	if(temp == NULL) return 0;
	for(int i = 0;i < m_textureSum;++ i)
	{
		if(fileNameCompare(temp, m_texInfo[i].textureName) == 1)
		{
			texInfo = m_texInfo[i];
			//texInfo.pTexture = m_texInfo[i].pTexture;
			//texInfo.textureMove = m_texInfo[i].textureMove;
			//texInfo.textureMove2 = m_texInfo[i].textureMove2;
			//texInfo.textureOffset = m_texInfo[i].textureOffset;
			//texInfo.textureSize = m_texInfo[i].textureSize;
			//strcpy(texInfo.targetFileName,m_texInfo[i].targetFileName);
			//strcpy(texInfo.textureName,m_texInfo[i].textureName);
			return 1;
		}
	}
	return 0;
}

void _XAllTexture::release()
{
	if(m_isInited == 0) return;

	XDELETE_ARRAY(m_pTexture);
	XDELETE_ARRAY(m_texInfo);

	m_isInited = 0;
	m_isEnable = 0;
}

int _XTextureData::load(const char * filename,int resoursePosition)
{
	if(m_isInited != 0) return 1;

	_XTextureInfo texInfo;
	if(resoursePosition != 0 && _XAllTexture::GetInstance().getTexture(filename,texInfo) != 0)
	{
		texture = texInfo.texture;
		textureMove = texInfo.textureMove;
		textureMove2 = texInfo.textureMove2;
		clipInsideRect.set(texInfo.textureOffset.x,texInfo.textureOffset.y,
			texInfo.textureOffset.x + texInfo.textureSize.x,texInfo.textureOffset.y + texInfo.textureSize.y);
		textureSize.set(clipInsideRect.getWidth() + textureMove.x + textureMove2.x,clipInsideRect.getHeight() + textureMove.y + textureMove2.y);
		isEnableInsideClip = 1;
	}else
	{//������һ������Դû�н��й������ᵼ����Դ�ظ����룬Ҳ����ͬһ����Դ������롣��������Ҫ������
		if(texture.load(filename,resoursePosition) == 0) 
		{
			return 0;
		}
		textureSize.set(texture.m_w,texture.m_h);
		isEnableInsideClip = 0;
	}

	m_isInited = 1;
	return 1;
}

int _XTextureData::loadEmpty()
{
	if(m_isInited != 0) return 1;

	isEnableInsideClip = 0;

	m_isInited = 1;
	return 1;
}

int _XTextureData::setACopy(const _XTextureData& temp)
{
	if(& temp == this) return 0;	//��ֹ����ֵ
	m_isInited = temp.m_isInited;

	texture = temp.texture;		//ָ����ͼ��ָ��
	textureMove = temp.textureMove;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����Ͻ�
	textureMove2 = temp.textureMove2;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����½�
	isEnableInsideClip = temp.isEnableInsideClip;			//��ͼģʽ��1���ڲ��ü�ģʽ 0��û�о����ڲ��ü�
	clipInsideRect = temp.clipInsideRect;		//�ڲ����е�����
	textureSize = temp.textureSize;		//��ͼ�ĳߴ��Ż�֮ǰ����ͼ�ߴ�

	return 1;
}

_XTextureData& _XTextureData::operator = (const _XTextureData& temp)
{
	if(& temp == this) return *this;	//��ֹ����ֵ
	m_isInited = temp.m_isInited;

	texture = temp.texture;		//ָ����ͼ��ָ��
	textureMove = temp.textureMove;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����Ͻ�
	textureMove2 = temp.textureMove2;		//��Դ�ļ��ڲü�ʱ���ƶ�λ�ã����½�
	isEnableInsideClip = temp.isEnableInsideClip;			//��ͼģʽ��1���ڲ��ü�ģʽ 0��û�о����ڲ��ü�
	clipInsideRect = temp.clipInsideRect;		//�ڲ����е�����
	textureSize = temp.textureSize;		//��ͼ�ĳߴ��Ż�֮ǰ����ͼ�ߴ�

	return * this;
}

void _XTextureData::release()
{
	if(m_isInited == 0) return;
	m_isInited = 0;
}

//����Դ��ԭ
int reductionTexture(const unsigned char *p,int length,const _XTextureInfo &texInfo)
{
	if(p == NULL) return 0;
	if(length <= 0) return 0;
//	SDL_Surface *temp = NULL;
	SDL_Surface *temp_back = NULL;
	SDL_RWops *fileData = SDL_RWFromMem((void *)p,length);
	if(fileData == 0) return 0;
	temp_back = IMG_Load_RW(fileData,1);
	if(temp_back == 0) return 0;
//	SDL_SetAlpha(temp, SDL_RLEACCEL | SDL_SRCALPHA, 255);
//	temp_back = SDL_DisplayFormatAlpha(temp);
	//������Ҫ���½�����ͼ
	int texW = texInfo.textureSize.x + texInfo.textureMove.x + texInfo.textureMove2.x;
	int texH = texInfo.textureSize.y + texInfo.textureMove.y + texInfo.textureMove2.y;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,texW,texH,32,0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
#else
	SDL_Surface * picArm = SDL_CreateRGBSurface(SDL_SWSURFACE,texW,texH,32,0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
#endif
	//��ʼ����ͼ����
	unsigned char *pData = (unsigned char *)picArm->pixels;
	unsigned char *pData1 = (unsigned char *)temp_back->pixels;
	memset(pData,0,(texW * texH) << 2);
	//����Ҫ�����ݴ洢����ͼ����
	for(int i = 0;i < texInfo.textureSize.y;++ i)
	{
		memcpy(pData + (int)((texInfo.textureMove.y + i) * texW * 4.0f) + (int)(texInfo.textureMove.x * 4.0f),
			pData1 + (int)((texInfo.textureOffset.y + i) * temp_back->w * 4.0f) + (int)(texInfo.textureOffset.x * 4.0f),
			texInfo.textureSize.x * 4.0f);
	}
	//������Ҫ����ͼ����
	savePNG(texInfo.textureName,picArm,Z_BEST_COMPRESSION);
	//�ͷ���Դ
	SDL_FreeSurface(picArm);
	picArm = NULL;
//	SDL_FreeSurface(temp);
//	temp = NULL;
	SDL_FreeSurface(temp_back);
	temp_back = NULL;
	//SDL_FreeRW(fileData);
	return 1;
}