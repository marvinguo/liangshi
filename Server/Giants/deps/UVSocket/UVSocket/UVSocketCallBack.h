////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2016, All Rights Reserved
// Author:	FU YAN
// Created:2016/01/05
// Describe:UVSocket回调接口类，使用时实现对应的Server/client接口
////////////////////////////////////////////////////////////////////////
#ifndef _UVSOCKETCALLBACK_H_
#define _UVSOCKETCALLBACK_H_

#include "UVSocketDef.h"

class CUVPeer;
class CUVServerSocket;
class CUVClientSocket;

class IUVServerCallback
{
public:
	//接收报文，需要自行处理分包粘包
	virtual void OnSocketRecv(CUVPeer* pPeer, char* pMsg, ssize_t nMsgSize) = 0;
	// 发送报文成功
	virtual void OnSocketSendSucc(CUVPeer* pPeer) = 0;
	//发送报文失败
	virtual void OnSocketSendFailed(CUVPeer* pPeer) = 0;
	//本机Socket关闭
	virtual void OnSocketClose() = 0;
	//对端连接成功
	virtual void OnPeerConnectSucc(CUVPeer* pPeer) = 0;
	//对端连接断开
	virtual void OnPeerClose(CUVPeer* pPeer) = 0;

	CUVServerSocket* GetServerSocket() { return m_pServerSocket; }

	void SetServerSocket(CUVServerSocket* pServerSocket) { m_pServerSocket = pServerSocket; }
private:
	CUVServerSocket* m_pServerSocket;
};

class IUVClientCallback
{
public:
	//接收报文，需要自行处理分包粘包
	virtual void OnSocketRecv(char* pMsg, ssize_t nMsgSize) = 0;
	// 发送报文成功
	virtual void OnSocketSendSucc() = 0;
	//发送报文失败
	virtual void OnSocketSendFailed() = 0;
	//本机Socket关闭
	virtual void OnSocketClose() = 0;
	//连上服务器成功
	virtual void OnSocketConnectSucc() = 0;
	//连接服务器失败
	virtual void OnSocketConnectFailed() = 0;

	CUVClientSocket* GetClientSocket() { return m_pClientSocket; }

	void SetClientSocket(CUVClientSocket* pClientSocket) { m_pClientSocket = pClientSocket; }
private:
	CUVClientSocket* m_pClientSocket;
};

#endif	//end of define _UVSOCKETCALLBACK_H_

