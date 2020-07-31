#pragma once
#include<stdlib.h>

void* operator new(size_t size);

void operator delete(void* p);

void* operator new[](size_t size);

void operator delete[](void* p);

void* mallocx(size_t size);

void mfreex(void* p);