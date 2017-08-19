#pragma once

#include "stl.h"
#include "LokiBuffer.h"

class LokiReceiveBuffer
{
public:
	LokiReceiveBuffer();
	LokiReceiveBuffer(const char* data, const int size);
	LokiReceiveBuffer(LokiReceiveBuffer& another);
	~LokiReceiveBuffer();

	LokiReceiveBuffer& operator= (const LokiReceiveBuffer &another);

public:
	void InsertData(char* data, int size);
	void InsertNetworkUInt32(int value);
	uint32_t Get4BytesHead();
	int Size(){return _queue.size();}
	LokiBuffer GetBuffer(int size);

	//char* data(){return (char*)_deque.data();}

private:
	queue<char> _queue;
};
