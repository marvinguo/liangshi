////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2016, All Rights Reserved
// Author:	FU YAN
// Created:2016/01/04
// Describe:连接传输对象
////////////////////////////////////////////////////////////////////////
#include "UVPeer.h"
#include "UVServerSocket.h"

SOCKET_ID CUVPeer::s_idSpawn = 0;

CUVLock CUVPeer::s_mutex;

CUVPeer::CUVPeer()
{
	m_idSocket = 0;
}

CUVPeer::~CUVPeer()
{

}

bool CUVPeer::Create(CUVServerSocket* pServerSocket)
{
	if (pServerSocket == NULL)
	{
		return false;
	}

	uv_loop_t* pLoop = pServerSocket->GetUVLoop();
	if (pLoop == NULL)
	{
		return false;
	}

	m_idSocket = CUVPeer::SpawnNewID();
	if (m_idSocket == 0)
	{
		return false;
	}

	if (uv_tcp_init(pLoop, &m_uvTcp) != 0)
	{
		return false;
	}

	if (uv_accept((uv_stream_t*)pServerSocket->GetTcpHandle(), (uv_stream_t*)&m_uvTcp) != 0)
	{
		return false;
	}

	return true;
}

SOCKET_ID CUVPeer::SpawnNewID()
{
	CScopedLock<CUVLock> lock(s_mutex);

	return ++s_idSpawn;
}

const char* CUVPeer::GetIP()
{
	if (m_strIP.empty())
	{
		struct sockaddr_storage addrIp;
		int nLen = sizeof(sockaddr_storage);
		uv_tcp_getpeername(&m_uvTcp, (sockaddr*)&addrIp, &nLen);
		char szIP[64]="";
		uv_ip4_name((sockaddr_in*)&addrIp, szIP, sizeof(szIP));
		m_strIP = szIP;
	}
	
	return m_strIP.c_str();
}

