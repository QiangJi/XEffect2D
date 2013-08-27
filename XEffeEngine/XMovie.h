#ifndef _XMOVIE_
#define _XMOVIE_
//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		2011.11.20
//--------------------------------
//������Ƶ������ת�������⣬�����Ƶʹ�õ���Ƶ�����ʱ���Ϊ44.1k���򽫻������Ƶ������һ�¡�
//��Ƶ�е���Ƶ��Ҫ�������Ҫ��2������16bit��λ��44.1�Ĳ�����
#include "glew.h"
#include "SDL.h"
#include "SDL_thread.h"

#include <time.h>
#include "XBasicFun.h"
#include "XOSDefine.h"

#ifdef OS_WINDOWS
#include <Windows.h>
#include "avformat.h"
#include "avcodec.h"
#endif
#ifdef OS_LINUX
#include <pthread.h>
#include "avformat.h"
#include "avcodec.h"
#endif

//1�����������Ŀ���
//2������ͼװ����OpenGL

#ifdef CONFIG_WIN32
#undef main /* We don't want SDL to override our main() */
#endif

#define SDL_AUDIO_BUFFER_SIZE 1024
#define VIDEO_FILE_NAME_LENGTH 256

#pragma comment(lib, "SDL.lib")

typedef struct VideoPict
{
	AVPicture *pict;
	//SDL_Overlay *bmp;
	unsigned char *data;
	struct VideoPict *next;
}VideoPict;

typedef struct VideoPictList
{
	VideoPict *first_pict;
	VideoPict *last_pict;
	int nb_pict;
	SDL_mutex *mutex;
}VideoPictList;

typedef struct PacketQueue
{
	AVPacketList *first_pkt , *last_pkt;
	int nb_packets;
	int size;
	SDL_mutex *mutex;
	SDL_cond *cond;
}PacketQueue;

#define MAX_BUFF_DEEP 128

class _XMovie
{
private:
	char m_isLoaded;	//�Ƿ��������
public:
	_XMovie();
	~_XMovie();

	void init(SDL_Surface *temp);		//�����ϴ���
	//��ӰƬ��ffmpeg���
	int load(const char *filename);		//����ӰƬ�ļ�������صĳ�ʼ��
	void waitingForBuffOver(void);		//�ȴ�ӰƬ������

	//unsigned char* GetData(void);
	int getClipWide(void);				//��ȡӰƬ��ʵ�ʿ��
	int getClipHigh(void);				//��ȡӰƬ��ʵ�ʸ߶�

	//int GetClipChannel(void);
	void setLoop(bool Replay);			//����ӰƬ�Ƿ�ѭ������ , TRUE��ѭ�� �� FALSE���
	double getCurrentPlayingTime();		//��ȡӰƬ��ǰ����ʱ��(��λ������)

	//void SetCurrentFrame(double Frame);	//���õ�ǰ����֡��
	
	int getAllTime(void);			//��ȡӰƬ��ʱ��(��λ������)
	float getVideoFrameRate();		//���ӰƬ��֡��
	bool getUpdateFlag(void);		//��ȡ��Ƶ�������Ƿ�Ϊ��

	void setPauseOrPlay(void);		//��ͣ���������ӰƬ
	void closeClip(void);			//�ر���Ƶ���ţ����ͷ���ص���Դ
	void gotoFrame(float temp);		//������ǰ��Ƶ��ĳһ֡
	unsigned char *m_nowFrame;		//ӰƬ��ǰ���ŵ�frame
	SDL_mutex* m_mutex;
	VideoPict *m_bmp[MAX_BUFF_DEEP];
	unsigned char *m_frameDataBuff[MAX_BUFF_DEEP];
	AVPicture m_framePic[MAX_BUFF_DEEP];
	int m_nowBuffPoint;
	int m_needVoice;				//ӰƬ�Ƿ���Ҫ����
	GLuint m_texture;
	GLuint * getTexture();

private:
	VideoPictList videopictlist;	//��Ƶ֡����
	static PacketQueue audioq;		//��Ƶ֡����
	AVFormatContext *pFormatCtx;	//��Ƶ��ʽ��������
	AVCodecContext *pCodeCtx;		//��Ƶ������������
	AVCodecContext *aCodeCtx;		//��Ƶ������������
	AVFrame *pFrame;				//֡����
	AVPacket packet;
	int videoStream;			//��Ƶ��������
	int audioStream;			//��Ƶ��������
	SDL_AudioSpec wanted_spec;	//�����豸������
	SDL_Surface *m_screen;		//�ӿھ��
//	BOOL bdecdraw;				//�Ƿ��ں�̨��ͼ�׶�??
	bool m_isBuffing;			//��ǰ�Ƿ���ڻ���״̬

	static bool m_isStop;		//�Ƿ���ͣ����
	bool m_isEnd;				//�Ƿ񲥷����
	static bool m_isQuit;		//�Ƿ��˳�����
	int m_nowPlayedTime;		//��ǰ���ŵ�ʱ��

	int m_allFrameSum;			//ӰƬ����֡��
	int m_nowFrameNumber;		//��ǰ���ŵ�֡��
	float m_nowFrameRate;		//��ǰ��Ƶ��֡��
	int m_videoWidth;			//��Ƶ�Ŀ��
	int m_videoHeight;			//��Ƶ�ĸ߶�
	bool m_isLoop;				//��Ƶ�Ƿ�ѭ������
	char videofilename[VIDEO_FILE_NAME_LENGTH];		//��Ƶ���ļ���

	void draw(void);								//��ͼ����������Ƶ�����е�ͼƬ������Ļ��
#ifdef OS_LINUX
	friend void *decoderThread(void *arg);			//��Ƶ�����߳�
	friend void *drawThread(void *arg);				//��ͼ����
	friend void audioCallBack(void *userdata , Uint8 *stream , int len);	//�����ص�����
#endif
#ifdef OS_WINDOWS
	friend DWORD WINAPI decoderThread(void *arg);	//��Ƶ�����߳�
	friend DWORD WINAPI drawThread(void *arg);		//��ͼ����
	friend void audioCallBack(void *userdata , Uint8 *stream , int len);	//�����ص�����
#endif
	void initPacketQueue(PacketQueue *q);			//��ʼ����Ƶ����
	void initVideoPictList(VideoPictList *vpl);		//��ʼ����Ƶ����
	void releaseVideoPictList();
	void releasePacketQueue();
	int putIntoPacketQueue(PacketQueue *q , AVPacket *pkt);						//����Ƶ��������������
	friend int getFromPacketQueue(PacketQueue *q , AVPacket *pkt ,int block);	//����Ƶ��������ȡ����
	friend int audioDecodeFrame(AVCodecContext *aCodecCtx, uint8_t *audio_buf, int buf_size);	//��Ƶ����֡
	void putIntoVideoPicList(VideoPictList *vpl,VideoPict *picTemp);			//����Ƶ�����в���һ֡����
	unsigned char * getFormVideoPicList(VideoPictList *vpl);					//����Ƶ֡��������ȡһ֡����
};
#endif
