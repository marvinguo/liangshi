////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2016, All Rights Reserved
// Author:	FU YAN
// Created:2016/01/05
// Describe:
////////////////////////////////////////////////////////////////////////
#include "UVSocketCallBack.h"

class CTestClient : public IUVClientCallback
{
public:
	//接收报文，需要自行处理分包粘包
	virtual void OnSocketRecv(char* pMsg, ssize_t nMsgSize);
	// 发送报文成功
	virtual void OnSocketSendSucc();
	//发送报文失败
	virtual void OnSocketSendFailed();
	//本机Socket关闭
	virtual void OnSocketClose();
	//连上服务器成功
	virtual void OnSocketConnectSucc();
	//连接服务器失败
	virtual void OnSocketConnectFailed();
};