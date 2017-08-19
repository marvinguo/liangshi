////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2016, All Rights Reserved
// Author:	FU YAN
// Created:2016/01/05
// Describe:
////////////////////////////////////////////////////////////////////////
#include "TestServer.h"
#include <stdio.h>

void CTestServer::OnSocketRecv(CUVPeer* pPeer, char* pMsg, ssize_t nMsgSize)
{
	char szMsg[2048];
	memcpy(szMsg, pMsg, nMsgSize);
	szMsg[nMsgSize-1] = 0;
	printf("Recv %lld bytes From Peer[%s]:%s\n", nMsgSize, pPeer->GetIP(), szMsg);
	CUVServerSocket* pServerSocket = this->GetServerSocket();
	if (pServerSocket)
	{
		pServerSocket->Send(pPeer, pMsg, (int)nMsgSize);
	}
}

void CTestServer::OnSocketSendSucc(CUVPeer* pPeer)
{
	printf("Send to Peer[%s] succ.\n", pPeer->GetIP());
}

void CTestServer::OnSocketSendFailed(CUVPeer* pPeer)
{
	printf("Send to Peer[%s] failed.\n", pPeer->GetIP());
}

void CTestServer::OnSocketClose()
{
	printf("Socket Closed.\n");
}

void CTestServer::OnPeerConnectSucc(CUVPeer* pPeer)
{
	printf("Peer[%s] Connect Succ.\n", pPeer->GetIP());
}

void CTestServer::OnPeerClose(CUVPeer* pPeer)
{
	printf("Peer[%s] closed.\n", pPeer->GetIP());
}
