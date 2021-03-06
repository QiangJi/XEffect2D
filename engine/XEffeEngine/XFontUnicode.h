#ifndef _JIA_XFONTUNICODE_
#define _JIA_XFONTUNICODE_
//++++++++++++++++++++++++++++++++
//Author:	贾胜华(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XFontBasic.h"

//建议将一些字体属性的内容放在字体文本文件当中


//注意页面值在代码中被限定最多不能超过100页
//这个字符的类使用一张完整的贴图
#define MAX_FONT_UNICODE_FILE_NAME_LENGTH (256)	//字符串图片的文件名的最大长度
#define UNICODE_FONT_PAGE_SUM (24)	//款字符串的页面数量(10)
#define MAX_UNICODE_TEXT_SUM (10000)	//字体库中字体元素的数量上限(16384)
#define UNICODE_BYTES_WIDTH (2)		//宽字节的字宽度为2

//给字体增加裁减功能
class _XFontUnicode:public _XFontBasic
{
private:
	_XSCounter *m_cp;	//引用计数器

	int m_pageW;			//字体页面像素尺寸
	int m_pageH;
	char *m_fontPageText;	//字体库页面中字体的排布文本
	int m_libFontSum;		//字体库中实际的元素数量

	void updateData();		//根据需要更新内部数据
	_XVector2 *m_textPosition;	//用于保存每个字符的位置
	_XRect *m_textRect;		//用于保存每个字符的剪切盒子
	int *m_textPageOrder;	//显示的字符图片的序列编号

	_XTextureData *m_texture;		//字体的贴图
	int m_pageSum;
	_XVector2 m_layout;		//字体图片的布局

public:
	_XBool init(const char *filenameIn,	//字体图片的文件名
		_XVector2 size,			//字体的像素大小
		_XVector2 layout,		//字体图片的布局
		int pageSum,			//字体库的页面数量
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	
	_XBool initEx(const char *filenameIn,_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE);	//简化版本，从资源中读取相关数据
	_XBool initFromTTF(const char *filenameIn,int fontSize,
		_XResourcePosition resoursePosition = RESOURCE_SYSTEM_DEFINE,bool withOutLine = false);	//从TTF中初始化（尚未实现）

	int getStringLengthPix();	//获取字符串的像素宽度(为了向下兼容，任然保留这个接口，实际上已经有新的接口来替代)
private:
	//char m_isAcopy;				//是否为一个备份
public:
	//建立temp的一个副本，自身没有资源，是用到的资源依赖于temp，temp的资源释放之后，就会出错
	_XFontUnicode& operator = (const _XFontUnicode& temp);
	_XBool setACopy(const _XFontUnicode & temp);
	_XFontUnicode(const _XFontUnicode & temp);	//拷贝构造函数

	void release(){}	//保持向下兼容，其实什么也不干
	void setRotateBasePoint(float x,float y);
	void draw();
	_XFontUnicode();
	_XFontUnicode(int maxStrLen);
	virtual ~_XFontUnicode();
	//改变字符串的最大长度
	void setMaxStrLen(int maxStrLen);
};

#include "XFontUnicode.inl"

#endif