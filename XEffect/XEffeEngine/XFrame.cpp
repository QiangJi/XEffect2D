//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "glew.h"	//������Ҫʹ��shader������Ҫ����ļ���������Ҫ����
#include "XFrame.h"
#include "XBasicSDL.h"
#include "XBasicOpenGL.h"

#include "XResourcePack.h"
#include "XLogBook.h"
//_XResourcePack ResourceTemp;

int _XFrame::init(const char *filename,int resoursePosition)
{
	if(m_isInited != 0) 
	{
		AddLogInfoStr("The action have initted!\n");
		return 1;
	}

	if(resoursePosition != RESOURCE_OUTSIDE)
	{
		resoursePosition = RESOURCE_INSIDE;
	}
	m_resoursePosition = resoursePosition;
	try
	{
		m_cp = new _XSCounter;
		if(m_cp == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	//���������ڴ�
	try
	{
		m_texnum = new _XTexture[MAX_FRAME_SUM];
		m_frameName = new char[MAX_FRAME_FILENAME_LENGTH];
		m_keyFrameArray = new int[MAX_FRAME_SUM];

		wre = new GLint[MAX_FRAME_SUM];
		hre = new GLint[MAX_FRAME_SUM];
		xpa = new GLint[MAX_FRAME_SUM];
		ypa = new GLint[MAX_FRAME_SUM];
		if(m_texnum == NULL || m_frameName == NULL || m_keyFrameArray == NULL
			|| wre == NULL || hre == NULL || xpa == NULL || ypa == NULL)
		{
			XDELETE_ARRAY(m_texnum);
			XDELETE_ARRAY(m_frameName);
			XDELETE_ARRAY(m_keyFrameArray);
			XDELETE_ARRAY(wre);
			XDELETE_ARRAY(hre);
			XDELETE_ARRAY(xpa);
			XDELETE_ARRAY(ypa);
			return 0;
		}
	}catch(...)
	{
		XDELETE_ARRAY(m_texnum);
		XDELETE_ARRAY(m_frameName);
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(wre);
		XDELETE_ARRAY(hre);
		XDELETE_ARRAY(xpa);
		XDELETE_ARRAY(ypa);
		return 0;
	}

	x = 0;			//�����λ��
	y = 0;
	setAngle(0);
	xsize = 1;		//��������ű���
	ysize = 1;

	strcpy(m_frameName,filename);

	//�����������е�����֡ͼƬ
	//load action's pictrue
	//note:the path of the action format is /namefold/name+action+xxx(frame).xxx
	//get name length
/*	int nameLength =0;
	for(int i = 0;i < 256;++ i)
	{
		if(m_frameName[i] == '\0') 
		{
			nameLength = i;
			break;
		}
	}*/
	int nameLength = strlen(m_frameName);
	if(nameLength <= 0)
	{
		AddLogInfoStr("Action file path error!\n");
		//exit(1);
		return 0;
	}
	//read text file		//�����ȡ����֡�������ı��ĵ�����
	char numble[3];
	char tempFrameName[256];
	strcpy(tempFrameName,m_frameName);

	for(int i = nameLength -1;i >0;-- i)
	{
		if(tempFrameName[i] == '/' || tempFrameName[i] == '\\') 
		{//find the key char
			tempFrameName[i + 1] = 'f';
			tempFrameName[i + 2] = 'r';
			tempFrameName[i + 3] = 'a';
			tempFrameName[i + 4] = 'm';
			tempFrameName[i + 5] = 'e';
			tempFrameName[i + 6] = '.';
			tempFrameName[i + 7] = 't';
			tempFrameName[i + 8] = 'x';
			tempFrameName[i + 9] = 't';
			tempFrameName[i +10] = '\0';
			break;
		}
	}	
	//�������Ҫ���Ƕ��߳���Դ���⣬Ψһ��һ�����߳����Ѿ����˻��⴦��
	if(m_resoursePosition == RESOURCE_INSIDE)
	{
	/*	static int checkedresource = 0;
		if(checkedresource == 0)
		{
			checkedresource = 1;
			if(ResourceTemp.checkCheckData() == 0)
			{//���صĴ������ﶨ����Ϸ
				DebugShow("Resource Data Error!\n");
				while(1)
				{
					Sleep(100);
				}
			}
		}*/

		int lengthTemp = _XResourcePack::GetInstance().getFileLength(tempFrameName);
		if(lengthTemp < 0) return 0;
		unsigned char *p = NULL;
		try
		{
			p = new unsigned char[lengthTemp + 1];
			if(p == NULL) return 0;
		}catch(...)
		{
			return 0;
		}
		_XResourcePack::GetInstance().unpackResource(tempFrameName,p);
		*(p + lengthTemp) = '\0';
		int offset = 0;
		sscanf((char *)(p + offset),"%d:",&m_allKeyFramesSum);
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(m_allKeyFramesSum <= 0 || m_allKeyFramesSum >= MAX_FRAME_SUM)
		{
			AddLogInfoNull("Action text file data error:%s!\n",tempFrameName);
			XDELETE_ARRAY(p);
			return 0;
		}
		sscanf((char *)(p + offset),"%d:",&m_allFramesSum);
		offset += getCharPosition((char *)(p + offset),':') + 1;
		if(m_allFramesSum <= 0 || m_allFramesSum >= MAX_FRAME_SUM)
		{
			AddLogInfoNull("Action text file data error:%s!\n",tempFrameName);
			XDELETE_ARRAY(p);
			return 0;
		}
		{//��ȡ��Ƿ�	D��default, M��menutrue
			char tempFlag = ' ';
			sscanf((char *)(p + offset),"%c:",&tempFlag);
			offset += getCharPosition((char *)(p + offset),':') + 1;
			if(tempFlag == 'D' || tempFlag == 'd')
			{
				for(int i =0;i < m_allFramesSum;++ i)
				{
					m_keyFrameArray[i] = i;
				}
			}else
			{	
				for(int i =0;i < m_allFramesSum;++ i)
				{
					sscanf((char *)(p + offset),"%d,",&m_keyFrameArray[i]);
					offset += getCharPosition((char *)(p + offset),',') + 1;
					if(m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
					{
						AddLogInfoNull("Action text file data error:%s -> %d!\n",tempFrameName,i);
						XDELETE_ARRAY(p);
						return 0;
					}
				}
			}
		}
		{//��ȡ��Ƿ�	D��default, M��menutrue
			char tempFlag = ' ';
			sscanf((char *)(p + offset),"%c:",&tempFlag);
			offset += getCharPosition((char *)(p + offset),':') + 1;
			if(tempFlag == 'D' || tempFlag == 'd')
			{
				for(int i =0;i < m_allFramesSum;++ i)
				{
					m_keyFramePosition[i].set(0,0);
				}
			}else
			{//��ȡƫ����Ϣ
				int temp_x;
				int temp_y;
				int temp_z;
				for(int i =0;i < m_allFramesSum;++ i)
				{
					sscanf((char *)(p + offset),"(%d,%d,%d),",&temp_z,&temp_x,&temp_y);
					offset += getCharPosition((char *)(p + offset),',') + 1;
					offset += getCharPosition((char *)(p + offset),',') + 1;
					offset += getCharPosition((char *)(p + offset),',') + 1;
					m_keyFramePosition[i].set(temp_x,temp_y);
				}
			}
		}
		XDELETE_ARRAY(p);
	}else
	{
		FILE *fp;
		if((fp = fopen(tempFrameName,"rb")) == NULL)
		{
			AddLogInfoStr("Action text file error!\n");
			return 0;
		}

		fscanf(fp,"%d:",&m_allKeyFramesSum);
		if(m_allKeyFramesSum <= 0 || m_allKeyFramesSum >= MAX_FRAME_SUM)
		{
			AddLogInfoNull("Action text file data error:%s!\n",tempFrameName);
			fclose(fp);
			return 0;
		}
		fscanf(fp,"%d:",&m_allFramesSum);
		if(m_allFramesSum <= 0 || m_allFramesSum >= MAX_FRAME_SUM)
		{
			AddLogInfoNull("Action text file data error:%s!\n",tempFrameName);
			fclose(fp);
			return 0;
		}
		{//��ȡ��Ƿ�	D��default, M��menutrue
			char tempFlag = ' ';
			fscanf(fp,"%c:",&tempFlag);
			if(tempFlag == 'D' || tempFlag == 'd')
			{
				for(int i =0;i < m_allFramesSum;++ i)
				{
					m_keyFrameArray[i] = i;
				}
			}else
			{	
				for(int i =0;i < m_allFramesSum;++ i)
				{
					fscanf(fp,"%d,",&m_keyFrameArray[i]);
					if(m_keyFrameArray[i] < 0 || m_keyFrameArray[i] >= m_allKeyFramesSum)
					{
						AddLogInfoNull("Action text file data error:%s -> %d!\n",tempFrameName,i);
						fclose(fp);
						return 0;
					}
				}
			}
		}
		{//��ȡ��Ƿ�	D��default, M��menutrue
			char tempFlag = ' ';
			fscanf(fp,"%c:",&tempFlag);
			if(tempFlag == 'D' || tempFlag == 'd')
			{
				for(int i =0;i < m_allFramesSum;++ i)
				{
					m_keyFramePosition[i].set(0,0);
				}
			}else
			{//��ȡƫ����Ϣ
				int temp_x;
				int temp_y;
				int temp_z;
				for(int i =0;i < m_allFramesSum;++ i)
				{
					fscanf(fp,"(%d,%d,%d),",&temp_z,&temp_x,&temp_y);
					m_keyFramePosition[i].set(temp_x,temp_y);
				}
			}
		}
		fclose(fp);
	}

	strcpy(tempFrameName,m_frameName);
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		numble[0] = '0' + i%10;
		numble[1] = '0' + (i/10)%10;
		numble[2] = '0' + (i/100)%10;
		tempFrameName[nameLength - 5] = numble[0];
		tempFrameName[nameLength - 6] = numble[1];
		tempFrameName[nameLength - 7] = numble[2];

		if(m_texnum[i].load(tempFrameName,m_resoursePosition) == 0)
		{
			AddLogInfoStr("The action pictrue load error!\n");
			return 0;
		}else
		{
			//�������֮�������صļ���
			if(isNPOT(m_texnum[i].m_w,m_texnum[i].m_h)) 
			{
				wre[i] = (int)powf(2.0, ceilf(logf((float)m_texnum[i].m_w)/logf(2.0f)));
				hre[i] = (int)powf(2.0, ceilf(logf((float)m_texnum[i].m_h)/logf(2.0f)));
				xpa[i] = (wre[i] - m_texnum[i].m_w)/2;
				ypa[i] = (hre[i] - m_texnum[i].m_h)/2;
			}else 
			{
				wre[i] = m_texnum[i].m_w;
				hre[i] = m_texnum[i].m_h;
				xpa[i] = 0;
				ypa[i] = 0;
			}
		}
	}
	//Ϊ������֡����ƫ�ƶ��������
	m_centerX = m_texnum[0].m_w * 0.5f + m_keyFramePosition[0].x;			//�ܵ�����֡��������
	m_centerY = m_texnum[0].m_h * 0.5f + m_keyFramePosition[0].y;
	for(int i = 0;i < m_allKeyFramesSum;++ i)
	{
		m_centerDW[i] = m_centerX - (m_texnum[i].m_w * 0.5f + m_keyFramePosition[i].x);
		m_centerDH[i] = m_centerY - (m_texnum[i].m_h * 0.5f + m_keyFramePosition[i].y);
	}

	m_isInited = 1;
	m_isEnd = 0;
	m_isSetEnd = 0;
	return 1;
}

int _XFrame::releaseMem()
{
//	if(m_isACopy == 0 && m_texnum != NULL)
//	{
//		for(int i = 0;i < MAX_FRAME_SUM;++ i)
//		{
//			m_texnum[i].release();
//		}
//	}
	//if(m_haveSelfMemery != 0)
	//{
	//	XDELETE_ARRAY(m_texnum);
	//	//XDELETE_ARRAY(m_width);
	//	//XDELETE_ARRAY(m_height);
	//	XDELETE_ARRAY(m_frameName);
	//	XDELETE_ARRAY(m_keyFrameArray);
	//	XDELETE_ARRAY(wre);
	//	XDELETE_ARRAY(hre);
	//	XDELETE_ARRAY(xpa);
	//	XDELETE_ARRAY(ypa);
	//	m_haveSelfMemery = 0;
	//}
	XDELETE_ARRAY(m_centerDW);
	XDELETE_ARRAY(m_centerDH);
	XDELETE_ARRAY(m_keyFramePosition);

	m_isInited = 0;
	return 1;
}

void _XFrame::draw(const GLuint *pTexnum) const
{//����֡����
	if(m_isVisiable == 0) return;
	if(0 != m_isDisappearAtEnd && 0 != m_isEnd) return;	//�������֮����ʧ

	int temp_Frame = m_keyFrameArray[(int)(m_nowFramesNumble)];

	if(m_pShaderProc != NULL) glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_TEXTURE_2D);

	if(m_isFlash == 0)
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}else
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}
	glEnable(GL_BLEND);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 2*wre[temp_Frame], 0, 2*hre[temp_Frame], -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	//+++++++++++++++++++
	//���������Ҫ�����Ż�������ѵ�һ֡���ڼ���ͼƬ���ģ����Ե�һ֡���Բ��ܷ�������ƫ�ƣ�
	//float dh = m_centerDW[temp_Frame] * xsize * sinAngle + m_centerDH[temp_Frame] * ysize * (cosAngle - 1) + m_centerDH[temp_Frame] * (ysize - 1.0f);
	//float dw = m_centerDW[temp_Frame] * xsize * (1 - cosAngle) + m_centerDH[temp_Frame] * ysize * sinAngle - m_centerDW[temp_Frame] * (xsize - 1.0f);
	float dh = m_centerDW[temp_Frame] * xsize * sinAngle + m_centerDH[temp_Frame] * ysize * cosAngle - m_centerDH[temp_Frame];
	float dw = m_centerDW[temp_Frame] - m_centerDW[temp_Frame] * xsize * cosAngle + m_centerDH[temp_Frame] * ysize * sinAngle;
	int halfW = m_texnum[temp_Frame].m_w >> 1;
	int halfH = m_texnum[temp_Frame].m_h >> 1;
	//-------------------
	glTranslatef(x + m_keyFramePosition[temp_Frame].x + dw + halfW,
		y + m_keyFramePosition[temp_Frame].y - dh +  halfH, 0);
	glRotatef(angle, 0, 0, 1);
	glScalef(xsize, ysize, 0);

	if(m_pShaderProc == NULL)
	{
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, pTexnum[temp_Frame]);
		else glBindTexture(GL_TEXTURE_2D, m_texnum[temp_Frame].m_texture);
	}else
	{
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		if(pTexnum != NULL) glBindTexture(GL_TEXTURE_2D, pTexnum[temp_Frame]);
		else glBindTexture(GL_TEXTURE_2D, m_texnum[temp_Frame].m_texture);

		(* m_pShaderProc)();

		glActiveTexture(GL_TEXTURE0);
	}

	glBegin(GL_QUADS);
	glColor4f(colorRed, colorGreen, colorBlue,alpha);

#ifdef GET_ALL_PIXEL
	XEE_AllPixel += m_texnum[temp_Frame].m_w * m_texnum[temp_Frame].m_h;
#endif
	if(m_isOverturn == 0)
	{
		glTexCoord2d(xpa[temp_Frame], ypa[temp_Frame]);
		glVertex2d(-halfW, -halfH);

		glTexCoord2d(xpa[temp_Frame] + m_texnum[temp_Frame].m_w, ypa[temp_Frame]);
		glVertex2d(halfW, -halfH);

		glTexCoord2d(xpa[temp_Frame] + m_texnum[temp_Frame].m_w, ypa[temp_Frame] + m_texnum[temp_Frame].m_h);
		glVertex2d(halfW, halfH);

		glTexCoord2d(xpa[temp_Frame], ypa[temp_Frame] + m_texnum[temp_Frame].m_h);
		glVertex2d(-halfW, halfH);
	}else
	{
		glTexCoord2d(xpa[temp_Frame], ypa[temp_Frame]);
		glVertex2d(halfW, -halfH);

		glTexCoord2d(xpa[temp_Frame] + m_texnum[temp_Frame].m_w, ypa[temp_Frame]);
		glVertex2d(-halfW, -halfH);

		glTexCoord2d(xpa[temp_Frame] + m_texnum[temp_Frame].m_w, ypa[temp_Frame] + m_texnum[temp_Frame].m_h);
		glVertex2d(-halfW, halfH);

		glTexCoord2d(xpa[temp_Frame], ypa[temp_Frame] + m_texnum[temp_Frame].m_h);
		glVertex2d(halfW, halfH);
	}
	glEnd();
	if(m_pShaderProc != NULL) glUseProgram(0);				//ֹͣʹ��shader

	glPopMatrix();
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_BLEND);

	if(m_pShaderProc != NULL) glPopAttrib();
}

void _XFrame::move(int timeDelay)
{
	if(0 == m_isInited || 0 != m_isEnd) return;
	if(0 != m_isEndImmediately && 0 != m_isSetEnd) m_isEnd = 1;
	m_nowFramesNumble += m_actionSpeed * timeDelay;			//�ƶ�����֡����
	if(m_nowFramesNumble >= m_allFramesSum) 
	{//������ŵ����һ֡�������֡
		m_nowFramesNumble =0;

		if(0 == m_isEndImmediately)
		{//������ǿ������̽����Ķ��������ﲥ�����һ��ѭ��֮�󣬼��ɽ���
			if(0 != m_isSetEnd) m_isEnd =1;
		}
		if(0 == m_isLoop)
		{//����ǲ�ѭ���Ķ�������Ҳ���ԱȽϽ���
			m_isEnd =1;
		}
	}else	
	if(m_nowFramesNumble < 0) 
	{//����ǵ��򲥷ŵĻ��������ж��Ƿ񲥷����(�Լ�������ɴ���)
		m_nowFramesNumble = m_allFramesSum - 0.05f;		//note this 0.05!!

		if(0 == m_isEndImmediately)
		{
			if(0 != m_isSetEnd) m_isEnd =1;
		}
		if(0 == m_isLoop)
		{
			m_isEnd =1;
		}
	}
}

void _XFrame::setAttribute(const _XVector2& position,int loop,int endImmediately,int startFrame,float actionSpeed,int disappearAtEnd,int isOverturn)
{
	if(m_isInited == 0) return; //����Ҫ��ʼ��֮����ܽ����������
	x = position.x;			//�����λ��
	y = position.y;

	if(loop != 0) m_isLoop = 1;
	else m_isLoop = 0;

	if(endImmediately != 0) m_isEndImmediately = 1;
	else m_isEndImmediately = 0;

	m_startFrame = startFrame;
	m_nowFramesNumble = m_startFrame;

	m_actionSpeed = actionSpeed;

	if(disappearAtEnd != 0) m_isDisappearAtEnd = 1;
	else m_isDisappearAtEnd = 0;

	if(m_isOverturn != isOverturn)
	{
		if(isOverturn != 0) m_isOverturn = 1;
		else m_isOverturn = 0;

		m_centerX = m_texnum[0].m_w * 0.5f + m_keyFramePosition[0].x;			//�ܵ�����֡��������
		m_centerY = m_texnum[0].m_h * 0.5f + m_keyFramePosition[0].y;
		for(int i = 0;i < m_allKeyFramesSum;++ i)
		{
			m_keyFramePosition[i].x = 2.0f * m_centerX - m_texnum[i].m_w - m_keyFramePosition[i].x;
			m_centerDW[i] = m_centerX - (m_texnum[i].m_w * 0.5f + m_keyFramePosition[i].x);
			m_centerDH[i] = m_centerY - (m_texnum[i].m_h * 0.5f + m_keyFramePosition[i].y);
		}
	}
}

_XFrame::_XFrame()
:m_isInited(0)
,m_resoursePosition(RESOURCE_OUTSIDE)
,angle(0.0f)
,angleRadian(0.0f)
,sinAngle(0.0f)
,cosAngle(1.0f)
,m_isFlash(0)
,m_pShaderProc(NULL)
,x(0.0f),y(0.0f)
,xsize(1.0f),ysize(1.0f)
,alpha(1.0f)
,colorRed(1.0f)
,colorGreen(1.0f)
,colorBlue(1.0f)
,m_isOverturn(0)	//Ĭ������²���ת
,m_isVisiable(1)
,m_allFramesSum(0)							//����֡��������֡��
,m_allKeyFramesSum(0)						//����֡�����йؼ�֡������
,m_nowFramesNumble(0.0f)					//��ǰ���ŵ��ڼ�֡
,m_actionSpeed(0.0f)						//����֡���ŵ��ٶ�
,m_isLoop(0)								//����֡�Ƿ����ѭ��
,m_startFrame(0)							//����һ֡��ʼ
,m_isEnd(1)								//����֡�Ƿ񲥷����
,m_isEndImmediately(0)					//�����Ƿ�Ϊ���̽�������
,m_isSetEnd(0)							//is end by user			//�Ƿ���������֡����
,m_isDisappearAtEnd(0)					//����֡�Ƿ��ڲ������֮����ʧ
,m_centerX(0.0f)
,m_centerY(0.0f)
//,m_haveSelfMemery(1)	//��־�ڹ��캯���з������ڴ�ռ�
,m_isACopy(0)
,m_cp(NULL)
,m_texnum(NULL)
,m_frameName(NULL)
,m_keyFrameArray(NULL)
,wre(NULL)
,hre(NULL)
,xpa(NULL)
,ypa(NULL)
{
	////���в����
	//m_texnum = new _XTexture[MAX_FRAME_SUM];
	//m_frameName = new char[MAX_FRAME_FILENAME_LENGTH];
	//m_keyFrameArray = new int[MAX_FRAME_SUM];

	//wre = new GLint[MAX_FRAME_SUM];
	//hre = new GLint[MAX_FRAME_SUM];
	//xpa = new GLint[MAX_FRAME_SUM];
	//ypa = new GLint[MAX_FRAME_SUM];
	//˽�п��ܱ仯��
	m_centerDW = new float[MAX_FRAME_SUM];
	m_centerDH = new float[MAX_FRAME_SUM];
	m_keyFramePosition = new _XVector2[MAX_FRAME_SUM];
}

//_XFrame::_XFrame(char withNoMemry)
//:m_isInited(0)
//,m_resoursePosition(RESOURCE_OUTSIDE)
//,angle(0.0f)
//,angleRadian(0.0f)
//,sinAngle(0.0f)
//,cosAngle(1.0f)
//,m_isFlash(0)
//,m_pShaderProc(NULL)
//,x(0.0f),y(0.0f)
//,xsize(1.0f),ysize(1.0f)
//,alpha(1.0f)
//,colorRed(1.0f)
//,colorGreen(1.0f)
//,colorBlue(1.0f)
//,m_isOverturn(0)	//Ĭ������²���ת
//,m_isVisiable(1)
//,m_allFramesSum(0)							//����֡��������֡��
//,m_allKeyFramesSum(0)						//����֡�����йؼ�֡������
//,m_nowFramesNumble(0.0f)					//��ǰ���ŵ��ڼ�֡
//,m_actionSpeed(0.0f)						//����֡���ŵ��ٶ�
//,m_isLoop(0)								//����֡�Ƿ����ѭ��
//,m_startFrame(0)							//����һ֡��ʼ
//,m_isEnd(1)								//����֡�Ƿ񲥷����
//,m_isEndImmediately(0)					//�����Ƿ�Ϊ���̽�������
//,m_isSetEnd(0)							//is end by user			//�Ƿ���������֡����
//,m_isDisappearAtEnd(0)					//����֡�Ƿ��ڲ������֮����ʧ
//,m_centerX(0.0f)
//,m_centerY(0.0f)
//,m_haveSelfMemery(0)	//��־�ڹ��캯���з������ڴ�ռ�
//,m_isACopy(0)
//,m_texnum(NULL)
//,m_frameName(NULL)
//,m_keyFrameArray(NULL)
//,wre(NULL)
//,hre(NULL)
//,xpa(NULL)
//,ypa(NULL)
//{
//	m_centerDW = new float[MAX_FRAME_SUM];
//	m_centerDH = new float[MAX_FRAME_SUM];
//	m_keyFramePosition = new _XVector2[MAX_FRAME_SUM];
//}

_XFrame::~_XFrame()
{
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XDELETE_ARRAY(m_texnum);
		XDELETE_ARRAY(m_frameName);
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(wre);
		XDELETE_ARRAY(hre);
		XDELETE_ARRAY(xpa);
		XDELETE_ARRAY(ypa);
		XDELETE(m_cp);
	}
	releaseMem();
}

_XFrame& _XFrame::operator = (const _XFrame& temp)
{
	if(&temp == this) return * this;	//��ֹû��Ҫ�����ҿ���
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//�ͷ��������Դ
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XDELETE_ARRAY(m_texnum);
		XDELETE_ARRAY(m_frameName);
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(wre);
		XDELETE_ARRAY(hre);
		XDELETE_ARRAY(xpa);
		XDELETE_ARRAY(ypa);
		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;

	m_isVisiable = temp.m_isVisiable;					//����ĽǶ�
	angle = temp.angle;					//����ĽǶ�
	angleRadian = temp.angleRadian;		//���ȱ�־�ĽǶ�
	sinAngle = temp.sinAngle;
	cosAngle = temp.cosAngle;
	x = temp.x;
	y = temp.y;							//�����λ��
	xsize = temp.xsize;
	ysize = temp.ysize;					//��������ųߴ�
	alpha = temp.alpha;					//͸����
	colorRed = temp.colorRed;			//��ɫ
	colorGreen = temp.colorGreen;		//��ɫ
	colorBlue = temp.colorBlue;			//��ɫ
	m_isOverturn = temp.m_isOverturn;	//�Ƿ�x�������ҷ�ת

	m_texnum = temp.m_texnum;

	m_isFlash = temp.m_isFlash;
	m_pShaderProc = temp.m_pShaderProc;

	m_allFramesSum = temp.m_allFramesSum;					//����֡��������֡��
	m_allKeyFramesSum = temp.m_allKeyFramesSum;				//����֡�����йؼ�֡������
	m_nowFramesNumble = temp.m_nowFramesNumble;				//��ǰ���ŵ��ڼ�֡
	m_actionSpeed = temp.m_actionSpeed;						//����֡���ŵ��ٶ�
	m_isLoop = temp.m_isLoop;								//����֡�Ƿ����ѭ��
	m_startFrame = temp.m_startFrame;						//����һ֡��ʼ
	m_isEnd = temp.m_isEnd;									//����֡�Ƿ񲥷����
	m_isEndImmediately = temp.m_isEndImmediately;			//�����Ƿ�Ϊ���̽�������
	m_isSetEnd = temp.m_isSetEnd;							//�Ƿ���������֡����
	m_isDisappearAtEnd = temp.m_isDisappearAtEnd;			//����֡�Ƿ��ڲ������֮����ʧ

	wre = temp.wre;
	hre = temp.hre;
	xpa = temp.xpa;
	ypa = temp.ypa;

	m_frameName = temp.m_frameName;
	m_keyFrameArray = temp.m_keyFrameArray;

	m_centerX = temp.m_centerX;			//�ܵ�����֡��������
	m_centerY = temp.m_centerY;

	memcpy(m_centerDW,temp.m_centerDW,sizeof(float) * MAX_FRAME_SUM);
	memcpy(m_centerDH,temp.m_centerDH,sizeof(float) * MAX_FRAME_SUM);
	memcpy(m_keyFramePosition,temp.m_keyFramePosition,sizeof(_XVector2) * MAX_FRAME_SUM);

	m_isInited = temp.m_isInited;
	m_isACopy = 1;
	return *this;
}

void _XFrame::setACopy(const _XFrame& temp)
{
	if(&temp == this) return;			//��ֹû��Ҫ�����ҿ���
	if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
	//�ͷ��������Դ
	if(m_cp != NULL && -- m_cp->m_counter <= 0)
	{//����û��������Ҫ�ͷ�
		XDELETE_ARRAY(m_texnum);
		XDELETE_ARRAY(m_frameName);
		XDELETE_ARRAY(m_keyFrameArray);
		XDELETE_ARRAY(wre);
		XDELETE_ARRAY(hre);
		XDELETE_ARRAY(xpa);
		XDELETE_ARRAY(ypa);
		XDELETE(m_cp);
	}
	m_cp = temp.m_cp;

	m_isVisiable = temp.m_isVisiable;					//����ĽǶ�
	angle = temp.angle;					//����ĽǶ�
	angleRadian = temp.angleRadian;		//���ȱ�־�ĽǶ�
	sinAngle = temp.sinAngle;
	cosAngle = temp.cosAngle;
	x = temp.x;
	y = temp.y;							//�����λ��
	xsize = temp.xsize;
	ysize = temp.ysize;					//��������ųߴ�
	alpha = temp.alpha;					//͸����
	colorRed = temp.colorRed;			//��ɫ
	colorGreen = temp.colorGreen;		//��ɫ
	colorBlue = temp.colorBlue;			//��ɫ
	m_isOverturn = temp.m_isOverturn;	//�Ƿ�x�������ҷ�ת
	
	m_texnum = temp.m_texnum;

	m_isFlash = temp.m_isFlash;
	m_pShaderProc = temp.m_pShaderProc;

	m_allFramesSum = temp.m_allFramesSum;					//����֡��������֡��
	m_allKeyFramesSum = temp.m_allKeyFramesSum;				//����֡�����йؼ�֡������
	m_nowFramesNumble = temp.m_nowFramesNumble;				//��ǰ���ŵ��ڼ�֡
	m_actionSpeed = temp.m_actionSpeed;						//����֡���ŵ��ٶ�
	m_isLoop = temp.m_isLoop;								//����֡�Ƿ����ѭ��
	m_startFrame = temp.m_startFrame;						//����һ֡��ʼ
	m_isEnd = temp.m_isEnd;									//����֡�Ƿ񲥷����
	m_isEndImmediately = temp.m_isEndImmediately;			//�����Ƿ�Ϊ���̽�������
	m_isSetEnd = temp.m_isSetEnd;							//�Ƿ���������֡����
	m_isDisappearAtEnd = temp.m_isDisappearAtEnd;			//����֡�Ƿ��ڲ������֮����ʧ

	wre = temp.wre;
	hre = temp.hre;
	xpa = temp.xpa;
	ypa = temp.ypa;

	m_frameName = temp.m_frameName;
	m_keyFrameArray = temp.m_keyFrameArray;

	m_centerX = temp.m_centerX;			//�ܵ�����֡��������
	m_centerY = temp.m_centerY;

	memcpy(m_centerDW,temp.m_centerDW,sizeof(float) * MAX_FRAME_SUM);
	memcpy(m_centerDH,temp.m_centerDH,sizeof(float) * MAX_FRAME_SUM);
	memcpy(m_keyFramePosition,temp.m_keyFramePosition,sizeof(_XVector2) * MAX_FRAME_SUM);

	m_isInited = temp.m_isInited;
	m_isACopy = 1;
}

_XFrame::_XFrame(const _XFrame &temp)
{
	if(&temp == this) return;			//��ֹû��Ҫ�����ҿ���
}