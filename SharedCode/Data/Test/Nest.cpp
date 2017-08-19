#include "../MessagePack/include/msgpack.hpp"
#include <string>
using namespace std;

class In
{
public:
	int _x;
	int _y;
	MSGPACK_DEFINE(_x, _y);
};

//±ØÐëÔÚpublic
class Out
{
public:
	string _description;
	In _in;
	MSGPACK_DEFINE(_description, _in);
};

void Nest()
{
	Out out;
	out._description = "hello Marvin";

	out._in._x = 10;
	out._in._y = 20;

	msgpack::sbuffer sbuf;
	msgpack::pack(sbuf, out);
	
	msgpack::zone zone;
	msgpack::object obj;
	msgpack::unpack(sbuf.data(), sbuf.size(), NULL, &zone, &obj);
	Out o;
	obj.convert(&o);

	msgpack::unpacked msg;
	msgpack::unpack(&msg, sbuf.data(), sbuf.size());
	msgpack::object obj1 = msg.get();
	Out o1;
	obj1.convert(&o1);


}