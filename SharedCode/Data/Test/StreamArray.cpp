#include "../MessagePack/include/msgpack.hpp"
#include <vector>
#include <string>
#include <iostream>

void StreamArray()
{
	// serializes multiple objects into one message containing an array using msgpack::packer.
	msgpack::sbuffer buffer;

	msgpack::packer<msgpack::sbuffer> pk(&buffer);
	pk.pack_array(3);
	pk.pack(std::string("Log message ... 1"));
	pk.pack(std::string("Log message ... 2"));
	pk.pack(std::string("Log message ... 3"));

	// serializes multiple objects into one message containing a map using msgpack::packer.
	msgpack::sbuffer buffer2;

	msgpack::packer<msgpack::sbuffer> pk2(&buffer2);
	pk2.pack_map(2);
	pk2.pack(std::string("x"));
	pk2.pack(3);
	pk2.pack(std::string("y"));
	pk2.pack(3.4321);

}