#ifndef _JIA_XBASICOPENGL_
#define _JIA_XBASICOPENGL_
//#include "gl.h"
//#include "glew.h"
//#include "stdlib.h"
#include "XBasicFun.h"
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//�����ڲ���ʽ			��ɫ�ɷ�	��ɫ�ɷ�	��ɫ�ɷ�	alpha�ɷ�
//GL_ALPHA				���ı�		���ı�		���ı�		At
//GL_LUMINANCE			Lt			Lt			Lt			���ı�
//GL_LUMINANCE_ALPHA	Lt			Lt			Lt			At
//GL_INTENSITY			It			It			It			It
//GL_RGB				Rt			Gt			Bt			���ı�
//GL_RGBA				Rt			Gt			Bt			At
enum _XBlendType
{
	BLEND_TWO_DATA,		//�������ӵĻ��
	BLEND_FOUR_DATA,		//�ĸ����ӵĻ��
};
extern unsigned int srcBlendMode[9];
extern unsigned int dstBlendMode[8];

extern void printShaderInfoLog(GLuint obj);						//���shader����Ϣ�ĺ���
extern int printOglError(const char *file, int line);					//��ʾOpenGL�Ĵ�����Ϣ
#define printOpenGLError() printOglError(__FILE__, __LINE__)	//��ӡ����ִ�д���ĺ���
extern void printProgramInfoLog(GLuint obj);					//��ӡ������Ϣ�ĺ���

extern char *textFileRead(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//��filename���ļ��ж�ȡshader����	//��Դλ�ã�0Ϊ�ⲿ��Դ��Ĭ��Ϊ�ⲿ��Դ
extern int textFileWrite(const char *filename, const char *s);					//��shader����sд�뵽filename���ļ���
extern _XBool setShader(const char* vertFile,const char* fragFile,int &pHandle,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//��Դλ�ã�0Ϊ�ⲿ��Դ��Ĭ��Ϊ�ⲿ��Դ

extern void changeSize(int h, int w);							//SDL�д��ڸı��¼�����Ӧ����

//extern _XBool TextureLoad(GLuint &texture,const char *filename,int withAlpha,int * w = NULL,int * h = NULL,
//					   _XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//ʹ��SDL������ͼ,Ĭ�ϵ���Դλ��Ϊ�ⲿ
extern _XBool TextureLoadEx(GLuint &texture,const char *filename,int * w = NULL,int * h = NULL,
						 _XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//ʹ��SDL������ͼ,Ĭ�ϵ���Դλ��Ϊ�ⲿ
//����һ��û�н��и�����ĺ����ӿڣ�����֮ǰȷ����Դ���ϸ���Լ��
extern _XBool TextureLoadQuarter(GLuint &texture,const char *filename);	//���ķ�֮һ�ߴ��ȡ��Դ

extern _XBool initOpenGL(int width,int height);
extern _XBool initOpenGL3D(int width,int height);
extern _XBool initOpenGLEx(const _XWindowData &windowData);
//��򵥵Ļ��ߺ���
extern void drawLine(int ax,int ay,int bx,int by,int width = 1,float r = 1.0f,float g = 1.0f,float b = 1.0f);
//���ƶ�λ����ָ��λ��֮�仮һ��ֱ�� type 0:ʵ�ߣ�1:����
extern void drawLine(int ax,int ay,int bx,int by,int width,float r,float g,float b,float a,int type = 0);
class _XVector2;
extern void drawLine(const _XVector2& p0,const _XVector2& p1,int width = 1,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,int type = 0);
extern void drawLine(const _XVector2& p,float angle,float length);	//angle[0 - 360]
class _XRect;
//���߿�ʵ�߻�������
extern void drawRect(const _XRect& rect,int width = 1,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f,int type = 0);
//���߿�
extern void drawBox(float x,float y,float sizeX,float sizeY,int w,float r,float g,float b,float a);
extern void drawBox(float x,float y,float sizeX,float sizeY,int w,float r = 1.0f,float g = 1.0f,float b = 1.0f);
extern void drawBox(float x,float y,float sizeX,float sizeY);
extern void drawBox(float x,float y,float sizeX,float sizeY,float angle);
extern void drawBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,int width = 1,float r = 1.0f,float g = 1.0f,float b = 1.0f);
extern void drawBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,int width,float r,float g,float b,float a);
//����
extern void drawPoint(int x,int y,float r,float g,float b,float a);
extern void drawPoint(int x,int y,float size,float r,float g,float b,float a);
//��Բ��
//px,px:λ��
//sr:�뾶
//an:������
extern void drawCircle(float px, float py, float sr, int an,float r = 1.0f,float g = 1.0f,float b = 1.0f,float a = 1.0f);
//��ʵ�ľ���
extern void drawFillBox(const _XVector2& pos,const _XVector2& size,
						float r = 1.0f,float g = 1.0f,float b = 1.0f);	//���ʵ�ľ���
extern void drawFillBoxEx(const _XVector2& pos,const _XVector2& size,
						float r = 1.0f,float g = 1.0f,float b = 1.0f,
						bool withLine = false,bool withRoundCorner = false);	//���ʵ�ľ���
extern void drawFillBox(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,
						float r = 1.0f,float g = 1.0f,float b = 1.0f);	//���ʵ���ı���
extern void drawFillBoxEx(const _XVector2& p0,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,
						float r = 1.0f,float g = 1.0f,float b = 1.0f,
						bool withLine = false);	//���ʵ���ı���
extern void drawFillBoxBW(const _XVector2& pos,const _XVector2& size,const _XVector2& cSize);	//���ڰ�ɫ�ļ��
//����ĺ����������һ����ͼ
class _XTexture;
extern void drawTexture(_XTexture &tex,_XVector2 &pos,int cW,int cH,_XVector2 *vArray,_XVector2 *uArray,int arrayW,int arrayH);	//ʹ��WITH_XSPRITE_EX��ʱ����Ҫ������ͼ��һ��
extern void drawTexture(unsigned int tex,const _XVector2 &pos,int texW,int texH,bool blend = false);
extern void drawTextureEx(unsigned int tex,const _XVector2 &pos,_XVector2 *vArray,_XVector2 *uArray,int arrayW,int arrayH,bool blend = false);
//����һ�ſհ���ͼ0:RGBA,1:RGB,2:�Ҷ�ͼ
extern GLuint emptyTexture(int x,int y,_XColorMode mode = COLOR_RGBA);	//ʹ��RGBA�ĸ�ʽ����һ��ָ����С�Ŀհ���ͼ
//withThread���Ƿ�ʹ�ö��߳�
//����Ļ�ӵ�x��y��ʼ��w��h�����ڵ�ͼ�񱣴浽png�ļ�
extern _XBool gameShot(const char *fileName,int w,int h,bool withThread = true,float x = 0.0f,float y = 0.0f);

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

class _XResourceInfo;
class _XPBO;
class _XTexture	//����ӿڽ���ֻ���ڣ�����������û�вü����ⲿ�ӿ�
{
private:
	_XSCounter *m_cp;	//���ü�����
	void release();	//�ͷ�ͼƬ��Դ
	_XBool m_isLoaded;	//�Ƿ�����
	//char * m_fileName;	//��ͼ��Դ���ļ���
	_XResourceInfo * m_resInfo;	//��Դ��ָ��
public:
	GLuint m_texture;	//��ͼ�ı��:0��Ч����Լ0��Ч
	int m_w;			//��ͼ�����ؿ��
	int m_h;			//��ͼ�����ظ߶�
	_XBool getIsEnable()	//��鵱ǰ��ͼ�Ƿ���Ч
	{
		return m_isLoaded;
	}
	_XColorMode m_mode;	//0:RGBA else RGB
public:
	//resoursePosition��ʾ��Դ��λ�ã�0��ʾ�ⲿ��Դ��1����ʾ�ڲ���Դ��������Դ�ļ����ж�ȡ��Դ	
	_XBool load(const char *filename,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);//��ָ���ļ�������ͼƬ��Դ
	//���������ӿ���δʵ��
	_XBool createTexture(int w,int h,_XColorMode mode);			//����һ��ָ����С�Ŀհ���ͼ
	_XBool createWithTexture(int w,int h,_XColorMode mode,unsigned int tex);	//ֱ��ʹ���Ѿ����ڵ���ͼ
	void updateTexture(const _XPBO &pbo);		//ʹ��ָ�����ݸ�����ͼ
	void updateTextureR2B(const _XPBO &pbo);		//ʹ��ָ�����ݸ�����ͼ
	void updateTexture(const void *data,int w = 0,int h = 0,int x = 0,int y = 0);		//ʹ��ָ�����ݸ�����ͼ
	void updateTextureR2B(const void *data,int w = 0,int h = 0,int x = 0,int y = 0);	//����������ͬ����ͬ���ں���ɫ��������
	//void updateTextureGray(const unsigned char *data);

	_XTexture()
	:m_texture(0)
	,m_isLoaded(XFalse)
	,m_cp(NULL)
	//,m_fileName(NULL)
	,m_mode(COLOR_RGBA)
	,m_resInfo(NULL)
	{}

	~_XTexture()
	{
		if(m_cp != NULL && -- m_cp->m_counter <= 0)
		{
			release();
#if TEX_DEBUG1
			CPSum --;
			printf("%d-\n",CPSum);
#endif
			XDELETE(m_cp);
		//	XDELETE_ARRAY(m_fileName);
		}
	}
	_XTexture(const _XTexture& temp);	//�������캯��
	_XTexture& operator = (const _XTexture &temp);
};
#endif
