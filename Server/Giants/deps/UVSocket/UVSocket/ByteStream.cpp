////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2016, All Rights Reserved
// Author:	FU YAN
// Created:2016/01/11
// Describe:
////////////////////////////////////////////////////////////////////////
#include "ByteStream.h"

#define BS_DEFAULT_BUFFER_CAPACITY		1024
#define BS_TABLE_INDEX_INVALID			(-1)

#define BS_APPEND(t, v) MACRO_WITH_FORCED_SEMICOLON(UV_ASSERT(m_bBufferOwner); if (this->GetSize() + sizeof(t) >= m_nCapacity) { this->Extend(); } t* pPointer = (t*)(m_pBuffer + this->GetSize()); *pPointer = val; m_pHead->usBodySize += sizeof(t); return *this;)
#define BS_READ(t, v) MACRO_WITH_FORCED_SEMICOLON(if (m_nReadIndex + sizeof(t) > this->GetBodySize()) { return *this; } t* pPointer = (t*)(m_pHead->szBuf + m_nReadIndex); val = *pPointer; m_nReadIndex += sizeof(t); return *this;)

CByteStream::CByteStream()
{
	m_bBufferOwner = true;
	m_pBuffer = new char[BS_DEFAULT_BUFFER_CAPACITY];
	m_pHead = (ByteStreamHead_t*)m_pBuffer;
	if (m_pHead)
	{
		m_pHead->usAction = 0;
		m_pHead->usBodySize = 0;
	}
	m_nCapacity = BS_DEFAULT_BUFFER_CAPACITY;
	m_nReadIndex = 0;
	m_nTableHeadIndex = BS_TABLE_INDEX_INVALID;
}

CByteStream::CByteStream(const CByteStream &c)
{
	if (&c == this)
	{
		return;
	}

	if (m_nCapacity != c.m_nCapacity)
	{
		m_pBuffer = new char[c.m_nCapacity];
		m_pHead = (ByteStreamHead_t*)m_pBuffer;
		if (m_pBuffer)
		{
			memcpy(m_pBuffer, c.m_pBuffer, c.GetSize());
		}
	}
	m_bBufferOwner = true;
	m_nCapacity = c.m_nCapacity;
	m_nReadIndex = c.m_nReadIndex;
	m_nTableHeadIndex = c.m_nTableHeadIndex;
}

CByteStream::CByteStream(const char* pBuf, int nSize)
{
	m_pBuffer = (char*)pBuf;
	m_pHead = (ByteStreamHead_t*)m_pBuffer;
	m_nCapacity = nSize;
	m_nReadIndex = 0;
	m_nTableHeadIndex = BS_TABLE_INDEX_INVALID;
	m_bBufferOwner = false;
}

CByteStream::~CByteStream()
{
	if (m_bBufferOwner)
	{
		SAFE_DELETE_ARRAY(m_pBuffer);
	}
	
	m_pHead = NULL;
}

void CByteStream::Clear()
{
	m_nReadIndex = 0;
	m_pHead->usAction = 0;
	m_pHead->usBodySize = 0;
	m_nTableHeadIndex = BS_TABLE_INDEX_INVALID;
}

const char* CByteStream::GetBufferPointer() const
{
	UV_ASSERT(this->IsTableFinished());

	return m_pBuffer;
}

int CByteStream::GetCapacity() const
{
	return m_nCapacity;
}

uint16_t CByteStream::GetSize() const
{
	if (m_pHead == NULL)
	{
		return 0;
	}

	return m_pHead->usBodySize + sizeof(ByteStreamHead_t);
}

uint16_t CByteStream::GetBodySize() const
{
	if (m_pHead == NULL)
	{
		return 0;
	}

	return m_pHead->usBodySize;
}

bool CByteStream::WriteBytes(const char* pBuf, size_t nSize)
{
	UV_ASSERT(m_bBufferOwner);

	if (pBuf == NULL)
	{
		return false;
	}

	if (this->GetSize() + nSize >= m_nCapacity)
	{
		bool bSucc = this->Extend();
		if (!bSucc)
		{
			return false;
		}
	}

	memcpy(m_pBuffer + this->GetSize(), pBuf, nSize);

	m_pHead->usBodySize += (uint16_t)nSize;

	return true;
}

int CByteStream::ReadBytes(char* pDstBuf, size_t nSize)
{
	if (pDstBuf == NULL)
	{
		return 0;
	}

	int nReadSize = (int)nSize;
	if (this->GetBodySize() - m_nReadIndex < nSize)
	{
		nReadSize = this->GetBodySize() - m_nReadIndex;
	}

	if (nReadSize <= 0)
	{
		return 0;
	}

	memcpy(pDstBuf, m_pHead->szBuf+m_nReadIndex, nReadSize);

	m_nReadIndex += nReadSize;

	return nReadSize;
}

void CByteStream::Dump()
{
	printf("ByteStream:[Capacity:%d], [Action:%hu] ,[Size:%hu], [BodySize:%hu], [ReadIndex:%d], [TableIndex:%d]\n", this->GetCapacity(), this->GetAction(), this->GetSize(), this->GetBodySize(),  m_nReadIndex, m_nTableHeadIndex);
}

uint16_t CByteStream::GetAction() const
{
	if (m_pHead == NULL)
	{
		return 0;
	}

	return m_pHead->usAction;
}

void CByteStream::SetAction(uint16_t usAction)
{
	if (m_pHead == NULL)
	{
		return;
	}

	m_pHead->usAction = usAction;
}

uint64_t CByteStream::GetPeerID() const
{
	if (m_pHead == NULL)
	{
		return 0;
	}

	uint64_t* p64IdPeer = (uint64_t*)(m_pHead->szBuf + this->GetBodySize() - sizeof(uint64_t));
	
	return *p64IdPeer;
}

void CByteStream::AddPeerID(uint64_t idPeer)
{
	*this << idPeer;
}


void CByteStream::RemovePeerID()
{
	if (m_pHead == NULL)
	{
		return;
	}

	m_pHead->usBodySize -= sizeof(uint64_t);
}

void CByteStream::Rewind()
{
	m_nReadIndex = 0;
}

CByteStream & CByteStream::operator<<(const bool val)
{
	BS_APPEND(bool, val);
}

CByteStream & CByteStream::operator<<(const char val)
{
	BS_APPEND(char, val);
}

CByteStream & CByteStream::operator<<(const uint8_t val)
{
	BS_APPEND(uint8_t, val);
}

CByteStream & CByteStream::operator<<(const int8_t val)
{
	BS_APPEND(int8_t, val);
}

CByteStream & CByteStream::operator<<(const uint16_t val)
{
	BS_APPEND(uint16_t, val);
}

CByteStream & CByteStream::operator<<(const int16_t val)
{
	BS_APPEND(int16_t, val);
}

CByteStream & CByteStream::operator<<(const uint32_t val)
{
	BS_APPEND(uint32_t, val);
}

CByteStream & CByteStream::operator<<(const int32_t val)
{
	BS_APPEND(int32_t, val);
}

CByteStream & CByteStream::operator<<(const int64_t val)
{
	BS_APPEND(int64_t, val);
}

CByteStream & CByteStream::operator<<(const uint64_t val)
{
	BS_APPEND(uint64_t, val);
}

CByteStream & CByteStream::operator<<(const float val)
{
	BS_APPEND(float, val);
}

CByteStream & CByteStream::operator<<(const double val)
{
	BS_APPEND(double, val);
}

CByteStream & CByteStream::operator<<(const long double val)
{
	BS_APPEND(long double, val);
}

CByteStream & CByteStream::operator<<(const char* szStr)
{
	if (szStr == NULL)
	{
		return *this;
	}
	uint16_t usSize = (uint16_t)(strlen(szStr));
	*this << usSize;

	this->WriteBytes(szStr, usSize);

	return *this;
}

CByteStream & CByteStream::operator<<(const std::string str)
{
	*this << (uint16_t)(str.size());
	this->WriteBytes(str.c_str(), str.size());

	return *this;
}

CByteStream & CByteStream::operator>>(bool& val)
{
	BS_READ(bool, val);
}

CByteStream & CByteStream::operator>>(char& val)
{
	BS_READ(char, val);
}

CByteStream & CByteStream::operator>>(uint8_t& val)
{
	BS_READ(uint8_t, val);
}

CByteStream & CByteStream::operator>>(int8_t& val)
{
	BS_READ(int8_t, val);
}

CByteStream & CByteStream::operator>>(uint16_t& val)
{
	BS_READ(uint16_t, val);
}

CByteStream & CByteStream::operator>>(int16_t& val)
{
	BS_READ(int16_t, val);
}

CByteStream & CByteStream::operator>>(uint32_t& val)
{
	BS_READ(uint32_t, val);
}

CByteStream & CByteStream::operator>>(int32_t& val)
{
	BS_READ(int32_t, val);
}

CByteStream & CByteStream::operator>>(uint64_t& val)
{
	BS_READ(uint64_t, val);
}

CByteStream & CByteStream::operator>>(int64_t& val)
{
	BS_READ(int64_t, val);
}

CByteStream & CByteStream::operator>>(float& val)
{
	BS_READ(float, val);
}

CByteStream & CByteStream::operator>>(double& val)
{
	BS_READ(double, val);
}

CByteStream & CByteStream::operator>>(long double& val)
{
	BS_READ(long double, val);
}

CByteStream & CByteStream::operator>>(char* szStr)
{
	UV_ASSERT(m_bBufferOwner);

	szStr[0] = 0;
	if (m_nReadIndex + sizeof(uint16_t) > this->GetBodySize())
	{ 
		return *this;
	}

	uint16_t* pPointer = (uint16_t*)(m_pHead->szBuf + m_nReadIndex);
	int nSize = *pPointer;
	m_nReadIndex += sizeof(uint16_t);

	this->ReadBytes(szStr, nSize);

	szStr[nSize] = 0;

	return *this;
}

CByteStream & CByteStream::operator>>(std::string& str)
{
	UV_ASSERT(m_bBufferOwner);

	if (m_nReadIndex + sizeof(uint16_t) > this->GetBodySize())
	{
		return *this;
	}

	uint16_t* pPointer = (uint16_t*)(m_pHead->szBuf + m_nReadIndex);
	int nSize = *pPointer;
	m_nReadIndex += sizeof(uint16_t);

	char* szStr = new char[nSize+1];
	if (szStr == NULL)
	{
		return *this;
	}

	this->ReadBytes(szStr, nSize);

	szStr[nSize] = 0;

	str = szStr;

	SAFE_DELETE_ARRAY(szStr);

	return *this;
}

void CByteStream::StartWriteTable()
{
	UV_ASSERT(m_bBufferOwner);
	if (this->GetSize() + sizeof(uint16_t) >= m_nCapacity)
	{
		this->Extend();
	}

	uint16_t* pusTableSize = (uint16_t*)(m_pBuffer+this->GetSize());
	*pusTableSize = 0;
	m_nTableHeadIndex = this->GetSize();
	m_pHead->usBodySize += sizeof(uint16_t);
}

void CByteStream::EndWriteTable()
{
	UV_ASSERT(m_bBufferOwner);
	UV_ASSERT(this->GetSize() > m_nTableHeadIndex + sizeof(uint16_t));
	UV_ASSERT(!this->IsTableFinished());

	uint16_t* pusTableSize = (uint16_t*)(m_pBuffer+m_nTableHeadIndex);
	*pusTableSize = this->GetSize() - m_nTableHeadIndex - (uint16_t)(sizeof(uint16_t));
	m_nTableHeadIndex = BS_TABLE_INDEX_INVALID;
}

void CByteStream::StartReadTable()
{
	m_nTableHeadIndex = m_nReadIndex;
	m_nReadIndex += sizeof(uint16_t);
}

void CByteStream::EndReadTable()
{
	//force ReadIndex move to next table head
	UV_ASSERT(!this->IsTableFinished());
	uint16_t* pusTableSize = (uint16_t*)(m_pHead->szBuf + m_nTableHeadIndex);
	m_nReadIndex = m_nTableHeadIndex + *pusTableSize + sizeof(uint16_t);
	m_nTableHeadIndex = BS_TABLE_INDEX_INVALID;
}

bool CByteStream::Extend(int nNewSize)
{
	UV_ASSERT(m_bBufferOwner);
	//扩容采用翻倍策略,若指定了目标大小，则会不断翻倍直到超过指定大小
	//如nNewSize指定为3000,则最后Capacity将扩容到4096
	int nNewCapacity = BS_DEFAULT_BUFFER_CAPACITY;
	while (m_nCapacity > 0 && nNewCapacity <= m_nCapacity)
	{
		nNewCapacity *= 2;
	}

	while (nNewSize > 0 && nNewCapacity < nNewSize)
	{
		nNewCapacity *= 2;
	}
	
	char* pBuf = new char[nNewCapacity];
	if (pBuf == NULL)
	{
		return false;
	}
	memcpy(pBuf, m_pBuffer, this->GetSize());
	SAFE_DELETE_ARRAY(m_pBuffer);
	m_pBuffer = pBuf;
	m_pHead = (ByteStreamHead_t*)m_pBuffer;
	m_nCapacity = nNewCapacity;

	return true;
}

bool CByteStream::IsTableFinished() const
{
	return (m_nTableHeadIndex == BS_TABLE_INDEX_INVALID);
}

CByteStream & CByteStream::operator=(const CByteStream &c)
{
	if (&c == this)
	{
		return *this;
	}

	if (m_nCapacity != c.m_nCapacity)
	{
		if (m_bBufferOwner)
		{
			SAFE_DELETE_ARRAY(m_pBuffer);
		}

		m_pBuffer = new char[c.m_nCapacity];
		m_pHead = (ByteStreamHead_t*)m_pBuffer;
		if (m_pBuffer)
		{
			memcpy(m_pBuffer, c.m_pBuffer, c.GetSize());
		}
	}
	
	m_nCapacity = c.m_nCapacity;
	m_nReadIndex = c.m_nReadIndex;
	m_nTableHeadIndex = c.m_nTableHeadIndex;
	m_bBufferOwner = true;

	return *this;
}

