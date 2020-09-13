
/*********************************************************************************************************
 *
 * 单件部分
 *
 *********************************************************************************************************/
template<class T, class adaptor, class TLock>
TLock JMSingleton<T,adaptor,TLock>::_cs;
template<class T, class adaptor, class TLock>
typename JMSingleton<T,adaptor,TLock>::TPointer JMSingleton<T,adaptor,TLock>::_pointer = NULL;


template<class T, class adaptor, class TLock>
typename JMSingleton<T,adaptor,TLock>::TPointer JMSingleton<T,adaptor,TLock>::Instance(void * param)
{
	if(!_pointer)
	{
		MutexGuard<TLock> guard( _cs );
		if(!_pointer)
		{
			_pointer = adaptor::construct(param);
		}
	}
	return _pointer;
}

template<class T, class adaptor, class TLock>
void JMSingleton<T,adaptor,TLock>::Reclaim(void)
{
	if( _pointer )
	{
		MutexGuard<TLock> guard( _cs );
		TPointer p = _pointer;
		_pointer = NULL;
		if( p )
			adaptor::destroy  (p);
	}
}



/*********************************************************************************************************
*
* 智能指针部分
*
*********************************************************************************************************/

template<class T, class TLock, bool OwnerManaged>
typename JMRefCounter<T,TLock,OwnerManaged>::CounterType JMRefCounter<T,TLock,OwnerManaged>::AddRef()
{
	return InterlockedIncrement(&_counter);
}

template<class T, class TLock, bool OwnerManaged>
typename JMRefCounter<T,TLock,OwnerManaged>::CounterType JMRefCounter<T,TLock,OwnerManaged>::Release()
{
	InterlockedDecrement(&_counter);
	if( _counter <= 0 )
	{
		MutexGuard<TLock> guard(_lock);
		_counter = 0;
		Destroy();
	}
	return _counter;
}

template<class T, class TLock, bool OwnerManaged>
BOOL JMRefCounter<T,TLock,OwnerManaged>::HasReference()
{
	return 0 < _counter;
}


template<class T, class Alloc, class TLock, class CM >
void JMSptr<T,Alloc,TLock,CM>::release()
{
	if( _counter )
	{
		_counter->Release();
		if( !_counter->HasReference() )
		{
			_counter->~CM();
			_alloc->Free(_counter);
			_counter = NULL;
		}
	}
}

template<class T, class Alloc, class TLock, class CM >
template<class A,class B,class C,class D>
void JMSptr<T,Alloc,TLock,CM>::copy( JMSptr<A,B,C,D> & right )
{
	release();
	_alloc		= right . _alloc;
	_counter	= right . _counter;
	if( _counter )
		_counter->AddRef();
}

template<class T, class Alloc, class TLock, class CM >
JMSptr<T,Alloc,TLock,CM>::JMSptr(T * ptr,Alloc * g)
:_counter(0),_alloc(g)
{
	if( ptr )
	{
		void * p = _alloc->Alloc(sizeof(CM));
		_counter = ::new (p) CM (ptr);
		_counter->AddRef();
	}
}

template<class T, class Alloc, class TLock, class CM >
template<class A,class B,class C,class D>
JMSptr<T,Alloc,TLock,CM>::JMSptr( JMSptr<A,B,C,D> & right )
:_counter(NULL),_alloc(0)
{
	copy(right);
}

template<class T, class Alloc, class TLock, class CM >
template<class A,class B,class C,class D>
JMSptr<T,Alloc,TLock,CM>& JMSptr<T,Alloc,TLock,CM>::operator = (JMSptr<A,B,C,D> & right)
{
	MutexGuard<TLock> guard(_lock);
	release();
	copy( right );
	return *this;
}

template<class T, class Alloc, class TLock, class CM >
JMSptr<T,Alloc,TLock,CM>& JMSptr<T,Alloc,TLock,CM>::operator = (JMSptr<T,Alloc,TLock,CM> & right)
{
	MutexGuard<TLock> guard(_lock);
	release();
	copy( right );
	return *this;
}



template<typename T>
static size_t jm_copy_s( T * pDes, size_t stElementNum, const T * pSrc, T temValue )
{
	if( !P_VALID(pDes) || stElementNum < 1 || !P_VALID(pSrc) )
		return 0;
	size_t re = 0;
	while( (*pDes++ = *pSrc++) != temValue && --stElementNum > 0 )
		++re;
	return re;
}

