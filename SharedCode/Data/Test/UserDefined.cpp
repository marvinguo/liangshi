#include "../MessagePack/include/msgpack.hpp"
#include <vector>
#include <string>
#include <iostream>

class myclass1
{
public:
	int x;

public:
	MSGPACK_DEFINE(x);
};

class myclass : public myclass1
{
public:
	std::string m_str;
	std::vector<int> m_vec;

public:
	MSGPACK_DEFINE(m_str, m_vec, x);
};

void UserDefined()
{
	std::vector<myclass> vec;
	// add some elements into vec...

	// you can serialize myclass directly
	msgpack::sbuffer sbuf;
	msgpack::pack(sbuf, vec);

	msgpack::unpacked msg;
	msgpack::unpack(&msg, sbuf.data(), sbuf.size());

	msgpack::object obj = msg.get();

	// you can convert object to myclass directly
	std::vector<myclass> rvec;
	obj.convert(&rvec);

}

void UserDefined1()
{
	myclass one;
	one.m_str = "Hello Marvin";
	one.x = 100;

	// you can serialize myclass directly
	msgpack::sbuffer sbuf;
	msgpack::pack(sbuf, one);

	msgpack::unpacked msg;
	msgpack::unpack(&msg, sbuf.data(), sbuf.size());

	msgpack::object obj = msg.get();

	myclass two;
	obj.convert(&two);
}