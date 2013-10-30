//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XMultiListBasic.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

void funMutiListBasicValueChangeV(void *pClass,int objectID)
{
	_XMultiListBasic *pPar = (_XMultiListBasic *)pClass;
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

void funMutiListBasicValueChangeMoveV(void *pClass,int objectID)
{
	_XMultiListBasic *pPar = (_XMultiListBasic *)pClass;
	int temp = pPar->m_verticalSlider.getNowValue();
	if(temp != pPar->m_showStartLine)
	{
		//��Ҫ���´�ֱƬѡ�����
		pPar->m_showStartLine = temp;
		pPar->updateSelectLine();
		pPar->updateShowChange();
	}
}

void funMutiListBasicValueChangeH(void *pClass,int objectID)
{
	_XMultiListBasic *pPar = (_XMultiListBasic *)pClass;
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

void funMutiListBasicValueChangeMoveH(void *pClass,int objectID)
{
	_XMultiListBasic *pPar = (_XMultiListBasic *)pClass;
	int temp = pPar->m_horizontalSlider.getNowValue();
	if(temp != pPar->m_showStartRow)
	{
		pPar->m_showStartRow = temp;
		pPar->updateShowChange();
	}
}

void funMutiListBasicStateChange(void *pClass,int objectID)
{
	_XMultiListBasic *pPar = (_XMultiListBasic *)pClass;
	//��ֵ������ȥ
	for(int i = 0;i < min(pPar->m_canShowLineSum,pPar->m_tableLineSum - pPar->m_showStartLine);++ i)
	{
		if(pPar->m_check0[i].getState()) pPar->m_check0State[i + pPar->m_showStartLine] = true;
		else pPar->m_check0State[i + pPar->m_showStartLine] = false;
		if(pPar->m_check1[i].getState()) pPar->m_check1State[i + pPar->m_showStartLine] = true;
		else pPar->m_check1State[i + pPar->m_showStartLine] = false;
	}
	if((pPar->m_funCheckStateChange) != NULL) (* pPar->m_funCheckStateChange)(pPar->m_pClass,pPar->m_objectID);
}

_XMultiListBasic::_XMultiListBasic()
:m_isInited(0)					//�Ƿ��ʼ��
,m_mutiListNormal(NULL)			//�����б����ͨ״̬
,m_mutiListDisable(NULL)		//�����б����Ч״̬
,m_mutiListSelect(NULL)			//�����б��б�ѡ���еı���
,m_mutiListMove(NULL)			//�ƶ�����ʱ�Ķ����ǩ
,m_mutiListTitle(NULL)			//�����б�ı��ⱳ��
,m_mutiListTitleEnd(NULL)		//�����б�ı���ָ���
,m_needShowVSlider(0)			//�Ƿ���Ҫ��ʾ��ֱ������
,m_needShowHSlider(0)			//�Ƿ���Ҫ��ʾˮƽ������
,m_tableRowSum(0)				//����е�����
,m_tableRow(NULL)				//�е�����
,m_tableLineSum(0)				//����е�����
,m_tableBox(NULL)				//����Ԫ�ص�����
,m_mouseLeftButtonDown(0)
,m_mouseMoveDown(0)
,m_haveSelectFlag(NULL)
,m_check0(NULL)
,m_check1(NULL)
,m_check0State(NULL)
,m_check1State(NULL)
,m_funSelectChange(NULL)
,m_funCheckStateChange(NULL)
{
}

_XMultiListBasic::~_XMultiListBasic()
{
	release();
}

void _XMultiListBasic::releaseTempMemory()
{
	XDELETE_ARRAY(m_check0);
	XDELETE_ARRAY(m_check1);
	XDELETE_ARRAY(m_check0State);
	XDELETE_ARRAY(m_check1State);
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
		//ɾ�����б���е�Ԫ��
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
	m_tableRowSum = 0;
	m_tableLineSum = 0;
	XDELETE_ARRAY(m_haveSelectFlag);
}

void _XMultiListBasic::release()
{
	if(m_isInited == 0) return;
	releaseTempMemory();

	_XControlManager::GetInstance().decreaseAObject(this);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	m_isInited = 0;
}

int _XMultiListBasic::init(const _XVector2& position,		//�ռ����ڵ�λ��
		const _XRect& Area,					//�ؼ���ʵ����ʾ����
		const _XMultiListTexture &tex,		//�ؼ�����ͼ
		const _XCheckTexture &checktex0,		//��ѡ�����ͼ
		const _XCheckTexture &checktex1,		//��ѡ�����ͼ
		const _XFontUnicode &font,			//�ؼ���ʹ�õ�����
		float strSize,						//��������Ŵ�С
		int rowSum,					//�ؼ��е�����
		int lineSum,				//�ؼ��е�����
		//const _XMouseRightButtonMenu& mouseMenu,	//�ؼ���ʹ�õ��Ҽ��˵�(Ŀǰ��Ч)
		const _XSlider &vSlider,	//��ֱ������
		const _XSlider &hSlider)	//ˮƽ������
{
	if(m_isInited != 0) return 0;	//�ظ���ʼ��
	//�����ǲ������
	if(Area.getWidth() <= 0 || Area.getHeight() <= 0) return 0;	//���뷶Χ����Ϊ��
	if(tex.mutiListDisable == NULL || tex.mutiListMove == NULL || tex.mutiListNormal == NULL
		|| tex.mutiListSelect == NULL || tex.mutiListTitle == NULL || tex.mutiListTitleEnd == NULL)
	{//��Ҫ����ͼ������
		return 0;
	}
	if(rowSum < 0 || lineSum < 0 || (rowSum == 0 && lineSum > 0))
	{//����Ϊ����
		return 0;
	}
	//���濪ʼ��ֵ
	m_position = position;
	m_mouseRect = Area;

	m_mutiListNormal = tex.mutiListNormal;			//�����б����ͨ״̬
	m_mutiListDisable = tex.mutiListDisable;			//�����б����Ч״̬
	m_mutiListSelect = tex.mutiListSelect;			//�����б��б�ѡ���еı���
	m_mutiListMove = tex.mutiListMove;			//�ƶ�����ʱ�Ķ����ǩ
	m_mutiListTitle = tex.mutiListTitle;			//�����б�ı��ⱳ��
	m_mutiListTitleEnd = tex.mutiListTitleEnd;		//�����б�ı���ָ���

	m_size.set(1.0f,1.0f);
	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(_FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(_FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
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

	m_haveSelect = 0;		//�Ƿ���ѡ��
	m_selectLineOrder = -1;	//ѡ�������һ��
	m_showStartLine = 0;	//��ʾ���Ǵӵڼ��п�ʼ��
	m_showStartRow = 0;		//��ʾ�ӵڼ��п�ʼ��
	m_needShowMove = 0;		//�Ƿ���Ҫ��ʾ�ƶ�������

	m_needShowVSlider = 0;			//�Ƿ���Ҫ��ʾ��ֱ������
	m_verticalSlider.setACopy(vSlider);		//��ֱ������
	_XControlManager::GetInstance().decreaseAObject(&m_verticalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_verticalSlider.setSize(m_size);
	m_verticalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiListBasicValueChangeV,funMutiListBasicValueChangeMoveV,this);

	m_needShowHSlider = 0;			//�Ƿ���Ҫ��ʾˮƽ������
	m_horizontalSlider.setACopy(hSlider);	//ˮƽ������
	_XControlManager::GetInstance().decreaseAObject(&m_horizontalSlider);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	m_horizontalSlider.setSize(m_size);
	m_horizontalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiListBasicValueChangeH,funMutiListBasicValueChangeMoveH,this);

	if(rowSum > MUTILIST_MAX_ROW_SUM) return 0;
	m_tableRowSum = rowSum;		//����е�����
	//Ϊ�е���������ڴ�ռ�
	if(m_tableRowSum == 0)
	{
		m_tableRow = NULL;
	}else
	{
		try
		{		
			m_tableRow = new _XMultiListOneRow;
			if(m_tableRow == NULL) 
			{
				releaseTempMemory();
				return 0;
			}
		}catch(...)
		{
			releaseTempMemory();
			return 0;
		}		
		m_tableRow->isEnable = 1;

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
			try
			{
				tempRow->nextRow = new _XMultiListOneRow;
				if(tempRow->nextRow == NULL)
				{
					releaseTempMemory();
					return 0;
				}
			}catch(...)
			{
				releaseTempMemory();
				return 0;
			}
			tempRow->nextRow->isEnable = 1;
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
	m_tableLineSum = lineSum;		//����е�����
	if(m_tableLineSum > 0)
	{
		try
		{
			m_haveSelectFlag = new bool[m_tableLineSum];
			m_check0State = new bool[m_tableLineSum];
			m_check1State = new bool[m_tableLineSum];

			if(m_haveSelectFlag == NULL || m_check0State == NULL || m_check1State == NULL) 
			{
				releaseTempMemory();
				return 0;
			}
		}catch(...)
		{
			releaseTempMemory();
			return 0;
		}
		for(int i = 0;i < m_tableLineSum;++ i)
		{
			m_haveSelectFlag[i] = false;
			m_check0State[i] = false;
			m_check1State[i] = false;
		}
	}else
	{
		m_haveSelectFlag = NULL;
		m_check0State = NULL;
		m_check1State = NULL;
	}
	//����ؼ�������ʾ������
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_size.x;
	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_size.y;
	m_canShowLineSum = m_showPixHight / m_nowTextHeight;
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);	//��ǰ�������Ӧ��Χ
	m_spriteSelect.setSize((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_nowTextHeight / m_mutiListSelect->textureSize.y);
	try
	{
		m_check0 = new _XCheck[m_canShowLineSum];
		m_check1 = new _XCheck[m_canShowLineSum];
		if(m_check0 == NULL || m_check1 == NULL) 
		{
			releaseTempMemory();
			return 0;
		}
	}catch(...)
	{
		releaseTempMemory();
		return 0;
	}
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		if(m_check0[i].init(_XVector2(m_position.x + 4.0f * m_size.x,m_position.y + (36.0f + m_nowTextHeight * i) * m_size.y),_XRect(4.0f,4.0f,28.0f,28.0f),checktex0,
			" ",m_caption,1.0f,_XVector2(0.0f,0.0f)) == 0) 
		{
			releaseTempMemory();
			return 0;
		}
		m_check0[i].setSize(m_size);
		m_check0[i].setWithCaption(0);
		if(m_check1[i].init(_XVector2(m_position.x + 4.0f * m_size.x + 32.0f * m_size.x,m_position.y + (36.0f + m_nowTextHeight * i) * m_size.y),_XRect(4.0f,4.0f,28.0f,28.0f),checktex1,
			" ",m_caption,1.0f,_XVector2(0.0f,0.0f)) == 0) 
		{
			releaseTempMemory();
			return 0;
		}
		m_check1[i].setSize(m_size);
		m_check1[i].setWithCaption(0);
		if(m_tableLineSum < m_canShowLineSum && i >= m_tableLineSum)
		{
			m_check0[i].disVisiable();
			m_check1[i].disVisiable();
		}else
		{
			m_check0[i].setVisiable();
			m_check1[i].setVisiable();
		}
		_XControlManager::GetInstance().decreaseAObject(&(m_check0[i]));
		_XControlManager::GetInstance().decreaseAObject(&(m_check1[i]));
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&m_check0[i]);
		_XObjectManager::GetInstance().decreaseAObject(&m_check1[i]);
#endif

		m_check0[i].setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiListBasicStateChange,this);
		m_check1[i].setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiListBasicStateChange,this);
	}
	//ΪԪ�ط����ڴ�ռ�
	if(m_tableLineSum == 0)
	{
		m_tableBox = NULL;
	}else
	{
		try
		{
			m_tableBox = new _XMultiListOneBox;
			if(m_tableBox == NULL) 
			{
				releaseTempMemory();
				return 0;
			}
		}catch(...)
		{
			releaseTempMemory();
			return 0;
		}
		m_tableBox->isEnable = 1;
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
				try
				{
					tempBox->nextBox = new _XMultiListOneBox;
					if(tempBox->nextBox == NULL) 
					{
						releaseTempMemory();
						return 0;
					}
				}catch(...)
				{
					releaseTempMemory();
					return 0;
				}
				tempBox = tempBox->nextBox;
				tempBox->isEnable = 1;
				
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
	m_mouseLeftButtonDown = 0;
	m_mouseMoveDown = 0;

	m_isVisiable = 1;
	m_isEnable = 1;
	m_isActive = 1;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MUTILIST);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = 1;
	updateShowChange();
	updateSliderState();
	return 1;
}

void _XMultiListBasic::updateSliderState()				//���ݱ�����������»�������״̬
{
	if(m_isInited == 0) return;		//���û�г�ʼ��ֱ���˳�
	//��ֱ������
	if(m_canShowLineSum < m_tableLineSum)
	{//�����ֱ������ʾ��Χ������Ҫ������ֱ��ʾ����������ѡ��Χ
		m_needShowVSlider = 1;			
		m_verticalSlider.setRange(m_tableLineSum - m_canShowLineSum,0.0f);		//��ֱ������
	}else
	{
		m_showStartLine = 0;
		m_needShowVSlider = 0;			
		m_verticalSlider.setRange(m_tableLineSum - m_canShowLineSum,0.0f);		//��ֱ������
	}
	//ˮƽ������
	if(m_tableRowSum <= 0)
	{//���û��������Ҫ��ʾˮƽ������
		m_needShowHSlider = 0;
	}else
	{
		int width = 0;
		float rowWidth[MUTILIST_MAX_ROW_SUM] ={0};
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
		{//����ˮƽ��ʾ�������Ҫ��ʾˮƽ������
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
			if(width > m_showPixWidth && i < m_tableRowSum && flag == 0)
			{
				flag = i;
				//break;
			}
		}
		if(flag != 0)
		{//����ˮƽ��ʾ�������Ҫ��ʾˮƽ������
			m_needShowHSlider = 1;
			//m_horizontalSlider.setRange(m_tableRowSum - i,0);
			m_horizontalSlider.setRange(width - m_showPixWidth + MUTILIST_TITLE_EXPAND_LENGTH * m_size.x,0);	//ע�������56��������չ��
		}else
		{
		//	if(m_needShowHSlider != 0)
		//	{//��Խ�絽��Խ�磬��Ҫ���³�ʼ��ƫ����
		//		m_rowDx = 0;
		//	}
			m_needShowHSlider = 0;
			m_showStartRow = 0;
			m_horizontalSlider.setNowValue(0);
		}
	}
}

void _XMultiListBasic::setCallbackFun(void (* funDataChange)(void *,int),void (* funCheckStateChange)(void *,int),void *pClass)
{
	if(m_isInited == 0) return;
	m_funCheckStateChange = funCheckStateChange;
	m_funSelectChange = funDataChange;
	m_pClass = pClass;
}

void _XMultiListBasic::clearAllSelect()
{
	if(m_isInited == 0) return;
	for(int i = 0;i < m_tableLineSum;++ i)
	{
		m_haveSelectFlag[i] = false;
	}
}

void _XMultiListBasic::setTileStr(const char *str,int order)
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
	char *tempTitle = NULL;
	try
	{
		tempTitle = new char[strlen(str) + 1];
		if(tempTitle == NULL) return;
	}catch(...)
	{
		return;
	}
	strcpy(tempTitle,str);
	XDELETE_ARRAY(tempRow->title);
	tempRow->title = tempTitle;
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

void _XMultiListBasic::setBoxStr(const char *str,int line,int row)	//����ĳһ����Ԫ�������
{
	//��������������ж�
	if(str == NULL) return;
	if(line < 0 || line >= m_tableLineSum) return;
	if(row < 0 || row >= m_tableRowSum) return;
	//Ѱ�ҵ�ָ���ı��
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
	char * tempStr = NULL;
	try
	{
		tempStr = new char[strlen(str) + 1];
		if(tempStr == NULL) return;
	}catch(...)
	{
		return;
	}
	//��ֵ
	strcpy(tempStr,str);
	XDELETE_ARRAY(tempBox->string);
	tempBox->string = tempStr;
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

void _XMultiListBasic::updateShowChange()				//�����б��ı仯��Ϣ�����б���еı�����ʾ���
{
	if(m_isInited == 0) return;		//���û�г�ʼ��ֱ���˳�
	if(m_tableRowSum <= 0) return;	//���û�б��ֱ�ӷ���
	//�������ʾ
	_XMultiListOneRow *tempRow = m_tableRow;
	float nowWidthPix = 0.0f;
	int row = 0;
	int showMaxRow = 0;
//	int showDx = m_showStartRow; //��ʾ�����ݱ���ˮƽƫ��	m_showStartRow(ˮƽƫ����)
	while(1)
	{
		if(nowWidthPix + tempRow->pixSize.x * m_size.x > m_showStartRow && nowWidthPix - m_showStartRow <= m_showPixWidth)
		{
			if(nowWidthPix < m_showStartRow && nowWidthPix + tempRow->pixSize.x * m_size.x - m_showStartRow <= m_showPixWidth)
			{//��ʾǰ��һ����
				tempRow->isShow = 1;
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				tempRow->m_spriteTitle.setPosition(tempRow->position.x - nowWidthPix,tempRow->position.y);
				tempRow->m_spriteTitle.setSize((float)(tempRow->pixSize.x * m_size.x - (m_showStartRow - nowWidthPix)) / m_mutiListTitle->textureSize.x,m_size.y);
				tempRow->needChip = 1;					//�Ƿ���Ҫ�и�
				tempRow->left = (m_showStartRow - nowWidthPix) / m_size.x;						//��ߵ��и�λ��
				tempRow->right = tempRow->pixSize.x;					//�ұߵ��и�λ��
				tempRow->text.setClipRect(tempRow->left,0.0f,tempRow->right,tempRow->text.getTextSize().y);
				tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_size.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				nowWidthPix += tempRow->pixSize.x * m_size.x;
			}else
			if(nowWidthPix + tempRow->pixSize.x * m_size.x - m_showStartRow <= m_showPixWidth && nowWidthPix >= m_showStartRow)
			{//����������ʾ
				tempRow->isShow = 1;
				nowWidthPix += tempRow->pixSize.x * m_size.x;
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				tempRow->m_spriteTitle.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				tempRow->m_spriteTitle.setSize(tempRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
				tempRow->text.disClip();
				tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_size.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				tempRow->needChip = 0;					//�Ƿ���Ҫ�и�
				tempRow->left = 0.0f;						//��ߵ��и�λ��
				tempRow->right = 0.0f;					//�ұߵ��и�λ��
			}else
			if(nowWidthPix >= m_showStartRow && nowWidthPix + tempRow->pixSize.x * m_size.x - m_showStartRow > m_showPixWidth)
			{//��ʾ����һ����
				tempRow->isShow = 1;
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				tempRow->m_spriteTitle.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				tempRow->m_spriteTitle.setSize((float)(m_showPixWidth - (nowWidthPix - m_showStartRow)) / m_mutiListTitle->textureSize.x,m_size.y);
				tempRow->needChip = 2;					//�Ƿ���Ҫ�и�
				tempRow->left = 0.0f;						//��ߵ��и�λ��
				tempRow->right = (m_showPixWidth - (nowWidthPix - m_showStartRow)) / m_size.x;					//�ұߵ��и�λ��
				tempRow->text.setClipRect(0.0f,0.0f,tempRow->right,tempRow->text.getTextSize().y);
				if(tempRow->pixSize.x * m_size.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y > 
					m_showPixWidth - (nowWidthPix - m_showStartRow))
				{
					tempRow->isShow = 2;
				}
				tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_size.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
				tempRow->text.setPosition(tempRow->position.x - m_showStartRow,tempRow->position.y);
				nowWidthPix += tempRow->pixSize.x * m_size.x;
			}else
			if(nowWidthPix < m_showStartRow && nowWidthPix + tempRow->pixSize.x * m_size.x - m_showStartRow > m_showPixWidth)
			{//������Ҫ��ͷȥβ��Ŀǰû�д���
				tempRow->isShow = 1;
				if(row > showMaxRow) showMaxRow = row;
				//�ı���Ҫ��ʾ�ı����λ��
				tempRow->m_spriteTitle.setPosition(tempRow->position.x - nowWidthPix,tempRow->position.y);
				tempRow->m_spriteTitle.setSize((float)(m_showPixWidth) / m_mutiListTitle->textureSize.x,m_size.y);
				tempRow->needChip = 2;					//�Ƿ���Ҫ�и�
				tempRow->left = (m_showStartRow - nowWidthPix) / m_size.x;						//��ߵ��и�λ��
				tempRow->right = (m_showPixWidth - (nowWidthPix - m_showStartRow)) / m_size.x;					//�ұߵ��и�λ��
				tempRow->text.setClipRect(tempRow->left,0.0f,tempRow->right,tempRow->text.getTextSize().y);
				if(tempRow->pixSize.x * m_size.x - m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y > 
					m_showPixWidth - (nowWidthPix - m_showStartRow))
				{
					tempRow->isShow = 2;
				}
				tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_size.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
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
	//�����޸����״̬������ʾ
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		if(m_tableLineSum < m_canShowLineSum && i >= m_tableLineSum)
		{
			m_check0[i].disVisiable();
			m_check1[i].disVisiable();
		}else
		{
			m_check0[i].setVisiable();
			m_check1[i].setVisiable();
		}
	}
	//�����б������ȫ������
	if(m_tableLineSum <= 0) return;	//���û����ֱ�ӷ���
	_XMultiListOneBox *tempBox = m_tableBox;
	tempRow = m_tableRow;
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
				if(tempRow->needChip != 0)
				{
					tempBox->text.setClipRect(tempRow->left,0.0f,tempRow->right,tempBox->text.getTextSize().y);
				}else
				{
					tempBox->text.disClip();
				}
			}else
			{
				tempBox->isShow = 0;
			}
		}else
		{
			tempBox->isShow = 0;
		}
		if(tempBox->nextBox == NULL)
		{
			break;
		}else
		{
			tempBox = tempBox->nextBox;
		}
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
	//��������б���ѡ��״̬����Ϣ
	for(int i = m_showStartLine;i < min(m_showStartLine + m_canShowLineSum,m_tableLineSum);++ i)
	{
		if(m_check0State[i]) m_check0[i - m_showStartLine].setState(true);
		else m_check0[i - m_showStartLine].setState(false);
		if(m_check1State[i]) m_check1[i - m_showStartLine].setState(true);
		else m_check1[i - m_showStartLine].setState(false);
	}
}
//ע�����ﴫ��ĳߴ�Ϊ����֮ǰ�ĳߴ磬Ҳ�����ǿռ����ű���Ϊ1��ʱ��óߴ�
int _XMultiListBasic::setRowWidth(int temp,int order)	//��������һ�еĿ��
{
	if(temp < MUTILIST_MIN_WIDTH || temp > m_showPixWidth / m_size.x) return 0;
	if(order < 0 || order >= m_tableRowSum) return 0;
	//�ҵ���Ӧ��һ��
	_XMultiListOneRow *tempRow = m_tableRow;
	for(int i = 0;i < order;++ i)
	{
		tempRow = tempRow->nextRow;
	}
	
	tempRow->pixSize.set(temp,tempRow->pixSize.y);	//�ı䵱ǰ�е��п�
	tempRow->stringShowWidth = tempRow->pixSize.x * m_size.x / m_nowTextWidth;	//�ı䵱ǰ�е���ʾ�ַ����
	tempRow->m_spriteTitleEnd.setPosition(tempRow->position.x + tempRow->pixSize.x * m_size.x - 
		m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
		m_tableRow->position.y);
	tempRow->text.setPosition(tempRow->position.x,tempRow->position.y);
	tempRow->m_spriteTitle.setSize(tempRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
	//�ı䵱ǰ�е���ʾ�ַ�
	char * str;
	try
	{
		str = new char[strlen(tempRow->title) + 1];
		if(str == NULL) 
		{
			return 0;
		}
	}catch(...)
	{
		return 0;
	}
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
			tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + tempRow->nextRow->pixSize.x * m_size.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
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
				try
				{
					str = new char[strlen(tempBox->string) + 1];
					if(str == NULL) return 0;
				}catch(...)
				{
					return 0;
				}
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
	return 1;
}

void _XMultiListBasic::draw()	//��溯��
{
	if(m_isInited == 0) return;	//���û�г�ʼ��ֱ���˳�
	if(m_isVisiable == 0) return;	//������ɼ�ֱ���˳�

	if(m_isEnable == 0)
	{
		m_spriteBackGround.draw(m_mutiListDisable);
	}else
	{
		m_spriteBackGround.draw(m_mutiListNormal);
	}
	//������ʾ�б������
	//��ʾ����
	if(m_tableRowSum > 0)
	{
		_XMultiListOneRow *tempRow = m_tableRow;
		while(1)
		{
			if(tempRow->isEnable != 0 && tempRow->isShow != 0)
			{
				tempRow->m_spriteTitle.draw(m_mutiListTitle);	//��ʾ���ⱳ��
				tempRow->text.draw();//��ʾ��������
				if(tempRow->isShow != 0 && tempRow->needChip != 2)
				{
					tempRow->m_spriteTitleEnd.draw(m_mutiListTitleEnd);	//��ʾ���������
				}
			}
			if(tempRow->nextRow == NULL)
			{
				break;
			}else
			{
				tempRow = tempRow->nextRow;
			}
		}
		if(m_tableLineSum > 0)
		{
			//������ʾѡ��Ľ��
			for(int i = 0;i < min(m_canShowLineSum,m_tableLineSum - m_showStartLine);++ i)
			{
				if(m_haveSelectFlag[m_showStartLine + i])
				{//��ʾѡ����
					m_spriteSelect.setPosition(m_position.x + m_mouseRect.left * m_size.x,
						m_position.y + (m_mouseRect.top + m_mutiListTitle->textureSize.y) * m_size.y + 
						i * m_nowTextHeight);

					m_spriteSelect.draw(m_mutiListSelect);
				}
			}
			if(m_haveSelect != 0)
			{
				if(m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
				{
					drawRect(m_selectRect,1.0f,0.0f,0.0f,0.0f,1.0f);
				}
			}
			_XMultiListOneBox *tempBox = m_tableBox;
			while(1)
			{
				if(tempBox->isEnable != 0 && tempBox->isShow != 0)
				{
					tempBox->text.draw();//��ʾ��������
				}
				if(tempBox->nextBox == NULL)
				{
					break;
				}else
				{
					tempBox = tempBox->nextBox;
				}
			}
		}
	}
	if(m_needShowMove != 0)
	{
		m_spriteMove.draw(m_mutiListMove);
	}
	if(m_needShowVSlider != 0)
	{
		m_verticalSlider.draw();
	}
	if(m_needShowHSlider != 0)
	{
		m_horizontalSlider.draw();
	}
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_check0[i].draw();
		m_check1[i].draw();
	}
}

int _XMultiListBasic::canGetFocus(float x,float y)
{
	if(m_isInited == 0) return 0;	//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 0;		//û�м���Ŀؼ������տ���
	if(m_isVisiable == 0) return 0;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}

int _XMultiListBasic::canLostFocus(float x,float y)
{
	return 1;
}

int _XMultiListBasic::mouseProc(float x,float y,_XMouseState mouseState)					//������궯������Ӧ����
{
	if(m_isInited == 0) return 0;		//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 0;		//û�м���Ŀؼ������տ���
	if(m_isVisiable == 0) return 0;		//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�

	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_check0[i].mouseProc(x,y,mouseState);
		m_check1[i].mouseProc(x,y,mouseState);
	}
	if(m_needShowVSlider != 0)
	{
		m_verticalSlider.mouseProc(x,y,mouseState);
	}
	if(m_needShowHSlider != 0)
	{
		m_horizontalSlider.mouseProc(x,y,mouseState);
	}
	//�����ж�����޸ı���������ж�
	if(m_tableRowSum > 0)
	{
		if(mouseState == _XMOUSE_LEFT_BUTTON_DOWN)
		{//�������
			_XRect tempRect;
			_XMultiListOneRow *tempRow = m_tableRow;
			int order = 0;
		//	int showDx = m_showStartRow;	//�����Ҫ����������ڿ�ʼ��ʾ����һ�е�ƫ��������Ϊ�е�λ�����ȶ��ڵ�һ�еľ���λ�ã�������Ҫ����ʵ�ʵ���ʾ������Ե���ʾλ��
			while(1)
			{
				if(tempRow->isShow != 0)
				{//��ʾ�����Ĳ����޸�
					tempRect.set(tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_size.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
						tempRow->position.y,
						tempRow->position.x - m_showStartRow + tempRow->pixSize.x * m_size.x,
						tempRow->position.y + tempRow->pixSize.y * m_size.y);
					if(tempRect.isInRect(x,y))
					{//����϶���Ч
						m_mouseLeftButtonDown = 1;
						m_startX = x;
					//	m_rowDx = m_showStartRow;
						m_changeRowOrder = order;
						m_changeRow = tempRow;
						//���ÿ�ʼ��ʾ�ƶ�ͼ��
						m_needShowMove = 1;
						m_spriteMove.setPosition(m_changeRow->position.x - m_showStartRow + m_changeRow->pixSize.x * m_size.x - 
							m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
							m_changeRow->position.y);
						break;
					}
				}
			//	if(order < m_showStartRow)
			//	{
			///		showDx += tempRow->pixSize.x * m_size.x;
			//	}
				if(tempRow->nextRow == NULL)
				{
					break;
				}else
				{
					tempRow = tempRow->nextRow;
				}
				order ++;
			}
		}
		if(mouseState == _XMOUSE_MOVE && m_mouseLeftButtonDown != 0)
		{//�϶���Ч
			//�����϶��Ľ��
			int dx = x - m_startX;
			m_startX = x;
			if(m_changeRow->position.x - m_showStartRow - m_tableRow->position.x + m_changeRow->pixSize.x * m_size.x + dx > m_showPixWidth)
			{//����ƶ�������ȡ���ƶ���������չ�����
				setRowWidth((m_showPixWidth - m_changeRow->position.x + m_showStartRow + m_tableRow->position.x) / m_size.x,m_changeRowOrder);
				m_needShowMove = 0;			//ȡ���ƶ�ͼ�����ʾ
				m_mouseLeftButtonDown = 0;
			}else
			if(m_changeRow->pixSize.x *m_size.x + dx < MUTILIST_MIN_WIDTH * m_size.x)
			{//���С����Сֵ����ʹ����Сֵ����ȡ���ƶ�
				setRowWidth(MUTILIST_MIN_WIDTH,m_changeRowOrder);
				m_needShowMove = 0;			//ȡ���ƶ�ͼ�����ʾ
				m_mouseLeftButtonDown = 0;
			}else
			{
				if(setRowWidth(m_changeRow->pixSize.x + dx / m_size.x,m_changeRowOrder) == 0)
				{//�϶�ʧ����ȥ������
					m_needShowMove = 0;			//ȡ���ƶ�ͼ�����ʾ
					m_mouseLeftButtonDown = 0;
				}
				m_spriteMove.setPosition(m_changeRow->position.x - m_showStartRow + m_changeRow->pixSize.x * m_size.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
					m_changeRow->position.y);
			}
		}
	}
	if(mouseState == _XMOUSE_LEFT_BUTTON_UP && m_mouseLeftButtonDown != 0)
	{//���̸����ȥ������
		m_needShowMove = 0;			//ȡ���ƶ�ͼ�����ʾ
		m_mouseLeftButtonDown = 0;
	}
	//�����ж���ѡ
	if(m_tableLineSum > 0)
	{
		if(mouseState == _XMOUSE_LEFT_BUTTON_DOWN)
		{//�������
			//�����ѡ��Χ
			_XRect tempRect;
			tempRect.set(m_position.x + m_mouseRect.left * m_size.x,
				m_position.y + m_mouseRect.top * m_size.x + m_mutiListTitle->textureSize.y * m_size.y,
				m_position.x + m_mouseRect.left * m_size.x + m_showPixWidth ,
				m_position.y + m_mouseRect.top * m_size.x + m_mutiListTitle->textureSize.y * m_size.y + m_showPixHight);
			if(tempRect.isInRect(x,y))
			{//�����ѡ������һ��
				int DLine = (y - tempRect.top) / m_nowTextHeight;
				if(DLine + m_showStartLine < m_tableLineSum)
				{//��ѡ��Ч
					if(m_haveSelect != 0 && m_selectLineOrder == DLine + m_showStartLine)
					{//�ظ�ѡ���ʾȡ��ѡ��
						m_haveSelect = 0;
						m_selectLineOrder = DLine + m_showStartLine;
						if(m_haveSelectFlag[m_selectLineOrder])
						{
							m_haveSelectFlag[m_selectLineOrder] = false;
							if(m_funSelectChange != NULL) (* m_funSelectChange)(m_pClass,m_objectID);
						}
					}else
					{
						m_haveSelect = 1;
						m_selectLineOrder = DLine + m_showStartLine;
						if(!m_haveSelectFlag[m_selectLineOrder])
						{
							m_haveSelectFlag[m_selectLineOrder] = true;
							if(m_funSelectChange != NULL) (* m_funSelectChange)(m_pClass,m_objectID);
						}
					}

					updateSelectLine();
					//�����갴����Ч
					m_mouseMoveDown = 1;
					m_oldLineOrder = m_selectLineOrder;
				}
			}
		}
		if(mouseState == _XMOUSE_MOVE && m_mouseMoveDown != 0)
		{//�϶���Ч
			//�����ѡ��Χ(ע��������չ��20��Ϊ��ʵ���϶�������Χ�Ĵ���)
			_XRect tempRect;
			tempRect.set(m_position.x + m_mouseRect.left * m_size.x,
				m_position.y + m_mouseRect.top * m_size.x + m_mutiListTitle->textureSize.y * m_size.y - 100,
				m_position.x + m_mouseRect.left * m_size.x + m_showPixWidth,
				m_position.y + m_mouseRect.top * m_size.x + m_mutiListTitle->textureSize.y * m_size.y + m_showPixHight + 100);
			if(tempRect.isInRect(x,y))
			{
				int DLine = (y - tempRect.top - 100) / m_nowTextHeight;
				if(DLine < 0 && m_showStartLine > 0)
				{//��������,����һ�񲢽������ʾ�����ƶ�
					if(moveUpLine(m_oldLineOrder) != 0)
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
					if(moveDownLine(m_oldLineOrder) != 0)
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
					if(moveUpLine(m_oldLineOrder) != 0)
					{
						m_oldLineOrder --;
					}
				}else
				if(DLine + m_showStartLine > m_oldLineOrder)
				{//�����ƶ�
					if(moveDownLine(m_oldLineOrder) != 0)
					{
						m_oldLineOrder ++;
					}
				}
			}
		}
	}
	if(mouseState == _XMOUSE_LEFT_BUTTON_UP && m_mouseMoveDown != 0)
	{
		m_mouseMoveDown = 0;
		m_oldLineOrder = m_selectLineOrder;
	}
	return 1;
}

void _XMultiListBasic::updateSelectLine()
{
	if(m_isInited == 0) return;		//���û�г�ʼ��ֱ���˳�
	if(m_haveSelect == 0) return;
	if(m_selectLineOrder >= m_showStartLine && m_selectLineOrder < m_showStartLine + m_canShowLineSum)
	{//����ѡ���λ��
		_XVector2 temp(m_position.x + m_mouseRect.left * m_size.x,
			m_position.y + (m_mouseRect.top + m_mutiListTitle->textureSize.y) * m_size.y + 
			(m_selectLineOrder - m_showStartLine) * m_nowTextHeight);
		m_selectRect.set(temp.x,temp.y,temp.x + m_showPixWidth,temp.y + m_nowTextHeight);
	}
}

//�ļ���������ʽΪXMultiList_ID.dat
int _XMultiListBasic::exportData(const char *fileName)	//���ݵ���
{//���ݵ������ļ���
	if(m_isInited == 0) return 0;		//���û�г�ʼ��ֱ���˳�
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
			return 0;
		}
	}else
	{
		if((fp = fopen(fileName,"wb")) == NULL)
		{
			printf("File open error!\n");
			return 0;
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
	//д������Ϣ
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
	return 1;
}

int _XMultiListBasic::importData(const char *fileName)	//���ݵ���
{//���ļ��н����ݵ���
	if(m_isInited == 0) return 0;		//���û�г�ʼ��ֱ���˳�
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
			return 0;
		}
	}else
	{
		if((fp = fopen(fileName,"rb")) == NULL)
		{
			printf("File open error!\n");
			return 0;
		}
	}
	//��ȡ����Ϣ
	int strLength = 0;
	int tempSum = 0;
	float tempWidth = 0;
	char *str;
	fread(&tempSum,sizeof(int),1,fp);
	if(tempSum != m_tableRowSum)
	{//��񲻷���
		setRowSum(tempSum);
	}
	for(int i = 0;i < m_tableRowSum;++ i)
	{
		fread(&tempWidth,sizeof(float),1,fp);
		setRowWidth(tempWidth,i);
		fread(&(strLength),sizeof(int),1,fp);
		try
		{
			str = new char[strLength + 1];
			if(str == NULL) 
			{
				return 0;
			}
		}catch(...)
		{
			return 0;
		}
		fread(str,strLength + 1,1,fp);
		setTileStr(str,i);
		XDELETE_ARRAY(str);
	}
	//��ȡ�����Ϣ
	fread(&tempSum,sizeof(int),1,fp);
	if(tempSum != m_tableLineSum)
	{//��񲻷���
		setLineSum(tempSum);
	}
	for(int i = 0;i < m_tableLineSum * m_tableRowSum;++ i)
	{
		fread(&(strLength),sizeof(int),1,fp);
		try
		{
			str = new char[strLength + 1];
			if(str == NULL) 
			{
				return 0;
			}
		}catch(...)
		{
			return 0;
		}
		fread(str,strLength + 1,1,fp);
		setBoxStr(str,i / m_tableRowSum,i % m_tableRowSum);
		XDELETE_ARRAY(str);
	}
	fclose(fp);
	return 1;
}

void _XMultiListBasic::initANewRowData(_XMultiListOneRow * upRow,int i)
{
	if(i == 0)
	{//��һ��Ԫ��
		m_tableRow->isEnable = 1;

		m_tableRow->order = 0;
		m_tableRow->text.setACopy(m_caption);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_tableRow->text));
#endif
		m_tableRow->text.setSize(m_size * m_fontSize);
		m_tableRow->pixSize.set(MUTILIST_MIN_WIDTH,m_caption.getTextSize().y * m_caption.getSize().x);
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
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
		m_tableRow->m_spriteTitleEnd.setSize(m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
			m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y);
		m_tableRow->m_spriteTitleEnd.setIsTransformCenter(POINT_LEFT_TOP);

		m_tableRow->stringShowWidth = m_tableRow->pixSize.x * m_size.x / m_nowTextWidth;
		m_tableRow->title = NULL;
		setTileStr("Title",0);	//����Ĭ����ʾ
		m_tableRow->isShow = 1;
	}else
	{
		upRow->nextRow->isEnable = 1;
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

		upRow->nextRow->stringShowWidth = m_tableRow->pixSize.x * m_size.x / m_nowTextWidth;
		upRow->nextRow->title = NULL;
		setTileStr("Title",i);	//����Ĭ����ʾ
		upRow->nextRow->isShow = 1;
	}
}

int _XMultiListBasic::setRowSum(int rowSum)		//��������������������հף�����ɾ��
{
	if(m_isInited == 0) return 0;		//���û�г�ʼ��ֱ���˳�
	if(rowSum < 0) return 0;			//�Ƿ�����ֱ�ӷ�Χʧ��
	if(rowSum == m_tableRowSum) return 1;	//�������û�иı�����ֱ�ӷ��سɹ�
	if(rowSum > MUTILIST_MAX_ROW_SUM) return 0;	//�����е����ֵ�����˳�
	//�������ʵ������޸Ŀؼ��е�����
	if(rowSum > m_tableRowSum)
	{//��ӣ���������ӵĻ��������;��Ϊ�����˳�����Ӱ�쵽�߼���������
		int i,j;
		int oldRowSum = m_tableRowSum;
		m_tableRowSum = rowSum;
		//��ӱ�������
		_XMultiListOneRow *tempRow = m_tableRow;
		_XMultiListOneRow *temp = NULL;
		if(oldRowSum == 0)
		{//ȫ�����
			try
			{
				m_tableRow = new _XMultiListOneRow;
				if(m_tableRow == NULL) return 0;
			}catch(...)
			{
				return 0;
			}
			initANewRowData(NULL,0);

			tempRow = m_tableRow;
			for(int i = 1;i < m_tableRowSum;++ i)
			{
				try
				{
					tempRow->nextRow = new _XMultiListOneRow;
					if(tempRow->nextRow == NULL) return 0;
				}catch(...)
				{
					return 0;
				}
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
					try
					{
						temp->nextRow = new _XMultiListOneRow;
						if(temp->nextRow == NULL) return 0;
					}catch(...)
					{
						return 0;
					}
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
		//��ӱ������
		_XMultiListOneBox *tempBox = m_tableBox;
		_XMultiListOneBox *temp1 = NULL;
		if(oldRowSum == 0)
		{//ȫ�����
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
							try
							{
								temp1->nextBox = new _XMultiListOneBox;
								if(temp1->nextBox == NULL) return 0;
							}catch(...)
							{
								return 0;
							}
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
		//ɾ���������
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
	return 1;
}

void _XMultiListBasic::initANewBoxData(_XMultiListOneBox * nowBox,_XMultiListOneRow * nowRow,int i,int j)
{
	nowBox->isEnable = 1;

	nowBox->isShow = 1;
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
//�������������Ҫ���ý�������ֵ��ѡ�����ֵ
int _XMultiListBasic::setLineSum(int lineSum)	//���������������Ĳ�����հף�����ɾ��
{
	if(m_isInited == 0) return 0;		//���û�г�ʼ��ֱ���˳�
	if(lineSum < 0) return 0;			//�Ƿ�����ֱ�ӷ�Χʧ��
	if(m_tableRowSum <= 0) return 0;	//�Ƿ�����ֱ�ӷ�Χʧ��
	if(lineSum == m_tableLineSum) return 1;	//�������û�иı�����ֱ�ӷ��سɹ�
	//��ѡ���ǽ��и���
	if(lineSum > 0)
	{
		bool *tempFlag = NULL;
		bool *tempFlag1 = NULL;
		bool *tempFlag2 = NULL;
		try
		{
			tempFlag = new bool[lineSum];
			tempFlag1 = new bool[lineSum];
			tempFlag2 = new bool[lineSum];
			if(tempFlag == NULL || tempFlag1 == NULL || tempFlag2 == NULL) return 0;
		}catch(...)
		{
			return 0;
		}
		for(int i = 0;i < min(lineSum,m_tableLineSum);++ i)
		{
			tempFlag[i] = m_haveSelectFlag[i];
			tempFlag1[i] = m_check0State[i];
			tempFlag2[i] = m_check1State[i];
		}
		for(int i = min(lineSum,m_tableLineSum);i < max(lineSum,m_tableLineSum);++ i)
		{
			tempFlag[i] = false;
			tempFlag1[i] = false;
			tempFlag2[i] = false;
		}
		XDELETE_ARRAY(m_haveSelectFlag);
		XDELETE_ARRAY(m_check0State);
		XDELETE_ARRAY(m_check1State);
		m_haveSelectFlag = tempFlag;
		m_check0State = tempFlag1;
		m_check1State = tempFlag2;
	}else
	{
		XDELETE_ARRAY(m_haveSelectFlag);
		XDELETE_ARRAY(m_check0State);
		XDELETE_ARRAY(m_check1State);
		m_haveSelectFlag = NULL;
		m_check0State = NULL;
		m_check1State = NULL;
	}

	int i,j;
	//�������ʵ������޸Ŀؼ��е�����
	if(lineSum > m_tableLineSum)
	{//���
		int oldLine = m_tableLineSum;
		m_tableLineSum = lineSum;
		_XMultiListOneBox *tempBox = m_tableBox;
		_XMultiListOneRow *tempRow;
		if(oldLine == 0)
		{//ȫ�����
			try
			{
				m_tableBox = new _XMultiListOneBox;
				if(m_tableBox == NULL) return 0;
			}catch(...)
			{
				return 0;
			}
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
					try
					{
						tempBox->nextBox = new _XMultiListOneBox;
						if(tempBox->nextBox == NULL) return 0;
					}catch(...)
					{
						return 0;
					}
					tempBox = tempBox->nextBox;
					initANewBoxData(tempBox,tempRow,i,j);

					tempRow = tempRow->nextRow;
					//ע������û�м���ڴ����ʧ��
				}
			}
			tempBox->nextBox = NULL;	//���һ����β
		}else
		{//���
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
						try
						{
							tempBox->nextBox = new _XMultiListOneBox;
							if(tempBox->nextBox == NULL) return 0;
						}catch(...)
						{
							return 0;
						}
						tempBox = tempBox->nextBox;
						tempBox->isEnable = 1;

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
	return 1;
}

int _XMultiListBasic::deleteLine(int order)		//ɾ��ĳһ��
{
	if(m_isInited == 0) return 0;							//���û�г�ʼ��ֱ���˳�
	if(order < 0 || order >= m_tableLineSum) return 0;		//�Ƿ�������
	//��ѡ���ǽ��и���
	if(m_tableLineSum > 1)
	{
		bool *tempFlag = NULL;
		bool *tempFlag1 = NULL;
		bool *tempFlag2 = NULL;
		try
		{
			tempFlag = new bool[m_tableLineSum - 1];
			tempFlag1 = new bool[m_tableLineSum - 1];
			tempFlag2 = new bool[m_tableLineSum - 1];
			if(tempFlag == NULL || tempFlag1 == NULL || tempFlag2 == NULL) return 0;
		}catch(...)
		{
			return 0;
		}
		for(int i = 0;i < m_tableLineSum - 1;++ i)
		{
			if(i < order) 
			{
				tempFlag[i] = m_haveSelectFlag[i];
				tempFlag1[i] = m_check0State[i];
				tempFlag2[i] = m_check1State[i];
			}else 
			{
				tempFlag[i] = m_haveSelectFlag[i + 1];
				tempFlag1[i] = m_check0State[i + 1];
				tempFlag2[i] = m_check1State[i + 1];
			}
		}
		XDELETE_ARRAY(m_haveSelectFlag);
		XDELETE_ARRAY(m_check0State);
		XDELETE_ARRAY(m_check1State);
		m_haveSelectFlag = tempFlag;
		m_check0State = tempFlag1;
		m_check1State = tempFlag2;
	}else
	{//ȫ��ɾ�������
		XDELETE_ARRAY(m_haveSelectFlag);
		XDELETE_ARRAY(m_check0State);
		XDELETE_ARRAY(m_check1State);
		m_haveSelectFlag = NULL;
		m_check0State = NULL;
		m_check1State = NULL;
	}
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
	return 1;
}

int _XMultiListBasic::deleteRow(int order)		//ɾ��ĳһ��
{
	if(m_isInited == 0) return 0;							//���û�г�ʼ��ֱ���˳�
	if(order < 0 || order >= m_tableRowSum) return 0;		//�Ƿ�������
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
			m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
			m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x * m_size.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			for(i = 1;i < m_tableRowSum - 1;++ i)	//�Ѿ�ɾ����һ��Ԫ��
			{
				temp->nextRow->order = i;
				temp->nextRow->position.set(temp->position.x + temp->pixSize.x * m_size.x,
					m_position.y + m_mouseRect.top * m_size.y);
				temp->nextRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
				temp->nextRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_size.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
				temp = temp->nextRow;
			}
		}
		//ɾ�����Ԫ��
		_XMultiListOneBox *tempBox = m_tableBox;
		_XMultiListOneBox *temp1 = NULL;

		tempBox = m_tableBox;
		for(i = 0;i < m_tableLineSum;++ i)
		{
			tempRow = m_tableRow;
			for(j = 0;j < m_tableRowSum;++ j)
			{
				if(i == 0 && j == 0)
				{//��һ��Ԫ�ص�ɾ�����⴦��������������
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
		{//���ֻ��һ�У���ɾ��֮��û�е�λ���
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
					tempRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_size.x - 
						m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
						m_tableRow->position.y);
				}
			}else
			if(i > order)
			{
				tempRow->order = i;
				tempRow->position.set(temp->position.x + temp->pixSize.x * m_size.x,
					m_position.y + m_mouseRect.top * m_size.y);
				tempRow->text.setPosition(temp->nextRow->position.x,temp->nextRow->position.y);
				tempRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_size.x - 
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}else
			{
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}
		}
		//ɾ�����Ԫ��
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
	return 1;
}


int _XMultiListBasic::insertALine(int order)		//��order������һ��
{
	if(m_isInited == 0) return 0;
	if(order < 0 || order >= m_tableLineSum) return 0;
	//��ѡ���ǽ��и���
	bool *tempFlag = NULL;
	bool *tempFlag1 = NULL;
	bool *tempFlag2 = NULL;
	try
	{
		tempFlag = new bool[m_tableLineSum + 1];
		tempFlag1 = new bool[m_tableLineSum + 1];
		tempFlag2 = new bool[m_tableLineSum + 1];
		if(tempFlag == NULL || tempFlag1 == NULL || tempFlag2 == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	for(int i = 0;i < m_tableLineSum + 1;++ i)
	{
		if(i < order) 
		{
			tempFlag[i] = m_haveSelectFlag[i]; 
			tempFlag1[i] = m_check0State[i]; 
			tempFlag2[i] = m_check1State[i]; 
		}else
		if(i == order) 
		{
			tempFlag[i] = false; 
			tempFlag1[i] = false; 
			tempFlag2[i] = false; 
		}else
		{
			tempFlag[i] = m_haveSelectFlag[i + 1];
			tempFlag1[i] = m_check0State[i + 1];
			tempFlag2[i] = m_check1State[i + 1];
		}
	}
	XDELETE_ARRAY(m_haveSelectFlag);
	XDELETE_ARRAY(m_check0State);
	XDELETE_ARRAY(m_check1State);
	m_haveSelectFlag = tempFlag;
	m_check0State = tempFlag1;
	m_check1State = tempFlag2;

	int i,j;
	if(order == 0)
	{//��ͷ������
		_XMultiListOneBox *tempBox = m_tableBox;
		_XMultiListOneRow *tempRow = m_tableRow;
		_XMultiListOneBox *temp1 = m_tableBox;
		try
		{
			m_tableBox = new _XMultiListOneBox;
			if(m_tableBox == NULL) return 0;
		}catch(...)
		{
			return 0;
		}
		m_tableBox->isEnable = 1;
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
			try
			{
				tempBox->nextBox = new _XMultiListOneBox;
				if(tempBox->nextBox == NULL) return 0;
			}catch(...)
			{
				return 0;
			}
			tempBox->nextBox->isEnable = 1;
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

		if(m_haveSelect != 0)
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
					try
					{
						temp1->nextBox = new _XMultiListOneBox;
						if(temp1->nextBox == NULL) 
						{
							return 0;
						}
					}catch(...)
					{
						return 0;
					}
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
				//	{//���һ����ӵ�Ԫ��
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
		if(m_haveSelect != 0)
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
	return 1;
}

int _XMultiListBasic::insertARow(int order)		//��order������һ��
{
	if(m_isInited == 0) return 0;
	if(order < 0 || order >= m_tableRowSum) return 0;
	if(m_tableRowSum >= MUTILIST_MAX_ROW_SUM) return 0;	//�����е����ֵ�����˳�
	int i,j;
	_XMultiListOneRow *tempRow = m_tableRow;
	_XMultiListOneRow *temp = NULL;
	_XMultiListOneBox *tempBox = m_tableBox;
	_XMultiListOneBox *temp1 = NULL;
	if(order == 0)
	{//��ͷ
		//�ı������
		tempRow = m_tableRow;
		try
		{
			m_tableRow = new _XMultiListOneRow;
			if(m_tableRow == NULL) return 0;
		}catch(...)
		{
			return 0;
		}
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
			tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x * m_size.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			tempRow = tempRow->nextRow;
		}
		//�ı�������
		m_tableRowSum ++;
		//�����һ��Ԫ��
		tempBox = m_tableBox;
		try
		{
			m_tableBox = new _XMultiListOneBox;
			if(m_tableBox == NULL) 
			{
				return 0;
			}
		}catch(...)
		{
			return 0;
		}
		m_tableBox->isEnable = 1;
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
					try
					{
						temp1->nextBox = new _XMultiListOneBox;
						if(temp1->nextBox == NULL) 
						{
							return 0;
						}
					}catch(...)
					{
						return 0;
					}
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
				try
				{
					temp->nextRow = new _XMultiListOneRow;
					if(temp->nextRow == NULL) 
					{
						return 0;
					}
				}catch(...)
				{
					return 0;
				}
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
				temp->nextRow->m_spriteTitle.setPosition(temp->nextRow->position);
				temp->nextRow->m_spriteTitleEnd.setPosition(temp->nextRow->position.x + m_tableRow->pixSize.x * m_size.x -
					m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
					m_tableRow->position.y);
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}else
			{
				temp = tempRow;
				tempRow = tempRow->nextRow;
			}
		}
		//�ı�������
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
					try
					{
						temp1->nextBox = new _XMultiListOneBox;
						if(temp1->nextBox == NULL) 
						{
							return 0;
						}
					}catch(...)
					{
						return 0;
					}
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
	return 1;
}

int _XMultiListBasic::moveDownLine(int order)	//��order������
{
	if(m_isInited == 0) return 0;
	if(order < 0 || order >= m_tableLineSum - 1) return 0;	//ֻ��һ�в����ƶ������ݳ�����ΧҲ�����ƶ�
	xChangeTwoSum(m_haveSelectFlag[order],m_haveSelectFlag[order + 1]);
	xChangeTwoSum(m_check0State[order],m_check0State[order + 1]);
	xChangeTwoSum(m_check1State[order],m_check1State[order + 1]);
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
	if(m_haveSelect != 0)
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
	return 1;
}

int _XMultiListBasic::moveRightRow(int order)	//��order������
{
	if(m_isInited == 0) return 0;
	if(order < 0 || order >= m_tableRowSum - 1) return 0;	//ֻ��һ�в����ƶ������ݳ�����ΧҲ�����ƶ�
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
		temp1->m_spriteTitle.setPosition(temp1->position);
		temp1->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->pixSize.x * m_size.x - 
			m_mutiListTitleEnd->textureSize.x * temp1->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,temp1->position.y);
		temp1->nextRow = tempRow;
		temp1->nextRow->order = temp1->order + 1;
		temp1->nextRow->position.set(temp1->position.x + temp1->pixSize.x * m_size.x,m_position.y + m_mouseRect.top * m_size.y);			
		temp1->nextRow->text.setPosition(temp1->nextRow->position.x,temp1->nextRow->position.y);
		temp1->nextRow->m_spriteTitle.setPosition(temp1->nextRow->position);
		temp1->nextRow->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->nextRow->pixSize.x * m_size.x - 
			m_mutiListTitleEnd->textureSize.x * temp1->nextRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
			temp1->nextRow->position.y);
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
				temp1->m_spriteTitle.setPosition(temp1->position);
				temp1->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->pixSize.x * m_size.x - 
					m_mutiListTitleEnd->textureSize.x * temp1->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,temp1->position.y);
				temp1->nextRow = tempRow;
				temp1->nextRow->order = temp1->order + 1;
				temp1->nextRow->position.set(temp1->position.x + temp1->pixSize.x * m_size.x,m_position.y + m_mouseRect.top * m_size.y);			
				temp1->nextRow->text.setPosition(temp1->nextRow->position.x,temp1->nextRow->position.y);
				temp1->nextRow->m_spriteTitle.setPosition(temp1->nextRow->position);
				temp1->nextRow->m_spriteTitleEnd.setPosition(temp1->position.x + temp1->nextRow->pixSize.x * m_size.x - 
					m_mutiListTitleEnd->textureSize.x * temp1->nextRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
					temp1->nextRow->position.y);
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
	return 1;
}

void _XMultiListBasic::setPosition(float x,float y)
{
	if(m_isInited == 0) return;
	m_position.set(x,y);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		//m_check0[i].setSize(m_size);
		m_check0[i].setPosition(m_position.x + 4.0f * m_size.x,m_position.y + (36.0f + m_nowTextHeight * i) * m_size.y);
		//m_check1[i].setSize(m_size);
		m_check1[i].setPosition(m_position.x + 4.0f * m_size.x + 32.0f * m_size.x,m_position.y + (36.0f + m_nowTextHeight * i) * m_size.y);
	}
	m_spriteBackGround.setPosition(m_position);
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
		m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x - 
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
		_XMultiListOneRow *tempRow = m_tableRow;
		int i,j;
		for(i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_size.x,
				m_position.y + m_mouseRect.top * m_size.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + tempRow->nextRow->pixSize.x * m_size.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y * m_size.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			tempRow = tempRow->nextRow;
		}
		//�ı����λ��
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
	if(m_haveSelect != 0)
	{
		updateSelectLine();
	}
	updateShowChange();	//������������������ƶ���ʱ���bug
	//updateSliderState();
}

void _XMultiListBasic::setSize(float x,float y)	//���óߴ�
{
	if(m_isInited == 0) return;
	if(x <= 0 && y <= 0) return;
	m_size.set(x,y);
	for(int i = 0;i < m_canShowLineSum;++ i)
	{
		m_check0[i].setSize(m_size);
		m_check0[i].setPosition(m_position.x + 4.0f * m_size.x,m_position.y + (36.0f + m_nowTextHeight * i) * m_size.y);
		m_check1[i].setSize(m_size);
		m_check1[i].setPosition(m_position.x + 4.0f * m_size.x + 32.0f * m_size.x,m_position.y + (36.0f + m_nowTextHeight * i) * m_size.y);
	}
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setSize(m_size);
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_verticalSlider.setSize(m_size);
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	m_horizontalSlider.setSize(m_size);
	m_spriteMove.setSize(m_size);
	m_caption.setSize(m_size * m_fontSize);
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;
	m_showPixWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_size.x;
	m_showPixHight = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight() - m_mutiListTitle->textureSize.y) * m_size.y;
	m_spriteSelect.setSize((float)m_showPixWidth / m_mutiListSelect->textureSize.x,m_nowTextHeight / m_mutiListSelect->textureSize.y);
	//�ı��������λ��
	if(m_tableRowSum > 0)
	{
		m_tableRow->position.set(m_position.x + (m_mouseRect.left + MUTILIST_MIN_WIDTH * 0) * m_size.x,
			m_position.y + m_mouseRect.top * m_size.y);
		m_tableRow->text.setPosition(m_tableRow->position.x,m_tableRow->position.y);
		m_tableRow->text.setSize(m_size * m_fontSize);
		m_tableRow->m_spriteTitle.setPosition(m_tableRow->position);
		m_tableRow->m_spriteTitle.setSize(m_tableRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
		m_tableRow->m_spriteTitleEnd.setPosition(m_tableRow->position.x + m_tableRow->pixSize.x - 
			m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,m_tableRow->position.y);
		m_tableRow->m_spriteTitleEnd.setSize(m_tableRow->pixSize.y * m_size.y/ m_mutiListTitle->textureSize.y,
			m_tableRow->pixSize.y * m_size.y/m_mutiListTitle->textureSize.y);
		_XMultiListOneRow *tempRow = m_tableRow;

		int i,j;
		for(i = 1;i < m_tableRowSum;++ i)
		{
			tempRow->nextRow->position.set(tempRow->position.x + tempRow->pixSize.x * m_size.x,
				m_position.y + m_mouseRect.top * m_size.y);
			tempRow->nextRow->text.setPosition(tempRow->nextRow->position.x,tempRow->nextRow->position.y);
			tempRow->nextRow->text.setSize(m_size * m_fontSize);
			tempRow->nextRow->m_spriteTitle.setPosition(tempRow->nextRow->position);
			tempRow->nextRow->m_spriteTitle.setSize(tempRow->nextRow->pixSize.x * m_size.x / m_mutiListTitle->textureSize.x,m_size.y);
			tempRow->nextRow->m_spriteTitleEnd.setPosition(tempRow->nextRow->position.x + m_tableRow->pixSize.x - 
				m_mutiListTitleEnd->textureSize.x * m_tableRow->pixSize.y / m_mutiListTitle->textureSize.y,
				m_tableRow->position.y);
			tempRow->nextRow->m_spriteTitleEnd.setSize(tempRow->nextRow->pixSize.y * m_size.y/ m_mutiListTitle->textureSize.y,
				tempRow->nextRow->pixSize.y * m_size.y/m_mutiListTitle->textureSize.y);
			tempRow = tempRow->nextRow;
		}
		//�ı����λ��
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
	if(m_haveSelect != 0)
	{
		updateSelectLine();
	}
	updateShowChange();
	updateSliderState();
}

int _XMultiListBasic::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}

_XVector2 _XMultiListBasic::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;
	if(order == 0) ret.set(m_position.x + (m_mouseRect.left - 64.0f) * m_size.x,m_position.y + m_mouseRect.top * m_size.y); else
	if(order == 1) ret.set(m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.top * m_size.y); else
	if(order == 2) ret.set(m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y); else
	if(order == 3) ret.set(m_position.x + (m_mouseRect.left - 64.0f) * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y); 

	return ret;
}
