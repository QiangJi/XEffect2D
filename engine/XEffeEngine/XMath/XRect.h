#ifndef _JIA_XRECT_
#define _JIA_XRECT_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
//�����Ⱦ���
class _XRect
{
public:
    float left;
    float top;
    float right;
    float bottom;
public:
    void set(float xl = 0.0f,float yt = 0.0f,float xr = 0.0f,float yb = 0.0f)
	{
		//if(xl > xr) xChangeTwoSum(&xl,&xr);
		//if(yt > yb) xChangeTwoSum(&yt,&yb);
		left = xl;
		top = yt;
		right = xr;
		bottom = yb;
	}
    void set(const _XVector2& leftTop,const _XVector2& rightBottom);
    _XBool setLeft(float xl);
    _XBool setRight(float xr);
    _XBool setTop(float yt);
    _XBool setBottom(float yb);
    float getLeft() const;
    float getRight() const;
    float getTop() const;
    float getBottom() const;
	_XRect(float xl =0.0f,float yt =0.0f,float xr =0.0f,float yb =0.0f)
	:left(xl)
	,top(yt)
	,right(xr)
	,bottom(yb)
	{}
	_XRect(const _XVector2& leftTop,const _XVector2& rightBottom)
	:left(leftTop.x)
	,top(leftTop.y)
	,right(rightBottom.x)
	,bottom(rightBottom.y)
	{}
    //_XRect()
    //    :left(0.0f)
    //    ,top(0.0f)
    //    ,right(0.0f)
    //    ,bottom(0.0f)
    //{
    //}
	//ע������ĺ���ʽ����ڲ�����ת�ľ��ε���ײ�жϣ��������ת����Ҫ��һ���޸Ĵ���
    _XBool isCrash(const _XRect& R0) const;
    _XBool isCrash(const _XVector2& leftTop,const _XVector2& rightBottom) const;
    _XBool isCrash(float xl,float yt,float xr,float yb) const;
    _XBool isInRect(const _XVector2& p0) const;
    _XBool isInRect(float x,float y) const;
    float getArea() const;
    _XVector2 getCenter() const;
	float getXCenter() const {return (left + right) * 0.5f;}
	float getYCenter() const {return (top + bottom) * 0.5f;}
    float getWidth() const;
    float getHeight() const;
    void setCenter(float x,float y);
    void setCenter(const _XVector2& p);
};
#include "XRect.inl"
#endif