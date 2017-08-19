////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2016, All Rights Reserved
// Author:	FU YAN
// Created:2016/01/05
// Describe:
////////////////////////////////////////////////////////////////////////
#include "UVClientSocket.h"
#include "TestClient.h"
#include "time.h"
#include "vld.h"


void TestClientSync()
{
	CUVClientSocket* pClientSocket = CUVClientSocket::CreateNew();
	CTestClient objTestClient;
	if (pClientSocket == NULL || !pClientSocket->Create(&objTestClient))
	{
		SAFE_RELEASE(pClientSocket);
		return;
	}

	UVSocketAddress objAddr;
	objAddr.strIP = "192.168.223.86";
	objAddr.nPort = 5816;

	pClientSocket->Connect(&objAddr);
	pClientSocket->SetRunMode(SOCKET_RUN_SYNC);


	char szMsg[2048];
	time_t tBegin = time(NULL);
	srand(tBegin);
	while (time(NULL) - tBegin < 30)
	{
		pClientSocket->Run();
		int nLen = rand() % 2048;
		for (int i = 0; i < nLen; ++i)
		{
			szMsg[i] = rand() % 127 + 1;
		}
		szMsg[nLen] = 0;
		pClientSocket->Send(szMsg, strlen(szMsg));
		Sleep(10);
	}

	SAFE_RELEASE(pClientSocket);
}

void TestClientAsync()
{
	CUVClientSocket* pClientSocket = CUVClientSocket::CreateNew();
	CTestClient objTestClient;
	if (pClientSocket == NULL || !pClientSocket->Create(&objTestClient))
	{
		SAFE_RELEASE(pClientSocket);
		return;
	}

	UVSocketAddress objAddr;
	objAddr.strIP = "192.168.223.86";
	objAddr.nPort = 5816;

	pClientSocket->Connect(&objAddr);
	pClientSocket->SetRunMode(SOCKET_RUN_SYNC);
	pClientSocket->Run();

	char szMsg[2048];
	time_t tBegin = time(NULL);
	srand(tBegin);
	while (time(NULL) - tBegin < 30)
	{
		int nLen = rand() % 2048;
		for (int i = 0; i < nLen; ++i)
		{
			szMsg[i] = rand() % 127 + 1;
		}
		szMsg[nLen] = 0;
		pClientSocket->Send(szMsg, strlen(szMsg));
		Sleep(10);
	}

	SAFE_RELEASE(pClientSocket);
}


int main()
{
	TestClientSync();
	TestClientAsync();

	return 0;
}