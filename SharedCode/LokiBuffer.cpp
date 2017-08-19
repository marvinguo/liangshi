#include "LokiBuffer.h"

#if defined(_MSC_VER) && (_MSC_VER >= 1600)
#pragma execution_character_set("utf-8")
#endif

LokiBuffer::LokiBuffer(void)
{
}

//我就弄不明白了，	msgpack::sLokiBuffer 为啥不能copy呀
LokiBuffer::LokiBuffer(const char* data, const int size)
{
	//_LokiBuffer.resize(size);
	_buffer.insert(_buffer.end(), data, data + size);
}

LokiBuffer::LokiBuffer(const LokiBuffer& another)
{
	_buffer.insert(_buffer.end(), another._buffer.begin(), another._buffer.end());
}

LokiBuffer::~LokiBuffer()
{
}

LokiBuffer& LokiBuffer::operator= (const LokiBuffer &another)
{
	_buffer = another._buffer;
	return *this;
}