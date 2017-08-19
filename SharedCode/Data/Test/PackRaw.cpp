#include "../MessagePack/include/msgpack.hpp"
#include <vector>
#include <string>
#include <iostream>

class RawData
{
public:
	RawData()
	{

	}

	RawData(RawData&data)
	{
		this->_x1 = data._x1;
		this->_x2 = data._x2;
		this->_x3 = data._x3;
		this->_x4 = data._x4;
		this->_x5 = data._x5;
		this->_x6 = data._x6;
		this->_x7 = data._x7;
	}

public:
	void Generate()
	{	
		msgpack::packer<msgpack::sbuffer> pk(&_sb);
		//pk.pack(std::string("Hello Data, can you see this?"));
		_x1 = 1;
		_x2 = 2;
		_x3 = 3;
		_x4 = 4;
		_x5 = 5;
		_x6 = 6;
		_x7 = 7;

		pk.pack(*this);
	}

public:	
	msgpack::sbuffer _sb;
	int _x1;
	int _x2;
	int _x3;
	int _x4;
	int _x5;
	int _x6;
	int _x7;

	MSGPACK_DEFINE(_x1, _x2, _x3, _x4, _x5, _x6, _x7);
};

void PackRaw()
{
	msgpack::sbuffer buffer;
	msgpack::packer<msgpack::sbuffer> pk(&buffer);
	//pk.pack(std::string("Hello PackRaw"));
	pk.pack(1);

	RawData data;
	data.Generate();

	int size;

	size = data._sb.size();

	pk.pack_raw(data._sb.size());
	pk.pack_raw_body(data._sb.data(), data._sb.size());

	size = buffer.size();

	// deserializes these objects using msgpack::unpacker.
	msgpack::unpacker pac;

	// feeds the buffer.
	pac.reserve_buffer(buffer.size());
	memcpy(pac.buffer(), buffer.data(), buffer.size());
	pac.buffer_consumed(buffer.size());

	// now starts streaming deserialization.
	msgpack::unpacked result;
	while(pac.next(&result))
	{
		msgpack::object object = result.get();
		if(object.type == msgpack::type::RAW)
		{
			msgpack::unpacked msg;
			msgpack::unpack(&msg, object.via.raw.ptr, object.via.raw.size);

			RawData data = msg.get().as<RawData>();
			std::cout << result.get() << std::endl;
		}
		else
			std::cout << result.get() << std::endl;
	}

}