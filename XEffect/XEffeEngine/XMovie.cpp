//++++++++++++++++++++++++++++++++
//Author:	��ʤ��(JiaShengHua)
//Version:	1.0.0
//Date:		See the header file
//--------------------------------
#include "XMovie.h"
#include "SDL_mixer.h"
#include "math.h"
#include "XBasicSDL.h"

PacketQueue _XMovie::audioq;
bool _XMovie::m_isStop = false;
bool _XMovie::m_isQuit = false;

_XMovie::_XMovie(void)
:videoStream(-1)
,audioStream(-1)
,m_isBuffing(true)
,m_isLoop(false)
,m_videoWidth(0)
,m_videoHeight(0)
,m_nowFrameNumber(0)
,m_needVoice(1)
,m_texture(NULL)
,m_isLoaded(0)
{
	videofilename[0] = '\0';
	videopictlist.nb_pict = 0;
	m_isEnd = false;
	m_isQuit = false;
	m_isStop = false;
}

_XMovie::~_XMovie(void)
{
}

//��Ƶ��ʾ����Ƶͬ���߳�
#ifdef OS_LINUX
void * drawThread(void *arg)
#endif
#ifdef OS_WINDOWS
DWORD WINAPI drawThread(void *arg)
#endif
{
	_XMovie *pthis = (_XMovie *)arg;
	float refreshtime = 1000.0f/(* pthis).m_nowFrameRate;//ӰƬ��ʾһ֡��ʱ�䣬��λ������
	float temptime = refreshtime;

	//��һ����ʱ��Ҫ�Ȼ�����
	pthis->waitingForBuffOver();

	int startTime = SDL_GetTicks();		//��¼��ʼ���ŵ�ʱ��
	pthis->m_nowPlayedTime = 0;			//���ŵ�ʱ��ͨ�����������

	while(1)
	{	
		if(pthis->m_isQuit)
		{
		//	SDL_CloseAudio();	//�ر������豸���˳�
			Mix_HookMusic(NULL,NULL);	//�������֮��ȡ������
			break;
		}
		if(pthis->m_isEnd)
		{//��������Ѿ�����
			if(!pthis->m_isLoop)
			{//�������Ҫѭ������
			//	SDL_CloseAudio();	//�ر������豸���˳�
				Mix_HookMusic(NULL,NULL);
				printf("NO Loop!\n");
				break;
			}
			printf("All times:%d\n",pthis->m_nowPlayedTime);
			pthis->m_isEnd = false;
			pthis->m_nowFrameNumber = 0;
			pthis->m_nowPlayedTime = 0;
			startTime = SDL_GetTicks();	
			printf("Loop!\n");
		}
		if(pthis->m_isStop)
		{//ֹͣ״̬
			mySleep(1);
			startTime = SDL_GetTicks();
			continue;
		}
		if(pthis->videopictlist.nb_pict > 0)
		{//��Ƶ������������
			pthis->m_nowPlayedTime += SDL_GetTicks() - startTime;
			startTime = SDL_GetTicks();
		//	playedTime = SDL_GetTicks() - startTime;
			if(pthis->m_nowPlayedTime>= pthis->m_nowFrameNumber * temptime)
			{//����ﵽʱ�����ʼ����
				pthis->draw();
				if(pthis->m_nowFrameNumber >= pthis->m_allFrameSum)
				{//���ʱ��Խ�磬�򲥷����
					pthis->m_isEnd = true;
					printf("Time play over!\n");
				}
			}else
			{//������ȵ�ʱ����ٲ���
				mySleep(pthis->m_nowFrameNumber * temptime - pthis->m_nowPlayedTime);
			}
		}else
		{
			printf("-->Some Wrong!\n");
			pthis->m_isStop = true;
			mySleep(5000);
			pthis->m_isStop = false;
			startTime = SDL_GetTicks();
		}
		mySleep(1);
	}
//	_endthread();
#ifdef OS_WINDOWS
	return 1;
#endif
}

//��Ƶ�����̣߳���DrawThread�̲߳���ʾ��Ƶ��ʱ�����Ƶ֡���뵽���У�����Ƶ���ŵ�����
#ifdef OS_WINDOWS
DWORD WINAPI decoderThread(void *arg)
#endif
#ifdef OS_LINUX
void * decoderThread(void *arg)
#endif
{
	_XMovie *pthis = (_XMovie *)arg;
	//AVPicture pict;
	int bufernum = (int)(2.0 * pthis->m_nowFrameRate); //���û�������СΪӰƬ2�벥�ŵ�֡��
	int frameFinished;		//���붯���Ƿ����
	if(bufernum >= MAX_BUFF_DEEP) bufernum = MAX_BUFF_DEEP;

	while(1)
	{
		if(pthis ->m_isQuit) 
		{
			break;
		}

		if((pthis->videopictlist.nb_pict) < bufernum)//�տ�ʼ����2��
		{//���û�л���������л���
			if(av_read_frame(pthis->pFormatCtx , &pthis->packet) >= 0)
			{//�����ȡ֡�ɹ�
				if(pthis->packet.stream_index == pthis->videoStream)
				{			
					avcodec_decode_video(pthis->pCodeCtx , pthis->pFrame , &frameFinished , pthis->packet.data , pthis->packet.size);//��Ƶ����
					if(frameFinished)
					{//�������ɹ�		
						SDL_LockMutex(pthis->m_mutex);
						//ʹ��RGB24�ķ�ʽ����֡��ʾ����
						img_convert(pthis->m_bmp[pthis->m_nowBuffPoint]->pict, PIX_FMT_RGB24,(AVPicture *)pthis->pFrame, pthis->pCodeCtx->pix_fmt,pthis->pCodeCtx->width,pthis->pCodeCtx->height); 
						pthis->putIntoVideoPicList(&pthis->videopictlist ,pthis->m_bmp[pthis->m_nowBuffPoint]);
						++ pthis->m_nowBuffPoint;
						if(pthis->m_nowBuffPoint >= MAX_BUFF_DEEP) pthis->m_nowBuffPoint = 0;
						SDL_UnlockMutex(pthis->m_mutex);
					}
				}
				if(pthis->packet.stream_index == pthis->audioStream &&  pthis->m_needVoice != 0)
				{//��ȡ�õ���Ƶ���Ž�����
					pthis->putIntoPacketQueue(&pthis->audioq , &pthis->packet);
				}else
				{
					av_free_packet(&pthis->packet);
				}
			}else	//�����ȡ֡ʧ�ܣ���Ĭ���Ѿ������� 
			if(pthis->m_isLoop)//�����Ƶ������ϣ�������ѭ��������ѭ������
			{
				av_free(pthis->pFormatCtx);
				if(av_open_input_file(&pthis->pFormatCtx ,pthis->videofilename,NULL,0,NULL) != 0)
				{//���´򿪲����ļ�
#ifdef OS_WINDOWS
					return 0;
#endif
#ifdef OS_LINUX
					break;
#endif
				}
			}else
			{//��Ƶ������ϣ��˳������߳�
				break;
			}
		}else//�����ʾ�������� 
		if(pthis->m_isBuffing)	//��һ�β��ŵ�ʱ��һ��Ҫ�Ȼ�����
		{//������ϣ�����bplay , ���в���
			pthis->m_isBuffing = false;
		}
		mySleep(1);
	}
//	_endthread();
#ifdef OS_WINDOWS
	return 1;
#endif
}


//����Ƶ������ȡ����Ƶ��
int getFromPacketQueue(PacketQueue *q, AVPacket *pkt, int block)
{ 
	AVPacketList *pkt1; 
	int ret; 

	SDL_LockMutex(q->mutex); 
	while(1)
	{ 
		if(_XMovie::m_isStop)
		{//��ͣ�Ļ�����ֹͣ��������
			SDL_UnlockMutex(q->mutex); 
			return -1;
		}

		pkt1 = q->first_pkt; 
		if (pkt1) 
		{ 
			q->first_pkt = pkt1->next; 
			if (!q->first_pkt) 
				q->last_pkt = NULL; 
			--(q->nb_packets); 
			q->size -= pkt1->pkt.size; 
			*pkt = pkt1->pkt; 
			av_free(pkt1); 
			ret = 1; 
			break; 
		}else 
		if(!block)
		{ 
			ret = 0; 
			break; 
		}else 
		{ 
			SDL_CondWait(q->cond, q->mutex); 
		} 
		mySleep(1);
	} 
	SDL_UnlockMutex(q->mutex); 
	return ret; 
} 

//����Ƶ���������Ƶ����
int audioDecodeFrame(AVCodecContext *aCodecCtx, uint8_t *audio_buf, int buf_size)
{ 
	static AVPacket pkt; 
	static uint8_t *audio_pkt_data = NULL; 
	static int audio_pkt_size = 0; 

	int len1 = 0;
	int data_size = 0; 
	while(1)
	{ 
		if(_XMovie::m_isQuit)
		{
			return -1;
		}
		while(audio_pkt_size > 0) 
		{ 
			if(_XMovie::m_isQuit)
			{
				return -1;
			}
			data_size = buf_size; 
			//��audio_pkt_data��ַ�ж�ȡaudio_pkt_size�����ݲ���ѹ��audio_buf�����ַ�г���Ϊdata_size
			len1 = avcodec_decode_audio(aCodecCtx, (int16_t *)audio_buf, &data_size, audio_pkt_data, audio_pkt_size); //��������
			if(len1 < 0) 
			{//���뷢������ 
				audio_pkt_size = 0; 
				break; 
			} 
			audio_pkt_data += len1;		//���һ����Ƶ����û��������ظ�����ֱ�����Ϊֹ
			audio_pkt_size -= len1; 
			if(data_size <= 0) 
			{     
				continue; 
			} 
			return data_size; 
		} 
		if(pkt.data) 
			av_free_packet(&pkt); 
 
		if(_XMovie::m_isStop)
		{//��ͣ�Ļ�����ֹͣ��������
			return -1;
		}
		if(getFromPacketQueue(&_XMovie::audioq, &pkt, 1) < 0)
		{//�����Ƶ��û�����ݣ��򷵻� 
			return -1; 
		} 
		audio_pkt_data = pkt.data; 
		audio_pkt_size = pkt.size; 
	} 
} 

//�����ص�������������Ƶ����,����Ƶ�����Ƶ����湩SDL����
void audioCallBack(void *userdata , Uint8 *stream , int len)
{
	AVCodecContext *aCodecCtx = (AVCodecContext *)userdata; 
	int len1, audio_size; 

//	static uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]; 
	static uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2]; 
	static unsigned int audio_buf_size = 0; 
	static unsigned int audio_buf_index = 0; 

	while(len > 0)
	{//����������Ҫ����ʱ 	
		if(audio_buf_index >= audio_buf_size)
		{//����ѹ����Ƶȫ���������ʱ�Ž�ѹ�������Ƶ
			if(_XMovie::m_isQuit)
			{
				return; 
			}
			audio_size = audioDecodeFrame(aCodecCtx, audio_buf, sizeof(audio_buf));//�������뺯�� 
			if(audio_size < 0) 
			{//�����Ƶ����û������.������ڴ�ռ�
				audio_buf_size = 1024; 
				memset(audio_buf, 0, audio_buf_size); 
			}else
			{//��¼��ѹ��������Ƶ������
				audio_buf_size = audio_size; 
			} 
			audio_buf_index = 0; 
		} 
		len1 = audio_buf_size - audio_buf_index;	//��¼��Ƶ�ĳ���
		if(len1 > len) 
			len1 = len;

		//������Ҫ����Ƶ����ת��(δ�����)
		//if(XEE_audioFrequency != aCodecCtx->sample_rate
		////	|| aCodecCtx->sample_fmt != XEE_audioFormat
		//	|| aCodecCtx->channels != XEE_audioChannel)
		//{
		//	SDL_AudioCVT  wav_cvt;
		//	int ret = SDL_BuildAudioCVT(&wav_cvt,
		//	//	aCodecCtx->sample_fmt, aCodecCtx->channels, aCodecCtx->sample_rate,
		//		XEE_audioFormat,aCodecCtx->channels,aCodecCtx->sample_rate,
		//		XEE_audioFormat,XEE_audioChannel,XEE_audioFrequency);
		//	if(ret == -1)
		//	{
		//		fprintf(stderr, "Couldn't build converter!\n");
		//	}
		//	wav_cvt.buf = (Uint8 *)malloc(len1 * wav_cvt.len_mult);
		//	wav_cvt.len = len1;
		//	memcpy(wav_cvt.buf,audio_buf + audio_buf_index,wav_cvt.len);
		//	SDL_ConvertAudio(&wav_cvt);
		//	SDL_MixAudio(stream,wav_cvt.buf, wav_cvt.len_cvt,128);
		//	free(wav_cvt.buf);
		//}else
		//{
		//	SDL_MixAudio(stream, (uint8_t *)audio_buf + audio_buf_index, len1,128);
		//}
		SDL_MixAudio(stream, (uint8_t *)audio_buf + audio_buf_index, len1,128);
		len -= len1; 
		stream += len1; 
		audio_buf_index += len1; 
	//	mySleep(1);
	}
}

//��ӰƬ ����ʼ��SDL , ffmpeg���
int _XMovie::load(const char *filename)
{
	if(m_isLoaded != 0) return 0;
	videoStream = -1;
	audioStream = -1;
	m_isBuffing = true;
	m_isStop = true;
//	m_isLoop = FALSE;
	m_videoWidth = 0;
	m_videoHeight = 0;
	m_nowFrameNumber = 0;
	videopictlist.nb_pict = 0;
	m_isEnd = false;
	m_isQuit = false;
	m_nowFrame = NULL;
//	m_needVoice = 1;

	int i = 0;
	strcpy(videofilename,filename);		//��¼��Ƶ�ļ���
	av_register_all();					//��ʼ��ffmpeg��
	if(av_open_input_file(&pFormatCtx ,videofilename , NULL , 0 , NULL) != 0)	//����Ƶ�ļ�
	{
		printf("can not open input file!\n");
		return 0;
	}
	if(av_find_stream_info(pFormatCtx) < 0)		//�����Ƶ����Ϣ
	{
		printf("can not find stream information!\n");
		return 0;
	}

	for(i = 0;i < pFormatCtx->nb_streams;++ i)
	{
		if((pFormatCtx->streams[i]->codec->codec_type) == CODEC_TYPE_VIDEO && videoStream < 0)
		{//����Ƶ��Ϣ�в�����Ƶ���������Ϣ
			videoStream = i;
		}
		if((pFormatCtx->streams[i]->codec->codec_type) == CODEC_TYPE_AUDIO && audioStream < 0)
		{//����Ƶ��Ϣ�в�����Ƶ���������Ϣ
			audioStream = i;
		}
	}

	m_videoWidth = 0;				//�����Ƶ�Ŀ��
	m_videoHeight = 0;			//�����Ƶ�ĸ߶�
	if(videoStream != -1)
	{//����ļ��д�����Ƶ��
		pCodeCtx = pFormatCtx->streams[videoStream]->codec;		//�õ���Ƶ���ı������Ϣ
		AVCodec *pCodec = avcodec_find_decoder(pCodeCtx->codec_id);		//��ffmpeg���в�������Ƶ�������Ϣ��ƥ��ı������

		if(pCodec == NULL)
		{//�Ҳ�����Ӧ�Ľ��뷽ʽ
			fprintf(stderr , "Unsupported codec!\n");
			return 0;
		}
		if(avcodec_open(pCodeCtx ,pCodec) < 0)
		{//������Ƶ�������Ϣ��ƥ��ı������
			printf("can not open video decoder!\n");
			return 0;
		}
		initVideoPictList(&videopictlist);	//��ʼ����Ƶ֡����
		m_videoWidth = pCodeCtx->width;			//�����Ƶ�Ŀ��
		m_videoHeight = pCodeCtx->height;			//�����Ƶ�ĸ߶�
	}

	if(audioStream != -1 && m_needVoice != 0)
	{//���������Ƶ�������濪����Ƶ����
		aCodeCtx = pFormatCtx->streams[audioStream]->codec;	//�õ���Ƶ���ı������Ϣ
		AVCodec *aCodec = avcodec_find_decoder(aCodeCtx->codec_id);	//��ffmpeg���в�������Ƶ�������Ϣ��ƥ��ı������
		if(aCodec == NULL)
		{//�Ҳ�����Ƶ������
			printf("can not find audio decoder information!\n");
		}else 
		if(avcodec_open(aCodeCtx ,aCodec) < 0)
		{//������Ƶ�������Ϣ��ƥ��ı������
			printf("can not open audio decoder!\n");
		}
		else
		{//���ݲ��ҵ�����Ƶ�������Ϣ����SDL�貥�ŵ���Ƶ��Ϣ
		/*	wanted_spec.freq = aCodeCtx->sample_rate;	//��Ƶ��Ƶ��
			wanted_spec.format = AUDIO_S16SYS;			//��Ƶ��ʽ
			wanted_spec.channels = aCodeCtx->channels;	//��Ƶ������
			wanted_spec.silence = 0;					//�Ƿ���
			wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;//��Ƶ��������С
			wanted_spec.callback = audioCallBack;		//��Ƶ����ص�����
			wanted_spec.userdata = aCodeCtx;			//�ص���������Ƶ������Դ*/

			//ע�⣺�������bug��������Ƶ����Ƶ������ʵ�ʵ���Ƶ������ڲ�࣬��Ҫ�޸�
			//Mix_CloseAudio();
			//const int    TMP_FREQ = aCodeCtx->sample_rate;
			//const Uint16 TMP_FORMAT = AUDIO_S16SYS;
			//const int    TMP_CHAN = aCodeCtx->channels;
			//const int    TMP_CHUNK_SIZE = SDL_AUDIO_BUFFER_SIZE;
			//Mix_OpenAudio(TMP_FREQ,TMP_FORMAT,TMP_CHAN,TMP_CHUNK_SIZE);
			//Mix_AllocateChannels(128);

			initPacketQueue(&audioq);//��ʼ������������
		}
	}

	pFrame = avcodec_alloc_frame();//����һ֡���ڴ�ռ䣬���ڴ�Ž�������Ƶ��Ϣ
	if(pFrame == NULL)
	{//��������ڴ�ʧ�ܣ���������ʾ
		printf("malloc Frame failed!\n");
		return 0;
	}

	m_nowFrameRate = (double)(pFormatCtx->streams[videoStream]->time_base.den)/(double)(pFormatCtx->streams[videoStream]->time_base.num);
	m_allFrameSum = pFormatCtx->streams[videoStream]->duration;
	//����OpenGL��ͼ
	if(m_texture != NULL)
	{
		glDeleteTextures(1,&m_texture);
	}
	glGenTextures(1,&m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//�����޸Ľ���һ������2��n�η�����ͼ
	int wR = (int)powf(2.0, ceilf(logf((float)m_videoWidth)/logf(2.0f)));
	int hR = (int)powf(2.0, ceilf(logf((float)m_videoHeight)/logf(2.0f)));
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,wR,hR, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,m_videoWidth,m_videoHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    //�����߳�
#ifdef OS_LINUX
	pthread_t tempP;
    if(pthread_create(&tempP, NULL, &decoderThread, (void*) this) != 0) 
	{
		printf("open thread error!\n");
	}
    if(pthread_create(&tempP, NULL, &drawThread, (void*) this) != 0) 
	{
		printf("open thread error!\n");
	}
#endif
#ifdef OS_WINDOWS
    if(CreateThread(0,0,decoderThread,this,0,NULL) == 0) 
	{
		printf("open thread error!\n");
	}
    if(CreateThread(0,0,drawThread,this,0,NULL) == 0) 
	{
		printf("open thread error!\n");
	}
#endif

//	_beginthread(decoderThread , 0 , this);	//����������߳�
//	_beginthread(drawThread , 0 , this);	//������ͼ���߳�

	int numBytes = avpicture_get_size(PIX_FMT_RGB24, pCodeCtx->width,pCodeCtx->height);	//��ȡ�ڴ�ߴ�
	m_nowBuffPoint = 0;
	for(int i = 0;i < MAX_BUFF_DEEP;++ i)
	{
		m_bmp[i] = (VideoPict*)av_malloc(sizeof(VideoPict));
		if(m_bmp[i] == NULL) return 0;

		m_frameDataBuff[i] = (unsigned char *)av_malloc(numBytes*sizeof(unsigned char));
		avpicture_fill(&m_framePic[i],m_frameDataBuff[i], PIX_FMT_RGB24,pCodeCtx->width,pCodeCtx->height);

		m_bmp[i]->pict = &m_framePic[i];
		m_bmp[i]->data = m_frameDataBuff[i];
	}

	printf("AllFrames:%d,FPS:%f\n",(* this).m_allFrameSum,(* this).getVideoFrameRate());
	m_mutex = SDL_CreateMutex();

	m_isLoaded = 1;

	return 1;
}

void _XMovie::init(SDL_Surface *temp)
{
	m_screen = temp;
}

//ȡ��ӰƬ�Ŀ��
int _XMovie::getClipWide(void)
{
	return m_videoWidth;
}

//ȡ��ӰƬ�ĸ߶�
int _XMovie::getClipHigh(void)
{
	return m_videoHeight;
}

//����ӰƬ�Ƿ�ѭ�����ţ�TRUEѭ�����ţ�FALSE���
void _XMovie::setLoop(bool isLoop)
{
	m_isLoop = isLoop;
}

//��ʼ����ӰƬ���������Ӧ�����ڲ�ͣ�Ĳ���ѹ��������ƵƬ�ϣ�
void _XMovie::waitingForBuffOver(void)
{
	while(1)
	{//�ȴ���Ƶ�������
		if(!m_isBuffing)
		{//��������֮��������豸
			if(audioStream != -1 &&  m_needVoice == 1)
			{//���������Ƶ��
			/*	SDL_AudioSpec spec;
				if(SDL_OpenAudio(&wanted_spec , &spec) < 0)
				{//�������豸
					fprintf(stderr ,"SDL_OpenAudio: %s\n" , SDL_GetError());
				}
				SDL_PauseAudio(0); */
				Mix_HookMusic(audioCallBack,aCodeCtx);
			}
			break;
		}
		mySleep(1);
	}	
}

double _XMovie::getCurrentPlayingTime()
{//����ӰƬ��ǰ����ʱ�䣬��λ������
	return m_nowFrameNumber*(1000.0/m_nowFrameRate);
}

int _XMovie::getAllTime(void)
{//����ӰƬ��ʱ�䣬��λ������
	return ((double)pFormatCtx->duration * 0.001f);
}

float _XMovie::getVideoFrameRate()
{//����ӰƬ�Ĳ���֡��
	return m_nowFrameRate;
}

bool _XMovie::getUpdateFlag(void)
{//��Ƶ������в��շ���TRUE�����򷵻�FALSE
	if(videopictlist.nb_pict > 0)
	{
		return true;
	}else
	{
		return false;
	}
}

//��ͣ���������ӰƬ
void _XMovie::setPauseOrPlay(void)
{	
	m_isStop = !m_isStop;
}

//�ر�ӰƬ����
void _XMovie::closeClip(void)
{
	if(m_isLoaded == 0) return;
	m_isQuit = true;
	mySleep(20);
	av_free(pFrame);
	avcodec_close(pCodeCtx);
	av_close_input_file(pFormatCtx);
	releaseVideoPictList();
	releasePacketQueue();
	if(m_texture != NULL)
	{
		glDeleteTextures(1,&m_texture);
	}
	m_isLoaded = 0;
}

void _XMovie::gotoFrame(float temp)	//������ǰ��Ƶ��ĳһ֡
{
	if(temp < 0) temp = 0;
	if(temp > 1) temp = 1;

	int armTime = temp * m_allFrameSum;
	int rt = av_seek_frame(pFormatCtx,videoStream,armTime,AVSEEK_FLAG_BACKWARD);
	printf("%d,%d,%d,%d\n",rt,m_allFrameSum,armTime,pFormatCtx->timestamp);
	m_nowFrameNumber = temp * m_allFrameSum - videopictlist.nb_pict;	//�û����е����ݿ��ٲ������
	m_nowPlayedTime = m_nowFrameNumber * 1000.0f / m_nowFrameRate;	//��������ʱ��
}

//��YUV��ʽ����Ƶ֡��ʾ������
void _XMovie::draw(void)
{  
	SDL_LockMutex(m_mutex);
	m_nowFrame = getFormVideoPicList(&videopictlist);
	SDL_UnlockMutex(m_mutex);
	if(m_nowFrame == NULL) return;

	++ m_nowFrameNumber;
}

GLuint * _XMovie::getTexture()
{
	if(m_nowFrame != NULL)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,m_texture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 
			m_videoWidth,m_videoHeight, GL_RGB,//GL_LUMINANCE,	
			GL_UNSIGNED_BYTE,m_nowFrame);
		glDisable( GL_TEXTURE_2D );
		m_nowFrame = NULL;
	}
	return &m_texture;
}

//��ʼ����Ƶ����
void _XMovie::initVideoPictList(VideoPictList *vpl)
{
	memset(vpl,0,sizeof(VideoPictList));
	vpl->mutex = SDL_CreateMutex();
}

void _XMovie::releaseVideoPictList()
{
	for(int i = 0;i < MAX_BUFF_DEEP;++ i)
	{
	//	SDL_FreeYUVOverlay(m_bmp[i]->bmp);
		av_free(m_bmp[i]->data);
		av_free(m_bmp[i]);
	}
}

//�ѽ�������Ƶ֡�ŵ�������
void _XMovie::putIntoVideoPicList(VideoPictList *vpl,VideoPict *picTemp)
{
	picTemp->next = NULL;
	SDL_LockMutex(vpl->mutex);
	if(vpl->last_pict == NULL)
	{//����ǿ���
		vpl->first_pict = picTemp;
	}else
	{//�������һ֡
		vpl->last_pict->next = picTemp;
	}
	vpl->last_pict = picTemp;
	++ vpl->nb_pict;
	SDL_UnlockMutex(vpl->mutex);
}

//�ڶ�����ȡ����Ƶ֡
unsigned char* _XMovie::getFormVideoPicList(VideoPictList *vpl)
{
	VideoPict *vp = NULL;
	if(vpl == NULL)
	{//���ӰƬ֡����Ϊ����,ֱ�ӷ���
		return NULL;
	}
	if(vpl->nb_pict <= 0) return NULL;
	SDL_LockMutex(vpl->mutex);
	vp = vpl->first_pict;
	if(vp)
	{
		vpl->first_pict = vp->next;
		if(vpl->first_pict == NULL)
		{
			vpl->last_pict = NULL;
			printf("buffer no data!\n");
		}
		--(vpl->nb_pict);
	}
	SDL_UnlockMutex(vpl->mutex);
	return vp->data;
}

//��ʼ����Ƶ������
void _XMovie::initPacketQueue(PacketQueue *q)
{
	memset(q,0,sizeof(PacketQueue));
	q->mutex = SDL_CreateMutex();
	q->cond = SDL_CreateCond();
}

//����Ƶ��pkt�ŵ�����q��
int _XMovie::putIntoPacketQueue(PacketQueue *q , AVPacket *pkt)
{
	AVPacketList *pkt_temp;
	//������Ҫ�������޸Ľ���Ƶ����ת������Ҫ�ĸ�ʽ����
	if(av_dup_packet(pkt) < 0)
	{
		return -1;
	}

	pkt_temp = (AVPacketList*)av_malloc(sizeof(AVPacketList));
	if(!pkt_temp)
	{
		return -1;
	}
	pkt_temp->pkt = *pkt;
	pkt_temp->next = NULL;

	SDL_LockMutex(q->mutex);
	if(!q->last_pkt)
	{
		q->first_pkt = pkt_temp;
	}
	else
	{
		q->last_pkt->next = pkt_temp;
	}
	q->last_pkt = pkt_temp;
	++ q->nb_packets;
	q->size += pkt_temp->pkt.size;
	SDL_CondSignal(q->cond);
	SDL_UnlockMutex(q->mutex);
	return 0;
}

void _XMovie::releasePacketQueue()
{
	AVPacketList *pkt1; 
	while(1)
	{ 
		pkt1 = audioq.first_pkt; 
		if(pkt1) 
		{ 
			audioq.first_pkt = pkt1->next; 
			if (!audioq.first_pkt) 
				audioq.last_pkt = NULL; 
			--(audioq.nb_packets); 
			audioq.size -= pkt1->pkt.size; 
			av_free(pkt1); 
		}else
		{
			break;
		}
	}
}
