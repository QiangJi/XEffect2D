#ifndef _JIA_XFONT_
#define _JIA_XFONT_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//�л�˵��������ÿ���ַ������ݷ����ı�ʱ����Ҫ���ַ������ڲ����ݽ��и��£��������������Ա�����һ�ε��ַ������ݣ��Ƚϲ�ͬ�Ĳ��ֲŽ��и��¡�
//Ҳ����ֱ�ӽ��ַ���ͨ��FBO����Ϊһ����ͼ��ֱ����ͨ������һ������ʾ�Ͳ���Ҫÿ�ζ����м��㣬
//���������������⣬1���ڴ����Ľϴ�2����ͼ�ĳߴ���������ֵ����ݶ������仯
#include "XFontBasic.h"

class _XFont:public _XFontBasic
{
private:
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
	void updateData();			//������Ҫ�����ڲ�����
	_XVector2 *m_textPosition;	//���ڱ���ÿ���ַ���λ��
	_XRect *m_textRect;		//���ڱ���ÿ���ַ��ļ��к���
private:
	_XVector2I m_layout;		//����ͼƬ�Ĳ���
	char m_isPassword;
	char m_isVisiable;
public:
	void setVisiable() {m_isVisiable = 1;}					//��������ɼ�
	void disVisiable() {m_isVisiable = 0;}						//����������ɼ�
	char getVisiable() const {return m_isVisiable;}					//��ȡ����Ƿ�ɼ���״̬ 
	void setIsPassWord(int temp)
	{
		if(temp != 0) temp = 1;
		if(m_isPassword != temp)
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
	int init(const char *filename,	//����ͼƬ���ļ���
		const _XVector2I& size,		//��������ش�С
		const _XVector2I& layout,		//����ͼƬ�Ĳ���
		int resoursePosition = 0);		
	int init(const char *filename,int resoursePosition = 0);	//����ӿ�,����Դ�ж�ȡ����

	_XFont& operator = (const _XFont& temp);
	int setACopy(const _XFont &temp);
	_XFont(const _XFont &temp);	//�������캯��

	void draw();
	_XFont();
	~_XFont();

	void setColor(float r,float g,float b,float a);
};

#include "XFont.inl"

#endif