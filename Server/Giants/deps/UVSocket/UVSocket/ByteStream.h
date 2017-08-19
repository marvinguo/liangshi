////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2016, All Rights Reserved
// Author:	FU YAN
// Created:2016/01/11
// Describe:ByteStream
////////////////////////////////////////////////////////////////////////
#ifndef _BYTESTREAM_H_
#define _BYTESTREAM_H_

#include <string>
#include "UVSocketDef.h"

#pragma warning(push)
#pragma warning(disable:4200)
typedef struct ByteStreamHead_s
{
	uint16_t usAction;			//消息类型
	uint16_t usBodySize;			//消息体的长度
	char szBuf[0];
}ByteStreamHead_t;
#pragma warning(pop)

/*
	格式说明:
	1. 总体结构（暂时只支持little endian）:
	===============================================================
	|  usAction(2Bytes)  |  usBodySize(2Bytes)  |  Content...       
	===============================================================
	2. bool, char, short, int, long long, float, double, long double(包括有符号和无符号类型):占用字节数等于该类型的字节数，内容为数值
	3. long, unsigned long: 由于长度是平台相关的，暂不支持
	4. c-style string, std::string:
	===============================================================
	| usSize(2Bytes) |  string content without zero end(usSize Bytes)
	===============================================================
	5. table（暂不支持嵌套）:
	===============================================================
	|  usTableSize(2Bytes)  | table content(除了table外的所有类型均可)
	===============================================================
*/
class CByteStream
{
public:
	CByteStream();
	CByteStream(const char* pBuf, int nSize);
	CByteStream(const CByteStream &c); // Copy constructor
	CByteStream &operator=(const CByteStream &c); // Copy assignment operator
	virtual ~CByteStream();

public:
	void Clear();
	const char* GetBufferPointer() const;
	int GetCapacity() const;
	uint16_t GetSize() const;
	uint16_t GetBodySize() const;
	bool WriteBytes(const char* pBuf, size_t nSize);
	int ReadBytes(char* pDstBuf, size_t nSize);
	void Dump();
	uint16_t GetAction() const;
	void SetAction(uint16_t usAction);
	uint64_t GetPeerID() const;
	void AddPeerID(uint64_t idPeer);
	void RemovePeerID();
	void Rewind();

public://Operator Overide
	// Operator << overloading
	// 不支持long和unsigned long，这两个的长度在不同OS上可能出现不同
	CByteStream &operator<<(const bool val);
	CByteStream &operator<<(const char val);
	CByteStream &operator<<(const uint8_t val);
	CByteStream &operator<<(const int8_t val);
	CByteStream &operator<<(const uint16_t val);
	CByteStream &operator<<(const int16_t val);
	CByteStream &operator<<(const uint32_t val);
	CByteStream &operator<<(const int32_t val);
	CByteStream &operator<<(const int64_t val);
	CByteStream &operator<<(const uint64_t val);
	CByteStream &operator<<(const float val);
	CByteStream &operator<<(const double val);
	CByteStream &operator<<(const long double val);
	CByteStream &operator<<(const char* szStr);
	CByteStream &operator<<(const std::string str);

	// Operator >> overloading
	CByteStream &operator>>(bool& val);
	CByteStream &operator>>(char& val);
	CByteStream &operator>>(uint8_t& val);
	CByteStream &operator>>(int8_t& val);
	CByteStream &operator>>(uint16_t& val);
	CByteStream &operator>>(int16_t& val);
	CByteStream &operator>>(uint32_t& val);
	CByteStream &operator>>(int32_t& val);
	CByteStream &operator>>(uint64_t& val);
	CByteStream &operator>>(int64_t& val);
	CByteStream &operator>>(float& val);
	CByteStream &operator>>(double& val);
	CByteStream &operator>>(long double& val);
	CByteStream &operator>>(char* szStr);
	CByteStream &operator>>(std::string& str);

public://table operations
	void StartWriteTable();
	void EndWriteTable();
	void StartReadTable();
	void EndReadTable();


private:
	bool Extend(int nNewSize = 0);
	bool IsTableFinished() const;

private:
	char *m_pBuffer;
	int m_nCapacity;
	int m_nReadIndex;
	ByteStreamHead_t* m_pHead;
	int m_nTableHeadIndex;
	bool m_bBufferOwner;
};

#endif	//end of define _BYTESTREAM_H_
