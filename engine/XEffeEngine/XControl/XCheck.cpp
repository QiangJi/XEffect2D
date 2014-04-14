//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XCheck.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
#include "XResourceManager.h"

_XCheckTexture::_XCheckTexture()
:m_isInited(XFalse)
,checkChoosed(NULL)			//ѡ��ťѡ�е���ͼ		
,checkDischoose(NULL)			//ѡ��ťδѡ�е���ͼ		
,checkDisableChoosed(NULL)		//��Ч״̬��ѡ��ťѡ�е���ͼ		
,checkDisableDischoose(NULL)	//��Ч״̬��ѡ��ťδѡ�е���ͼ	
{
}
_XCheckTexture::~_XCheckTexture()
{
	release();
}
_XBool _XCheckTexture::init(const char *choosed,const char *disChoose,const char *disableChoosed,const char *disableDischoose,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	//ע����������״̬����ͼ������Ҫ�У����򽫲��ܳ�ʼ��
	if(choosed == NULL || disChoose == NULL || disableChoosed == NULL || disableDischoose == NULL) return 0;
	int ret = 1;
	
	if((checkChoosed = createATextureData(choosed,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(checkDischoose = createATextureData(disChoose,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(checkDisableChoosed = createATextureData(disableChoosed,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(checkDisableDischoose = createATextureData(disableDischoose,resoursePosition)) == NULL) ret = 0;
	
	if(ret == 0)
	{
		releaseTex();
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
#define CHECK_CONFIG_FILENAME ("Check.txt")
_XBool _XCheckTexture::initEx(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(filename == NULL) return XFalse;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,CHECK_CONFIG_FILENAME);
	//�ȴ������ļ�
	if(resoursePosition == RESOURCE_SYSTEM_DEFINE) resoursePosition = XEE::defaultResourcePosition;
	if(resoursePosition == RESOURCE_LOCAL_FOLDER)
	{//�ⲿ��Դ
		FILE *fp = NULL;
		if((fp = fopen(tempFilename,"r")) == 0) return 0; //��Ϣ�ļ���ȡʧ��
		//���濪ʼ���ζ�ȡ����
		int flag = 0;
		char resFilename[MAX_FILE_NAME_LENGTH] = "";
		//chooose
		fscanf(fp,"%d:",&flag);
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			fclose(fp);
			return XFalse;
		}
		fscanf(fp,"%s",resFilename);
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkChoosed = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			fclose(fp);
			return XFalse;
		}
		//dischoose
		fscanf(fp,"%d:",&flag);
		if(flag == 0)
		{//û����ͨ״̬�ǲ��е�
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		fscanf(fp,"%s",resFilename);
		sprintf(tempFilename,"%s/%s",filename,resFilename);
		if((checkDischoose = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((checkDisableChoosed = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((checkDisableDischoose = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((checkChoosed = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((checkDischoose = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((checkDisableChoosed = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((checkDisableDischoose = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		//�������ݶ�ȡ���
		XDELETE_ARRAY(p);
	}
	m_isInited = XTrue;
	return XTrue;
}
void _XCheckTexture::releaseTex()
{
	XDELETE(checkChoosed);
	XDELETE(checkDischoose);
	XDELETE(checkDisableChoosed);
	XDELETE(checkDisableDischoose);
}
void _XCheckTexture::release()
{
	if(!m_isInited) return;
	releaseTex();
	m_isInited = XFalse;
}

_XCheck::_XCheck()
:m_isInited(XFalse)					//�������Ƿ񱻳�ʼ��
,m_checkChoosed(NULL)			//ѡ��ťѡ�е���ͼ
,m_checkDischoose(NULL)		//ѡ��ťδѡ�е���ͼ
,m_checkDisableChoosed(NULL)	//��Ч״̬��ѡ��ťѡ�е���ͼ
,m_checkDisableDischoose(NULL)	//��Ч״̬��ѡ��ťδѡ�е���ͼ
,m_funInit(NULL)				//�ؼ���ʼ����ʱ����ã������Ŀǰû��ʵ����Ч��
,m_funRelease(NULL)				//�ؼ�ע����ʱ����ã������Ŀǰû��ʵ����Ч��
,m_funMouseOn(NULL)				//�������ʱ����
,m_funMouseDown(NULL)			//��갴��ʱ����		
,m_funMouseUp(NULL)				//��굯��ʱ����
,m_funStateChange(NULL)			//�ؼ�״̬�ı�ʱ����
,m_pClass(NULL)
,m_state(XFalse)
,m_withCaption(1)
,m_pVariable(NULL)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
{
}
_XCheck::~_XCheck()
{
	release();
}
void _XCheck::release()
{
	_XControlManager::GetInstance().decreaseAObject(this);	//ע��������
	if(m_funRelease != NULL) m_funRelease(m_pClass,m_objectID);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
}
//ע�⣺����Ĭ���ĸ�״̬����ͼ�ߴ�һ�£����򽫻�������벻��������
_XBool _XCheck::init(const _XVector2 & position,	//�ؼ���λ��
		const _XRect &Area,					//�ؼ���ͼƬ����Ӧ��Χ
		const _XCheckTexture &tex,			//�ؼ�����ͼ
		const char *caption,const _XFontUnicode &font,float captionSize,	//��ʾ������������Ϣ
		const _XVector2 &textPosition)		//�����λ��
{
	if(m_isInited) return XFalse;
	if(Area.getHeight() <= 0 || Area.getWidth() <= 0) return XFalse;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	m_position = position;
	m_mouseRect = Area;

	if(tex.checkChoosed == NULL || tex.checkDisableChoosed == NULL 
		|| tex.checkDisableDischoose == NULL || tex.checkDischoose == NULL) return XFalse;
	m_checkChoosed = tex.checkChoosed;			//ѡ��ťѡ�е���ͼ
	m_checkDischoose = tex.checkDischoose;		//ѡ��ťδѡ�е���ͼ
	m_checkDisableChoosed = tex.checkDisableChoosed;	//��Ч״̬��ѡ��ťѡ�е���ͼ
	m_checkDisableDischoose = tex.checkDisableDischoose;	//��Ч״̬��ѡ��ťδѡ�е���ͼ
	m_withoutTex = XFalse;

	m_caption.setACopy(font);			//�������ı���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_size.set(1.0f,1.0f);
	m_textPosition = textPosition;		//���ֵ����λ��

	m_sprite.init(m_checkChoosed->texture.m_w,m_checkChoosed->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(captionSize < 0) return XFalse;
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_state = XFalse;				//��ѡ���ѡ��״̬
	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_CHECK);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XCheck::initEx(const _XVector2 & position,	//�ؼ���λ��
		const _XCheckTexture &tex,			//�ؼ�����ͼ
		const char *caption,const _XFontUnicode &font,float captionSize)	//��ʾ������������Ϣ
{
	if(m_isInited) return XFalse;
	if(tex.m_mouseRect.getHeight() <= 0 || tex.m_mouseRect.getWidth() <= 0) return XFalse;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	m_position = position;
	m_mouseRect = tex.m_mouseRect;

	if(tex.checkChoosed == NULL || tex.checkDisableChoosed == NULL 
		|| tex.checkDisableDischoose == NULL || tex.checkDischoose == NULL) return XFalse;
	m_checkChoosed = tex.checkChoosed;			//ѡ��ťѡ�е���ͼ
	m_checkDischoose = tex.checkDischoose;		//ѡ��ťδѡ�е���ͼ
	m_checkDisableChoosed = tex.checkDisableChoosed;	//��Ч״̬��ѡ��ťѡ�е���ͼ
	m_checkDisableDischoose = tex.checkDisableDischoose;	//��Ч״̬��ѡ��ťδѡ�е���ͼ
	m_withoutTex = XFalse;

	m_caption.setACopy(font);			//�������ı���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_size.set(1.0f,1.0f);
	m_textPosition = tex.m_fontPosition;		//���ֵ����λ��

	m_sprite.init(m_checkChoosed->texture.m_w,m_checkChoosed->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(captionSize < 0) return 0;
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_state = XFalse;				//��ѡ���ѡ��״̬
	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_CHECK);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XCheck::initPlus(const char * path,			//�ؼ�����ͼ
		const char *caption,const _XFontUnicode &font,
		float captionSize,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XCHECK_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	_XCheckTexture * tex = (_XCheckTexture *)m_resInfo->m_pointer;

	if(tex->m_mouseRect.getHeight() <= 0 || tex->m_mouseRect.getWidth() <= 0) return XFalse;	//�ռ����Ҫ��һ����Ӧ���䣬��Ȼ����ֳ������
	m_position.set(0.0f,0.0f);
	m_mouseRect = tex->m_mouseRect;

	if(tex->checkChoosed == NULL || tex->checkDisableChoosed == NULL 
		|| tex->checkDisableDischoose == NULL || tex->checkDischoose == NULL) return XFalse;
	m_checkChoosed = tex->checkChoosed;			//ѡ��ťѡ�е���ͼ
	m_checkDischoose = tex->checkDischoose;		//ѡ��ťδѡ�е���ͼ
	m_checkDisableChoosed = tex->checkDisableChoosed;	//��Ч״̬��ѡ��ťѡ�е���ͼ
	m_checkDisableDischoose = tex->checkDisableDischoose;	//��Ч״̬��ѡ��ťδѡ�е���ͼ
	m_withoutTex = XFalse;

	m_caption.setACopy(font);			//�������ı���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_size.set(1.0f,1.0f);
	m_textPosition = tex->m_fontPosition;		//���ֵ����λ��

	m_sprite.init(m_checkChoosed->texture.m_w,m_checkChoosed->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_sprite.setPosition(m_position);
	m_sprite.setSize(m_size);
	m_sprite.setIsTransformCenter(POINT_LEFT_TOP);

	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(captionSize < 0) return XFalse;
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_state = XFalse;				//��ѡ���ѡ��״̬
	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_CHECK);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XCheck::initWithoutTex(const char *caption,const _XFontUnicode &font,
	float captionSize,const _XRect& area,_XVector2 captionPosition)
{//��δ���
	if(m_isInited) return XFalse;

	m_position.set(0.0f,0.0f);
	m_mouseRect = area;

	m_withoutTex = XTrue;

	m_caption.setACopy(font);			//�������ı���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_size.set(1.0f,1.0f);
	m_textPosition = captionPosition;		//���ֵ����λ��

	m_caption.setString(caption);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(captionSize < 0) return XFalse;
	m_textSize.set(captionSize,captionSize);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);

	m_state = XFalse;				//��ѡ���ѡ��״̬
	if(m_funInit != NULL) m_funInit(m_pClass,m_objectID);

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_CHECK);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void _XCheck::draw()
{
	if(!m_isInited) return ;	//���û�г�ʼ��ֱ���˳�
	if(!m_isVisiable) return;	//������ɼ�ֱ���˳�
	if(m_withoutTex)
	{
		if(m_state)
		{
			if(m_isEnable) 
			{//����X
				drawFillBoxEx(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
					_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.75f,0.75f,0.75f);
				drawLine(m_position.x + m_mouseRect.left * m_size.x + 2,
					m_position.y + m_mouseRect.top * m_size.y + 2,
					m_position.x + m_mouseRect.right * m_size.x - 2,
					m_position.y + m_mouseRect.bottom * m_size.y - 2,1,0.5f,0.5f,0.5f);
				drawLine(m_position.x + m_mouseRect.left * m_size.x + 2,
					m_position.y + m_mouseRect.bottom * m_size.y - 2,
					m_position.x + m_mouseRect.right * m_size.x - 2,
					m_position.y + m_mouseRect.top * m_size.y + 2,1,0.5f,0.5f,0.5f);
			}else 
			{
				drawFillBoxEx(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
					_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.5f,0.5f,0.5f);
				drawLine(m_position.x + m_mouseRect.left * m_size.x + 2,
					m_position.y + m_mouseRect.top * m_size.y + 2,
					m_position.x + m_mouseRect.right * m_size.x - 2,
					m_position.y + m_mouseRect.bottom * m_size.y - 2,1,0.25f,0.25f,0.25f);
				drawLine(m_position.x + m_mouseRect.left * m_size.x + 2,
					m_position.y + m_mouseRect.bottom * m_size.y - 2,
					m_position.x + m_mouseRect.right * m_size.x - 2,
					m_position.y + m_mouseRect.top * m_size.y + 2,1,0.25f,0.25f,0.25f);
			}
		}else
		{
			if(m_isEnable) 
			{
				drawFillBoxEx(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
					_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.85f,0.85f,0.85f);
			}else 
			{
				drawFillBoxEx(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
					_XVector2(m_mouseRect.getWidth() * m_size.x,m_mouseRect.getHeight() * m_size.y),0.5f,0.5f,0.5f);
			}
		}
	}else
	{
		if(m_state)
		{
			if(m_isEnable) m_sprite.draw(m_checkChoosed);
			else m_sprite.draw(m_checkDisableChoosed);
		}else
		{
			if(m_isEnable) m_sprite.draw(m_checkDischoose);
			else m_sprite.draw(m_checkDisableDischoose);
		}
	}
	if(m_withCaption != 0) m_caption.draw();	//�����Ҫ�������������
}
void _XCheck::setPosition(float x,float y)
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_position.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	if(!m_withoutTex) m_sprite.setPosition(m_position);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	updateChildPos();
}
void _XCheck::setSize(float x,float y)
{
	if(x <= 0 && y <= 0) return;
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	m_size.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	if(!m_withoutTex) m_sprite.setSize(m_size);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	updateChildSize();
}
void _XCheck::setText(const char *temp)
{
	if(temp == NULL) return;
	m_caption.setString(temp);
	m_mouseClickArea.set(m_caption.getBox(0).x,m_caption.getBox(0).y,m_caption.getBox(2).x,m_caption.getBox(2).y);
}
_XBool _XCheck::canGetFocus(float x,float y)
{
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	if(m_withCaption)
	{
		if(m_mouseClickArea.isInRect(x,y) || m_nowMouseRect.isInRect(x,y)) return XTrue;
	}else
	{
		if(m_nowMouseRect.isInRect(x,y)) return XTrue;
	}
	return XFalse;
}
_XBool _XCheck::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�

	if(mouseState == MOUSE_LEFT_BUTTON_UP)
	{
		if(m_withCaption)
		{
			if(m_mouseClickArea.isInRect(x,y) || m_nowMouseRect.isInRect(x,y))
			{//������Ч��ִ����ز���
				m_state = !m_state;
				if(m_pVariable != NULL) *m_pVariable = m_state;
				if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
				if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
				return XTrue;
			}
		}else
		{
			if(m_nowMouseRect.isInRect(x,y))
			{//������Ч��ִ����ز���
				m_state = !m_state;
				if(m_pVariable != NULL) *m_pVariable = m_state;
				if(m_funMouseUp != NULL) m_funMouseUp(m_pClass,m_objectID);
				if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
				return XTrue;
			}
		}
	}
	return XFalse;
}
_XBool _XCheck::keyboardProc(int keyOrder,_XKeyState keyState)
{//�س����߿ո���Ըı�����ؼ���ѡ��״̬
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�

	if(keyState == KEY_STATE_UP)
	{//��������ʱ������Ӧ
		if((keyOrder == XKEY_RETURN || keyOrder == XKEY_SPACE) && m_isBeChoose != 0)
		{//���¿ո�����߻س�������Ч
			m_state = !m_state;
			if(m_pVariable != NULL) *m_pVariable = m_state;
			if(m_funStateChange != NULL) m_funStateChange(m_pClass,m_objectID);
		}
	}
	return XTrue;
}
void _XCheck::setCallbackFun(void (* funInit)(void *,int),
							 void (* funRelease)(void *,int),
							 void (* funMouseOn)(void *,int),
							 void (* funMouseDown)(void *,int),
							 void (* funMouseUp)(void *,int),
							 void (*funStateChange)(void *,int),void *pClass)
{//ʵ����������Բ������жϣ��������������ע��
	if(funInit != NULL) m_funInit = funInit;				//�ؼ���ʼ����ʱ����ã������Ŀǰû��ʵ����Ч��
	if(funRelease != NULL) m_funRelease = funRelease;				//�ؼ�ע����ʱ����ã������Ŀǰû��ʵ����Ч��
	if(funMouseOn != NULL) m_funMouseOn = funMouseOn;				//�������ʱ����
	if(funMouseDown != NULL) m_funMouseDown = funMouseDown;			//��갴��ʱ����		
	if(funMouseUp != NULL) m_funMouseUp = funMouseUp;				//��굯��ʱ����
	if(funStateChange != NULL) m_funStateChange = funStateChange;			//�ؼ�״̬�ı�ʱ����
	if(pClass != NULL) m_pClass = pClass;
	else m_pClass = this;
}
_XBool _XCheck::setACopy(const _XCheck &temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited) return XFalse;
	if(!_XControlBasic::setACopy(temp)) return XFalse;

	if(!m_isInited)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_CHECK);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}

	m_isInited = temp.m_isInited;					//�������Ƿ񱻳�ʼ��
	if(!m_caption.setACopy(temp.m_caption))	return XFalse;		//�������ı���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif

	m_pVariable = temp.m_pVariable;
	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_checkChoosed = temp.m_checkChoosed;			//ѡ��ťѡ�е���ͼ
	m_checkDischoose = temp.m_checkDischoose;		//ѡ��ťδѡ�е���ͼ
	m_checkDisableChoosed = temp.m_checkDisableChoosed;	//��Ч״̬��ѡ��ťѡ�е���ͼ
	m_checkDisableDischoose = temp.m_checkDisableDischoose;	//��Ч״̬��ѡ��ťδѡ�е���ͼ

	m_funInit = temp.m_funInit;				//�ؼ���ʼ����ʱ����ã������Ŀǰû��ʵ����Ч��
	m_funRelease = temp.m_funRelease;				//�ؼ�ע����ʱ����ã������Ŀǰû��ʵ����Ч��
	m_funMouseOn = temp.m_funMouseOn;				//�������ʱ���ã������Ŀǰû��ʵ����Ч��
	m_funMouseDown = temp.m_funMouseDown;			//��갴��ʱ���ã������Ŀǰû��ʵ����Ч��		
	m_funMouseUp = temp.m_funMouseUp;				//��굯��ʱ����
	m_funStateChange = temp.m_funStateChange;			//�ؼ�״̬�ı�ʱ����
	m_pClass = temp.m_pClass;

	m_withCaption = temp.m_withCaption;
	m_sprite.setACopy(temp.m_sprite);			//������ʾ��ͼ�ľ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_sprite);
#endif
	m_textPosition = temp.m_textPosition;		//���ֵ����λ��
	m_textSize = temp.m_textSize;			//���ֵĳߴ�

	m_state = temp.m_state;				//��ѡ���ѡ��״̬
	m_textColor = temp.m_textColor;				//��ѡ���ѡ��״̬
	m_mouseClickArea = temp.m_mouseClickArea;	//���������Ӧ��Χ

	return XTrue;
}
_XBool _XCheck::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
_XVector2 _XCheck::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;
	float left = 0.0f;
	float right = 0.0f;
	float top = 0.0f;
	float bottom = 0.0f;
	if(m_withCaption)
	{
		if(m_nowMouseRect.left <= m_mouseClickArea.left) left = m_nowMouseRect.left;
		else left = m_mouseClickArea.left;
		if(m_nowMouseRect.top <= m_mouseClickArea.top) top = m_nowMouseRect.top;
		else top = m_mouseClickArea.top;
		if(m_nowMouseRect.right <= m_mouseClickArea.right) right = m_mouseClickArea.right;
		else right = m_nowMouseRect.right;
		if(m_nowMouseRect.bottom <= m_mouseClickArea.bottom) bottom = m_mouseClickArea.bottom;
		else bottom = m_nowMouseRect.bottom;
	}else
	{
		left = m_nowMouseRect.left;
		top = m_nowMouseRect.top;
		right = m_mouseClickArea.right;
		bottom = m_mouseClickArea.bottom;
	}

	if(order == 0) ret.set(left,top);else
	if(order == 1) ret.set(right,top);else
	if(order == 2) ret.set(right,bottom);else
	if(order == 3) ret.set(left,bottom);

	return ret;
}