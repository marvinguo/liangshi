////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2016, All Rights Reserved
// Author:	FU YAN
// Created:2016/01/05
// Describe:Client socket base on libuv
////////////////////////////////////////////////////////////////////////
#ifndef _UVCLIENTSOCKET_H_
#define _UVCLIENTSOCKET_H_

#include "UVSocketDef.h"
#include "UVSocketCallBack.h"

class CUVClientSocket
{
private:
	CUVClientSocket();
	~CUVClientSocket();
	void Close();
	void CleanUp();
public://Common
	static CUVClientSocket* CreateNew() { return new CUVClientSocket; }
	void Release() { delete this; }
	bool Create(IUVClientCallback* pSocketCallBack);
	
	uv_loop_t* GetUVLoop();
	IUVClientCallback* GetSocketCallback();
	uv_tcp_t* GetTcpHandle();
	const char* GetTargetIP() { return m_stAddr.strIP.c_str(); }
	int GetTargetPort() { return m_stAddr.nPort; }
	bool IsClosed() { return m_bClosed; }
	void SetRunMode(SocketRunMode nMode) { m_nMode = nMode; }
	SocketRunMode GetRunMode() { return m_nMode; }
public://client
	bool Connect(UVSocketAddress* pAddr);
	bool Run();
	bool Send(char* pMsg, int nMsgSize);

public://ProcessSocketCallbacks
	void ProcessSocketConnectSucc();
	void ProcessSocketConnectFailed();
	void ProcessSocketRecv(char* pMsg, ssize_t nMsgSize);
	void ProcessSocketSendSucc();
	void ProcessSocketSendFailed();
	void ProcessSocketClose();

public://static callbacks
	static void OnConnect(uv_connect_t* req, int status);
	static void OnClose(uv_handle_t* handle);
	static void OnAlloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
	static void OnRecv(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void OnSend(uv_write_t* req, int status);
	static void OnRunThread(void* arg);

private:
	uv_loop_t m_uvLoop;
	uv_tcp_t m_uvTcp;
	IUVClientCallback* m_pSocketCallBack;
	UVSocketAddress m_stAddr;
	uv_connect_t m_uvConReq;
	uv_thread_t m_uvThread;
	bool m_bClosed;
	SocketRunMode m_nMode;
};

#endif	//end of define _UVCLIENTSOCKET_H_
