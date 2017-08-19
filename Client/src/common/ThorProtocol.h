#pragma once

#include "Qt.h"
#include "stl.h"
#include "library.h"
#include "ThorClient.h"
#include "ThorServer.h"

/*
雷神，主神奥汀长子，大地女神弗奥琴之子。
托尔身材魁梧，臂力过人，他长着一双豹眼、满脸金胡子。
托尔衣着简陋，不过他的腰上束着一条魔带，这条魔带使他的力气加倍。
托尔戴着铁手套，执着米奥尔尼尔（ Mjollner ）魔锤。
托尔常年在东方防御冰霜巨人和巨蟒的侵袭，保卫众神祇的家园。
托尔刚正不阿并敢于顶撞奥丁，托尔脾气暴躁但豪迈行侠。
众神末日来临时，雷神托尔与米德加尔德巨蟒（格拉弗维尼尔）再战，与巨蟒同归于尽。
*/

enum ThorProtocolType
{
	ThorProtocolType_Sync = 101,
	ThorProtocolType_Control = 102,
};

class ThorProtocol : public QObject
{
	Q_OBJECT
public:
	ThorProtocol(ThorClient* client, ThorServer* server, QObject *parent = 0);
	~ThorProtocol();

	//1 sync, 2 control
	int GetType(){return _type;}

	void SendServerInfo(ThorProtocolType type, QString ip, int port);
	void StartListen();
	void StopListen();

Q_SIGNALS:
	void OnGetServerInfo(ThorProtocolType type, QString ip, int port);

private Q_SLOTS:
	void OnDataReadTriggered(LokiBuffer& buffer);


private:
	string _ip;
	int _port;

private:
	int _type;
	ThorServer* _server;
	ThorClient* _client;

public:
	MSGPACK_DEFINE(_type, _ip, _port);
};

