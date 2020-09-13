#pragma once

#include "mutex.h"

namespace namepolicy
{

	//-------------------------------------------------------------------------------------------------------
	// 事件类型
	//-------------------------------------------------------------------------------------------------------
	enum  EEventType
	{
		EVT_NULL				=	-1,

		EVT_PlayerLoginOut		=   1,
	

		EVT_End,
	};

	//--------------------------------------------------------------------------------------------------------
	// 游戏内事件
	//--------------------------------------------------------------------------------------------------------
	template<class T>
	class EventMgr;

	class EventObj
	{
		template<class T> friend class EventMgr;

	private:
		EventObj(DWORD dwSender, VOID* pMsg, EEventType eType, DWORD dwSize)
		{
			// 初始化
			m_dwSender = dwSender;
			m_dwSize = dwSize;
			m_eType = eType;

			if( 0 == dwSize )
			{
				m_pMsg = NULL;
			}
			else
			{
				m_pMsg = new BYTE[dwSize];
				memcpy(m_pMsg, pMsg, dwSize);
			}
		}

		~EventObj() { SAFE_DEL_ARRAY(m_pMsg); }

		DWORD		GetSender() { return m_dwSender; }
		VOID*		GetEvent() { return m_pMsg; }
		DWORD		GetEventSize() { return m_dwSize; }
		EEventType	GetEventType() { return m_eType; }

	private:
		DWORD				m_dwSender;
		VOID*				m_pMsg;
		EEventType			m_eType;
		DWORD				m_dwSize;
	};

	//-------------------------------------------------------------------------------------------------------
	// 该类的成员函数只能在主线程调用（AddEvent除外）
	//-------------------------------------------------------------------------------------------------------
	template<class T>
	class EventMgr
	{
	public:
		typedef VOID (T::*EVENTMESSAGEFUNC)(DWORD dwSender, VOID* pMsg);
		typedef std::map<EEventType, EVENTMESSAGEFUNC>	EventFuncMap;

	public:
		BOOL Init();
		~EventMgr();

		VOID Update();		
		VOID AddEvent(DWORD dwSender, EEventType eType, DWORD dwSize, VOID* pMsg);

	protected:
		static VOID RegisterEventFunc(EEventType EventType, EVENTMESSAGEFUNC EventMessageFunc);

	private:
		TList<EventObj*>				m_listEventObj;	
		Mutex							m_Lock;
		static EventFuncMap				m_mapEventFunc;	// 事件触发器类型及对应的处理函数
	};

	template<class T>
	typename EventMgr<T>::EventFuncMap EventMgr<T>::m_mapEventFunc; 

	//-------------------------------------------------------------------------------------------------------
	// 注册事件处理函数
	//-------------------------------------------------------------------------------------------------------
	template<class T>
	VOID EventMgr<T>::RegisterEventFunc(EEventType eEventType, EVENTMESSAGEFUNC EventMessageFunc)
	{
		m_mapEventFunc.insert(make_pair(eEventType,	EventMessageFunc));
	}

	//-------------------------------------------------------------------------------------------------------
	// 事件管理类初始化
	//-------------------------------------------------------------------------------------------------------
	template<class T>
	BOOL EventMgr<T>::Init()
	{
		return TRUE;
	}

	//-------------------------------------------------------------------------------------------------------
	// 析构函数
	//-------------------------------------------------------------------------------------------------------
	template<class T>
	EventMgr<T>::~EventMgr()
	{
		EventObj* pEvent = m_listEventObj.PopFront();
		while( P_VALID(pEvent) )
		{
			SAFE_DEL(pEvent);
			pEvent = m_listEventObj.PopFront();
		}
	}


	//-------------------------------------------------------------------------------------------------------
	// 管理类事件处理
	//-------------------------------------------------------------------------------------------------------
	template<class T>
	VOID EventMgr<T>::Update()
	{
		EventObj* pOjb = m_listEventObj.PopFront();
		while( P_VALID(pOjb) )
		{
			EventFuncMap::iterator it = m_mapEventFunc.find(pOjb->GetEventType());
			if( it != m_mapEventFunc.end() )
			{
				EVENTMESSAGEFUNC handler = it->second;
				(((T*)this)->*handler)(pOjb->GetSender(), pOjb->GetEvent());
			}
			SAFE_DEL(pOjb);

			pOjb = m_listEventObj.PopFront();
		}
	}

	//-------------------------------------------------------------------------------------------------------
	// 加入事件
	//-------------------------------------------------------------------------------------------------------
	template<class T>
	VOID EventMgr<T>::AddEvent(DWORD dwSender, EEventType eType, DWORD dwSize, VOID* pMsg)
	{
		ASSERT( eType > EVT_NULL && eType < EVT_End );

		EventObj* pObj = new EventObj(dwSender, pMsg, eType, dwSize);

		m_Lock.Acquire();
		m_listEventObj.PushBack(pObj);
		m_Lock.Release();
	}

}