#pragma once
#include<stdlib.h>
#include<assert.h>
#include <mutex>

#define MAX_MEMORY_SIZE 512

class MemoryAlloc;
class MemoryBlock
{
public:
	//所属大内存块
	MemoryAlloc * pAlloc;
	//下一块位置
	MemoryBlock* pNext;
	//内存块编号
	int nID;
	//引用次数
	int nRef;
	//是否在内存池中
	bool bPool;
	//预
private:
	char c1, c2, c3;
};

class MemoryAlloc
{
public:
	MemoryAlloc()
	{
		m_pBuf = nullptr;
		m_pHeader = nullptr;
		m_nSize = 0;
		m_nBlockSize = 0;
	}
	~MemoryAlloc()
	{
		if (nullptr != m_pBuf)
		{
			free(m_pBuf);
		}
	}
	void* allocMemory(size_t nSize)
	{
		if (nullptr == m_pBuf)
		{
			InitMemory();
		}
		MemoryBlock* pReturn = nullptr;
		if (nullptr == m_pHeader)
		{
			pReturn = (MemoryBlock*)malloc(nSize + sizeof(MemoryBlock));
			pReturn->bPool = false;
			pReturn->nID = -1;
			pReturn->nRef = 1;
			pReturn->pAlloc = nullptr;
			pReturn->pNext = nullptr;
			//CCellLog::Info("allocMem: %x, id = %d, size = %d\n", (uint32_t)pReturn, pReturn->nID, nSize);
		}
		else
		{
			pReturn = m_pHeader;
			m_pHeader = m_pHeader->pNext;
			assert(0 == pReturn->nRef);
			pReturn->nRef = 1;
		}
		//CCellLog::Info("allocMem: %x, id = %d, size = %d\n", (uint32_t)pReturn, pReturn->nID, nSize);

		return ((char*)pReturn + sizeof(MemoryBlock));
	}

	void freeMemory(void* pMemory)
	{
		MemoryBlock* pBlock = (MemoryBlock*)((char*)pMemory - sizeof(MemoryBlock));
		if (--pBlock->nRef != 0)
		{
			return;
		}

		if (pBlock->bPool)
		{
			pBlock->pNext = m_pHeader;
			m_pHeader = pBlock;
		}
		else
		{
			free(pBlock);	
		}
	}

	void InitMemory()
	{
		//必然是空指针
		assert(nullptr == m_pBuf);
		if (nullptr != m_pBuf)
			return;
		//真实内存池大小
		size_t realSize = m_nSize + sizeof(MemoryBlock);
		//计算内存池的大小
		size_t bufSize = realSize * m_nBlockSize;
		//向系统申请内存
		m_pBuf = (char*)malloc(bufSize);
		//初始化内存池
		m_pHeader = (MemoryBlock*)m_pBuf;
		m_pHeader->bPool = true;
		m_pHeader->nID = 0;
		m_pHeader->nRef = 0;
		m_pHeader->pAlloc = this;
		m_pHeader->pNext = nullptr;

		//遍历内存块初始化
		MemoryBlock* pTemp1 = m_pHeader;
		for(size_t n = 1; n < m_nBlockSize; n++)
		{
			MemoryBlock* pTemp2 = (MemoryBlock*)(m_pBuf + (n*realSize));
			pTemp2->bPool = true;
			pTemp2->nID = n;
			pTemp2->nRef = 0;
			pTemp2->pAlloc = this;
			pTemp2->pNext = nullptr;
			pTemp1->pNext = pTemp2;
			pTemp1 = pTemp2;
		}
	}
	
protected:
	//内存池地址
	char* m_pBuf;
	//头部内存单元
	MemoryBlock* m_pHeader;
	//内存单元大小
	size_t m_nSize;
	//内存单元的数量
	size_t m_nBlockSize;
};

template<size_t nSize,size_t nBlockSize>
class MemoryAlloctor :public MemoryAlloc
{
public:
	MemoryAlloctor()
	{
		// 8 4 61/8 = 7
		//字节对齐
		size_t n = sizeof(void*);
		//(7+8)+8
		m_nSize = (nSize/n)*n+(nSize%n?n:0);
		m_nBlockSize = nBlockSize;
	}
};

class MemoryMgr
{
public:
	MemoryMgr() 
	{
		init(0, 64, &m_mem64);
		init(65, 128, &m_mem128);
		init(129, 256, &m_mem256);
		init(257, 512, &m_mem512);
		//init(513, 1024, &m_mem1024);
	}

	~MemoryMgr()
	{

	}

	static MemoryMgr& Instance()
	{
		static MemoryMgr mgr;
		return mgr;
	}

	void* allocMem(size_t nSize)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (nSize <= MAX_MEMORY_SIZE)
		{			
			return m_azAlloc[nSize]->allocMemory(nSize);
		}
		MemoryBlock* pRetBlock = (MemoryBlock*)malloc(nSize + sizeof(MemoryBlock));
		pRetBlock->bPool = false;
		pRetBlock->nID = -1;
		pRetBlock->nRef = 1;
		pRetBlock->pAlloc = nullptr;
		pRetBlock->pNext = nullptr;
		//CCellLog::Info("allocMem: %x, id = %d, size = %d\n", (uint32_t)pRetBlock, pRetBlock->nID, nSize);
		return  ((char*)pRetBlock + sizeof(MemoryBlock));
	}

	void freeMem(void* pMem)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		//向左偏移
		MemoryBlock* pBlock = (MemoryBlock*)((char*)pMem - sizeof(MemoryBlock));
		//CCellLog::Info("freeMem: %x, id = %d\n", (uint32_t)pBlock, pBlock->nID);
		if (pBlock->bPool)
		{
			pBlock->pAlloc->freeMemory(pMem);
			return;
		}
		else
		{
			if (--pBlock->nRef == 0)
			{
				free(pBlock);
				return;
			}
		}
	}

	void addRef(void* pMem)
	{
		MemoryBlock* pBlock = (MemoryBlock*)((char*)pMem + sizeof(MemoryBlock));
		++pBlock->nRef;	
	}

private:
	void init(int nBegin,int nEnd,MemoryAlloc* pMemA)
	{
		int n = nBegin;
		while (n <= nEnd)
		{
			m_azAlloc[n] = pMemA;
			++n;
		}
	}
	
private:
	MemoryAlloctor<64, 1000> m_mem64;
	MemoryAlloctor<128, 100> m_mem128;
	MemoryAlloctor<256, 100> m_mem256;
	MemoryAlloctor<512, 10> m_mem512;
	//MemoryAlloctor<1024, 10> m_mem1024;
	MemoryAlloc* m_azAlloc[MAX_MEMORY_SIZE + 1];
	//多线程下锁
	std::mutex m_mutex;
};