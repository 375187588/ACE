#pragma once
#include "Common.h"

template <typename T, typename... Args>
void outLog(T head, Args... args)
{
	ACE_OS::printf(head,args...);
}

inline void outLog(const char* pValue)
{
	ACE_OS::printf(pValue);
}
