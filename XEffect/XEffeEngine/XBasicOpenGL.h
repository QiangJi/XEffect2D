#ifndef _JIA_XBASICOPENGL_
#define _JIA_XBASICOPENGL_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
extern unsigned int srcBlendMode[9];
extern unsigned int dstBlendMode[8];
extern char XEE_isOpenGL2Support;	//OpenGL�Ƿ�֧��2.0�Ƿ�֧��GLSL
extern int XEE_MaxTetureSize;		//�Կ�֧�ֵ������ͼ�ߴ�

extern void printShaderInfoLog(GLuint obj);						//���shader����Ϣ�ĺ���
extern int printOglError(char *file, int line);					//��ʾOpenGL�Ĵ�����Ϣ
#define printOpenGLError() printOglError(__FILE__, __LINE__)	//��ӡ����ִ�д���ĺ���
extern void printProgramInfoLog(GLuint obj);					//��ӡ������Ϣ�ĺ���

extern char *textFileRead(const char *filename,int resoursePosition = 0);	//��filename���ļ��ж�ȡshader����	//��Դλ�ã�0Ϊ�ⲿ��Դ��Ĭ��Ϊ�ⲿ��Դ
extern int textFileWrite(const char *filename, const char *s);					//��shader����sд�뵽filename���ļ���
extern void setShader(const char* vertFile,const char* fragFile,int &pHandle,int resoursePosition = 0);	//��Դλ�ã�0Ϊ�ⲿ��Դ��Ĭ��Ϊ�ⲿ��Դ

extern void changeSize(int h, int w);							//SDL�д��ڸı��¼�����Ӧ����

extern int SDL_Texture(GLuint &texture,const char *filename,int withAlpha,int * w = NULL,int * h = NULL,int resoursePosition = 0);	//ʹ��SDL������ͼ,Ĭ�ϵ���Դλ��Ϊ�ⲿ
extern int SDL_TextureEx(GLuint &texture,const char *filename,int * w = NULL,int * h = NULL,int resoursePosition = 0);	//ʹ��SDL������ͼ,Ĭ�ϵ���Դλ��Ϊ�ⲿ
//����һ��û�н��и�����ĺ����ӿڣ�����֮ǰȷ����Դ���ϸ���Լ��
extern int SDL_TextureQuarter(GLuint &texture,const char *filename);	//���ķ�֮һ�ߴ��ȡ��Դ

extern int initOpenGL(int width,int height);
//���ƶ�λ����ָ��λ��֮�仮һ��ֱ�� type 0:ʵ�ߣ�1:����
extern void drawLine(int ax,int ay,int bx,int by,int width = 1,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,int type = 0);
class _XVector2;
extern void drawLine(const _XVector2& p0,const _XVector2& p1,int width = 1,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,int type = 0);
class _XRect;
extern void drawRect(const _XRect& rect,int width = 1,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,int type = 0);
extern void drawBox(float x,float y,float sizeX,float sizeY);
extern void drawBox(float x,float y,float sizeX,float sizeY,float angle);
//����һ�ſհ���ͼ
extern GLuint EmptyTexture(int x,int y);

extern int gameShot(const char *fileName,int w,int h);	//����Ļ��0 - w ��0 - h�����ڵ�ͼ�񱣴��ͼƬpng�ļ�
extern void clearScreen();					//�����Ļ������
extern void updateScreen();					//������Ļ������

#include "XBasicFun.h"
#include "string.h"
class _XTexture;
class _XTextureInfo;
class _XFrame;			//�򵥲���
class _XFrameEx;
class _XNodeLine;		//�򵥲���
class _XFontUnicode;	//�򵥲���
class _XSCounter	//���ܼ���������
{
private:
	friend _XTexture;
	friend _XTextureInfo;
	friend _XFrame;
	friend _XFrameEx;
	friend _XNodeLine;
	friend _XFontUnicode;

	int m_counter;
	_XSCounter()
		:m_counter(1)
	{
	}
};
#define TEX_DEBUG (0)
#define TEX_DEBUG1 (0)
#if TEX_DEBUG
extern int XTSum;
#endif
#if TEX_DEBUG1
extern int CPSum;
#endif

class _XTexture
{
private:
	_XSCounter *m_cp;	//���ü�����
	void release();	//�ͷ�ͼƬ��Դ
	char m_isLoaded;	//�Ƿ�����
	char * m_fileName;	//��ͼ��Դ���ļ���
public:
	GLuint m_texture;	//��ͼ�ı��:0��Ч����Լ0��Ч
	int m_w;			//��ͼ�����ؿ��
	int m_h;			//��ͼ�����ظ߶�
	char getIsEnable()	//��鵱ǰ��ͼ�Ƿ���Ч
	{
		if(m_isLoaded == 0) return 0;
		return 1;
	}
public:
	//resoursePosition��ʾ��Դ��λ�ã�0��ʾ�ⲿ��Դ��1����ʾ�ڲ���Դ��������Դ�ļ����ж�ȡ��Դ	
	int load(const char *filename,int resoursePosition = 0);//��ָ���ļ�������ͼƬ��Դ

	_XTexture::_XTexture()
	:m_texture(0)
	,m_isLoaded(0)
	,m_cp(NULL)
	,m_fileName(NULL)
	{
	}
	//_XTexture(GLuint tex,int w,int h);
	_XTexture::~_XTexture()
	{

//		if(m_cp != NULL)
//		{
//			printf("|%d+%d\n",m_cp->m_counter,m_texture);
//		}
//#endif
		if(m_cp != NULL && -- m_cp->m_counter <= 0)
		{
			release();
#if TEX_DEBUG1
			CPSum --;
			printf("%d-\n",CPSum);
#endif
			XDELETE(m_cp);
			XDELETE_ARRAY(m_fileName);
		}
	}

	_XTexture(const _XTexture& temp)	//�������캯��
	{
		if(this == &temp) return;		//��ֹ��������
		if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
		m_texture = temp.m_texture;
		m_w = temp.m_w;
		m_h = temp.m_h;
		m_isLoaded = temp.m_isLoaded;
		m_cp = temp.m_cp;
		m_fileName = temp.m_fileName;
		return;	
	}
	_XTexture& operator = (const _XTexture &temp)	//��ֵ������
	{
		if(this == &temp) return *this;		//��ֹ��������
		if(temp.m_cp != NULL) ++temp.m_cp->m_counter;
		if(m_cp != NULL && -- m_cp->m_counter <= 0)
		{
			release();	//���ü����������ͷ���Դ
#if TEX_DEBUG1
			CPSum --;
			printf("%d-\n",CPSum);
#endif
			XDELETE(m_cp);
			XDELETE_ARRAY(m_fileName);
		}
		m_texture = temp.m_texture;
		m_w = temp.m_w;
		m_h = temp.m_h;
		m_isLoaded = temp.m_isLoaded;
		m_cp = temp.m_cp;
		m_fileName = temp.m_fileName;
		return *this;	
	}
};
inline int _XTexture::load(const char *filename,int resoursePosition)//��ָ���ļ�������ͼƬ��Դ
{
	if(m_isLoaded != 0) return 1;	//�����Դ�Ѿ������ˣ������ظ�����
	try
	{
#if TEX_DEBUG1
		CPSum ++;
		printf("%d-\n",CPSum);
#endif
		m_cp = new _XSCounter;
		if(m_cp == NULL) return 0;
	}catch(...)
	{
		return 0;
	}
	try
	{
		m_fileName = new char[MAX_FILE_NAME_LENGTH];
		if(m_fileName == NULL) 
		{
			XDELETE(m_cp);
			return 0;
		}
	}catch(...)
	{
		XDELETE(m_cp);
		return 0;
	}
	strcpy(m_fileName,filename);
	if(SDL_TextureEx(m_texture,filename,&m_w,&m_h,resoursePosition) != 0)
	{
#if TEX_DEBUG
		++ XTSum;
		printf("%d:%d+%s\n",XTSum,m_texture,filename);
#endif
		m_isLoaded = 1;
		return 1;
	}
	XDELETE(m_cp);
	XDELETE_ARRAY(m_fileName);
	m_isLoaded = 0;
	return 0;
	//return SDL_TextureEx(m_texture,filename,&m_w,&m_h,resoursePosition);
}

inline void _XTexture::release()	//�ͷ�ͼƬ��Դ
{
	if(m_isLoaded == 0) return;
#if TEX_DEBUG
	-- XTSum;
	printf("%d-%d\n",XTSum,m_texture);
#endif
	if(glIsTexture(m_texture))
	{
		glDeleteTextures(1, &m_texture);
	}
	m_isLoaded = 0;
}

#define MAX_FBO_TEXTURE_SUM (256)
//�����Ƕ�FBO��֧��
class _XFBO
{
private:
	GLuint m_fboId;								//FBO��ID
	//GLuint m_rboId;							//RBO��ID
	GLuint m_textureId[MAX_FBO_TEXTURE_SUM];    //FBO��Ӧ��ͼ��ID
	int m_nowFBOTextureSum;
public:
	int init(int w,int h); //w��h������ĳߴ磬���س�ʼ���Ƿ�ɹ�
	void useFBO(int windowsW,int windowsH);
	void attachTex(int order);
	void removeFBO();
	void addOneTexture(int w,int h);
	GLuint getTexture(int order)	//��������
	{
		if(order >= 0 && order < m_nowFBOTextureSum)
		{
			return m_textureId[order];
		}
		return -1;
	}
	void release();
	_XFBO()
		:m_nowFBOTextureSum(0)
	{
	}
	~_XFBO()
	{
		release();
	}
};

extern int isFBOSupported();		//�жϵײ��Ƿ�֧��FBO
extern int isFramebufferReady();	//�ж�FBO״̬�Ƿ����

#endif
