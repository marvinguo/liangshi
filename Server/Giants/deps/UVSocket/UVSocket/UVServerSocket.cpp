////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2015, All Rights Reserved
// Author:	FU YAN
// Created:2015/12/31
// Describe:
////////////////////////////////////////////////////////////////////////
#include "UVServerSocket.h"

CUVServerSocket::CUVServerSocket():m_ObjPool(UV_SERVER_SOCKET_POOL_SIZE, UV_SERVER_SOCKET_POOL_SIZE)
{
	m_nMaxIncomingSocketNum = 0;
	uv_loop_init(&m_uvLoop);
	m_uvLoop.data = this;
	m_pSocketCallBack = NULL;
	m_bClosed = false;
	m_nMode = SOCKET_RUN_ASYNC;
}

CUVServerSocket::~CUVServerSocket()
{
	this->Close();
	if (m_nMode == SOCKET_RUN_ASYNC)
	{
		uv_thread_join(&m_uvThread);
	}
	else
	{
		this->CleanUp();
	}
}

bool CUVServerSocket::Create(IUVServerCallback* pSocketCallBack)
{
	if (pSocketCallBack == NULL)
	{
		return false;
	}

	m_pSocketCallBack = pSocketCallBack;

	pSocketCallBack->SetServerSocket(this);

	return true;
}

void CUVServerSocket::Close()
{
	uv_close((uv_handle_t*)&m_uvTcp, CUVServerSocket::OnCloseServer);
	m_bClosed = true;
}

void CUVServerSocket::CleanUp()
{
	uv_run(&m_uvLoop, UV_RUN_DEFAULT);
	uv_loop_close(&m_uvLoop);
}

uv_loop_t* CUVServerSocket::GetUVLoop()
{
	return &m_uvLoop;
}

bool CUVServerSocket::Listen(UVSocketAddress* pAddr)
{
	if (pAddr == NULL)
	{
		return false;
	}

	if (pAddr->strIP.empty() || pAddr->nPort == 0)
	{
		return false;
	}

	if (uv_tcp_init(&m_uvLoop, &m_uvTcp) != 0)
	{
		return false;
	}

	m_uvTcp.data = this;
	struct sockaddr_in addr;
	if (uv_ip4_addr(pAddr->strIP.c_str(), pAddr->nPort, &addr) != 0)
	{
		return false;
	}
	
	if (uv_tcp_bind(&m_uvTcp, (const struct sockaddr*) &addr, 0) != 0)
	{
		return false;
	}

	if (uv_listen((uv_stream_t*)&m_uvTcp, SOMAXCONN, CUVServerSocket::OnNewConnection) != 0)
	{
		return false;
	}

	return true;
}

bool CUVServerSocket::SetMaxIncomingSocketNum(int nMaxSockets)
{
	if (nMaxSockets < 0)
	{
		return false;
	}

	m_nMaxIncomingSocketNum = nMaxSockets;

	return true;
}


int CUVServerSocket::GetMaxIncomingSocketNum()
{
	return m_nMaxIncomingSocketNum;
}

void CUVServerSocket::CloseConnection(CUVPeer* pPeer)
{
	if (pPeer == NULL)
	{
		return;
	}

	uv_close((uv_handle_t*)pPeer->GetTcpHandle(), CUVServerSocket::OnCloseConnection);
}

void CUVServerSocket::CloseAllConnections()
{
	for (PEER_SET::iterator it = m_mapPeers.begin(); it != m_mapPeers.end(); ++it)
	{
		CUVPeer* pPeer = it->second;
		if (pPeer == NULL)
		{
			continue;
		}

		uv_close((uv_handle_t*)pPeer->GetTcpHandle(), CUVServerSocket::OnCloseConnection);
	}
}

bool CUVServerSocket::Run()
{
	if (m_nMode == SOCKET_RUN_ASYNC)
	{
		if (uv_thread_create(&m_uvThread, CUVServerSocket::OnRunThread, this) != 0)
		{
			return false;
		}
	}
	else//SOCKET_RUN_SYNC
	{
		if (!this->IsClosed())
		{
			uv_run(&m_uvLoop, UV_RUN_NOWAIT);
		}
	}

	return true;
}

void CUVServerSocket::SetRunMode(SocketRunMode nMode)
{
	m_nMode = nMode;
}

IUVServerCallback* CUVServerSocket::GetSocketCallback()
{
	return m_pSocketCallBack;
}

uv_tcp_t* CUVServerSocket::GetTcpHandle()
{
	return &m_uvTcp;
}

bool CUVServerSocket::Send(CUVPeer* pPeer, char* pMsg, int nMsgSize)
{
	if (pPeer == NULL || pMsg == NULL || nMsgSize == 0)
	{
		return false;
	}

	uv_tcp_t* pPeerTcp = pPeer->GetTcpHandle();

	uv_write_t* pWriteReq = new uv_write_t;
	uv_buf_t buf = uv_buf_init(pMsg, nMsgSize);
	pWriteReq->data = this;

	if (uv_write(pWriteReq, (uv_stream_t*)pPeerTcp, &buf, 1, CUVServerSocket::OnSend) != 0)
	{
		SAFE_DELETE(pWriteReq);
		return false;
	}

	return true;
}

bool CUVServerSocket::AddPeer(CUVPeer* pPeer)
{
	if (pPeer == NULL)
	{
		return false;
	}

	m_mapPeers.insert(std::make_pair(pPeer->GetTcpHandle(), pPeer));

	return true;
}

void CUVServerSocket::DelPeer(uv_tcp_t* pPeerTcp)
{
	CUVPeer* pPeer = this->QueryPeer(pPeerTcp);
	if (pPeer == NULL)
	{
		return;
	}

	m_mapPeers.erase(pPeerTcp);

	SAFE_RELEASE(pPeer);
}

CUVPeer* CUVServerSocket::QueryPeer(uv_tcp_t* pPeerTcp)
{
	if (pPeerTcp == NULL)
	{
		return NULL;
	}

	PEER_SET::iterator it = m_mapPeers.find(pPeerTcp);
	if (it == m_mapPeers.end())
	{
		return NULL;
	}

	return it->second;
}

size_t CUVServerSocket::GetPeerSize()
{
	return m_mapPeers.size();
}

UVSocketBuffer* CUVServerSocket::BorrowBuffer()
{
	return m_ObjPool.borrowObject();
}

void CUVServerSocket::ReturnBuffer(UVSocketBuffer* pBuffer)
{
	m_ObjPool.returnObject(pBuffer);
}

void CUVServerSocket::ProcessSocketConnectSucc(CUVPeer* pPeer)
{
	IUVServerCallback* pServerCallback = this->GetSocketCallback();
	if (pServerCallback)
	{
		pServerCallback->OnPeerConnectSucc(pPeer);
	}
}

void CUVServerSocket::ProcessSocketRecv(CUVPeer* pPeer, char* pMsg, ssize_t nMsgSize)
{
	IUVServerCallback* pServerCallback = this->GetSocketCallback();
	if (pServerCallback)
	{
		pServerCallback->OnSocketRecv(pPeer, pMsg, nMsgSize);
	}
}

void CUVServerSocket::ProcessSocketSendSucc(CUVPeer* pPeer)
{
	IUVServerCallback* pServerCallback = this->GetSocketCallback();
	if (pServerCallback)
	{
		pServerCallback->OnSocketSendSucc(pPeer);
	}
}

void CUVServerSocket::ProcessSocketSendFailed(CUVPeer* pPeer)
{
	IUVServerCallback* pServerCallback = this->GetSocketCallback();
	if (pServerCallback)
	{
		pServerCallback->OnSocketSendFailed(pPeer);
	}
}

void CUVServerSocket::ProcessPeerClose(CUVPeer* pPeer)
{
	IUVServerCallback* pServerCallback = this->GetSocketCallback();
	if (pServerCallback)
	{
		pServerCallback->OnPeerClose(pPeer);
	}
}

void CUVServerSocket::ProcessSocketClose()
{
	IUVServerCallback* pServerCallback = this->GetSocketCallback();
	if (pServerCallback)
	{
		pServerCallback->OnSocketClose();
	}
}

void CUVServerSocket::OnNewConnection(uv_stream_t* server, int status)
{
	if (server == NULL)
	{
		return;
	}

	if (status != 0)
	{
		return;
	}

	CUVServerSocket* pServerSocket = (CUVServerSocket*)server->data;
	if (pServerSocket == NULL)
	{
		return;
	}

	//连接数量控制，超过上限不处理连接请求
	if (pServerSocket->GetMaxIncomingSocketNum() > 0 && pServerSocket->GetPeerSize() >= pServerSocket->GetMaxIncomingSocketNum())
	{
		return;
	}

	CUVPeer* pPeer = CUVPeer::CreateNew();
	if (pPeer == NULL || !pPeer->Create(pServerSocket))
	{
		SAFE_RELEASE(pPeer);
		return;
	}

	uv_tcp_t* pPeerTcp = pPeer->GetTcpHandle();
	if (pPeerTcp == NULL)
	{
		return;
	}

	pPeerTcp->data = pServerSocket;
	pServerSocket->AddPeer(pPeer);

	uv_read_start((uv_stream_t*)pPeerTcp, CUVServerSocket::OnAlloc, CUVServerSocket::OnRecv);

	pServerSocket->ProcessSocketConnectSucc(pPeer);
}

void CUVServerSocket::OnCloseConnection(uv_handle_t* handle)
{
	if (handle == NULL)
	{
		return;
	}

	CUVServerSocket* pServerSocket = (CUVServerSocket*)handle->data;
	if (pServerSocket == NULL)
	{
		return;
	}

	CUVPeer* pPeer = pServerSocket->QueryPeer((uv_tcp_t*)handle);

	pServerSocket->ProcessPeerClose(pPeer);

	pServerSocket->DelPeer((uv_tcp_t*)handle);
}

void CUVServerSocket::OnAlloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	if (buf == NULL)
	{
		return;
	}
	CUVServerSocket* pServerSocket = (CUVServerSocket*)handle->data;
	if (pServerSocket == NULL)
	{
		return;
	}

	UVSocketBuffer* pBuff = pServerSocket->BorrowBuffer();
	if (pBuff == NULL)
	{
		return;
	}

	buf->base = pBuff->buf;
	buf->len = UV_SOCKET_BUFFER_SIZE;
}

void CUVServerSocket::OnCloseServer(uv_handle_t* handle)
{
	if (handle == NULL)
	{
		return;
	}

	CUVServerSocket* pServerSocket = (CUVServerSocket*)handle->data;
	if (pServerSocket == NULL)
	{
		return;
	}

	pServerSocket->ProcessSocketClose();
	pServerSocket->CloseAllConnections();
}

void CUVServerSocket::OnRecv(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	if (stream == NULL || buf == NULL)
	{
		return;
	}

	CUVServerSocket* pServerSocket = (CUVServerSocket*)stream->data;
	if (pServerSocket == NULL)
	{
		pServerSocket->ReturnBuffer((UVSocketBuffer*)buf->base);
		return;
	}

	if (nread <= 0 || nread > buf->len)
	{
		pServerSocket->ReturnBuffer((UVSocketBuffer*)buf->base);
		return;
	}

	CUVPeer* pPeer = pServerSocket->QueryPeer((uv_tcp_t*)stream);

	pServerSocket->ProcessSocketRecv(pPeer, buf->base, nread);

	pServerSocket->ReturnBuffer((UVSocketBuffer*)buf->base);
}

void CUVServerSocket::OnSend(uv_write_t* req, int status)
{
	if (req == NULL)
	{
		return;
	}

	CUVServerSocket* pServerSocket = (CUVServerSocket*)req->data;
	if (pServerSocket == NULL)
	{
		SAFE_DELETE(req);
		return;
	}

	CUVPeer* pPeer = pServerSocket->QueryPeer((uv_tcp_t*)req->handle);

	if (status == 0)
	{
		pServerSocket->ProcessSocketSendSucc(pPeer);
	}
	else
	{
		pServerSocket->ProcessSocketSendFailed(pPeer);
	}

	SAFE_DELETE(req);
}

void CUVServerSocket::OnRunThread(void* arg)
{
	CUVServerSocket* pServerSocket = (CUVServerSocket*)arg;
	if (pServerSocket == NULL)
	{
		return;
	}

	uv_loop_t* pLoop = pServerSocket->GetUVLoop();
	while (!pServerSocket->IsClosed())
	{
		uv_run(pLoop, UV_RUN_NOWAIT);
	}

	pServerSocket->CleanUp();
}
