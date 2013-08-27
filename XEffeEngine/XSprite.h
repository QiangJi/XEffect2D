#ifndef _JIA_XSPRITE_
#define _JIA_XSPRITE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.4.9
//--------------------------------
//������Ҫ֧�����漸������
//1���ƶ�
//2������
//3���ü�
//4����ת
//5���������ŵĻ�׼��
//6��������ת�Ļ�׼��
//7���������ҷ�ת
//ע������֡��������Ҫ����2��n�η�

//��������ź�λ�ô����߼��Ĳ�������

//���飺ʵ�־���Ķ���βü�����ʵ��һЩ����ı���Ч��

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
#include "XTextureInformation.h"
#include "XObjectBasic.h"

//�¹��ܵ�ʵ�ֽ��飺ʵ�ֶ���βü�
#define MAX_SPRITE_FILENAME_LENGTH (256)
#define IS_USE_SOLIDIFY (0)

enum _XTransformMode
{
	POINT_LEFT_TOP = 0,
	POINT_CENTER = 1
};

enum _XTurnOverMode	//����ķ�תģʽ
{
	_TURN_OVER_MODE_NULL,	//û�н��з�ת
	_TURN_OVER_MODE_LEFT_TO_RIGHT,	//���ҷ�ת
	_TURN_OVER_MODE_UP_TO_DOWN,		//���·�ת
};

class _XSprite:public _XObjectBasic	//����Ľṹ��
{
private:
	char m_resoursePosition;				//��Դλ�� 0:�ⲿ 1:�ڲ�

	_XTextureData m_textureData;

	_XVector2 m_setPosition;			//���鱻���õ�λ�ã��ڲ�ʹ�õ�λ�ã������õ�λ�����������
	_XVector2 m_setTransformCenter;		//�û����õ���״�任��λ�ã�ʵ��ʹ�õı��ε�λ�������õ�λ�����������

	_XRect m_clipOutsideRect;			//�û����õ��ⲿ�ü��ߴ�
	_XRect m_clipRect;					//���յĲü��ߴ磬����ߴ����ڲ��ü��ߴ����ⲿ�ü��ߴ����
	char m_isEnableOutsideChip;			//�Ƿ��������
	char m_isVisiable;

	void updateClipAndRotatoData();		//������ͼ�ı�������²ü�����ת������
	char m_needUpdateInsideData;		//�Ƿ���Ҫ�����ڲ�����
	void updateInsideData();			//���ھ�����������Ա仯����Ҫ���¾����һЩ�ڲ�����
public:
	void disClip();						//ȡ��2D����ü���Ĭ��Ϊ���ü�
	void setClipRect(_XRect temp);		//����2D����ü���Χ
	//ע������������⣬���ͼƬС���귽���п��࣬�ü�֮��ƴ�ϣ�Ȼ����������������Ҫ��֤left��topΪ0�������������⡣
	void setClipRect(float left,float top,float right,float bottom);	//����2D����ü���Χ

	int isInRect(float x,float y);	//��x��y�Ƿ���������ϣ����x��y����Ļ�ľ�������
	_XVector2 getPosition() const
	{
		return m_setPosition;
	}
	//��ȡ�ĸ���������꣬Ŀǰ�Ȳ�������ת������
	_XVector2 getBox(int order);
private:
	// �������һЩ������������εĴ���
	_XTurnOverMode m_turnOverMode;	//�Ƿ�����������״�ı�ǩ
	float m_shapeData[16];	//�������״����������
private:
	//GLfloat x, y;			//�������ʾλ��
	_XTransformMode m_isTransformCenter;

	//ע��Ĭ�ϵ���ת�����ŵ����ĵ���ͳһ��ΪͼƬ�����ģ����ͼƬ���ڲü�����Ϊ�ü���������Ͻ�
	_XVector2 m_transformCenter;		//���λ���ת����ʵλ��
	char m_changeTransformCenterManual;	//�û��Ƿ��ֶ����ù���ת���ĵ�λ�ã�������ת�������Ͻ���ת�������û�����
public:
	void setIsTransformCenter(_XTransformMode temp);	//�������ŵĻ�׼��
	_XTransformMode getTransformMode() const
	{
		return m_isTransformCenter;
	}

	void setTransformCenter(float x,float y)	//����Ƿ񾭹�����
	{
		m_changeTransformCenterManual = 1;
		if(m_textureData.isEnableInsideClip != 0)
		{
			m_setTransformCenter.set(x,y);
			if(m_clipOutsideRect.left < m_textureData.textureMove.x) m_transformCenter.x = m_clipOutsideRect.left - m_textureData.textureMove.x + m_setTransformCenter.x;
			else m_transformCenter.x = m_setTransformCenter.x;
			if(m_clipOutsideRect.top < m_textureData.textureMove.y) m_transformCenter.y = m_clipOutsideRect.top - m_textureData.textureMove.y + m_setTransformCenter.y;
			else m_transformCenter.y = m_setTransformCenter.y; 

			//m_transformCenter.set(x - m_textureData.textureMove.x,y - m_textureData.textureMove.y);
		}else
		{
			m_transformCenter.set(x,y);
		}
	}
public:
	void setAlpha(float temp);
	float getAlpha() const; 
	float getAngle() const
	{
		return angle;
	}
	void setAngle(float temp);
	int getW() const;
	int getH() const;
	void setVisiable() {m_isVisiable = 1;}					//��������ɼ�
	void disVisiable() {m_isVisiable = 0;}						//����������ɼ�
	char getVisiable() const {return m_isVisiable;}					//��ȡ����Ƿ�ɼ���״̬ 

private:
	GLfloat angle;			//����ĽǶ�
	float sinAngle;			//�Ƕȵ�����	//�����Ϊ���Ż������ٶȶ��趨�ı���
	float cosAngle;			//�Ƕȵ�����

	GLfloat m_positionX;	//������õ�λ��
	GLfloat m_positionY;
	GLfloat xsize, ysize;	//��������ųߴ�
	GLfloat alpha;			//͸����
	GLfloat colorRed;		//��ɫ
	GLfloat colorGreen;		//��ɫ
	GLfloat colorBlue;		//��ɫ
//	char m_isOverturn;		//�Ƿ�x�������ҷ�ת
	//Ϊ�����������ٶȣ����涨��һЩ�м����
	int wr,hr,xp,yp;
//	char *m_frameName;				//����֡�ļ���
public:	
	void setPosition(const _XVector2& position);			//���þ��������
	void setPosition(float a,float b);			//���þ��������

	void setSize(const _XVector2& size);		//���þ�������ű���
	void setSize(float a,float b);	//���þ�������ű���
	_XVector2 getSize() const
	{
		return _XVector2(xsize,ysize);
	}

	_XTexture * getTexture();	//���ص�ǰ֡����ͼָ��
	_XTextureData * getTextureData();	//���ص�ǰ֡����ͼ����
#if IS_USE_SOLIDIFY
	//++++++++++++++++++++++++++++++++++++++++++++
	//����Ϊ�������ٶȶ�����ı���
	//���̻���������ʾ��Ϣ�̻�����ʾ�б��У�����ʾ�����в��ٱ仯��ʾϸ�ڣ���������̶���û��ϸ�ڱ仯��Ԫ������ʹ���������
	void setIsUseSolidify(int temp);
private:
	int m_glListOrder;		//��ʾ�б���
	int m_needSolidify;		//�Ƿ���Ҫ�̻�
	//--------------------------------------------
#endif
private:
	char m_isInited;
	char m_blendTypeScr;
	char m_blendTypeDst;
public:
	void setBlendType(int typeScr,int typeDst)
	{
		if(typeScr < 0) typeScr = 0;
		if(typeDst < 0) typeDst = 0;
		if(typeScr >= 9 ) typeScr = 8;
		if(typeDst >= 8 ) typeDst = 7;
		m_blendTypeScr = typeScr;
		m_blendTypeDst = typeDst;
	}
	void (*m_pShaderProc)(void);	//����һ��shader�ĺ���ָ��
public:
	int init(const char *filename,						//ͼƬ������
		int resoursePosition = 0,					//��Դ��λ�� 0�ⲿ 1�ڲ�
		_XTransformMode isTransformCenter = POINT_CENTER);	//�Ƿ�ʹ��������ת	

	int init(int tempW = 0,int tempH = 0,int needSizeCheck = 1,_XTransformMode isTransformCenter = POINT_CENTER);	//�Ƿ�Ϊ����֡����

	int release();									//�ͷ���Դ

	//ע�⣺
	//�ⲿ����drawϵ�к�����ʱ���ǲ��ܴ��ݲ�����

	//������Ļ�ͼ��ʽ
	void drawBasic(const GLuint *pTexnum = NULL);					//��ԭʼ�������Ļ�ͼ��ʽ�����ܽϵ�
	void drawBasic(const _XTextureData *pTexData);// = NULL);					//��ԭʼ�������Ļ�ͼ��ʽ�����ܽϵ�
	//���������������Ⱥ�黯�Ż�
	//ע��������������ⲿ����ͼƬʱһ��Ҫȷ�ϲ���ƴ���Ż��е�ͼƬ������������ʾ����
	void draw(const GLuint *pTexnum = NULL);			//���ⲿ������ͼ�����л��ƣ�����������ͼ������ԭ�е���ͼ������ͬ�ĳߴ磬������������
	void draw(const _XTextureData *pTexData);// = NULL);			//���ⲿ������ͼ�����л��ƣ�����������ͼ������ԭ�е���ͼ������ͬ�ĳߴ磬������������
	//û�вü���û�з�ת��û��͸��ͨ����ʱ���ʹ�����������������������,���ַ����ٶȽϿ죬��Դ��ʡ
	void drawEx(const GLuint *pTexnum = NULL);				//���Ч�Ļ�ͼ��ʽ������Ҫ�ü��ȹ���
	void drawEx(const _XTextureData *pTexDatam);// = NULL);				//���Ч�Ļ�ͼ��ʽ������Ҫ�ü��ȹ���
	//����Ҫ���л��
	void drawWithoutBlend(const GLuint *pTexnum = NULL);
	void drawWithoutBlend(const _XTextureData *pTexDatam);

	void setColor(float r,float g,float b,float a);		//С��0����ı���һԪ��
	void setColor(const _XFColor& color);		//С��0����ı���һԪ��
	//���ƾ���
private:
	int m_isACopy;	//�ж��Ƿ�Ϊ�����壬������û���Լ�����Դ�����ͷ���Դ��ʱ����Ҫ����
	_XSprite(const _XSprite& temp);					//�ض��帳ֵ���캯��Ϊ˽�У���ֹ������ö���ɴ���

	//�����ǹ��ڽǶȲü�������
	char m_needAngleClip;	//�Ƿ���Ҫ�ǶȲü�
	float m_clipAngle;		//�ǶȲü��ĽǶ� 0 - 360��
	_XVector2 m_pointArray[14];	//����Ҫ��ʱ��ˢ����Щ����
	int m_pointArraySize;	
#ifdef GET_ALL_PIXEL
	float m_thisDrawArea;	//����������
#endif
	char m_needUpdatePointArray;	//�Ƿ���Ҫ�����ڲ���������
	void updatePointArray();	//�����ڲ�����ˢ��
public:
	void setAngleClip(float angle)
	{
		m_needAngleClip = 1;
		m_clipAngle = angle;	//����Ƕ���Ҫ����
		m_needUpdatePointArray = 1;
	}
	void disAngleClip()
	{
		m_needAngleClip = 0;
		m_needUpdatePointArray = 1;
	}
public:
	_XSprite& operator = (const _XSprite& temp);	//��Ŀ�����Դ���й��ã�������
	void setACopy(const _XSprite& temp);			//��Ŀ�����Դ���й��ã�������

	void setOverturn(char temp);	//�������ҷ�ת(Ϊ�˱�����ɰ汾�ļ��ݱ�������ӿ�)
	void setLeft2Right();	//�������ҷ�ת
	void setUp2Down();		//�������·�ת
	void resetLeftRightUpDown();	//�ظ��������ҵ���ȷλ��
	_XTurnOverMode getTurnOverMode() const
	{
		return m_turnOverMode;
	}

	_XSprite();
	~_XSprite();
};

#include "XSprite.inl"

#endif