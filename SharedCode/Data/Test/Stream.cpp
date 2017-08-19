#include "../MessagePack/include/msgpack.hpp"
#include <vector>
#include <string>
#include <iostream>

void Stream()
{
	// serializes multiple objects using msgpack::packer.
	msgpack::sbuffer buffer;

	msgpack::packer<msgpack::sbuffer> pk(&buffer);
	pk.pack(std::string("Log message ... 1"));
	pk.pack(std::string("Log message ... 2"));
	pk.pack(std::string("Log message ... 3"));
	pk.pack(1);

	// deserializes these objects using msgpack::unpacker.
	msgpack::unpacker pac;

	// feeds the buffer.
	pac.reserve_buffer(buffer.size());
	memcpy(pac.buffer(), buffer.data(), buffer.size());
	pac.buffer_consumed(buffer.size());

	// now starts streaming deserialization.
	msgpack::unpacked result;
	while(pac.next(&result)) {
		std::cout << result.get() << std::endl;
	}

	// results:
	// $ g++ stream.cc -lmsgpack -o stream
	// $ ./stream
	// "Log message ... 1"
	// "Log message ... 2"
	// "Log message ... 3"
}