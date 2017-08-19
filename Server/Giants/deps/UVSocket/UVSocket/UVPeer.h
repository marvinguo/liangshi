////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2016, All Rights Reserved
// Author:	FU YAN
// Created:2016/01/04
// Describe:
////////////////////////////////////////////////////////////////////////
#ifndef _UVPEER_H_
#define _UVPEER_H_

#include <string>
#include "UVLock.h"
#include "UVSocketDef.h"

class CUVServerSocket;
class CUVPeer
{
private:
	CUVPeer();
	~CUVPeer();
public:
	static CUVPeer* CreateNew() { return new CUVPeer; }
	void Release() { delete this; }
	bool Create(CUVServerSocket* pServerSocket);
	static SOCKET_ID SpawnNewID();
	uv_tcp_t* GetTcpHandle() { return &m_uvTcp; }
	const char* GetIP();
	SOCKET_ID GetSocketID() { return m_idSocket; }
private:
	SOCKET_ID m_idSocket;
	uv_tcp_t m_uvTcp;
	std::string m_strIP;

	static SOCKET_ID s_idSpawn;
	static CUVLock s_mutex;
};


#endif	//end of define _UVPEER_H_
