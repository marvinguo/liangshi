#pragma once

#include "Qt.h"
#include "stl.h"
#include "../../../SharedCode/LokiBuffer.h"
#include "SocketClient.h"

/*
提尔是战神，巨人希米儿之子。传说他是契约的担保人，盟誓的临护者。
当其他的神同芬里斯怪狼开玩笑、把它捆绑起来的时候，提尔作为信用的保证人将手臂伸进狼的嘴里。
狼发现搁绑它的众神实际上是设下圈套，立即咬断提尔的手臂。从此提尔成了独臂神。
但他身佩宝剑，总显得威风凛凛。
古代按剑盟誓的习俗即起源于北欧人对战神提尔的崇拜。许多传统的剑舞，都是为纪念战神而编导的
*/

namespace Tyr
{
	enum TyrCommandType
	{
		LS = 1001,
		MV = 1002,
		RM = 1003,
		PUT = 1004,
		GET = 1005,
		Zoom = 1006,
		Pen = 1007,
	};

class TyrCommand
{
public:
	TyrCommand(TyrCommandType type):_type(type){}
	virtual ~TyrCommand(){}

	TyrCommandType GetType(){return(TyrCommandType) _type;}
	static TyrCommand* CreateFromType(TyrCommandType type, LokiBuffer& buffer);
	virtual LokiBuffer GetMsgPackData() = 0;

private:
	int _type;
};

class FileSyncInfo
{
public:
	string _filename;
	int _size;
	bool _isDir;

	operator QVariant() const
	{
		return QVariant::fromValue(*this);
	}
public:
	MSGPACK_DEFINE(_filename, _size, _isDir);
};
Q_DECLARE_METATYPE(FileSyncInfo);

class LSCommand : public TyrCommand
{
public:
	LSCommand():TyrCommand(LS){}
	~LSCommand(){}

	void ls(QString path);

	void SetPath(QString path){_path = path.toStdString();}
	QString GetPath(){return QString(_path.c_str());}

	void SetFiles(vector<FileSyncInfo>& files){_files = files;}
	vector<FileSyncInfo> GetFiles(){return _files;}

	virtual LokiBuffer GetMsgPackData();

private:
	string _path;
	vector<FileSyncInfo> _files;

public:
	MSGPACK_DEFINE(_path, _files);
};

class MVCommand : public TyrCommand
{
public:
	MVCommand():TyrCommand(MV){}
	~MVCommand(){}

	void cd(QString path);
	virtual LokiBuffer GetMsgPackData();

private:
	string _path;

public:
	MSGPACK_DEFINE(_path);
};


class RMCommand : public TyrCommand
{
public:
	RMCommand():TyrCommand(RM){}
	~RMCommand(){}

	void cd(QString path);
	virtual LokiBuffer GetMsgPackData();

private:
	string _path;

public:
	MSGPACK_DEFINE(_path);
};

class PUTCommand : public TyrCommand
{
public:
	PUTCommand():TyrCommand(PUT){}
	~PUTCommand(){}

	void put(QString path, LokiBuffer& buffer);
	QString GetPath(){return QString(_path.c_str());}
	LokiBuffer GetContent(){return _content;}

	virtual LokiBuffer GetMsgPackData();

private:
	string _path;
	LokiBuffer _content;

public:
	MSGPACK_DEFINE(_path, _content);
};

class GETCommand : public TyrCommand
{
public:
	GETCommand():TyrCommand(GET){}
	~GETCommand(){}

	void cd(QString path);
	virtual LokiBuffer GetMsgPackData();

private:
	string _path;

public:
	MSGPACK_DEFINE(_path);
};

class ZoomCommand : public TyrCommand
{
public:
	ZoomCommand():TyrCommand(Zoom){}
	~ZoomCommand(){}

	void ls(QString path);

	void SetPath(QString path){_path = path.toStdString();}
	QString GetPath(){return QString(_path.c_str());}

	virtual LokiBuffer GetMsgPackData();

private:
	string _path;

public:
	MSGPACK_DEFINE(_path);
};

class PenCommand : public TyrCommand
{
public:
	PenCommand():TyrCommand(Pen){}
	~PenCommand(){}

	void cd(QString path);
	virtual LokiBuffer GetMsgPackData();

private:
	string _path;

public:
	MSGPACK_DEFINE(_path);
};

}
