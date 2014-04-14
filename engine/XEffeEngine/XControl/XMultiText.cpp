//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XMultiText.h"
#include "XObjectManager.h" 
#include "XControlManager.h"
#include "XResourceManager.h"

void funMutiTextValueChangeV(void *pClass,int objectID)
{
	_XMultiText *pPar = (_XMultiText *)pClass;
//	printf("V ID:%d\n",pPar->m_objectID);
//	printf("V Now value:%f\n",pPar->m_verticalSlider.getNowValue());
	//��Ӧ��ֵ�ĸı�
	int temp = pPar->m_verticalSlider.getNowValue();
	if(temp != pPar->m_nowStartLine)
	{
		pPar->setStartLine(temp);
		pPar->updateLineStr();
		pPar->m_verticalSlider.setNowValue(temp);
	}else
	{//΢���ƶ�ʱ��Ĵ���
		if(pPar->m_verticalSlider.getNowValue() > pPar->m_nowStartLine)
		{
			temp += 1;
			pPar->setStartLine(temp);
			pPar->updateLineStr();
			pPar->m_verticalSlider.setNowValue(temp);
		}else
		if(pPar->m_verticalSlider.getNowValue() < pPar->m_nowStartLine)
		{
			temp -= 1;
			pPar->setStartLine(temp);
			pPar->updateLineStr();
			pPar->m_verticalSlider.setNowValue(temp);
		}
	}
}
void funMutiTextValueChangeMoveV(void *pClass,int objectID)
{
	_XMultiText *pPar = (_XMultiText *)pClass;
//	printf("V ID:%d\n",pPar->m_objectID);
//	printf("V Now value:%f\n",pPar->m_verticalSlider.getNowValue());
	//��Ӧ��ֵ�ĸı�
	int temp = pPar->m_verticalSlider.getNowValue();
	if(temp != pPar->m_nowStartLine)
	{
		//��Ҫ���´�ֱƬѡ�����
		pPar->setStartLine(temp);
		pPar->updateLineStr();
	}
}
void funMutiTextValueChangeH(void *pClass,int objectID)
{
	_XMultiText *pPar = (_XMultiText *)pClass;
//	printf("H ID:%d\n",pPar->m_objectID);
//	printf("H Now value:%f\n",pPar->m_horizontalSlider.getNowValue());
	//��Ӧ��ֵ�ĸı�
	int temp = pPar->m_horizontalSlider.getNowValue();
	if(temp != pPar->m_nowStartPixels)
	{
		printf("%d\n",temp);
		pPar->m_nowStartPixels = temp;
		pPar->updateLineStr();
		pPar->m_horizontalSlider.setNowValue(temp);
	}
/*	if(temp != pPar->m_lineStr[0].showStart)
	{
		for(int i = 0;i < pPar->m_nowShowLineSum;++ i)
		{
			pPar->m_lineStr[i].showStart = temp;
		}
		pPar->updateLineStr();
		pPar->m_horizontalSlider.setNowValue(temp);
	}else
	{
		if(pPar->m_horizontalSlider.getNowValue() > pPar->m_lineStr[0].showStart)
		{
			temp += 1;
			for(int i = 0;i < pPar->m_nowShowLineSum;++ i)
			{
				pPar->m_lineStr[i].showStart = temp;
			}
			pPar->updateLineStr();
			pPar->m_horizontalSlider.setNowValue(temp);
		}else
		if(pPar->m_horizontalSlider.getNowValue() < pPar->m_lineStr[0].showStart)
		{
			temp -= 1;
			for(int i = 0;i < pPar->m_nowShowLineSum;++ i)
			{
				pPar->m_lineStr[i].showStart = temp;
			}
			pPar->updateLineStr();
			pPar->m_horizontalSlider.setNowValue(temp);
		}
	}*/
}
void funMutiTextValueChangeMoveH(void *pClass,int objectID)
{
	_XMultiText *pPar = (_XMultiText *)pClass;
//	printf("H ID:%d\n",pPar->m_objectID);
//	printf("H Now value:%f\n",pPar->m_horizontalSlider.getNowValue());
	//��Ӧ��ֵ�ĸı�
	int temp = pPar->m_horizontalSlider.getNowValue();
	if(temp != pPar->m_nowStartPixels)
	{
		pPar->m_nowStartPixels = temp;
		pPar->updateLineStr();
	}
/*	if(temp != pPar->m_lineStr[0].showStart)
	{
		for(int i = 0;i < pPar->m_nowShowLineSum;++ i)
		{
			pPar->m_lineStr[i].showStart = temp;
		}
		pPar->updateLineStr();
	}*/
}
_XMultiText::_XMultiText()
:m_isInited(XFalse)
,m_nowString(NULL)				//��ǰ������ַ���������
,m_mutiEditNormal(NULL)			//�������ͨ״̬
,m_mutiEditDisable(NULL)		//�������Ч״̬
,m_mutiEditSelect(NULL)			//�����Ƭѡ��ɫ
//,m_funInputChenge(NULL)			//�������ݷ����ı��ʱ�����
//,m_funInputOver(NULL)			//ȷ���������֮���ʱ�����
,m_lineStr(NULL)
,m_resInfo(NULL)
,m_withoutTex(XFalse)
,m_funSelect(NULL)
,m_pClass(NULL)
{
}
_XMultiText::~_XMultiText()
{
	release();
}
void _XMultiText::release()
{
	if(!m_isInited) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_lineStr != NULL)
	{
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			XDELETE_ARRAY(m_lineStr[i].showString);
		}
	}
	XDELETE_ARRAY(m_lineStr);
	XDELETE_ARRAY(m_nowString);
	_XControlManager::GetInstance().decreaseAObject(this);	//ע��������
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
void _XMultiText::setStartLine(int temp)
{
	if(m_haveSelect)
	{
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].haveAllSelect = XFalse;
	}
	m_nowStartLine = temp;
	if(m_haveSelect && 
		(m_nowSelectLineOrder >= m_nowStartLine && m_nowSelectLineOrder < m_nowShowLineSum - m_nowStartLine))
	{
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].haveAllSelect = XTrue;
		if(!m_withoutTex)
		{
			m_lineStr[m_nowSelectLineOrder - m_nowStartLine].spriteSelect.setPosition(m_nowMouseRect.left,
				m_nowMouseRect.top + m_textHeight * (m_nowSelectLineOrder - m_nowStartLine));
			m_lineStr[m_nowSelectLineOrder - m_nowStartLine].spriteSelect.setSize(m_nowMouseRect.getWidth() / m_mutiEditSelect->textureSize.x,
							m_textHeight / m_mutiEditSelect->textureSize.y);
		}
	}
}
_XBool _XMultiText::init(const _XVector2& position,	//�ؼ����ڵ�λ��
		const _XRect& Area,	//�ؼ��ķ�Χ
		const _XMultiTextTexture& tex,	//�ؼ�����ͼ
		const char *str,const _XFontUnicode& font,float strSize,		//�ؼ������弰�����Ϣ
		//const _XVector2& strPosition,
		//const _XMouseRightButtonMenu &mouseMenu,
		const _XSlider &vSlider,const _XSlider &hSlider)	//�ؼ����������������ؼ�
{
	if(m_isInited) return XFalse;	//��ֹ�ظ���ʼ��
	if(tex.editNormal == NULL || tex.editInsert == NULL || tex.editDisable == NULL || tex.editSelect == NULL) return XFalse;

	//��Ҫע��������뷶Χ����Ҫ����ʾ����һ���ַ������򽫻�������⣬Ŀǰ���ﲢû�д���ȥ�жϣ�����ʵ��ʹ������Ҫע���������
	if(Area.getWidth() <= 0 || Area.getHeight() <= 0) return XFalse;	//���뷶Χ����Ϊ��
	if(strSize <= 0) return XFalse;		//�ַ����Ĵ�С����Ϊ�Ƿ�ֵ
	if(str != NULL && strlen(str) >= MAX_STRING_LENGTH) return XFalse;

	//���濪ʼ��ֵ
	m_position = position;
	m_mouseRect = Area;
	m_withoutTex = XFalse;

	m_mutiEditNormal = tex.editNormal;			//�������ͨ״̬
	m_mutiEditDisable = tex.editDisable;		//�������Ч״̬
	m_mutiEditSelect = tex.editSelect;			//�����Ƭѡ��ɫ
	m_mutiEditUpon = tex.editUpon;
	m_size.set(1.0f,1.0f);

	//m_textPosition = strPosition;			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize.set(strSize,strSize);				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����

	m_spriteBackGround.init(m_mutiEditNormal->texture.m_w,m_mutiEditNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setSize(m_size);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_verticalSlider.setACopy(vSlider);
	_XControlManager::GetInstance().decreaseAObject(&m_verticalSlider);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_verticalSlider.setSize(m_size);
	m_verticalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiTextValueChangeV,funMutiTextValueChangeMoveV,this);
	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������

	m_horizontalSlider.setACopy(hSlider);
	_XControlManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	m_horizontalSlider.setSize(m_size);
	m_horizontalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiTextValueChangeH,funMutiTextValueChangeMoveH,this);
	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������

	m_textWidth = font.getTextSize().x * font.getSize().x * 0.5f;
	m_textHeight = font.getTextSize().y * font.getSize().y;
	//���㵱ǰ������ʾ������
	m_nowShowLineSum = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight())/m_textHeight;	//��Ӧ�ý�ȥ�߶ȣ���Ӧ�ü�ȥbuttom ���Ժ�������
	//���㵱ǰһ�п�����ʾ���ٸ��ַ�(���0.5����Ϊ��ʾӢ����ĸȴʹ�������ֿ�)
	//m_nowShowLineWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth())/m_textWidth;	//��Ӧ�ü�ȥ��ȣ���Ӧ�ü�ȥright���Ժ�������

	m_nowStartPixels = 0;
	m_nowMaxLineWidthPixels = 0;
	m_MaxLineWidthPixels = m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth();

	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);	//��ǰ�������Ӧ��Χ
	//Ϊÿ�з����ڴ�ռ�
	m_lineStr = createArrayMem<_XMultiTextLineString>(m_nowShowLineSum);
	if(m_lineStr == NULL) return XFalse;

	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		//m_lineStr[i].showString = createArrayMem<char>(m_nowShowLineWidth + 1);
		m_lineStr[i].showString = createArrayMem<char>(MAX_LINE_STRING_LENGTH + 1);
		if(m_lineStr[i].showString == NULL)
		{
			for(int j = 0;j < i;++ j)
			{
				XDELETE_ARRAY(m_lineStr[j].showString);
			}
			XDELETE_ARRAY(m_lineStr);
			return XFalse;
		}
		m_lineStr[i].isEnable = XFalse;
		m_lineStr[i].nowText.setACopy(font);
		m_lineStr[i].nowText.setMaxStrLen(MAX_LINE_STRING_LENGTH);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_lineStr[i].nowText));
#endif
		m_lineStr[i].nowText.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
		m_lineStr[i].nowText.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
		m_lineStr[i].nowText.setColor(m_textColor);
		m_lineStr[i].nowText.setPosition(m_position.x + (m_mouseRect.left - m_nowStartPixels) * m_size.x,
			m_position.y + m_mouseRect.top * m_size.y + m_textHeight * i);
		m_lineStr[i].nowText.setSize(m_size);
	//	m_lineStr[i].showLength = m_nowShowLineWidth;
	//	m_lineStr[i].showStart = 0;
		m_lineStr[i].haveAllSelect = XFalse;
		m_lineStr[i].spriteSelect.init(m_mutiEditSelect->texture.m_w,m_mutiEditSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_lineStr[i].spriteSelect));
#endif
		m_lineStr[i].spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);
		m_lineStr[i].showString[0] = '\0';
	}
	//Ϊ�����ַ��������ڴ�ռ�
	m_nowString = createArrayMem<char>(MAX_STRING_LENGTH);
	if(m_nowString == NULL) 
	{
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			XDELETE_ARRAY(m_lineStr[i].showString);
		}
		XDELETE_ARRAY(m_lineStr);
		return XFalse;
	}
	m_nowString[0] = '\0';
	//�����ַ���
	if(str != NULL) strcpy(m_nowString,str);
	//�����ַ���
	m_nowStartLine = 0;
	updateLineStr();

	m_haveSelect = XFalse;		//�Ƿ����ַ�������ѡ��
	m_nowSelectLineOrder = -1;

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MUTITEXT);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XMultiText::initEx(const _XVector2& position,	//�ؼ����ڵ�λ��
	const _XMultiTextTexture& tex,	//�ؼ�����ͼ
	const char *str,const _XFontUnicode& font, float strSize,		//�ؼ������弰�����Ϣ
	const _XSlider &vSlider,const _XSlider &hSlider)	//�ؼ����������������ؼ�
{
	if(m_isInited) return XFalse;	//��ֹ�ظ���ʼ��
	if(tex.editNormal == NULL || tex.editInsert == NULL || tex.editDisable == NULL || tex.editSelect == NULL) return XFalse;

	//��Ҫע��������뷶Χ����Ҫ����ʾ����һ���ַ������򽫻�������⣬Ŀǰ���ﲢû�д���ȥ�жϣ�����ʵ��ʹ������Ҫע���������
	if(tex.m_mouseRect.getWidth() <= 0 || tex.m_mouseRect.getHeight() <= 0) return XFalse;	//���뷶Χ����Ϊ��
	if(strSize <= 0) return XFalse;		//�ַ����Ĵ�С����Ϊ�Ƿ�ֵ
	if(str != NULL && strlen(str) >= MAX_STRING_LENGTH) return XFalse;

	//���濪ʼ��ֵ
	m_position = position;
	m_mouseRect = tex.m_mouseRect;
	m_withoutTex = XFalse;

	m_mutiEditNormal = tex.editNormal;			//�������ͨ״̬
	m_mutiEditDisable = tex.editDisable;		//�������Ч״̬
	m_mutiEditSelect = tex.editSelect;			//�����Ƭѡ��ɫ
	m_mutiEditUpon = tex.editUpon;
	m_size.set(1.0f,1.0f);

	//m_textPosition = strPosition;			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize.set(strSize,strSize);				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����

	m_spriteBackGround.init(m_mutiEditNormal->texture.m_w,m_mutiEditNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setSize(m_size);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_verticalSlider.setACopy(vSlider);
	_XControlManager::GetInstance().decreaseAObject(&m_verticalSlider);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_verticalSlider.setSize(m_size);
	m_verticalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiTextValueChangeV,funMutiTextValueChangeMoveV,this);
	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������

	m_horizontalSlider.setACopy(hSlider);
	_XControlManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	m_horizontalSlider.setSize(m_size);
	m_horizontalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiTextValueChangeH,funMutiTextValueChangeMoveH,this);
	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������

	m_textWidth = font.getTextSize().x * font.getSize().x * 0.5f;
	m_textHeight = font.getTextSize().y * font.getSize().y;
	//���㵱ǰ������ʾ������
	m_nowShowLineSum = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight())/m_textHeight;	//��Ӧ�ý�ȥ�߶ȣ���Ӧ�ü�ȥbuttom ���Ժ�������
	//���㵱ǰһ�п�����ʾ���ٸ��ַ�(���0.5����Ϊ��ʾӢ����ĸȴʹ�������ֿ�)
	//m_nowShowLineWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth())/m_textWidth;	//��Ӧ�ü�ȥ��ȣ���Ӧ�ü�ȥright���Ժ�������

	m_nowStartPixels = 0;
	m_nowMaxLineWidthPixels = 0;
	m_MaxLineWidthPixels = m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth();

	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);	//��ǰ�������Ӧ��Χ
	//Ϊÿ�з����ڴ�ռ�
	m_lineStr = createArrayMem<_XMultiTextLineString>(m_nowShowLineSum);
	if(m_lineStr == NULL) return XFalse;

	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		m_lineStr[i].showString = createArrayMem<char>(MAX_LINE_STRING_LENGTH + 1);
		//m_lineStr[i].showString = createArrayMem<char>(m_nowShowLineWidth + 1);
		if(m_lineStr[i].showString == NULL)
		{
			for(int j = 0;j < i;++ j)
			{
				XDELETE_ARRAY(m_lineStr[j].showString);
			}
			XDELETE_ARRAY(m_lineStr);
			return XFalse;
		}
		m_lineStr[i].isEnable = XFalse;
		m_lineStr[i].nowText.setACopy(font);
		m_lineStr[i].nowText.setMaxStrLen(MAX_LINE_STRING_LENGTH);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_lineStr[i].nowText));
#endif
		m_lineStr[i].nowText.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
		m_lineStr[i].nowText.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
		m_lineStr[i].nowText.setColor(m_textColor);
		m_lineStr[i].nowText.setPosition(m_position.x + (m_mouseRect.left - m_nowStartPixels) * m_size.x,
			m_position.y + m_mouseRect.top * m_size.y + m_textHeight * i);
		m_lineStr[i].nowText.setSize(m_size);
	//	m_lineStr[i].showLength = m_nowShowLineWidth;
	//	m_lineStr[i].showStart = 0;
		m_lineStr[i].haveAllSelect = XFalse;
		m_lineStr[i].spriteSelect.init(m_mutiEditSelect->texture.m_w,m_mutiEditSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_lineStr[i].spriteSelect));
#endif
		m_lineStr[i].spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);
		m_lineStr[i].showString[0] = '\0';
	}
	//Ϊ�����ַ��������ڴ�ռ�
	m_nowString = createArrayMem<char>(MAX_STRING_LENGTH);
	if(m_nowString == NULL) 
	{
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			XDELETE_ARRAY(m_lineStr[i].showString);
		}
		XDELETE_ARRAY(m_lineStr);
		return XFalse;
	}
	m_nowString[0] = '\0';
	//�����ַ���
	if(str != NULL) strcpy(m_nowString,str);
	//�����ַ���
	m_nowStartLine = 0;
	updateLineStr();

	m_haveSelect = XFalse;		//�Ƿ����ַ�������ѡ��
	m_nowSelectLineOrder = -1;

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MUTITEXT);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XMultiText::initPlus(const char *path,
		const char *str,const _XFontUnicode& font, float strSize,
		_XResourcePosition resoursePosition)
{
	if(m_isInited) return XFalse;	//��ֹ�ظ���ʼ��
	m_resInfo = _XResourceManager::GetInstance().loadResource(path,RESOURCE_TYPE_XEDIT_TEX,resoursePosition);
	if(m_resInfo == NULL) return XFalse;
	_XEditTexture * tex = (_XEditTexture *)m_resInfo->m_pointer;
	if(tex->editNormal == NULL || tex->editInsert == NULL || tex->editDisable == NULL || tex->editSelect == NULL) return XFalse;

	//��Ҫע��������뷶Χ����Ҫ����ʾ����һ���ַ������򽫻�������⣬Ŀǰ���ﲢû�д���ȥ�жϣ�����ʵ��ʹ������Ҫע���������
	if(tex->m_mouseRect.getWidth() <= 0 || tex->m_mouseRect.getHeight() <= 0) return XFalse;	//���뷶Χ����Ϊ��
	if(strSize <= 0) return XFalse;		//�ַ����Ĵ�С����Ϊ�Ƿ�ֵ
	if(str != NULL && strlen(str) >= MAX_STRING_LENGTH) return XFalse;

	//���濪ʼ��ֵ
	m_position.set(0.0f,0.0f);
	m_mouseRect = tex->m_mouseRect;
	m_withoutTex = XFalse;

	m_mutiEditNormal = tex->editNormal;			//�������ͨ״̬
	m_mutiEditDisable = tex->editDisable;		//�������Ч״̬
	m_mutiEditSelect = tex->editSelect;			//�����Ƭѡ��ɫ
	m_mutiEditUpon = tex->editUpon;
	m_size.set(1.0f,1.0f);

	//m_textPosition = strPosition;			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize.set(strSize,strSize);				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����

	m_spriteBackGround.init(m_mutiEditNormal->texture.m_w,m_mutiEditNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setSize(m_size);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	//m_verticalSlider.setACopy(vSlider);
	char tempPath[MAX_FILE_NAME_LENGTH];
	sprintf(tempPath,"%s/SliderV",path);
	m_verticalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_VERTICAL,resoursePosition);
	_XControlManager::GetInstance().decreaseAObject(&m_verticalSlider);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_verticalSlider.setSize(m_size);
	m_verticalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiTextValueChangeV,funMutiTextValueChangeMoveV,this);
	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������

	//m_horizontalSlider.setACopy(hSlider);
	sprintf(tempPath,"%s/SliderH",path);
	m_horizontalSlider.initPlus(tempPath,100.0f,0.0f,SLIDER_TYPE_HORIZONTAL,resoursePosition);
	_XControlManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	m_horizontalSlider.setSize(m_size);
	m_horizontalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiTextValueChangeH,funMutiTextValueChangeMoveH,this);
	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������

	m_textWidth = font.getTextSize().x * font.getSize().x * 0.5f;
	m_textHeight = font.getTextSize().y * font.getSize().y;
	//���㵱ǰ������ʾ������
	m_nowShowLineSum = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight())/m_textHeight;	//��Ӧ�ý�ȥ�߶ȣ���Ӧ�ü�ȥbuttom ���Ժ�������
	//���㵱ǰһ�п�����ʾ���ٸ��ַ�(���0.5����Ϊ��ʾӢ����ĸȴʹ�������ֿ�)
	//m_nowShowLineWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth())/m_textWidth;	//��Ӧ�ü�ȥ��ȣ���Ӧ�ü�ȥright���Ժ�������

	m_nowStartPixels = 0;
	m_nowMaxLineWidthPixels = 0;
	m_MaxLineWidthPixels = m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth();

	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);	//��ǰ�������Ӧ��Χ
	//Ϊÿ�з����ڴ�ռ�
	m_lineStr = createArrayMem<_XMultiTextLineString>(m_nowShowLineSum);
	if(m_lineStr == NULL) return XFalse;

	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		m_lineStr[i].showString = createArrayMem<char>(MAX_LINE_STRING_LENGTH + 1);
		//m_lineStr[i].showString = createArrayMem<char>(m_nowShowLineWidth + 1);
		if(m_lineStr[i].showString == NULL)
		{
			for(int j = 0;j < i;++ j)
			{
				XDELETE_ARRAY(m_lineStr[j].showString);
			}
			XDELETE_ARRAY(m_lineStr);
			return XFalse;
		}
		m_lineStr[i].isEnable = XFalse;
		m_lineStr[i].nowText.setACopy(font);
		m_lineStr[i].nowText.setMaxStrLen(MAX_LINE_STRING_LENGTH);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_lineStr[i].nowText));
#endif
		m_lineStr[i].nowText.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
		m_lineStr[i].nowText.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
		m_lineStr[i].nowText.setColor(m_textColor);
		m_lineStr[i].nowText.setPosition(m_position.x + (m_mouseRect.left - m_nowStartPixels) * m_size.x,
			m_position.y + m_mouseRect.top * m_size.y + m_textHeight * i);
		m_lineStr[i].nowText.setSize(m_size);
	//	m_lineStr[i].showLength = m_nowShowLineWidth;
	//	m_lineStr[i].showStart = 0;
		m_lineStr[i].haveAllSelect = XFalse;
		m_lineStr[i].spriteSelect.init(m_mutiEditSelect->texture.m_w,m_mutiEditSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_lineStr[i].spriteSelect));
#endif
		m_lineStr[i].spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);
		m_lineStr[i].showString[0] = '\0';
	}
	//Ϊ�����ַ��������ڴ�ռ�
	m_nowString = createArrayMem<char>(MAX_STRING_LENGTH);
	if(m_nowString == NULL) 
	{
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			XDELETE_ARRAY(m_lineStr[i].showString);
		}
		XDELETE_ARRAY(m_lineStr);
		return XFalse;
	}
	m_nowString[0] = '\0';
	//�����ַ���
	if(str != NULL) strcpy(m_nowString,str);
	//�����ַ���
	m_nowStartLine = 0;
	updateLineStr();

	m_haveSelect = XFalse;		//�Ƿ����ַ�������ѡ��
	m_nowSelectLineOrder = -1;

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MUTITEXT);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
_XBool _XMultiText::initWithoutTex(const _XRect& area,
		const char *str,const _XFontUnicode& font, float strSize)
{
	if(m_isInited) return XFalse;	//��ֹ�ظ���ʼ��

	//��Ҫע��������뷶Χ����Ҫ����ʾ����һ���ַ������򽫻�������⣬Ŀǰ���ﲢû�д���ȥ�жϣ�����ʵ��ʹ������Ҫע���������
	if(strSize <= 0) return XFalse;		//�ַ����Ĵ�С����Ϊ�Ƿ�ֵ
	if(str != NULL && strlen(str) >= MAX_STRING_LENGTH) return XFalse;

	//���濪ʼ��ֵ
	m_position.set(0.0f,0.0f);
	m_mouseRect = area;
	m_withoutTex = XTrue;

	m_size.set(1.0f,1.0f);

	m_textSize.set(strSize,strSize);				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����

//	m_verticalSlider.initWithoutTex(_XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight() - DEFAULT_SLIDER_WIDTH),
//		_XRect(0,0,DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_VERTICAL,100.0f,0.0f,_XVector2(0.0f,0.0f));
	m_verticalSlider.initWithoutTex(_XRect(0,0,DEFAULT_SLIDER_WIDTH,m_mouseRect.getHeight() - DEFAULT_SLIDER_WIDTH),
		100.0f,0.0f,SLIDER_TYPE_VERTICAL);
	_XControlManager::GetInstance().decreaseAObject(&m_verticalSlider);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_verticalSlider.setSize(m_size);
	m_verticalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiTextValueChangeV,funMutiTextValueChangeMoveV,this);
	m_needShowVSlider = XFalse;			//�Ƿ���Ҫ��ʾ��ֱ������

//	m_horizontalSlider.initWithoutTex(_XRect(0,0,m_mouseRect.getWidth() - DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),
//		_XRect(0,0,DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),SLIDER_TYPE_HORIZONTAL,100.0f,0.0f,_XVector2(0.0f,0.0f));
	m_horizontalSlider.initWithoutTex(_XRect(0,0,m_mouseRect.getWidth() - DEFAULT_SLIDER_WIDTH,DEFAULT_SLIDER_WIDTH),
		100.0f,0.0f,SLIDER_TYPE_HORIZONTAL);
	_XControlManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	m_horizontalSlider.setSize(m_size);
	m_horizontalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiTextValueChangeH,funMutiTextValueChangeMoveH,this);
	m_needShowHSlider = XFalse;			//�Ƿ���Ҫ��ʾˮƽ������

	m_textWidth = font.getTextSize().x * font.getSize().x * 0.5f;
	m_textHeight = font.getTextSize().y * font.getSize().y;
	//���㵱ǰ������ʾ������
	m_nowShowLineSum = (m_mouseRect.getHeight() - m_horizontalSlider.getMouseRectHeight())/m_textHeight;	//��Ӧ�ý�ȥ�߶ȣ���Ӧ�ü�ȥbuttom ���Ժ�������
	//���㵱ǰһ�п�����ʾ���ٸ��ַ�(���0.5����Ϊ��ʾӢ����ĸȴʹ�������ֿ�)
	//m_nowShowLineWidth = (m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth())/m_textWidth;	//��Ӧ�ü�ȥ��ȣ���Ӧ�ü�ȥright���Ժ�������

	m_nowStartPixels = 0;
	m_nowMaxLineWidthPixels = 0;
	m_MaxLineWidthPixels = m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth();

	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);	//��ǰ�������Ӧ��Χ
	//Ϊÿ�з����ڴ�ռ�
	m_lineStr = createArrayMem<_XMultiTextLineString>(m_nowShowLineSum);
	if(m_lineStr == NULL) return XFalse;

	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		m_lineStr[i].showString = createArrayMem<char>(MAX_LINE_STRING_LENGTH + 1);
		//m_lineStr[i].showString = createArrayMem<char>(m_nowShowLineWidth + 1);
		if(m_lineStr[i].showString == NULL)
		{
			for(int j = 0;j < i;++ j)
			{
				XDELETE_ARRAY(m_lineStr[j].showString);
			}
			XDELETE_ARRAY(m_lineStr);
			return XFalse;
		}
		m_lineStr[i].isEnable = XFalse;
		m_lineStr[i].nowText.setACopy(font);
		m_lineStr[i].nowText.setMaxStrLen(MAX_LINE_STRING_LENGTH);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_lineStr[i].nowText));
#endif
		m_lineStr[i].nowText.setAlignmentModeX(FONT_ALIGNMENT_MODE_X_LEFT); //�������������
		m_lineStr[i].nowText.setAlignmentModeY(FONT_ALIGNMENT_MODE_Y_UP);	 //���������϶���
		m_lineStr[i].nowText.setColor(m_textColor);
		m_lineStr[i].nowText.setPosition(m_position.x + (m_mouseRect.left - m_nowStartPixels) * m_size.x,
			m_position.y + m_mouseRect.top * m_size.y + m_textHeight * i);
		m_lineStr[i].nowText.setSize(m_size);
	//	m_lineStr[i].showLength = m_nowShowLineWidth;
	//	m_lineStr[i].showStart = 0;
		m_lineStr[i].haveAllSelect = XFalse;
		m_lineStr[i].showString[0] = '\0';
	}
	//Ϊ�����ַ��������ڴ�ռ�
	m_nowString = createArrayMem<char>(MAX_STRING_LENGTH);
	if(m_nowString == NULL) 
	{
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			XDELETE_ARRAY(m_lineStr[i].showString);
		}
		XDELETE_ARRAY(m_lineStr);
		return XFalse;
	}
	m_nowString[0] = '\0';
	//�����ַ���
	if(str != NULL) strcpy(m_nowString,str);
	//�����ַ���
	m_nowStartLine = 0;
	updateLineStr();

	m_haveSelect = XFalse;		//�Ƿ����ַ�������ѡ��
	m_nowSelectLineOrder = -1;

	m_isVisiable = XTrue;
	m_isEnable = XTrue;
	m_isActive = XTrue;

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MUTITEXT);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = XTrue;
	return XTrue;
}
void _XMultiText::updateLineStr()
{//��ʵ�ʵ��ַ������䵽����ȥ
	//if(m_isInited == 0) return;	//���û�г�ʼ��ֱ���˳�
	if(m_nowString == NULL) return;
	/*int lineLength = 0;
	int maxLineLength = 0;
	int lineSum = 0;
	char tempChar[MAX_LINE_STRING_LENGTH];
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		m_lineStr[i].isEnable = 0;
	}
	if(m_nowStartLine == 0)
	{//����Ǵ��ַ�����ͷ��ʾ������Ҫ��ʼ����һ�е�ͷλ�ַ�����ʼ
		m_lineStr[0].lineHeadorder = 0;
	}
	for(int i = 0;;++ i)
	{		
		if(m_nowString[i] == '\0')
		{
			if(i == 0)
			{//���ַ���(ʲôҲ����ʾ��ֱ���˳�)
				break;
			}else
			{//���һ��
				if(lineLength == 0)
				{//������һ�еĳ���Ϊ��Ҳʲô��������
					break;
				}else
				{
					if(lineSum >= m_nowStartLine && lineSum < m_nowStartLine + m_nowShowLineSum)
					{//�����ַ���������ʾ�ڴ�
						if(lineLength >= m_lineStr[lineSum - m_nowStartLine].showStart)
						{
							m_lineStr[lineSum - m_nowStartLine].isEnable = 1;
							if(lineLength - m_lineStr[lineSum - m_nowStartLine].showStart > m_lineStr[lineSum - m_nowStartLine].showLength)
							{//ʵ���ַ����ĳ��ȳ�����ʾ����
								memcpy(m_lineStr[lineSum - m_nowStartLine].showString,tempChar + m_lineStr[lineSum - m_nowStartLine].showStart,
									m_lineStr[lineSum - m_nowStartLine].showLength);
								m_lineStr[lineSum - m_nowStartLine].showString[m_lineStr[lineSum - m_nowStartLine].showLength] = '\0';
							}else
							{//ʵ���ַ����ĳ���С����ʾ����
								memcpy(m_lineStr[lineSum - m_nowStartLine].showString,tempChar + m_lineStr[lineSum - m_nowStartLine].showStart,
									lineLength - m_lineStr[lineSum - m_nowStartLine].showStart);
								m_lineStr[lineSum - m_nowStartLine].showString[lineLength - m_lineStr[lineSum - m_nowStartLine].showStart] = '\0';
							}
							m_lineStr[lineSum - m_nowStartLine].nowText.setString(m_lineStr[lineSum - m_nowStartLine].showString);
						}else
						{
							//m_lineStr[lineSum - m_nowStartLine].isEnable = 0;
							m_lineStr[lineSum - m_nowStartLine].isEnable = 1;
							m_lineStr[lineSum - m_nowStartLine].nowText.setString(" ");
						}
						m_lineStr[lineSum - m_nowStartLine].lineEndOrder = i;
						m_lineStr[lineSum - m_nowStartLine].lineLength = m_lineStr[lineSum - m_nowStartLine].lineEndOrder - 
							m_lineStr[lineSum - m_nowStartLine].lineHeadorder + 1;
					}
					if(lineSum + 1 >= m_nowStartLine && lineSum < m_nowStartLine + m_nowShowLineSum - 1)
					{
						m_lineStr[lineSum - m_nowStartLine].lineHeadorder = i + 1;
					}
					lineSum ++;
					if(lineLength > maxLineLength)
					{//��¼��󳤶�
						maxLineLength = lineLength;
					}
					break;
				}
			}
		}else
		if(m_nowString[i] == '\n')
		{//����
			tempChar[lineLength] = m_nowString[i];	//��¼���з�
			if(lineSum >= m_nowStartLine && lineSum < m_nowStartLine + m_nowShowLineSum)
			{//�����ַ���������ʾ�ڴ�
				if(lineLength >= m_lineStr[lineSum - m_nowStartLine].showStart)
				{
					m_lineStr[lineSum - m_nowStartLine].isEnable = 1;
					if(lineLength - m_lineStr[lineSum - m_nowStartLine].showStart > m_lineStr[lineSum - m_nowStartLine].showLength)
					{//ʵ���ַ����ĳ��ȳ�����ʾ����
						memcpy(m_lineStr[lineSum - m_nowStartLine].showString,tempChar + m_lineStr[lineSum - m_nowStartLine].showStart,
							m_lineStr[lineSum - m_nowStartLine].showLength);
						m_lineStr[lineSum - m_nowStartLine].showString[m_lineStr[lineSum - m_nowStartLine].showLength] = '\0';
					}else
					{//ʵ���ַ����ĳ���С����ʾ����
						memcpy(m_lineStr[lineSum - m_nowStartLine].showString,tempChar + m_lineStr[lineSum - m_nowStartLine].showStart,
							lineLength - m_lineStr[lineSum - m_nowStartLine].showStart);
						m_lineStr[lineSum - m_nowStartLine].showString[lineLength - m_lineStr[lineSum - m_nowStartLine].showStart] = '\0';
					}
					m_lineStr[lineSum - m_nowStartLine].nowText.setString(m_lineStr[lineSum - m_nowStartLine].showString);
				}else
				{//���û����Ҫ��ʾ���ַ�������û���ַ���Ҫѡ��
					//m_lineStr[lineSum - m_nowStartLine].isEnable = 0;
					m_lineStr[lineSum - m_nowStartLine].isEnable = 1;
					m_lineStr[lineSum - m_nowStartLine].nowText.setString(" ");
				}
				m_lineStr[lineSum - m_nowStartLine].lineEndOrder = i;
				m_lineStr[lineSum - m_nowStartLine].lineLength = m_lineStr[lineSum - m_nowStartLine].lineEndOrder - 
					m_lineStr[lineSum - m_nowStartLine].lineHeadorder + 1;
				m_lineStr[lineSum - m_nowStartLine].isEnterEnd = 1;
			}
			if(lineSum + 1 >= m_nowStartLine && lineSum < m_nowStartLine + m_nowShowLineSum - 1)
			{
				m_lineStr[lineSum - m_nowStartLine + 1].lineHeadorder = i + 1;
			}
			lineSum ++;
			lineLength ++;
			if(lineLength > maxLineLength)
			{//��¼��󳤶�
				maxLineLength = lineLength;
			}
			lineLength = 0;
		}else
		{//����ǺϷ����ַ����¼
			tempChar[lineLength] = m_nowString[i];	//��¼���з�
			lineLength ++;
			//if(lineLength >= MAX_LINE_STRING_LENGTH - 1)
			//{//������������ʾ�ַ���ҲҪ����
			//	if(lineSum >= m_nowStartLine && lineSum < m_nowStartLine + m_nowShowLineSum)
			//	{//�����ַ���������ʾ�ڴ�
			//		if(lineLength >= m_lineStr[lineSum - m_nowStartLine].showStart)
			//		{
			//			m_lineStr[lineSum - m_nowStartLine].isEnable = 1;
			//			if(lineLength - m_lineStr[lineSum - m_nowStartLine].showStart > m_lineStr[lineSum - m_nowStartLine].showLength)
			//			{//ʵ���ַ����ĳ��ȳ�����ʾ����
			//				memcpy(m_lineStr[lineSum - m_nowStartLine].showString,tempChar + m_lineStr[lineSum - m_nowStartLine].showStart,
			//					m_lineStr[lineSum - m_nowStartLine].showLength);
			//				m_lineStr[lineSum - m_nowStartLine].showString[m_lineStr[lineSum - m_nowStartLine].showLength] = '\0';
			//			}else
			//			{//ʵ���ַ����ĳ���С����ʾ����
			//				memcpy(m_lineStr[lineSum - m_nowStartLine].showString,tempChar + m_lineStr[lineSum - m_nowStartLine].showStart,
			//					lineLength - m_lineStr[lineSum - m_nowStartLine].showStart);
			//				m_lineStr[lineSum - m_nowStartLine].showString[lineLength - m_lineStr[lineSum - m_nowStartLine].showStart] = '\0';
			//			}
			//			m_lineStr[lineSum - m_nowStartLine].nowText.setString(m_lineStr[lineSum - m_nowStartLine].showString);
			//		}else
			//		{
			//			m_lineStr[lineSum - m_nowStartLine].isEnable = 0;
			//		}
			//		m_lineStr[lineSum - m_nowStartLine].lineEndOrder = i;
			//		m_lineStr[lineSum - m_nowStartLine].lineLength = m_lineStr[lineSum - m_nowStartLine].lineEndOrder - 
			//			m_lineStr[lineSum - m_nowStartLine].lineHeadorder + 1;
			//		m_lineStr[lineSum - m_nowStartLine].isEnterEnd = 0;
			//	}
			//	if(lineSum + 1 >= m_nowStartLine && lineSum < m_nowStartLine + m_nowShowLineSum - 1)
			//	{
			//		m_lineStr[lineSum - m_nowStartLine + 1].lineHeadorder = i + 1;
			//	}
			//	lineSum ++;
			//	lineLength ++;
			//	if(lineLength > maxLineLength)
			//	{//��¼��󳤶�
			//		maxLineLength = lineLength;
			//	}
			//	lineLength = 0;
			//}
		}
	}
	m_nowMaxLineSum = lineSum;
	m_nowMaxLineLength = maxLineLength;
	if(m_nowMaxLineSum > m_nowShowLineSum)
	{//���ʵ�ʵ�����������ʾ���������������Ҫ��ʾ�����϶���
		m_needShowVSlider = 1;
		m_verticalSlider.setNowValue(m_nowStartLine);
		m_verticalSlider.setRange(m_nowMaxLineSum - m_nowShowLineSum,0.0f);
	}else
	{
		m_needShowVSlider = 0;
	}
	if(m_nowMaxLineLength > m_nowShowLineWidth)
	{//���ʵ�ʵ�����������ʾ���������������Ҫ��ʾ�����϶���
		m_needShowHSlider = 1;
		m_horizontalSlider.setNowValue(m_lineStr[0].showStart);
		m_horizontalSlider.setRange(m_nowMaxLineLength - m_nowShowLineWidth,0.0f);
	}else
	{
		m_needShowHSlider = 0;
	}*/
	//�����������������޸�
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		m_lineStr[i].isEnable = XFalse;
		m_lineStr[i].nowText.disClip();
	}
	if(m_nowStartLine == 0) m_lineStr[0].lineHeadorder = 0;//����Ǵ��ַ�����ͷ��ʾ������Ҫ��ʼ����һ�е�ͷλ�ַ�����ʼ

	int maxlineWidth = 0;
	int lineOrder = 0;	//��ǰ���к�
	if(strlen(m_nowString) > 0)
	{
		int index = 0;
		char tempChar[MAX_LINE_STRING_LENGTH];
		for(int i = 0;;++ i)
		{
			if(m_nowString[i] < 0)
			{//������
				++ i;
				continue;
			}else
			if(m_nowString[i] == '\n' || m_nowString[i] == '\0')
			{//�ַ�������
			/*	if(lineLength >= m_lineStr[lineSum - m_nowStartLine].showStart)
				{
					m_lineStr[lineSum - m_nowStartLine].isEnable = 1;
					if(lineLength - m_lineStr[lineSum - m_nowStartLine].showStart > m_lineStr[lineSum - m_nowStartLine].showLength)
					{//ʵ���ַ����ĳ��ȳ�����ʾ����
						memcpy(m_lineStr[lineSum - m_nowStartLine].showString,tempChar + m_lineStr[lineSum - m_nowStartLine].showStart,
							m_lineStr[lineSum - m_nowStartLine].showLength);
						m_lineStr[lineSum - m_nowStartLine].showString[m_lineStr[lineSum - m_nowStartLine].showLength] = '\0';
					}else
					{//ʵ���ַ����ĳ���С����ʾ����
						memcpy(m_lineStr[lineSum - m_nowStartLine].showString,tempChar + m_lineStr[lineSum - m_nowStartLine].showStart,
							lineLength - m_lineStr[lineSum - m_nowStartLine].showStart);
						m_lineStr[lineSum - m_nowStartLine].showString[lineLength - m_lineStr[lineSum - m_nowStartLine].showStart] = '\0';
					}
					m_lineStr[lineSum - m_nowStartLine].nowText.setString(m_lineStr[lineSum - m_nowStartLine].showString);
				}else
				{//���û����Ҫ��ʾ���ַ�������û���ַ���Ҫѡ��
					//m_lineStr[lineSum - m_nowStartLine].isEnable = 0;
					m_lineStr[lineSum - m_nowStartLine].isEnable = 1;
					m_lineStr[lineSum - m_nowStartLine].nowText.setString(" ");
				}*/
				if(lineOrder >= m_nowStartLine && lineOrder < m_nowStartLine + m_nowShowLineSum)
				{
					memcpy(tempChar,m_nowString + index,i - index);
					tempChar[i - index] = '\0';
					m_lineStr[lineOrder - m_nowStartLine].isEnable = XTrue;
					m_lineStr[lineOrder - m_nowStartLine].nowText.setString(tempChar);
					m_lineStr[lineOrder - m_nowStartLine].lineWidthPixel = (i - index + 1) * m_lineStr[lineOrder - m_nowStartLine].nowText.getTextSize().x * 0.5f;
					//m_lineStr[lineOrder - m_nowStartLine].lineWidthPixel = m_lineStr[lineOrder - m_nowStartLine].nowText.getMaxPixelWidth();
					if(m_lineStr[lineOrder - m_nowStartLine].lineWidthPixel > maxlineWidth)
					{
						maxlineWidth = m_lineStr[lineOrder - m_nowStartLine].lineWidthPixel;
					}
					m_lineStr[lineOrder - m_nowStartLine].nowText.setClipRect(m_nowStartPixels,0.0f,m_MaxLineWidthPixels + m_nowStartPixels,
						m_lineStr[lineOrder - m_nowStartLine].nowText.getTextSize().y);//�����ַ����ü�
					//����λ��Ҳ��Ҫ����Ӧ�ĸı�
					//����δ��ɣ�
					m_lineStr[lineOrder - m_nowStartLine].nowText.setPosition(m_position.x + (m_mouseRect.left - m_nowStartPixels) * m_size.x,
						m_position.y + m_mouseRect.top * m_size.y + m_textHeight * (lineOrder - m_nowStartLine));

					m_lineStr[lineOrder - m_nowStartLine].lineHeadorder = index;
					m_lineStr[lineOrder - m_nowStartLine].lineEndOrder = i;
					//m_lineStr[lineOrder - m_nowStartLine].lineLength = m_lineStr[lineOrder - m_nowStartLine].lineEndOrder - 
					//	m_lineStr[lineOrder - m_nowStartLine].lineHeadorder + 1;
				//	if(m_nowString[i] == '\n') m_lineStr[lineOrder - m_nowStartLine].isEnterEnd = 1;
				//	else m_lineStr[lineOrder - m_nowStartLine].isEnterEnd = 0;
				}
			//	if(lineOrder + 1 >= m_nowStartLine && lineOrder < m_nowStartLine + m_nowShowLineSum - 1)
			//	{
			//		m_lineStr[lineOrder - m_nowStartLine + 1].lineHeadorder = i + 1;
			//	}

				if(m_nowString[i] == '\0') break;
				++lineOrder;
				index = i + 1;
			}else
			{//��ͨ�ַ�
			}
		}
	}
	//�ж��Ƿ���Ҫ��ʾ��ֱ�϶���
	m_nowMaxLineSum = lineOrder;
	if(m_nowMaxLineSum > m_nowShowLineSum)
	{//���ʵ�ʵ�����������ʾ���������������Ҫ��ʾ�����϶���
		m_needShowVSlider = XTrue;
		m_verticalSlider.setNowValue(m_nowStartLine);
		m_verticalSlider.setRange(m_nowMaxLineSum - m_nowShowLineSum,0.0f);
	}else
	{
		m_needShowVSlider = XFalse;
	}
	//�ж��Ƿ���Ҫ��ʾˮƽ�϶���
	m_nowMaxLineWidthPixels = maxlineWidth;
	if(m_nowMaxLineWidthPixels > m_MaxLineWidthPixels)
	{//���ʵ�ʵ�����������ʾ���������������Ҫ��ʾ�����϶���
		m_needShowHSlider = XTrue;
		m_horizontalSlider.setNowValue(m_nowStartPixels);
		m_horizontalSlider.setRange(m_nowMaxLineWidthPixels - m_MaxLineWidthPixels,0.0f);
	}else
	{
		m_needShowHSlider = XFalse;
	}
}
void _XMultiText::draw()
{
	if(!m_isInited) return ;	//���û�г�ʼ��ֱ���˳�
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
			//m_spriteBackGround.draw(m_mutiEditDisable);	//�����Ч����ʾ��Ч,����ʾ�������
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
			//m_spriteBackGround.draw(m_mutiEditNormal); //�����Ч����ʾ��Ч,��ʾ�������
		}
		//if(m_haveSelect != 0) m_spriteSelect.draw(&(m_mutiEditSelect->m_texture));	//��ʾѡ��ķ�Χ
		//��ʾ��ǰ������ַ���
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			if(m_lineStr[i].isEnable)
			{
				if(m_lineStr[i].haveAllSelect)
				{
					//m_lineStr[i].spriteSelect.draw(m_mutiEditSelect);
					drawFillBoxEx(m_position + _XVector2(m_mouseRect.left * m_size.x,m_mouseRect.top * m_size.y + i * m_textHeight),
					_XVector2((m_mouseRect.getWidth() - m_verticalSlider.getMouseRectWidth()) * m_size.x,
					m_textHeight),0.85f,0.85f,0.85f);
				}
				m_lineStr[i].nowText.draw();
			}
		}
		//��ʾ������
		if(m_needShowVSlider) m_verticalSlider.draw();
		if(m_needShowHSlider) m_horizontalSlider.draw();
		//if(m_mutiEditUpon != NULL) m_spriteBackGround.draw(m_mutiEditUpon);
	}else
	{
		if(!m_isEnable) m_spriteBackGround.draw(m_mutiEditDisable);	//�����Ч����ʾ��Ч,����ʾ�������
		else m_spriteBackGround.draw(m_mutiEditNormal); //�����Ч����ʾ��Ч,��ʾ�������
		//if(m_haveSelect != 0) m_spriteSelect.draw(&(m_mutiEditSelect->m_texture));	//��ʾѡ��ķ�Χ
		//��ʾ��ǰ������ַ���
		for(int i = 0;i < m_nowShowLineSum;++ i)
		{
			if(m_lineStr[i].isEnable)
			{
				if(m_lineStr[i].haveAllSelect)
				{
					m_lineStr[i].spriteSelect.draw(m_mutiEditSelect);
				}
				m_lineStr[i].nowText.draw();
			}
		}
		//��ʾ������
		if(m_needShowVSlider) m_verticalSlider.draw();
		if(m_needShowHSlider) m_horizontalSlider.draw();
		if(m_mutiEditUpon != NULL) m_spriteBackGround.draw(m_mutiEditUpon);
	}
}
_XBool _XMultiText::canGetFocus(float x,float y)
{
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}
_XBool _XMultiText::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(!m_isInited) return XFalse;	//���û�г�ʼ��ֱ���˳�
	if(!m_isActive) return XFalse;		//û�м���Ŀؼ������տ���
	if(!m_isVisiable) return XFalse;	//������ɼ�ֱ���˳�
	if(!m_isEnable) return XFalse;		//�����Ч��ֱ���˳�
	
	if(m_needShowVSlider) m_verticalSlider.mouseProc(x,y,mouseState);
	if(m_needShowHSlider) m_horizontalSlider.mouseProc(x,y,mouseState);
	//�ж�������Ƿ��ڷ�Χ��
	if(mouseState == MOUSE_LEFT_BUTTON_UP && m_nowMouseRect.isInRect(x,y))
	{//��ѡ��Ч�������ѡ����ֵ
		int temp = (y - m_nowMouseRect.top) / m_textHeight;	//���㵱ǰѡ�������
		if(temp < 0) temp = 0;
		if(temp >= m_nowShowLineSum) temp = m_nowShowLineSum - 1;
		if(temp + m_nowStartLine < m_nowMaxLineSum)
		{
			if(m_haveSelect && m_nowSelectLineOrder != temp + m_nowStartLine)
			{//��������Ҫ�޳�ԭ�е�ѡ������ֻ����һ������ѡ��״̬
				m_lineStr[m_nowSelectLineOrder - m_nowStartLine].haveAllSelect = 0;	//ע����������ʾһ���б�ѡ�У����ԣ�����Ҫ���ͷ��β
			}
			if(m_haveSelect && m_nowSelectLineOrder == temp + m_nowStartLine)
			{//����ظ�ѡ���������
			}else
			{
				m_haveSelect = XTrue;
				m_nowSelectLineOrder = temp + m_nowStartLine;
				if(m_funSelect != NULL) m_funSelect(m_pClass,getControlID());
				//�����ѡ����
				m_lineStr[temp].haveAllSelect = XTrue;	//ע����������ʾһ���б�ѡ�У����ԣ�����Ҫ���ͷ��β
				//����ѡȡ��λ��
				if(!m_withoutTex)
				{
					m_lineStr[temp].spriteSelect.setPosition(m_nowMouseRect.left, m_nowMouseRect.top + m_textHeight * temp);
					m_lineStr[temp].spriteSelect.setSize(m_nowMouseRect.getWidth() / m_mutiEditSelect->textureSize.x,
						m_textHeight / m_mutiEditSelect->textureSize.y);
				}
			}
		}
	}
	return XTrue;
}
void _XMultiText::setPosition(float x,float y)
{
	m_position.set(x,y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);	//��ǰ�������Ӧ��Χ

	if(!m_withoutTex) m_spriteBackGround.setPosition(m_position);
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		m_lineStr[i].nowText.setPosition(m_position.x + (m_mouseRect.left - m_nowStartPixels) * m_size.x,
			m_position.y + m_mouseRect.top * m_size.y + m_textHeight * i);
	}
	if(m_haveSelect && !m_withoutTex)
	{//����ѡȡ
		int temp = m_nowSelectLineOrder - m_nowStartLine;
		m_lineStr[temp].spriteSelect.setPosition(m_nowMouseRect.left,m_nowMouseRect.top + m_textHeight * temp);
	}
}
void _XMultiText::setSize(float x,float y)
{
	if(x <= 0 && y <= 0) return;
	m_size.set(x,y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);	//��ǰ�������Ӧ��Χ
	if(!m_withoutTex)
	{
		m_spriteBackGround.setPosition(m_position);
		m_spriteBackGround.setSize(m_size);
	}
	m_verticalSlider.setPosition(m_position.x + (m_mouseRect.right - m_verticalSlider.getMouseRectWidth()) * m_size.x,
		m_position.y + m_mouseRect.top * m_size.y);
	m_verticalSlider.setSize(m_size);
	m_horizontalSlider.setPosition(m_position.x + m_mouseRect.left * m_size.x,
		m_position.y + (m_mouseRect.bottom - m_horizontalSlider.getMouseRectHeight()) * m_size.y);
	m_horizontalSlider.setSize(m_size);

	m_lineStr[0].nowText.setSize(m_size);
	m_textWidth = m_lineStr[0].nowText.getTextSize().x * m_lineStr[0].nowText.getSize().x * 0.5f;
	m_textHeight = m_lineStr[0].nowText.getTextSize().y * m_lineStr[0].nowText.getSize().y;
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		m_lineStr[i].nowText.setPosition(m_position.x + (m_mouseRect.left - m_nowStartPixels) * m_size.x,
			m_position.y + m_mouseRect.top * m_size.y + m_textHeight * i);
		m_lineStr[i].nowText.setSize(m_size);
	}
	if(m_haveSelect && !m_withoutTex)
	{//����ѡȡ
		int temp = m_nowSelectLineOrder - m_nowStartLine;
		m_lineStr[temp].spriteSelect.setPosition(m_nowMouseRect.left,m_nowMouseRect.top + m_textHeight * temp);
		m_lineStr[temp].spriteSelect.setSize(m_nowMouseRect.getWidth() / m_mutiEditSelect->textureSize.x,
			m_textHeight / m_mutiEditSelect->textureSize.y);
	}
}
void _XMultiText::checkStr()			//����ַ����ĵ��п���Ƿ��������
{
	int lineLength = 0;		//��ǰ���п�
	int strLength = strlen(m_nowString);
	char *tempStr = createArrayMem<char>(MAX_STRING_LENGTH);
	if(tempStr == NULL) return;
	for(int i = 0;;++ i)
	{
		if(m_nowString[i] < 0)
		{
			++ i;
			lineLength += 2;
		}else
		{
			if(m_nowString[i] == '\0')
			{
				lineLength = 0;
				break;
			}else
			if(m_nowString[i] == '\n')
			{
				lineLength = 0;
			}else
			{
				++ lineLength;
			}
		}
		if(lineLength >= MAX_LINE_STRING_LENGTH - 2)
		{//�������ȵ���ʾ������ֱ�Ӳ��뻻�з�
			++ strLength;
			if(strLength >= MAX_STRING_LENGTH)
			{//�ַ����ĳ��ȳ�����ʾ����
				strcpy(tempStr,m_nowString + i);
				tempStr[MAX_STRING_LENGTH - i -1] = '\0';
			}else
			{
				strcpy(tempStr,m_nowString + i);
			}
			m_nowString[i] = '\n';
			strcpy(m_nowString + i + 1,tempStr);
			lineLength = 0;
		}
	}
	XDELETE_ARRAY(tempStr);
}
void _XMultiText::setString(const char *str)
{
	if(!m_isInited) return;	//����Ҫ��ʼ��
	if(str == NULL) return;		//��ָ����Ҫ�˳�
	//�����ַ���
	strcpy(m_nowString,str);
	//��������������ַ�������Ҫ����һ�е������ʾ�������������ں����ͳһ����
	checkStr();

	//�����ַ���
	m_nowStartLine = 0;
	updateLineStr();

	m_haveSelect = XFalse;		//�Ƿ����ַ�������ѡ��
	m_nowSelectLineOrder = -1;
	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		m_lineStr[i].haveAllSelect = XFalse;
	}
}
void _XMultiText::addString(const char *str)
{
	if(!m_isInited) return;		//���û�г�ʼ��ֱ���˳�
	if(str == NULL) return;
	if(strlen(str) == 0) return;
	if(strlen(str) + strlen(m_nowString) >= MAX_STRING_LENGTH) return;
	strcat(m_nowString,str);
	checkStr();
	updateLineStr();
}
void _XMultiText::addALine(const char *str)		//���ַ���������ַ���
{
	if(!m_isInited) return;		//���û�г�ʼ��ֱ���˳�
	if(str == NULL) return;
	if(strlen(str) + strlen(m_nowString) + 1 >= MAX_STRING_LENGTH) return;
	if(strlen(str) == 0) return;
	if(m_nowString[strlen(m_nowString) - 1] != '\n')
	{//���������ַ��������Իس���������������뻻�з�
		if(strlen(str) + strlen(m_nowString) + 2 >= MAX_STRING_LENGTH) return;
		strcat(m_nowString,"\n");
	}
	strcat(m_nowString,str);

	checkStr();
	if(m_needShowVSlider && m_verticalSlider.getNowValue() == m_verticalSlider.getMaxValue())
	{
		updateLineStr();
		m_verticalSlider.setNowValue(m_verticalSlider.getMaxValue());
	}else
	{
		updateLineStr();
	}
}
_XBool _XMultiText::setACopy(const _XMultiText &temp)
{
	if(& temp == this) return XTrue;	//��ֹ����ֵ
	if(!temp.m_isInited) return XFalse;
	
	//not finished
	if(m_isInited) release();
	if(!_XControlBasic::setACopy(temp)) return XFalse;
	if(!m_isInited)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_MUTITEXT);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}

	m_isInited = temp.m_isInited;				//�Ƿ��ʼ��
	if(m_resInfo != NULL) _XResourceManager::GetInstance().releaseResource(m_resInfo);
	m_resInfo = _XResourceMng.copyResource(temp.m_resInfo);
	m_withoutTex = temp.m_withoutTex;

	m_funSelect = temp.m_funSelect;
	m_pClass = temp.m_pClass;

	m_needShowVSlider = temp.m_needShowVSlider;			//�Ƿ���Ҫ��ʾ��ֱ������
	m_verticalSlider.setACopy(temp.m_verticalSlider);		//��ֱ������
	_XControlManager::GetInstance().decreaseAObject(&m_verticalSlider);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_verticalSlider);
#endif
	m_verticalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiTextValueChangeV,funMutiTextValueChangeMoveV,this);
	m_needShowHSlider = temp.m_needShowHSlider;			//�Ƿ���Ҫ��ʾˮƽ������
	m_horizontalSlider.setACopy(temp.m_horizontalSlider);	//ˮƽ������
	_XControlManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_horizontalSlider);
#endif
	m_horizontalSlider.setCallbackFun(NULL,NULL,NULL,NULL,NULL,funMutiTextValueChangeH,funMutiTextValueChangeMoveH,this);

	m_nowString = createArrayMem<char>(MAX_STRING_LENGTH);
	if(m_nowString == NULL) return XFalse;

	strcpy(m_nowString,temp.m_nowString);

	m_mutiEditNormal = temp.m_mutiEditNormal;		//�������ͨ״̬
	m_mutiEditDisable = temp.m_mutiEditDisable;		//�������Ч״̬
	m_mutiEditSelect = temp.m_mutiEditSelect;		//�����Ƭѡ��ɫ
	m_mutiEditUpon = temp.m_mutiEditUpon;

	m_spriteBackGround.setACopy(temp.m_spriteBackGround);	//������ʾ�����ı�����ͼ
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackGround);
#endif
	//m_textPosition = temp.m_textPosition;			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize = temp.m_textSize;				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����

	m_haveSelect = temp.m_haveSelect;		//�Ƿ����ַ�������ѡ��
	m_nowSelectLineOrder = temp.m_nowSelectLineOrder;	//��ǰ�ĵ��б�ѡ�������һ��

	m_nowStartLine = temp.m_nowStartLine;		//��ǰ��ʾ�ĵ�һ�����ܵĵڼ���
	m_nowShowLineSum = temp.m_nowShowLineSum;		//��ǰ������ʾ������
	//m_nowShowLineWidth = temp.m_nowShowLineWidth;		//��ǰһ�п�����ʾ�Ŀ��
	m_nowMaxLineSum = temp.m_nowMaxLineSum;	//��ǰ�ַ�����ʵ�ʵ�����
	//m_nowMaxLineLength = temp.m_nowMaxLineLength;	//��ǰ�ַ�����ʵ�ʵ��п�

	m_nowStartPixels = temp.m_nowStartPixels;
	m_nowMaxLineWidthPixels = temp.m_nowMaxLineWidthPixels;
	m_MaxLineWidthPixels = temp.m_MaxLineWidthPixels;

	m_lineStr = createArrayMem<_XMultiTextLineString>(m_nowShowLineSum);
	if(m_lineStr == NULL) return XFalse;

	for(int i = 0;i < m_nowShowLineSum;++ i)
	{
		m_lineStr[i].showString = createArrayMem<char>(MAX_LINE_STRING_LENGTH + 1);
		//m_lineStr[i].showString = createArrayMem<char>(m_nowShowLineWidth + 1);
		if(m_lineStr[i].showString == NULL) return XFalse;

		strcpy(m_lineStr[i].showString,temp.m_lineStr[i].showString);
		m_lineStr[i].isEnable = temp.m_lineStr[i].isEnable;				//��һ���Ƿ�Ҫ��ʾ���Ƿ���Ч
	//	m_lineStr[i].showStart = temp.m_lineStr[i].showStart;				//�����ַ�����ʾ���ֵ�ͷ�ڵ�ǰ�е�ƫ��
	//	m_lineStr[i].showLength = temp.m_lineStr[i].showLength;				//��ʾ�ַ����ĳ���
		m_lineStr[i].nowText.setACopy(temp.m_lineStr[i].nowText);		//������ʾ��һ���ַ���������
		//m_lineStr[i].nowText.setMaxStrLen(MAX_LINE_STRING_LENGTH);
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_lineStr[i].nowText));
#endif
		m_lineStr[i].lineHeadorder = temp.m_lineStr[i].lineHeadorder;			//��һ���ַ���ȫ���е�ͷλ��
		m_lineStr[i].lineEndOrder = temp.m_lineStr[i].lineEndOrder;			//��һ���ַ���ȫ���е�βλ��
		//m_lineStr[i].lineLength = temp.m_lineStr[i].lineLength;				//��һ���ַ����ĳ���
		//m_lineStr[i].isEnterEnd = temp.m_lineStr[i].isEnterEnd;			//�Ƿ���Ϊ���ж�������ʾ1����������Ϊ��ʾ�ַ����ĳ������ƶ�����0
		m_lineStr[i].spriteSelect.setACopy(temp.m_lineStr[i].spriteSelect);		//�ַ�����ʾѡ�񱳾���ɫ�ľ���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(&(m_lineStr[i].spriteSelect));
#endif
		m_lineStr[i].haveAllSelect = temp.m_lineStr[i].haveAllSelect;			//�������Ƿ�汻����ѡ��
	}

//	m_funInputChenge = temp.m_funInputChenge;		//�������ݷ����ı��ʱ�����
//	m_funInputOver = temp.m_funInputOver;		//ȷ���������֮���ʱ�����
//	m_pClass = temp.m_pClass;

	m_timer = temp.m_timer;	//���ǲ��������˸ʱʹ�õ�ʱ���ǣ����ʱ��ʹ�ò�׼ȷ�ļ�ʱ��ʽ
	m_textColor = temp.m_textColor;
	m_textWidth = temp.m_textWidth;	//���ǲ��������˸ʱʹ�õ�ʱ���ǣ����ʱ��ʹ�ò�׼ȷ�ļ�ʱ��ʽ
	m_textHeight = temp.m_textHeight;	//���ǲ��������˸ʱʹ�õ�ʱ���ǣ����ʱ��ʹ�ò�׼ȷ�ļ�ʱ��ʽ
	return XTrue;
}
_XBool _XMultiText::exportData(const char *fileName)		//���ݵ���
{
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	FILE *fp;
	if(fileName == NULL)
	{
		char fileNameT[] = "XMultiText_xxx.txt";
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
	if(m_nowString != NULL)
	{
		//fprintf(fp,"%s",m_nowString);
		fwrite(m_nowString,sizeof(char),strlen(m_nowString) + 1,fp);
	}
	fclose(fp);
	return XTrue;
}
_XBool _XMultiText::importData(const char *fileName)		//���ݵ���
{
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	FILE *fp;
	if(fileName == NULL)
	{
		char fileNameT[] = "XMultiText_xxx.txt";
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
	if(m_nowString != NULL)
	{
		//fscanf(fp,"%s",m_nowString);
		//strcpy(m_nowString,fp->_base);
		fseek(fp, 0, SEEK_END);
		int count = ftell(fp);
		rewind(fp);
		fread(m_nowString,sizeof(char),count,fp);
		setString(m_nowString);
		fclose(fp);
		return XTrue;
	}else
	{
		fclose(fp);
		return XFalse;
	}
}
_XBool _XMultiText::deleteSelectLine()		//ɾ����ǰѡ���һ��
{
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	if(!m_haveSelect) return XFalse;
	//�ҵ�ָ����һ��
	int lineSum = 0;
	int lineStart = 0;
	int lineEnd = 0;
	char *tempStr = createArrayMem<char>(MAX_STRING_LENGTH);
	if(tempStr == NULL) return XFalse;
	memset(tempStr,0,MAX_STRING_LENGTH);
	if(m_nowSelectLineOrder == 0)
	{//��һ��
		for(int i = 0;;++ i)
		{	
			if(m_nowString[i] < 0) 
			{
				++ i;
				continue;
			}
			if(m_nowString[i] == '\0')
			{//ֻ��һ��
				setString("");
				break;
			}
			if(m_nowString[i] == '\n')
			{
				lineEnd = i;
				strcpy(tempStr,m_nowString + i + 1);
				setString(tempStr);
				break;
			}
		/*	if(i >= MAX_LINE_STRING_LENGTH - 1)
			{//����һ�еĳ���Ҳ��Ҫ��������(�����Ľ�����û���н�����'\0')
				lineEnd = i;
				strcpy(tempStr,m_nowString + i);
				setString(tempStr);
				break;
			}*/
		}
	}else
	if(m_nowSelectLineOrder == m_nowMaxLineSum - 1)
	{//���һ��
	//	int lineLength = 0;
		for(int i = 0;;++ i)
		{
			if(m_nowString[i] < 0)
			{//����
				++ i;
				continue;
			}
			if(m_nowString[i] == '\0')
			{
				break;
			}else
			if(m_nowString[i] == '\n')
			{
				lineSum++;
	//			lineLength = 0;
				if(lineSum == m_nowSelectLineOrder)
				{
					m_nowString[i] = '\0';
					strcpy(tempStr,m_nowString);
					setString(tempStr);
					if(m_nowMaxLineSum - m_nowShowLineSum < 0)
					{
						setStartLine(0);
						updateLineStr();
					}else
					{
						setStartLine(m_nowMaxLineSum - m_nowShowLineSum);
						updateLineStr();
					}
					break;
				}
			}
	/*		else
			{
				lineLength ++;
				if(lineLength >= MAX_LINE_STRING_LENGTH - 1)
				{//����һ����ʾ�����ֵҲ��Ҫ����
					lineSum++;
					lineLength = 0;
					if(lineSum == m_nowSelectLineOrder)
					{
						m_nowString[i + 1] = '\0';
						strcpy(tempStr,m_nowString);
						setString(tempStr);
						if(m_nowMaxLineSum - m_nowShowLineSum < 0)
						{
							setStartLine(0);
							updateLineStr();
						}else
						{
							setStartLine(m_nowMaxLineSum - m_nowShowLineSum);
							updateLineStr();
						}
						break;
					}
				}
			}*/
		}
	}else
	{
	//	int lineLength = 0;
		for(int i = 0;;++ i)
		{	
			if(m_nowString[i] < 0)
			{
				++ i;
				continue;
			}
			if(m_nowString[i] == '\0')
			{
				//������
				if(lineSum == m_nowSelectLineOrder)
				{
					m_nowString[lineStart] = '\0';
					strcpy(tempStr,m_nowString);
					setString(tempStr);
					if(m_nowMaxLineSum - m_nowShowLineSum < 0)
					{
						setStartLine(0);
						updateLineStr();
					}else
					{
						setStartLine(m_nowMaxLineSum - m_nowShowLineSum);
						updateLineStr();
					}
					break;
				}
			}else
			if(m_nowString[i] == '\n')
			{
				lineSum ++;
	//			lineLength = 0;
				if(lineSum == m_nowSelectLineOrder)
				{//��ͷ
					lineStart = i + 1;
				}
				if(lineSum == m_nowSelectLineOrder + 1)
				{
					m_nowString[lineStart] = '\0';
					int tempStartLine = m_nowStartLine;
					strcpy(tempStr,m_nowString);
					strcpy(tempStr + strlen(m_nowString),&m_nowString[i] + 1);
					setString(tempStr);
					if(m_nowMaxLineSum - m_nowShowLineSum < 0)
					{
						setStartLine(0);
						updateLineStr();
					}else
					if(tempStartLine + m_nowShowLineSum <= m_nowMaxLineSum)
					{
						setStartLine(tempStartLine);
						updateLineStr();
					}else
					{
						setStartLine(m_nowMaxLineSum - m_nowShowLineSum);
						updateLineStr();
					}
					break;
				}
			}
	/*		else
			{
				lineLength ++;
				if(lineLength >= MAX_LINE_STRING_LENGTH - 1)
				{//����һ����ʾ�����ֵҲ��Ҫ����
					lineSum ++;
					lineLength = 0;
					if(lineSum == m_nowSelectLineOrder)
					{//��ͷ
						lineStart = i + 1;
					}
					if(lineSum == m_nowSelectLineOrder + 1)
					{
						m_nowString[lineStart] = '\0';
						int tempStartLine = m_nowStartLine;
						strcpy(tempStr,m_nowString);
						//��Ϊ�ǳ����ַ����������������ﲻ��Ҫ����\n����������
						//strcpy(tempStr + strlen(m_nowString),&m_nowString[i] + 1);
						strcpy(tempStr + strlen(m_nowString),m_nowString + lineStart + MAX_LINE_STRING_LENGTH - 1);
						setString(tempStr);
						if(m_nowMaxLineSum - m_nowShowLineSum < 0)
						{
							setStartLine(0);
							updateLineStr();
						}else
						if(tempStartLine + m_nowShowLineSum <= m_nowMaxLineSum)
						{
							setStartLine(tempStartLine);
							updateLineStr();
						}else
						{
							setStartLine(m_nowMaxLineSum - m_nowShowLineSum);
							updateLineStr();
						}
						break;
					}
				}
			}*/
		}
	}
	XDELETE_ARRAY(tempStr);
	return XTrue;
}
_XBool _XMultiText::moveUpSelectLine()	//��ѡ���һ������
{
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	if(!m_haveSelect) return XFalse;
	if(!moveUpLine(m_nowSelectLineOrder))
	{//�ƶ�ʧ��
		return XFalse;
	}else
	{
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].haveAllSelect = XFalse;
		m_nowSelectLineOrder --;
		if(m_nowSelectLineOrder < m_nowStartLine)
		{
			m_nowStartLine = m_nowSelectLineOrder;
			updateLineStr();
		}
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].haveAllSelect = XTrue;
		if(!m_withoutTex)
		{
			m_lineStr[m_nowSelectLineOrder - m_nowStartLine].spriteSelect.setPosition(m_nowMouseRect.left,
				m_nowMouseRect.top + m_textHeight * (m_nowSelectLineOrder - m_nowStartLine));
			m_lineStr[m_nowSelectLineOrder - m_nowStartLine].spriteSelect.setSize(m_nowMouseRect.getWidth() / m_mutiEditSelect->textureSize.x,
							m_textHeight / m_mutiEditSelect->textureSize.y);
		}
		return XTrue;
	}
}
_XBool _XMultiText::moveUpLine(int order)	//��ĳһ������
{
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	if(!m_haveSelect) return XFalse;
	if(order <= 0 || order >= m_nowMaxLineSum) return XFalse;	//�Ƿ���ֵ���˳�
	int upLineStart = 0;	//��һ�п�ʼ��λ��
	int thisLineStart = 0;	//��һ�п�ʼ��λ��
	int nextLineStart = 0;	//��һ�н�����λ��
	int lineSum = 0;		//��ǰ���б�
//	int lineLength = 0;		//��ǰ���п�
	char tempChar0[MAX_LINE_STRING_LENGTH];
	char tempChar1[MAX_LINE_STRING_LENGTH];
	for(int i = 0;;++ i)
	{
		if(m_nowString[i] < 0)
		{
			++ i;
			continue;
		}
		if(m_nowString[i] == '\0')
		{
			lineSum++;
			if(lineSum == order - 1)
			{
				upLineStart = i + 1;	//��һ�е���ͷ
			}else
			if(lineSum == order)
			{
				thisLineStart = i + 1;	//��ǰ�е���ͷ
			}else
			if(lineSum == order + 1)
			{//���ﲻ��Ҫ����\0����ֽڲ�Ȼ�ͻ������
				//nextLineEnd = i + 1;		//��ǰ�е���β
				nextLineStart = i;		//��ǰ�е���β
				//�����ַ��������˳�
				memcpy(tempChar0,m_nowString + upLineStart,thisLineStart - upLineStart);
				tempChar0[thisLineStart - upLineStart - 1] = '\0';	//�����Ļ��з��ĳƽ�����
				memcpy(tempChar1,m_nowString + thisLineStart,nextLineStart - thisLineStart);
				tempChar1[nextLineStart - thisLineStart] = '\n';	//������һ�����з�
				memcpy(m_nowString + upLineStart,tempChar1,nextLineStart - thisLineStart + 1);
				memcpy(m_nowString + upLineStart + nextLineStart - thisLineStart + 1,tempChar0,thisLineStart - upLineStart);
				updateLineStr();
				break;
			}
	//		lineLength = 0;
		}else
		if(m_nowString[i] == '\n')
		{
			lineSum++;
			if(lineSum == order - 1)
			{
				upLineStart = i + 1;	//��һ�е���ͷ
			}else
			if(lineSum == order)
			{
				thisLineStart = i + 1;	//��ǰ�е���ͷ
			}else
			if(lineSum == order + 1)
			{
				nextLineStart = i + 1;		//��ǰ�е���β
				//�����ַ��������˳�
				memcpy(tempChar0,m_nowString + upLineStart,thisLineStart - upLineStart);
				tempChar0[thisLineStart - upLineStart] = '\0';
				memcpy(tempChar1,m_nowString + thisLineStart,nextLineStart - thisLineStart);
				tempChar1[nextLineStart - thisLineStart] = '\0';
				memcpy(m_nowString + upLineStart,tempChar1,nextLineStart - thisLineStart);
				memcpy(m_nowString + upLineStart + nextLineStart - thisLineStart,tempChar0,thisLineStart - upLineStart);
				updateLineStr();
				break;
			}
	//		lineLength = 0;
		}
	/*	else
		{
			lineLength ++;
			if(lineLength >= MAX_LINE_STRING_LENGTH - 1)
			{
				lineSum ++;
				if(lineSum == order - 1)
				{
					upLineStart = i + 1;	//��һ�е���ͷ
				}else
				if(lineSum == order)
				{
					thisLineStart = i + 1;	//��ǰ�е���ͷ
				}else
				if(lineSum == order + 1)
				{
					nextLineStart = i + 1;		//��ǰ�е���β
					//�����ַ��������˳�
					memcpy(tempChar0,m_nowString + upLineStart,thisLineStart - upLineStart);
					tempChar0[thisLineStart - upLineStart] = '\0';
					memcpy(tempChar1,m_nowString + thisLineStart,nextLineStart - thisLineStart);
					tempChar1[nextLineStart - thisLineStart] = '\0';
					memcpy(m_nowString + upLineStart,tempChar1,nextLineStart - thisLineStart);
					memcpy(m_nowString + upLineStart + nextLineStart - thisLineStart,tempChar0,thisLineStart - upLineStart);
					updateLineStr();
					break;
				}
				lineLength = 0;
			}
		}*/
	}
	return XTrue;
}
_XBool _XMultiText::moveDownSelectLine()	//��ѡ���һ������
{
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	if(!m_haveSelect) return XFalse;
	if(!moveDownLine(m_nowSelectLineOrder))
	{//�ƶ�ʧ��
		return XFalse;
	}else
	{
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].haveAllSelect = XFalse;
		m_nowSelectLineOrder ++;
		if(m_nowSelectLineOrder >= m_nowStartLine + m_nowShowLineSum)
		{
			m_nowStartLine ++;
			updateLineStr();
		}
		m_lineStr[m_nowSelectLineOrder - m_nowStartLine].haveAllSelect = XTrue;
		if(!m_withoutTex)
		{
			m_lineStr[m_nowSelectLineOrder - m_nowStartLine].spriteSelect.setPosition(m_nowMouseRect.left,
				m_nowMouseRect.top + m_textHeight * (m_nowSelectLineOrder - m_nowStartLine));
			m_lineStr[m_nowSelectLineOrder - m_nowStartLine].spriteSelect.setSize(m_nowMouseRect.getWidth() / m_mutiEditSelect->textureSize.x,
							m_textHeight / m_mutiEditSelect->textureSize.y);
		}
		return XTrue;
	}
}
_XBool _XMultiText::moveDownLine(int order)	//��ĳһ������
{
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	if(!m_haveSelect) return XFalse;
	if(order < 0 || order >= m_nowMaxLineSum - 1) return XFalse;	//�Ƿ���ֵ���˳�
	int thisLineStart = 0;	//��һ�п�ʼ��λ��
	int nextLineStart = 0;	//��һ�п�ʼ��λ��
	int next2LineStart = 0;	//��һ�н�����λ��
	int lineSum = 0;		//��ǰ���б�
//	int lineLength = 0;		//��ǰ���п�
	char tempChar0[MAX_LINE_STRING_LENGTH];
	char tempChar1[MAX_LINE_STRING_LENGTH];
	for(int i = 0;;++ i)
	{
		if(m_nowString[i] < 0)
		{
			++ i;
			continue;
		}
		if(m_nowString[i] == '\0')
		{
			lineSum++;
			if(lineSum == order)
			{
				thisLineStart = i + 1;	//��ǰ�е���ͷ
			}else
			if(lineSum == order + 1)
			{
				nextLineStart = i + 1;		//��ǰ�е���β
			}else
			if(lineSum == order + 2)
			{//���ﲻ��Ҫ����\0����ֽڲ�Ȼ�ͻ������
				//next2LineStart = i + 1;		//��ǰ�е���β
				next2LineStart = i;		//��ǰ�е���β
				//�����ַ��������˳�
			/*	if(nextLineStart - thisLineStart >= MAX_LINE_STRING_LENGTH - 1)
				{
					memcpy(tempChar0,m_nowString + thisLineStart,nextLineStart - thisLineStart);
					tempChar0[nextLineStart - thisLineStart] = '\0';
					memcpy(tempChar1,m_nowString + nextLineStart,next2LineStart - nextLineStart);
					tempChar1[next2LineStart - nextLineStart] = '\n';
					memcpy(m_nowString + thisLineStart,tempChar1,next2LineStart - nextLineStart + 1);
					memcpy(m_nowString + thisLineStart + next2LineStart - nextLineStart + 1,tempChar0,nextLineStart - thisLineStart + 1);
				}else*/
				{
					memcpy(tempChar0,m_nowString + thisLineStart,nextLineStart - thisLineStart);
					tempChar0[nextLineStart - thisLineStart - 1] = '\0';
					memcpy(tempChar1,m_nowString + nextLineStart,next2LineStart - nextLineStart);
					tempChar1[next2LineStart - nextLineStart] = '\n';
					memcpy(m_nowString + thisLineStart,tempChar1,next2LineStart - nextLineStart + 1);
					memcpy(m_nowString + thisLineStart + next2LineStart - nextLineStart + 1,tempChar0,nextLineStart - thisLineStart);
				}
				updateLineStr();
				break;
			}
	//		lineLength = 0;
		}else
		if(m_nowString[i] == '\n')
		{
			lineSum++;
			if(lineSum == order)
			{
				thisLineStart = i + 1;	//��ǰ�е���ͷ
			}else
			if(lineSum == order + 1)
			{
				nextLineStart = i + 1;		//��ǰ�е���β
			}else
			if(lineSum == order + 2)
			{
				next2LineStart = i + 1;		//��ǰ�е���β
				//�����ַ��������˳�
				memcpy(tempChar0,m_nowString + thisLineStart,nextLineStart - thisLineStart);
				tempChar0[nextLineStart - thisLineStart] = '\0';
				memcpy(tempChar1,m_nowString + nextLineStart,next2LineStart - nextLineStart);
				tempChar1[next2LineStart - nextLineStart] = '\0';
				memcpy(m_nowString + thisLineStart,tempChar1,next2LineStart - nextLineStart);
				memcpy(m_nowString + thisLineStart + next2LineStart - nextLineStart,tempChar0,nextLineStart - thisLineStart);
				updateLineStr();
				break;
			}
	//		lineLength = 0;
		}
	/*	else
		{
			lineLength ++;
			if(lineLength >= MAX_LINE_STRING_LENGTH - 1)
			{
				lineSum++;
				if(lineSum == order)
				{
					thisLineStart = i + 1;	//��ǰ�е���ͷ
				}else
				if(lineSum == order + 1)
				{
					nextLineStart = i + 1;		//��ǰ�е���β
				}else
				if(lineSum == order + 2)
				{
					next2LineStart = i + 1;		//��ǰ�е���β
					//�����ַ��������˳�
					memcpy(tempChar0,m_nowString + thisLineStart,nextLineStart - thisLineStart);
					tempChar0[nextLineStart - thisLineStart] = '\0';
					memcpy(tempChar1,m_nowString + nextLineStart,next2LineStart - nextLineStart);
					tempChar1[next2LineStart - nextLineStart] = '\0';
					memcpy(m_nowString + thisLineStart,tempChar1,next2LineStart - nextLineStart);
					memcpy(m_nowString + thisLineStart + next2LineStart - nextLineStart,tempChar0,nextLineStart - thisLineStart);
					updateLineStr();
					break;
				}
				lineLength = 0;
			}
		}*/
	}
	return XTrue;
}
_XBool _XMultiText::insertALine(const char *str,int lineOrder)	//���ı���ָ���в���һ��
{
	if(!m_isInited) return XFalse;		//���û�г�ʼ��ֱ���˳�
	if(!m_haveSelect) return XFalse;
	if(lineOrder < 0 || lineOrder >= m_nowMaxLineSum) return XFalse;	//�Ƿ���ֵ���˳�
	if(str == NULL || strlen(str) == 0) return XFalse;
	if(strlen(str) + strlen(m_nowString) + 1 >= MAX_STRING_LENGTH) return XFalse;
	int needAddEnter = 0;
	if(str[strlen(str) - 1] != '\n')
	{//���������ַ��������Իس���������������뻻�з�
		if(strlen(str) + strlen(m_nowString) + 2 >= MAX_STRING_LENGTH) return XFalse;
		needAddEnter = 1;
	}
	int thisLineStart = 0;	//��һ�п�ʼ��λ��
	//int nextLineStart = 0;	//��һ�п�ʼ��λ��
	int lineSum = 0;		//��ǰ���б�
	char *tempStr = createArrayMem<char>(MAX_STRING_LENGTH);
	if(tempStr == NULL) return XFalse;
	if(lineOrder == 0)
	{
		strcpy(tempStr,m_nowString);
		strcpy(m_nowString,str);
		if(needAddEnter == 1)
		{
			strcat(m_nowString,"\n");
		}
		strcat(m_nowString,tempStr);
		checkStr();
		updateLineStr();

	}else
	{
		for(int i = 0;;++ i)
		{
			if(m_nowString[i] < 0)
			{
				++ i;
				continue;
			}
			if(m_nowString[i] == '\0')
			{//�������һ�л�û�н��У����ǲ����ܵģ��߼���
				return XFalse;
			}else
			if(m_nowString[i] == '\n')
			{
				lineSum ++;
				if(lineSum == lineOrder)
				{
					thisLineStart = i + 1;
					strcpy(tempStr,m_nowString + thisLineStart);
					m_nowString[thisLineStart] = '\0';
					strcat(m_nowString,str);
					if(needAddEnter == 1)
					{
						strcat(m_nowString,"\n");
					}
					strcat(m_nowString,tempStr);
					checkStr();
					updateLineStr();
					break;
				}
			}
		}
	}
	XDELETE_ARRAY(tempStr);
	return XTrue;
}
_XBool _XMultiText::isInRect(float x,float y)
{
	if(!m_isInited) return XFalse;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}
_XVector2 _XMultiText::getBox(int order)
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