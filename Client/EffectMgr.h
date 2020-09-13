#pragma once
#include "..\WorldDefine\msg_combat.h"
#include "CombatEvent.h"
#include "GUIEffectMgr.h"

namespace Cool3D
{
	class EffectTarget;
};
class Role;
class Player;

/** \class EffectMgr
	\brief 特效管理器
*/
class EffectMgr
{
public:
	/** \特效类型
	*/
	enum EEffectType
	{
		EET_Common,         // 普通特效
		EET_Role,           // 角色特效
		EET_Launch,         // 发射特效
		EET_Buff,           // Buff特效
		EET_BeAttack,       // 被攻击特效
		EET_Bone,			// 骨骼绑定的特效
		EET_UiSound,		// 界面特效声音元素
		EET_Win,				// 获得特效
		EET_VirtualRole,       // 虚拟角色特效（元神）
	};
	/** \特效的缩放类型
	*/
	enum EEffectScaleType
	{
		EEST_Null,           // 不缩放
		EEST_Y,				 // 根据高度缩放
		EEST_RoleScale,      // 使用角色的缩放来缩放特效
		EEST_XYZ,			 // 使用X,Y,Z的平均值缩放
		EEST_YOutOnly,       // 根据高度放大，不缩小
		EEST_XZ,			 // 使用X,Z的平均值缩放
	};

	/** \特效类型
	*/
	enum EBoneEffectType
	{
		EBET_Head,         // 头
		EBET_Chest,        // 胸
		EBET_Neck,         // 项链
		EBET_Mantle,       // 披风
		EBET_Wrist_R,        // 手腕
		EBET_Wrist_L,
		EBET_Waist,        // 腰部
		EBET_Leg_R,		   // 腿
		EBET_Leg_L,	
		EBET_Foot_R,		   // 脚
		EBET_Foot_L,
	};

private:
	/** \特效基本数据
	*/
	struct tagBaseEffectData
	{
		const EEffectType       eType;                // 特效类型
		SGAttachableEffectNode*	pSGNode;              // 特效Scene node
		DWORD				    dwBindRoleID;         // 特效绑定的角色ID（如果没有绑定角色，ID==GT_INVALID）
		bool                    bBindTagNode;		  // 是否绑定了挂载点或骨骼
		EEffectScaleType		eScaleType;           // 缩放类型
		bool                    bPullOnWater;         // 如果在水下，拉到水面以上
		tagBaseEffectData( EEffectType type ) : eType( type ), pSGNode( NULL ), dwBindRoleID( GT_INVALID ), bBindTagNode( false )
			, eScaleType( EEST_Null ), bPullOnWater( false ){}
	};
	typedef map<DWORD, tagBaseEffectData*> EffecMap;

	/** \角色特效
	*/
	struct tagRoleEffectData : public tagBaseEffectData
	{
		tagRoleEffectData() : tagBaseEffectData( EET_Role ) {}
	};

	/** \发射特效
	*/
	struct tagLaunchEffectData : public tagBaseEffectData
	{
		tagHitTargetEvent evtHitTarget;
		bool              bEventSended;
		tagLaunchEffectData() : tagBaseEffectData( EET_Launch ), bEventSended( false ) {}
	};

	/** \Buff特效
	*/
	struct tagBuffEffectData : public tagBaseEffectData
	{
		DWORD dwBuffID;								// BUFF ID
		tagBuffEffectData() : tagBaseEffectData( EET_Buff ), dwBuffID( GT_INVALID ) {}
	};

	// 获得特效
	struct tagWinEffectData : public tagBaseEffectData
	{
		DWORD dwSrcRoleID;
		DWORD dwTarRoleID;

		tagWinEffectData() : tagBaseEffectData( EET_Win ),dwSrcRoleID(GT_INVALID),dwTarRoleID(GT_INVALID) {}
	}; 

	/** \被攻击特效
	*/
	struct tagBeAttackEffectData : public tagBaseEffectData
	{
		DWORD					dwSrcRoleID;	//攻击者ID
		DWORD					dwTarRoleID;	//目标ID	
		EHitTargetCause			eCause;			//命中目标原因
		DWORD					dwMisc;			//如果是技能，为技能ID;如果是物品，物品ID
		DWORD					dwMisc2;		//管道序号
		DWORD					dwSerial;		//技能或使用物品序列号
		tagBeAttackEffectData() : tagBaseEffectData( EET_BeAttack ) {}
	};

	/** \虚拟角色特效
	*/
	struct tagVirtualRoleEffectData : public tagBaseEffectData
	{
		tagVirtualRoleEffectData() : tagBaseEffectData( EET_VirtualRole ) {}
	};


	/** \角色骨骼特效
	*/
	struct tagBoneEffectData : public tagBaseEffectData
	{
		EBoneEffectType		eType;		 // 位置
		bool				bIsBoneNode; 
		tstring				szBoneName;  //骨骼名称
		tagBoneEffectData() : tagBaseEffectData( EET_Bone ) {}
	};

	/** \UIsound特效
	*/
	struct tagUISoundEffectData : public tagBaseEffectData
	{
		GUIEffectMgr::EEffectType eSoundType;								// 声音类型
		tagUISoundEffectData() : tagBaseEffectData( EET_UiSound ), eSoundType( GUIEffectMgr::EET_Common ) {}
	};

	enum { EFFECT_START_ID = 1 };
	enum { MIN_EFFECT_NUM = 30, MAX_EFFECT_NUM = 150 };  // 根据特效细节以及当前特效数量限制某些特效播放

public:
	EffectMgr(void);
	~EffectMgr(void);

	/** \初始化，由战斗系统调用
	*/
	VOID Init();
	
	/** \释放数据，由战斗系统调用
	*/
	VOID Destroy();
	
	/** \更新特效，由战斗系统调用
	*/
	VOID Update();

	/** \播放特效，指定位置、缩放、旋转
	*/
	DWORD PlayEffect(const TCHAR* szPath, const Vector3& vPos, const Vector3& vScale, const Vector3& vRot );
	
	/** \在指定位置播放一个特效
	    \szFolderName-特效所在文件夹名称，如果为NULL，则认为szEffectName就是特效的路径
	*/
	DWORD PlayEffect(const Vector3& vPos,float yaw,const TCHAR* szEffectName, const Vector3* pSize=NULL, EEffectScaleType eScaleType = EEST_Null, const TCHAR* szFolderName = _T("combat") );
	
	/** \播放一个角色特效,返回特效ID,播放失败返回GT_INVALID
	*/
	DWORD PlayRoleEffect(DWORD dwRoleID,const TCHAR* szEffectName,const TCHAR* szTagNodeName=NULL, EEffectScaleType eScaleType = EEST_Y, const TCHAR* szFolderName = _T("combat"), const bool bPullOnWater = false );
	
	/** \播放一个虚拟角色特效,返回特效ID,播放失败返回GT_INVALID
	*/
	DWORD PlayVirtualEffect(DWORD dwRoleID,const TCHAR* szEffectName,const TCHAR* szTagNodeName=NULL, EEffectScaleType eScaleType = EEST_Y, const TCHAR* szFolderName = _T("combat"), const bool bPullOnWater = false );
	
	/** \停止绑定在虚拟角色上的所有特效
	*/
	VOID StopVirtualEffect(DWORD dwRoleID);

	/** \播放发射特效,返回特效ID
	*/
	DWORD PlayLaunchEffect(const tagHitTargetEvent& evt,const TCHAR* szEffectName,const TCHAR* szTagNodeName=NULL);
	
	/** \根据nNumEffect和szEffectBaseName随机播放一个被攻击特效，返回特效ID
	*/
	DWORD PlayBeAttackEffect(DWORD dwRoleID,const TCHAR* szEffectBaseName,const int nNumEffect, 
		DWORD dwSrcRoleID, EHitTargetCause eCause, DWORD dwMisc, DWORD dwSerial, EEffectScaleType eScaleType = EEST_YOutOnly );
	
	/** \根据dwRoleID,dwSrcRoleID和指定的技能停止播放角色所有该技能产生的被攻击特效
	*/
	VOID StopBeAttackEffect(DWORD dwRoleID, DWORD dwSrcRoleID, EHitTargetCause eCause, DWORD dwMisc, DWORD dwSerial);
	
	// 播放获得特效
	DWORD PlayWinEffect( DWORD dwFromRoleID, DWORD dwTargetID, const TCHAR* szEffectName );

	/** \播放一个Buff特效,返回特效ID
	*/
	VOID PlayBuffEffect(DWORD dwRoleID,DWORD dwBuffID, EEffectScaleType eScaleType = EEST_Y );
	
	/** \停止一个Buff特效
	*/
	VOID StopBuffEffect(DWORD dwRoleID,DWORD dwBuffID);
	
	/** \停止一个特效
	*/
	VOID StopEffect( const DWORD dwEffectID );

    /** \删除一个特效
    */
    VOID RemoveEffect(const DWORD dwEffectID);

	/** \删除场景中除帮定在本地玩家之外的所有特效
	*/
	VOID ClearSenceEffect();

	/** \显示\隐藏特效
	*/
	VOID ShowEffect( const DWORD dwEffectID, const bool bShow );
	
	/** \显示\隐藏绑定在角色身上的所有特效
	*/
	VOID ShowBindRoleEffect( const DWORD dwBindRoleID, const bool bShow );

	/** \获取容器中特效的数量
	*/
	size_t GetNumEffect() const { return m_mapEffect.size(); }
	/** \特效数量是否已经达到上限（有些不重要的特效就可以不播放）
	*/
	bool                 IsEffectNumFull();

	void PlayCacheBuff( DWORD dwRoleID );

	/*	\特效绑定到骨骼 add by zsu
	*/
	DWORD PlayRoleEquipEffect(DWORD dwRoleID,const TCHAR* szEffectName,const TCHAR* szBoneName=NULL, EBoneEffectType eType = EBET_Mantle,EEffectScaleType eScaleType = EEST_Y, const TCHAR* szFolderName = _T("combat"), const bool bPullOnWater = false );
	VOID StopRoleEquipEffect(DWORD dwRoleID,EBoneEffectType eType);
	const tstring GetGlowName( const TCHAR* szBaseName, const BYTE byVal ) const;

	/*  \ui特效系统中配合使用的声音特效文件
	*/
	DWORD PlayUiSoundEffect(const TCHAR* szEffectName,GUIEffectMgr::EEffectType eType,const TCHAR* szFolderName = _T("ui"));
	VOID StopUiSoundEffect(GUIEffectMgr::EEffectType eType);

	static EffectMgr* Inst();

private:

	SGAttachableEffectNode*		LoadSFXFile( const TCHAR* szEffectName, const TCHAR* szFolderName = _T("combat") );
	DWORD						AddEffect( tagBaseEffectData* pData, Role* pBindRole, const TCHAR* szTagNodeName );
	VOID						DeleteEffect( tagBaseEffectData* pData );    // 不会删除map中的节点
	VOID						OnCloseMap( tagGameEvent* );
	BOOL                        UpdataRoleEffect( tagBaseEffectData* pData );
	VOID                        UpdataTargetPath( tagLaunchEffectData* pData, const float fTimeDelta );
	VOID                        PullOnWater( tagBaseEffectData* pData );
	Vector3						GetRoleEffectScale( const EEffectScaleType eScaleType, Role* pRole );
	float 						GetEffectScale( const EEffectScaleType eScaleType, const Vector3& vSize );
	Vector3						GetBeAttackPos( Role* pRole );
	bool                        SFXCanMove( const TCHAR* szFileName );
	VOID                        UpdataEffectMatrix( tagBaseEffectData* pData, Role* pRole );

	DWORD						AddBoneEffect( tagBoneEffectData* pData, Role* pBindRole, const TCHAR* szBoneName );
	VOID                        UpdataBoneEffectMatrix( tagBaseEffectData* pData, Role* pRole );
	VOID						UpdateBeAtkEffectMsg(tagBeAttackEffectData* pData);
	VOID						UpdataVirtualEffectMatrix( tagBaseEffectData* pData, Player* pRole, const TCHAR* szBoneName );

	// 获得特效使用的相关位置
	Vector3						GetWinEffectPos( Role* pRole, bool bTar );
	void						UpdateTargetPath4Win( tagWinEffectData* pData, const float fTimeDelta );

private:
	TObjRef<GameFrameMgr>		m_pGameFrameMgr;
	TSFPTrunk<EffectMgr>		m_Trunk;
	EffecMap					m_mapEffect;				// 特效容器
	DWORD						m_dwCurEffectID;            // 记录当前特效ID

	// 在角色属性初始化为完成的时候的buff应该缓存，待属性初始化完毕要一起显示出来
	std::map<DWORD,DWORD>		m_mapNeedProcessBuff;
};
