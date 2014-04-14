//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XButton.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
#include "XResourceManager.h"

_XButtonTexture::_XButtonTexture()
:m_isInited(XFalse)
,buttonNormal(NULL)			//��ͨ״̬�µİ�ť��ͼ
,buttonDown(NULL)			//����״̬�µİ�ť��ͼ
,buttonOn(NULL)				//����״̬�µİ�ť��ͼ
,buttonDisable(NULL)		//��Ч״̬�µİ�ť��ͼ
,m_areaPointSum(0)
,m_pArea(NULL)
{
}
_XButtonTexture::~_XButtonTexture()
{
	release();
}
_XBool _XButtonTexture::init(const char *normal,const char *down,const char *on,const char *disable,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(normal == NULL) return XFalse;
	int ret = 1;

	if(normal != NULL && 
		(buttonNormal = createATextureData(normal,resoursePosition)) == NULL) ret = 0;
	if(down != NULL && ret != 0 && 
		(buttonDown = createATextureData(down,resoursePosition)) == NULL) ret = 0;
	if(on != NULL && ret != 0 && 
		(buttonOn = createATextureData(on,resoursePosition)) == NULL) ret = 0;
	if(disable != NULL && ret != 0 && 
		(buttonDisable = createATextureData(disable,resoursePosition)) == NULL) ret = 0;
	
	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}
	m_isInited = XTrue;
	return XTrue;
}
#define BUTTON_CONFIG_FILENAME ("Button.txt")
_XBool _XButtonTexture::initEx(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(filename == NULL) return XFalse;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,BUTTON_CONFIG_FILENAME);
	//�ȴ������ļ�
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition == RESOURCE_LOCAL_FOLDER)
	{//�ⲿ��Դ
		FILE *fp = NULL;
		if((fp = fopen(tempFilename,"r")) == 0) return XFalse; //��Ϣ�ļ���ȡʧ��
		//���濪ʼ���ζ�ȡ����
		int flag = 0;
		char resFilename[MAX_FILE_NAME_LENGTH] = "";
		//normal
		fscanf(fp,"%d:",&flag);
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			fclose(fp);
			return XFalse;
		}
		fscanf(fp,"%s",resFilename);
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((buttonNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			fclose(fp);
			return XFalse;
		}
		//down
		fscanf(fp,"%d:",&flag);
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		fscanf(fp,"%s",resFilename);
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((buttonDown = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		//on
		fscanf(fp,"%d:",&flag);
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		fscanf(fp,"%s",resFilename);
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((buttonOn = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		//disable
		fscanf(fp,"%d:",&flag);
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		fscanf(fp,"%s",resFilename);
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((buttonDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		//��ȡ��������
		int l,t,r,b,x,y;
		fscanf(fp,"%d,%d,%d,%d,",&l,&t,&r,&b);
		m_mouseRect.set(l,t,r,b);
		fscanf(fp,"%d,%d,",&x,&y);
		m_fontPosition.set(x,y);
		//�����ȡ�������Ϣ
		fscanf(fp,"%d,",&m_areaPointSum);
		if(m_areaPointSum > 0)
		{
			m_pArea = createArrayMem<_XVector2>(m_areaPointSum);	//rational:MLK
			if(m_pArea == NULL)
			{
				releaseTex();
				fclose(fp);
				return XFalse;
			}
			//�����ȡ����������
			for(int i = 0;i < m_areaPointSum;++ i)
			{
				fscanf(fp,"%d,%d,",&x,&y);
				m_pArea[i].set(x,y);
			}
		}
		//�������ݶ�ȡ���
		fclose(fp);
	}else
	{
		unsigned char *p = _XResourcePack::GetInstance().getFileData(tempFilename);
		if(p == NULL) return XFalse;
		//���濪ʼ���ζ�ȡ����
		int flag = 0;
		char resFilename[MAX_FILE_NAME_LENGTH] = "";
		int offset = 0;
		//normal
		sscanf((char *)(p + offset),"%d:",&flag);
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			XDELETE_ARRAY(p);
			return XFalse;
		}
		sscanf((char *)(p + offset),"%s",resFilename);
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((buttonNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//down
		sscanf((char *)(p + offset),"%d:",&flag);
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		sscanf((char *)(p + offset),"%s",resFilename);
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((buttonDown = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//on
		sscanf((char *)(p + offset),"%d:",&flag);
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		sscanf((char *)(p + offset),"%s",resFilename);
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((buttonOn = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//disable
		sscanf((char *)(p + offset),"%d:",&flag);
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		sscanf((char *)(p + offset),"%s",resFilename);
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((buttonDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//��ȡ��������
		int l,t,r,b,x,y;
		sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b);
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_mouseRect.set(l,t,r,b);
		sscanf((char *)(p + offset),"%d,%d,",&x,&y);
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_fontPosition.set(x,y);
		//�����ȡ�������Ϣ
		sscanf((char *)(p + offset),"%d,",&m_areaPointSum);
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		if(m_areaPointSum > 0)
		{
			m_pArea = createArrayMem<_XVector2>(m_areaPointSum);
			if(m_pArea == NULL)
			{
				releaseTex();
				XDELETE_ARRAY(p);
				return XFalse;
			}
			//�����ȡ����������
			for(int i = 0;i < m_areaPointSum;++ i)
			{
				sscanf((char *)(p + offset),"%d,%d,",&x,&y);
				offset += getCharPosition((char *)(p + offset),'\n') + 1;
				m_pArea[i].set(x,y);
			}
		}
		//�������ݶ�ȡ���
		XDELETE_ARRAY(p);
	}
	m_isInited = XTrue;
	return XTrue;
}
void _XButtonTexture::releaseTex()
{
	XDELETE(buttonNormal);
	XDELETE(buttonDown);
	XDELETE(buttonOn);
	XDELETE(buttonDisable);
}
void _XButtonTexture::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}

_XButton::_XButton()
:m_isInited(XFalse)
,m_buttonNormal(NULL)		
,m_buttonDown(NULL)	
,m_buttonOn(NULL)		
,m_buttonDisable(NULL)	
,m_funInit(NULL)
,m_funRelease(NULL)
,m_funMouseOn(NULL)
,m_funMouseDown(NULL)
,m_funMouseUp(NULL)
,m_pClass(NULL)
,m_hotKey(-1)
,m_nowButtonState(BUTTON_STATE_NORMAL)
,m_upMousePoint(0,0)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
{
}
_XButton::~_XButton()
{
	release();
}
void _XButton::release()
{
	_XControlManager::GetInstance().decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
}
void _XButton::setTexture(const _XButtonTexture& tex)
{
	if(tex.buttonNormal == NULL) return;//��������ͨ״̬����ͼ����Ϊ�գ�����ֱ�ӷ��ش���
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;
}
_XBool _XButton::init(const _XVector2& position,	//�ؼ����ڵ�λ��
		const _XRect& Area,				//�ؼ������Ӧ������
		const _XButtonTexture &tex,		//�ؼ�����ͼ��Ϣ
		const char *caption,const _XFontUnicode &font,float captionSize,_XVector2 textPosition)	//�ؼ�����������Ϣ
{
	if(m_isInited) return XTrue;	//����Ѿ���ʼ�������ֹ�ظ���ʼ��
	if(captionSize <= 0) return XFalse;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return XFalse;	//��ť�Ŀؼ�����Ҫ��һ�������Ӧ����
	m_mouseRect = Area;
	m_position = position;
	m_textPosition = textPosition;
	if(tex.buttonNormal == NULL) return XFalse;//��������ͨ״̬����ͼ����Ϊ�գ�����ֱ�ӷ��ش���
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;
	m_withoutTex = XFalse;

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //����������ж���
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_size.set(1.0f,1.0f);
	m_sprite.init(m_buttonNormal->texture.m_w,m_buttonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
	//������ж��룬���������λ����Ҫ����
	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_nowButtonState = BUTTON_STATE_NORMAL;

	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_BUTTON);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XButton::initEx(const _XVector2& position,	//�ؼ����ڵ�λ��
		const _XButtonTexture &tex,		//�ؼ�����ͼ��Ϣ
		const char *caption,const _XFontUnicode &font,float captionSize)
{
	if(m_isInited) return XTrue;	//����Ѿ���ʼ�������ֹ�ظ���ʼ��
	if(captionSize <= 0) return XFalse;
	if(tex.m_mouseRect.getHeight() <= 0 || tex.m_mouseRect.getWidth() <= 0) return XFalse;	//��ť�Ŀؼ�����Ҫ��һ�������Ӧ����
	m_mouseRect = tex.m_mouseRect;
	m_position = position;
	m_textPosition = tex.m_fontPosition;
	if(tex.buttonNormal == NULL) return XFalse;//��������ͨ״̬����ͼ����Ϊ�գ�����ֱ�ӷ��ش���
	m_buttonNormal = tex.buttonNormal;
	if(tex.buttonDown != NULL) m_buttonDown = tex.buttonDown;
	else m_buttonDown = tex.buttonNormal;
	if(tex.buttonOn != NULL) m_buttonOn = tex.buttonOn;
	else m_buttonOn = tex.buttonNormal;
	if(tex.buttonDisable != NULL) m_buttonDisable = tex.buttonDisable;
	else m_buttonDisable = tex.buttonNormal;
	m_withoutTex = XFalse;

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //����������ж���
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_size.set(1.0f,1.0f);
	m_sprite.init(m_buttonNormal->texture.m_w,m_buttonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
	//������ж��룬���������λ����Ҫ����
	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_nowButtonState = BUTTON_STATE_NORMAL;

	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_BUTTON);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XButton::initPlus(const char * path,const char *caption,const _XFontUnicode &font,
					   float captionSize,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XTrue;	//����Ѿ���ʼ�������ֹ�ظ���ʼ��
	if(captionSize <= 0) return XFalse;
	if(path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XBUTTON_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	_XButtonTexture * tex = (_XButtonTexture *)m_resInfo->m_pointer;
	if(tex->m_mouseRect.getHeight() <= 0 || tex->m_mouseRect.getWidth() <= 0) return XFalse;	//��ť�Ŀؼ�����Ҫ��һ�������Ӧ����
	m_mouseRect = tex->m_mouseRect;
	m_position.set(0.0f,0.0f);
	m_textPosition = tex->m_fontPosition;
	if(tex->buttonNormal == NULL) return XFalse;//��������ͨ״̬����ͼ����Ϊ�գ�����ֱ�ӷ��ش���
	m_buttonNormal = tex->buttonNormal;
	if(tex->buttonDown != NULL) m_buttonDown = tex->buttonDown;
	else m_buttonDown = tex->buttonNormal;
	if(tex->buttonOn != NULL) m_buttonOn = tex->buttonOn;
	else m_buttonOn = tex->buttonNormal;
	if(tex->buttonDisable != NULL) m_buttonDisable = tex->buttonDisable;
	else m_buttonDisable = tex->buttonNormal;
	m_withoutTex = XFalse;

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //����������ж���
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_size.set(1.0f,1.0f);
	m_sprite.init(m_buttonNormal->texture.m_w,m_buttonNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);
	//������ж��룬���������λ����Ҫ����
	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_nowButtonState = BUTTON_STATE_NORMAL;

	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_BUTTON);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XButton::initWithoutTex(const char *caption,const _XFontUnicode &font,
		float captionSize,const _XRect& area,_XVector2 textPosition)	//û����ͼ����ʽ,ֱ����д����ͼ������ͼ(��δʵ�֣�δ��ɹ���֮һ)
{
	if(m_isInited) return XTrue;	//����Ѿ���ʼ�������ֹ�ظ���ʼ��
	if(captionSize <= 0) return XFalse;
	if(area.getHeight() <= 0 || area.getWidth() <= 0) return XFalse;	//��ť�Ŀؼ�����Ҫ��һ�������Ӧ����
	m_mouseRect = area;
	m_position.set(0.0f,0.0f);
	m_textPosition = textPosition;
	m_withoutTex = XTrue;

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_MIDDLE); //����������ж���
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_size.set(1.0f,1.0f);
	//������ж��룬���������λ����Ҫ����
	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_nowButtonState = BUTTON_STATE_NORMAL;

	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_BUTTON);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void _XButton::setPosition(float x,float y)
{
	if(!m_isInited) return;
	m_position.set(x,y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(!m_withoutTex) m_sprite.setPosition(m_position);
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	updateChildPos();
}
void _XButton::setSize(float x,float y)
{
	if(!m_isInited) return;
	if(x <= 0 && y <= 0) return;
	m_size.set(x,y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	if(!m_withoutTex) m_sprite.setSize(m_size);
	mouseProc(m_upMousePoint.x,m_upMousePoint.y,MOUSE_MOVE);
	updateChildSize();
}
_XBool _XButton::canGetFocus(float x,float y)
{
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return m_nowMouseRect.isInRect(x,y);
}
_XBool _XButton::mouseProc(float x,float y,_XMouseState mouseState)
{
	m_upMousePoint.set(x,y);
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	
	if(m_nowMouseRect.isInRect(x,y))
	{//��궯���ڷ�Χ��
		if(mouseState == MOUSE_MOVE && m_nowButtonState == BUTTON_STATE_NORMAL)
		{
			m_nowButtonState = BUTTON_STATE_ON;
			if(m_funMouseOn != NULL) m_funMouseOn(m_pClass,m_objectID);
		}else
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN && (m_nowButtonState == BUTTON_STATE_NORMAL || m_nowButtonState == BUTTON_STATE_ON))
		{
			m_nowButtonState = BUTTON_STATE_DOWN;
			if(m_funMouseDown != NULL) m_funMouseDown(m_pClass,m_objectID);
		}else
		if(mouseState == MOUSE_LEFT_BUTTON_UP && m_nowButtonState == BUTTON_STATE_DOWN)
		{
			m_nowButtonState = BUTTON_STATE_ON;
			if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
		}
	}else
	{
		if(m_nowButtonState == BUTTON_STATE_ON || m_nowButtonState == BUTTON_STATE_DOWN)
		{
			m_nowButtonState = BUTTON_STATE_NORMAL;
		}
	}
	return XTrue;
}
_XBool _XButton::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�

	if(keyState == KEY_STATE_UP)
	{//��������ʱ������Ӧ
		if(m_nowButtonState == BUTTON_STATE_NORMAL)
		{//��ťֻ������ͨ״̬�²�����Ӧ��ݼ�����ֹ��ͻ
			if(keyOrder == m_hotKey || (keyOrder ==  XKEY_RETURN && m_isBeChoose))
			{
				if(m_funMouseDown != NULL) m_funMouseDown(m_pClass,m_objectID);
				if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
				return XTrue;
			}
		}
	}
	return XFalse;
}
void _XButton::draw()
{
	if(!m_isInited) return ;	//���û�г�ʼ��ֱ���˳�
	if(!m_isVisiable) return;	//������ɼ�ֱ���˳�
	if(m_withoutTex)
	{
		if(m_nowButtonState == BUTTON_STATE_NORMAL) 
		{
			drawFillBoxEx(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,
				m_mouseRect.getHeight() * m_size.y),0.75f,0.75f,0.75f);//,true,true); 
		}else
		if(m_nowButtonState == BUTTON_STATE_DOWN) 
		{
			drawFillBoxEx(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,
				m_mouseRect.getHeight() * m_size.y),0.5f,0.5f,0.5f);//,true,true); 
		}else
		if(m_nowButtonState == BUTTON_STATE_ON) 
		{
			drawFillBoxEx(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,
				m_mouseRect.getHeight() * m_size.y),0.85f,0.85f,0.85f);//,true,true);
		}else
		if(m_nowButtonState == BUTTON_STATE_DISABLE) 
		{
			drawFillBoxEx(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2(m_mouseRect.getWidth() * m_size.x,
				m_mouseRect.getHeight() * m_size.y),0.35f,0.35f,0.35f);//,true,true);
		}
	}else
	{
		if(m_nowButtonState == BUTTON_STATE_NORMAL) m_sprite.draw(m_buttonNormal); else
		if(m_nowButtonState == BUTTON_STATE_DOWN) m_sprite.draw(m_buttonDown); else
		if(m_nowButtonState == BUTTON_STATE_ON) m_sprite.draw(m_buttonOn); else
		if(m_nowButtonState == BUTTON_STATE_DISABLE) m_sprite.draw(m_buttonDisable);
	}

	//��ʾ��ť������
	m_caption.draw();
}
void _XButton::setCallbackFun(void (* funInit)(void *,int),
							  void (* funRelease)(void *,int),
							  void (* funMouseOn)(void *,int),
							  void (* funMouseDown)(void *,int),
							  void (* funMouseUp)(void *,int),void *pClass)
{
	if(funInit != NULL) m_funInit = funInit;
	if(funRelease != NULL) m_funRelease = funRelease;
	if(funMouseOn != NULL) m_funMouseOn = funMouseOn;
	if(funMouseDown != NULL) m_funMouseDown = funMouseDown;
	if(funMouseUp != NULL) m_funMouseUp = funMouseUp;
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
_XBool _XButton::setACopy(const _XButton &temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited) return XFalse;
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_BUTTON);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}
	m_isInited = temp.m_isInited;					//��ť�Ƿ񱻳�ʼ��
	m_nowButtonState = temp.m_nowButtonState;		//��ǰ�İ�ť״̬

	if(!m_caption.setACopy(temp.m_caption))	 return XFalse;
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif

	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_buttonNormal = temp.m_buttonNormal;			//��ͨ״̬�µİ�ť��ͼ(������Σ������ͼһ��Ҫ��)
	m_buttonDown = temp.m_buttonDown;			//����״̬�µİ�ť��ͼ
	m_buttonOn = temp.m_buttonOn;				//����״̬�µİ�ť��ͼ
	m_buttonDisable = temp.m_buttonDisable;			//��Ч״̬�µİ�ť��ͼ

	m_funInit = temp.m_funInit;
	m_funRelease = temp.m_funRelease;
	m_funMouseOn = temp.m_funMouseOn;
	m_funMouseDown = temp.m_funMouseDown;
	m_funMouseUp = temp.m_funMouseUp;
	m_pClass = temp.m_pClass;				//�ص������Ĳ���

	m_sprite.setACopy(temp.m_sprite);	//������ʾ��ͼ�ľ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_textPosition = temp.m_textPosition;	//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize = temp.m_textSize;		//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	m_upMousePoint = temp.m_upMousePoint;	//�ϴμ�¼�����λ��

	m_hotKey = temp.m_hotKey;
	return XTrue;
}
_XBool _XButton::isInRect(float x,float y)		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
{
	if(!m_isInited) return XFalse;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
_XVector2 _XButton::getBox(int order)			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
{	
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(!m_isInited) return ret;

	if(order == 0) ret.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y);else
	if(order == 1) ret.set(m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.top * m_size.y);else
	if(order == 2) ret.set(m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);else
	if(order == 3) ret.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	return ret;
}