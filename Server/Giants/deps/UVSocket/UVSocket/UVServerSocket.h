////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2015, All Rights Reserved
// Author:	FU YAN
// Created:2015/12/31
// Describe:Interface of uvsocket
////////////////////////////////////////////////////////////////////////
#ifndef _UVSERVERSOCKET_H_
#define _UVSERVERSOCKET_H_

#include <map>
#include "UVSocketDef.h"
#include "ObjectPool.h"
#include "UVPeer.h"
#include "UVSocketCallBack.h"

#define UV_SERVER_SOCKET_POOL_SIZE		256

class CUVServerSocket
{
private:
	CUVServerSocket();
	~CUVServerSocket();
	void Close();
	void CleanUp();
public://Common
	static CUVServerSocket* CreateNew() { return new CUVServerSocket; }
	void Release() { delete this; }
	bool Create(IUVServerCallback* pSocketCallBack);
	
	uv_loop_t* GetUVLoop();
	IUVServerCallback* GetSocketCallback();
	uv_tcp_t* GetTcpHandle();
	bool IsClosed() { return m_bClosed; }
public://Server
	bool Listen(UVSocketAddress* pAddr);
	bool SetMaxIncomingSocketNum(int nMaxSockets);
	int GetMaxIncomingSocketNum();
	void CloseConnection(CUVPeer* pPeer);
	void CloseAllConnections();
	bool Send(CUVPeer* pPeer, char* pMsg, int nMsgSize);
	bool Run();
	void SetRunMode(SocketRunMode nMode);
	SocketRunMode GetRunMode() { return m_nMode; }
private://PeerSet
	bool AddPeer(CUVPeer* pPeer);
	void DelPeer(uv_tcp_t* pPeerTcp);
	CUVPeer* QueryPeer(uv_tcp_t* pPeerTcp);
	size_t GetPeerSize();

public://BufferPool
	UVSocketBuffer* BorrowBuffer();
	void ReturnBuffer(UVSocketBuffer* pBuffer);

public://ProcessSocketCallbacks
	void ProcessSocketConnectSucc(CUVPeer* pPeer);
	void ProcessSocketRecv(CUVPeer* pPeer, char* pMsg, ssize_t nMsgSize);
	void ProcessSocketSendSucc(CUVPeer* pPeer);
	void ProcessSocketSendFailed(CUVPeer* pPeer);
	void ProcessPeerClose(CUVPeer* pPeer);
	void ProcessSocketClose();

public://static callbacks
	static void OnNewConnection(uv_stream_t* server, int status);
	static void OnCloseConnection(uv_handle_t* handle);
	static void OnAlloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
	static void OnCloseServer(uv_handle_t* handle);
	static void OnRecv(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
	static void OnSend(uv_write_t* req, int status);
	static void OnRunThread(void* arg);

private:
	uv_loop_t m_uvLoop;
	uv_tcp_t m_uvTcp;
	ObjectPool<UVSocketBuffer> m_ObjPool;
	int m_nMaxIncomingSocketNum;
	typedef std::map<uv_tcp_t*, CUVPeer*> PEER_SET;
	PEER_SET m_mapPeers;
	IUVServerCallback* m_pSocketCallBack;
	uv_thread_t m_uvThread;
	bool m_bClosed;
	SocketRunMode m_nMode;
};

#endif	//end of define _UVSERVERSOCKET_H_
