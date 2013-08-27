#ifndef _JIA_XFONTUNICODE_
#define _JIA_XFONTUNICODE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XFontBasic.h"

//���齫һЩ�������Ե����ݷ��������ı��ļ�����


//ע��ҳ��ֵ�ڴ����б��޶���಻�ܳ���100ҳ
//����ַ�����ʹ��һ����������ͼ
#define MAX_FONT_UNICODE_FILE_NAME_LENGTH 256	//�ַ���ͼƬ���ļ�������󳤶�
#define UNICODE_FONT_PAGE_SUM 10	//���ַ�����ҳ������
#define MAX_UNICODE_TEXT_SUM 16384	//�����������Ԫ�ص���������
#define UNICODE_BYTES_WIDTH 2		//���ֽڵ��ֿ��Ϊ2

//���������Ӳü�����
class _XFontUnicode:public _XFontBasic
{
private:
	_XSCounter *m_cp;	//���ü�����

	int m_pageW;			//����ҳ�����سߴ�
	int m_pageH;
	char *m_fontPageText;	//�����ҳ����������Ų��ı�
	int m_libFontSum;		//�������ʵ�ʵ�Ԫ������
	char m_isVisiable;

private:
	void updateData();		//������Ҫ�����ڲ�����
	_XVector2 *m_textPosition;	//���ڱ���ÿ���ַ���λ��
	_XRect *m_textRect;		//���ڱ���ÿ���ַ��ļ��к���
	int *m_textPageOrder;	//��ʾ���ַ�ͼƬ�����б��

	//+++++++++++++++++++++++++++
	//���������Ӳü�����
	_XRect m_clipRect;	//����ü��ľ�����������������Ͻǵ�һ����Ծ���
	char m_isCliped;
public:
	void setClipRect(_XRect temp);
	void setClipRect(float left,float top,float right,float bottom);
	void disClip();
	//---------------------------
private:
	_XTextureData *m_texture;		//�������ͼ

	int m_pageSum;
	_XVector2I m_layout;		//����ͼƬ�Ĳ���
	char m_isPassword;
public:
	void setIsPassword(int temp)
	{
		if(temp != 0) temp = 1;
		if(temp != m_isPassword)
		{
			m_isPassword = temp;
			m_needUpdateData = 1;
		}
	}
	char getIsPassword() const
	{
		return m_isPassword;
	}
public:
	int init(const char *filenameIn,	//����ͼƬ���ļ���
		_XVector2I size,			//��������ش�С
		_XVector2I layout,		//����ͼƬ�Ĳ���
		int pageSum,			//������ҳ������
		int resoursePosition = 0);	
	int init(const char *filenameIn,int resoursePosition = 0);	//�򻯰汾������Դ�ж�ȡ�������
	void setVisiable() {m_isVisiable = 1;}					//��������ɼ�
	void disVisiable() {m_isVisiable = 0;}						//����������ɼ�
	char getVisiable() const {return m_isVisiable;}					//��ȡ����Ƿ�ɼ���״̬ 

	void setColor(float r,float g,float b,float a);
	void setColor(const _XFColor& color);
	int getStringLengthPix();	//��ȡ�ַ��������ؿ��(Ϊ�����¼��ݣ���Ȼ��������ӿڣ�ʵ�����Ѿ����µĽӿ������)
private:
	//char m_isAcopy;				//�Ƿ�Ϊһ������
public:
	//����temp��һ������������û����Դ�����õ�����Դ������temp��temp����Դ�ͷ�֮�󣬾ͻ����
	_XFontUnicode& operator = (const _XFontUnicode& temp);
	int setACopy(const _XFontUnicode & temp);
	_XFontUnicode(const _XFontUnicode & temp);	//�������캯��

	void release(){}	//�������¼��ݣ���ʵʲôҲ����
	void setRotateBasePoint(float x,float y);
	void draw();
	_XFontUnicode();
	~_XFontUnicode();
};

#include "XFontUnicode.inl"

#endif