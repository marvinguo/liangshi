////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2016, All Rights Reserved
// Author:	FU YAN
// Created:2016/01/05
// Describe:
////////////////////////////////////////////////////////////////////////
#ifndef _TESTSERVER_H_
#define _TESTSERVER_H_

#include "UVServerSocket.h"

class CTestServer : public IUVServerCallback
{
public:
	//接收报文，需要自行处理分包粘包
	virtual void OnSocketRecv(CUVPeer* pPeer, char* pMsg, ssize_t nMsgSize);
	// 发送报文成功
	virtual void OnSocketSendSucc(CUVPeer* pPeer);
	//发送报文失败
	virtual void OnSocketSendFailed(CUVPeer* pPeer);
	//本机Socket关闭
	virtual void OnSocketClose();
	//对端连接成功
	virtual void OnPeerConnectSucc(CUVPeer* pPeer);
	//对端连接断开
	virtual void OnPeerClose(CUVPeer* pPeer);
};

#endif	//end of define _TESTSERVER_H_