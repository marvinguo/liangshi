////////////////////////////////////////////////////////////////////////
// Copyright(c) 1999-2016, All Rights Reserved
// Author:	FU YAN
// Created:2016/01/05
// Describe:
////////////////////////////////////////////////////////////////////////
#include "UVLock.h"

CUVLock::CUVLock()
{
	uv_mutex_init(&m_uvMutex);
}

CUVLock::~CUVLock()
{
	uv_mutex_destroy(&m_uvMutex);
}

void CUVLock::lock()
{
	uv_mutex_lock(&m_uvMutex);
}

bool CUVLock::tryLock()
{
	return uv_mutex_trylock(&m_uvMutex) == 0;
}

void CUVLock::unlock()
{
	uv_mutex_unlock(&m_uvMutex);
}