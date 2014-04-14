#ifndef _JIA_MOVIECORE_
#define _JIA_MOVIECORE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2014.3.20
//--------------------------------
#include "XEffeEngine.h"
//����ӰƬ������Ļ��࣬���е�ӰƬ�����඼�Ǽ̳����������
class _XMovieCore
{
protected:
public:
	virtual ~_XMovieCore() {;}
	virtual _XBool load(const char *filename) = 0;	//����ָ��ӰƬ
	virtual _XBool upDateFrame() = 0;			//������Ƶ�����Ƿ����
	virtual void draw() = 0;					//���
	virtual void updateTex(_XTexture &tex) = 0;
	virtual unsigned int * getTexture() = 0;
	virtual void setPauseOrPlay(void) = 0;
	virtual void closeClip(void) = 0;
	virtual void setLoop(_XBool isLoop) = 0;
	virtual double getCurrentPlayingTime() const = 0;
};
template<typename T>
class _XMovie
{
private:
	_XMovieCore *m_pObj;
public:
	_XMovie()
	{
		m_pObj = createMem<T>();
	}
	~_XMovie()
	{
		XDELETE(m_pObj);
	}
	_XBool load(const char *filename) {return m_pObj->load(filename);}
	_XBool upDateFrame() {return m_pObj->upDateFrame();}
	void draw() {m_pObj->draw();}
	void updateTex(_XTexture &tex) {m_pObj->updateTex(tex);}
	unsigned int * getTexture() {return m_pObj->getTexture();}
	void setPauseOrPlay(void) {m_pObj->setPauseOrPlay();}
	void closeClip(void) {m_pObj->closeClip();}
	void setLoop(_XBool isLoop) {m_pObj->setLoop(isLoop);}
	double getCurrentPlayingTime() const {return m_pObj->getCurrentPlayingTime();};
};
#endif