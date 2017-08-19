////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2016, All Rights Reserved
// Author:	FU YAN
// Created:2016/01/05
// Describe:
////////////////////////////////////////////////////////////////////////
#include "UVSocketCallBack.h"
#include "TestServer.h"
#include "vld.h"
#include "time.h"
#include "ByteStream.h"
#include <string>

void TestServerSync()
{
	CTestServer objServer;
	CUVServerSocket* pServerSocket = CUVServerSocket::CreateNew();
	if (pServerSocket == NULL || !pServerSocket->Create(&objServer))
	{
		SAFE_RELEASE(pServerSocket);
		return;
	}

	UVSocketAddress objAddr;
	objAddr.strIP = "0.0.0.0";
	objAddr.nPort = 5816;
	pServerSocket->Listen(&objAddr);
	pServerSocket->SetRunMode(SOCKET_RUN_SYNC);

	time_t tBegin = time(NULL);
	while (time(NULL) - tBegin < 10)
	{
		pServerSocket->Run();
	}

	SAFE_RELEASE(pServerSocket);
}

void TestServerAsync()
{
	CTestServer objServer;
	CUVServerSocket* pServerSocket = CUVServerSocket::CreateNew();
	if (pServerSocket == NULL || !pServerSocket->Create(&objServer))
	{
		SAFE_RELEASE(pServerSocket);
		return;
	}

	UVSocketAddress objAddr;
	objAddr.strIP = "0.0.0.0";
	objAddr.nPort = 5816;
	pServerSocket->Listen(&objAddr);
	pServerSocket->Run();
	Sleep(10000);

	SAFE_RELEASE(pServerSocket);
}

void TestByteStream()
{
	CByteStream bs;

	bool a = false;
	char szText[32] = "我爱你中国ILuvU";
	char c = 'c';
	unsigned char uc = 'd';
	short sNum = 123;
	unsigned short usNum = 777;
	std::string str = "中国是世界四大文明古国之一，有着悠久的历史，距今约5000年前，以中原地区为中心开始出现聚落组织进而成国家和朝代，后历经多次演变和朝代更迭。20世纪初辛亥革命后，君主政体退出历史舞台，共和政体建立。1949年中华人民共和国成立后，在中国大陆建立了人民代表大会制度的政体。";
	int nNum = 189459045;
	unsigned int unNum = 30678768;
	long long llNum = 234567983434;
	unsigned long long ullNum = 89456895765689;
	float x = 7.5f;
	double y = 6.666666666;
	long double z = 7.7777777777777;


	printf("%lld,%lld\n", sizeof(uint64_t), sizeof(ByteStreamHead_t));
	
	bs.SetAction(1);
	bs.Dump();
	bs << a;
	bs.Dump();
	bs << szText;
	bs.Dump();
	bs << c;
	bs.Dump();
	bs << uc;
	bs.Dump();
	bs << sNum;
	bs.Dump();
	bs << usNum;
	bs.Dump();
	bs << str;
	bs.Dump();
	bs << nNum;
	bs.Dump();
	bs << unNum;
	bs.Dump();
	bs << llNum;
	bs.Dump();
	bs << ullNum;
	bs.Dump();
	bs << x;
	bs.Dump();
	bs << y;
	bs.Dump();
	bs << z;
	bs.Dump();

	for (int i = 0; i < 10; ++i)
	{
		bs.StartWriteTable();
		bs << a << str << x << y << z;
		bs.Dump();
		bs.EndWriteTable();
	}

	bs.AddPeerID(777);

	bool a2;
	char szText2[32];
	char c2;
	unsigned char uc2;
	short sNum2;
	unsigned short usNum2;
	std::string str2;
	int nNum2;
	unsigned int unNum2;
	long long llNum2;
	unsigned long long ullNum2;
	float x2;
	double y2;
	long double z2;
	uint64_t idPeer;

	bs >> a2 >> szText2 >> c2 >> uc2 >> sNum2 >> usNum2 >> str2 >> nNum2 >> unNum2 >> llNum2 >> ullNum2 >> x2 >> y2 >> z2;

	bool bEqual = (a == a2) && (strcmp(szText, szText2) == 0) && (c== c2) && (uc == uc2) && (sNum == sNum2) && (usNum == usNum2)
		&& (str == str2) && (nNum == nNum2) && (unNum == unNum2) && (llNum == llNum2) 
		&& (ullNum == ullNum2) && (x == x2) && (y == y2) && (z == z2);
	printf("%s\n", bEqual? "Equal" : "Not Equal");

	bs.Dump();	
	printf("PeerId:%llu\n", bs.GetPeerID());
	bs.RemovePeerID();
	bs.Dump();

	CByteStream bs2;
	bs2 = bs;

	for (int i = 0; i < 10; ++i)
	{
		bs2.StartReadTable();
		bs2 >> a2 >> str2 >> x2 >> y2 >> z2;
		bs2.EndReadTable();
		bEqual = (a == a2) && (strcmp(szText, szText2) == 0) && (x == x2) && (y == y2) && (z == z2);
		printf("%s\n", bEqual ? "Equal" : "Not Equal");
	}

	bs.Clear();
	bs.Dump();

	CByteStream bs3(bs2.GetBufferPointer(), bs2.GetSize());

	bs3.Dump();
	bs2.Dump();
	bs2.Clear();
	bs2.Dump();

	
}

int main()
{
	TestByteStream();
	TestServerSync();
	TestServerAsync();

	return 0;
}