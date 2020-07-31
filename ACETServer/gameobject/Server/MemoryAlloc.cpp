#include"MemoryAlloc.h"
#include"MemoryMgr.hpp"


void* operator new(size_t nSize)
{
	return MemoryMgr::Instance().allocMem(nSize);
}

void operator delete(void* p)
{
	MemoryMgr::Instance().freeMem(p);
}

void* operator new[](size_t nSize)
{
	return MemoryMgr::Instance().allocMem(nSize);
}

void operator delete[](void* p)
{
	MemoryMgr::Instance().freeMem(p);
}

void* mallocx(size_t nSize)
{
	return malloc(nSize);
}

void mfreex(void* p)
{
	free(p);
}