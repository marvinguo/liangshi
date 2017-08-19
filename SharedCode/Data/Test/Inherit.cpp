#include "../MessagePack/include/msgpack.hpp"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

typedef int MYINT;

class IMessagable
{
public:
	virtual void GenerateMessPackData() = 0;

public:
	int _type;
	MYINT _x;
	MYINT _y;
	msgpack::sbuffer sbuf;
};

class Draw : public IMessagable
{
public:
	Draw() {_type = 1;}
	
	virtual void GenerateMessPackData()
	{
		msgpack::pack(sbuf, _x);
		msgpack::pack(sbuf, _y);
		msgpack::pack(sbuf, _points);
	}


public:
	vector<int> _points;
};

class Text : public IMessagable
{
public:
	Text() {_type = 2;}

	virtual void GenerateMessPackData()
	{
		msgpack::sbuffer sbuf;
		msgpack::pack(sbuf, _x);
		msgpack::pack(sbuf, _y);
		msgpack::pack(sbuf, _text);
	}

public:
	string _text;
};

class PpxFile
{
public:
	PpxFile()
	{
		_header="GY";
		_version = 1;
	}

	void Save()
	{
		// serializes multiple objects using msgpack::packer.
		msgpack::sbuffer buffer;
		msgpack::packer<msgpack::sbuffer> pk(&buffer);
		pk.pack(_header);
		pk.pack(_version);

		for(vector<IMessagable*>::iterator i = _contents.begin(); i != _contents.end(); i++)
		{
			IMessagable* msg = *i;
			msg->GenerateMessPackData();

			pk.pack(msg->_type);
			pk.pack_raw(msg->sbuf.size());
			pk.pack_raw_body(msg->sbuf.data(), msg->sbuf.size());
		}

		int size = buffer.size();

		Load(buffer);
	}

	void Load(msgpack::sbuffer& data)
	{
		int size = data.size();

		// deserializes these objects using msgpack::unpacker.
		msgpack::unpacker pac;

		// feeds the buffer.
		pac.reserve_buffer(data.size());
		memcpy(pac.buffer(), data.data(), data.size());
		pac.buffer_consumed(data.size());

		// now starts streaming deserialization.
		msgpack::unpacked result;
		while(pac.next(&result)) {
			std::cout << result.get() << std::endl;
		}
	}

public:
	string _header;
	int _version;
	vector<IMessagable*> _contents;
};


void Inherit()
{
	Draw* draw = new Draw();
	draw->_x = 10;
	draw->_x = 10;
	draw->_points.push_back(1);
	draw->_points.push_back(2);
	draw->_points.push_back(3);
	draw->_points.push_back(4);
	draw->_points.push_back(5);
	

	Text* text = new Text();
	text->_x = 20;
	text->_y = 20;
	text->_text = "hello Marvin";

	PpxFile file;
	file._contents.push_back(draw);
	file._contents.push_back(text);

	file.Save();
}