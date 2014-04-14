//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XMultiList.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourcePack.h"
#include "XResourceManager.h"

void funMutiListValueChangeV(void *pClass,int objectID)
{
	_XMultiList *pPar = (_XMultiList *)pClass;
	int temp = pPar->m_verticalSlider.getNowValue();
	if(temp != pPar->m_showStartLine)
	{
		pPar->m_showStartLine = temp;
		pPar->updateSelectLine();
		pPar->updateShowChange();
		pPar->m_verticalSlider.setNowValue(temp);
	}else
	{//΢���ƶ�ʱ��Ĵ���
		if(pPar->m_verticalSlider.getNowValue() > pPar->m_showStartLine)
		{
			temp += 1;
			pPar->m_showStartLine = temp;
			pPar->updateSelectLine();
			pPar->updateShowChange();
			pPar->m_verticalSlider.setNowValue(temp);
		}else
		if(pPar->m_verticalSlider.getNowValue() < pPar->m_showStartLine)
		{
			temp -= 1;
			pPar->m_showStartLine = temp;
			pPar->updateSelectLine();
			pPar->updateShowChange();
			pPar->m_verticalSlider.setNowValue(temp);
		}
	}
}
void funMutiListValueChangeMoveV(void *pClass,int objectID)
{
	_XMultiList *pPar = (_XMultiList *)pClass;
	int temp = pPar->m_verticalSlider.getNowValue();
	if(temp != pPar->m_showStartLine)
	{
		//��Ҫ���´�ֱƬѡ�����
		pPar->m_showStartLine = temp;
		pPar->updateSelectLine();
		pPar->updateShowChange();
	}
}
void funMutiListValueChangeH(void *pClass,int objectID)
{
	_XMultiList *pPar = (_XMultiList *)pClass;
	int temp = pPar->m_horizontalSlider.getNowValue();
	if(temp != pPar->m_showStartRow)
	{
		pPar->m_showStartRow = temp;
		pPar->updateShowChange();
		pPar->m_horizontalSlider.setNowValue(temp);
	}else
	{
		if(pPar->m_horizontalSlider.getNowValue() > pPar->m_showStartRow)
		{
			temp += 1;
			pPar->m_showStartRow = temp;
			pPar->updateShowChange();
			pPar->m_horizontalSlider.setNowValue(temp);
		}else
		if(pPar->m_horizontalSlider.getNowValue() < pPar->m_showStartRow)
		{
			temp -= 1;
			pPar->m_showStartRow = temp;
			pPar->updateShowChange();
			pPar->m_horizontalSlider.setNowValue(temp);
		}
	}
}
void funMutiListValueChangeMoveH(void *pClass,int objectID)
{
	_XMultiList *pPar = (_XMultiList *)pClass;
	int temp = pPar->m_horizontalSlider.getNowValue();
	if(temp != pPar->m_showStartRow)
	{
		pPar->m_showStartRow = temp;
		pPar->updateShowChange();
	}
}
_XMultiListTexture::_XMultiListTexture()
:m_isInited(XFalse)
,mutiListNormal(NULL)			//�����б�����ͨ״̬
,mutiListDisable(NULL)			//�����б�����Ч״̬
,mutiListSelect(NULL)			//�����б��б�ѡ���еı���
,mutiListMove(NULL)			//�ƶ�����ʱ�Ķ����ǩ
,mutiListTitle(NULL)			//�����б��ı��ⱳ��
,mutiListTitleEnd(NULL)		//�����б��ı���ָ���
{
}
_XMultiListTexture::~_XMultiListTexture()
{
	release();
}
_XBool _XMultiListTexture::init(const char *normal,const char *disable,const char *select,const char *move,
							const char *title,const char *titleEnd,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	//�������е���ͼ������Ϊ��
	if(normal == NULL || disable == NULL || select == NULL || 
		move == NULL || title == NULL || titleEnd == NULL) return XFalse;

	int ret = 1;
	//���濪ʼ������ͼ
	if((mutiListNormal = createATextureData(normal,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 && 
		(mutiListSelect = createATextureData(select,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 && 
		(mutiListDisable = createATextureData(disable,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 && 
		(mutiListMove = createATextureData(move,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 && 
		(mutiListTitle = createATextureData(title,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 && 
		(mutiListTitleEnd = createATextureData(titleEnd,resoursePosition)) == NULL) ret = 0;

	if(ret == 0)
	{
		XDELETE(mutiListNormal);
		XDELETE(mutiListDisable);
		XDELETE(mutiListSelect);
		XDELETE(mutiListMove);
		XDELETE(mutiListTitle);
		XDELETE(mutiListTitleEnd);
		return XFalse;
	}

	m_isInited = XTrue;
	return XTrue;
}
void _XMultiListTexture::releaseTex()
{
	XDELETE(mutiListNormal);
	XDELETE(mutiListDisable);
	XDELETE(mutiListSelect);
	XDELETE(mutiListMove);
	XDELETE(mutiListTitle);
	XDELETE(mutiListTitleEnd);
}
#define MULTILIST_CONFIG_FILENAME ("MultiList.txt")
_XBool _XMultiListTexture::initEx(const char *filename,_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;
	if(filename == NULL) return XFalse;
	char tempFilename[MAX_FILE_NAME_LENGTH];
	sprintf(tempFilename,"%s/%s",filename,MULTILIST_CONFIG_FILENAME);
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
		if((mutiListNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((mutiListDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((mutiListSelect = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((mutiListMove = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((mutiListTitle = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((mutiListTitleEnd = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			fclose(fp);
			return XFalse;
		}
		//��ȡ��������
		int l,t,r,b;
		fscanf(fp,"%d,%d,%d,%d,",&l,&t,&r,&b);
		m_mouseRect.set(l,t,r,b);
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
		if((mutiListNormal = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((mutiListDisable = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((mutiListSelect = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((mutiListMove = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((mutiListTitle = createATextureData(tempFilename,resoursePosition)) == NULL)
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
		if((mutiListTitleEnd = createATextureData(tempFilename,resoursePosition)) == NULL)
		{//��Դ��ȡʧ��
			releaseTex();
			XDELETE_ARRAY(p);
			return XFalse;
		}
		//��ȡ��������
		int l,t,r,b;
		sscanf((char *)(p + offset),"%d,%d,%d,%d,",&l,&t,&r,&b);
		offset += getCharPosition((char *)(p + offset),'\n') + 1;
		m_mouseRect.set(l,t,r,b);
		//�������ݶ�ȡ���
		XDELETE_ARRAY(p);
	}
	m_isInited = XTrue;
	return XTrue;
}
void _XMultiListTexture::release()
{
	if(!m_isInited) return;
	XDELETE(mutiListNormal);
	XDELETE(mutiListDisable);
	XDELETE(mutiListSelect);
	XDELETE(mutiListMove);
	XDELETE(mutiListTitle);
	XDELETE(mutiListTitleEnd);
	m_isInited = XFalse;
}
_XMultiList::_XMultiList()
:m_isInited(XFalse)					//�Ƿ��ʼ��
,m_mutiListNormal(NULL)			//�����б�����ͨ״̬
,m_mutiListDisable(NULL)		//�����б�����Ч״̬
,m_mutiListSelect(NULL)			//�����б��б�ѡ���еı���
,m_mutiListMove(NULL)			//�ƶ�����ʱ�Ķ����ǩ
,m_mutiListTitle(NULL)			//�����б��ı��ⱳ��
,m_mutiListTitleEnd(NULL)		//�����б��ı���ָ���
,m_needShowVSlider(XFalse)			//�Ƿ���Ҫ��ʾ��ֱ������
,m_needShowHSlider(XFalse)			//�Ƿ���Ҫ��ʾˮƽ������
,m_tableRowSum(0)				//�����е�����
,m_tableRow(NULL)				//�е�����
,m_tableLineSum(0)				//�����е�����
,m_tableBox(NULL)				//����Ԫ�ص�����
,m_mouseLeftButtonDown(XFalse)
,m_mouseMoveDown(XFalse)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
,m_funSelectFun(NULL)
,m_withMouseDrag(XTrue)
{
}
_XMultiList::~_XMultiList()
{
	release();
}
void _XMultiList::releaseTempMemory()
{
	if(m_tableRow != NULL)
	{
		//ɾ�����е�����Ϣ
		_XMultiListOneRow *tempRow = m_tableRow;
		_XMultiListOneRow *temp;
		for(int i = 0;i < m_tableRowSum;++ i)
		{
			temp = tempRow;
			tempRow = tempRow->nextRow;
			XDELETE_ARRAY(temp->title);
			XDELETE(temp);
		}
		m_tableRow = NULL;
	}

	if(m_tableBox != NULL)
	{
		//ɾ�����б����е�Ԫ��
		_XMultiListOneBox *tempBox = m_tableBox;
		_XMultiListOneBox *temp1;
		for(int i = 0;i < m_tableLineSum;++ i)
		{
			for(int j = 0;j < m_tableRowSum;++ j)
			{
				temp1 = tempBox;
				tempBox = tempBox->nextBox;
				XDELETE_ARRAY(temp1->string);
				XDELETE(temp1);
			}
		}
		m_tableBox = NULL;
	}
	m_tableLineSum = 0;
	m_tableRowSum = 0;
}
void _XMultiList::release()
{	
	if(!m_isInited) return;
	releaseTempMemory();

	_XControlManager::GetInstance().decreaseAObject(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	if(m_resInfo != NULL)
	{
		_XResourceManager::GetInstance().releaseResource(m_resInfo);
		m_resInfo = NULL;
	}
	m_isInited = XFalse;
}
_XBool _XMultiList::init(const _XVector2& position,		//�ռ����ڵ�λ��
		const _XRect& Area,					//�ؼ���ʵ����ʾ����
		const _XMultiListTexture &tex,		//�ؼ�����ͼ
		const _XFontUnicode &font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum,				//�ؼ��е�����
		//const _XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		const _XSlider &vSlider,	//��ֱ������
		const _XSlider &hSlider)	//ˮƽ������
{
	if(m_isInited) return XFalse;	//�ظ���ʼ��
	//�����ǲ������
	if(Area.getWidth() <= 0 || Area.getHeight() <= 0) return XFalse;	//���뷶Χ����Ϊ��
	if(tex.mutiListDisable == NULL || tex.mutiListMove == NULL || tex.mutiListNormal == NULL
		|| tex.mutiListSelect == NULL || tex.mutiListTitle == NULL || tex.mutiListTitleEnd == NULL)
	{//��Ҫ����ͼ������
		return XFalse;
	}
	if(rowSum < 0 || lineSum < 0 || (rowSum == 0 && lineSum > 0))
	{//����Ϊ����
		return XFalse;
	}
	if(rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;
	//���濪ʼ��ֵ
	m_position = position;
	m_mouseRect = Area;
	m_withoutTex = XFalse;

	m_mutiListNormal = tex.mutiListNormal;			//�����б�����ͨ״̬
	m_mutiListDisable = tex.mutiListDisable;			//�����б�����Ч״̬
	m_mutiListSelect = tex.mutiListSelect;			//�����б��б�ѡ���еı���
	m_mutiListMove = tex.mutiListMove;			//�ƶ�����ʱ�Ķ����ǩ
	m_mutiListTitle = tex.mutiListTitle;			//�����б��ı��ⱳ��
	m_mutiListTitleEnd = tex.mutiListTitleEnd;		//�����б��ı���ָ���

	m_size.set(1.0f,1.0f);
	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(strSize,strSize);
	m_caption.setSize(m_fontSize);
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;

	m_spriteBackGround.init(m_mutiListNormal->texture.m_w,m_mutiListNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setSize(m_size);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_mutiListSelect->texture.m_w,m_mutiListSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setSize(m_size);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteMove.init(m_mutiListMove->texture.m_w,m_mutiListMove->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteMove);
#endif
	m_spriteMove.setSize(m_size);
	m_spriteMove.setIsTransformCenter(POINT_LEFT_TOP);

	m_haveSelect = XFalse;		//�Ƿ���ѡ��
	m_selectLineOrder = -1;	//ѡ�������һ��
	m_showStartLine = 0;	//��ʾ���Ǵӵڼ��п�ʼ��
	m_showStartRow = 0;		//��ʾ�ӵڼ��п�ʼ��
	m_needShowMove = XFalse;		//�Ƿ���Ҫ��ʾ�ƶ�������

	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������
	m_verticalSlider.setACopy(vSlider);		//��ֱ������
	_XControlManager::GetInstance().decreaseAObject(&m_verticalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_verticalSlider.setSize(m_size);
	m_verticalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiListValueChangeV,funMutiListValueChangeMoveV,this);

	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������
	m_horizontalSlider.setACopy(hSlider);	//ˮƽ������
	_XControlManager::GetInstance().decreaseAObject(&m_horizontalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	m_horizontalSlider.setSize(m_size);
	m_horizontalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiListValueChangeH,funMutiListValueChangeMoveH,this);

	m_tableRowSum = rowSum;		//�����е�����
	//Ϊ�е����������ڴ�ռ�
	if(m_tableRowSum == 0)
	{
		m_tableRow = NULL;
	}else
	{
		m_tableRow = createMem<_XMultiListOneRow>();
		if(m_tableRow == NULL) return 0;

		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setSize(m_size * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_nowTextHeight);
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_size.x,
			m_position.y + m_mouseRect.top * m_size.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		m_tableRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableRow->m_spriteTitle));
#endif
		m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitle.setSize(m_tableRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
		m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
		m_tableRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableRow->m_spriteTitleEnd));
#endif
		m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_size.x - 
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
			m_tableRow->position.y);
		m_tableRow->m_spriteTitleEnd.setSize(m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
			m_tableRow->pixSize.y * m_size.y/m_mutiListTitle->textureSize.y);
		m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_size.x / m_nowTextWidth;
		m_tableRow->title = NULL;
		setTileStr("Title",0);	//����Ĭ����ʾ
		m_tableRow->isShow = 1;

		m_tableRow->needChip = 0;					//�Ƿ���Ҫ�и�
		m_tableRow->left = 0;						//��ߵ��и�λ��
		m_tableRow->right = 0;					//�ұߵ��и�λ��

		_XMultiListOneRow *tempRow = m_tableRow;
		for(int i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow = createMem<_XMultiListOneRow>();
			if(tempRow->nextRow == NULL) 
			{
				releaseTempMemory();
				return 0;
			}
			tempRow->nextRow->isEnable = XTrue;
			tempRow->nextRow->order = i;
			tempRow->nextRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().decreaseAObject(&(tempRow->nextRow->text));
#endif
			tempRow->nextRow->text.setSize(m_size * m_fontSize);
			tempRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_nowTextHeight);
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_size.x,
				m_position.y + m_mouseRect.top * m_size.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			tempRow->nextRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().decreaseAObject(&(tempRow->nextRow->m_spriteTitle));
#endif
			tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->m_spriteTitle.setSize(m_tableRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
			tempRow->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
			tempRow->nextRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().decreaseAObject(&(tempRow->nextRow->m_spriteTitleEnd));
#endif
			tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x * m_size.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			tempRow->nextRow->m_spriteTitleEnd.setSize(m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y);
			tempRow->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

			tempRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_size.x / m_nowTextWidth;
			tempRow->nextRow->title = NULL;
			setTileStr("Title",i);	//����Ĭ����ʾ
			tempRow->nextRow->isShow = 1;
			tempRow->needChip = 0;					//�Ƿ���Ҫ�и�
			tempRow->left = 0;						//��ߵ��и�λ��
			tempRow->right = 0;					//�ұߵ��и�λ��

			tempRow = tempRow->nextRow;
			//ע������û�м���ڴ����ʧ��
		}
		tempRow->nextRow = NULL;	//���һ����β
	}
	m_tableLineSum = lineSum;		//�����е�����
	//����ؼ�������ʾ������
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_size.x;
	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_size.y;
	m_canShowLineSum = m_showPixHight / m_nowTextHeight;
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);	//��ǰ�������Ӧ��Χ
	m_spriteSelect.setSize((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_nowTextHeight / m_mutiListSelect->textureSize.y);
	//ΪԪ�ط����ڴ�ռ�
	if(m_tableLineSum == 0)
	{
		m_tableBox = NULL;
	}else
	{
		m_tableBox = createMem<_XMultiListOneBox>();
		if(m_tableBox == NULL)
		{
			releaseTempMemory();
			return 0;
		}
		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = 1;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setSize(m_size * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_nowTextHeight * 1);
		m_tableBox->string = NULL;
		setBoxStr("Line",0,0);

		_XMultiListOneBox *tempBox = m_tableBox;
		int i,j;
		_XMultiListOneRow *tempRow;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == 0 && i == 0) 
				{
					tempRow = tempRow->nextRow;
					//tempBox = tempBox->nextBox;
					continue;
				}
				tempBox->nextBox = createMem<_XMultiListOneBox>();
				if(tempBox->nextBox == NULL) 
				{
					releaseTempMemory();
					return XFalse;
				}
				tempBox = tempBox->nextBox;
				tempBox->isEnable = XTrue;
				
				tempBox->isShow = 1;
				tempBox->order.set(j,i);
				tempBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
				_XObjectManager::GetInstance().decreaseAObject(&(tempBox->text));
#endif
				tempBox->text.setSize(m_size * m_fontSize);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
				tempBox->string = NULL;
				setBoxStr("Line",i,j);
				
				tempRow = tempRow->nextRow;
				//ע������û�м���ڴ����ʧ��
			}
		}
		tempBox->nextBox = NULL;	//���һ����β
	}
	m_mouseLeftButtonDown = XFalse;
	m_mouseMoveDown = XFalse;

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MUTILIST);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	updateShowChange();
	updateSliderState();
	return XTrue;
}
_XBool _XMultiList::initEx(const _XVector2& position,		//�ռ����ڵ�λ��
	const _XMultiListTexture &tex,		//�ؼ�����ͼ
	const _XFontUnicode &font,			//�ؼ���ʹ�õ�����
	float strSize,						//��������Ŵ�С
	int rowSum,					//�ؼ��е�����
	int lineSum,				//�ؼ��е�����
	//const _XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
	const _XSlider &vSlider,	//��ֱ������
	const _XSlider &hSlider)	//ˮƽ������
{
	if(m_isInited) return XFalse;	//�ظ���ʼ��
	//�����ǲ������
	if(tex.m_mouseRect.getWidth() <= 0 || tex.m_mouseRect.getHeight() <= 0) return XFalse;	//���뷶Χ����Ϊ��
	if(tex.mutiListDisable == NULL || tex.mutiListMove == NULL || tex.mutiListNormal == NULL
		|| tex.mutiListSelect == NULL || tex.mutiListTitle == NULL || tex.mutiListTitleEnd == NULL)
	{//��Ҫ����ͼ������
		return XFalse;
	}
	if(rowSum < 0 || lineSum < 0 || (rowSum == 0 && lineSum > 0))
	{//����Ϊ����
		return XFalse;
	}
	if(rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;
	//���濪ʼ��ֵ
	m_position = position;
	m_mouseRect = tex.m_mouseRect;
	m_withoutTex = XFalse;

	m_mutiListNormal = tex.mutiListNormal;			//�����б�����ͨ״̬
	m_mutiListDisable = tex.mutiListDisable;			//�����б�����Ч״̬
	m_mutiListSelect = tex.mutiListSelect;			//�����б��б�ѡ���еı���
	m_mutiListMove = tex.mutiListMove;			//�ƶ�����ʱ�Ķ����ǩ
	m_mutiListTitle = tex.mutiListTitle;			//�����б��ı��ⱳ��
	m_mutiListTitleEnd = tex.mutiListTitleEnd;		//�����б��ı���ָ���

	m_size.set(1.0f,1.0f);
	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(strSize,strSize);
	m_caption.setSize(m_fontSize);
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;

	m_spriteBackGround.init(m_mutiListNormal->texture.m_w,m_mutiListNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setSize(m_size);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_mutiListSelect->texture.m_w,m_mutiListSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setSize(m_size);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteMove.init(m_mutiListMove->texture.m_w,m_mutiListMove->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteMove);
#endif
	m_spriteMove.setSize(m_size);
	m_spriteMove.setIsTransformCenter(POINT_LEFT_TOP);

	m_haveSelect = XFalse;		//�Ƿ���ѡ��
	m_selectLineOrder = -1;	//ѡ�������һ��
	m_showStartLine = 0;	//��ʾ���Ǵӵڼ��п�ʼ��
	m_showStartRow = 0;		//��ʾ�ӵڼ��п�ʼ��
	m_needShowMove = XFalse;		//�Ƿ���Ҫ��ʾ�ƶ�������

	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������
	m_verticalSlider.setACopy(vSlider);		//��ֱ������
	_XControlManager::GetInstance().decreaseAObject(&m_verticalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_verticalSlider.setSize(m_size);
	m_verticalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiListValueChangeV,funMutiListValueChangeMoveV,this);

	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������
	m_horizontalSlider.setACopy(hSlider);	//ˮƽ������
	_XControlManager::GetInstance().decreaseAObject(&m_horizontalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	m_horizontalSlider.setSize(m_size);
	m_horizontalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiListValueChangeH,funMutiListValueChangeMoveH,this);

	m_tableRowSum = rowSum;		//�����е�����
	//Ϊ�е����������ڴ�ռ�
	if(m_tableRowSum == 0)
	{
		m_tableRow = NULL;
	}else
	{
		m_tableRow = createMem<_XMultiListOneRow>();
		if(m_tableRow == NULL) return 0;

		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setSize(m_size * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_nowTextHeight);
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_size.x,
			m_position.y + m_mouseRect.top * m_size.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		m_tableRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableRow->m_spriteTitle));
#endif
		m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitle.setSize(m_tableRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
		m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
		m_tableRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableRow->m_spriteTitleEnd));
#endif
		m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_size.x - 
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
			m_tableRow->position.y);
		m_tableRow->m_spriteTitleEnd.setSize(m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
			m_tableRow->pixSize.y * m_size.y/m_mutiListTitle->textureSize.y);
		m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_size.x / m_nowTextWidth;
		m_tableRow->title = NULL;
		setTileStr("Title",0);	//����Ĭ����ʾ
		m_tableRow->isShow = 1;

		m_tableRow->needChip = 0;					//�Ƿ���Ҫ�и�
		m_tableRow->left = 0;						//��ߵ��и�λ��
		m_tableRow->right = 0;					//�ұߵ��и�λ��

		_XMultiListOneRow *tempRow = m_tableRow;
		for(int i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow = createMem<_XMultiListOneRow>();
			if(tempRow->nextRow == NULL) 
			{
				releaseTempMemory();
				return 0;
			}
			tempRow->nextRow->isEnable = XTrue;
			tempRow->nextRow->order = i;
			tempRow->nextRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().decreaseAObject(&(tempRow->nextRow->text));
#endif
			tempRow->nextRow->text.setSize(m_size * m_fontSize);
			tempRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_nowTextHeight);
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_size.x,
				m_position.y + m_mouseRect.top * m_size.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			tempRow->nextRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().decreaseAObject(&(tempRow->nextRow->m_spriteTitle));
#endif
			tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->m_spriteTitle.setSize(m_tableRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
			tempRow->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
			tempRow->nextRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().decreaseAObject(&(tempRow->nextRow->m_spriteTitleEnd));
#endif
			tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x * m_size.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			tempRow->nextRow->m_spriteTitleEnd.setSize(m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y);
			tempRow->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

			tempRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_size.x / m_nowTextWidth;
			tempRow->nextRow->title = NULL;
			setTileStr("Title",i);	//����Ĭ����ʾ
			tempRow->nextRow->isShow = 1;
			tempRow->needChip = 0;					//�Ƿ���Ҫ�и�
			tempRow->left = 0;						//��ߵ��и�λ��
			tempRow->right = 0;					//�ұߵ��и�λ��

			tempRow = tempRow->nextRow;
			//ע������û�м���ڴ����ʧ��
		}
		tempRow->nextRow = NULL;	//���һ����β
	}
	m_tableLineSum = lineSum;		//�����е�����
	//����ؼ�������ʾ������
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_size.x;
	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_size.y;
	m_canShowLineSum = m_showPixHight / m_nowTextHeight;
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);	//��ǰ�������Ӧ��Χ
	m_spriteSelect.setSize((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_nowTextHeight / m_mutiListSelect->textureSize.y);
	//ΪԪ�ط����ڴ�ռ�
	if(m_tableLineSum == 0)
	{
		m_tableBox = NULL;
	}else
	{
		m_tableBox = createMem<_XMultiListOneBox>();
		if(m_tableBox == NULL)
		{
			releaseTempMemory();
			return 0;
		}
		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = 1;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setSize(m_size * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_nowTextHeight * 1);
		m_tableBox->string = NULL;
		setBoxStr("Line",0,0);

		_XMultiListOneBox *tempBox = m_tableBox;
		int i,j;
		_XMultiListOneRow *tempRow;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == 0 && i == 0) 
				{
					tempRow = tempRow->nextRow;
					//tempBox = tempBox->nextBox;
					continue;
				}
				tempBox->nextBox = createMem<_XMultiListOneBox>();
				if(tempBox->nextBox == NULL) 
				{
					releaseTempMemory();
					return 0;
				}
				tempBox = tempBox->nextBox;
				tempBox->isEnable = XTrue;
				
				tempBox->isShow = 1;
				tempBox->order.set(j,i);
				tempBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
				_XObjectManager::GetInstance().decreaseAObject(&(tempBox->text));
#endif
				tempBox->text.setSize(m_size * m_fontSize);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
				tempBox->string = NULL;
				setBoxStr("Line",i,j);
				
				tempRow = tempRow->nextRow;
				//ע������û�м���ڴ����ʧ��
			}
		}
		tempBox->nextBox = NULL;	//���һ����β
	}
	m_mouseLeftButtonDown = XFalse;
	m_mouseMoveDown = XFalse;

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MUTILIST);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	updateShowChange();
	updateSliderState();
	return XTrue;
}
_XBool _XMultiList::initPlus(const char * path,		//�ؼ�����ͼ
		const _XFontUnicode &font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum,				//�ؼ��е�����
		//const _XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		_XResourcePosition resoursePosition)	//ˮƽ������
{
	if(m_isInited) return XFalse;	//�ظ���ʼ��
	//�����ǲ������
	if(path == NULL) return XFalse;
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XMULTILIST_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	_XMultiListTexture * tex = (_XMultiListTexture *)m_resInfo->m_pointer;
	if(tex->m_mouseRect.getWidth() <= 0 || tex->m_mouseRect.getHeight() <= 0) return XFalse;	//���뷶Χ����Ϊ��
	if(tex->mutiListDisable == NULL || tex->mutiListMove == NULL || tex->mutiListNormal == NULL
		|| tex->mutiListSelect == NULL || tex->mutiListTitle == NULL || tex->mutiListTitleEnd == NULL)
	{//��Ҫ����ͼ������
		return XFalse;
	}
	if(rowSum < 0 || lineSum < 0 || (rowSum == 0 && lineSum > 0)) return XFalse;
	if(rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;
	//���濪ʼ��ֵ
	m_position.set(0.0f,0.0f);
	m_mouseRect = tex->m_mouseRect;
	m_withoutTex = XFalse;

	m_mutiListNormal = tex->mutiListNormal;			//�����б�����ͨ״̬
	m_mutiListDisable = tex->mutiListDisable;			//�����б�����Ч״̬
	m_mutiListSelect = tex->mutiListSelect;			//�����б��б�ѡ���еı���
	m_mutiListMove = tex->mutiListMove;			//�ƶ�����ʱ�Ķ����ǩ
	m_mutiListTitle = tex->mutiListTitle;			//�����б��ı��ⱳ��
	m_mutiListTitleEnd = tex->mutiListTitleEnd;		//�����б��ı���ָ���

	m_size.set(1.0f,1.0f);
	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(strSize,strSize);
	m_caption.setSize(m_fontSize);
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;

	m_spriteBackGround.init(m_mutiListNormal->texture.m_w,m_mutiListNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setSize(m_size);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_mutiListSelect->texture.m_w,m_mutiListSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setSize(m_size);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteMove.init(m_mutiListMove->texture.m_w,m_mutiListMove->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteMove);
#endif
	m_spriteMove.setSize(m_size);
	m_spriteMove.setIsTransformCenter(POINT_LEFT_TOP);

	m_haveSelect = XFalse;		//�Ƿ���ѡ��
	m_selectLineOrder = -1;	//ѡ�������һ��
	m_showStartLine = 0;	//��ʾ���Ǵӵڼ��п�ʼ��
	m_showStartRow = 0;		//��ʾ�ӵڼ��п�ʼ��
	m_needShowMove = XFalse;		//�Ƿ���Ҫ��ʾ�ƶ�������

	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������
	//m_verticalSlider.setACopy(vSlider);		//��ֱ������
	char tempPath[MAX_FILE_NAME_LENGTH];
	sprintf(tempPath,"%s/SliderV",path);
	m_verticalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_VERTICAL,resoursePosition);
	_XControlManager::GetInstance().decreaseAObject(&m_verticalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_verticalSlider.setSize(m_size);
	m_verticalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiListValueChangeV,funMutiListValueChangeMoveV,this);

	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������
	//m_horizontalSlider.setACopy(hSlider);	//ˮƽ������
	sprintf(tempPath,"%s/SliderH",path);
	m_horizontalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_HORIZONTAL,resoursePosition);
	_XControlManager::GetInstance().decreaseAObject(&m_horizontalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	m_horizontalSlider.setSize(m_size);
	m_horizontalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiListValueChangeH,funMutiListValueChangeMoveH,this);

	m_tableRowSum = rowSum;		//�����е�����
	//Ϊ�е����������ڴ�ռ�
	if(m_tableRowSum == 0)
	{
		m_tableRow = NULL;
	}else
	{
		m_tableRow = createMem<_XMultiListOneRow>();
		if(m_tableRow == NULL) return 0;

		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setSize(m_size * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_nowTextHeight);
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_size.x,
			m_position.y + m_mouseRect.top * m_size.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		m_tableRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableRow->m_spriteTitle));
#endif
		m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitle.setSize(m_tableRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
		m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
		m_tableRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableRow->m_spriteTitleEnd));
#endif
		m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_size.x - 
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
			m_tableRow->position.y);
		m_tableRow->m_spriteTitleEnd.setSize(m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
			m_tableRow->pixSize.y * m_size.y/m_mutiListTitle->textureSize.y);
		m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_size.x / m_nowTextWidth;
		m_tableRow->title = NULL;
		setTileStr("Title",0);	//����Ĭ����ʾ
		m_tableRow->isShow = 1;

		m_tableRow->needChip = 0;					//�Ƿ���Ҫ�и�
		m_tableRow->left = 0;						//��ߵ��и�λ��
		m_tableRow->right = 0;					//�ұߵ��и�λ��

		_XMultiListOneRow *tempRow = m_tableRow;
		for(int i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow = createMem<_XMultiListOneRow>();
			if(tempRow->nextRow == NULL) 
			{
				releaseTempMemory();
				return 0;
			}
			tempRow->nextRow->isEnable = XTrue;
			tempRow->nextRow->order = i;
			tempRow->nextRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().decreaseAObject(&(tempRow->nextRow->text));
#endif
			tempRow->nextRow->text.setSize(m_size * m_fontSize);
			tempRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_nowTextHeight);
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_size.x,
				m_position.y + m_mouseRect.top * m_size.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			tempRow->nextRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().decreaseAObject(&(tempRow->nextRow->m_spriteTitle));
#endif
			tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->m_spriteTitle.setSize(m_tableRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
			tempRow->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
			tempRow->nextRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().decreaseAObject(&(tempRow->nextRow->m_spriteTitleEnd));
#endif
			tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x * m_size.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			tempRow->nextRow->m_spriteTitleEnd.setSize(m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y);
			tempRow->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

			tempRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_size.x / m_nowTextWidth;
			tempRow->nextRow->title = NULL;
			setTileStr("Title",i);	//����Ĭ����ʾ
			tempRow->nextRow->isShow = 1;
			tempRow->needChip = 0;					//�Ƿ���Ҫ�и�
			tempRow->left = 0;						//��ߵ��и�λ��
			tempRow->right = 0;					//�ұߵ��и�λ��

			tempRow = tempRow->nextRow;
			//ע������û�м���ڴ����ʧ��
		}
		tempRow->nextRow = NULL;	//���һ����β
	}
	m_tableLineSum = lineSum;		//�����е�����
	//����ؼ�������ʾ������
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_size.x;
	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_size.y;
	m_canShowLineSum = m_showPixHight / m_nowTextHeight;
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);	//��ǰ�������Ӧ��Χ
	m_spriteSelect.setSize((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_nowTextHeight / m_mutiListSelect->textureSize.y);
	//ΪԪ�ط����ڴ�ռ�
	if(m_tableLineSum == 0)
	{
		m_tableBox = NULL;
	}else
	{
		m_tableBox = createMem<_XMultiListOneBox>();
		if(m_tableBox == NULL)
		{
			releaseTempMemory();
			return 0;
		}
		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = 1;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setSize(m_size * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_nowTextHeight * 1);
		m_tableBox->string = NULL;
		setBoxStr("Line",0,0);

		_XMultiListOneBox *tempBox = m_tableBox;
		int i,j;
		_XMultiListOneRow *tempRow;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == 0 && i == 0) 
				{
					tempRow = tempRow->nextRow;
					//tempBox = tempBox->nextBox;
					continue;
				}
				tempBox->nextBox = createMem<_XMultiListOneBox>();
				if(tempBox->nextBox == NULL) 
				{
					releaseTempMemory();
					return 0;
				}
				tempBox = tempBox->nextBox;
				tempBox->isEnable = XTrue;
				
				tempBox->isShow = 1;
				tempBox->order.set(j,i);
				tempBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
				_XObjectManager::GetInstance().decreaseAObject(&(tempBox->text));
#endif
				tempBox->text.setSize(m_size * m_fontSize);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
				tempBox->string = NULL;
				setBoxStr("Line",i,j);
				
				tempRow = tempRow->nextRow;
				//ע������û�м���ڴ����ʧ��
			}
		}
		tempBox->nextBox = NULL;	//���һ����β
	}
	m_mouseLeftButtonDown = XFalse;
	m_mouseMoveDown = XFalse;

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MUTILIST);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	updateShowChange();
	updateSliderState();
	return XTrue;
}
_XBool _XMultiList::initWithoutTex(const _XRect& area,
		const _XFontUnicode &font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum)
{
	if(m_isInited) return XFalse;	//�ظ���ʼ��
	//�����ǲ������
	if(rowSum < 0 || lineSum < 0 || (rowSum == 0 && lineSum > 0)) return XFalse;
	if(rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;
	//���濪ʼ��ֵ
	m_position.set(0.0f,0.0f);
	m_mouseRect = area;
	m_withoutTex = XTrue;

	m_size.set(1.0f,1.0f);
	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);
	m_fontSize.set(strSize,strSize);
	m_caption.setSize(m_fontSize);
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;

	m_haveSelect = XFalse;		//�Ƿ���ѡ��
	m_selectLineOrder = -1;	//ѡ�������һ��
	m_showStartLine = 0;	//��ʾ���Ǵӵڼ��п�ʼ��
	m_showStartRow = 0;		//��ʾ�ӵڼ��п�ʼ��
	m_needShowMove = XFalse;		//�Ƿ���Ҫ��ʾ�ƶ�������

	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������
//	m_verticalSlider.initWithoutTex(_XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight() - DEFAULT_SLIDER_WIDTH),_XRect(0.0f,0.0f,DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_VERTICAL,100.0f,0.0f,_XVector2(0.0f,0.0f));
	m_verticalSlider.initWithoutTex(_XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight() - DEFAULT_SLIDER_WIDTH),100.0f,0.0f,SLIDER_TYPE_VERTICAL);
	_XControlManager::GetInstance().decreaseAObject(&m_verticalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_verticalSlider.setSize(m_size);
	m_verticalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiListValueChangeV,funMutiListValueChangeMoveV,this);

	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������
//	m_horizontalSlider.initWithoutTex(_XRect(0,0,m_mouseRect.getWidth() - DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),_XRect(0.0f,0.0f,DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_HORIZONTAL,100.0f,0.0f,_XVector2(0.0f,0.0f));
	m_horizontalSlider.initWithoutTex(_XRect(0,0,m_mouseRect.getWidth() - DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH));
	_XControlManager::GetInstance().decreaseAObject(&m_horizontalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	m_horizontalSlider.setSize(m_size);
	m_horizontalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiListValueChangeH,funMutiListValueChangeMoveH,this);

	m_tableRowSum = rowSum;		//�����е�����
	//Ϊ�е����������ڴ�ռ�
	if(m_tableRowSum == 0)
	{
		m_tableRow = NULL;
	}else
	{
		m_tableRow = createMem<_XMultiListOneRow>();
		if(m_tableRow == NULL) return 0;

		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setSize(m_size * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_nowTextHeight);
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_size.x,
			m_position.y + m_mouseRect.top * m_size.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_size.x / m_nowTextWidth;
		m_tableRow->title = NULL;
		setTileStr("Title",0);	//����Ĭ����ʾ
		m_tableRow->isShow = 1;

		m_tableRow->needChip = 0;					//�Ƿ���Ҫ�и�
		m_tableRow->left = 0;						//��ߵ��и�λ��
		m_tableRow->right = 0;					//�ұߵ��и�λ��

		_XMultiListOneRow *tempRow = m_tableRow;
		for(int i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow = createMem<_XMultiListOneRow>();
			if(tempRow->nextRow == NULL) 
			{
				releaseTempMemory();
				return 0;
			}
			tempRow->nextRow->isEnable = XTrue;
			tempRow->nextRow->order = i;
			tempRow->nextRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().decreaseAObject(&(tempRow->nextRow->text));
#endif
			tempRow->nextRow->text.setSize(m_size * m_fontSize);
			tempRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_nowTextHeight);
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_size.x,
				m_position.y + m_mouseRect.top * m_size.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);

			tempRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_size.x / m_nowTextWidth;
			tempRow->nextRow->title = NULL;
			setTileStr("Title",i);	//����Ĭ����ʾ
			tempRow->nextRow->isShow = 1;
			tempRow->needChip = 0;					//�Ƿ���Ҫ�и�
			tempRow->left = 0;						//��ߵ��и�λ��
			tempRow->right = 0;					//�ұߵ��и�λ��

			tempRow = tempRow->nextRow;
			//ע������û�м���ڴ����ʧ��
		}
		tempRow->nextRow = NULL;	//���һ����β
	}
	m_tableLineSum = lineSum;		//�����е�����
	//����ؼ�������ʾ������
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_size.x;
//	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_size.y;
	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - DEFAULT_TITLE_HEIGHT) * m_size.y;
	m_canShowLineSum = m_showPixHight / m_nowTextHeight;
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);	//��ǰ�������Ӧ��Χ
//	m_spriteSelect.setSize((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_nowTextHeight / m_mutiListSelect->textureSize.y);
	//ΪԪ�ط����ڴ�ռ�
	if(m_tableLineSum == 0)
	{
		m_tableBox = NULL;
	}else
	{
		m_tableBox = createMem<_XMultiListOneBox>();
		if(m_tableBox == NULL)
		{
			releaseTempMemory();
			return 0;
		}
		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = 1;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setSize(m_size * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_nowTextHeight * 1);
		m_tableBox->string = NULL;
		setBoxStr("Line",0,0);

		_XMultiListOneBox *tempBox = m_tableBox;
		int i,j;
		_XMultiListOneRow *tempRow;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == 0 && i == 0) 
				{
					tempRow = tempRow->nextRow;
					//tempBox = tempBox->nextBox;
					continue;
				}
				tempBox->nextBox = createMem<_XMultiListOneBox>();
				if(tempBox->nextBox == NULL) 
				{
					releaseTempMemory();
					return 0;
				}
				tempBox = tempBox->nextBox;
				tempBox->isEnable = XTrue;
				
				tempBox->isShow = 1;
				tempBox->order.set(j,i);
				tempBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
				_XObjectManager::GetInstance().decreaseAObject(&(tempBox->text));
#endif
				tempBox->text.setSize(m_size * m_fontSize);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
				tempBox->string = NULL;
				setBoxStr("Line",i,j);
				
				tempRow = tempRow->nextRow;
				//ע������û�м���ڴ����ʧ��
			}
		}
		tempBox->nextBox = NULL;	//���һ����β
	}
	m_mouseLeftButtonDown = XFalse;
	m_mouseMoveDown = XFalse;

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MUTILIST);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	updateShowChange();
	updateSliderState();
	return XTrue;
}
void _XMultiList::updateSliderState()				//���ݱ������������»�������״̬
{
	if(!m_isInited) return;		//���û�г�ʼ��ֱ���˳�
	//��ֱ������
	if(m_canShowLineSum < m_tableLineSum)
	{//�����ֱ������ʾ��Χ������Ҫ������ֱ��ʾ����������ѡ��Χ
		m_needShowVSlider = XTrue;			
		m_verticalSlider.setRange(m_tableLineSum - m_canShowLineSum,0.0f);		//��ֱ������
	}else
	{
		m_showStartLine = 0;
		m_needShowVSlider = XFalse;			
		m_verticalSlider.setRange(m_tableLineSum - m_canShowLineSum,0.0f);		//��ֱ������
	}
	//ˮƽ������
	if(m_tableRowSum <= 0)
	{//���û��������Ҫ��ʾˮƽ������
		m_needShowHSlider = XFalse;
	}else
	{
		int width = 0;
		float rowWidth[MUTILIST_MAX_ROW_SUM];
		memset(rowWidth,0,MUTILIST_MAX_ROW_SUM * sizeof(float));
		//����1:ֻҪ����1������ʾˮƽ������
	/*	if(m_tableRowSum > 1)
		{
			m_needShowHSlider = 1;
			m_horizontalSlider.setRange(m_tableRowSum - 1,0);
		}*/
	/*	//����2:��Ҫ������ʾ��Χ����ʾˮƽ������
		_XMultiListOneRow *tempRow = m_tableRow;
		int i = 0;
		while(1)
		{
			rowWidth[i] = tempRow->pixSize.x * m_size.x;
			if(tempRow->nextRow == NULL)
			{
				break;
			}else
			{
				tempRow = tempRow->nextRow;
			}
			i++;
		}
		int flag = 0;
		for(i = 0;i < m_tableRowSum;++ i)
		{
			width += rowWidth[m_tableRowSum - i -1];
			if(width > m_showPixWidth && i < m_tableRowSum)
			{
				flag = 1;
				break;
			}
		}
		if(flag == 1)
		{//����ˮƽ��ʾ��������Ҫ��ʾˮƽ������
			m_needShowHSlider = 1;
			m_horizontalSlider.setRange(m_tableRowSum - i,0);
		//	m_horizontalSlider.setRange(m_tableRowSum - 1,0);
		}else
		{
			if(m_needShowHSlider != 0)
			{//��Խ�絽��Խ�磬��Ҫ���³�ʼ��ƫ����
				m_rowDx = 0;
			}
			m_needShowHSlider = 0;
			m_showStartRow = 0;
			m_horizontalSlider.setNowValue(0);
		}*/
		//����2:��Ҫ������ʾ��Χ����ʾˮƽ������
		_XMultiListOneRow *tempRow = m_tableRow;
		int i = 0;
		while(1)
		{
			rowWidth[i] = tempRow->pixSize.x * m_size.x;
			if(tempRow->nextRow == NULL) break;
			else tempRow = tempRow->nextRow;
			i++;
		}
		int flag = 0;
		for(i = 0;i < m_tableRowSum;++ i)
		{
			width += rowWidth[m_tableRowSum - i -1];
			if(width > m_showPixWidth && i < m_tableRowSum && flag == 0)
			{
				flag = i;
				//break;
			}
		}
		if(flag != 0)
		{//����ˮƽ��ʾ��������Ҫ��ʾˮƽ������
			m_needShowHSlider = XTrue;
			//m_horizontalSlider.setRange(m_tableRowSum - i,0);
			m_horizontalSlider.setRange(width - m_showPixWidth + MUTILIST_TITLE_EXPAND_LENGTH * m_size.x,0);	//ע�������56��������չ��
		}else
		{
		//	if(m_needShowHSlider != 0)
		//	{//��Խ�絽��Խ�磬��Ҫ���³�ʼ��ƫ����
		//		m_rowDx = 0;
		//	}
			m_needShowHSlider = XFalse;
			m_showStartRow = 0;
			m_horizontalSlider.setNowValue(0);
		}
	}
}
void _XMultiList::setTileStr(const char *str,int order)
{
	if(str == NULL) return;
	if(order < 0 || order >= m_tableRowSum) return;
	//Ѱ�ҵ�ָ������
	_XMultiListOneRow *tempRow;
	if(order == 0)
	{
		tempRow = m_tableRow;
	}else
	{
		tempRow = m_tableRow;
		for(int i = 1;i <= order;++ i)
		{
			tempRow = tempRow->nextRow;
		}
	}
	//�޸�ָ���е��ַ���
	XDELETE_ARRAY(tempRow->title);
	tempRow->title = createArrayMem<char>(strlen(str) + 1);
	if(tempRow->title == NULL) return;

	strcpy(tempRow->title,str);
	//�޸�ָ���е���ʾ�ַ���
	if((int)(strlen(tempRow->title)) >= tempRow->stringShowWidth)
	{//���������ʾ����
		if(tempRow->stringShowWidth >= 2)
		{//����3���ַ�������ʾ...������ʾ..
			tempRow->title[tempRow->stringShowWidth] = '\0';
			tempRow->title[tempRow->stringShowWidth - 1] = '.';
			tempRow->title[tempRow->stringShowWidth - 2] = '.';
		}else
		{
			tempRow->title[tempRow->stringShowWidth] = '\0';
		}
		tempRow->text.setString(tempRow->title);
		strcpy(tempRow->title,str);
	}else
	{
		tempRow->text.setString(tempRow->title);
	}
}
void _XMultiList::setBoxStr(const char *str,int line,int row)	//����ĳһ����Ԫ�������
{
	//��������������ж�
	if(str == NULL) return;
	if(line < 0 || line >= m_tableLineSum) return;
	if(row < 0 || row >= m_tableRowSum) return;
	//Ѱ�ҵ�ָ���ı���
	_XMultiListOneBox *tempBox = m_tableBox;
	for(int i = 0;i < line * m_tableRowSum + row;++ i)
	{
		tempBox = tempBox->nextBox;
	}
	_XMultiListOneRow *tempRow = m_tableRow;
	for(int i = 0;i < row;++ i)
	{
		tempRow = tempRow->nextRow;
	}
	//�����ڴ�ռ�
	XDELETE_ARRAY(tempBox->string);
	tempBox->string = createArrayMem<char>(strlen(str) + 1);
	if(tempBox->string == NULL) return;

	//��ֵ
	strcpy(tempBox->string,str);
	//������ʾ�ַ�
	if((int)(strlen(tempBox->string)) >= tempRow->stringShowWidth)
	{//���������ʾ����
		if(tempRow->stringShowWidth >= 2)
		{//����3���ַ�������ʾ...������ʾ..
			tempBox->string[tempRow->stringShowWidth] = '\0';
			tempBox->string[tempRow->stringShowWidth - 1] = '.';
			tempBox->string[tempRow->stringShowWidth - 2] = '.';
		}else
		{
			tempBox->string[tempRow->stringShowWidth] = '\0';
		}
		tempBox->text.setString(tempBox->string);
		strcpy(tempBox->string,str);
	}else
	{
		tempBox->text.setString(tempBox->string);
	}
}
char *_XMultiList::getBoxStr(int line,int row)
{
	//��������������ж�
	if(line < 0 || line >= m_tableLineSum) return NULL;
	if(row < 0 || row >= m_tableRowSum) return NULL;
	//Ѱ�ҵ�ָ���ı���
	_XMultiListOneBox *tempBox = m_tableBox;
	for(int i = 0;i < line * m_tableRowSum + row;++ i)
	{
		tempBox = tempBox->nextBox;
	}
	_XMultiListOneRow *tempRow = m_tableRow;
	for(int i = 0;i < row;++ i)
	{
		tempRow = tempRow->nextRow;
	}
	return tempBox->string;
}
void _XMultiList::updateShowChange()				//�����б���ı仯��Ϣ�����б����еı������ʾ���
{
	if(!m_isInited) return;		//���û�г�ʼ��ֱ���˳�
	if(m_tableRowSum <= 0) return;	//���û�б���ֱ�ӷ���
	//�������ʾ
	_XMultiListOneRow *tempRow = m_tableRow;
	int nowWidthPix = 0;
	int row = 0;
	int showMaxRow = 0;
//	int showDx = m_showStartRow; //��ʾ�����ݱ����ˮƽƫ��	m_showStartRow(ˮƽƫ����)
	while(1)
	{
		if(nowWidthPix + tempRow->pixSize.x * m_size.x > m_showStartRow && nowWidthPix - m_showStartRow <= m_showPixWidth)
		{
			if(nowWidthPix < m_showStartRow && nowWidthPix + tempRow->pixSize.x * m_size.x - m_showStartRow <= m_showPixWidth)
			{//��ʾǰ��һ����
				tempRow->isShow = 1;	//ͷ
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				tempRow->needChip = 1;					//�Ƿ���Ҫ�и�
				tempRow->left = (m_showStartRow - nowWidthPix) / m_size.x;						//��ߵ��и�λ��
				tempRow->right = tempRow->pixSize.x;					//�ұߵ��и�λ��
				tempRow->text.setClipRect(tempRow->left,0.0f,tempRow->right,tempRow->text.getTextSize().y);
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - nowWidthPix;
					tempRow->pixLen = tempRow->pixSize.x * m_size.x - (m_showStartRow - nowWidthPix);
				}else
				{
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - nowWidthPix,tempRow->position.y);
					tempRow->m_spriteTitle.setSize((float)(tempRow->pixSize.x * m_size.x - (m_showStartRow - nowWidthPix)) / m_mutiListTitle->textureSize.x,m_size.y);

					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_size.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				nowWidthPix += tempRow->pixSize.x * m_size.x;
			}else
			if(nowWidthPix + tempRow->pixSize.x * m_size.x - m_showStartRow <= m_showPixWidth && nowWidthPix >= m_showStartRow)
			{//����������ʾ
				tempRow->isShow = 1;//����
				nowWidthPix += tempRow->pixSize.x * m_size.x;
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				tempRow->text.disClip();
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - m_showStartRow;
					tempRow->pixLen = tempRow->pixSize.x * m_size.x;
				}else
				{
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
					tempRow->m_spriteTitle.setSize(tempRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_size.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				tempRow->needChip = 0;					//�Ƿ���Ҫ�и�
				tempRow->left = 0.0f;						//��ߵ��и�λ��
				tempRow->right = 0.0f;					//�ұߵ��и�λ��
			}else
			if(nowWidthPix >= m_showStartRow && nowWidthPix + tempRow->pixSize.x * m_size.x - m_showStartRow > m_showPixWidth)
			{//��ʾ����һ����
				tempRow->isShow = 1;	//β
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				tempRow->needChip = 1;					//�Ƿ���Ҫ�и�
				tempRow->left = 0.0f;						//��ߵ��и�λ��
				tempRow->right = (m_showPixWidth - (nowWidthPix - m_showStartRow)) / m_size.x;					//�ұߵ��и�λ��
				tempRow->text.setClipRect(0.0f,0.0f,tempRow->right,tempRow->text.getTextSize().y);
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - m_showStartRow;
					tempRow->pixLen = m_showPixWidth - (nowWidthPix - m_showStartRow);
					if(tempRow->pixSize.x * m_size.x - DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_size.y / DEFAULT_TITLE_HEIGHT > 
						m_showPixWidth - (nowWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
				}else
				{
					if(tempRow->pixSize.x * m_size.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y > 
						m_showPixWidth - (nowWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
					tempRow->m_spriteTitle.setSize((float)(m_showPixWidth - (nowWidthPix - m_showStartRow)) / m_mutiListTitle->textureSize.x,m_size.y);

					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_size.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				nowWidthPix += tempRow->pixSize.x * m_size.x;
			}else
			if(nowWidthPix < m_showStartRow && nowWidthPix + tempRow->pixSize.x * m_size.x - m_showStartRow > m_showPixWidth)
			{//������Ҫ��ͷȥβ��Ŀǰû�д���
				tempRow->isShow = 2; //�м�
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				tempRow->needChip = 1;					//�Ƿ���Ҫ�и�
				tempRow->left = (m_showStartRow - nowWidthPix) / m_size.x;						//��ߵ��и�λ��
				tempRow->right = (m_showPixWidth - (nowWidthPix - m_showStartRow)) / m_size.x;					//�ұߵ��и�λ��
				tempRow->text.setClipRect(tempRow->left,0.0f,tempRow->right,tempRow->text.getTextSize().y);
				if(m_withoutTex)
				{
					tempRow->posX = tempRow->position.x - nowWidthPix;
					tempRow->pixLen = m_showPixWidth;
					if(tempRow->pixSize.x * m_size.x - DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_size.y / DEFAULT_TITLE_HEIGHT > 
						m_showPixWidth - (nowWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
				}else
				{
					if(tempRow->pixSize.x * m_size.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y > 
						m_showPixWidth - (nowWidthPix - m_showStartRow))
					{
						tempRow->isShow = 2;
					}
					tempRow->m_spriteTitle.setPosition(tempRow->position.x - nowWidthPix,tempRow->position.y);
					tempRow->m_spriteTitle.setSize((float)(m_showPixWidth) / m_mutiListTitle->textureSize.x,m_size.y);

					tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_size.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				nowWidthPix += tempRow->pixSize.x * m_size.x;
			}else
			{
				printf("haha\n");
			}
		}else
		{
			nowWidthPix += tempRow->pixSize.x * m_size.x;
			tempRow->isShow = 0;
		}
	//	if(row < m_showStartRow)
	//	{//����ˮƽƫ��
	//		showDx += tempRow->pixSize.x * m_size.x;
	//	}
		if(tempRow->nextRow == NULL)
		{
			break;
		}else
		{
			row ++;
			tempRow = tempRow->nextRow;
		}
	}
	//�����б�������ȫ������
	if(m_tableLineSum <= 0) return;	//���û����ֱ�ӷ���
	_XMultiListOneBox *tempBox = m_tableBox;
	tempRow = m_tableRow;
	if(tempRow == NULL) return;
	row = 0;
	int line = 0;
	while(1)
	{
		if(line >= m_showStartLine && line < m_showStartLine + m_canShowLineSum)
		{
			//if(row >= m_showStartRow && row <= showMaxRow)
			if(tempRow->isShow != 0)
			{
				tempBox->isShow = 1;
				tempBox->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y + m_nowTextHeight * (line - m_showStartLine + 1));
				if(tempRow->needChip != 0) tempBox->text.setClipRect(tempRow->left,0.0f,tempRow->right,tempBox->text.getTextSize().y);
				else tempBox->text.disClip();
			}else
			{
				tempBox->isShow = 0;
			}
		}else
		{
			tempBox->isShow = 0;
		}
		if(tempBox->nextBox == NULL) break;
		else tempBox = tempBox->nextBox;
		++row;
		if(row >= m_tableRowSum)
		{
			row = 0;
			++ line;
			tempRow = m_tableRow;
		}else
		{
			tempRow = tempRow->nextRow;
		}
	}
}
//ע�����ﴫ��ĳߴ�Ϊ����֮ǰ�ĳߴ磬Ҳ�����ǿռ����ű���Ϊ1��ʱ��óߴ�
_XBool _XMultiList::setRowWidth(int temp,int order)	//��������һ�еĿ���
{
	if(temp < MUTILIST_MIN_WIDTH || temp > m_showPixWidth / m_size.x) return XFalse;
	if(order < 0 || order >= m_tableRowSum) return XFalse;
	//�ҵ���Ӧ��һ��
	_XMultiListOneRow *tempRow = m_tableRow;
	for(int i = 0;i < order;++ i)
	{
		tempRow = tempRow->nextRow;
	}
	
	tempRow->pixSize.set(temp,tempRow->pixSize.y);	//�ı䵱ǰ�е��п�
	tempRow->stringShowWidth = tempRow->pixSize.x * m_size.x / m_nowTextWidth;	//�ı䵱ǰ�е���ʾ�ַ�����
	if(!m_withoutTex)
	{
		tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x + tempRow->pixSize.x * m_size.x - 
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
			m_tableRow->position.y);
		tempRow->m_spriteTitle.setSize(tempRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
	}
	tempRow->text.setPosition(tempRow->position.x,tempRow->position.y);
	//�ı䵱ǰ�е���ʾ�ַ�
	char * str = createArrayMem<char>(strlen(tempRow->title) + 1);
	if(str == NULL) return XFalse;

	strcpy(str,tempRow->title);
	//�޸�ָ���е���ʾ�ַ���
	if((int)(strlen(tempRow->title)) >= tempRow->stringShowWidth)
	{//���������ʾ����
		if(tempRow->stringShowWidth >= 2)
		{//����3���ַ�������ʾ...������ʾ..
			tempRow->title[tempRow->stringShowWidth] = '\0';
			tempRow->title[tempRow->stringShowWidth - 1] = '.';
			tempRow->title[tempRow->stringShowWidth - 2] = '.';
		}else
		{
			tempRow->title[tempRow->stringShowWidth] = '\0';
		}
		tempRow->text.setString(tempRow->title);
		strcpy(tempRow->title,str);
	}else
	{
		tempRow->text.setString(tempRow->title);
	}
	XDELETE_ARRAY(str);
	//�޸ĺ���������е�λ��
	while(1)
	{
		if(tempRow->nextRow == NULL)
		{
			break;
		}else
		{
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_size.x,tempRow->nextRow->position.y);
			if(!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + tempRow->nextRow->pixSize.x * m_size.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			tempRow = tempRow->nextRow;
		}
	}
	//�ı����е�Ԫ������
	int i,j;
	_XMultiListOneBox *tempBox = m_tableBox;
	for(i = 0;i < m_tableLineSum;++ i)
	{
		tempRow = m_tableRow;
		for(j = 0;j < m_tableRowSum;++ j)
		{
			if(j == order)
			{//�ı��ַ���
				str = createArrayMem<char>(strlen(tempBox->string) + 1);
				if(str == NULL) return XFalse;

				strcpy(str,tempBox->string);
				if((int)(strlen(tempBox->string)) >= tempRow->stringShowWidth)
				{//���������ʾ����
					if(tempRow->stringShowWidth >= 2)
					{//����3���ַ�������ʾ...������ʾ..
						tempBox->string[tempRow->stringShowWidth] = '\0';
						tempBox->string[tempRow->stringShowWidth - 1] = '.';
						tempBox->string[tempRow->stringShowWidth - 2] = '.';
					}else
					{
						tempBox->string[tempRow->stringShowWidth] = '\0';
					}
					tempBox->text.setString(tempBox->string);
					strcpy(tempBox->string,str);
				}else
				{
					tempBox->text.setString(tempBox->string);
				}
				XDELETE_ARRAY(str);
			}
			if(j > order)
			{//�ı�λ��
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
			}

			tempRow = tempRow->nextRow;
			tempBox = tempBox->nextBox;
		}
	}
	//������ʾ����
	updateShowChange();
	updateSliderState();
	return XTrue;
}
void _XMultiList::draw()	//��溯��
{
	if(!m_isInited) return;	//���û�г�ʼ��ֱ���˳�
	if(!m_isVisiable) return;	//������ɼ�ֱ���˳�

	if(m_withoutTex)
	{
		if(!m_isEnable) 
		{
			drawFillBoxEx(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_size.x,
				(m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_size.y),0.6f,0.6f,0.6f);
			drawFillBoxEx(m_position + _XVector2((m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
				m_mouseRect.top * m_size.y),
				_XVector2(m_verticalSlider.getMouseRectWidth() * m_size.x,
				(m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_size.y),0.5f,0.5f,0.5f);
			drawFillBoxEx(m_position + _XVector2(m_mouseRect.left * m_size.x,
				(m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y),
				_XVector2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_size.x,
				m_horizontalSlider.getMouseRectHeight() * m_size.y),0.5f,0.5f,0.5f);
			drawFillBoxEx(m_position + _XVector2((m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
				(m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y),
				_XVector2(m_verticalSlider.getMouseRectWidth() * m_size.x,
				m_horizontalSlider.getMouseRectHeight() * m_size.y),0.25f,0.25f,0.25f);
		}else
		{
			drawFillBoxEx(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y),
				_XVector2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_size.x,
				(m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_size.y),0.75f,0.75f,0.75f);
			drawFillBoxEx(m_position + _XVector2((m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
				m_mouseRect.top * m_size.y),
				_XVector2(m_verticalSlider.getMouseRectWidth() * m_size.x,
				(m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_size.y),0.65f,0.65f,0.65f);
			drawFillBoxEx(m_position + _XVector2(m_mouseRect.left * m_size.x,
				(m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y),
				_XVector2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_size.x,
				m_horizontalSlider.getMouseRectHeight() * m_size.y),0.65f,0.65f,0.65f);
			drawFillBoxEx(m_position + _XVector2((m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
				(m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y),
				_XVector2(m_verticalSlider.getMouseRectWidth() * m_size.x,
				m_horizontalSlider.getMouseRectHeight() * m_size.y),0.45f,0.45f,0.45f);
		}
		if(m_haveSelect && m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
		{
			drawFillBoxEx(_XVector2(m_position.x + m_mouseRect.left * m_size.x,
				m_position.y + (m_mouseRect.top + DEFAULT_TITLE_HEIGHT) * m_size.y + 
				(m_selectLineOrder - m_showStartLine) * m_nowTextHeight),
				_XVector2(m_showPixWidth,m_nowTextHeight),0.85f,0.85f,0.85f);
			//m_spriteSelect.draw(m_mutiListSelect);
		}
		//������ʾ�б�������
		if(m_tableRowSum > 0)
		{
			_XMultiListOneRow *tempRow = m_tableRow;
			while(1)
			{
				if(tempRow->isEnable && tempRow->isShow != 0)
				{
					drawFillBoxEx(_XVector2(tempRow->posX,tempRow->position.y),_XVector2(tempRow->pixLen,DEFAULT_TITLE_HEIGHT * m_size.y),0.8f,0.8f,0.8f);
				//	tempRow->m_spriteTitle.draw(m_mutiListTitle);	//��ʾ���ⱳ��
					tempRow->text.draw();//��ʾ��������
					if(tempRow->isShow == 1)
					{//���ܻ�Խ��
						int x = tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_size.x - DEFAULT_END_WIDTH * tempRow->pixSize.y * m_size.y / DEFAULT_TITLE_HEIGHT;
						drawLine(x,m_tableRow->position.y,x,m_tableRow->position.y + DEFAULT_TITLE_HEIGHT * m_size.y,1,0.4f,0.4f,0.4f);
						//tempRow->m_spriteTitleEnd.draw(m_mutiListTitleEnd);	//��ʾ���������
					}
				}
				if(tempRow->nextRow == NULL) break;
				else tempRow = tempRow->nextRow;
			}
			if(m_tableLineSum > 0)
			{
				_XMultiListOneBox *tempBox = m_tableBox;
				while(1)
				{
					if(tempBox->isEnable && tempBox->isShow != 0) tempBox->text.draw();//��ʾ��������
					if(tempBox->nextBox == NULL) break;
					else tempBox = tempBox->nextBox;
				}
			}
		}
		if(m_needShowMove) 
		{
			int x = m_changeRow->position.x - m_showStartRow + m_changeRow->pixSize.x * m_size.x - 
								DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_size.y / DEFAULT_TITLE_HEIGHT;
			drawLine(x,m_changeRow->position.y,x,m_changeRow->position.y + (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight()) * m_size.y,1,0.25f,0.25f,0.25f);
			//m_spriteMove.draw(m_mutiListMove);
		}
	}else
	{
		if(!m_isEnable) m_spriteBackGround.draw(m_mutiListDisable);
		else m_spriteBackGround.draw(m_mutiListNormal);
		if(m_haveSelect && m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
		{
			m_spriteSelect.draw(m_mutiListSelect);
		}
		//������ʾ�б�������
		if(m_tableRowSum > 0)
		{
			_XMultiListOneRow *tempRow = m_tableRow;
			while(1)
			{
				if(tempRow->isEnable && tempRow->isShow != 0)
				{
					tempRow->m_spriteTitle.draw(m_mutiListTitle);	//��ʾ���ⱳ��
					tempRow->text.draw();//��ʾ��������
					if(tempRow->isShow == 1)tempRow->m_spriteTitleEnd.draw(m_mutiListTitleEnd);	//��ʾ���������
				}
				if(tempRow->nextRow == NULL) break;
				else tempRow = tempRow->nextRow;
			}
			if(m_tableLineSum > 0)
			{
				_XMultiListOneBox *tempBox = m_tableBox;
				while(1)
				{
					if(tempBox->isEnable && tempBox->isShow != 0) tempBox->text.draw();//��ʾ��������
					if(tempBox->nextBox == NULL) break;
					else tempBox = tempBox->nextBox;
				}
			}
		}
		if(m_needShowMove) m_spriteMove.draw(m_mutiListMove);
	}
	if(m_needShowVSlider) m_verticalSlider.draw();
	if(m_needShowHSlider) m_horizontalSlider.draw();
}
_XBool _XMultiList::canGetFocus(float x,float y)
{
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
_XBool _XMultiList::mouseProc(float x,float y,_XMouseState mouseState)					//������궯������Ӧ����
{
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�

	if(m_needShowVSlider != 0) m_verticalSlider.mouseProc(x,y,mouseState);
	if(m_needShowHSlider != 0) m_horizontalSlider.mouseProc(x,y,mouseState);
	//�����ж�����޸ı����������ж�
	if(m_tableRowSum > 0)
	{
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN)
		{//�������
			_XRect tempRect;
			_XMultiListOneRow *tempRow = m_tableRow;
			int order = 0;
		//	int showDx = m_showStartRow;	//�����Ҫ����������ڿ�ʼ��ʾ����һ�е�ƫ��������Ϊ�е�λ�����ȶ��ڵ�һ�еľ���λ�ã�������Ҫ����ʵ�ʵ���ʾ������Ե���ʾλ��
			while(1)
			{
				if(tempRow->isShow != 0)
				{//��ʾ�����Ĳ����޸�
					if(m_withoutTex)
					{
						tempRect.set(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_size.x - 
							DEFAULT_END_WIDTH * m_tableRow->pixSize.y * m_size.y / DEFAULT_TITLE_HEIGHT,
							tempRow->position.y,
							tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_size.x,
							tempRow->position.y + tempRow->pixSize.y * m_size.y);
					}else
					{
						tempRect.set(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_size.x - 
							m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
							tempRow->position.y,
							tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_size.x,
							tempRow->position.y + tempRow->pixSize.y * m_size.y);
					}
					if(tempRect.isInRect(x,y))
					{//����϶���Ч
						m_mouseLeftButtonDown = XTrue;
						m_startX = x;
					//	m_rowDx = m_showStartRow;
						m_changeRowOrder = order;
						m_changeRow = tempRow;
						//���ÿ�ʼ��ʾ�ƶ�ͼ��
						m_needShowMove = XTrue;
						if(!m_withoutTex)
						{
							m_spriteMove.setPosition(m_changeRow->position.x - m_showStartRow + m_changeRow->pixSize.x * m_size.x - 
								m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
								m_changeRow->position.y);
						}
						break;
					}
				}
			//	if(order < m_showStartRow)
			//	{
			///		showDx += tempRow->pixSize.x * m_size.x;
			//	}
				if(tempRow->nextRow == NULL) break;
				else tempRow = tempRow->nextRow;
				order ++;
			}
		}
		if(mouseState == MOUSE_MOVE && m_mouseLeftButtonDown)
		{//�϶���Ч
			//�����϶��Ľ��
			int dx = x - m_startX;
			m_startX = x;
			if(m_changeRow->position.x - m_showStartRow - m_tableRow->position.x + m_changeRow->pixSize.x * m_size.x + dx > m_showPixWidth)
			{//����ƶ�������ȡ���ƶ���������չ�����
				setRowWidth((m_showPixWidth - m_changeRow->position.x + m_showStartRow + m_tableRow->position.x) / m_size.x,m_changeRowOrder);
				m_needShowMove = XFalse;			//ȡ���ƶ�ͼ�����ʾ
				m_mouseLeftButtonDown = XFalse;
			}else
			if(m_changeRow->pixSize.x *m_size.x + dx < MUTILIST_MIN_WIDTH * m_size.x)
			{//���С����Сֵ����ʹ����Сֵ����ȡ���ƶ�
				setRowWidth(MUTILIST_MIN_WIDTH,m_changeRowOrder);
				m_needShowMove = XFalse;			//ȡ���ƶ�ͼ�����ʾ
				m_mouseLeftButtonDown = XFalse;
			}else
			{
				if(!setRowWidth(m_changeRow->pixSize.x + dx / m_size.x,m_changeRowOrder))
				{//�϶�ʧ����ȥ������
					m_needShowMove = XFalse;			//ȡ���ƶ�ͼ�����ʾ
					m_mouseLeftButtonDown = XFalse;
				}
				if(!m_withoutTex)
				{
					m_spriteMove.setPosition(m_changeRow->position.x - m_showStartRow + m_changeRow->pixSize.x * m_size.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
						m_changeRow->position.y);
				}
			}
		}
	}
	if(mouseState == MOUSE_LEFT_BUTTON_UP && m_mouseLeftButtonDown)
	{//���̸����ȥ������
		m_needShowMove = XFalse;			//ȡ���ƶ�ͼ�����ʾ
		m_mouseLeftButtonDown = XFalse;
	}
	//�����ж���ѡ
	if(m_tableLineSum > 0)
	{
		if(mouseState == MOUSE_LEFT_BUTTON_DOWN)
		{//�������
			//�����ѡ��Χ
			_XRect tempRect;
			if(m_withoutTex)
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_size.x,
					m_position.y + m_mouseRect.top * m_size.x + DEFAULT_TITLE_HEIGHT * m_size.y,
					m_position.x + m_mouseRect.left * m_size.x + m_showPixWidth ,
					m_position.y + m_mouseRect.top * m_size.x + DEFAULT_TITLE_HEIGHT * m_size.y + m_showPixHight);
			}else
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_size.x,
					m_position.y + m_mouseRect.top * m_size.x + m_mutiListTitle->textureSize.y * m_size.y,
					m_position.x + m_mouseRect.left * m_size.x + m_showPixWidth ,
					m_position.y + m_mouseRect.top * m_size.x + m_mutiListTitle->textureSize.y * m_size.y + m_showPixHight);
			}
			if(tempRect.isInRect(x,y))
			{//�����ѡ������һ��
				int DLine = (y - tempRect.top) / m_nowTextHeight;
				if(DLine + m_showStartLine < m_tableLineSum)
				{//��ѡ��Ч
					m_haveSelect = XTrue;
					m_selectLineOrder = DLine + m_showStartLine;
					updateSelectLine();
					//�����갴����Ч
					m_mouseMoveDown = XTrue;
					m_oldLineOrder = m_selectLineOrder;
					if(m_funSelectFun != NULL) (*m_funSelectFun)(m_pClass,m_objectID);
				}
			}
		}
		if(mouseState == MOUSE_MOVE && m_mouseMoveDown)
		{//�϶���Ч
			//�����ѡ��Χ(ע��������չ��20��Ϊ��ʵ���϶�������Χ�Ĵ���)
			_XRect tempRect;
			if(m_withoutTex)
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_size.x,
					m_position.y + m_mouseRect.top * m_size.x + DEFAULT_TITLE_HEIGHT * m_size.y - 100,
					m_position.x + m_mouseRect.left * m_size.x + m_showPixWidth,
					m_position.y + m_mouseRect.top * m_size.x + DEFAULT_TITLE_HEIGHT * m_size.y + m_showPixHight + 100);
			}else
			{
				tempRect.set(m_position.x + m_mouseRect.left * m_size.x,
					m_position.y + m_mouseRect.top * m_size.x + m_mutiListTitle->textureSize.y * m_size.y - 100,
					m_position.x + m_mouseRect.left * m_size.x + m_showPixWidth,
					m_position.y + m_mouseRect.top * m_size.x + m_mutiListTitle->textureSize.y * m_size.y + m_showPixHight + 100);
			}
			if(tempRect.isInRect(x,y))
			{
				int DLine = (y - tempRect.top - 100) / m_nowTextHeight;
				if(DLine < 0 && m_showStartLine > 0)
				{//��������,����һ�񲢽�������ʾ�����ƶ�
					if(m_withMouseDrag && moveUpLine(m_oldLineOrder))
					{
						m_oldLineOrder --;
						m_showStartLine --;
						m_verticalSlider.setNowValue(m_showStartLine);
						updateShowChange();
						updateSliderState();
					}
				}else
				if(DLine >= m_canShowLineSum && m_showStartLine < m_tableLineSum - m_canShowLineSum)
				{//��������,����һ�񲢽������ʾ�����ƶ�
					if(m_withMouseDrag && moveDownLine(m_oldLineOrder))
					{
						m_oldLineOrder ++;
						m_showStartLine ++;
						m_verticalSlider.setNowValue(m_showStartLine);
						updateShowChange();
						updateSliderState();
					}
				}else
				if(DLine + m_showStartLine < m_oldLineOrder)
				{//�����ƶ�
					if(m_withMouseDrag && moveUpLine(m_oldLineOrder))
					{
						m_oldLineOrder --;
					}
				}else
				if(DLine + m_showStartLine > m_oldLineOrder)
				{//�����ƶ�
					if(m_withMouseDrag && moveDownLine(m_oldLineOrder))
					{
						m_oldLineOrder ++;
					}
				}
			}
		}
	}
	if(mouseState == MOUSE_LEFT_BUTTON_UP && m_mouseMoveDown)
	{
		m_mouseMoveDown = XFalse;
		m_oldLineOrder = m_selectLineOrder;
	}
	return XTrue;
}
void _XMultiList::updateSelectLine()
{
	if(!m_isInited) return;		//���û�г�ʼ��ֱ���˳�
	if(!m_haveSelect) return;
	if(m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum && !m_withoutTex)
	{//����ѡ���λ��
		m_spriteSelect.setPosition(m_position.x + m_mouseRect.left * m_size.x,
			m_position.y + (m_mouseRect.top + m_mutiListTitle->textureSize.y) * m_size.y + 
			(m_selectLineOrder - m_showStartLine) * m_nowTextHeight);
	}
}
//�ļ���������ʽΪXMultiList_ID.dat
_XBool _XMultiList::exportData(const char *fileName)	//���ݵ���
{//���ݵ������ļ���
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	FILE *fp;
	if(fileName == NULL)
	{
		char fileNameT[] = "XMultiList_xxx.dat";
		fileNameT[10] = (m_objectID /100) % 10 + '0';
		fileNameT[11] = (m_objectID /10) % 10 + '0';
		fileNameT[12] = m_objectID % 10 + '0';	 
		if((fp = fopen(fileNameT,"wb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}else
	{
		if((fp = fopen(fileName,"wb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}
	//д������Ϣ
	_XMultiListOneRow *tempRow = m_tableRow;
	int strLength = 0;
	float tempWidth = 0;
	fwrite(&m_tableRowSum,sizeof(int),1,fp);
	for(int i = 0;i < m_tableRowSum;++ i)
	{
		tempWidth = tempRow->pixSize.x;
		fwrite(&(tempWidth),sizeof(float),1,fp);
		strLength = strlen(tempRow->title);
		fwrite(&(strLength),sizeof(int),1,fp);
		fwrite(tempRow->title,strLength + 1,1,fp);
		tempRow = tempRow->nextRow;
	}
	//д�������Ϣ
	_XMultiListOneBox *tempBox = m_tableBox;
	fwrite(&m_tableLineSum,sizeof(int),1,fp);
	for(int i = 0;i < m_tableLineSum * m_tableRowSum;++ i)
	{
		strLength = strlen(tempBox->string);
		fwrite(&(strLength),sizeof(int),1,fp);
		fwrite(tempBox->string,strLength + 1,1,fp);
		tempBox = tempBox->nextBox;
	}
	fclose(fp);
	return XTrue;
}
_XBool _XMultiList::importData(const char *fileName)	//���ݵ���
{//���ļ��н����ݵ���
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	FILE *fp;
	if(fileName == NULL)
	{
		char fileNameT[] = "XMultiList_xxx.dat";
		fileNameT[10] = (m_objectID /100) % 10 + '0';
		fileNameT[11] = (m_objectID /10) % 10 + '0';
		fileNameT[12] = m_objectID % 10 + '0';	 
		if((fp = fopen(fileNameT,"rb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}else
	{
		if((fp = fopen(fileName,"rb")) == NULL)
		{
			printf("File open error!\n");
			return XFalse;
		}
	}
	//��ȡ����Ϣ
	int strLength = 0;
	int tempSum = 0;
	float tempWidth = 0;
	char *str;
	fread(&tempSum,sizeof(int),1,fp);
	if(tempSum != m_tableRowSum)
	{//���񲻷���
		setRowSum(tempSum);
	}
	for(int i = 0;i < m_tableRowSum;++ i)
	{
		fread(&tempWidth,sizeof(float),1,fp);
		setRowWidth(tempWidth,i);
		fread(&(strLength),sizeof(int),1,fp);
		str = createArrayMem<char>(strLength + 1);
		if(str == NULL) return XFalse;
		fread(str,strLength + 1,1,fp);
		setTileStr(str,i);
		XDELETE_ARRAY(str);
	}
	//��ȡ������Ϣ
	fread(&tempSum,sizeof(int),1,fp);
	if(tempSum != m_tableLineSum)
	{//���񲻷���
		setLineSum(tempSum);
	}
	for(int i = 0;i < m_tableLineSum * m_tableRowSum;++ i)
	{
		fread(&(strLength),sizeof(int),1,fp);
		str = createArrayMem<char>(strLength + 1);
		if(str == NULL) return XFalse;
		fread(str,strLength + 1,1,fp);
		setBoxStr(str,i / m_tableRowSum,i % m_tableRowSum);
		XDELETE_ARRAY(str);
	}
	fclose(fp);
	return XTrue;
}
void _XMultiList::initANewRowData(_XMultiListOneRow * upRow,int i)
{
	if(i == 0)
	{//��һ��Ԫ��
		m_tableRow->isEnable = XTrue;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setSize(m_size * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_caption.getTextSize().y * m_caption.getSize().x);
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_size.x,
			m_position.y + m_mouseRect.top * m_size.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);\
		if(!m_withoutTex)
		{
			m_tableRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().decreaseAObject(&(m_tableRow->m_spriteTitle));
#endif
			m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
			m_tableRow->m_spriteTitle.setSize(m_tableRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
			m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
			m_tableRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().decreaseAObject(&(m_tableRow->m_spriteTitleEnd));
#endif
			m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_size.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
			m_tableRow->m_spriteTitleEnd.setSize(m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y);
			m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);
		}

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_size.x / m_nowTextWidth;
		m_tableRow->title = NULL;
		setTileStr("Title",0);	//����Ĭ����ʾ
		m_tableRow->isShow = XTrue;
	}else
	{
		upRow->nextRow->isEnable = XTrue;
		upRow->nextRow->order = i;
		upRow->nextRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(upRow->nextRow->text));
#endif
		upRow->nextRow->text.setSize(m_size * m_fontSize);
		upRow->nextRow->pixSize.set(MUTILIST_MIN_WIDTH,m_caption.getTextSize().y * m_caption.getSize().x);
		upRow->nextRow->position.set(upRow->position.x + upRow->pixSize.x * m_size.x,
			m_position.y + m_mouseRect.top * m_size.y);
		upRow->nextRow->text.setPosition(upRow->nextRow->position.x,upRow->nextRow->position.y);
		if(!m_withoutTex)
		{
			upRow->nextRow->m_spriteTitle.init(m_mutiListTitle->texture.m_w,m_mutiListTitle->texture.m_h,1);	//���ñ��ⱳ���ľ���
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().decreaseAObject(&(upRow->nextRow->m_spriteTitle));
#endif
			upRow->nextRow->m_spriteTitle.setPosition(upRow->nextRow->position);
			upRow->nextRow->m_spriteTitle.setSize(m_tableRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
			upRow->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
			upRow->nextRow->m_spriteTitleEnd.init(m_mutiListTitleEnd->texture.m_w,m_mutiListTitleEnd->texture.m_h,1);
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().decreaseAObject(&(upRow->nextRow->m_spriteTitleEnd));
#endif
			upRow->nextRow->m_spriteTitleEnd.setPosition(upRow->nextRow->position.x + m_tableRow->pixSize.x * m_size.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			upRow->nextRow->m_spriteTitleEnd.setSize(m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y);
			upRow->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);
		}

		upRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_size.x / m_nowTextWidth;
		upRow->nextRow->title = NULL;
		setTileStr("Title",i);	//����Ĭ����ʾ
		upRow->nextRow->isShow = XTrue;
	}
}
_XBool _XMultiList::setRowSum(int rowSum)		//��������������������հף�����ɾ��
{
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	if(rowSum < 0) return XFalse;			//�Ƿ�����ֱ�ӷ�Χʧ��
	if(rowSum == m_tableRowSum) return XTrue;	//�������û�иı�����ֱ�ӷ��سɹ�
	if(rowSum > MUTILIST_MAX_ROW_SUM) return XFalse;	//�����е����ֵ�����˳�
	//�������ʵ������޸Ŀؼ��е�����
	if(rowSum > m_tableRowSum)
	{//����
		int i,j;
		int oldRowSum = m_tableRowSum;
		m_tableRowSum = rowSum;
		//���ӱ�������
		_XMultiListOneRow *tempRow = m_tableRow;
		_XMultiListOneRow *temp = NULL;
		if(oldRowSum == 0)
		{//ȫ������
			m_tableRow = createMem<_XMultiListOneRow>();
			if(m_tableRow == NULL) return XFalse;

			initANewRowData(NULL,0);

			tempRow = m_tableRow;
			for(i = 1;i < m_tableRowSum;++ i)
			{
				tempRow->nextRow = createMem<_XMultiListOneRow>();
				if(tempRow->nextRow == NULL) return XFalse;

				initANewRowData(tempRow,i);

				tempRow = tempRow->nextRow;
				//ע������û�м���ڴ����ʧ��
			}
			tempRow->nextRow = NULL;	//���һ����β

		}else
		{
			for(i = 0;i < m_tableRowSum;++ i)
			{
				if(i >= oldRowSum)
				{//�����µ���
					temp->nextRow = createMem<_XMultiListOneRow>();
					if(temp->nextRow == NULL) return 0;

					initANewRowData(temp,i);

					temp = temp->nextRow;
					temp->nextRow = tempRow;
				}else
				{
					temp = tempRow;
					tempRow = tempRow->nextRow;
				}
			}
		}
		//���ӱ�������
		_XMultiListOneBox *tempBox = m_tableBox;
		_XMultiListOneBox *temp1 = NULL;
		if(oldRowSum == 0)
		{//ȫ������
			m_tableLineSum = 0;
			m_tableBox = NULL;
		}else
		{
			if(m_tableLineSum > 0)
			{
				for(i = 0;i < m_tableLineSum;++ i)
				{
					tempRow = m_tableRow;
					for(j = 0;j < m_tableRowSum;++ j)
					{
						if(j >= oldRowSum)
						{
							temp1->nextBox = createMem<_XMultiListOneBox>();
							if(temp1->nextBox == NULL) return 0;

							temp1 = temp1->nextBox;
							temp1->isEnable = 1;

							temp1->isShow = 1;
							temp1->order.set(j,i);
							temp1->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
							_XObjectManager::GetInstance().decreaseAObject(&(temp1->text));
#endif
							temp1->text.setSize(m_size * m_fontSize);
							temp1->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
							temp1->string = NULL;
							setBoxStr("Line",i,j);

							temp1->nextBox = tempBox;
							tempRow = tempRow->nextRow;
						}else
						{
							temp1 = tempBox;
							tempBox = tempBox->nextBox;
							tempRow = tempRow->nextRow;
						}
					}
				}
			}
		}
	}else
	{//ɾ��
		int i,j;
		//ɾ����������
		_XMultiListOneRow *tempRow = m_tableRow;
		_XMultiListOneRow *temp = NULL;
		if(rowSum > 0)
		{
			for(i = 0;i < m_tableRowSum;++ i)
			{
				if(i >= rowSum)
				{//ɾ������Ԫ�ص�����
					temp->nextRow = tempRow->nextRow;
					XDELETE_ARRAY(tempRow->title);
					XDELETE(tempRow);
					tempRow = temp->nextRow;
				}else
				{
					temp = tempRow;
					tempRow = tempRow->nextRow;
				}
			}
		}else
		{//ȫ��ɾ��
			tempRow = m_tableRow;
			for(i = 0;i < m_tableRowSum;++ i)
			{
				temp = tempRow;
				tempRow = tempRow->nextRow;
				XDELETE_ARRAY(temp->title);
				XDELETE(temp);
			}
			m_tableRowSum = 0;
			m_tableRow = NULL;
		}
		//ɾ����������
		_XMultiListOneBox *tempBox = m_tableBox;
		_XMultiListOneBox *temp1 = NULL;
		if(rowSum > 0)
		{
			for(i = 0;i < m_tableLineSum;++ i)
			{
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j >= rowSum)
					{//ɾ�����ڵ�Ԫ��
						//�ͷ���Դ
						temp1->nextBox = tempBox->nextBox;
						XDELETE_ARRAY(tempBox->string);
						XDELETE(tempBox);
						tempBox = temp1->nextBox;
					}else
					{
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
					}
				}
			}
			//������ص���ֵ
			m_tableRowSum = rowSum;
		}else
		{
			tempBox = m_tableBox;
			for(i = 0;i < m_tableLineSum;++ i)
			{
				for(j = 0;j < m_tableRowSum;++ j)
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					XDELETE_ARRAY(temp1->string);
					XDELETE(temp1);
				}
			}
			m_tableLineSum = 0;
			m_tableBox = NULL;
		}
	}
	updateShowChange();
	updateSliderState();
	return XTrue;
}
void _XMultiList::initANewBoxData(_XMultiListOneBox * nowBox,_XMultiListOneRow * nowRow,int i,int j)
{
	nowBox->isEnable = XTrue;

	nowBox->isShow = XTrue;
	nowBox->order.set(j,i);
	nowBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&(nowBox->text));
#endif
	nowBox->text.setSize(m_size * m_fontSize);
	nowBox->text.setPosition(nowRow->position.x,nowRow->position.y + m_nowTextHeight * (i + 1));
	nowBox->string = NULL;
	setBoxStr("Line",i,j);
}
_XBool _XMultiList::setLineSum(int lineSum)	//���������������Ĳ�����հף�����ɾ��
{
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	if(lineSum < 0) return XFalse;			//�Ƿ�����ֱ�ӷ�Χʧ��
	if(m_tableRowSum <= 0) return XFalse;	//�Ƿ�����ֱ�ӷ�Χʧ��
	if(lineSum == m_tableLineSum) return XTrue;	//�������û�иı�����ֱ�ӷ��سɹ�
	int i,j;
	if(m_haveSelect != 0 && m_selectLineOrder >= lineSum) m_haveSelect = XFalse;
	//�������ʵ������޸Ŀؼ��е�����
	if(lineSum > m_tableLineSum)
	{//����
		int oldLine = m_tableLineSum;
		m_tableLineSum = lineSum;
		_XMultiListOneBox *tempBox = m_tableBox;
		_XMultiListOneRow *tempRow;
		if(oldLine == 0)
		{//ȫ������
			m_tableBox = createMem<_XMultiListOneBox>();
			if(m_tableBox == NULL) return 0;

			initANewBoxData(m_tableBox,m_tableRow,0,0);

			tempBox = m_tableBox;
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j == 0 && i == 0) 
					{
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->nextBox = createMem<_XMultiListOneBox>();
					if(tempBox->nextBox == NULL) return XFalse;

					tempBox = tempBox->nextBox;
					initANewBoxData(tempBox,tempRow,i,j);

					tempRow = tempRow->nextRow;
					//ע������û�м���ڴ����ʧ��
				}
			}
			tempBox->nextBox = NULL;	//���һ����β
		}else
		{//����
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(i >= oldLine || (i == oldLine - 1 && j == m_tableRowSum - 1))	//���һ��Ԫ��ҲҪ��ô����
					{
						if(i == m_tableLineSum - 1 && j == m_tableRowSum - 1)
						{//���һ��Ԫ��
							continue;
						}
						tempBox->nextBox = createMem<_XMultiListOneBox>();
						if(tempBox->nextBox == NULL) return XFalse;

						tempBox = tempBox->nextBox;
						tempBox->isEnable = XTrue;

						tempBox->isShow = 1;
						tempBox->order.set(j,i);
						tempBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
						_XObjectManager::GetInstance().decreaseAObject(&(tempBox->text));
#endif
						tempBox->text.setSize(m_size * m_fontSize);
						tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
						tempBox->string = NULL;
						if(j + 1 >= m_tableRowSum)
						{
							setBoxStr("Line",i + 1,0);
						}else
						{
							setBoxStr("Line",i,j + 1);
						}

						tempRow = tempRow->nextRow;
						//ע������û�м���ڴ����ʧ��
					}else
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
					}
				}
			}
			tempBox->nextBox = NULL;	//���һ����β
		}
	}else
	{//ɾ��
		_XMultiListOneBox *tempBox = m_tableBox;
		_XMultiListOneBox *temp1 = NULL;
		if(lineSum == 0)
		{//ȫ��ɾ��
			for(i = 0;i < m_tableLineSum;++ i)
			{
				for(j = 0;j < m_tableRowSum;++ j)
				{//ɾ�����Ԫ��
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					XDELETE_ARRAY(temp1->string);
					XDELETE(temp1);
				}
			}
			m_tableLineSum = lineSum;
			m_tableBox = NULL;
		}else
		{
			for(i = 0;i < m_tableLineSum;++ i)
			{
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(i >= lineSum)
					{//ɾ�����Ԫ��
						temp1->nextBox = tempBox->nextBox;
						XDELETE_ARRAY(tempBox->string);
						XDELETE(tempBox);
						tempBox = temp1->nextBox;
					}else
					{
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
					}
				}
			}
			m_tableLineSum = lineSum;
		}
	}
	updateShowChange();
	updateSliderState();
	return XTrue;
}
_XBool _XMultiList::deleteLine(int order)		//ɾ��ĳһ��
{
	if(!m_isInited) return XFalse;							//���û�г�ʼ��ֱ���˳�
	if(order < 0 || order >= m_tableLineSum) return XFalse;		//�Ƿ�������
	int i,j;
	_XMultiListOneRow *tempRow = m_tableRow;
	_XMultiListOneBox *tempBox = m_tableBox;
	_XMultiListOneBox *temp1 = NULL;
	if(order == 0)
	{//��ͷ,ɾ����һ��
		for(i = 0;i < m_tableRowSum;++ i)
		{
			temp1 = tempBox->nextBox;
			XDELETE_ARRAY(tempBox->string);
			XDELETE(tempBox);
			tempBox = temp1;
		}
		m_tableBox = tempBox;
		//�ı���浥Ԫ���λ��
		if(m_tableLineSum > 1)
		{
			tempBox = m_tableBox;
			m_tableBox->order.set(0,0);
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_nowTextHeight * 1);
			for(i = 0;i < m_tableLineSum - 1;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(i == 0 && j == 0)
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->order.set(i,j);
					tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * 1);
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}else
	{
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order)
				{//����ɾ��Ҫ��
					temp1->nextBox = tempBox->nextBox;
					XDELETE_ARRAY(tempBox->string);
					XDELETE(tempBox);
					tempBox = temp1->nextBox;
				}else
				if(i > order)
				{//���ú��浥Ԫ���λ��
					if(temp1->nextBox != NULL)
					{
						temp1->nextBox->order.set(j,i);
						temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
					}
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	m_tableLineSum --;
	if(m_haveSelect != 0)
	{//����ѡ��
		if(m_selectLineOrder == order)
		{//����ɾ������ѡ�����һ�У���ȡ��ѡ��
			m_haveSelect = 0;
			updateSelectLine();
		}else
		if(m_selectLineOrder > order)
		{
			m_selectLineOrder --;
			updateSelectLine();
		}
	}
	updateShowChange();
	updateSliderState();
	return XTrue;
}
_XBool _XMultiList::deleteRow(int order)		//ɾ��ĳһ��
{
	if(!m_isInited) return XFalse;							//���û�г�ʼ��ֱ���˳�
	if(order < 0 || order >= m_tableRowSum) return XFalse;		//�Ƿ�������
	if(order == 0)
	{
		int i,j;
		//ɾ����ͷ
		_XMultiListOneRow *tempRow = m_tableRow;
		_XMultiListOneRow *temp = tempRow->nextRow;
		XDELETE_ARRAY(tempRow->title);
		XDELETE(tempRow);
		if(m_tableRowSum == 1)
		{//���ֻ��һ��
			m_tableRow = NULL;
		}else
		{
			m_tableRow = temp;
			//���ñ����λ��
			m_tableRow->order = 0;
			m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_size.x + MUTILIST_MIN_WIDTH * m_size.x * 0,
				m_position.y + m_mouseRect.top * m_size.y);
			m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
			if(!m_withoutTex)
			{
				m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
				m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_size.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			for(i = 1;i < m_tableRowSum - 1;++ i)	//�Ѿ�ɾ����һ��Ԫ��
			{
				temp->nextRow->order = i;
				temp->nextRow->position.set(temp->position.x + temp->pixSize.x * m_size.x,
					m_position.y + m_mouseRect.top * m_size.y);
				temp->nextRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
				if(!m_withoutTex)
				{
					temp->nextRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_size.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				temp = temp->nextRow;
			}
		}
		//ɾ������Ԫ��
		_XMultiListOneBox *tempBox = m_tableBox;
		_XMultiListOneBox *temp1 = NULL;

		tempBox = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == 0 && j == 0)
				{//��һ��Ԫ�ص�ɾ�����⴦����������������
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
				//	tempRow = tempRow->nextRow;
					continue;
				}else
				if(j == order)
				{
					temp1->nextBox = tempBox->nextBox;
					XDELETE_ARRAY(tempBox->string);
					XDELETE(tempBox);
					tempBox = temp1->nextBox;
				//	tempRow = tempRow->nextRow;
				}else
				{
				//	if(i == m_tableLineSum - 1 && j == m_tableRowSum - 1)
				//	{//���һ��Ԫ���˳�
				//		break;
				//	}
					if(tempBox != NULL)
					{
						if(m_tableRowSum > 1)
						{
							tempBox->order.set(j,i);
							tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
							tempRow = tempRow->nextRow;
						}
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
					}
				}
			}
		}
		//ɾ����һ��Ԫ��
		tempBox = m_tableBox;
		temp1 = tempBox->nextBox;
		XDELETE_ARRAY(tempBox->string);
		XDELETE(tempBox);
		if(m_tableRowSum == 1)
		{//���ֻ��һ�У���ɾ��֮��û�е�λ����
			m_tableBox = NULL;
			m_tableLineSum = 0;
		}else
		{
			tempBox = temp1;
			m_tableBox = tempBox;
			m_tableBox->order.set(0,0);
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_nowTextHeight * 0);
		}
	}else
	{
		int i,j;
		//ɾ����ͷ
		_XMultiListOneRow *tempRow = m_tableRow;
		_XMultiListOneRow *temp = tempRow->nextRow;
		for(i = 0;i < m_tableRowSum;++ i)
		{
			if(i == order)
			{
				temp->nextRow = tempRow->nextRow;
				XDELETE_ARRAY(tempRow->title);
				XDELETE(tempRow);
				tempRow = temp->nextRow;
				if(tempRow != NULL)
				{
					tempRow->order = i;
					tempRow->position.set(temp->position.x + temp->pixSize.x * m_size.x,
						m_position.y + m_mouseRect.top * m_size.y);
					tempRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
					if(!m_withoutTex)
					{
						tempRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_size.x - 
							m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
							m_tableRow->position.y);
					}
				}
			}else
			if(i > order)
			{
				tempRow->order = i;
				tempRow->position.set(temp->position.x + temp->pixSize.x * m_size.x,
					m_position.y + m_mouseRect.top * m_size.y);
				tempRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
				if(!m_withoutTex)
				{
					tempRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_size.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}else
			{
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}
		}
		//ɾ������Ԫ��
		_XMultiListOneBox *tempBox = m_tableBox;
		_XMultiListOneBox *temp1 = NULL;

		tempBox = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == order)
				{
					temp1->nextBox = tempBox->nextBox;
					XDELETE_ARRAY(tempBox->string);
					XDELETE(tempBox);
					tempBox = temp1->nextBox;
					//	tempRow = tempRow->nextRow;
				}else
				{
				//	if(i == m_tableLineSum - 1 && j == m_tableRowSum - 1)
				//	{//���һ��Ԫ���˳�
				//		break;
				//	}
					if(tempBox != NULL)
					{
						tempBox->order.set(j,i);
						tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
						temp1 = tempBox;
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
					}
				}
			}
		}
	}
	m_tableRowSum--;
	updateShowChange();
	updateSliderState();
	return XTrue;
}
_XBool _XMultiList::insertALine(int order)		//��order������һ��
{
	if(!m_isInited) return XFalse;
	if(order < 0 || order >= m_tableLineSum) return XFalse;
	int i,j;
	if(order == 0)
	{//��ͷ������
		_XMultiListOneBox *tempBox = m_tableBox;
		_XMultiListOneRow *tempRow = m_tableRow;
		_XMultiListOneBox *temp1 = m_tableBox;
		m_tableBox = createMem<_XMultiListOneBox>();
		if(m_tableBox == NULL) return XFalse;

		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = 1;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setSize(m_size * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_nowTextHeight * 1);
		m_tableBox->string = NULL;
		setBoxStr("Line",0,0);
		tempBox = m_tableBox;
		for(i = 0;i < m_tableRowSum - 1;++ i)
		{
			tempBox->nextBox = createMem<_XMultiListOneBox>();
			if(tempBox->nextBox == NULL) return XFalse;

			tempBox->nextBox->isEnable = XTrue;
			tempBox->nextBox->isShow = 1;
			tempBox->nextBox->order.set(i,0);
			tempBox->nextBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
			_XObjectManager::GetInstance().decreaseAObject(&(tempBox->nextBox->text));
#endif
			tempBox->nextBox->text.setSize(m_size * m_fontSize);
			tempBox->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * 1);
			tempBox->nextBox->string = NULL;
			setBoxStr("Line",0,i + 1);
			tempBox = tempBox->nextBox;
			tempRow = tempRow->nextRow;
		}
		tempBox->nextBox = temp1;
		//�����������Ԫ�������ƶ�
		tempBox = m_tableBox;

		m_tableLineSum ++;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == 0 && j == 0)
				{
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
					continue;
				}
				tempBox->order.set(j,i);
				tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
				tempBox = tempBox->nextBox;
				tempRow = tempRow->nextRow;
			}
		}

		if(m_haveSelect)
		{
			if(m_selectLineOrder >= order)
			{
				m_selectLineOrder ++;
				updateSelectLine();
			}
		}
	}else
	{
		_XMultiListOneRow *tempRow = m_tableRow;
		_XMultiListOneBox *tempBox = m_tableBox;
		_XMultiListOneBox *temp1 = NULL;

		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order)
				{
					temp1->nextBox = createMem<_XMultiListOneBox>();
					if(temp1->nextBox == NULL) return 0;

					temp1->nextBox->isEnable = 1;
					temp1->nextBox->isShow = 1;
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
					_XObjectManager::GetInstance().decreaseAObject(&(temp1->nextBox->text));
#endif
					temp1->nextBox->text.setSize(m_size * m_fontSize);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
					temp1->nextBox->string = NULL;
					setBoxStr("Line",i,j);
					temp1 = temp1->nextBox;
					temp1->nextBox = tempBox;
				//	if(j == m_tableRowSum - 1)
				//	{//���һ�����ӵ�Ԫ��
				//		temp1->nextBox = tempBox;
				//	}
					tempRow = tempRow->nextRow;
				}else
				if(i > order)
				{//�ı�λ��
					if(temp1->nextBox != NULL)
					{
						temp1->nextBox->order.set(j,i);
						temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
					}
					temp1 = tempBox;
					tempBox = tempBox->nextBox;	
					tempRow = tempRow->nextRow;
				}else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;	
					tempRow = tempRow->nextRow;
				}
			}
		}
		m_tableLineSum ++;
		if(m_haveSelect)
		{
			if(m_selectLineOrder >= order)
			{
				m_selectLineOrder ++;
				updateSelectLine();
			}
		}
	}
	updateShowChange();
	updateSliderState();
	return XTrue;
}
_XBool _XMultiList::insertARow(int order)		//��order������һ��
{
	if(!m_isInited) return XFalse;
	if(order < 0 || order >= m_tableRowSum) return XFalse;
	if(m_tableRowSum >= MUTILIST_MAX_ROW_SUM) return XFalse;	//�����е����ֵ�����˳�
	int i,j;
	_XMultiListOneRow *tempRow = m_tableRow;
	_XMultiListOneRow *temp = NULL;
	_XMultiListOneBox *tempBox = m_tableBox;
	_XMultiListOneBox *temp1 = NULL;
	if(order == 0)
	{//��ͷ
		//�ı������
		tempRow = m_tableRow;
		m_tableRow = createMem<_XMultiListOneRow>();
		if(m_tableRow == NULL) return 0;

		initANewRowData(NULL,0);
/*		m_tableRow->isEnable = 1;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
		m_tableRow->text.setSize(m_size * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_caption.getTextSize().y * m_caption.getSize().x);
		m_tableRow->position.set(m_objRect.left + m_edgeDistance.left * m_size.x + MUTILIST_MIN_WIDTH * m_size.x * 0,
			m_objRect.top + m_edgeDistance.top * m_size.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		m_tableRow->m_spriteTitle.init(1,m_mutiListTitle->m_w,m_mutiListTitle->m_h,1);	//���ñ��ⱳ���ľ���
		m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitle.setSize(m_tableRow->pixSize.x / m_mutiListTitle->m_w,1.0);
		m_tableRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
		m_tableRow->m_spriteTitleEnd.init(1,m_mutiListTitleEnd->m_w,m_mutiListTitleEnd->m_h,1);
		m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
		m_tableRow->m_spriteTitleEnd.setSize(m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y);
		m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x / (m_tableRow->text.m_size.x * m_tableRow->text.m_showSize.x * 0.5);
		m_tableRow->title = NULL;
		setTileStr("Title",0);	//����Ĭ����ʾ
		m_tableRow->isShow = 1;*/

		m_tableRow->nextRow = tempRow;
		tempRow = m_tableRow;
		for(i = 0;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow->order = i + 1;
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_size.x,
				m_position.y + m_mouseRect.top * m_size.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			if(!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x * m_size.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			tempRow = tempRow->nextRow;
		}
		//�ı��������
		m_tableRowSum ++;
		//�����һ��Ԫ��
		tempBox = m_tableBox;
		m_tableBox = createMem<_XMultiListOneBox>();
		if(m_tableBox == NULL) return 0;

		m_tableBox->isEnable = XTrue;
		m_tableBox->isShow = 1;
		m_tableBox->order.set(0,0);
		m_tableBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableBox->text));
#endif
		m_tableBox->text.setSize(m_size * m_fontSize);
		m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_nowTextHeight * 1);
		m_tableBox->string = NULL;
		setBoxStr("Line",0,0);
		m_tableBox->nextBox = tempBox;
		tempBox = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;j++)
			{
				if(i == 0 && j == 0)
				{//��һ��Ԫ����ʱ������
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				if(j == order)
				{//����
					temp1->nextBox = createMem<_XMultiListOneBox>();
					if(temp1->nextBox == NULL) return 0;

					temp1->nextBox->isEnable = XTrue;
					temp1->nextBox->isShow = 1;
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
					_XObjectManager::GetInstance().decreaseAObject(&(temp1->nextBox->text));
#endif
					temp1->nextBox->text.setSize(m_size * m_fontSize);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
					temp1->nextBox->string = NULL;
					setBoxStr("Line",i,j);
					temp1 = temp1->nextBox;
					temp1->nextBox = tempBox;
					tempRow = tempRow->nextRow;
				}else
				if(j > order)
				{//�ƶ�
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}else
	{
		//�ı������
		tempRow = m_tableRow;
		for(i = 0;i < m_tableRowSum + 1;++ i)
		{
			if(i == order)
			{//����
				temp->nextRow = createMem<_XMultiListOneRow>();
				if(temp->nextRow == NULL) return 0;

				initANewRowData(temp,i);
			/*	temp->nextRow->isEnable = 1;
				temp->nextRow->order = i;
				temp->nextRow->text.setACopy(m_caption);
				temp->nextRow->pixSize.set(MUTILIST_MIN_WIDTH * m_size.x,m_tableRow->text.m_size.y * m_tableRow->text.m_showSize.x);
				temp->nextRow->position.set(temp->position.x + temp->pixSize.x,
					m_objRect.top + m_edgeDistance.top * m_size.y);
				temp->nextRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
				temp->nextRow->m_spriteTitle.init(1,m_mutiListTitle->m_w,m_mutiListTitle->m_h,1);	//���ñ��ⱳ���ľ���
				temp->nextRow->m_spriteTitle.setPosition(temp->nextRow->position);
				temp->nextRow->m_spriteTitle.setSize(m_tableRow->pixSize.x / m_mutiListTitle->m_w,1.0);
				temp->nextRow->m_spriteTitle.setIsTransformCenter(POINT_LEFT_TOP);	//���ñ������������Ϊ���ϽǶ���
				temp->nextRow->m_spriteTitleEnd.init(1,m_mutiListTitleEnd->m_w,m_mutiListTitleEnd->m_h,1);
				temp->nextRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
				temp->nextRow->m_spriteTitleEnd.setSize(m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y);
				temp->nextRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

				temp->nextRow->stringShowWidth = m_tableRow->pixSize.x / (m_tableRow->text.m_size.x * m_tableRow->text.m_showSize.x * 0.5);
				temp->nextRow->title = NULL;
				setTileStr("Title",i);	//����Ĭ����ʾ
				temp->nextRow->isShow = 1;*/

				temp = temp->nextRow;
				temp->nextRow = tempRow;
			}else
			if(i > order)
			{
				temp->nextRow->order = i;
				temp->nextRow->position.set(temp->position.x + temp->pixSize.x * m_size.x,
					m_position.y + m_mouseRect.top * m_size.y);
				temp->nextRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
				if(!m_withoutTex)
				{
					temp->nextRow->m_spriteTitle.setPosition(temp->nextRow->position);
					temp->nextRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_size.x -
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}else
			{
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}
		}
		//�ı��������
		m_tableRowSum ++;
		tempBox = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;j++)
			{
				if(i == 0 && j == 0)
				{//��һ��Ԫ����ʱ������
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				if(j == order)
				{//����
					temp1->nextBox = createMem<_XMultiListOneBox>();
					if(temp1->nextBox == NULL) return 0;

					temp1->nextBox->isEnable = XTrue;
					temp1->nextBox->isShow = 1;
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
					_XObjectManager::GetInstance().decreaseAObject(&(temp1->nextBox->text));
#endif
					temp1->nextBox->text.setSize(m_size * m_fontSize);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
					temp1->nextBox->string = NULL;
					setBoxStr("Line",i,j);
					temp1 = temp1->nextBox;
					temp1->nextBox = tempBox;
					tempRow = tempRow->nextRow;
				}else
				if(j > order)
				{//�ƶ�
					temp1->nextBox->order.set(j,i);
					temp1->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}else
				{
					temp1 = tempBox;
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	updateShowChange();
	updateSliderState();
	return XTrue;
}
_XBool _XMultiList::moveDownLine(int order)	//��order������
{
	if(!m_isInited) return XFalse;
	if(order < 0 || order >= m_tableLineSum - 1) return XFalse;	//ֻ��һ�в����ƶ������ݳ�����ΧҲ�����ƶ�
	//���濪ʼ�ƶ�
	_XMultiListOneBox *tempBox = m_tableBox;
	_XMultiListOneBox *temp1 = NULL;
	_XMultiListOneBox temp2;
	//2�е�6���˵�Ľڵ�
	_XMultiListOneBox * point0 = NULL;
	_XMultiListOneBox * point1 = NULL;
	_XMultiListOneBox * point2 = NULL;
	_XMultiListOneBox * point3 = NULL;
	_XMultiListOneBox * point4 = NULL;
	_XMultiListOneBox * point5 = NULL;
	int i,j;
	if(order == 0)
	{//�ƶ����ǵ�һ��
		temp1 = m_tableBox;
		point1 = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order)
				{
					if(j == m_tableRowSum - 1)
					{
						point2 = tempBox;
						point3 = tempBox->nextBox;
					}
					tempBox = tempBox->nextBox;
				}else
				if(i == order + 1)
				{
					temp2.order = tempBox->order;
					temp2.text.setPosition(tempBox->text.getPosition());
					tempBox->order = temp1->order;
					tempBox->text.setPosition(temp1->text.getPosition());
					temp1->order = temp2.order;
					temp1->text.setPosition(temp2.text.getPosition());
					if(j == m_tableRowSum - 1)
					{
						point4 = tempBox;
						point5 = tempBox->nextBox;
					}
					temp1 = temp1->nextBox;
					tempBox = tempBox->nextBox;
				}else
				if(i > order + 1)
				{//���ݽ������
					break;
				}else
				{
					tempBox = tempBox->nextBox;
				}
			}
		}
		//point0->nextBox = point3;
		point2->nextBox = point5;
		point4->nextBox = point1;
		m_tableBox = point3;
	}else
	{
		for(i = 0;i < m_tableLineSum;++ i)
		{
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == order - 1 && j == m_tableRowSum - 1)
				{
					point0 = tempBox;
					point1 = tempBox->nextBox;
				}
				if(i == order)
				{
					temp1 = tempBox;
					if(j == m_tableRowSum - 1)
					{
						point2 = tempBox;
						point3 = tempBox->nextBox;
					}
					tempBox = tempBox->nextBox;
				}else
				if(i == order + 1)
				{
					temp2.order = tempBox->order;
					temp2.text.setPosition(tempBox->text.getPosition());
					tempBox->order = temp1->order;
					tempBox->text.setPosition(temp1->text.getPosition());
					temp1->order = temp2.order;
					temp1->text.setPosition(temp2.text.getPosition());
					if(j == m_tableRowSum - 1)
					{
						point4 = tempBox;
						point5 = tempBox->nextBox;
					}
					temp1 = temp1->nextBox;
					tempBox = tempBox->nextBox;
				}else
				if(i > order + 1)
				{//���ݽ������
					break;
				}else
				{
					tempBox = tempBox->nextBox;
				}
			}
		}
		point0->nextBox = point3;
		point2->nextBox = point5;
		point4->nextBox = point1;
	}
	if(m_haveSelect)
	{
		if(m_selectLineOrder == order)
		{
			m_selectLineOrder ++;
			updateSelectLine();
		}else
		if(m_selectLineOrder == order + 1)
		{
			m_selectLineOrder --;
			updateSelectLine();
		}
	}
	updateShowChange();
	updateSliderState();
	return XTrue;
}
_XBool _XMultiList::moveRightRow(int order)	//��order������
{
	if(!m_isInited) return XFalse;
	if(order < 0 || order >= m_tableRowSum - 1) return XFalse;	//ֻ��һ�в����ƶ������ݳ�����ΧҲ�����ƶ�
	int i = 0,j = 0;
	_XMultiListOneRow *tempRow = m_tableRow;
	_XMultiListOneRow *temp1 = NULL;
	_XMultiListOneRow *point0 = NULL;
	_XMultiListOneRow *point1 = NULL;

	_XMultiListOneBox *tempBox = m_tableBox;
	_XMultiListOneBox *temp2 = NULL;
	_XMultiListOneBox *point2 = NULL;
	_XMultiListOneBox *point3 = NULL;
	if(order == 0)
	{
		//�ƶ���ͷ
	//	point0 = tempRow;
	//	tempRow = tempRow->nextRow;
		temp1 = tempRow->nextRow;
		point1 = temp1->nextRow;

	//	point0->nextRow = temp1;
		m_tableRow = temp1;
		temp1->order = tempRow->order;
		temp1->position = tempRow->position;
		temp1->text.setPosition(temp1->position.x,temp1->position.y);

		temp1->nextRow = tempRow;
		temp1->nextRow->order = temp1->order + 1;
		temp1->nextRow->position.set(temp1->position.x + temp1->pixSize.x * m_size.x,m_position.y + m_mouseRect.top * m_size.y);			
		temp1->nextRow->text.setPosition(temp1->nextRow->position.x,temp1->nextRow->position.y);
		if(!m_withoutTex)
		{
			temp1->m_spriteTitle.setPosition(temp1->position);
			temp1->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->pixSize.x * m_size.x - 
				m_mutiListTitleEnd->textureSize.x * temp1->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,temp1->position.y);
			temp1->nextRow->m_spriteTitle.setPosition(temp1->nextRow->position);
			temp1->nextRow->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->nextRow->pixSize.x * m_size.x - 
				m_mutiListTitleEnd->textureSize.x * temp1->nextRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
				temp1->nextRow->position.y);
		}
		temp1->nextRow->nextRow = point1;

		//�ƶ�����
		if(m_tableLineSum > 1)
		{
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j == m_tableRowSum - 1)
					{
						point2 = tempBox;
						tempBox = tempBox->nextBox;
						temp2 = tempBox->nextBox;
						point3 = temp2->nextBox;

						point2->nextBox = temp2;
						temp2->order = tempBox->order;
						temp2->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 2));
						temp2->nextBox = tempBox;
						if(j == m_tableRowSum -1)
						{
							tempRow = m_tableRow;
							temp2->nextBox->order.set(0,temp2->order.y);
							j = 0;
							i++;
						}else
						{//��������ǲ�����ֵ�
							tempRow = tempRow->nextRow;
							temp2->nextBox->order.set(temp2->order.x + 1,temp2->order.y);
							j++;
						}
						temp2->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 2));
						temp2->nextBox->nextBox = point3;
						if(i >= m_tableLineSum - 1)
						{
							break;
						}
					}else
					{
						tempRow = tempRow->nextRow;
						tempBox = tempBox->nextBox;
					}
				}
			}
		}
		//����һ��Ԫ��
	//	point2 = tempBox;
		tempBox = m_tableBox;
		temp2 = tempBox->nextBox;
		point3 = temp2->nextBox;

		m_tableBox = temp2;
		temp2->order = tempBox->order;
		temp2->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
		temp2->nextBox = tempBox;

		tempRow = tempRow->nextRow;
		temp2->nextBox->order.set(temp2->order.x + 1,temp2->order.y);

		temp2->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
		temp2->nextBox->nextBox = point3;
	}else
	{
		//�ƶ���ͷ
		for(i = 0;i < m_tableRowSum;++ i)
		{
			if(i == order - 1)
			{
				point0 = tempRow;
				tempRow = tempRow->nextRow;
				temp1 = tempRow->nextRow;
				point1 = temp1->nextRow;

				point0->nextRow = temp1;
				temp1->order = tempRow->order;
				temp1->position = tempRow->position;
				temp1->text.setPosition(temp1->position.x,temp1->position.y);
				temp1->nextRow = tempRow;
				temp1->nextRow->order = temp1->order + 1;
				temp1->nextRow->position.set(temp1->position.x + temp1->pixSize.x * m_size.x,m_position.y + m_mouseRect.top * m_size.y);			
				temp1->nextRow->text.setPosition(temp1->nextRow->position.x,temp1->nextRow->position.y);
				if(!m_withoutTex)
				{
					temp1->m_spriteTitle.setPosition(temp1->position);
					temp1->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->pixSize.x * m_size.x - 
						m_mutiListTitleEnd->textureSize.x * temp1->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,temp1->position.y);
					temp1->nextRow->m_spriteTitle.setPosition(temp1->nextRow->position);
					temp1->nextRow->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->nextRow->pixSize.x * m_size.x - 
						m_mutiListTitleEnd->textureSize.x * temp1->nextRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
						temp1->nextRow->position.y);
				}
				temp1->nextRow->nextRow = point1;
				break;
			}
			tempRow = tempRow->nextRow;
		}
		//�ƶ�����
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(j == order - 1)
				{
					point2 = tempBox;
					tempBox = tempBox->nextBox;
					temp2 = tempBox->nextBox;
					point3 = temp2->nextBox;

					point2->nextBox = temp2;
					temp2->order = tempBox->order;
					temp2->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
					temp2->nextBox = tempBox;
					if(j == m_tableRowSum -1)
					{//��������ǲ�����ֵ�
						tempRow = tempRow->nextRow;
						temp2->nextBox->order.set(0,temp2->order.y);
						j = 0;
						i ++;
					}else
					{
						tempRow = tempRow->nextRow;
						temp2->nextBox->order.set(temp2->order.x + 1,temp2->order.y);
						j++;
					}
					temp2->nextBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
					temp2->nextBox->nextBox = point3;
					if(i >= m_tableLineSum - 1)
					{
						break;
					}
				}else
				{
					tempRow = tempRow->nextRow;
					tempBox = tempBox->nextBox;
				}
			}
		}
	}
	updateShowChange();
	updateSliderState();
	return XTrue;
}
void _XMultiList::setPosition(float x,float y)
{
	if(!m_isInited) return;
	m_position.set(x,y);
	if(!m_withoutTex) m_spriteBackGround.setPosition(m_position);
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	//�ı��������λ��
	if(m_tableRowSum > 0)
	{
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_size.x,
			m_position.y + m_mouseRect.top * m_size.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		if(!m_withoutTex)
		{
			m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
			m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
		}
		_XMultiListOneRow *tempRow = m_tableRow;
		int i,j;
		for(i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_size.x,
				m_position.y + m_mouseRect.top * m_size.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			if(!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + tempRow->nextRow->pixSize.x * m_size.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
			}
			tempRow = tempRow->nextRow;
		}
		//�ı�����λ��
		if(m_tableLineSum > 0)
		{
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_nowTextHeight * 1);

			_XMultiListOneBox *tempBox = m_tableBox;
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j == 0 && i == 0) 
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	if(m_haveSelect) updateSelectLine();
	updateShowChange();	//������������������ƶ���ʱ���bug
	//updateSliderState();
}
void _XMultiList::setSize(float x,float y)	//���óߴ�
{
	if(!m_isInited) return;
	if(x <= 0 && y <= 0) return;
	m_size.set(x,y);
	if(!m_withoutTex)
	{
		m_spriteBackGround.setPosition(m_position);
		m_spriteBackGround.setSize(m_size);
		m_spriteMove.setSize(m_size);
	}
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_verticalSlider.setSize(m_size);
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	m_horizontalSlider.setSize(m_size);
	m_caption.setSize(m_size * m_fontSize);
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_size.x;
	if(!m_withoutTex)
	{
		m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_size.y;
		m_spriteSelect.setSize((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_nowTextHeight / m_mutiListSelect->textureSize.y);
	}else
	{
		m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - DEFAULT_TITLE_HEIGHT) * m_size.y;
	}
	//�ı��������λ��
	if(m_tableRowSum > 0)
	{
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_size.x,
			m_position.y + m_mouseRect.top * m_size.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		m_tableRow->text.setSize(m_size * m_fontSize);
		if(!m_withoutTex)
		{
			m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
			m_tableRow->m_spriteTitle.setSize(m_tableRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
			m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
			m_tableRow->m_spriteTitleEnd.setSize(m_tableRow->pixSize.y * m_size.y/ m_mutiListTitle->textureSize.y,
				m_tableRow->pixSize.y * m_size.y/m_mutiListTitle->textureSize.y);
		}
		_XMultiListOneRow *tempRow = m_tableRow;

		int i,j;
		for(i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_size.x,
				m_position.y + m_mouseRect.top * m_size.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			tempRow->nextRow->text.setSize(m_size * m_fontSize);
			if(!m_withoutTex)
			{
				tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
				tempRow->nextRow->m_spriteTitle.setSize(tempRow->nextRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
				tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
				tempRow->nextRow->m_spriteTitleEnd.setSize(tempRow->nextRow->pixSize.y * m_size.y/ m_mutiListTitle->textureSize.y,
					tempRow->nextRow->pixSize.y * m_size.y/m_mutiListTitle->textureSize.y);
			}
			tempRow = tempRow->nextRow;
		}
		//�ı�����λ��
		if(m_tableLineSum > 0)
		{
			m_tableBox->text.setPosition(m_tableRow->position.x,m_tableRow->position.y + m_nowTextHeight * 1);
			m_tableBox->text.setSize(m_size * m_fontSize);

			_XMultiListOneBox *tempBox = m_tableBox;
			for(i = 0;i < m_tableLineSum;++ i)
			{
				tempRow = m_tableRow;
				for(j = 0;j < m_tableRowSum;++ j)
				{
					if(j == 0 && i == 0) 
					{
						tempBox = tempBox->nextBox;
						tempRow = tempRow->nextRow;
						continue;
					}
					tempBox->text.setPosition(tempRow->position.x,tempRow->position.y + m_nowTextHeight * (i + 1));
					tempBox->text.setSize(m_size * m_fontSize);
					tempBox = tempBox->nextBox;
					tempRow = tempRow->nextRow;
				}
			}
		}
	}
	if(m_haveSelect) updateSelectLine();
	updateShowChange();
	updateSliderState();
}
_XBool _XMultiList::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
_XVector2 _XMultiList::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(!m_isInited) return ret;
	if(order == 0) ret.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y); else
	if(order == 1) ret.set(m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.top * m_size.y); else
	if(order == 2) ret.set(m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y); else
	if(order == 3) ret.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y); 

	return ret;
}