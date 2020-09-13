//-----------------------------------------------------------------------------
//!\file mem_map.h
//!\author Lyp
//!
//!\date 2004-06-17
//! last 2009-03-05
//!
//!\brief 内存分析
//-----------------------------------------------------------------------------
#pragma once

#ifdef _MEM_TRACE_

#undef new
#undef malloc
#undef calloc
#undef realloc
#undef free

#include <string>
namespace vEngine {
//-----------------------------------------------------------------------------
//!\brief 内存分析
//! 需要查找内存泄露调用堆栈时，可以先在Alloc函数设置一个断点，再启动程序，
//!	当程序断下来后，将m_dwBreakAddress修改为内存泄露地址，然后取消断点，继续
//! 运行程序。当发生设定地址的内存分配时，Alloc会自动断下来
//-----------------------------------------------------------------------------

//内存泄露分析的参数，只有对超过这个时间以后的内存泄露才报告，单位是毫秒
#define  START_MEM_CHECK_TICK	180 * 1000
class VENGINE_API MemMap
{
public:
	LPVOID Alloc(LPCSTR szFile, INT nLine, size_t uiSize, BOOL bArray);
	VOID Free(LPVOID pAddress, BOOL bArray);
	VOID WriteFile();
	FILE *CreateMemFile();
	LPCTSTR Env();
	VOID ShowMemUseSorted();

	MemMap();
	~MemMap();

private:
	struct tagMemNode
	{
		LPVOID	pAddress;		// 分配地址
		LPCSTR	szFile;			// 文件名
		INT		nLine;			// 行号
		size_t	uiSize;			// 分配大小
		size_t  uiTickCount;    // 分配的时间点
		bool	bArray;			// 是否数组请求
		bool	bArrayError;	// 数组释放错误
		bool	bBoundError;	// 越界操作
		bool	bReserved;		// 为了对齐而保留
	};

	struct tagNodeStat
	{
		std::string	nodeIdentify;	//按照文件+行数组成的内存块标示
		size_t	uiAllocCount;	//分配的次数
		size_t	uiFreeCount;	//释放的次数
		size_t	uiRemainSize;	//当前保留的内存空间
	};

	VOLATILE BOOL					m_bTerminate;
	TSafeMap<LPVOID, tagMemNode*>	m_mapMem;
	TSafeList<tagMemNode*>			m_listArrayErrorNode;
	TSafeList<tagMemNode*>			m_listBoundErrorNode;

	DWORD							m_dwStartTick; //类初始化到时候时间点

	DWORD m_dwBreakAddress;
};

} // namespace vEngine {

VOID VENGINE_API WriteMem();
VOID VENGINE_API ShowMemUseSorted();
LPCTSTR VENGINE_API EnvString();

LPVOID VENGINE_API AllocateMemory(size_t uiSize, LPCSTR szFile, INT nLine, BOOL bArray, LPVOID pAddress=NULL); 
VOID VENGINE_API DeallocateMemory(LPVOID pAddress, BOOL bArray);

inline LPVOID operator new(size_t uiSize, LPCSTR szFile, INT nLine) { return AllocateMemory(uiSize, szFile, nLine, FALSE); }
inline LPVOID operator new[](size_t uiSize, LPCSTR szFile, INT nLine) { return AllocateMemory(uiSize, szFile, nLine, TRUE); }
inline VOID operator delete(LPVOID pAddress) { DeallocateMemory(pAddress, FALSE); }
inline VOID operator delete[](LPVOID pAddress) { DeallocateMemory(pAddress, TRUE); }

// These two routines should never get called, unless an error occures during the 
// allocation process.  These need to be defined to make Visual C++ happy.
inline VOID operator delete(LPVOID pAddress, LPCSTR szFile, INT nLine) { szFile;nLine;DeallocateMemory(pAddress, FALSE); }
inline VOID operator delete[](LPVOID pAddress, LPCSTR szFile, INT nLine) { szFile;nLine;DeallocateMemory(pAddress, TRUE); }

#define new							new(__FILE__, __LINE__)
#define malloc(uiSize)				AllocateMemory(uiSize, __FILE__, __LINE__,  FALSE)
#define calloc(uiNum, uiSize)		AllocateMemory(((uiSize)*(uiNum)), __FILE__, __LINE__, FALSE)
#define realloc(pAddress, uiSize)	AllocateMemory(uiSize, __FILE__, __LINE__, FALSE, pAddress)
#define free(pAddress)				DeallocateMemory(pAddress, FALSE)


#endif // #ifdef _MEM_TRACE_
