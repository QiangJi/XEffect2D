#ifndef _JIA_XOBJECTBASIC_
#define _JIA_XOBJECTBASIC_

#include "XBasicClass.h"

//������ܱ�������������������Ļ��࣬�ܱ�����������������������������
class _XObjectBasic
{
//private:
protected:
public:
	virtual int isInRect(float x,float y) = 0;		//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	virtual _XVector2 getBox(int order) = 0;			//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������

	virtual void setPosition(float x,float y) = 0;			//���������λ��
	virtual void setPosition(const _XVector2& pos) = 0;		//���������λ��
	virtual _XVector2 getPosition() const  = 0;				//��ȡ����ĳߴ�

	virtual _XVector2 getSize() const  = 0;					//��ȡ����ĳߴ�
	virtual void setSize(float x,float y) = 0;				//��������ĳߴ�
	virtual void setSize(const _XVector2& size) = 0;		//��������ĳߴ�

	virtual float getAngle() const = 0;						//��ȡ����ĽǶ�
	virtual void setAngle(float angle) = 0;					//��������ĽǶ�
	//virtual ~_XObjectBasic();
	virtual void setVisiable() = 0;					//��������ɼ�
	virtual void disVisiable() = 0;					//����������ɼ�
	virtual char getVisiable() const = 0;					//��ȡ����Ƿ�ɼ���״̬ 
};
#endif
