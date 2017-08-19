////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2015, All Rights Reserved
// Author:	FU YAN
// Created:2015/12/31
// Describe:UVSocket Definition
////////////////////////////////////////////////////////////////////////
#ifndef _UVSOCKETDEF_H_
#define _UVSOCKETDEF_H_

#include <string>
#include <stdint.h>
#include <assert.h>
#include "uv.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)	if (p) { delete p; p = NULL; }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)	if (p) { p->Release(); p = NULL; }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) if (p) { delete[] p; p = NULL; }
#endif // !SAFE_DELETE_ARRAY

#define MACRO_WITH_FORCED_SEMICOLON(x) \
   __pragma(warning(push)) \
   __pragma(warning(disable:4127)) \
   do { x } while (false) \
   __pragma(warning(pop))

#ifdef _DEBUG
#define UV_ASSERT(x) assert(x)
#else
#define UV_ASSERT(x) ((void)0)
#endif

#define UV_SOCKET_BUFFER_SIZE		2048

typedef struct UVSocketBuffer_s
{
	char buf[UV_SOCKET_BUFFER_SIZE];
}UVSocketBuffer;

typedef struct UVSocketAddress_s
{
	std::string strIP;
	int nPort;
}UVSocketAddress;

typedef uint64_t SOCKET_ID;

enum SocketRunMode
{
	SOCKET_RUN_ASYNC,		//异步运行，收发包都在独立的线程，SocketCallback回调也是在独立的线程触发
	SOCKET_RUN_SYNC		//同步运行，需要自己在线程中循环调用
};

#endif	//end of define _UVSOCKETDEF_H_
