#ifndef _JIA_XBASICCLASS_
#define _JIA_XBASICCLASS_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:        2011.4.9
//--------------------------------
#include "XBasicFun.h"

class _XVector2I;

//������2D��
class _XVector2
{
public:
    float x;
    float y;

public:
    void reset();    //���������
	void set(const _XVector2 &temp);
    void set(float a,float b);    //���õ�ǰ���ֵ
    void add(float a,float b);    //����ĳ��ֵ

    _XVector2 operator + (const _XVector2& temp) const;
    _XVector2 operator - (const _XVector2& temp) const;
    _XVector2 operator * (const _XVector2& temp) const;
    _XVector2 operator / (const _XVector2& temp) const;
    _XVector2 operator + (const float& temp) const;
    _XVector2 operator - (const float& temp) const;
    _XVector2 operator * (const float& temp) const;
    _XVector2 operator / (const float& temp) const;
    void operator += (const _XVector2& temp);
    void operator -= (const _XVector2& temp);
    void operator *= (const _XVector2& temp);
    void operator /= (const _XVector2& temp);
    void operator += (const float & temp);
    void operator -= (const float & temp);
    void operator *= (const float & temp);
    void operator /= (const float & temp);

	_XVector2& operator = (const _XVector2I & temp);
	_XVector2(const _XVector2I & temp);

    float getLength(const _XVector2& P0) const;        //��ȡ������֮��ľ���
    float getLength(float a,float b) const;		//��ȡ������֮��ľ���
	float getLengthSqure(const _XVector2& P0) const;
	float getLengthSqure(float a,float b) const;
	float getLengthSqure() const;	//��õ�ԭ��ľ����ƽ��
	float getLength() const;		//��õ�ԭ��ľ���
    float getAngle(const _XVector2& P0) const;            //��ȡ�ӵ�ǰ��ָ��Ŀ���ĽǶ�
    float getAngle(float a,float b) const;            //��ȡ�ӵ�ǰ��ָ��Ŀ���ĽǶ�
	float getAngle() const;
    
    _XVector2(float a,float b);
    _XVector2();
};

//�߶ε���
class _XLine
{
private:
	_XVector2 m_startPosition;	//�߶ε���ʼ��
	_XVector2 m_endPosition;	//�߶ε��յ�
	char m_isEnable;			//�Ƿ���Ч
public:
	double A,B,C;	//Ax + By + C = 0;
	double m_data0;	//A^2 + B^2
	double m_data1;	//sqrt(A^2 + B^2)
	double m_angle;
public:
	void set(const _XVector2 &startPosition,const _XVector2 &endPosition);
	void set(float startX,float startY,float endX,float endY);
	float getLineLength() const;	//����߶εĳ���
	float getLineAngle() const;	//����߶εĽǶ�
	_XVector2 getStartPosition() const;	//����߶ε���ʼλ��
	_XVector2 getEndPosition() const;		//����߶ε��յ�λ��
	void setStartPosition(const _XVector2& startPosition);
	void setStartPosition(float x,float y);
	void setEndPosition(const _XVector2& endPosition);
	void setEndPosition(float x,float y);
	int getIsEnable() const
	{
		return m_isEnable;
	}
	void setEnable(int temp)
	{
		if(temp == 0) m_isEnable = 0;
		else m_isEnable = 1;
	}
	_XLine()
	:m_isEnable(0)
	,A(0.0)
	,B(0.0)
	,C(0.0)
	,m_data0(0.0)
	,m_data1(0.0)
	,m_angle(0.0)
	,m_startPosition(0.0f,0.0f)
	,m_endPosition(1.0f,1.0f)
	{
	}
};

//+++++++����ֱ���˶��µ�һ�麯��++++++++
//����ֵ -2�������� -1���ᷢ����ײ ret > 0
//����ֱ���˶�������ײʱ��ļ���
extern double getTwoLineTime(const _XVector2& L1S,const _XVector2& L1E,const _XVector2& L2S,const _XVector2& L2E,float distance,float timer);	//��ȡ�����߶�ĳ���������С��
//����ֱ���˶�����̶������ײʱ�����
extern double getLineToPointTime(const _XVector2& L1S,const _XVector2& L1E,const _XVector2& point,float distance,float timer);
//����ֱ���˶�����̶��߶ε���ײʱ�����
extern double getObjectToLineTime(const _XVector2& L1S,const _XVector2& L1E,const _XVector2& L2S,const _XVector2& L2E,float distance,float timer);	//��ȡ�����߶�ĳ���������С��
//�˶��������˶��߶ε���ײʱ�����
//�˶��߶����˶��߶ε���ײʱ�����
//+++++++�ȼ���ֱ���˶���һ�麯��++++++++
//�ȼ���ֱ���˶���������ײʱ��ļ���(ʱ�䵥λͳһΪms)
extern double getCrashTimerB2B(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,
							   const _XVector2& position2,const _XVector2& speed2,const _XVector2& a2,float distance,float time);
//�ȼ���ֱ���˶�������̶�����ײ��ʱ�����
extern double getCrashTimerB2P(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,
							   const _XVector2& position2,float distance,float time);
//�ȼ���ֱ���˶�������̶�ֱ����ײ��ʱ�����
extern double getCrashTimerB2L(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,
							   const _XVector2& positionS,const _XVector2& positionE,float distance,float time);
//������������ж�_XLineʹ��const˵����Ȼ�������֪��Ϊʲô��
extern double getCrashTimerB2L(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,const _XLine& pL,float distance,float time);
//extern double getCrashTimerB2L(const _XVector2& position1,const _XVector2& speed1,const _XVector2& a1,_XLine& pL,float distance,float time);
extern void printfR();
//������дһЩ�����Ĺ�ʽ
//�㵽ֱ�߾���Ĺ�ʽ
extern double distancePointToLine(const _XVector2& point,const _XVector2& lineS,const _XVector2& lineE);//ax+by+c=0 x0,y0 |ax0+by0+c|/��(a^2+b^2)
extern double distancePointToLine(const _XVector2& point,double a,double b,double c);
//�㵽ֱ�ߵĴ��㹫ʽ
extern _XVector2 curtatePointToLine(const _XVector2& point,const _XVector2& lineS,const _XVector2& lineE);	//��ֱ�߷���Ϊax+by+c=0,������Ϊ(m,n)����Ϊ((b*b*m-a*b*n-a*c)/(a*a+b*b),(a*a*n-a*b*m-b*c)/(a*a+b*b))
extern _XVector2 curtatePointToLine(const _XVector2& point,double a,double b,double c);
//��ֱ�߽���Ĺ�ʽ
extern _XVector2 crossPointTineToLine(const _XVector2& line1S,const _XVector2& line1E,const _XVector2& line2S,const _XVector2& line2E);
extern _XVector2 crossPointTineToLine(double a0,double b0,double c0,double a1,double b1,double c1);
//����ֱ�߹�ʽ
extern int twoPointLine(const _XVector2& lineS,const _XVector2& lineE,double *a,double *b,double *c);//(x-x1)/(x2-x1)=(y-y1)/(y2-y1)
//�㵽�߶ε���̾���
extern double minDistancePointToLine(const _XVector2& point,const _XVector2& lineS,const _XVector2& lineE);
//���߶ε���̾���
extern _XVector2 minDistanceTwoLine(const _XVector2& line1S,const _XVector2& line1E,const _XVector2& line2S,const _XVector2& line2E);
//��ȡp�Ƿ���p1 p2 p3 p4Χ�ɵ��ı����ڲ�
extern int getIsInRect(const _XVector2& p,const _XVector2& p1,const _XVector2& p2,const _XVector2& p3,const _XVector2& p4);
//�жϵ�p�Ƿ���pR��Χ�ɵĶ�����ڲ�
extern int getIsInRect(const _XVector2& p,const _XVector2 *pR,int pRSum);

//������3D��
class _XVector3
{
public:
    float x;
    float y;
    float z;

public:
    void reset();//���������
    void add(float a,float b,float c);//����������ֵ
    void set(float a,float b,float c);//������ֵ����һ�����ֵ
    _XVector3(float a,float b,float c);    
    _XVector3();
};

//�����Ⱦ���
class _XRect
{
public:
    float left;
    float top;
    float right;
    float bottom;
public:
    void set(float xl =0,float yt =0,float xr =0,float yb =0);
    void set(const _XVector2& leftTop,const _XVector2& rightBottom);
    bool setLeft(float xl);
    bool setRight(float xr);
    bool setTop(float yt);
    bool setBottom(float yb);
    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;
    _XRect(float xl =0.0f,float yt =0.0f,float xr =0.0f,float yb =0.0f);
    _XRect(const _XVector2& leftTop,const _XVector2& rightBottom);
    //_XRect()
    //    :left(0.0f)
    //    ,top(0.0f)
    //    ,right(0.0f)
    //    ,bottom(0.0f)
    //{
    //}
	//ע������ĺ���ʽ����ڲ�����ת�ľ��ε���ײ�жϣ��������ת����Ҫ��һ���޸Ĵ���
    bool isCrash(const _XRect& R0) const;
    bool isCrash(const _XVector2& leftTop,const _XVector2& rightBottom) const;
    bool isCrash(float xl,float yt,float xr,float yb) const;
    bool isInRect(const _XVector2& p0) const;
    bool isInRect(float x,float y) const;
    float getArea() const;
    _XVector2 getCenter() const;
	float getXCenter() const {return (left + right) * 0.5f;}
	float getYCenter() const {return (top + bottom) * 0.5f;}
    float getWidth() const;
    float getHeight() const;
    void setCenter(float x,float y);
    void setCenter(const _XVector2& p);
};

//����2D�㣬���ǵ��������������ٶȽϿ죬���Բ��ر����
class _XVector2I
{
public:
    int x;
    int y;

public:
    void set(int a,int b);

    float getLength(const _XVector2I& P0) const;
    float getAngle(const _XVector2I& P0) const;

	_XVector2I& operator = (const _XVector2 & temp);
	_XVector2I(const _XVector2 & temp);

    _XVector2I(int a,int b);
    _XVector2I();
};

//���ξ���
class _XIRect
{
private:
public:
    int left;
    int top;
    int right;
    int bottom;
public:
    void set(int xl =0,int yt =0,int xr =0,int yb =0);

    bool isCrash(_XIRect& R0);

    _XIRect(int xl =0,int yt =0,int xr =0,int yb =0);
    _XIRect(const _XVector2I& p0,const _XVector2I& p1);
   //_XIRect()
   //     :left(0)
   //     ,top(0)
   //     ,right(0)
   //     ,bottom(0)
   // {
   // }
    void set(const _XVector2I& p0,const _XVector2I& p1);
    void set(const _XVector2& p0,const _XVector2& p1);
    bool isInRect(const _XVector2I& p0) const;
    int getArea() const;
    _XVector2 getCenter() const;
    int getWidth() const;
    int getHeight() const;
    void setCenter(int x,int y);
    void setCenter(const _XVector2I& p);
};

//���ֽڵ���ɫ 0 - 255
class _XCColor
{
public:
    unsigned char cR;    //0 - 255
    unsigned char cG;
    unsigned char cB;
    unsigned char cA;
    _XCColor()
        :cR(0)
        ,cG(0)
        ,cB(0)
        ,cA(0)
    {
    }
    void setColor(unsigned char r,unsigned char g,unsigned char b,unsigned char a);
};

//����������ɫ 0.0 - 1.0
class _XFColor
{
//private:
public:
    float fR;    //0 - 1
    float fG;
    float fB;
    float fA;
public:
    _XFColor()
        :fR(0.0f)
        ,fG(0.0f)
        ,fB(0.0f)
        ,fA(0.0f)
    {
    }
	_XFColor(float r,float g,float b,float a)
		:fR(r)
        ,fG(g)
        ,fB(b)
        ,fA(a)
	{
		if(r < 0.0f) r = 0;
		if(r > 1.0f) r = 1.0f;
		if(g < 0.0f) g = 0;
		if(g > 1.0f) g = 1.0f;
		if(b < 0.0f) b = 0;
		if(b > 1.0f) b = 1.0f;
		if(a < 0.0f) a = 0;
		if(a > 1.0f) a = 1.0f;
	}
    void setColor(float r,float g,float b,float a);
	_XFColor operator * (const _XFColor& temp) const
	{
		return _XFColor(fR * temp.fR,fG * temp.fG,fB * temp.fB,fA * temp.fA);
	}
	void setR(float r)
	{
		if(r < 0.0f) r = 0;
		if(r > 1.0f) r = 1.0f;
		fR = r;
	}
	void setG(float g)
	{
		if(g < 0.0f) g = 0;
		if(g > 1.0f) g = 1.0f;
		fG = g;
	}
	void setB(float b)
	{
		if(b < 0.0f) b = 0;
		if(b > 1.0f) b = 1.0f;
		fB = b;
	}
	void setA(float a)
	{
		if(a < 0.0f) a = 0;
		if(a > 1.0f) a = 1.0f;
		fA = a;
	}
	float getR() const {return fR;}
	float getG() const {return fG;}
	float getB() const {return fB;}
	float getA() const {return fA;}
};

#include "XOSDefine.h"

#ifdef OS_WINDOWS
#include "windows.h"

//������������̻߳���
class _XCritical 
{ 
public: 
    _XCritical(); 
    ~_XCritical();
    void Lock();

    void Unlock();

private: 
    CRITICAL_SECTION m_sec; 
};

typedef DWORD pthread_t;    
typedef _XCritical pthread_mutex_t;

extern void pthread_mutex_init(_XCritical &temp,int sum);
extern void pthread_mutex_lock(_XCritical &temp);
extern void pthread_mutex_unlock(_XCritical &temp);
extern void close(int temp);
extern int initSocket();
#endif

#include  <complex>
#include  <iostream>
#include  <math.h>
using   namespace   std;

//�����õ��Ĵη��̵ĺ�����������������Ч�ӵ��������Լ����������ֵĽ�
extern int quarticEquation(
					  double a,double b,double c,double d,double e,
					  complex<double> &x1,complex<double> &x2,
					  complex<double> &x3,complex<double> &x4
					  );
//���溯�����ٷ�װ�����ز����������ֵĽ�
extern int quarticEquation(
					  double a,double b,double c,double d,double e,
					  double *x);

#include "XBasicClass.inl"
#endif