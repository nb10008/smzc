

/* fuck c plus plus template */


#ifdef _MSC_VER
	/* for visual c++ */
	#define __JAM_MEM_PREFIX
	#define __JAM_MEM_EXTERN					static
#else
	/* for gcc */
	#define __JAM_MEM_PREFIX					Memory::
	#define __JAM_MEM_EXTERN
#endif


#define JAM_STD_NO_CONSTRUCT(type)				template <> __JAM_MEM_EXTERN type* __JAM_MEM_PREFIX _construct<type>(void* p) { return (type*)p; }
#define JAM_STD_INT_NO_CONSTRUCT(type)			JAM_STD_NO_CONSTRUCT(type); JAM_STD_NO_CONSTRUCT(unsigned type)

#define JAM_STD_NO_ARRAY_CONSTRUCT(type)		template <> __JAM_MEM_EXTERN type* __JAM_MEM_PREFIX _constructArray<type>(void* p, size_t size) { return (type*)p; }
#define JAM_STD_INT_NO_ARRAY_CONSTRUCT(type)	JAM_STD_NO_ARRAY_CONSTRUCT(type); JAM_STD_NO_ARRAY_CONSTRUCT(unsigned type)

#define JAM_STD_NO_DESTRUCT(type)				template <> __JAM_MEM_EXTERN void __JAM_MEM_PREFIX _destruct<type>(type* p) {}
#define JAM_STD_INT_NO_DESTRUCT(type)			JAM_STD_NO_DESTRUCT(type); JAM_STD_NO_DESTRUCT(unsigned type)

#define JAM_STD_NO_ARRAY_DESTRUCT(type)			template <> __JAM_MEM_EXTERN void __JAM_MEM_PREFIX _destructArray<type>(type* p, size_t size) {}
#define JAM_STD_INT_NO_ARRAY_DESTRUCT(type)		JAM_STD_NO_ARRAY_DESTRUCT(type); JAM_STD_NO_ARRAY_DESTRUCT(unsigned type)


#define JAM_STD_NO_TYPE							JAM_STD_NO_CONSTRUCT(void);	JAM_STD_NO_CONSTRUCT(bool);	 JAM_STD_NO_CONSTRUCT(float);		\
												JAM_STD_NO_CONSTRUCT(double); JAM_STD_NO_CONSTRUCT(long double);							\
												JAM_STD_NO_DESTRUCT(void); JAM_STD_NO_DESTRUCT(bool); JAM_STD_NO_DESTRUCT(float);			\
												JAM_STD_NO_DESTRUCT(double); JAM_STD_NO_DESTRUCT(long double);

#define JAM_STD_NO_INT							JAM_STD_INT_NO_CONSTRUCT(char); JAM_STD_INT_NO_CONSTRUCT(int); JAM_STD_INT_NO_CONSTRUCT(short);		\
												JAM_STD_INT_NO_CONSTRUCT(long); JAM_STD_INT_NO_CONSTRUCT(long long);								\
												JAM_STD_INT_NO_DESTRUCT(char); JAM_STD_INT_NO_DESTRUCT(int); JAM_STD_INT_NO_DESTRUCT(short);		\
												JAM_STD_INT_NO_DESTRUCT(long); JAM_STD_INT_NO_DESTRUCT(long long);

#define JAM_STD_NO_TYPE_ARRAY					JAM_STD_NO_ARRAY_CONSTRUCT(void); JAM_STD_NO_ARRAY_CONSTRUCT(bool); JAM_STD_NO_ARRAY_CONSTRUCT(float);	\
												JAM_STD_NO_ARRAY_CONSTRUCT(double); JAM_STD_NO_ARRAY_CONSTRUCT(long double);							\
												JAM_STD_NO_ARRAY_DESTRUCT(void); JAM_STD_NO_ARRAY_DESTRUCT(bool); JAM_STD_NO_ARRAY_DESTRUCT(float);		\
												JAM_STD_NO_ARRAY_DESTRUCT(double); JAM_STD_NO_ARRAY_DESTRUCT(long double);

#define JAM_STD_NO_INT_ARRAY					JAM_STD_INT_NO_ARRAY_CONSTRUCT(char); JAM_STD_INT_NO_ARRAY_CONSTRUCT(int); JAM_STD_INT_NO_ARRAY_CONSTRUCT(short);	\
												JAM_STD_INT_NO_ARRAY_CONSTRUCT(long); JAM_STD_INT_NO_ARRAY_CONSTRUCT(long long);									\
												JAM_STD_INT_NO_ARRAY_DESTRUCT(char); JAM_STD_INT_NO_ARRAY_DESTRUCT(int); JAM_STD_INT_NO_ARRAY_DESTRUCT(short);		\
												JAM_STD_INT_NO_ARRAY_DESTRUCT(long); JAM_STD_INT_NO_ARRAY_DESTRUCT(long long);


#define JAM_NO_STD								JAM_STD_NO_TYPE				\
												JAM_STD_NO_INT				\
												JAM_STD_NO_TYPE_ARRAY		\
												JAM_STD_NO_INT_ARRAY



class Memory
{
public:
	//  构造对象
    template <typename Ty >
    static Ty* _construct(void* p)
	{
		return new (p) Ty;
	}

    template <typename Ty >
    static Ty* _constructArray(void* p, size_t size)
	{
		for(size_t i = 0; i < size; i++)
		{
			new ((Ty*)p + i) Ty;
		}
		return (Ty*)p;
	}

    //  析构对象
    template <typename Ty >
    static void _destruct(Ty* p)
	{
		p->~Ty();
	}

    template <typename Ty >
    static void _destructArray(Ty* p, size_t size)
	{
		for(size_t i = 0; i < size; ++i)
		{
			(p + i)->~Ty();
		}
	}

	//  辅助函数，外部请不要调用
    template <typename Ty >
    static Ty* __newArray(size_t size)
	{
		void* p = jam_malloc(sizeof(Ty) * size + sizeof(int32_t));
		int32_t* c = (int32_t*)p;
		p = c + 1;
		*c = (int32_t)size;
		return _constructArray<Ty>(p, size);
	}

    template <typename Ty >
    static void __delete(Ty* p)
	{
		_destruct<Ty>(p);
		jam_free(p);
	}

    template <typename Ty >
    static void __deleteArray(Ty* p)
	{
		int32_t* c = (int32_t*)p;
		--c;
		_destructArray<Ty>(p, *c);
		jam_free(c);
	}


#ifdef _MSC_VER
	JAM_NO_STD
#endif


private:
	Memory(){}
	~Memory(){}
};


#ifndef _MSC_VER
	JAM_NO_STD
#endif



#define New(type)						new (jam_malloc(sizeof(type))) type
#define NewArray(type, size)			JamSDK::Memory::__newArray<type>(size)
#define Delete(ptr)						if((ptr)) { JamSDK::Memory::__delete(ptr); (ptr) = 0; }
#define DeleteArray(ptr)				if((ptr)) { JamSDK::Memory::__deleteArray(ptr); (ptr) = 0; }

