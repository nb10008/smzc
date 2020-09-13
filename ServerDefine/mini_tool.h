/********************************************************************
	created:	2009/12/18
	created:	18:12:2009   11:36
	filename: 	e:\loong\trunk\LoongWorld\mini_tool.h
	file path:	e:\loong\trunk\LoongWorld
	file base:	mini_tool
	file ext:	h
	author:		Jason
	
	purpose:	小工具
*********************************************************************/


#ifndef mini_tool_h__
#define mini_tool_h__

namespace jm_tool{

/**
* \ingroup mythread
*
* 简单的锁定守卫类
*
* \version 1.0
* first version
*
* \date 2009-12-11
*
* \author Jason
*
* \log 
* 2009-12-11 created
*
*/
template<class MLOCK>
class MutexGuard
{
	MLOCK & m_lck;
public:
	MutexGuard(MLOCK &l)
		:m_lck(l)
	{
		m_lck.Lock();
	}
	~MutexGuard(void)
	{
		m_lck.Unlock();
	}
};

/**
* \ingroup mythread
*
* 锁的基类
*
* \version 1.0
* first version
*
* \date 2009-12-11
*
* \author Jason
*
* \log 
* 2009-12-11 created
*
*/
template<typename T>
class MLock
{
public:
	typedef T TLock;
	MLock()
	{}
	virtual ~MLock(void){}
	virtual void Lock(void){}
	virtual void Unlock(void){}
};

typedef MLock<int> EmptyLock;

/**
* \ingroup mythread
*
* 临界区锁
*
* \version 1.0
* first version
*
* \date 2010-12-16
*
* \author Jason
*
*/
class CSCreatedLock : public MLock<CRITICAL_SECTION>
{
	TLock m_cs;
public:
	CSCreatedLock()
	{
		InitializeCriticalSection  ( &m_cs );
	}
	virtual ~CSCreatedLock(void)
	{
		DeleteCriticalSection  ( &m_cs );
	}
	virtual void Lock(void)
	{
		EnterCriticalSection  ( &m_cs );
	}
	virtual void Unlock(void)
	{
		LeaveCriticalSection  ( &m_cs );
	}
};

/**
* \ingroup mythread
*
* 信号基类
*
* \version 1.0
* first version
*
* \date 2010-12-16
*
* \author Jason
*
*/
class signal
{
public:
	signal(){}
	virtual ~signal(){}
	virtual int wait(int timeout) = 0;
	virtual int notify() = 0;
	virtual int release() = 0;
};
/**
* \ingroup mythread
*
* 信号量
*
* \version 1.0
* first version，适用于资源锁
*
* \date 2010-12-16
*
* \author Jason
*
*/
class JSemaphore : public signal
{
public:
	JSemaphore(LONG lInitCount, LONG lMaxCount)
		:m_lCurNum(lInitCount), m_lMaxNum(lMaxCount) { m_hSemaphore = CreateSemaphore(NULL, m_lCurNum, m_lMaxNum,NULL); }
	virtual ~JSemaphore(){ CloseHandle(m_hSemaphore); }

	virtual int wait(int timeout) 
	{ 
		if( WaitForSingleObject( m_hSemaphore,INFINITE) == WAIT_OBJECT_0 )
		{
			InterlockedExchangeAdd((LPLONG)&m_lCurNum, -1); 
			return 0;
		}
		return -1;
	}
	virtual int notify(){ ReleaseSemaphore( m_hSemaphore, 1,NULL); InterlockedExchangeAdd((LPLONG)&m_lCurNum, 1); return m_lCurNum;}
	virtual int release() { return m_lCurNum;}

private:
	HANDLE			m_hSemaphore;
	volatile LONG	m_lCurNum;
	volatile LONG	m_lMaxNum;
};

/**
* \ingroup mythread
*
* 事件
*
* \version 1.0
* first version
*
* \date 2010-12-16
*
* \author Jason
*
*/
class JEvent : public signal
{
public:
	JEvent() { m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL); }
	JEvent(BOOL bManual, BOOL bInitState) { m_hEvent = CreateEvent(NULL, bManual, bInitState, NULL);  }
	virtual ~JEvent() { CloseHandle(m_hEvent); }

	virtual int notify() { SetEvent(m_hEvent); return 0;}
	virtual int release() { ResetEvent(m_hEvent); return 0;}
	virtual int wait(int timeout)	
	{ 
		if( WaitForSingleObject(m_hEvent, timeout) == WAIT_OBJECT_0 )
			return 0;
		return -1;
	}

private:
	HANDLE	m_hEvent;
};

/**
 * \ingroup LoongWorld
 *
 *
 * \version 1.0
 * first version
 *
 * \date 2010-01-09
 *
 * \author Jason
 *
 * \par license
 * This code is absolutely free to use and modify. The code is provided "as is" with
 * no expressed or implied warranty. The author accepts no liability if it causes
 * any damage to your computer, causes your pet to fall ill, increases baldness
 * or makes your car start emitting strange noises when you start it up.
 * This code has no bugs, just undocumented features!
 * 
 * \todo 子类实现svc方法，完成自己的工作
 *
 * \bug 
 *
 */
class JMThread 
{
	struct tagParam
	{
		JMThread * pThis;
		LPVOID pParam;
	};
	tagParam m_lparam;
	DWORD _thread_id;
	LONG _terminate;
	enum {MAX_THREAD = MAXIMUM_WAIT_OBJECTS};
	HANDLE _handles[MAX_THREAD];
	DWORD _threadids[MAX_THREAD];
	bool _isover[MAX_THREAD];
	int _count;

	CSCreatedLock _cs;
private:
	static DWORD WINAPI _threadproc(LPVOID parameter)
	{
		tagParam * lparam = reinterpret_cast<tagParam*> (parameter);
		if( lparam && lparam->pThis )
		{
			DWORD re = lparam->pThis->svc  ( lparam->pParam );
			DWORD id = GetCurrentThreadId  ();
			MutexGuard<CSCreatedLock> guard(lparam->pThis->_cs);
			for( int i = 0 ; i < lparam->pThis->_count ; ++i )
			{
				if( lparam->pThis->_threadids[i] == id )
				{
					lparam->pThis->_isover[i] = true;
					break;
				}
			}
			return re;
		}
		return 0;
	}
protected:
	virtual DWORD svc(LPVOID paramter) = 0;
public:
	BOOL IsTerminate()
	{
		return InterlockedCompareExchange(&_terminate,TRUE,TRUE);
	}
	void Terminate()
	{
		InterlockedExchange(&_terminate,TRUE);
		HANDLE handles[MAX_THREAD] = {0};
		INT nCount = 0;
		for( int i = 0, j = 0 ; i < MAX_THREAD; ++i )
		{
			if(_handles[i])
				handles[nCount++] = _handles[i];
		}
		if( nCount > 0 )
			WaitForMultipleObjects  (nCount,handles,TRUE,20 * 1000);
		for(int i = 0 ;i < nCount; ++i)
		{
			if(handles[i])
				CloseHandle  (handles[i]);
		}
		ResetStatus();
	}
	void Reset()
	{
		InterlockedExchange(&_terminate,FALSE);
		ResetStatus();
	}
	bool IsOver()
	{
		MutexGuard<CSCreatedLock> guard(_cs);
		for( int i = 0 ; i < _count ; ++i )
		{
			if( !_isover[i] )
			{
				return false;
			}
		}
		return true;
	}
	void ResetStatus()
	{
		memset(_handles,0,sizeof(_handles));
		memset  (_isover,0,sizeof(_isover));
		memset(_threadids,0,sizeof(_threadids));
		_count = 0;
	}
public:
	JMThread()
		:_terminate(FALSE),_count(0)
	{
		//memset(_isover,0,sizeof(_isover));
		//memset(_threadids,0,sizeof(_threadids));
		ResetStatus();
	}
	virtual ~JMThread(){}
	int Active(int num,LPVOID param)
	{
		if( num <= 0 )
			return -1;

		Reset();

		m_lparam.pThis = this;
		m_lparam.pParam = param;
		if( num > MAX_THREAD )
			num = MAX_THREAD;
		_count = num;
		int i = 0;
		HANDLE h;
		while(num-- > 0)
		{
			h = ::CreateThread  (0,0,_threadproc,&m_lparam,0,&_thread_id);
			if( NULL != h )
			{
				MutexGuard<CSCreatedLock> guard(_cs);
				_threadids[i] = _thread_id;
				_handles[i++] = h;
			}
		}

		_count = i;

		return i;
	}
};

/*********************************************************************************************************
*
* 单件部分
*
*********************************************************************************************************/

template<typename T>
struct JMSingletonAdaptor
{
	typedef T * pointer;
	static pointer construct(void * p = NULL){return new T;}
	static void destroy(pointer p){delete p;}
};

template< class T, class adaptor = JMSingletonAdaptor<T>, class TLock = CSCreatedLock >
class JMSingleton
{
public:
	typedef typename adaptor::pointer TPointer;
private:
	static TPointer _pointer;
	static TLock _cs;
	virtual ~JMSingleton(){}
	JMSingleton(){}
public:
	static TPointer Instance(void * param = NULL);
	static void Reclaim(void);
};


/*********************************************************************************************************
*
* 智能指针部分
*
*********************************************************************************************************/

template<class T, class TLock, bool OwnerManaged = false>
struct JMRefCounter
{
	typedef LONG CounterType;
	TLock _lock;
private:
	volatile CounterType _counter;
	T * _obj;
public:
	JMRefCounter(T*p)
		:_obj(p),_counter(0)
	{}
	~JMRefCounter(){}

	CounterType AddRef();
	CounterType Release();
	BOOL HasReference();
	T * GetPtr()
	{
		MutexGuard<TLock> guard(_lock);
		return _obj;
	}
protected:
	virtual void Destroy()
	{
		T * p = _obj;
		_obj = NULL;
		if( p )
			delete p;
	}
};
template<class T, class TLock>
struct JMRefCounter<T,TLock,true>
{
	typedef typename T::CounterType CounterType;
	TLock _lock;
private:
	T * _obj;
public:
	JMRefCounter(T*p)
		:_obj(p)
	{}
	~JMRefCounter(){}

	CounterType AddRef()
	{
		MutexGuard<TLock> guard(_lock);
		return _obj->AddRef();
	}
	CounterType Release()
	{
		MutexGuard<TLock> guard(_lock);
		return _obj->Release();
	}
	BOOL HasReference()
	{
		MutexGuard<TLock> guard(_lock);
		return _obj->HasReference();
	}
	T * GetPtr()
	{
		MutexGuard<TLock> guard(_lock);
		return _obj;
	}
protected:
	virtual void Destroy()
	{
		T * p = _obj;
		_obj = NULL;
		if( p )
			delete p;
	}
};

// 兼容vEngine；里面的内存池
template<class T, class Alloc, class TLock = EmptyLock, class CM = JMRefCounter<T,TLock> >
class JMSptr
{
	CM * _counter;
	Alloc * _alloc;
	TLock _lock;
	void release();
	template<class A,class B,class C,class D>
	void copy( JMSptr<A,B,C,D> & right );
public:
	JMSptr(T * ptr,Alloc * g);
	template<class A,class B,class C,class D>
	JMSptr( JMSptr<A,B,C,D> & right );

	~JMSptr()
	{
		MutexGuard<TLock> guard(_lock);
		release();
	}

	T & operator * ()
	{
		MutexGuard<TLock> guard(_lock);
		return *_counter->GetPtr();
	}

	T * operator -> ()
	{
		MutexGuard<TLock> guard(_lock);
		return _counter->GetPtr();
	}

	//operator T * ()
	//{
	//	return _ptr;
	//}

	JMSptr& operator=(JMSptr& rigtht);

	template<class A,class B,class C,class D>
	JMSptr& operator = (JMSptr<A,B,C,D> & right);
};






template<typename T>
struct JMArrayAllocAdaptor
{
	typedef T * pointer;
	static pointer construct(size_t dem, void * p = NULL)
	{
		size_t size = dem;
		if( size > 0 )
			return new T[dem];
		return NULL;
	}
	static void destroy(pointer p){if(p) delete[] p;}
};

template<class T,class Alloc=JMArrayAllocAdaptor<T> >
class TArray
{
	typename Alloc::pointer _data;
	size_t _size;
public:
	TArray(size_t demension)
		: _size(demension)
	{
		_data = Alloc::construct(_size);
	}
	~TArray(){Alloc::destroy(_data);}
	T& operator[] (int index)
	{
		return _data[index];
	}

	size_t Size() const
	{
		return _size;
	}

	operator T*()
	{
		return _data;
	}
};

class CSimWinDll
{
	HINSTANCE _hInst;
public:
	CSimWinDll()
		:_hInst(0){}
	~CSimWinDll()
	{
		Free();
	}

	bool Load(LPCTSTR file)
	{
		_hInst = LoadLibrary(file);
		return _hInst != NULL;
	}

	void Free(void)
	{
		if( _hInst )
			FreeLibrary(_hInst);
		_hInst = NULL;
	}

	template<typename FuncT>
	FuncT 	GetProc(LPCSTR func_name)
	{
		return reinterpret_cast<FuncT>(GetProcAddress(_hInst,func_name));
	}
};

template<typename T>
static size_t jm_copy_s( T * pDes, size_t stElementNum, const T * pSrc, T temValue );

#if defined( __USE_JM_TOOL_TEMP )

#include "mini_tool.inl"

#endif


// 合并内存池
template<class Lock>
class TSMPool
{
	typedef typename MutexGuard<Lock> TGuard;
	Lock _lock;
public:
	__forceinline LPVOID Alloc(DWORD dwBytes);
	__forceinline VOID  Free(LPVOID pMem);
	__forceinline DWORD GetSize() { TGuard g(_lock); return m_dwCurrentSize; }

	TSMPool(DWORD dwMaxPoolSize=16*1024*1024)
	{
		m_dwMaxSize = dwMaxPoolSize;
		m_dwCurrentSize = 0;
		ZeroMemory(m_Pool, sizeof(m_Pool));
	}
	~TSMPool()
	{
		for( int n=0; n<16; n++ )
		{
			while( m_Pool[n].pFirst )
			{
				tagNode* pNode = m_Pool[n].pFirst;
				m_Pool[n].pFirst = m_Pool[n].pFirst->pNext;
				free(pNode);
			}
		}
	}

private:
	struct tagNode	// 内存块头描述
	{
		tagNode*	pNext;
		tagNode*	pPrev;
		INT			nIndex;
		DWORD		dwSize;
		DWORD		dwUseTime;
		DWORD		pMem[1];	// 实际内存空间
	};

	struct
	{
		tagNode*	pFirst;
		tagNode*	pLast;
	} m_Pool[16];

	DWORD m_dwMaxSize;		// 外部设定的最大允许空闲内存
	DWORD m_dwCurrentSize;	// 内存池中空闲内存总数

	// 垃圾收集
	__forceinline VOID GarbageCollect(DWORD dwExpectSize, DWORD dwUseTime);
	// 返回最匹配的大小
	__forceinline INT SizeToIndex(DWORD dwSize, DWORD& dwRealSize);
};


//-----------------------------------------------------------------------------
// 分配
//-----------------------------------------------------------------------------
template<class Lock>
LPVOID TSMPool<Lock>::Alloc(DWORD dwBytes)
{
	TGuard g(_lock);
	DWORD dwRealSize = 0;
	INT nIndex = SizeToIndex(dwBytes, dwRealSize);
	if( GT_INVALID != nIndex )
	{
		if( m_Pool[nIndex].pFirst )	// 池里有，就从池里分配
		{
			tagNode* pNode = m_Pool[nIndex].pFirst;
			m_Pool[nIndex].pFirst = m_Pool[nIndex].pFirst->pNext;
			if( m_Pool[nIndex].pFirst )
				m_Pool[nIndex].pFirst->pPrev = NULL;
			else
				m_Pool[nIndex].pLast = NULL;
			m_dwCurrentSize -= dwRealSize;
			++pNode->dwUseTime;
			return pNode->pMem;	
		}
	}

	tagNode* pNode = (tagNode*)malloc(dwRealSize + sizeof(tagNode) - sizeof(DWORD));
	if( !pNode )
		return NULL;

	pNode->nIndex = nIndex;
	pNode->dwSize = dwRealSize;
	pNode->pNext = NULL;
	pNode->pPrev = NULL;
	pNode->dwUseTime = 0;
	return pNode->pMem;	// 从实际内存中分配
}


//-----------------------------------------------------------------------------
// 释放
//-----------------------------------------------------------------------------
template<class Lock>
VOID TSMPool<Lock>::Free(LPVOID pMem)
{
	TGuard g(_lock);
	tagNode* pNode = (tagNode*)(((LPBYTE)pMem) - sizeof(tagNode) + sizeof(DWORD));
	if( GT_INVALID != pNode->nIndex )
	{
		if( pNode->dwSize + m_dwCurrentSize > m_dwMaxSize && pNode->dwUseTime > 0 )
			GarbageCollect(pNode->dwSize*2, pNode->dwUseTime);	// 垃圾收集

		if( pNode->dwSize + m_dwCurrentSize <= m_dwMaxSize ) // 内存池可以容纳
		{
			pNode->pPrev = NULL;
			pNode->pNext = m_Pool[pNode->nIndex].pFirst;
			if( m_Pool[pNode->nIndex].pFirst )
				m_Pool[pNode->nIndex].pFirst->pPrev = pNode;
			else
				m_Pool[pNode->nIndex].pLast = pNode;

			m_Pool[pNode->nIndex].pFirst = pNode;
			m_dwCurrentSize += pNode->dwSize;
			return;
		}
	}

	free(pNode);
}


//-----------------------------------------------------------------------------
// 垃圾收集
//-----------------------------------------------------------------------------
template<class Lock>
VOID TSMPool<Lock>::GarbageCollect(DWORD dwExpectSize, DWORD dwUseTime)
{
	DWORD dwFreeSize = 0;
	for(INT n=15; n>=0; --n)	// 从最大的开始回收
	{
		if( !m_Pool[n].pFirst )
			continue;

		tagNode* pNode = m_Pool[n].pLast; // 从最后开始释放，因为后面的Node使用次数少
		while( pNode )
		{
			tagNode* pTempNode = pNode;
			pNode = pNode->pPrev;
			if( pTempNode->dwUseTime < dwUseTime )	// 释放此节点
			{
				if( pNode )
					pNode->pNext = pTempNode->pNext;
				if( pTempNode->pNext )
					pTempNode->pNext->pPrev = pNode;
				if( m_Pool[n].pLast == pTempNode )
					m_Pool[n].pLast = pNode;
				if( m_Pool[n].pFirst == pTempNode )
					m_Pool[n].pFirst = pTempNode->pNext;

				m_dwCurrentSize -= pTempNode->dwSize;
				dwFreeSize += pTempNode->dwSize;
				free(pTempNode);
			}

			if( dwFreeSize >= dwExpectSize )
				return;
		}
	}
}


//-----------------------------------------------------------------------------
// 返回最匹配的大小
//-----------------------------------------------------------------------------
template<class Lock>
INT TSMPool<Lock>::SizeToIndex(DWORD dwSize, DWORD& dwRealSize)
{
	if( dwSize<=32 )		{ dwRealSize = 32;			return 0; }
	if( dwSize<=64 )		{ dwRealSize = 64;			return 1; }
	if( dwSize<=128 )		{ dwRealSize = 128;			return 2; }
	if( dwSize<=256 )		{ dwRealSize = 256;			return 3; }
	if( dwSize<=512 )		{ dwRealSize = 512;			return 4; }
	if( dwSize<=1024 )		{ dwRealSize = 1024;		return 5; }
	if( dwSize<=2*1024 )	{ dwRealSize = 2*1024;		return 6; }
	if( dwSize<=4*1024 )	{ dwRealSize = 4*1024;		return 7; }
	if( dwSize<=8*1024 )	{ dwRealSize = 8*1024;		return 8; }
	if( dwSize<=16*1024 )	{ dwRealSize = 16*1024;		return 9; }
	if( dwSize<=32*1024 )	{ dwRealSize = 32*1024;		return 10; }
	if( dwSize<=64*1024 )	{ dwRealSize = 64*1024;		return 11; }
	if( dwSize<=128*1024 )	{ dwRealSize = 128*1024;	return 12; }
	if( dwSize<=256*1024 )	{ dwRealSize = 256*1024;	return 13; }
	if( dwSize<=512*1024 )	{ dwRealSize = 512*1024;	return 14; }
	if( dwSize<=1024*1024 )	{ dwRealSize = 1024*1024;	return 15; }
	dwRealSize = dwSize;
	return GT_INVALID;
}


#ifdef _MSC_VER 

#define make_object(pAlloc,T, p, param )	\
	do\
	{\
		if( !P_VALID(pAlloc) )\
		{\
			p = NULL;\
			break;\
		}\
		LPVOID pData = pAlloc->Alloc(sizeof(T));\
		if( P_VALID(pData) )\
		{\
			p = (T*)pData;\
			p-> T::T param;\
		}\
		else\
			p = NULL;\
	}while(0)

#else 

#define make_object(pAlloc,T, p, param )	\
	do\
	{\
		if( !P_VALID(pAlloc) )\
		{\
			p = NULL;\
			break; \
		}\
		LPVOID pData = pAlloc->Alloc(sizeof(T));\
		if( P_VALID(pData) )\
		{\
			p = new(pData)T param;\
		}\
		else\
			p = NULL;\
	}while(0)

#endif

#define free_object(pAlloc,T,p)	\
	do\
	{\
		if( P_VALID(pAlloc) && P_VALID(p) )\
		{\
			p->~##T();\
			pAlloc->Free(p);\
		}\
		p = NULL;\
	}while(0)


/**
* \ingroup mythread
*
* 加入水位标的队列
*
* \version 1.0
* first version
* Syn 同步策略类或者结构体，只包含类型定义
*
* \date 2010-12-16
*
* \author Jason
*
*/
struct tagJSyn
{
	typedef CSCreatedLock Lock;
	typedef JEvent Signal;
};
template<typename T,class Syn,class Con/*template<typename Y,xx> class Con=queue<Y,xx>*/ >
class JQueue 
{
	volatile long _cur;
	long _low_lim;
	long _hight_lim;
	volatile LONG _terminate;

	typedef typename MutexGuard<typename Syn::Lock> TGuard;
	typedef typename Syn::Signal TSignal;
	typedef Con TCon;

	typename Syn::Lock 	_lock;
	TCon	_container;
	TSignal 	_hi_signal;
	TSignal	_lo_signal;

public:
	typedef typename T & reference ;
	
	JQueue(int low=5,int hight=200)
		:_low_lim(low),_hight_lim(hight),_cur(0),_terminate(0)
	{}
	virtual ~JQueue(){ clear(); }

	void wait_hight(int timeout)
	{
		if( _cur >= _hight_lim )
		{
			if( _hi_signal.wait(timeout) == 0 )
				_hi_signal.release();
		}
		::InterlockedIncrement(&_cur);
	}
	void notify_low()
	{
		if( _cur > _low_lim )
			_lo_signal.notify();
	}
	void wait_low(int timeout)
	{
		if( _cur <= _low_lim )
		{
			if( _lo_signal.wait(timeout) == 0 )
				_lo_signal.release();
		}
		if( _cur > 0 ) 
			::InterlockedDecrement(&_cur);
	}
	void wait_whenprocess(int timeout)
	{
		if( _cur <= _low_lim )
		{
			if( _lo_signal.wait(timeout) == 0 )
				_lo_signal.release();
		}
	}
	void notify_hight()
	{
		if( _cur < _hight_lim )
			_hi_signal.notify();
	}

	void push(const T & t, int timeout = 50)
	{ 
		if(terminate())
			return;
		notify_low();
		wait_hight(timeout);
		TGuard g(_lock);
		_container.push(t); 
	}
	bool pop(reference t ,int timeout = 50)
	{ 
		//if(terminate())
		//	return false;
		notify_hight();
		wait_low(timeout);
		TGuard g(_lock);
		if( _container.size() > 0 )
		{
			t = _container.front(); 
			_container.pop(); 
			return true;
		}
		return false; 
	}
	size_t size() 
	{
		TGuard g(_lock);
		return _container.size();
	}
	virtual void clear(){}

	void terminate(BOOL b){::InterlockedExchange((LPLONG)&_terminate,b);}
	bool terminate() const {return _terminate;}
};



}


template<class Result,class CLASS,class P1,class P2>
class TMemFun2
{	
public:
	explicit TMemFun2(Result (CLASS::*_Pm)(P1,P2))
		: _Pmemfun(_Pm)
	{	
	}

	Result operator()(CLASS *_Pleft,P1 p1,P2 p2) const
	{	
		return ((_Pleft->*_Pmemfun)(p1,p2));
	}

private:
	Result (CLASS::*_Pmemfun)(P1,P2);	
};

extern size_t jm_tscopy_s( LPTSTR pDes, size_t stElementNum, LPCTSTR pSrc );
extern size_t jm_tscopy_s( LPTSTR pDes, size_t stElementNum, LPCTSTR pSrc, size_t stNumTobeCopied );




#endif // mini_tool_h__





