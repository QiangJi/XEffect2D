#ifndef _JIA_XSTRINGFUN_
#define _JIA_XSTRINGFUN_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.1.1
//--------------------------------
//���ﶨ�嶼�����ַ���������ط���
#include "XOSDefine.h"
#include <string>
//���ַ������ҵ������һ������Ҫ����ַ���λ��,ע����������������һ��û�н��������ַ������ᵼ�����ش���
//����-1��ʾ�Ҳ�������ַ�
extern _XBool getIsNumber(const char * p);	//����ַ����Ƿ�Ϊ��ֵ
extern int getCharSum(const char *p,char temp);	//ͳ���ַ������ض��ַ�������
extern int getCharPosition(const char *p,char temp,int mode = 0);	//mode 0:������ң�1:�������
extern _XBool fileNameCompare(const char *p1,const char *p2);	//�Ƚ�����·�������ļ����Ƿ�һ�£������Ǵ�Сд
extern int getPathDeep(const char *p);			//��ȡ·������ȣ���ȡ·��Ŀ¼�Ĳ��� e.g.:c:/x/y/xx.txt return 3
extern int getPathDeepByChar(const char *p);	//��ȡ·������ȣ���ȡ·��Ŀ¼���ַ������ e.g.:c:/x/y/xx.txt return 6
extern _XBool getFirstWord(const char *p,int &s,int &e);		//��ȡ�ַ����еĵ�һ������ e.g:"  xia jia ... " return "xia"
inline  _XBool isAtUnicodeEnd(const char *p,int pos)	//���ַ����е�ǰ��λ���Ƿ�Ϊ�ַ��Ľ���(XTrue)���������ַ����м�(XFalse)
{
	int len = strlen(p);
	if(p == NULL || pos < 0 || pos > len) return XFalse;
	if(pos == 0 || pos == len) return XTrue;
	int index = 0;
	while(index < pos)
	{
		if(p[index] < 0) index += 2;
		else ++ index;
	}
	return (index == pos);
}
//��16���Ƶ�pת����10����
inline int hexCharToInt(char p)	//16�����ַ�תint����ֵ
{
	if(p >= '0' && p <= '9') return p - '0';
	if(p >= 'a' && p <= 'f') return p - 'a' + 10;
	if(p >= 'A' && p <= 'F') return p - 'A' + 10;
	return '0';
}
//��tempת����radix�����ַ���������֧��2���ƣ�8���ƣ�10���ƣ�16����
inline std::string intToStr(int temp,int radix = 16)
{
	char str[128];
	itoa(temp,str,radix);
	return str;
}
#include "windows.h"
inline std::string getWorkPath()
{//��ȡ��ǰ����·��
	char path[512];
	GetCurrentDirectory(512,path);
	return path;
}
inline void setWorkPath(const std::string &path)
{//���õ�ǰ����·��
	SetCurrentDirectory(path.c_str());
}
inline std::string getCurrentExeFileFullPath()
{
	char lpFileName[MAX_PATH];
	GetModuleFileName(NULL,lpFileName,MAX_PATH);
	return lpFileName;
}
inline std::string getFullPath(const std::string &filename)
{
	//ע������û�з���filename��ǰ׺����./����../���������
	//return getWorkPath() + "/" + filename;
	char currentPath[MAX_PATH + 1];
	GetCurrentDirectory(MAX_PATH,currentPath);	//��¼��ǰ��·��
	int len = getCharPosition(filename.c_str(),'\\',1);
	int len1 = getCharPosition(filename.c_str(),'/',1);
	if(len < 0 && len1 < 0) return getWorkPath() + "/" + filename;
	
	char path[MAX_PATH + 1];
	char path1[MAX_PATH + 1];
	strcpy(path,filename.c_str());
	if(len >= len1) len = len + 1;
	else len = len1 + 1;
	path[len] = '\0';
	SetCurrentDirectory(path);  //�õõ������·�����õ�ǰĿ¼
	GetCurrentDirectory(MAX_PATH,path1);  //�õ���ǰĿ¼�ľ���·��
	
	SetCurrentDirectory(currentPath);	//�ָ���ǰ��·��
	strcat(path1,"/");
	strcpy(path,filename.c_str());
	strcat(path1,path + len);
	return  path1;
}
inline std::string getTextFromClipboard()
{//��ճ�����ж�ȡ����
	HWND h = FindWindow(NULL,"Control");
	if(!OpenClipboard(h)) return ""; 
	if(!IsClipboardFormatAvailable(CF_TEXT)) return ""; 
	HGLOBAL hglb = GetClipboardData(CF_TEXT); 
    if(hglb == NULL) return "";
    LPVOID lptstr = GlobalLock(hglb);
	GlobalUnlock(hglb); 
	CloseClipboard();
	if(lptstr != NULL) return (char *)lptstr;
	else return "";
}
inline void setTextToClipboard(const std::string & str)
{//������д�뵽ճ����
	HWND h = FindWindow(NULL,"Control");
	if(!OpenClipboard(h)) return; 
	size_t size = sizeof(TCHAR)*(1 + str.size());
	HGLOBAL hResult = GlobalAlloc(GMEM_MOVEABLE,size); 
	if(hResult == NULL)
	{	//�ڴ����ʧ��
		return;
		CloseClipboard();
	}
	LPTSTR lptstrCopy = (LPTSTR)GlobalLock(hResult);
	if(lptstrCopy == NULL)
	{
		GlobalFree(hResult);
		CloseClipboard();
	}
	memcpy(lptstrCopy,str.c_str(), size); 
	GlobalUnlock(hResult); 
#ifndef _UNICODE
	if(SetClipboardData(CF_TEXT,hResult) == NULL)
#else
	if(SetClipboardData(CF_UNICODETEXT,hResult) == NULL)
#endif
	{
		printf("Unable to set Clipboard data");
		GlobalFree(hResult); // Edit by Leo Davidson 16/Sep/2010: Free buffer if clipboard didn't take it.
		CloseClipboard();
		return;
	}

	CloseClipboard();
}
//���ַ���tempװ���ɶ�Ӧ������������֧��2���ƣ�8���ƣ�10���ƣ�16����
//ע���������û�п�����Խ�����⣬���ַ�����ʾ�����ݳ���int���ܱ�ʾ�����Χʱ�������������ִ���
//���������ַ������ƶ��������ݲ���ʱ���ú���û�н��м�飬������ɴ���
extern int strToInt(const std::string &temp,int radix = 10);
//�ַ�ת���ķ���
extern char* localeToUTF8(const char* src);	//�ַ�ת��
extern char* UTF8ToLocale(const char* src);	
extern wchar_t* charToWchar(const char* src);
extern wchar_t oneCharToWchar(const char* src);
#endif