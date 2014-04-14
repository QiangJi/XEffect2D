#ifndef _JIA_XVECTOR4_
#define _JIA_XVECTOR4_
//++++++++++++++++++++++++++++++++
//Author:    ��ʤ��(JiaShengHua)
//Version:    1.0.0
//Date:       2014.1.1
//--------------------------------
#include "XVector3.h"

class _XVector4
{
public:
	float x;
	float y;
	float z;
	float w;
	void reset();//���������
    void add(float a,float b,float c,float d);//����������ֵ
    void set(float a,float b,float c,float d);//������ֵ����һ�����ֵ
	_XVector4 operator - (const _XVector4& temp) const;
	_XVector4 operator + (const _XVector4& temp) const;
	_XVector4 operator * (const _XVector4& v) const;	//CROSS product
	_XVector4 operator + (const float& temp) const;
	_XVector4 operator - (const float& temp) const;
	_XVector4 operator * (const float& temp) const;
	_XVector4 operator / (const float& temp) const;
	void operator += (const float& temp);
	void operator -= (const float& temp);
	void operator *= (const float& temp);
	void operator /= (const float& temp);
	_XVector4 conjugate() const;	//��Ԫ�صĹ�����Ԫ��
	_XVector4 inverse() const;		//��Ԫ�ص���
    _XVector4()
		:x(0.0f),y(0.0f),z(0.0f),w(0.0f)
	{}
	_XVector4(float a,float b,float c,float d)
		:x(a),y(b),z(c),w(d)
	{}
	_XVector4& operator = (const float* temp); //�����߼�������
	_XVector4(float *temp)	//�����߼�������
	{
		if(temp == NULL)
		{
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
			w = 0.0f;
		}else
		{
			x = temp[0];
			y = temp[1];
			z = temp[2];
			w = temp[3];
		}
	}
	float getLength() const;
	float getLengthSqure() const;
	float dot(const _XVector4 &v) const {return x*v.x + y*v.y + z*v.z + w * v.w;}	//���
	void getRotate(float angle,const _XVector3 &v);	//��vΪ��ת��angle�Ƕȵ���Ԫ��
	void getRotateX(float ax);
	void getRotateY(float ay);
	void getRotateZ(float az);
	void getFromPlane(const _XVector3 &normal,const _XVector3 &point);	//����ƽ��ķ����Լ�ƽ���ϵĵ㣬�����и�ƽ�����Ԫ��
	void getFromPlane(const _XVector3 &p0,const _XVector3 &p1,const _XVector3 &p2);	//����ƽ�������������и�ƽ�����Ԫ��
	operator float* () const {return (float*) this;}
	operator const float* () const {return (const float*) this;}
};
typedef _XVector4 _XQuaternions;
#include "XVector4.inl"
//ͨ�����������һ��ƽ��
inline _XVector4 getPlane(const _XVector3 p0,const _XVector3 p1,const _XVector3 p2)
{
	float d = 1.0f / (-p0.x*p1.y*p2.z+p0.x*p2.y*p1.z+p1.x*p0.y*p2.z-p2.x*p0.y*p1.z-p1.x*p2.y*p0.z+p2.x*p1.y*p0.z);
	//float d = 1.0f / (p0.x*p1.y*p2.z-p0.x*p2.y*p1.z-p1.x*p0.y*p2.z+p2.x*p0.y*p1.z+p1.x*p2.y*p0.z-p2.x*p1.y*p0.z);
	return _XVector4((p0.y*p1.z-p0.y*p2.z-p1.y*p0.z+p1.y*p2.z+p2.y*p0.z-p2.y*p1.z) * d,
		(-p0.x*p1.z+p0.x*p2.z+p1.x*p0.z-p1.x*p2.z-p2.x*p0.z+p2.x*p1.z) * d,
		(p0.x*p1.y-p0.x*p2.y-p1.x*p0.y+p1.x*p2.y+p2.x*p0.y-p2.x*p1.y) * d,
		1.0f);
}
inline _XVector4 normalize(const _XVector4& v) 
{
	float d = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	if(d == 0.0f) d = 0.0000001f;
	return _XVector4(v.x/d,v.y/d,v.z/d,v.w/d);
}
//��������ƽ��Ľ���
extern _XVector3 getPoint(const _XVector4& v0,const _XVector4& v1,const _XVector4& v2);
//�����v0ת��v1��Ҫ����ת��Ԫ��
inline _XVector4 getSub(const _XVector4& v0,const _XVector4& v1)
{
	return v0.inverse() * v1;
}
//��Ԫ�صĲ�ֵ���㣬��v0��v1֮���ֵ��ϵ��Ϊr��r��ȡֵ��Χһ��Ϊ[0.0 - 1.0]
extern _XVector4 slerp(const _XVector4& v0,const _XVector4& v1,float r);
//aquad()������ʽ��δʵ��
inline _XVector4 toVector4(const _XVector3& e)	//��ŷ���ǵ���Ԫ��֮���ת��
{
	float xc = cos(e.x * 0.5f);
	float xs = sin(e.x * 0.5f);
	float yc = cos(e.y * 0.5f);
	float ys = sin(e.y * 0.5f);
	float zc = cos(e.z * 0.5f);
	float zs = sin(e.z * 0.5f);

	return _XVector4(-xs * yc * zc - xc * ys * zs,
		xs * yc * zs - xc * ys * zc,
		xs * ys * zc - xc * yc * zs,
		xc * yc * zc + xs * ys * zs);
}
inline _XVector3 toVector3(const _XVector4& e)	//����Ԫ����ŷ����֮���ת��
{
	//���� - ����
	_XVector3 ret;
	float sp = -2.f * (e.y * e.z + e.w * e.x);
	if(fabs(sp) > 0.9999f)
	{
		ret.z = 0.0f;
		ret.x = PI_HALF * sp;
		ret.y = atan2(-e.x * e.z - e.w * e.y,0.5f - e.y * e.y - e.z * e.z);
	}else
	{
		ret.x = asin(sp);
		ret.y = atan2(e.x * e.z - e.w * e.y,0.5f - e.x * e.x - e.y * e.y);
		ret.z = atan2(e.x * e.y - e.w * e.z,0.5f - e.x * e.x - e.z * e.z);
	}
	//���� - ����
	//float sp = -2.f * (e.y * e.z - e.w * e.x);
	//if(fabs(sp) > 0.9999f)
	//{
	//	ret.x = 0.0f;
	//	ret.y = PI_HALF * sp;
	//	ret.z = atan2(-e.x * e.z + e.w * e.y,0.5f - e.y * e.y - e.z * e.z);
	//}else
	//{
	//	ret.x = atan2(-e.x * e.z + e.w * e.y,0.5f - e.x * e.x - e.z * e.z);
	//	ret.y = asin(sp);
	//	ret.z = atan2(-e.x * e.z + e.w * e.y,0.5f - e.x * e.x - e.y * e.y);
	//}
	return ret;
}
#endif