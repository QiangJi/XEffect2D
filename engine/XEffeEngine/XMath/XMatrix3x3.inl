//inline �����Ķ���
inline void _XMatrix3x3::loadIdentity()
{
	memset(data,0,9 * sizeof(float));
	data[0] = 1.0f;
	data[4] = 1.0f;
	data[8] = 1.0f;
}
inline _XMatrix3x3 _XMatrix3x3::operator*(const _XMatrix3x3 & rhs) const
{
	return _XMatrix3x3(data[0] * rhs.data[0] + data[1] * rhs.data[3] + data[2] * rhs.data[6],
		data[0] * rhs.data[1] + data[1] * rhs.data[4] + data[2] * rhs.data[7],
		data[0] * rhs.data[2] + data[1] * rhs.data[5] + data[2] * rhs.data[8],
		
		data[3] * rhs.data[0] + data[4] * rhs.data[3] + data[5] * rhs.data[6],
		data[3] * rhs.data[1] + data[4] * rhs.data[4] + data[5] * rhs.data[7],
		data[3] * rhs.data[2] + data[4] * rhs.data[5] + data[5] * rhs.data[8],
		
		data[6] * rhs.data[0] + data[7] * rhs.data[3] + data[8] * rhs.data[6],
		data[6] * rhs.data[1] + data[7] * rhs.data[4] + data[8] * rhs.data[7],
		data[6] * rhs.data[2] + data[7] * rhs.data[5] + data[8] * rhs.data[8]);
}
inline _XMatrix3x3 _XMatrix3x3::anti()
{
	return _XMatrix3x3(data[0],data[3],data[6],
		data[1],data[4],data[7],
		data[2],data[5],data[8]);
}
inline float& _XMatrix3x3::operator () (int x,int y) 
{
	if(x < 0 || x >= 3 || y < 0 || y >= 3)
	{
		printf("matrix access out of range\n");
		exit(1);
	}
	return data[x + y * 3];
}
inline float _XMatrix3x3::operator () (int x,int y) const 
{
	if(x < 0 || x >= 3 || y < 0 || y >= 3)
	{
		printf("matrix access out of range\n");
		exit(1);
	}
	return data[x + y * 3];
}
inline float _XMatrix3x3::getValue()	//������������ʽ��ֵ
{
	return data[0] * data[4] * data[8] + data[1] * data[5] * data[6] + data[2] * data[3] * data[7]
		- data[2] * data[4] * data[6] - data[1] * data[3] * data[8] - data[0] * data[7] * data[5];
}
inline _XVector3 _XMatrix3x3::operator *(const _XVector3 &v)	//v������������������û�����壬OpenGL��Ĭ��Ϊ������
{
	return _XVector3((data[0]*v.x + data[1]*v.y + data[2]*v.z),
	                (data[3]*v.x + data[4]*v.y + data[5]*v.z),
	                (data[6]*v.x + data[7]*v.y + data[8]*v.z));
}