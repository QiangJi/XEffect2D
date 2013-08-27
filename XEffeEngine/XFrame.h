#ifndef _JIA_XFRAME_
#define _JIA_XFRAME_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------

//�Ż�˵��
//1����������ͼ���ļ�������ÿ֡�ĳߴ綼��ͬ����һ���ĺ���������һ��ͼƬ�ϣ�����ʱ�Զ��ü�
//2����һ��С�����Զ���ͼ�������Ч�ߴ�Ĳü���Ȼ�����е�ͼƬ�����������ƴ�ϡ�
//С������ȡ���Ϊx��֡��λ�úͳߴ磬�Լ�ƫ��λ�ã�������ָ�����ļ���

#include "XOSDefine.h"
#ifdef OS_WINDOWS
#include "windows.h"
#endif
#include "SDL.h"
#include "SDL_image.h"
#include "gl.h"
#include "XBasicClass.h"

#include "math.h"
#include "XBasicFun.h"
#include "XBasicOpenGL.h"

#define MAX_FRAME_FILENAME_LENGTH 256	//�ļ����ĳ���
#define MAX_FRAME_SUM 65		//����֡���ֻ��256֡

class _XFrame
{
private:
	_XSCounter *m_cp;		//���ü�����

	int m_resoursePosition;	//��Դλ�� 0:�ⲿ 1:�ڲ�
	char m_isInited;		//�Ƿ��Ѿ������˳�ʼ��

	float angle;			//����ĽǶ�
	float angleRadian;		//���ȱ�־�ĽǶ�
	float sinAngle;
	float cosAngle;
public:
	void setAngle(float temp);
	float getAngleDegree();	//��ýǶ�
	float getAngleRadian();	//��û���
	char m_isFlash;	//�Ƿ�����
	void (*m_pShaderProc)(void);	//����һ��shader�ĺ���ָ��
private:
	float x, y;			//�����λ��
	float xsize, ysize;	//��������ųߴ�
	float alpha;			//͸����
	float colorRed;		//��ɫ
	float colorGreen;		//��ɫ
	float colorBlue;		//��ɫ
	char m_isOverturn;		//�Ƿ�x�������ҷ�ת
	char m_isVisiable;

	_XTexture *m_texnum;						//����֡ͼƬ��ָ��

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
	//GLint *m_width;
	//GLint *m_height;							//����ĳߴ�
	int *wre;
	int *hre;
	int *xpa;
	int *ypa;

	char *m_frameName;				//����֡�ļ���
	int *m_keyFrameArray;			//����֡�Ĺؼ�֡�������

	//++++++++++++++++++++++++++++++++++++
	//Ϊ����ƫ�ƶ�����ı���������ͼ�����ת�������Ƕ���ͼƬ���Ľ��еģ����Ե�����֡��������ƫ��ʱ��
	//������Ҫ��������ƫ�ƴ�������ת���������⣨������ʱû�п��ǣ���
	float m_centerX;			//�ܵ�����֡��������
	float m_centerY;
	float *m_centerDW;							//ÿ֡������ƫ��
	float *m_centerDH;			
	_XVector2 *m_keyFramePosition;	//�ؼ�֡ƫ������
	//------------------------------------
public:
	void setVisiable() {m_isVisiable = 1;}					//��������ɼ�
	void disVisiable() {m_isVisiable = 0;}						//����������ɼ�
	char getVisiable() const {return m_isVisiable;}					//��ȡ����Ƿ�ɼ���״̬ 

	void setAttribute(const _XVector2& position,			//����֡���ŵ�λ��
		int loop,				//����֡�Ƿ�ѭ��
		int endImmediately,		//����֡�Ƿ���������
		int startFrame,			//����֡����ʼ֡
		float actionSpeed,		//����֡�Ĳ����ٶ�
		int disappearAtEnd,		//����֡�����������֮���Ƿ���ʧ
		int isOverturn = 0);	//�Ƿ�ת

	void setPosition(const _XVector2& position);			//���þ��������
	void setPosition(float a,float b);			//���þ��������
	_XVector2 getPosition() const;

	int getNowX() const;	//��þ���ͼƬ��ǰ��λ��X(����������ƫ��֮�������Ҫ���¿���)
	int getNowY() const;	//��þ���ͼƬ��ǰ��λ��Y(����������ƫ��֮�������Ҫ���¿���)

	int getNowWidth() const;	//��õ�ǰ֡�Ŀ��
	int getNowHeight() const;	//��õ�ǰ֡�ĸ߶�

	int getInitHalfWidth() const;		//��ȡ��һ֡�İ���,Ҳ��������֡���Ͻǵ�����֡�е�Ŀ��
	int getInitHalfHeight() const;	//��ȡ��һ֡�İ�߶�,Ҳ��������֡���Ͻǵ�����֡�е�ĸ߶�

	void setSize(const _XVector2& size);		//���þ�������ű���
	void setSize(float x,float y);	//���þ�������ű���
	_XVector2 getSize();

	int getIsEnd() const;	//�������֡�Ƿ񲥷����
	void setEnd();	//�����������������
	void reset();	//�����Ѿ�������ɵ�����֡�������²���
	void move(int timeDelay);	//�ƽ�����֡����

	_XTexture * getTexture(int frameFlag = 0);	//���ص�ǰ֡����ͼָ�� frameFlag = 1ʱ���ص�������֡��ͼƬ���������ָ��

	int init(const char *filename,			//ͼƬ������
		int resoursePosition = 0);		//��Դ��λ��				
private:
	int releaseMem();									//�ͷ���Դ
public:
	void release(){}		//����ԭ�еĽӿ����¼��ݣ���ʵ����ɶҲ����
	void draw(const GLuint *pTexnum = NULL) const;			//��ͼ��������֡������pTexnumΪ��ͼ��ָ�� ����֡ʱpTexnumΪ����֡��ͼ���ָ��
	void setColor(float r,float g,float b,float a);		//С��0����ı���һԪ��
	void setAlpha(float alpha);	//����͸����
	float getAlpha() const;

	void setActionSpeed(float actionSpeed);	//��������֡���ŵ��ٶ�
private:
	//char m_haveSelfMemery;			//�Ƿ��ڹ��캯���з����ڴ�ռ�
	//��󷵻ص���һ��Ӧ�ã����Բ���Ҫ���忽�����캯��
	int m_isACopy;	//�ж��Ƿ�Ϊ�����壬������û���Լ�����Դ�����ͷ���Դ��ʱ����Ҫ����
	_XFrame(const _XFrame& temp);	//���ظ�ֵ���캯��Ϊ˽�г�Ա����ֹ����Ҫ���������
public:
	_XFrame& operator = (const _XFrame& temp);
	void setACopy(const _XFrame& temp);		//����һ��ͬ���壬��������з����ڴ������︴����ͬ����Դ�����û�з����ڴ�ռ䣬�����ָ��Ŀ����ڴ�ռ�

	void setRestart();			//�������»ص���ʼ֡��ʼ����

	_XFrame();
	//_XFrame(char withNoMemry);	//�������ڴ�ռ䣬����������֡ʹ��ͬһ��ͼƬ��Դʱ����ʹ���������
	~_XFrame();
};

#include "XFrame.inl"

#endif