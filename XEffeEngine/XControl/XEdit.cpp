//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file.
//--------------------------------
#include "XEdit.h"
#include "XObjectManager.h" 
#include "XControlManager.h"

//��һ������Edit�ڲ����õ��ڴ�ռ䣬��Ҫ������ʵ��֮��ĸ���ճ��ʱ���ڴ��������
char copyString[MAX_INPUT_STRING_LENGTH] = "";

_XEditTexture::_XEditTexture()
:m_isInited(0)
,editNormal(NULL)			//�������ͨ״̬
,editDisable(NULL)			//�������Ч״̬
,editSelect(NULL)			//�����Ƭѡ��ɫ
,editInsert(NULL)			//����������
,editUpon(NULL)
{
}

_XEditTexture::~_XEditTexture()
{
	release();
}

int _XEditTexture::init(const char *normal,const char *disable,const char *select,const char *insert,const char *upon,int resoursePosition)
{
	if(m_isInited != 0) return 0;	//��ֹ�ظ���ʼ��
	if(normal == NULL || disable == NULL || insert == NULL || select == NULL) return 0;//��Щ�ؼ�����Դ����Ϊ��
	int ret = 1;
	if((editNormal = createATextureData(normal,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editDisable = createATextureData(disable,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editInsert = createATextureData(insert,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editSelect = createATextureData(select,resoursePosition)) == NULL) ret = 0;
	if(ret != 0 &&
		(editUpon = createATextureData(upon,resoursePosition)) == NULL) ret = 0;

	if(ret == 0)
	{
		XDELETE(editNormal);
		XDELETE(editDisable);
		XDELETE(editSelect);
		XDELETE(editInsert);
		XDELETE(editUpon);
		return 0;
	}

	m_isInited = 1;
	return 1;
}

void _XEditTexture::release()
{
	if(m_isInited == 0) return;
	XDELETE(editNormal);
	XDELETE(editDisable);
	XDELETE(editSelect);
	XDELETE(editInsert);
	XDELETE(editUpon);
	m_isInited = 0;
}

_XEdit::_XEdit()
:m_isInited(0)					//�Ƿ��ʼ��
,m_mouseRightButtonMenu(NULL)	//����Ҽ��˵�
,m_nowString(NULL)				//��ǰ������ַ���
,m_tempNowString(NULL)
//,m_insertString(NULL)			//������ַ���
//,m_selectString(NULL)			//ѡȡ���ַ���
,m_haveSelect(0)
,m_editNormal(NULL)				//�������ͨ״̬
,m_editDisable(NULL)			//�������Ч״̬
,m_editSelect(NULL)				//�����Ƭѡ��ɫ
,m_editInsert(NULL)				//����������
,m_funInputChenge(NULL)			//�������ݷ����ı��ʱ�����
,m_funInputOver(NULL)			//ȷ���������֮���ʱ�����
{
}

_XEdit::~_XEdit()
{
	release();
}

int _XEdit::init(const _XVector2& position,		//�ؼ���λ��
		const _XRect& Area,					//�ؼ��������Ӧ����
		const _XEditTexture &tex,			//�ؼ�����ͼ
		const char *str,					//�ؼ��ĳ�ʼ���ַ���
		const _XFontUnicode &font,			//�ؼ�������
		float strSize,	//�ؼ���������Ϣ
		_XMouseRightButtonMenu * mouseMenu)		//�ؼ����Ҽ��˵�
{
	if(m_isInited != 0) return 0;
	if(tex.editNormal == NULL || tex.editInsert == NULL || tex.editDisable == NULL || tex.editSelect == NULL) return 0;

	//��Ҫע��������뷶Χ����Ҫ����ʾ����һ���ַ������򽫻�������⣬Ŀǰ���ﲢû�д���ȥ�жϣ�����ʵ��ʹ������Ҫע���������
	if(Area.getWidth() <= 0 || Area.getHeight() <= 0) return 0;	//���뷶Χ����Ϊ��
	if(strSize <= 0) return 0;		//�ַ����Ĵ�С����Ϊ�Ƿ�ֵ
	if(str != NULL && strlen(str) >= MAX_INPUT_STRING_LENGTH) return 0;
	if(mouseMenu != NULL)
	{//������Ҫ�Ľ�������ʹ��ָ�룬���ܵ����޸ĵ�Ӱ��
		try
		{
			m_mouseRightButtonMenu = new _XMouseRightButtonMenu;
			if(m_mouseRightButtonMenu == NULL) return 0;
		}catch(...)
		{
			return 0;
		}		
		m_mouseRightButtonMenu->setACopy(* mouseMenu);
		//�ӿؼ��������н�����ؼ�ע����
		_XControlManager::GetInstance().decreaseAObject(m_mouseRightButtonMenu);	//ע��������
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(m_mouseRightButtonMenu);
#endif
	}

	//���濪ʼ��ֵ
	m_mouseRect = Area;	//��¼������Ӧ��Χ
	m_position = position;

	m_editNormal = tex.editNormal;			//�������ͨ״̬
	m_editDisable = tex.editDisable;		//�������Ч״̬
	m_editSelect = tex.editSelect;			//�����Ƭѡ��ɫ
	m_editInsert = tex.editInsert;			//����������
	m_editUpon = tex.editUpon;			//����������
	m_size.set(1.0f,1.0f);

	try
	{
		m_nowString = new char[MAX_INPUT_STRING_LENGTH];	//��ǰ������ַ���
		if(m_nowString == NULL) 
		{
			XDELETE(m_mouseRightButtonMenu);
			return 0;
		}
	}catch(...)
	{
		XDELETE(m_mouseRightButtonMenu);
		return 0;
	}		
	m_nowString[0] = '\0';
	try
	{
		m_tempNowString = new char[MAX_INPUT_STRING_LENGTH];	//��ǰ������ַ���
		if(m_tempNowString == NULL) 
		{
			XDELETE_ARRAY(m_nowString);
			XDELETE(m_mouseRightButtonMenu);
			return 0;
		}
	}catch(...)
	{
		XDELETE_ARRAY(m_nowString);
		XDELETE(m_mouseRightButtonMenu);
		return 0;
	}		
	m_tempNowString[0] = '\0';
//	m_insertString = new char[MAX_INPUT_STRING_LENGTH];	//��ǰ������ַ���
//	if(m_insertString == NULL) return 0;
//	m_insertString[0] = '\0';
//	m_selectString = new char[MAX_INPUT_STRING_LENGTH];	//��ǰ������ַ���
//	if(m_selectString == NULL) return 0;
//	m_selectString[0] = '\0';
	try
	{
		m_nowShowString = new char[MAX_INPUT_STRING_LENGTH];	//��ǰ������ַ���
		if(m_nowShowString == NULL) 
		{
			XDELETE_ARRAY(m_tempNowString);
			XDELETE_ARRAY(m_nowString);
			XDELETE(m_mouseRightButtonMenu);
			return 0;
		}
	}catch(...)
	{
		XDELETE_ARRAY(m_tempNowString);
		XDELETE_ARRAY(m_nowString);
		XDELETE(m_mouseRightButtonMenu);
		return 0;
	}		
	m_nowShowString[0] = '\0';

	m_caption.setACopy(font);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	m_caption.setAlignmentModeX(_FONT_ALIGNMENT_MODE_X_LEFT); //�������������
	m_caption.setAlignmentModeY(_FONT_ALIGNMENT_MODE_Y_MIDDLE); //����������ж���
	m_textColor.setColor(0.0f,0.0f,0.0f,1.0f);
	m_caption.setColor(m_textColor);							//�����������ɫΪ��ɫ

	m_textPosition.set(m_mouseRect.left + TEXT_EDGE_WIDTH,m_mouseRect.top + m_mouseRect.getHeight() * 0.5f);			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize.set(strSize,strSize);				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����
	if(str != NULL)
	{
		m_caption.setString(str);
		strcpy(m_nowString,str);
		m_nowInsertPoint = strlen(m_nowString);	//��ǰѡ�������λ��
		m_nowStringLength = strlen(m_nowString);	//��ǰ�����ַ����ĳ���
	}else
	{
		m_caption.setString(" ");
		m_nowString[0] = '\0';
		m_nowInsertPoint = 0;	//��ǰѡ�������λ��
		m_nowStringLength = 0;	//��ǰ�����ַ����ĳ���
	}
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	//m_caption.m_isPassWord = 1;
//	m_selectText.setACopy(* font);
//	m_selectText.setString(" ");
//	m_selectText.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_size.x,m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_size.y);
//	m_selectText.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);

	m_haveSelect = 0;		//�Ƿ����ַ�������ѡ��
	m_selectMouseDown = 0;
	m_selectStart = -1;		//ѡ�����ʼλ��
	m_selectEnd = -1;		//ѡ��Ľ���λ��

	m_spriteBackGround.init(m_editNormal->texture.m_w,m_editNormal->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackGround);
#endif
	m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setSize(m_size);
	m_spriteBackGround.setIsTransformCenter(POINT_LEFT_TOP);

	m_spriteSelect.init(m_editSelect->texture.m_w,m_editSelect->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteSelect);
#endif
	m_spriteSelect.setPosition(m_position);
	m_spriteSelect.setSize(m_size);
	m_spriteSelect.setIsTransformCenter(POINT_LEFT_TOP);

	//����������п�����ʾ���ַ����ĳ���(����ȡ0.5������Ϊ��ʾӢ���ַ�����ȴ��ʹ�õ����������)
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * TEXT_EDGE_WIDTH) * m_size.x / m_nowTextWidth;
	m_nowShowStart = 0;	//��ʼ����ǰ��ʾ����ʼλ��
	upDataShowStr();

	m_spriteInsert.init(m_editInsert->texture.m_w,m_editInsert->texture.m_h,1);
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteInsert);
#endif
	upDateInsertShowPosition();
	m_spriteInsert.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_spriteInsert.setIsTransformCenter(POINT_LEFT_TOP);

	//printf("Can Show String Length:%d\n",m_canShowLength);

	m_timer = 0;

	m_keyShiftState = 0;
	m_keyCapsLockState = 0;
	m_keyNumLockState = 1;
	m_keyCtrlState = 0;

	m_isVisiable = 1;
	m_isEnable = 1;
	m_isActive = 1;
	m_isBeChoose = 0; 

	_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_EDIT);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	m_isInited = 1;
	return 1;
}

void _XEdit::upDataShowStr()
{//��û��ʵ��
	if(m_nowStringLength <= m_nowShowStart + m_canShowLength) strcpy(m_nowShowString,m_nowString + m_nowShowStart);
	else
	{
		memcpy(m_nowShowString,m_nowString + m_nowShowStart,m_canShowLength);
		m_nowShowString[m_canShowLength] = '\0';
	}
	//���µ���ʾ��
	m_caption.setString(m_nowShowString);
}

void _XEdit::drawUp()				//���С�˵�
{
	if(m_isInited == 0) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isVisiable == 0) return;	//������ɼ�ֱ���˳�
	//��ʾ����Ҽ��˵�
	if(m_mouseRightButtonMenu != NULL) m_mouseRightButtonMenu->draw();
}

void _XEdit::draw()
{
	if(m_isInited == 0) return ;	//���û�г�ʼ��ֱ���˳�
	if(m_isVisiable == 0) return;	//������ɼ�ֱ���˳�
	
	if(m_isEnable == 0) m_spriteBackGround.draw(m_editDisable);	//�����Ч����ʾ��Ч,����ʾ�������
	else m_spriteBackGround.draw(m_editNormal);//�����Ч����ʾ��Ч,��ʾ�������

	if(m_haveSelect != 0) m_spriteSelect.draw(m_editSelect);//��ʾѡ��ķ�Χ

	//��ʾ��ǰ������ַ���
	m_caption.draw();
	if(m_isEnable != 0)
	{
		if(m_isBeChoose)
		{
			++m_timer;
			if(m_timer > 50) m_timer = 0;
			else
			if(m_timer > 25) m_spriteInsert.draw(m_editInsert);
		}
	}
//	//��ʾ����Ҽ��˵�
//	if(m_mouseRightButtonMenu != NULL)
//	{
//		m_mouseRightButtonMenu->draw();
//	}
	if(m_editUpon != NULL) m_spriteBackGround.draw(m_editUpon);	
}

void _XEdit::release()
{
	if(m_isInited == 0) return;
	XDELETE_ARRAY(m_nowString);
	XDELETE_ARRAY(m_tempNowString);
	//XDELETE_ARRAY(m_insertString);
	//XDELETE_ARRAY(m_selectString);
	XDELETE_ARRAY(m_nowShowString);
	XDELETE(m_mouseRightButtonMenu);
	_XControlManager::GetInstance().decreaseAObject(this);	//ע��������
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(this);
#endif
	m_isInited = 0;
}

void _XEdit::setString(const char *str)
{//����������ٺܸ��ӵ��߼�����������û����ڴ�������׶λ������ڴ���Ƭѡ�׶ε�ʱ������޸Ľ�����ɽ�Ϊ���Ӱ�죬����������ܻ���һЩ�����Ի��Ĵ���
	if(str == NULL) return;
	m_caption.setString(str);
	strcpy(m_nowString,str);
	m_nowInsertPoint = strlen(m_nowString);	//��ǰѡ�������λ��
	m_nowStringLength = strlen(m_nowString);	//��ǰ�����ַ����ĳ���
	//ȡ������Ƭѡ����Ϣ
	m_haveSelect = 0;		//�Ƿ����ַ�������ѡ��
	m_selectMouseDown = 0;
	m_selectStart = -1;		//ѡ�����ʼλ��
	m_selectEnd = -1;		//ѡ��Ľ���λ��
	//����������п�����ʾ���ַ����ĳ���(����ȡ0.5������Ϊ��ʾӢ���ַ�����ȴ��ʹ�õ����������)
	m_canShowLength = (m_mouseRect.getWidth() - 2.0f * TEXT_EDGE_WIDTH) * m_size.x / m_nowTextWidth;
	m_nowShowStart = 0;	//��ʼ����ǰ��ʾ����ʼλ��
	upDataShowStr();

	upDateInsertShowPosition();
}

void _XEdit::upDateInsertShowPosition()
{//����*0.5����Ϊֻ��ʾӢ�ģ�����ȴʹ����������⣬���Կ��Ҫ��С��0.5
	m_spriteInsert.setPosition(m_position.x + m_textPosition.x * m_size.x 
		+ (m_nowInsertPoint - m_nowShowStart) * m_nowTextWidth,
		m_position.y + m_textPosition.y * m_size.y - m_nowTextHeight * 0.5f);
}

void _XEdit::deleteSelectStr()
{
	if(getSelectEnd() == m_nowStringLength)
	{//Ƭѡ������
		m_nowStringLength -= getSelectLength();
		m_nowString[getSelectHead()] = '\0';
		changeInsertPoint(getSelectHead() - m_nowInsertPoint);
	}else
	{
		strcpy(m_nowString + getSelectHead(),m_nowString + getSelectEnd());
		m_nowStringLength -= getSelectLength();
		changeInsertPoint(getSelectHead() - m_nowInsertPoint);
	}
}

void _XEdit::mouseRightMenuProc()
{
	int TextChange = 0;
	if(m_mouseRightButtonMenu->getLastChoose() == 0)
	{//����
		if(m_haveSelect != 0)
		{//����Ƭѡ��û��Ƭѡ��ʲôҲ����
			//1������Ƭѡ������
			memcpy(copyString,m_nowString + getSelectHead(),getSelectLength());
			copyString[getSelectLength()] = '\0';
			//2��ɾ��Ƭѡ������
			deleteSelectStr();
			m_haveSelect = 0;	//Ƭѡ����
			TextChange = 1;
		}
	}else
	if(m_mouseRightButtonMenu->getLastChoose() == 1)
	{//����
		if(m_haveSelect != 0)
		{//����Ƭѡ��û��Ƭѡ��ʲôҲ����
			//����Ƭѡ������
			memcpy(copyString,m_nowString + getSelectHead(),getSelectLength());
			copyString[getSelectLength()] = '\0';
		}
	}else
	if(m_mouseRightButtonMenu->getLastChoose() == 2)
	{//ճ��
		//�жϹ����ڴ�ռ����Ƿ�������
		if(copyString[0] != '\0')
		{
			char haveEnd = 0;
			for(int i = 0;i < MAX_INPUT_STRING_LENGTH;++ i)
			{
				if(copyString[i] == '\0')
				{
					haveEnd = 1;
					break;
				}
			}
			if(haveEnd != 0)
			{
				if(m_haveSelect != 0)
				{//����Ƭѡ(����ʵ�����滻����)
					if(m_nowStringLength + strlen(copyString) - getSelectLength() > MAX_INPUT_STRING_LENGTH - 2)
					{//��������ܳ��ȣ���������Ҫ��������Ŀǰ����Ϊ��������
					}else
					{							
						//1��ɾ��Ƭѡ����
						deleteSelectStr();
						m_haveSelect = 0;	//Ƭѡ����
						//2�����븴������
						if(m_nowInsertPoint == m_nowStringLength)
						{//��������ַ�����ĩβ���
							strcat(m_nowString,copyString);
							m_nowStringLength = strlen(m_nowString);
							//m_nowInsertPoint = m_nowStringLength;
							changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
							TextChange = 1;
						}else
						{
							strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
							strcpy(m_nowString + m_nowInsertPoint,copyString);
							strcat(m_nowString,m_tempNowString);
							m_nowStringLength = strlen(m_nowString);
							//m_nowInsertPoint += strlen(copyString);
							changeInsertPoint(strlen(copyString));
							TextChange = 1;
						}
					}
				}else
				{//û��Ƭѡ
					//�ж�ճ���������Ƿ�ᳬ���ַ������ܳ���
					if(m_nowStringLength + strlen(copyString) > MAX_INPUT_STRING_LENGTH - 2)
					{//��������ܳ��ȣ���������Ҫ��������Ŀǰ����Ϊ��������
					}else
					{
						if(m_nowInsertPoint == m_nowStringLength)
						{//��������ַ�����ĩβ���
							strcat(m_nowString,copyString);
							m_nowStringLength = strlen(m_nowString);
						//	m_nowInsertPoint = m_nowStringLength;
							changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
							TextChange = 1;
						}else
						{
							strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
							strcpy(m_nowString + m_nowInsertPoint,copyString);
							strcat(m_nowString,m_tempNowString);
							m_nowStringLength = strlen(m_nowString);
							//m_nowInsertPoint += strlen(copyString);
							changeInsertPoint(strlen(copyString));
							TextChange = 1;
						}
					}
				}
			}
		}
	}else
	if(m_mouseRightButtonMenu->getLastChoose() == 3)
	{//����(�������Ƭѡ��ȡ��Ƭѡ)
		if(m_haveSelect != 0) m_haveSelect = 0;
	}
	printf("%d\n",m_mouseRightButtonMenu->getLastChoose());
	if(TextChange != 0)
	{
		upDataShowStr();	//��Ϊ�ַ������ı䣬����������Ҫ�����ַ�����ʾ
	//	m_caption.setString(m_nowString);
	//	m_spriteInsert.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_size.x + m_nowInsertPoint * m_caption.m_size.x * m_caption.m_showSize.x * 0.5,
	//		m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_size.y);
		if(m_funInputChenge != NULL) (* m_funInputChenge)(m_pClass,m_objectID);
	}
}

void _XEdit::changeInsertPoint(int sum)
{
	if(sum == 0) return;
	int tempNewPosition = m_nowInsertPoint + sum;
	if(tempNewPosition < 0) return;
	if(tempNewPosition > m_nowStringLength) return;
	//�����ƶ�
	int tempOldPosition = m_nowInsertPoint;		//�ɹ���λ��
	m_nowInsertPoint = tempNewPosition;
	//����Ƿ�Ҫ�ƶ���ʾ����
	if(m_nowInsertPoint < m_nowShowStart || m_nowInsertPoint > m_nowShowStart + m_canShowLength)
	{//������ʾ��Χ��Ҫ�޸���ʾ��Χ
		if(m_nowInsertPoint < m_nowShowStart)
		{//��ǰ�ƶ�
			if(tempOldPosition != m_nowShowStart)
			{//���֮ǰ���û���������Ŀ�ͷ������Ҫ���µ���ͷ
				m_nowShowStart = m_nowInsertPoint;
				upDateInsertShowPosition();
			}else
			{
				m_nowShowStart = m_nowInsertPoint;
			}
		}else
		if(m_nowInsertPoint > m_nowShowStart + m_canShowLength)
		{//����ƶ�
			if(tempOldPosition != m_nowShowStart + m_canShowLength)
			{//���֮ǰ�Ĺ�겻��������β����Ҫ���¹�굽��β
				m_nowShowStart = m_nowInsertPoint - m_canShowLength;
				upDateInsertShowPosition();
			}else
			{
				m_nowShowStart = m_nowInsertPoint - m_canShowLength;
			}
		}
		upDataShowStr();
	}else
	{//����Ҫ�޸���ʾ��Χ
		upDateInsertShowPosition();
	}
}

void _XEdit::upDataSelectShow()
{
	int tempHead = getSelectHead();
	int tempEnd = getSelectEnd();
	if(tempHead < m_nowShowStart) tempHead = m_nowShowStart;
	if(tempHead >= m_nowShowStart + m_canShowLength)
	{//����������Ҫ��ʾ
		tempHead = m_nowShowStart + m_canShowLength;
	}
	if(tempEnd <= m_nowShowStart)
	{//�������Ҳ����Ҫ��ʾ
		tempEnd = m_nowShowStart;
	}
	if(tempEnd > m_nowShowStart + m_canShowLength) tempEnd = m_nowShowStart + m_canShowLength;
	//����ʵ�ʵ��������ѡ��λ�ú�ѡ��ͼƬ��Ҫ���õĳߴ�
	m_spriteSelect.setPosition(m_position.x + m_textPosition.x * m_size.x + (tempHead - m_nowShowStart) * m_nowTextWidth,
		m_position.y + m_textPosition.y * m_size.y - m_nowTextHeight * 0.5f);
	if(tempEnd - tempHead < m_canShowLength)
	{
		m_spriteSelect.setSize((tempEnd - tempHead) * m_nowTextWidth /m_editSelect->textureSize.x,
			m_nowTextHeight / m_editSelect->textureSize.y);
	}else
	{
		m_spriteSelect.setSize(m_canShowLength * m_nowTextWidth /m_editSelect->textureSize.x,
			m_nowTextHeight / m_editSelect->textureSize.y);
	}
}

int _XEdit::canGetFocus(float x,float y)
{
	if(m_isInited == 0) return 0;	//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 0;		//û�м���Ŀؼ������տ���
	if(m_isVisiable == 0) return 0;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�
	return isInRect(x,y);
}

int _XEdit::canLostFocus(float x,float y)
{
	if(m_isInited == 0) return 1;		//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 1;	//������ɼ�ֱ���˳�
	if(m_isVisiable == 0) return 1;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 1;		//�����Ч��ֱ���˳�
	if(m_mouseRightButtonMenu != NULL && m_mouseRightButtonMenu->getVisiable() != 0) return 0;
	//if(m_isBeChoose != 0) return 0;
	return 1;
}

int _XEdit::mouseProc(float x,float y,_XMouseState mouseState)
{
	if(m_isInited == 0) return 0;		//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 0;	//������ɼ�ֱ���˳�
	if(m_isVisiable == 0) return 0;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�
	//���������Ӧ�����λ��
//	_XRect temp;
//	temp.set(m_objRect.left,m_objRect.top,m_objRect.left + m_objRect.getWidth() * m_size.x,m_objRect.top + m_objRect.getHeight() * m_size.y);
	if(m_mouseRightButtonMenu != NULL)
	{//�Ҽ��˵��������������
		int  flagMouseMenu = 0;
		if(m_mouseRightButtonMenu->getVisiable() != 0) flagMouseMenu = 1;
		if(mouseState == _XMOUSE_RIGHT_BUTTON_UP)
		{//������Ҽ���������������Ҫ�ڿռ䷶Χ��
			if(m_nowMouseRect.isInRect(x,y) && m_isBeChoose != 0)
			{
				m_mouseRightButtonMenu->mouseProc(x,y,mouseState);
			}
		}else
		{
			m_mouseRightButtonMenu->mouseProc(x,y,mouseState);
		}
		if(flagMouseMenu == 1 && m_mouseRightButtonMenu->getVisiable() == 0)
		{//�����˿�ݼ�(����ʵ�ֶ�Ӧ�˵���Ĺ���)
			mouseRightMenuProc();//�Ҽ��˵�����Ӧ
			if(m_mouseRightButtonMenu->getLastChoose() !=  -1)
			{//����˵��в��������������Ϣ����
				return 1;	//���Ҽ��˵��Ĳ�����Ч�Ļ�������������Ϣ�������ظ�����
			}
		}
	} 
	if(m_nowMouseRect.isInRect(x,y))
	{//����ڷ�Χ�ڵ��
		//���������Ҽ������Գ���
		if(mouseState == _XMOUSE_LEFT_BUTTON_DOWN || mouseState == _XMOUSE_RIGHT_BUTTON_DOWN)
		{//���	
			if(m_isBeChoose == 0)	m_isBeChoose = 1;//����ؼ�û�д��ڼ���״̬�򼤻�

			if(m_nowStringLength > 0)
			{//�ַ��������ַ�ʱ������ͬͨ�����ѡ�����λ��(����ĳ���10��Ϊ�˷���ѡ��ͷ����β)
				if(m_nowMouseRect.isInRect(x,y))
				{//��갴�¶�����Ч
					//�������λ��
					if(x <= m_position.x + m_textPosition.x * m_size.x)
					{//ͷ
						if(m_nowInsertPoint != m_nowShowStart) changeInsertPoint(m_nowShowStart - m_nowInsertPoint);
					}else
					if(x >= m_position.x + (m_mouseRect.right - TEXT_EDGE_WIDTH) * m_size.x)
					{//β
						if(m_nowStringLength <= m_canShowLength)
						{
							if(m_nowInsertPoint != m_nowStringLength) changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
						}else
						{//����ʾ�Ľ���
							if(m_nowInsertPoint != m_nowShowStart + m_canShowLength) changeInsertPoint(m_nowShowStart + m_canShowLength - m_nowInsertPoint);
						}
					}else
					{//��Χ��
						//����ʹ��0.5�ĳ�������ΪĿǰʹ�õ���Ӣ�����뵫��ʹ�õ���������������⣬����Ҫ����һ���ռ������
						//Ϊ�����ѡ��ʱ�ĸо��������λ����ַ��Ŀ�ȣ�ʹѡ���ʱ������ио�
						int tempInsertPoint = (x + m_nowTextWidth * 0.5f - m_position.x - m_textPosition.x * m_size.x) / m_nowTextWidth;
						if(tempInsertPoint + m_nowShowStart > m_nowStringLength)
						{
							if(m_nowInsertPoint != m_nowStringLength) changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
						}else
						{
							if(m_nowInsertPoint != tempInsertPoint + m_nowShowStart) changeInsertPoint(tempInsertPoint + m_nowShowStart - m_nowInsertPoint);
						}
					}
					if(mouseState == _XMOUSE_LEFT_BUTTON_DOWN)
					{//�����������£��������Ƭѡ����
						m_haveSelect = 0;
						m_selectEnd = m_nowInsertPoint;
						m_selectStart = m_nowInsertPoint;
						m_selectMouseDown = 1;
					}else
					if(mouseState == _XMOUSE_RIGHT_BUTTON_DOWN)
					{//������Ҽ����²������ٷ�Χ��Ҳ�������Ƭѡ����
						if(m_nowInsertPoint > getSelectEnd() || m_nowInsertPoint < getSelectHead())
						{
							m_haveSelect = 0;
						}
					}
				}
			}
		}
	}else
	{//����ڷ�Χ������ȥ����
		if(mouseState == _XMOUSE_LEFT_BUTTON_DOWN || mouseState == _XMOUSE_RIGHT_BUTTON_DOWN)
		{//���
			if(m_isBeChoose != 0) m_isBeChoose = 0;//����ؼ�û�д��ڼ���״̬�򼤻�
		}
	}

	if(mouseState == _XMOUSE_LEFT_BUTTON_DOWN)
	{//�κ����������²����������Ƭѡ����
		if(m_haveSelect != 0) m_haveSelect = 0;
	}

	if(mouseState == _XMOUSE_MOVE)
	{
		if(m_selectMouseDown != 0)
		{//�����갴�£������϶�������ΪƬѡ��������
			//����Ƭѡ��λ��
			if(x <= m_position.x + m_textPosition.x * m_size.x)
			{//ͷ
				if(m_nowShowStart > 0 && x <= m_position.x + m_textPosition.x * m_size.x - TEXT_EDGE_WIDTH) 
				{//�������ͷ��������ǰ�ƶ�
					m_selectEnd = m_nowShowStart - 1;
				}else 
				{
					m_selectEnd = m_nowShowStart;
				}
			}else
			if(x >= m_position.x + (m_mouseRect.right - TEXT_EDGE_WIDTH) * m_size.x)
			{//β
				if(m_nowShowStart + m_canShowLength >= m_nowStringLength)
				{//����ַ���û����ʾ��
					m_selectEnd = m_nowStringLength;
				}else
				{
					if(x >= m_position.x + (m_mouseRect.right - TEXT_EDGE_WIDTH) * m_size.x + TEXT_EDGE_WIDTH)
					{//�������β��������ƶ�
						m_selectEnd = m_nowShowStart + m_canShowLength + 1;
					}else
					{
						m_selectEnd = m_nowShowStart + m_canShowLength;
					}
				}
			}else
			{//����ʹ��0.5�ĳ�������ΪĿǰʹ�õ���Ӣ�����뵫��ʹ�õ���������������⣬����Ҫ����һ���ռ������
				int tempPosition = (x - m_position.x - m_textPosition.x * m_size.x) / m_nowTextWidth;
				if(tempPosition + m_nowShowStart >= m_nowStringLength) m_selectEnd = m_nowStringLength;
				else m_selectEnd = m_nowShowStart + tempPosition;
			}
			if(m_selectEnd != m_nowInsertPoint) changeInsertPoint(m_selectEnd - m_nowInsertPoint);	//�������ҲҪ����仯
			if(m_selectEnd != m_selectStart)
			{//Ƭѡ��Ч
				m_haveSelect = 1;
				upDataSelectShow();
			}else m_haveSelect = 0;	//Ƭѡ��Ч
		}
	}	
	if(m_isBeChoose != 0 && (mouseState == _XMOUSE_LEFT_BUTTON_UP || mouseState == _XMOUSE_RIGHT_BUTTON_UP))
	{//����ǵ����¼��������Ƿ��ڷ�Χ�ڣ�ֻҪ�ռ䴦�ڼ���״̬�����������Ӧ
		if(m_selectMouseDown != 0)
		{//Ƭѡ�����ж�
			if(m_selectEnd != m_selectStart) m_haveSelect = 1;	//Ƭѡ��Ч
			else m_haveSelect = 0;	//Ƭѡ��Ч
			m_selectMouseDown = 0;
		}
	}
	return 1;
}

int _XEdit::keyboardProc(int keyOrder,_XKeyState keyState)
{
	if(m_isInited == 0) return 0;	//���û�г�ʼ��ֱ���˳�
	if(m_isActive == 0) return 0;	//������ɼ�ֱ���˳�
	if(m_isVisiable == 0) return 0;	//������ɼ�ֱ���˳�
	if(m_isEnable == 0) return 0;		//�����Ч��ֱ���˳�

	if(m_mouseRightButtonMenu != NULL)
	{
		if(m_mouseRightButtonMenu->getVisiable() != 0)
		{//����Ҽ��˵��ɼ�,�����а������������������Ӱ��
			m_mouseRightButtonMenu->keyboardProc(keyOrder,keyState);
			if(m_mouseRightButtonMenu->getVisiable() == 0)
			{//�����˿�ݼ�(����ʵ�ֶ�Ӧ�˵���Ĺ���)
				mouseRightMenuProc();//�Ҽ��˵�����Ӧ
			}
			return 0;
		}
	}
	
	if(keyState == _XKEY_STATE_DOWN && m_isBeChoose != 0)
	{
	//	printf("down %d\n",keyOrder);
		if(keyOrder ==  SDLK_RSHIFT || keyOrder ==  SDLK_LSHIFT)
		{//shift������
			m_keyShiftState = 1;
		}else
		if(keyOrder ==  SDLK_CAPSLOCK)
		{//CapsLock������
			m_keyCapsLockState = 1;
		}else
		if(keyOrder ==  SDLK_NUMLOCK)
		{//CapsLock������
			m_keyNumLockState = 1;
		}else
		if(keyOrder ==  SDLK_RCTRL || keyOrder ==  SDLK_LCTRL)
		{//Ctrl������
			m_keyCtrlState = 1;
		}
	}

	int TextChange = 0;		//�ı������Ƿ����仯
	if(keyState == _XKEY_STATE_UP && m_isBeChoose != 0)
	{//��������ʱ������Ӧ
		if(m_keyCtrlState == 0)
		{//ctrlû�а��µ�����������ܴ���
			if(m_haveSelect == 0)
			{//û��Ƭѡ��ʱ��
				if(m_nowStringLength < MAX_INPUT_STRING_LENGTH - 2)
				{//������볤��С����󳤶ȣ�����Լ������룬����ֻ������һЩ����
					//�����ǿɴ�ӡ�ַ��Ĵ���			
					TextChange = keyJudgement(keyOrder);
				}
			}else
			{//����Ƭѡ��ʱ��
				if(m_nowStringLength - getSelectLength() < MAX_INPUT_STRING_LENGTH - 2)
				{//������볤��С����󳤶ȣ�����Լ������룬����ֻ������һЩ����
					//�����ǿɴ�ӡ�ַ��Ĵ���			
					TextChange = keyJudgement(keyOrder);
				}
			}
		}else
		{//ctrl���µ�����������Ϊ���ܼ�����
			if(keyOrder ==  SDLK_c)
			{//ctrl + c ����
				if(m_haveSelect != 0)
				{//����Ƭѡ��û��Ƭѡ��ʲôҲ����
					//����Ƭѡ������
					memcpy(copyString,m_nowString + getSelectHead(),getSelectLength());
					copyString[getSelectLength()] = '\0';
				}
			}else
			if(keyOrder ==  SDLK_v)
			{//ctrl + v ճ��
				//�жϹ����ڴ�ռ����Ƿ�������
				if(copyString[0] != '\0')
				{
					char haveEnd = 0;
					for(int i = 0;i < MAX_INPUT_STRING_LENGTH;++ i)
					{
						if(copyString[i] == '\0')
						{
							haveEnd = 1;
							break;
						}
					}
					if(haveEnd != 0)
					{
						if(m_haveSelect != 0)
						{//����Ƭѡ(����ʵ�����滻����)
							if(m_nowStringLength + strlen(copyString) - getSelectLength() > MAX_INPUT_STRING_LENGTH - 2)
							{//��������ܳ��ȣ���������Ҫ��������Ŀǰ����Ϊ��������
							}else
							{							
								//1��ɾ��Ƭѡ����
								deleteSelectStr();
								m_haveSelect = 0;	//Ƭѡ����
								//2�����븴������
								if(m_nowInsertPoint == m_nowStringLength)
								{//��������ַ�����ĩβ���
									strcat(m_nowString,copyString);
									m_nowStringLength = strlen(m_nowString);
									//m_nowInsertPoint = m_nowStringLength;
									changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
									TextChange = 1;
								}else
								{
									strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
									strcpy(m_nowString + m_nowInsertPoint,copyString);
									strcat(m_nowString,m_tempNowString);
									m_nowStringLength = strlen(m_nowString);
									//m_nowInsertPoint += strlen(copyString);
									changeInsertPoint(strlen(copyString));
									TextChange = 1;
								}
							}
						}else
						{//û��Ƭѡ
							//�ж�ճ���������Ƿ�ᳬ���ַ������ܳ���
							if(m_nowStringLength + strlen(copyString) > MAX_INPUT_STRING_LENGTH - 2)
							{//��������ܳ��ȣ���������Ҫ��������Ŀǰ����Ϊ��������
							}else
							{
								if(m_nowInsertPoint == m_nowStringLength)
								{//��������ַ�����ĩβ���
									strcat(m_nowString,copyString);
									m_nowStringLength = strlen(m_nowString);
								//	m_nowInsertPoint = m_nowStringLength;
									changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
									TextChange = 1;
								}else
								{
									strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
									strcpy(m_nowString + m_nowInsertPoint,copyString);
									strcat(m_nowString,m_tempNowString);
									m_nowStringLength = strlen(m_nowString);
									//m_nowInsertPoint += strlen(copyString);
									changeInsertPoint(strlen(copyString));
									TextChange = 1;
								}
							}
						}
					}
				}			
			}
		}
		if(keyOrder ==  SDLK_BACKSPACE)
		{//���˼�
			if(m_haveSelect == 0)
			{//û��Ƭѡ��ʱ��
				if(m_nowInsertPoint > 0)
				{
					-- m_nowStringLength;
					changeInsertPoint(-1);
					if(m_nowStringLength == m_nowInsertPoint) m_nowString[m_nowInsertPoint] = '\0';	//�ַ���ĩβ
					else strcpy(m_nowString + m_nowInsertPoint,m_nowString + m_nowInsertPoint + 1);
					TextChange = 1;
				}
			}else
			{//����Ƭѡ��ʱ��
				if(m_selectMouseDown == 0)
				{//Ƭѡ�Ѿ�����
					//1��ɾ��Ƭѡ����
					deleteSelectStr();
					m_haveSelect = 0;	//Ƭѡ����
					TextChange = 1;
				}else
				{//Ƭѡ��δ����
					//����������
					if(m_selectEnd != m_selectStart)
					{//Ƭѡ��Ч
						m_haveSelect = 1;
						//1��ɾ��Ƭѡ����
						deleteSelectStr();
					}else m_haveSelect = 0;	//Ƭѡ��Ч

					m_selectMouseDown = 0;
					m_haveSelect = 0;	//Ƭѡ����
					TextChange = 1;
					//Ȼ���������¿�ʼ����
					m_haveSelect = 0;
					m_selectEnd = m_nowInsertPoint;
					m_selectStart = m_nowInsertPoint;
					m_selectMouseDown = 1;
				}
			}
		}else
		if(keyOrder ==  SDLK_DELETE || (m_keyShiftState == m_keyNumLockState && keyOrder ==  SDLK_KP_PERIOD))
		{//Delete����
			if(m_haveSelect == 0)
			{//û��Ƭѡ��ʱ��
				if(m_nowStringLength == m_nowInsertPoint)
				{//�ַ���ĩβ��������
				}else
				{
					strcpy(m_nowString + m_nowInsertPoint,m_nowString + m_nowInsertPoint + 1);
					-- m_nowStringLength;
					TextChange = 1;
				}
			}else
			{//����Ƭѡ��ʱ��
				if(m_selectMouseDown == 0)
				{//Ƭѡ�Ѿ�����
					//1��ɾ��Ƭѡ����
					deleteSelectStr();
					m_haveSelect = 0;	//Ƭѡ����
					TextChange = 1;
				}else
				{//Ƭѡ��δ����
					//����������
					if(m_selectEnd != m_selectStart)
					{//Ƭѡ��Ч
						m_haveSelect = 1;
						//1��ɾ��Ƭѡ����
						deleteSelectStr();
					}else m_haveSelect = 0;	//Ƭѡ��Ч

					m_selectMouseDown = 0;
					m_haveSelect = 0;	//Ƭѡ����
					TextChange = 1;
					//Ȼ���������¿�ʼ����
					m_haveSelect = 0;
					m_selectEnd = m_nowInsertPoint;
					m_selectStart = m_nowInsertPoint;
					m_selectMouseDown = 1;
				}
			}
		}else
		if(keyOrder ==  SDLK_RSHIFT || keyOrder ==  SDLK_LSHIFT)
		{//shift������
			m_keyShiftState = 0;
		}else
		if(keyOrder ==  SDLK_CAPSLOCK)
		{//CapsLock������
			m_keyCapsLockState = 0;
		}else
		if(keyOrder ==  SDLK_NUMLOCK)
		{//NumLock������
			m_keyNumLockState = 0;
		}else
		if(keyOrder ==  SDLK_RCTRL || keyOrder ==  SDLK_LCTRL)
		{//Ctrl������
			m_keyCtrlState = 0;
		}else
		if(keyOrder ==  SDLK_MENU)
		{//�˵��������£����൱��������һ�
			//�൱��������һ��¼�
			m_mouseRightButtonMenu->mouseProc(m_position.x + m_textPosition.x * m_size.x + (m_nowInsertPoint - m_nowShowStart) * m_nowTextWidth,
				m_position.y + m_textPosition.y * m_size.y,_XMOUSE_RIGHT_BUTTON_UP);
		}else
		if(keyOrder ==  SDLK_LEFT || (m_keyShiftState == m_keyNumLockState && keyOrder ==  SDLK_KP4))
		{//���󰸼�����(�ַ�������ı䣬���ǻ�ı�����λ��)
			if(m_keyShiftState == 0)
			{//���û�а���shift��
				if(m_selectMouseDown == 0)
				{
					if(m_haveSelect == 0) changeInsertPoint(-1);	//û��Ƭѡ,�ƶ�����λ��
					else
					{//����Ƭѡ
					//	m_nowInsertPoint = m_selectStart;
					//	m_spriteInsert.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_size.x + m_nowInsertPoint * m_caption.m_size.x * m_caption.m_showSize.x * 0.5,
					//		m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_size.y);
						changeInsertPoint(m_selectStart - m_nowInsertPoint);
						m_haveSelect = 0;
					}
				}
			}else
			{//�������shift����ΪƬѡ����
				if(m_haveSelect == 0)
				{//��ǿ�ʼƬѡ
					m_haveSelect = 0;
					m_selectEnd = m_nowInsertPoint;
					m_selectStart = m_nowInsertPoint;
					//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
					if(m_nowInsertPoint > 0)
					{
						changeInsertPoint(-1);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//Ƭѡ��Ч
						m_haveSelect = 1;
						upDataSelectShow();
					}else m_haveSelect = 0;//Ƭѡ��Ч
				}else
				{//�޸�Ƭѡ���
					//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
					if(m_nowInsertPoint > 0)
					{
						changeInsertPoint(-1);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//Ƭѡ��Ч
						m_haveSelect = 1;
						upDataSelectShow();
					}else m_haveSelect = 0;//Ƭѡ��Ч
				}
			}
		}else
		if(keyOrder ==  SDLK_RIGHT || (m_keyShiftState == m_keyNumLockState && keyOrder ==  SDLK_KP6))
		{//���󰸼�����(�ַ�������ı䣬���ǻ�ı�����λ��)
			if(m_keyShiftState == 0)
			{//���û�а���shift��
				if(m_selectMouseDown == 0)
				{
					if(m_haveSelect == 0) changeInsertPoint(1);	//û��Ƭѡ,�ƶ�����λ��
					else
					{//����Ƭѡ
						changeInsertPoint(m_selectEnd - m_nowInsertPoint);
						m_haveSelect = 0;
					}
				}
			}else
			{//�������shift����ΪƬѡ����
				if(m_haveSelect == 0)
				{//��ǿ�ʼƬѡ
					m_haveSelect = 0;
					m_selectEnd = m_nowInsertPoint;
					m_selectStart = m_nowInsertPoint;
					//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
					if(m_nowInsertPoint < m_nowStringLength)
					{
						changeInsertPoint(1);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//Ƭѡ��Ч
						m_haveSelect = 1;
						upDataSelectShow();
					}else m_haveSelect = 0;//Ƭѡ��Ч
				}else
				{//�޸�Ƭѡ���
					//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
					if(m_nowInsertPoint < m_nowStringLength)
					{
						changeInsertPoint(1);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//Ƭѡ��Ч
						m_haveSelect = 1;
						upDataSelectShow();
					}else m_haveSelect = 0;	//Ƭѡ��Ч
				}
			}
		}else
		if(keyOrder ==  SDLK_HOME || (m_keyShiftState == m_keyNumLockState && keyOrder ==  SDLK_KP7))
		{//home
			if(m_keyShiftState == 0)
			{//���shiftû�а���
				if(m_selectMouseDown == 0)
				{//���û�����ѡ��(��������ѡ���򰴼���Ч)
					if(m_nowInsertPoint != 0) changeInsertPoint(-m_nowInsertPoint);
					if(m_haveSelect != 0) m_haveSelect = 0;
				}
			}else
			{//���shift������
				if(m_haveSelect == 0)
				{//��ǿ�ʼƬѡ
					m_haveSelect = 0;
					m_selectEnd = m_nowInsertPoint;
					m_selectStart = m_nowInsertPoint;
					//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
					if(m_nowInsertPoint > 0)
					{//��ѡ���ƶ���ͷ
						changeInsertPoint(-m_nowInsertPoint);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//Ƭѡ��Ч
						m_haveSelect = 1;
						upDataSelectShow();
					}else m_haveSelect = 0;	//Ƭѡ��Ч
				}else
				{//�޸�Ƭѡ���
					//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
					if(m_nowInsertPoint > 0)
					{//��ѡ���ƶ���ͷ
						changeInsertPoint(-m_nowInsertPoint);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//Ƭѡ��Ч
						m_haveSelect = 1;
						upDataSelectShow();
					}else m_haveSelect = 0;	//Ƭѡ��Ч
				}
			}
		}else
		if(keyOrder ==  SDLK_END || (m_keyShiftState == m_keyNumLockState && keyOrder ==  SDLK_KP1))
		{//end(���û��Ƭѡ����ʱ������Ч)
			if(m_keyShiftState == 0)
			{//���shiftû�а���
				if(m_selectMouseDown == 0)
				{//���û�����ѡ��(��������ѡ���򰴼���Ч)
					if(m_nowInsertPoint != m_nowStringLength) changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
					if(m_haveSelect != 0) m_haveSelect = 0;
				}
			}else
			{//���shift������
				if(m_haveSelect == 0)
				{//��ǿ�ʼƬѡ
					m_haveSelect = 0;
					m_selectEnd = m_nowInsertPoint;
					m_selectStart = m_nowInsertPoint;
					//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
					if(m_nowInsertPoint < m_nowStringLength)
					{//��ѡ���ƶ���ͷ
						changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//Ƭѡ��Ч
						m_haveSelect = 1;
						upDataSelectShow();
					}else m_haveSelect = 0;	//Ƭѡ��Ч
				}else
				{//�޸�Ƭѡ���
					//Ȼ���ƶ���꣬���޸�Ƭѡ״̬
					if(m_nowInsertPoint < m_nowStringLength)
					{//��ѡ���ƶ���ͷ
						changeInsertPoint(m_nowStringLength - m_nowInsertPoint);
						m_selectEnd = m_nowInsertPoint;
					}
					if(m_selectEnd != m_selectStart)
					{//Ƭѡ��Ч
						m_haveSelect = 1;
						upDataSelectShow();
					}else m_haveSelect = 0;	//Ƭѡ��Ч
				}
			}
		}

		if(TextChange != 0)
		{
			//����ַ��������ı䣬��Ҫ�����ַ���������
			upDataShowStr();	//�����ַ�������ʾ
			if(m_funInputChenge != NULL) (* m_funInputChenge)(m_pClass,m_objectID);
			return 1;
		}
		if(keyOrder ==  SDLK_RETURN || keyOrder == SDLK_KP_ENTER)
		{//�س���
			if(m_haveSelect != 0) m_haveSelect = 0;
			if(m_funInputOver != NULL) (* m_funInputOver)(m_pClass,m_objectID);
			return 1;
		}
	}
	return 0;
}

void _XEdit::setPosition(const _XVector2& position)
{
	setPosition(position.x,position.y);
}

void _XEdit::setPosition(float x,float y)
{
	if(m_isInited == 0) return;	//���û�г�ʼ��ֱ���˳�
	m_position.set(x,y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_spriteBackGround.setPosition(m_position);
	upDateInsertShowPosition();
	upDataSelectShow();
}

void _XEdit::setSize(const _XVector2& size)
{
	setSize(size.x,size.y);
}

void _XEdit::setSize(float x,float y)
{
	if(x <= 0 && y <= 0) return;
	if(m_isInited == 0) return;	//���û�г�ʼ��ֱ���˳�
	m_size.set(x,y);
	m_caption.setPosition(m_position.x + m_textPosition.x * m_size.x,m_position.y + m_textPosition.y * m_size.y);
	m_caption.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
	m_nowMouseRect.set(m_position.x + m_mouseRect.left * m_size.x,m_position.y + m_mouseRect.top * m_size.y,
		m_position.x + m_mouseRect.right * m_size.x,m_position.y + m_mouseRect.bottom * m_size.y);
	m_nowTextWidth = m_caption.getTextSize().x * m_caption.getSize().x * 0.5f;
	m_nowTextHeight = m_caption.getTextSize().y * m_caption.getSize().y;
//	m_selectText.setPosition(m_objRect.left + (m_edgeDistance.left + m_textPosition.x) * m_size.x,m_objRect.top + (m_edgeDistance.top + m_textPosition.y) * m_size.y);
//	m_selectText.setSize(m_size);
	//m_spriteBackGround.setPosition(m_position);
	m_spriteBackGround.setSize(m_size);
	upDateInsertShowPosition();
	//m_spriteSelect.setSize(m_size);
	upDataSelectShow();
	m_spriteInsert.setSize(m_textSize.x * m_size.x,m_textSize.y * m_size.y);
}

int _XEdit::setACopy(const _XEdit &temp)			//����Ϊһ���������ò�����Դ		
{
	if(& temp == this) return 1;	//��ֹ����ֵ
	if(temp.m_isInited == 0) return 0;
	if(m_isInited != 0) release();//��������Ѿ���ʼ��������Ҫ�ͷ���Դ
	if(_XControlBasic::setACopy(temp) == 0) return 0;
	if(m_isInited == 0)
	{
		_XControlManager::GetInstance().addAObject(this,CTRL_OBJ_EDIT);	//�������������ע�ᵱǰ���
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().addAObject(this,OBJ_CONTROL);
#endif
	}

	m_isInited = temp.m_isInited;	//�Ƿ��ʼ��
	if(temp.m_mouseRightButtonMenu != NULL)
	{
		try
		{
			m_mouseRightButtonMenu = new _XMouseRightButtonMenu;
			if(m_mouseRightButtonMenu == NULL) return 0;
		}catch(...)
		{
			return 0;
		}
		m_mouseRightButtonMenu->setACopy(* (temp.m_mouseRightButtonMenu));	//����Ҽ��˵�
		//�ӿؼ��������н�����ؼ�ע����
		_XControlManager::GetInstance().decreaseAObject(m_mouseRightButtonMenu);	//ע��������
#if WITH_OBJECT_MANAGER
		_XObjectManager::GetInstance().decreaseAObject(m_mouseRightButtonMenu);
#endif
	}

	if(m_caption.setACopy(temp.m_caption) == 0)	return 0;		//��ʾ��ǰ������ַ���
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_caption);
#endif
	try
	{
		m_nowString = new char[MAX_INPUT_STRING_LENGTH];	//��ǰ������ַ���
		if(m_nowString == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	strcpy(m_nowString,temp.m_nowString);
	try
	{
		m_tempNowString = new char[MAX_INPUT_STRING_LENGTH];	//��ǰ������ַ���
		if(m_tempNowString == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	strcpy(m_tempNowString,temp.m_tempNowString);
	try
	{
		m_nowShowString = new char[MAX_INPUT_STRING_LENGTH];	//��ǰ������ַ���
		if(m_nowShowString == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	strcpy(m_nowShowString,temp.m_nowShowString);

	m_nowInsertPoint = temp.m_nowInsertPoint;	//��ǰѡ�������λ��,�����ַ���������λ��
	m_nowStringLength = temp.m_nowStringLength;	//��ǰ�����ַ����ĳ���
	m_haveSelect = temp.m_haveSelect;		//�Ƿ����ַ�������ѡ��
	m_selectMouseDown = temp.m_selectMouseDown;	//����Ƿ���׼��Ƭѡ
	m_selectStart = temp.m_selectStart;		//ѡ�����ʼλ��
	m_selectEnd = temp.m_selectEnd;		//ѡ��Ľ���λ��

	m_editNormal = temp.m_editNormal;		//�������ͨ״̬
	m_editDisable = temp.m_editDisable;		//�������Ч״̬
	m_editSelect = temp.m_editSelect;		//�����Ƭѡ��ɫ
	m_editInsert = temp.m_editInsert;		//����������
	m_editUpon = temp.m_editUpon;		//����������

	m_spriteBackGround.setACopy(temp.m_spriteBackGround);	//������ʾ�����ı�����ͼ
	m_spriteSelect.setACopy(temp.m_spriteSelect);		//������ʾѡ�����ݵı�����ɫ
	m_spriteInsert.setACopy(temp.m_spriteInsert);		//������ʾ�������
#if WITH_OBJECT_MANAGER
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteBackGround);
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteSelect);
	_XObjectManager::GetInstance().decreaseAObject(&m_spriteInsert);
#endif
	m_textPosition = temp.m_textPosition;			//������ʾ��λ�ã�������ڿؼ���λ��������
	m_textSize = temp.m_textSize;				//������ʾ�ĳߴ磬����ߴ����ռ�����ųߴ����

	m_funInputChenge = temp.m_funInputChenge;		//�������ݷ����ı��ʱ�����
	m_funInputOver = temp.m_funInputOver;		//ȷ���������֮���ʱ�����

	m_timer = temp.m_timer;	//���ǲ��������˸ʱʹ�õ�ʱ���ǣ����ʱ��ʹ�ò�׼ȷ�ļ�ʱ��ʽ
	//���������ڰ����жϵ��м����
	m_keyShiftState = temp.m_keyShiftState;		//shift������״̬	0����1����
	m_keyCapsLockState = temp.m_keyCapsLockState;	//CapsLock������״̬	1��Ч��0��Ч
	m_keyNumLockState = temp.m_keyNumLockState;		//NumLock������״̬	1��Ч��0��Ч
	m_keyCtrlState = temp.m_keyCtrlState;		//Ctrl������״̬ 0����1����

	m_canShowLength = temp.m_canShowLength;		//������п�����ʾ���ַ����ĳ���
	m_nowShowStart = temp.m_nowShowStart;			//��ǰ���������ʾ���ַ�������ʼλ��
	m_pClass = temp.m_pClass;

	m_textColor = temp.m_textColor;
	m_nowTextWidth = temp.m_nowTextWidth;
	m_nowTextHeight = temp.m_nowTextHeight;
	return 1;
}

int _XEdit::keyJudgement(int keyOrder)
{
	int TextChange = 0;
	char tempChar = 0;
//	printf("%d\n",keyOrder);
	if(keyOrder == SDLK_SPACE)
	{
		tempChar = ' ';
		TextChange = 1;
	}else
	if(m_keyShiftState == 0)
	{
		switch(keyOrder)
		{
		case SDLK_0:
			tempChar = '0';
			TextChange = 1;
			break;
		case SDLK_1:
			tempChar = '1';
			TextChange = 1;
			break;
		case SDLK_2:
			tempChar = '2';
			TextChange = 1;
			break;
		case SDLK_3:
			tempChar = '3';
			TextChange = 1;
			break;
		case SDLK_4:
			tempChar = '4';
			TextChange = 1;
			break;
		case SDLK_5:
			tempChar = '5';
			TextChange = 1;
			break;
		case SDLK_6:
			tempChar = '6';
			TextChange = 1;
			break;
		case SDLK_7:
			tempChar = '7';
			TextChange = 1;
			break;
		case SDLK_8:
			tempChar = '8';
			TextChange = 1;
			break;
		case SDLK_9:
			tempChar = '9';
			TextChange = 1;
			break;
		//���Ű���
		case SDLK_BACKQUOTE:
			tempChar = '`';
			TextChange = 1;
			break;
		case SDLK_MINUS:
			tempChar = '-';
			TextChange = 1;
			break;
		case SDLK_EQUALS:
			tempChar = '=';
			TextChange = 1;
			break;
		case SDLK_LEFTBRACKET:
			tempChar = '[';
			TextChange = 1;
			break;
		case SDLK_RIGHTBRACKET:
			tempChar = ']';
			TextChange = 1;
			break;
		case SDLK_SEMICOLON:
			tempChar = ';';
			TextChange = 1;
			break;
		case SDLK_QUOTE:
			tempChar = '\'';
			TextChange = 1;
			break;
		case SDLK_BACKSLASH:
			tempChar = '\\';
			TextChange = 1;
			break;
		case SDLK_COMMA:
			tempChar = ',';
				TextChange = 1;
			break;
		case SDLK_PERIOD:
			tempChar = '.';
			TextChange = 1;
			break;
		case SDLK_SLASH:
			tempChar = '/';
			TextChange = 1;
			break;
		}
	}else
	{
		switch(keyOrder)
		{
		case SDLK_0:
			tempChar = ')';
			TextChange = 1;
			break;
		case SDLK_1:
			tempChar = '!';
			TextChange = 1;
			break;
		case SDLK_2:
			tempChar = '@';
			TextChange = 1;
			break;
		case SDLK_3:
			tempChar = '#';
			TextChange = 1;
			break;
		case SDLK_4:
			tempChar = '$';
			TextChange = 1;
			break;
		case SDLK_5:
			tempChar = '%';
			TextChange = 1;
			break;
		case SDLK_6:
			tempChar = '^';
			TextChange = 1;
			break;
		case SDLK_7:
			tempChar = '&';
			TextChange = 1;
			break;
		case SDLK_8:
			tempChar = '*';
			TextChange = 1;
			break;
		case SDLK_9:
			tempChar = '(';
			TextChange = 1;
			break;
			//���Ű���
		case SDLK_BACKQUOTE:
			tempChar = '~';
			TextChange = 1;
			break;
		case SDLK_MINUS:
			tempChar = '_';
			TextChange = 1;
			break;
		case SDLK_EQUALS:
			tempChar = '+';
			TextChange = 1;
			break;
		case SDLK_LEFTBRACKET:
			tempChar = '{';
			TextChange = 1;
			break;
		case SDLK_RIGHTBRACKET:
			tempChar = '}';
			TextChange = 1;
			break;
		case SDLK_SEMICOLON:
			tempChar = ':';
			TextChange = 1;
			break;
		case SDLK_QUOTE:
			tempChar = '\"';
			TextChange = 1;
			break;
		case SDLK_BACKSLASH:
			tempChar = '|';
			TextChange = 1;
			break;
		case SDLK_COMMA:
			tempChar = '<';
			TextChange = 1;
			break;
		case SDLK_PERIOD:
			tempChar = '>';
			TextChange = 1;
			break;
		case SDLK_SLASH:
			tempChar = '?';
			TextChange = 1;
			break;
		}
	}
	//��ĸ��
	if(TextChange == 0)
	{
	//	if((m_keyShiftState == 0 && m_keyCapsLockState == 0) || (m_keyShiftState == 1 || m_keyCapsLockState == 1))
		if(m_keyShiftState == m_keyCapsLockState)
		{
			switch(keyOrder)
			{
			case SDLK_a:
				tempChar = 'a';
				TextChange = 1;
				break;
			case SDLK_b:
				tempChar = 'b';
				TextChange = 1;
				break;
			case SDLK_c:
				tempChar = 'c';
				TextChange = 1;
				break;
			case SDLK_d:
				tempChar = 'd';
				TextChange = 1;
				break;
			case SDLK_e:
				tempChar = 'e';
				TextChange = 1;
				break;
			case SDLK_f:
				tempChar = 'f';
				TextChange = 1;
				break;
			case SDLK_g:
				tempChar = 'g';
				TextChange = 1;
				break;
			case SDLK_h:
				tempChar = 'h';
				TextChange = 1;
				break;
			case SDLK_i:
				tempChar = 'i';
				TextChange = 1;
				break;
			case SDLK_j:
				tempChar = 'j';
				TextChange = 1;
				break;
			case SDLK_k:
				tempChar = 'k';
				TextChange = 1;
				break;
			case SDLK_l:
				tempChar = 'l';
				TextChange = 1;
				break;
			case SDLK_m:
				tempChar = 'm';
				TextChange = 1;
				break;
			case SDLK_n:
				tempChar = 'n';
				TextChange = 1;
				break;
			case SDLK_o:
				tempChar = 'o';
				TextChange = 1;
				break;
			case SDLK_p:
				tempChar = 'p';
				TextChange = 1;
				break;
			case SDLK_q:
				tempChar = 'q';
				TextChange = 1;
				break;
			case SDLK_r:
				tempChar = 'r';
				TextChange = 1;
				break;
			case SDLK_s:
				tempChar = 's';
				TextChange = 1;
				break;
			case SDLK_t:
				tempChar = 't';
				TextChange = 1;
				break;
			case SDLK_u:
				tempChar = 'u';
				TextChange = 1;
				break;
			case SDLK_v:
				tempChar = 'v';
				TextChange = 1;
				break;
			case SDLK_w:
				tempChar = 'w';
				TextChange = 1;
				break;
			case SDLK_x:
				tempChar = 'x';
				TextChange = 1;
				break;
			case SDLK_y:
				tempChar = 'y';
				TextChange = 1;
				break;
			case SDLK_z:
				tempChar = 'z';
				TextChange = 1;
				break;
			}
		}else
		{
			switch(keyOrder)
			{
			case SDLK_a:
				tempChar = 'A';
				TextChange = 1;
				break;
			case SDLK_b:
				tempChar = 'B';
				TextChange = 1;
				break;
			case SDLK_c:
				tempChar = 'C';
				TextChange = 1;
				break;
			case SDLK_d:
				tempChar = 'D';
				TextChange = 1;
				break;
			case SDLK_e:
				tempChar = 'E';
				TextChange = 1;
				break;
			case SDLK_f:
				tempChar = 'F';
				TextChange = 1;
				break;
			case SDLK_g:
				tempChar = 'G';
				TextChange = 1;
				break;
			case SDLK_h:
				tempChar = 'H';
				TextChange = 1;
				break;
			case SDLK_i:
				tempChar = 'I';
				TextChange = 1;
				break;
			case SDLK_j:
				tempChar = 'J';
				TextChange = 1;
				break;
			case SDLK_k:
				tempChar = 'K';
				TextChange = 1;
				break;
			case SDLK_l:
				tempChar = 'L';
				TextChange = 1;
				break;
			case SDLK_m:
				tempChar = 'M';
				TextChange = 1;
				break;
			case SDLK_n:
				tempChar = 'N';
				TextChange = 1;
				break;
			case SDLK_o:
				tempChar = 'O';
				TextChange = 1;
				break;
			case SDLK_p:
				tempChar = 'P';
				TextChange = 1;
				break;
			case SDLK_q:
				tempChar = 'Q';
				TextChange = 1;
				break;
			case SDLK_r:
				tempChar = 'R';
				TextChange = 1;
				break;
			case SDLK_s:
				tempChar = 'S';
				TextChange = 1;
				break;
			case SDLK_t:
				tempChar = 'T';
				TextChange = 1;
				break;
			case SDLK_u:
				tempChar = 'U';
				TextChange = 1;
				break;
			case SDLK_v:
				tempChar = 'V';
				TextChange = 1;
				break;
			case SDLK_w:
				tempChar = 'W';
				TextChange = 1;
				break;
			case SDLK_x:
				tempChar = 'X';
				TextChange = 1;
				break;
			case SDLK_y:
				tempChar = 'Y';
				TextChange = 1;
				break;
			case SDLK_z:
				tempChar = 'Z';
				TextChange = 1;
				break;
			}
		}
	}
	//С����
	if(TextChange == 0)
	{
		if(m_keyShiftState != m_keyNumLockState)
		{
			switch(keyOrder)
			{
			case SDLK_KP0:
				tempChar = '0';
				TextChange = 1;
				break;
			case SDLK_KP1:
				tempChar = '1';
				TextChange = 1;
				break;
			case SDLK_KP2:
				tempChar = '2';
				TextChange = 1;
				break;
			case SDLK_KP3:
				tempChar = '3';
				TextChange = 1;
				break;
			case SDLK_KP4:
				tempChar = '4';
				TextChange = 1;
				break;
			case SDLK_KP5:
				tempChar = '5';
				TextChange = 1;
				break;
			case SDLK_KP6:
				tempChar = '6';
				TextChange = 1;
				break;
			case SDLK_KP7:
				tempChar = '7';
				TextChange = 1;
				break;
			case SDLK_KP8:
				tempChar = '8';
				TextChange = 1;
				break;
			case SDLK_KP9:
				tempChar = '9';
				TextChange = 1;
				break;
			case SDLK_KP_PERIOD:
				tempChar = '.';
				TextChange = 1;
				break;
			}
		}
		switch(keyOrder)
		{
		case SDLK_KP_DIVIDE:
			tempChar = '/';
			TextChange = 1;
			break;
		case SDLK_KP_MULTIPLY:
			tempChar = '*';
			TextChange = 1;
			break;
		case SDLK_KP_MINUS:
			tempChar = '-';
			TextChange = 1;
			break;
		case SDLK_KP_PLUS:
			tempChar = '+';
			TextChange = 1;
			break;
		}
	}
	if(TextChange != 0)
	{
		if(m_haveSelect != 0)
		{//����Ƭѡ״̬
			if(m_selectMouseDown == 0)
			{//Ƭѡ�Ѿ�����
				//1��ɾ��Ƭѡ����
				deleteSelectStr();
				m_haveSelect = 0;	//Ƭѡ����

				if(m_nowInsertPoint == m_nowStringLength)
				{//ĩβ
					m_nowString[m_nowInsertPoint] = tempChar;
				//	++ m_nowInsertPoint;
					++ m_nowStringLength;
					m_nowString[m_nowStringLength] = '\0';
					changeInsertPoint(1);
				}else
				{//�м����
					//strcpy(m_nowString + m_nowInsertPoint + 1,m_nowString + m_nowInsertPoint);
					strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
					m_nowString[m_nowInsertPoint] = tempChar;
					strcpy(m_nowString + m_nowInsertPoint + 1,m_tempNowString);
				//	++ m_nowInsertPoint;
					++ m_nowStringLength;
					changeInsertPoint(1);
				}
			}else
			{//Ƭѡ��δ����
				//����������
				if(m_selectEnd != m_selectStart)
				{//Ƭѡ��Ч
					m_haveSelect = 1;
					//���밴��
					//1��ɾ��Ƭѡ����
					deleteSelectStr();
					m_haveSelect = 0;	//Ƭѡ����
					if(m_nowInsertPoint == m_nowStringLength)
					{//ĩβ
						m_nowString[m_nowInsertPoint] = tempChar;
						//++ m_nowInsertPoint;
						++ m_nowStringLength;
						m_nowString[m_nowStringLength] = '\0';
						changeInsertPoint(1);
					}else
					{//�м����
						//strcpy(m_nowString + m_nowInsertPoint + 1,m_nowString + m_nowInsertPoint);
						strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
						m_nowString[m_nowInsertPoint] = tempChar;
						strcpy(m_nowString + m_nowInsertPoint + 1,m_tempNowString);
						//++ m_nowInsertPoint;
						++ m_nowStringLength;
						changeInsertPoint(1);
					}
				}else
				{//Ƭѡ��Ч
					m_haveSelect = 0;
				}
				m_selectMouseDown = 0;
				//Ȼ���������¿�ʼ����
				m_haveSelect = 0;
				m_selectEnd = m_nowInsertPoint;
				m_selectStart = m_nowInsertPoint;
				m_selectMouseDown = 1;
			}
		}else
		{//������Ƭѡ
			if(m_nowInsertPoint == m_nowStringLength)
			{//ĩβ
				m_nowString[m_nowInsertPoint] = tempChar;
				//++ m_nowInsertPoint;
				++ m_nowStringLength;
				m_nowString[m_nowStringLength] = '\0';
				changeInsertPoint(1);
			}else
			{//�м����
				//strcpy(m_nowString + m_nowInsertPoint + 1,m_nowString + m_nowInsertPoint);
				strcpy(m_tempNowString,m_nowString + m_nowInsertPoint);
				m_nowString[m_nowInsertPoint] = tempChar;
				strcpy(m_nowString + m_nowInsertPoint + 1,m_tempNowString);
				//++ m_nowInsertPoint;
				++ m_nowStringLength;
				changeInsertPoint(1);
			}
			if(m_selectMouseDown != 0)
			{//���������Ƭѡ��꣬����Ҫ������
				m_selectEnd = m_nowInsertPoint;
				m_selectStart = m_nowInsertPoint;
			}
		}
	}
	return TextChange;
}

int _XEdit::isInRect(float x,float y)
{
	if(m_isInited == 0) return 0;
	return getIsInRect(_XVector2(x,y),getBox(0),getBox(1),getBox(2),getBox(3));
}

_XVector2 _XEdit::getBox(int order)
{
	_XVector2 ret;
	ret.set(0.0f,0.0f);
	if(m_isInited == 0) return ret;
	if(order == 0) ret.set(m_nowMouseRect.left,m_nowMouseRect.top);else
	if(order == 1) ret.set(m_nowMouseRect.right,m_nowMouseRect.top);else
	if(order == 2) ret.set(m_nowMouseRect.right,m_nowMouseRect.bottom);else
	if(order == 3) ret.set(m_nowMouseRect.left,m_nowMouseRect.bottom);
	return ret;
}