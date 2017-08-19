#include "LokiReceiveBuffer.h"

#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <arpa/inet.h>
#elif defined(__APPLE__)
#include <arpa/inet.h>
#endif

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LokiReceiveBuffer::LokiReceiveBuffer(void)
{
}

LokiReceiveBuffer::LokiReceiveBuffer(const char* data, const int size)
{
	for(int i = 0; i < size; i++)
		_queue.push(data[i]);
}

LokiReceiveBuffer::LokiReceiveBuffer(LokiReceiveBuffer& another)
{
	_queue = another._queue;
	//_deque.insert(_deque.end(), another.data(), another.data() + another.size());
}

LokiReceiveBuffer::~LokiReceiveBuffer()
{
}

LokiReceiveBuffer& LokiReceiveBuffer::operator= (const LokiReceiveBuffer &another)
{
	_queue = another._queue;
	return *this;
}

void LokiReceiveBuffer::InsertData(char* data, int size)
{
	for(int i = 0; i < size; i++)
	{
		char c = data[i];
		_queue.push(c);
	}
}

void LokiReceiveBuffer::InsertNetworkUInt32(int value)
{
	uint32_t uvalue = (uint32_t)value;
	uint32_t networkValue = htonl(uvalue);

	uint8_t bytes[4];

	bytes[0] = (networkValue >> 24) & 0xFF;
	bytes[1] = (networkValue >> 16) & 0xFF;
	bytes[2] = (networkValue >> 8) & 0xFF;
	bytes[3] = networkValue & 0xFF;

	_queue.push(bytes[3]);
	_queue.push(bytes[2]);
	_queue.push(bytes[1]);
	_queue.push(bytes[0]);
}

//服务器，客户端都是intel, ipad是arm,可能需要考虑结尾的问题
uint32_t LokiReceiveBuffer::Get4BytesHead()
{
	if(_queue.size() < 4)
		return 0;

	uint8_t lsb = _queue.front();
	_queue.pop();
	uint8_t next_lsb = _queue.front();
	_queue.pop();
	uint8_t next_msb =_queue.front();
	_queue.pop();
	uint8_t msb = _queue.front();
	_queue.pop();

	uint32_t dword_from_bytes = (msb << 24) | (next_msb << 16) | (next_lsb << 8) | lsb;
	return dword_from_bytes;
}

LokiBuffer LokiReceiveBuffer::GetBuffer(int size)
{
	LokiBuffer buffer;

	if(Size() >= size)
	{
		for(int i = 0; i < size; i++)
		{
			buffer.AddChar(_queue.front());
			_queue.pop();
		}
	}
	return buffer;
}