#pragma once
#include "NavColliderPublicDef.h"
#include "NavCollider_Jump.h"

namespace WorldBase
{
	class NavCollider;
	class NavCarrierNode;
	/** \class NavCollider_HitFly
		\brief 击飞碰撞检测器
	*/
	class WORLD_BASE_API NavCollider_HitFly
	{
		enum EState						//状态
		{
			EState_None,				//未进行任何操作
			EState_Jump,				//跳跃
			EState_VDrop,				//垂直掉落
			EState_SlideXY,				//水平滑行
			EState_HitFlyStop,
		};

		struct tagInput
		{
			Vector3 roleSize;
			float	g;
			float	cdTimeOut;
			bool	bTrnOnly;
			bool	bIgnoreCarrier;
			float	fXZSpeed;
			float	fYSpeed;
			Vector3 xzDir;
			DWORD	cdBoxFlag;		//碰撞标志位
		};

	public:
		NavCollider_HitFly(void);
		~NavCollider_HitFly(void);

		/** 初始化
		*/
		void Init(const Vector3& startPos,const Vector3& xzDir,const Vector3& roleSize,float xzSpeed,float ySpeed,float g=1200.0f,float cdTimeOut=60.0f,bool bTrnOnly=false,bool bIgnoreCarrier=false);
		/** 击飞
		*/
		ENavResult Update(NavCollider* pCollider,const Vector3& startPos,float startTime,float deltaTime,BitMap* pSpecCangoMap,Vector3& outPos,float& endTime,DWORD& dwCarrierObjID,bool bOnWaterCheck=false);

	private:
		tagInput			m_input;
		EState				m_state;
		NavCollider_Jump	m_jumpCollider;
		float				m_fStartTime;
		float				m_fYSpeed;
		float				m_fXZSpeed;	// 即时速度
		float				m_fOriXZSpeed;// 初始速度
		Vector3				m_CurPos;
		Vector3				m_SlideStartPos;

		class Collider;

		friend class Collider;

		Collider* m_pCollider;
	};
}//namespace WorldBase