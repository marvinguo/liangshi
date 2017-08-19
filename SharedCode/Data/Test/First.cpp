#include "../MessagePack/include/msgpack.hpp"
#include <vector>
#include <string>
#include <iostream>

void First()
{
	// serializes this object.
	std::vector<std::string> vec;
	vec.push_back("Hello");
	vec.push_back("MessagePack");

	// serialize it into simple buffer.
	msgpack::sbuffer sbuf;
	msgpack::pack(sbuf, vec);

	// deserialize it.
	msgpack::unpacked msg;
	msgpack::unpack(&msg, sbuf.data(), sbuf.size());

	// print the deserialized object.
	msgpack::object obj = msg.get();
	std::cout << obj << std::endl;  //=> ["Hello", "MessagePack"]

	// convert it into statically typed object.
	std::vector<std::string> rvec;
	obj.convert(&rvec);
}