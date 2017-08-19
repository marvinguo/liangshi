////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2015, All Rights Reserved
// Author:	FU YAN
// Created:2015/12/31
// Describe:UVLock
////////////////////////////////////////////////////////////////////////
#ifndef _UVLOCK_H_
#define _UVLOCK_H_

#include "uv.h"

class CUVLock
{
public:
	CUVLock();
	~CUVLock();

	void lock();
	/// Locks the mutex. Blocks if the mutex
	/// is held by another thread.

	bool tryLock();
	/// Tries to lock the mutex. Returns false immediately
	/// if the mutex is already held by another thread.
	/// Returns true if the mutex was successfully locked.

	void unlock();
	/// Unlocks the mutex so that it can be acquired by
	/// other threads.
private:
	uv_mutex_t m_uvMutex;
};



template <class M>
class CScopedLock
	/// A class that simplifies thread synchronization
	/// with a mutex.
	/// The constructor accepts a Mutex (and optionally
	/// a timeout value in milliseconds) and locks it.
	/// The destructor unlocks the mutex.
{
public:
	explicit CScopedLock(M& mutex) : _mutex(mutex)
	{
		_mutex.lock();
	}

	~CScopedLock()
	{

		_mutex.unlock();
	}

private:
	M& _mutex;

	CScopedLock();
	CScopedLock(const CScopedLock&);
};

typedef CScopedLock<CUVLock> UVScopedLock;

#endif	//end of define _UVLOCK_H_
