#pragma once

#include "stl.h"
#include "library.h"

class LokiBuffer
{
public:
	LokiBuffer();
	LokiBuffer(const char* data, const int size);
	LokiBuffer(const LokiBuffer& another);
	~LokiBuffer();

	LokiBuffer& operator= (const LokiBuffer &another);

public:
	void AddChar(char c){_buffer.push_back(c);}
	char* Data(){return (char*)_buffer.data();}
	int Size(){return _buffer.size();}

private:
	vector<int8_t> _buffer;
public:
	MSGPACK_DEFINE(_buffer);
};
