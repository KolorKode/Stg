#ifndef __DSP_ODDCAST_H
#define __DSP_ODDCAST_H

#include <pthread.h>

#include "cbuffer.h"

#include "libedcast_socket.h"
#ifdef HAVE_VORBIS
#include <vorbis/vorbisenc.h>
#endif

#include <stdio.h>
#include <time.h>

#ifdef _DMALLOC_
#include <dmalloc.h>
#endif

#include <OggStegoEncoder.h>

/*
#ifdef _UNICODE
#define char_t wchar_t
#define atoi _wtoi
#define LPCSTR LPCWSTR 
#define strcpy wcscpy
#define strcmp wcscmp
#define strlen wcslen
#define fopen _wfopen
#define strstr wcsstr
#define sprintf swprintf
#define strcat wcscat
#define fgets fgetws
#else
#define char_t char
#endif
*/
#define char_t char

#ifdef __cplusplus
extern "C" {
#endif
#include "libedcast_resample.h"
#ifdef __cplusplus
}
#endif

#ifdef WIN32
#include <BladeMP3EncDLL.h>
#else
#ifdef HAVE_LAME
#include <lame/lame.h>
#endif
#endif

#ifdef HAVE_FAAC
#undef MPEG2 // hack *cough* hack
typedef signed int  int32_t; 
#include <faac.h>
#endif

#ifdef HAVE_AACP
#include "enc_if.h"
typedef AudioCoder* (*CREATEAUDIO3TYPE)(int , int , int , unsigned int , unsigned int *, char_t *);
typedef unsigned int (*GETAUDIOTYPES3TYPE)(int, char_t *);
#endif

#define LM_FORCE 0
#define LM_ERROR 1
#define LM_INFO 2
#define LM_DEBUG 3
#ifdef WIN32
#define LOG_FORCE LM_FORCE, TEXT(__FILE__), __LINE__
#define LOG_ERROR LM_ERROR, TEXT(__FILE__), __LINE__
#define LOG_INFO LM_INFO, TEXT(__FILE__), __LINE__
#define LOG_DEBUG LM_DEBUG, TEXT(__FILE__), __LINE__
#else
#define LOG_FORCE LM_FORCE, __FILE__, __LINE__
#define LOG_ERROR LM_ERROR, __FILE__, __LINE__
#define LOG_INFO LM_INFO, __FILE__, __LINE__
#define LOG_DEBUG LM_DEBUG, __FILE__, __LINE__
#endif


#ifdef HAVE_FLAC
#include <FLAC/stream_encoder.h>
extern "C" {
#include <FLAC/metadata.h>
}
#endif

#define FormatID 'fmt '   /* chunkID for Format Chunk. NOTE: There is a space at the end of this ID. */
// For skin stuff
#define WINDOW_WIDTH		276
#define WINDOW_HEIGHT		150

#ifndef FALSE
#define FALSE false
#endif
#ifndef TRUE
#define TRUE true
#endif
#ifndef WIN32
#include <sys/ioctl.h>
#else
#include <mmsystem.h>
#endif
// Callbacks
#define	BYTES_PER_SECOND 1

#define FRONT_END_ODDCAST_PLUGIN 1
#define FRONT_END_TRANSCODER 2

typedef struct tagLAMEOptions {
	int		cbrflag;
	int		out_samplerate;
	int		quality;
#ifdef WIN32
	int		mode;
#else
#ifdef HAVE_LAME
	MPEG_mode	mode;
#endif
#endif
	int		brate;
	int		copywrite;
	int		original;
	int		strict_ISO;
	int		disable_reservoir;
	char_t		VBR_mode[25];
	int		VBR_mean_bitrate_kbps;
	int		VBR_min_bitrate_kbps;
	int		VBR_max_bitrate_kbps;
	int		lowpassfreq;
	int		highpassfreq;
} LAMEOptions;

typedef struct {
	char_t	RIFF[4];
	long	chunkSize;
	char_t	WAVE[4];
} RIFFChunk;

typedef struct {
  char_t		chunkID[4];
  long		chunkSize;

  short          wFormatTag;
  unsigned short wChannels;
  unsigned long  dwSamplesPerSec;
  unsigned long  dwAvgBytesPerSec;
  unsigned short wBlockAlign;
  unsigned short wBitsPerSample;

/* Note: there may be additional fields here, depending upon wFormatTag. */

} FormatChunk;


typedef struct {
	char_t	chunkID[4];
	long	chunkSize;
	short *	waveformData;
} DataChunk;

struct wavhead
{
	unsigned int  main_chunk;
	unsigned int  length;
	unsigned int  chunk_type;
	unsigned int  sub_chunk;
	unsigned int  sc_len;
	unsigned short  format;
	unsigned short  modus;
	unsigned int  sample_fq;
	unsigned int  byte_p_sec;
	unsigned short  byte_p_spl;
	unsigned short  bit_p_spl;
	unsigned int  data_chunk;
	unsigned int  data_length;
};


static struct wavhead   wav_header;

// Global variables....gotta love em...
typedef struct {
	long		currentSamplerate;
	int		currentBitrate;
	int		currentBitrateMin;
	int		currentBitrateMax;
	int		currentChannels;
	int		gSCSocket;
	int		gSCSocket2;
	int		gSCSocketControl;
	CMySocket	dataChannel;
	CMySocket	controlChannel;
	int		gSCFlag;
	int		gCountdown;
	int		gAutoCountdown;
	int		automaticconnect;
	char_t		gSourceURL[1024];
	char_t		gServer[256];
	char_t		gPort[10];
	char_t		gPassword[256];
	int		weareconnected;
	char_t		gIniFile[1024];
	char_t		gAppName[256];
	char_t		gCurrentSong[1024];
	int			gSongUpdateCounter;
	char_t		gMetadataUpdate[10];
	int			gPubServ;
	char_t		gServIRC[20];
	char_t		gServICQ[20];
	char_t		gServAIM[20];
	char_t		gServURL[1024];
	char_t		gServDesc[1024];
	char_t		gServName[1024];
	char_t		gServGenre[100];
	char_t		gMountpoint[100];
	char_t		gFrequency[10];
	char_t		gChannels[10];
	int			gAutoReconnect;
	int 		gReconnectSec;
	char_t		gAutoStart[10];
	char_t		gAutoStartSec[20];
	char_t		gQuality[5];
#ifndef WIN32
#ifdef HAVE_LAME
	lame_global_flags *gf;
#endif
#endif
	int		gCurrentlyEncoding;
	int		gFLACFlag;
	int		gAACFlag;
	int		gAACPFlag;
	int		gOggFlag;
	char_t		gIceFlag[10];
	int		gLAMEFlag;
	char_t		gOggQuality[25];
	int			gLiveRecordingFlag;
	int		gOggBitQualFlag;
	char_t	gOggBitQual[40];
	char_t	gEncodeType[25];
	int		gAdvancedRecording;
	int		gNOggchannels;
	char_t		gModes[4][255];
	int		gShoutcastFlag;
	int		gIcecastFlag;
	int		gIcecast2Flag;
	char_t		gSaveDirectory[1024];
	char_t		gLogFile[1024];
	int			gLogLevel;
	FILE		*logFilep;
	int		gSaveDirectoryFlag;
	int		gSaveAsWAV;
	FILE		*gSaveFile;
	LAMEOptions	gLAMEOptions;
	int		gLAMEHighpassFlag;
	int		gLAMELowpassFlag;

	int		oggflag;
	int		serialno;
#ifdef HAVE_VORBIS
	ogg_sync_state  oy_stream;
	ogg_packet header_main_save;
	ogg_packet header_comments_save;
	ogg_packet header_codebooks_save;
#endif
	bool		ice2songChange;
	int			in_header;
	long		 written;
	int		vuShow;

	int		gLAMEpreset;
	char_t	gLAMEbasicpreset[255];
	char_t	gLAMEaltpreset[255];
	char_t    gSongTitle[1024];
	char_t    gManualSongTitle[1024];
	int		gLockSongTitle;
    int     gNumEncoders;

	res_state	resampler;
	int	initializedResampler;
	void (*sourceURLCallback)(void *, void *);
	void (*destURLCallback)(void *, void *);
	void (*serverStatusCallback)(void *, void *);
	void (*generalStatusCallback)(void *, void *);
	void (*writeBytesCallback)(void *, void *);
	void (*serverTypeCallback)(void *, void *);
	void (*serverNameCallback)(void *, void *);
	void (*streamTypeCallback)(void *, void *);
	void (*bitrateCallback)(void *, void *);
	void (*VUCallback)(int, int);
	long	startTime;
	long	endTime;
	char_t	sourceDescription[255];

	char_t	gServerType[25];
#ifdef WIN32
	WAVEFORMATEX waveFormat;
	HWAVEIN      inHandle;
	WAVEHDR                 WAVbuffer1;
	WAVEHDR                 WAVbuffer2;
#else
	int	inHandle; // for advanced recording
#endif
	unsigned long result;
	short int WAVsamplesbuffer1[1152*2];
	short int WAVsamplesbuffer2[1152*2];
	bool  areLiveRecording;
	char_t	gAdvRecDevice[255];
#ifndef WIN32
	char_t	gAdvRecServerTitle[255];
#endif
	int		gLiveInSamplerate;
#ifdef WIN32
	// These are for the LAME DLL
        BEINITSTREAM            beInitStream;
        BEENCODECHUNK           beEncodeChunk;
        BEDEINITSTREAM          beDeinitStream;
        BECLOSESTREAM           beCloseStream;
        BEVERSION               beVersion;
        BEWRITEVBRHEADER        beWriteVBRHeader;
        HINSTANCE       hDLL;
        HINSTANCE       hFAACDLL;
        HINSTANCE       hAACPDLL;
        DWORD           dwSamples;
        DWORD           dwMP3Buffer;
        HBE_STREAM      hbeStream;
#endif
		char_t	gConfigFileName[255];
		char_t	gOggEncoderText[255];
		int		gForceStop;
		char_t	gCurrentRecordingName[1024];
		long	lastX;
		long	lastY;
		long	lastDummyX;
		long	lastDummyY;

#ifdef HAVE_VORBIS
		ogg_stream_state os;
		vorbis_dsp_state vd;
		vorbis_block     vb;
		vorbis_info      vi;
#endif

		int	frontEndType;
		int	ReconnectTrigger;
#ifdef HAVE_AACP
		CREATEAUDIO3TYPE	CreateAudio3;
		GETAUDIOTYPES3TYPE	GetAudioTypes3;

		AudioCoder *(*finishAudio3)(char_t *fn, AudioCoder *c);
		void (*PrepareToFinish)(const char_t *filename, AudioCoder *coder);
		AudioCoder * aacpEncoder;
#endif
#ifdef HAVE_FAAC
		faacEncHandle aacEncoder;
#endif
		unsigned long samplesInput, maxBytesOutput;
		float   *faacFIFO;
		int     faacFIFOendpos;
		char_t		gAACQuality[25];
		char_t		gAACCutoff[25];
        int     encoderNumber;
        bool    forcedDisconnect;
        time_t     forcedDisconnectSecs;
        int    autoconnect;
        char_t    externalMetadata[255];
        char_t    externalURL[255];
        char_t    externalFile[255];
        char_t    externalInterval[25];
        char_t    *vorbisComments[30];
        int     numVorbisComments;
        char_t    outputControl[255];
        char_t    metadataAppendString[255];
        char_t    metadataRemoveStringBefore[255];
        char_t    metadataRemoveStringAfter[255];
        char_t    metadataWindowClass[255];
        bool    metadataWindowClassInd;
#ifdef HAVE_FLAC
		FLAC__StreamEncoder	*flacEncoder;
		FLAC__StreamMetadata *flacMetadata;
		int	flacFailure;
#endif
		char_t	*configVariables[255];
		int		numConfigVariables;
		pthread_mutex_t mutex;

		char_t	WindowsRecDevice[255];

		int		LAMEJointStereoFlag;
		CBUFFER	circularBuffer;
/**************************************/
		OggStegoEncoder ose;
/**************************************/
} edcastGlobals;


void addConfigVariable(edcastGlobals *g, char_t *variable);
int initializeencoder(edcastGlobals *g);
void getCurrentSongTitle(edcastGlobals *g, char_t *song, char_t *artist, char_t *full);
void initializeGlobals(edcastGlobals *g);
void ReplaceString(char_t *source, char_t *dest, char_t *from, char_t *to);
void config_read(edcastGlobals *g);
void config_write(edcastGlobals *g);
int connectToServer(edcastGlobals *g);
int disconnectFromServer(edcastGlobals *g);
int do_encoding(edcastGlobals *g, short int *samples, int numsamples, int nch);
void URLize(char_t *input, char_t *output, int inputlen, int outputlen);
int updateSongTitle(edcastGlobals *g, int forceURL);
int setCurrentSongTitleURL(edcastGlobals *g, char_t *song);
void icecast2SendMetadata(edcastGlobals *g);
int ogg_encode_dataout(edcastGlobals *g);
int	trimVariable(char_t *variable);
int readConfigFile(edcastGlobals *g,int readOnly = 0);
int writeConfigFile(edcastGlobals *g);
void    printConfigFileValues();
void ErrorMessage(char_t *title, char_t *fmt, ...);
int setCurrentSongTitle(edcastGlobals *g,char_t *song);
char_t*   getSourceURL(edcastGlobals *g);
void    setSourceURL(edcastGlobals *g,char_t *url);
long    getCurrentSamplerate(edcastGlobals *g);
int     getCurrentBitrate(edcastGlobals *g);
int     getCurrentChannels(edcastGlobals *g);
int  ocConvertAudio(edcastGlobals *g,float *in_samples, float *out_samples, int num_in_samples, int num_out_samples);
int initializeResampler(edcastGlobals *g,long inSampleRate, long inNCH);
int handle_output(edcastGlobals *g, float *samples, int nsamples, int nchannels, int in_samplerate);
void setServerStatusCallback(edcastGlobals *g,void (*pCallback)(void *,void *));
void setGeneralStatusCallback(edcastGlobals *g, void (*pCallback)(void *,void *));
void setWriteBytesCallback(edcastGlobals *g, void (*pCallback)(void *,void *));
void setServerTypeCallback(edcastGlobals *g, void (*pCallback)(void *,void *));
void setServerNameCallback(edcastGlobals *g, void (*pCallback)(void *,void *));
void setStreamTypeCallback(edcastGlobals *g, void (*pCallback)(void *,void *));
void setBitrateCallback(edcastGlobals *g, void (*pCallback)(void *,void *));
void setVUCallback(edcastGlobals *g, void (*pCallback)(int, int));
void setSourceURLCallback(edcastGlobals *g, void (*pCallback)(void *,void *));
void setDestURLCallback(edcastGlobals *g, void (*pCallback)(void *,void *));
void setSourceDescription(edcastGlobals *g, char_t *desc);
int  getOggFlag(edcastGlobals *g);
bool  getLiveRecordingFlag(edcastGlobals *g);
void setLiveRecordingFlag(edcastGlobals *g, bool flag);
void setDumpData(edcastGlobals *g, int dump);
void setConfigFileName(edcastGlobals *g, char_t *configFile);
char_t *getConfigFileName(edcastGlobals *g);
char_t*	getServerDesc(edcastGlobals *g);
int	getReconnectFlag(edcastGlobals *g);
int getReconnectSecs(edcastGlobals *g);
int getIsConnected(edcastGlobals *g);
int resetResampler(edcastGlobals *g);
void setOggEncoderText(edcastGlobals *g, char_t *text);
int getLiveRecordingSetFlag(edcastGlobals *g);
char_t *getCurrentRecordingName(edcastGlobals *g);
void setCurrentRecordingName(edcastGlobals *g, char_t *name);
void setForceStop(edcastGlobals *g, int forceStop);
long	getLastXWindow(edcastGlobals *g);
long	getLastYWindow(edcastGlobals *g);
void	setLastXWindow(edcastGlobals *g, long x);
void	setLastYWindow(edcastGlobals *g, long y);
long	getLastDummyXWindow(edcastGlobals *g);
long	getLastDummyYWindow(edcastGlobals *g);
void	setLastDummyXWindow(edcastGlobals *g, long x);
void	setLastDummyYWindow(edcastGlobals *g, long y);
long	getVUShow(edcastGlobals *g);
void	setVUShow(edcastGlobals *g, long x);
int		getFrontEndType(edcastGlobals *g);
void	setFrontEndType(edcastGlobals *g, int x);
int		getReconnectTrigger(edcastGlobals *g);
void	setReconnectTrigger(edcastGlobals *g, int x);
char_t *getCurrentlyPlaying(edcastGlobals *g);
long GetConfigVariableLong(char_t *appName, char_t *paramName, long defaultvalue, char_t *desc);
char_t	*getLockedMetadata(edcastGlobals *g);
void	setLockedMetadata(edcastGlobals *g, char_t *buf);
int		getLockedMetadataFlag(edcastGlobals *g);
void	setLockedMetadataFlag(edcastGlobals *g, int flag);
void	setSaveDirectory(edcastGlobals *g, char_t *saveDir);
char_t *getSaveDirectory(edcastGlobals *g);
char_t *getgLogFile(edcastGlobals *g);
void	setgLogFile(edcastGlobals *g,char_t *logFile);
int getSaveAsWAV(edcastGlobals *g);
void setSaveAsWAV(edcastGlobals *g, int flag);
FILE *getSaveFileP(edcastGlobals *g);
long getWritten(edcastGlobals *g);
void setWritten(edcastGlobals *g, long writ);
int deleteConfigFile(edcastGlobals *g);
void	setAutoConnect(edcastGlobals *g, int flag);
void addVorbisComment(edcastGlobals *g, char_t *comment);
void freeVorbisComments(edcastGlobals *g);
void addBasicEncoderSettings(edcastGlobals *g);
void addUISettings(edcastGlobals *g);
void setDefaultLogFileName(char_t *filename);
void LogMessage(edcastGlobals *g, int type, char_t *source, int line, char_t *fmt, ...);
char_t *getWindowsRecordingDevice(edcastGlobals *g);
void	setWindowsRecordingDevice(edcastGlobals *g, char_t *device);
int getLAMEJointStereoFlag(edcastGlobals *g);
void	setLAMEJointStereoFlag(edcastGlobals *g, int flag);
int triggerDisconnect(edcastGlobals *g);
#endif
