////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2016, All Rights Reserved
// Author:	FU YAN
// Created:2016/01/05
// Describe:
////////////////////////////////////////////////////////////////////////
#include "TestClient.h"
#include <stdio.h>
#include "UVClientSocket.h"

void CTestClient::OnSocketRecv(char* pMsg, ssize_t nMsgSize)
{
	char* szMsg = new char[nMsgSize + 1];
	memcpy(szMsg, pMsg, nMsgSize);
	szMsg[nMsgSize] = 0;
	printf("Recv %lld bytes:%s\n", nMsgSize, szMsg);
}

void CTestClient::OnSocketSendSucc()
{
	printf("Send Succ.\n");
}

void CTestClient::OnSocketSendFailed()
{
	printf("Send Failed.\n");
}

void CTestClient::OnSocketClose()
{
	printf("Socket Closed.\n");
}

void CTestClient::OnSocketConnectSucc()
{
	CUVClientSocket* pClientSocket = this->GetClientSocket();
	printf("Connect To [%s:%d] Succ.\n", pClientSocket->GetTargetIP(), pClientSocket->GetTargetPort());
}

void CTestClient::OnSocketConnectFailed()
{
	printf("Connect Failed.\n");
	CUVClientSocket* pClientSocket = this->GetClientSocket();
	if (pClientSocket)
	{
		pClientSocket->Connect(NULL);
	}
}
