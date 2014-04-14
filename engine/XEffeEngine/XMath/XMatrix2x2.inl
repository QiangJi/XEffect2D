inline void _XMatrix2x2::loadIdentity()
{
	data[0] = 1;
	data[1] = 0;
	data[2] = 0;
	data[3] = 1;
}
inline _XMatrix2x2 _XMatrix2x2::anti()		//���þ���
{
	return _XMatrix2x2(data[0],data[3],data[2],data[1]);
}
//inline void _XMatrix2x2::anti()
//{
//	float temp = data[3];
//	data[3] = data[1];
//	data[1] = temp;
//}
inline float _XMatrix2x2::operator () (int x,int y) const
{
	if(x < 0 || x >= 2 || y < 0 || y >= 2)
	{
		printf("matrix access out of range\n");
		exit(1);
	}
	return data[x + y * 2];
}
inline float& _XMatrix2x2::operator () (int x,int y) 
{
	if(x < 0 || x >= 2 || y < 0 || y >= 2)
	{
		printf("matrix access out of range\n");
		exit(1);
	}
	return data[x + y * 2];
}
inline _XVector2 _XMatrix2x2::operator *(const _XVector2 &v)	//v������������������û�����壬OpenGL��Ĭ��Ϊ������
{//Ӧ��������ϵ�仯�е����ź���ת
	return _XVector2(v.x * data[0] + v.y * data[1],v.x * data[2] + v.y * data[3]);
}
inline _XMatrix2x2 _XMatrix2x2::operator*(const _XMatrix2x2 & rhs) const
{
	return _XMatrix2x2(data[0] * rhs.data[0] + data[1] * rhs.data[2],
		data[0] * rhs.data[1] + data[1] * rhs.data[3],
			
		data[2] * rhs.data[0] + data[3] * rhs.data[2],
		data[2] * rhs.data[1] + data[3] * rhs.data[3]);
}
inline float _XMatrix2x2::getValue() 
{
	return data[0] * data[3] - data[1] * data[2];
}