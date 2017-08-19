////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2016, All Rights Reserved
// Author:	FU YAN
// Created:2016/01/05
// Describe:
////////////////////////////////////////////////////////////////////////
#include "UVClientSocket.h"
#include "UVPeer.h"

CUVClientSocket::CUVClientSocket()
{
	uv_loop_init(&m_uvLoop);
	m_uvLoop.data = this;
	m_pSocketCallBack = NULL;
	uv_tcp_init(&m_uvLoop, &m_uvTcp);
	m_bClosed = false;
	m_nMode = SOCKET_RUN_ASYNC;
}

CUVClientSocket::~CUVClientSocket()
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

bool CUVClientSocket::Create(IUVClientCallback* pSocketCallBack)
{
	if (pSocketCallBack == NULL)
	{
		return false;
	}

	m_uvTcp.data = this;

	m_pSocketCallBack = pSocketCallBack;

	pSocketCallBack->SetClientSocket(this);

	return true;
}

void CUVClientSocket::Close()
{
	uv_close((uv_handle_t*)&m_uvTcp, CUVClientSocket::OnClose);
	m_bClosed = true;
}

void CUVClientSocket::CleanUp()
{
	//清理资源
	uv_run(&m_uvLoop, UV_RUN_DEFAULT);
	uv_loop_close(&m_uvLoop);
}

uv_loop_t* CUVClientSocket::GetUVLoop()
{
	return &m_uvLoop;
}

IUVClientCallback* CUVClientSocket::GetSocketCallback()
{
	return m_pSocketCallBack;
}

uv_tcp_t* CUVClientSocket::GetTcpHandle()
{
	return &m_uvTcp;
}

bool CUVClientSocket::Connect(UVSocketAddress* pAddr)
{
	if (pAddr != NULL)
	{
		m_stAddr = *pAddr;		
	}

	if (m_stAddr.nPort == 0 || m_stAddr.strIP.empty())
	{
		return false;
	}

	struct sockaddr_in stConAddr;
	if (uv_ip4_addr(m_stAddr.strIP.c_str(), m_stAddr.nPort, &stConAddr) != 0)
	{
		return false;
	}

	m_uvConReq.data = this;

	if (uv_tcp_connect(&m_uvConReq, &m_uvTcp, (const sockaddr*)&stConAddr, CUVClientSocket::OnConnect) != 0)
	{
		return false;
	}

	return true;
}

bool CUVClientSocket::Run()
{
	if (m_nMode == SOCKET_RUN_ASYNC)
	{
		if (uv_thread_create(&m_uvThread, CUVClientSocket::OnRunThread, this) != 0)
		{
			return false;
		}
	}
	else
	{
		if (!this->IsClosed())
		{
			uv_run(&m_uvLoop, UV_RUN_NOWAIT);
		}
		
	}

	return true;
}

bool CUVClientSocket::Send(char* pMsg, int nMsgSize)
{
	if (pMsg == NULL)
	{
		return false;
	}

	uv_write_t* pWriteReq = new uv_write_t;
	uv_buf_t buf = uv_buf_init(pMsg, nMsgSize);
	pWriteReq->data = this;

	if (uv_write(pWriteReq, (uv_stream_t*)&m_uvTcp, &buf, 1, CUVClientSocket::OnSend) != 0)
	{
		SAFE_DELETE(pWriteReq);
		return false;
	}

	return true;
}

void CUVClientSocket::ProcessSocketConnectSucc()
{
	IUVClientCallback* pClientCallback = this->GetSocketCallback();
	if (pClientCallback)
	{
		pClientCallback->OnSocketConnectSucc();
	}
}

void CUVClientSocket::ProcessSocketConnectFailed()
{
	IUVClientCallback* pClientCallback = this->GetSocketCallback();
	if (pClientCallback)
	{
		pClientCallback->OnSocketConnectFailed();
	}
}

void CUVClientSocket::ProcessSocketRecv(char* pMsg, ssize_t nMsgSize)
{
	IUVClientCallback* pClientCallback = this->GetSocketCallback();
	if (pClientCallback)
	{
		pClientCallback->OnSocketRecv(pMsg, nMsgSize);
	}
}

void CUVClientSocket::ProcessSocketSendSucc()
{
	IUVClientCallback* pClientCallback = this->GetSocketCallback();
	if (pClientCallback)
	{
		pClientCallback->OnSocketSendSucc();
	}
}

void CUVClientSocket::ProcessSocketSendFailed()
{
	IUVClientCallback* pClientCallback = this->GetSocketCallback();
	if (pClientCallback)
	{
		pClientCallback->OnSocketSendFailed();
	}
}

void CUVClientSocket::ProcessSocketClose()
{
	IUVClientCallback* pClientCallback = this->GetSocketCallback();
	if (pClientCallback)
	{
		pClientCallback->OnSocketClose();
	}
}

void CUVClientSocket::OnConnect(uv_connect_t* req, int status)
{
	if (req == NULL)
	{
		return;
	}

	CUVClientSocket* pClientSocket = (CUVClientSocket*)req->data;
	if (pClientSocket == NULL)
	{
		return;
	}

	if (status == 0)
	{
		pClientSocket->ProcessSocketConnectSucc();
		uv_read_start(req->handle, CUVClientSocket::OnAlloc, CUVClientSocket::OnRecv);
	}
	else
	{
		pClientSocket->ProcessSocketConnectFailed();
	}

}

void CUVClientSocket::OnClose(uv_handle_t* handle)
{
	if (handle == NULL)
	{
		return;
	}

	CUVClientSocket* pClientSocket = (CUVClientSocket*)handle->data;
	if (pClientSocket)
	{
		pClientSocket->ProcessSocketClose();
	}
}

void CUVClientSocket::OnAlloc(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	if (handle == NULL || buf == NULL)
	{
		return;
	}

	buf->base = new char[suggested_size];
	buf->len = (ULONG)suggested_size;
}

void CUVClientSocket::OnRecv(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
	if (stream == NULL || buf == NULL)
	{
		return;
	}
	char* pMsg = buf->base;

	CUVClientSocket* pClientSocket = (CUVClientSocket*)stream->data;
	if (pClientSocket == NULL)
	{
		SAFE_DELETE_ARRAY(pMsg);
		return;
	}

	if (nread <= 0 || nread > buf->len)
	{
		SAFE_DELETE_ARRAY(pMsg);
		return;
	}

	pClientSocket->ProcessSocketRecv(buf->base, nread);

	SAFE_DELETE_ARRAY(pMsg);
}

void CUVClientSocket::OnSend(uv_write_t* req, int status)
{
	if (req == NULL)
	{
		return;
	}

	CUVClientSocket* pClientSocket = (CUVClientSocket*)req->data;
	if (pClientSocket)
	{
		if (status == 0)
		{
			pClientSocket->ProcessSocketSendSucc();
		}
		else
		{
			pClientSocket->ProcessSocketSendFailed();
		}
	}

	SAFE_DELETE(req);
}

void CUVClientSocket::OnRunThread(void* arg)
{
	CUVClientSocket* pClientSocket = (CUVClientSocket*)arg;
	if (pClientSocket == NULL)
	{
		return;
	}

	uv_loop_t* pLoop = pClientSocket->GetUVLoop();

	while (!pClientSocket->IsClosed())
	{
		uv_run(pLoop, UV_RUN_NOWAIT);
	}

	pClientSocket->CleanUp();
}
