#ifndef _JIA_XBASICFUN_
#define _JIA_XBASICFUN_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

#include "XOSDefine.h"
#include "stdio.h"

#define PI 3.1415926535897932384626433832795f
#define PI_HALF 1.5707963267948966192313216916395f								//(PI * 0.5)
#define PI2 6.283185307179586476925286766558f									//(PI * 2.0)
#define RADIAN_TO_ANGLE	57.295779513082320876798154814114f						//(/PI * 180.0)
#define ANGLE_TO_RADIAN	0.01745329251994329576923690768488f						//(/180.0 * PI)

//#define XDELETE_ARRAY(p) { if (p != NULL) { delete [] p; p = NULL; } }
//#define XDELETE(p) { if (p != NULL) { delete p; p = NULL; } }
template<typename T>
void XDELETE(T *&p)
{
	if(p != NULL)
	{
		delete p;
		p = NULL;
	}
}

template<typename T>
void XDELETE_ARRAY(T *&p)
{
	if(p != NULL)
	{
		delete [] p;
		p = NULL;
	}
}

template<typename T>
T * createMem()
{
	T * ret = NULL;
	try
	{
		ret = new T;
		if(ret == NULL) return NULL;
	}catch(...)
	{
		return NULL;
	}
	return ret;
}

template<typename T>
T * createArrayMem(int size)
{
	if(size <= 0) return NULL;
	T * ret = NULL;
	try
	{
		ret = new T[size];
		if(ret == NULL) return NULL;
	}catch(...)
	{
		return NULL;
	}
	return ret;
}

#define MAX_FILE_NAME_LENGTH 256	//�ļ����ĳ���

//#define SIN_BOOK	//sin�����Ƿ�ʹ�ò��ֵ䷽ʽ
#ifdef SIN_BOOK
extern void initSinData();
extern float xSin(float x);
#endif

//#define SQRT_BOOK	//�����Ƿ�ʹ�ò��ֵ䷽ʽ

#ifdef SQRT_BOOK
extern void initSqrtData();
extern float xSqrt(float x);
#endif

//ģ�������뽫����ɱ����������ֻ�����
template<class type> void xChangeTwoSum(type &sum0,type &sum1)
{
	type temp = sum0;
	sum0 = sum1;
	sum1 = temp;
}
//extern void xChangeTwoSum(int &sum0,int &sum1);			//����������������ֵ
//extern void xChangeTwoSum(float &sum0,float &sum1);		//����������������ֵ
//extern void xChangeTwoSum(double &sum0,double &sum1);	//��������˫���ȸ�������ֵ
extern int random(int x);	//ǿ�����������ȡֵ��Χ0 - 0x07ffffff
extern float randomf(float max);
extern float randomf(float x, float y);
extern float randomPS();	//ģ�Ⲵ�ɷֲ����漴��������0.0f - 1.0f����ֵ
//0 - RAND_MAX(0x7fff)		//����һ��0 - x֮�������
extern void randomMomery(char *p,int size);	//���ֽ�Ϊ��λ����p��ָ��Ĵ�СΪsize���ڴ�ռ�

//bits operate
extern int getBit(unsigned char ch,int pos);			//��ȡchĳһλ��ֵ
extern int setBit(unsigned char &ch,int pos,int state);	//����chĳһλ��ֵ
extern int setBitAnti(unsigned char &ch,int pos);		//��chĳһλ��ֵ�÷�

//�ǶȻ���
extern float getAngleXY(float x,float y);	//���ڻ��x,y���ɵĽǶ�
extern float angleAbbreviation(float angle);			//���ȽǶȻ���

//���ַ������ҵ������һ������Ҫ����ַ���λ��,ע����������������һ��û�н��������ַ������ᵼ�����ش���
//����-1��ʾ�Ҳ�������ַ�
extern int getCharPosition(const char *p,char temp,int mode = 0);	//mode 0:������ң�1:�������

extern int fileNameCompare(const char *p1,const char *p2);	//�Ƚ�����·�������ļ����Ƿ�һ�£������Ǵ�Сд
extern int getPathDeep(const char *p);			//��ȡ·������ȣ���ȡ·��Ŀ¼�Ĳ��� e.g.:c:/x/y/xx.txt return 3
extern int getPathDeepByChar(const char *p);	//��ȡ·������ȣ���ȡ·��Ŀ¼���ַ������ e.g.:c:/x/y/xx.txt return 6

extern int getMinWellSize2n(int temp);	//Ѱ�ұ�temp�����С�ķ���2��n�η��ĳߴ�

extern int mySleep(unsigned long sleepUSecond);	//���������ʱ����us��1000000��֮1�룬��1000��֮1����

//���HL1��HL2�ĺ͵�һ�룬�������������������ģ��������û����
extern void getHalfData(unsigned char H1,unsigned char L1,unsigned char H2,unsigned char L2,unsigned char &Ho,unsigned char &Lo);
class _XVector2;
extern void vector4Sort(_XVector2* v,_XVector2* u);//���������е�����ת���ν���˳ʱ������
extern float maping1D(float x,float xmin,float xmax,float ymin,float ymax);//һάӳ��
//4�������Ҫ���Ѿ������˵�
extern int getEdgePoint(float angle,_XVector2 * points,const _XVector2* v,const _XVector2* u);

struct _XSystemTime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int millisecond;
    _XSystemTime()
        :year(0)
        ,month(0)
        ,day(0)
        ,hour(0)
        ,minute(0)
        ,second(0)
        ,millisecond(0)
    {
    }
};

extern _XSystemTime getTimeMs();

#ifdef GET_ALL_PIXEL
extern int XEE_AllPixel;	//�ܵ���ͼ����
#endif

#endif 
