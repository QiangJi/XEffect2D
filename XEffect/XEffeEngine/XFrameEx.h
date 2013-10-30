#ifndef _JIA_XFRAMEEX_
#define _JIA_XFRAMEEX_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------

//�Ż�˵��
//1����������ͼ���ļ�������ÿ֡�ĳߴ綼��ͬ����һ���ĺ���������һ��ͼƬ�ϣ�����ʱ�Զ��ü�
//2����һ��С�����Զ���ͼ�������Ч�ߴ�Ĳü���Ȼ�����е�ͼƬ�����������ƴ�ϡ�
//С������ȡ���Ϊx��֡��λ�úͳߴ磬�Լ�ƫ��λ�ã�������ָ�����ļ���

#include "XSprite.h"

#define MAX_FRAME_FILENAME_LENGTH 256	//�ļ����ĳ���
//#define MAX_FRAME_SUM 256		//����֡���ֻ��256֡

class _XFrameEx:public _XObjectBasic	
{
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//����������д������֡����Ķ���
private:
	_XSprite *m_pSprite;
	//------------------------------------------------------------------
	_XSCounter *m_cp;		//���ü�����
	int m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
	float angle;			//����ĽǶ�
	char m_isVisiable;
public:
	void setAngle(float temp);
	float getAngle() const;
	void setVisiable() {m_isVisiable = 1;}					//��������ɼ�
	void disVisiable() {m_isVisiable = 0;}						//����������ɼ�
	char getVisiable() const {return m_isVisiable;}					//��ȡ����Ƿ�ɼ���״̬ 
private:
	float x, y;			//�����λ��
	float xsize, ysize;	//��������ųߴ�
	float alpha;			//͸����
	float colorRed;		//��ɫ
	float colorGreen;		//��ɫ
	float colorBlue;		//��ɫ
	char m_isOverturn;		//�Ƿ�x�������ҷ�ת

	int m_allFramesSum;							//����֡��������֡��
	int m_allKeyFramesSum;						//����֡�����йؼ�֡������
	float m_nowFramesNumble;					//��ǰ���ŵ��ڼ�֡
	float m_actionSpeed;						//����֡���ŵ��ٶ�
	char m_isLoop;								//����֡�Ƿ����ѭ��
	int m_startFrame;							//����һ֡��ʼ
	char m_isEnd;								//����֡�Ƿ񲥷����
	char m_isEndImmediately;					//�����Ƿ�Ϊ���̽�������
	char m_isSetEnd;							//is end by user			//�Ƿ���������֡����
	char m_isDisappearAtEnd;					//����֡�Ƿ��ڲ������֮����ʧ

	char *m_frameName;				//����֡�ļ���
	int *m_keyFrameArray;			//����֡�Ĺؼ�֡�������
public:
	void setAttribute(const _XVector2& position,			//����֡���ŵ�λ��
		int loop,				//����֡�Ƿ�ѭ��
		int endImmediately,		//����֡�Ƿ���������
		int startFrame,			//����֡����ʼ֡
		float actionSpeed,		//����֡�Ĳ����ٶ�(�����ֵ������0.05����)
		int disappearAtEnd,		//����֡�����������֮���Ƿ���ʧ
		int isOverturn = 0);	//�Ƿ�ת

	void setPosition(const _XVector2& position);			//���þ��������
	void setPosition(float a,float b);			//���þ��������
	_XVector2 getPosition() const;

	void setSize(const _XVector2& size);		//���þ�������ű���
	void setSize(float x,float y);	//���þ�������ű���
	_XVector2 getSize() const;

	int getIsEnd() const;	//�������֡�Ƿ񲥷����
	void setEnd();	//�����������������
	void reset();	//�����Ѿ�������ɵ�����֡�������²���
	void move(int timeDelay);	//�ƽ�����֡����
	int getAllFramesSum() const
	{
		return m_allFramesSum;
	}
	float getNowFramesNumber() const
	{
		return m_nowFramesNumble;
	}

	_XSprite * getNowSprite(char frameFlag = 0);	//���ص�ǰ֡�ľ���ָ�룬��� frameFlag = 1�򷵻ص��������������е�ָ��
private:
	char m_isInited;		//�Ƿ��ʼ��
	char m_blendTypeScr;	//blendģʽ���ο�OpenGL��Blend�Ĳ���
	char m_blendTypeDst;
public:
	void setBlendType(int typeScr,int typeDst)	//���û��ģʽ
	{
		if(typeScr < 0) typeScr = 0;
		if(typeDst < 0) typeDst = 0;
		if(typeScr > 8 ) typeScr = 8;
		if(typeDst > 7 ) typeDst = 7;
		m_blendTypeScr = typeScr;
		m_blendTypeDst = typeDst;
		for(int i = 0;i < m_allKeyFramesSum;++ i)
		{
			m_pSprite[i].setBlendType(m_blendTypeScr,m_blendTypeDst);
		}
	}
	void (*m_pShaderProc)(void);	//����һ��shader�ĺ���ָ��

	int init(const char *filename,			//ͼƬ������
		int resoursePosition = 0);		//��Դ��λ��				
private:
	int releaseMem();			//�ͷ��Լ�����Դ�����ڱ�������õ���Դ��Ҫ�������ü�����������
public:
	void release(){}			//����ԭ�еĽӿ����¼��ݣ���ʵ����ɶҲ����
	void draw();			//��ʾ����֡
	void setColor(float r,float g,float b,float a);		//С��0����ı���һԪ��
	void setAlpha(float a);
	float getAlpha() const;

	void setActionSpeed(float actionSpeed);	//��������֡���ŵ��ٶ�
	//������Ϊ���������Ľӿ�
	int isInRect(float x,float y);	//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getBox(int order);	//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
private:
	int m_isACopy;	//�ж��Ƿ�Ϊ�����壬������û���Լ�����Դ�����ͷ���Դ��ʱ����Ҫ����
	_XFrameEx(const _XFrameEx &temp);	//���ظ�ֵ���캯��Ϊ˽�У���ֹ������ö���ɴ���
public:
	_XFrameEx& operator = (const _XFrameEx& temp);	//����һ��ͬ���壬����ָ����ڲ���Դ��ʹ��Ŀ�����Դ��(������Դ�����ǻ���ɣ�û���Լ��������Դ���������)
	void setACopy(const _XFrameEx& temp);		//����һ��ͬ���壬����ָ����ڲ���Դ��ʹ��Ŀ�����Դ��(������Դ�����ǻ���ɣ�û���Լ��������Դ���������)

	void setRestart();			//�������»ص���ʼ֡��ʼ����

	_XFrameEx();
	~_XFrameEx();
};

#include "XFrameEx.inl"

#endif