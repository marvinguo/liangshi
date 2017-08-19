#pragma once
#include <string>
#include <vector>
using namespace std;
#include "sigslot_liangshi.h"

#if defined(WEBRTC_WIN) || defined(Q_OS_WIN32) || _WIN32
#define LIANGSHIEXOPRT _declspec(dllexport)
#else
#define LIANGSHIEXOPRT __attribute__ ((visibility("default")))
#endif

struct AVCodecContext;
struct AVFrame;

class LIANGSHIEXOPRT Ares
{
public:
	Ares();
	Ares(const Ares&);
	Ares& operator=(const Ares&);
	virtual ~Ares();

public:
	void Init(char* UTF8_filename, int width, int height);
	void Release();
	void EncodeFrame(char* RGBbuffer);

private:
	FILE* _file;
	AVCodecContext* _context;
	AVFrame* _RGBFrame;
	AVFrame* _YUVFrame;
	char* _YUVbuffer;
	char* _outBuffer;
};
;