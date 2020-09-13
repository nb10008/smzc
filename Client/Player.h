#pragma once
#include "role.h"
#include "..\WorldDefine\ItemDefine.h"
#include "AvatarEquipNode.h"
#include "AvatarAniPlayer.h"
#include "RoleTitleProtoData.h"
#include "ActionState.h"

class FSM_RP;
class Pet;
struct tagRemoteRoleFabaoInfo;
struct tagNS_GetRemoteRoleState;

/** \class Player
	\brief 玩家角色
*/
class Player :	public Role
{
public:
	Player(void);
	virtual ~Player(void);

	//--Role-------------------------------------------------------------------
	virtual void InitFSM();
	virtual void AttachSceneNodeToSG(SceneGraph* pSG);
	virtual void DetachSceneNodeFromSG(SceneGraph* pSG);
	virtual void Update();
	virtual void OnNetCmd(tagNetCmd* pNetCmd);
	virtual void OnGameEvent(tagGameEvent* pGameEvent);
	virtual SceneNode* GetSceneNode() { return m_pAvatarNode; }
	virtual SceneNode* GetRealSceneNode() { if(P_VALID(m_pEntityNode)) return m_pEntityNode; else return m_pAvatarNode; }
	virtual void GetHeadPos(Vector3& out);
	virtual Vector3 GetRoleSize();
	virtual bool IsDead();
	virtual bool IsAttributeInited() const;
	virtual void Hide( const bool bHide );
	virtual bool IsHide() const { return m_bHide; }
	virtual VOID FaceTo(const Vector3& pos,BOOL bFaceRound=TRUE);
	virtual VOID FaceBack(void);
	virtual void ReplayLowerAni();
	virtual void SetTransparent(float alpha);

	//--动作播放----------------------------------------------------------------
	AvatarAniPlayer* GetAniPlayer()	{ return &m_aniPlayer;}
	NodeAnimationCtrl* GetAniCtrl();
	AnimationCtrl*	GetTranAniCtrl(); // 变形
	void  SetMount(Pet *pPet);
	void  SetPos(const Vector3& pos);
	void  SetYaw(float angle,bool bShade=true);
	float GetMoveSpeed();
	float GetMoveYaw(){ return m_moveYaw;}
	void SetMoveYaw(float yaw){ m_moveYaw=yaw;}

	bool TransformIsTrackEnd(const TCHAR* szTrackName);
	void TransformPlayTrack(const TCHAR* szTrackName,bool loop,bool replay=false,float timePos=0.0f);

	//--角色可视属性及装备----------------------------------------------------
	/** \加载默认的avatar
	*/
	void LoadDefaultAvatar();
	/** \加载指定的avatar，pEpuip=NULL则只加载att
	*/
	void LoadAvatar(const tagAvatarAtt& att, const tagAvatarEquip& ep, const tagDisplaySet& displaySet);
	/** \更新avtatar，pEpuip=NULL则只更新att
	*/
	void UpdateAvatar(const tagAvatarAtt& att, const tagAvatarEquip& ep, SceneGraph* pSG);
	/** \换单个装备
	*/
	void SetAvatarEquip( const EAvatarElement eAvaEnt, const tagEquipDisplay& ed );
	/** \获得单个装备
	*/
	const tagEquipDisplay& GetAvatarEquip( const EAvatarElement eAvaEnt ) const;
	/** \获得角色可视属性数据
	*/
	const tagAvatarAtt& GetAvatarAtt() const;
	/** \拿出/收回武器
	*/
	void HandUpWeapon( const bool bHandUp );
	/** \获取主手武器类型
	*/
	EItemTypeEx GetRWeaponType() const;
	/** \获取附手武器类型
	*/
	EItemTypeEx GetLWeaponType() const;
	/** \展开/关闭所有装备(播放变形动画)
	*/
	void OpenAllEquip( const bool bOpen );
	/** \所有装备是否已打开
	*/
	bool IsAllEquipOpened();
	/** \所有装备是否已关闭
	*/
	bool IsAllEquipClosed();
	/** \更新龙魂特效
	*/
	void UpdataLongHunEffect( const EAvatarElement eAvaEnt, const BYTE byEquipEffect );
	/** \更新套装特效
	*/
	void UpdataSuitEffect( const DWORD dwSuitID );

	// 法宝特效
	VOID PlayTrumpEffect( const tagRemoteRoleFabaoInfo& trump );
	VOID PlayTrumpEffect(INT nLvChange = 0);
	VOID ClearTrumpEffect();

	// 时装特效
	VOID PlayFashionEffect( BYTE byQizhi );

	//--玩家属性相关-----------------------------------------------------------
	/** \初始化玩家属性
	*/
	void InitAttribute(tagNS_GetRemoteRoleState* pCmd);
	/** \设置玩家姓名
	*/
	void SetPlayerName(LPCTSTR szName, DWORD dwNameCrc = GT_INVALID);

    //! 设置玩家称号
    void SetPlayerTitle(DWORD dwTitleID);

	// 设置角色摊位等级
	void SetStallLevel(BYTE byLevel);

	// 设置玩家仙界修行点
	void SetMiraclePrcNum(INT16 nNum);

    //! 设置玩家绑定的复活点
    void SetRebornPoint(DWORD dwMapID, Vector3& dwRebornPoint);

    //! 设置玩家可记录传送点
    void SetTransportPoint(DWORD dwMapID, Vector3& dwPoint);
    //! 得到当前玩家设置的可记录传送点的地图名
    tstring GetTransportPointName();
    //! 得到当前玩家设置的可记录传送点的坐标
    Vector3 GetTransportPoint();

	/** \设置玩家职业
	*/
	virtual void SetPlayerClass(EClassType eClass);
    void SetPlayerClassEx(EClassTypeEx eClassEx);
    /** \获得玩家职业
    */
    EClassType GetPlayerClass() { return m_eClass; }
    EClassTypeEx GetPlayerClassEx() { return m_eClassEx; }

	/** \设置玩家神职
	*/
	void SetPlayerClergy(DWORD dwID);
	/** \获得玩家神职
	*/
	DWORD GetPlayerClergy() { return m_dwClergyID; }

	// 获得摊位等级
	BYTE GetPlayerStallLevel() { return m_byStallLevel; }


    /** \设置玩家配偶
    */
    void SetPlayerLoverID(DWORD dwLoverID);
    void SetPlayerIsHasWedding(BOOL bWedding) { m_bHaveWedding = bWedding; }
    /** \获得玩家配偶,无则返回GT_INVALID
    */
    DWORD GetPlayerLoverID() { return m_dwLoverID; }
    BOOL IsHaveWedding() { return m_bHaveWedding; }

	/** \获取玩家性别
		\玩家性别未去到则返回0xFF表示未知
	*/
	BYTE GetPlayerSex(){if(P_VALID(m_pAvatarNode))return m_pAvatarNode->GetAtt().bySex; return 0xFF;}
	/** \设置角色状态
	*/
	void SetRoleState(DWORD dwValue);
	/** \设置角色状态
	*/
	void SetRoleState(ERoleState eType, bool bFlag);
	/** \获取角色状态
	*/
	DWORD GetRoleState(){ return m_roleState;}
	/** \获取角色是否进入某状态
	*/
	bool GetRoleState(ERoleState eType){return (m_roleState&eType) != 0;}

    //! 获取称号
    DWORD GetTitleID() { return m_dwTitleID; }
    tstring GetTitleName() {return m_strTitleName; }
    
    //! 获取当前绑定的复活点名称
    tstring GetRebornPoinName();

	/** \获取PK状态
	*/
	ERolePKState GetPKState(){ return m_ePKState;}
	ERolePKState GetPKLastState(){ return m_ePKLastState;}
	/** \是否可以水上行走
	*/
	bool CanWaterWalk() { return 0!=(m_roleState&ERS_WaterWalk); }
	/** \是否在帮
	*/
	bool IsInGuild() const {return P_VALID(m_dwGuildID);}
	/** \是否有家族
	*/
	bool IsInFamily() const {return P_VALID(m_dwFamilyID);}
    /** \显示设置
    */
	const tagDisplaySet& GetDisplaySet();
	void SetDisplaySet( const tagDisplaySet& ds );

	/** \设置玩家帮派ID
	*/
	void SetRoleGuildID( DWORD dwID ){m_dwGuildID = dwID;}
	/** \设置玩家帮派职位
	*/
	void SetRoleGuildPos( INT8 n8Pos ){m_n8GuildPos = n8Pos;}
	/** \获取玩家帮派ID
	*/
	DWORD GetRoleGuildID(){return m_dwGuildID;}
	/** \获取玩家帮派职位
	*/
	INT8 GetRoleGuildPos(){return m_n8GuildPos;}

	/** \设置玩家家族ID
	*/
	void SetRoleFamilyID( DWORD dwID ){m_dwFamilyID = dwID;}
	/** \获取玩家家族ID
	*/
	DWORD GetRoleFamilyID(){return m_dwFamilyID;}
	/** \设置玩家是否是族长
	*/
	void SetRoleFamilyLeader( bool b ){ m_bFamilyLeader = b;}
	/** \获取玩家是否是族长
	*/
	bool IsRoleFamilyLeader(){return m_bFamilyLeader;}

	//--效果相关-----------------------------------------------------------
	/** \显示/隐藏武器
	*/
	void HideWeapon(bool bHide);

	/** \显示/隐藏副手武器
	*/
	void HideLWeapon(bool bHide);

	// 显示/隐藏武器附属模型
	void HideWeaponPertain(bool bHide);

	// 播放/停止播放套装特效
	void PlaySuitEffect(bool bPlay);

	virtual bool IsMoveState();


	// 职业判断	
	BOOL VocationCheck(DWORD dwVocationLimit);

	//--
	virtual bool IsReadyState();
	virtual bool IsStandState();
	virtual bool IsStyleActionState();
	virtual bool IsIdleState();
	virtual bool IsSkillState();
	virtual EActionState GetActionState();

	Pet*	GetMount(void)	{ return m_pMount; }
    void SwitchMountPitch(bool bPitchEnabled);

	VOID SetTransSkinIndex(BYTE byIndex);

	/**	\计算骑乘时屁股距离地面的高度
	*/
	float GetBunsHeight(void);

	/** \发送显示/隐藏摊位
	*/
	void SendEventStallAppear(bool bAppear, BYTE byStallLevel = 1);

	// 得到师傅ID
	DWORD GetMasterID() { return m_dwMasterID; }

	// 设置师傅ID
	VOID SetMasterID( DWORD dwMasterID );

	/** 角色身上的每件装备都可以设置最多3个 扩展特效，主要是为了在创建角色时候展示高级装备
	*/
	void SetEquipEffectEx( EAvatarElement eElementType, tstring strEffectName, INT index );
	// 设置组队增益Buffer标志
	void DisableTeamBufferFlag() { m_bLastTeamBuf = FALSE; m_bCurTeamBuf = FALSE; }
	BOOL GetTeamBufferFlag() { return m_bCurTeamBuf; }
	// 设置摆摊模型时的相关状态
	void SetBaiTanMode( bool bFlag );

	//  [8/4/2010 zhangsu] 骨骼特效
	void LoadEquipEffect(const tagAvatarEquip& ep);
	void LoadSingleEquipEffect(const tagEquipDisplay& ep);
	void RemoveSingleEquipEffect(EAvatarElement eType);

	//  [8/9/2010 zhangsu] 获得战场状态
	EBuffCombatState GetCombatAreaState();

	FSM_RP*	 GetFSM();
	tstring GetTransformSkinPath(const TCHAR* pModelName, int index );
	void GetFeetPos( Vector3& out );
	Vector3 GetVRPos(float fScale = 1.0f);
	Vector3 GetFaceTo();
	//获得玩家元神buff等级(返回GT_INVALID表明没有元神buff)
	DWORD GetHolyBuffID();
	//角色是否出于元神觉醒状态
	BOOL IsPlayerHolyAwake();

	void LoadTransformModel(const TCHAR* szSklFileName, const TCHAR* szModelName);
	void LoadTransformSkin(const TCHAR* szFileName);
	void UnloadTransformModel( BOOL IsVirtualRole = FALSE, DWORD dwRoleID = GT_INVALID );
		
protected:
	/** \发送状态改变游戏事件
	*/
	void SendEventRoleStateChange(const DWORD dwOldState);
	/**	\计算骑乘时的世界矩阵
	*/
	bool CalRideMat(Pet *pMount,Matrix4& outRide,UINT index);
	/** \处理消息点
	*/
	virtual void OnMsgCode( const DWORD dwMsgCode );
	// 更新组队增益Buffer
	void UpdateTeamBuffer();

	/*	\特殊特效，目前仅为飞升特效
	*/
	void UpdatePlayerEffect();
protected:
	AvatarEquipNode*	m_pAvatarNode;		//布娃娃结点
	AvatarAniPlayer		m_aniPlayer;		//动画播放器

	EntityNode*			m_pEntityNode;		//变形后的模型
	tstring				m_strTransformModelName; // 变形后模型名称
	float				m_fTransformModelSfxScale;		// 变形后模型特效缩放系数

	tstring				m_curTranfromTrack;	// 变形模型当前播放的track

	float				m_moveYaw;			//角色移动方向

	DWORD				m_dwRoleNameCrc;	//玩家名称Crc值
	EClassType			m_eClass;			//专精职业
	EClassTypeEx		m_eClassEx;			//英雄职业
	DWORD				m_roleState;		//服务器传过来的玩家状态
	ERolePKState		m_ePKState;			//PK当前状态
	ERolePKState		m_ePKLastState;		//PK上一个状态

	bool                m_bAttributeInited;	//是否已从服务器获得属性
	bool                m_bHide;

	Pet*				m_pMount;			//坐骑
	int					m_bunsBoneIndex;	//臀部骨骼索引

	DWORD				m_dwGuildID;		//所属帮派ID
	INT8				m_n8GuildPos;		//所属帮派职位

	DWORD				m_dwFamilyID;		//所属家族ID
	bool				m_bFamilyLeader;	//是否为族长

    DWORD               m_dwTitleID;        //当前称号id(无效为GT_INVALID)
    tstring             m_strTitleName;     //当前称号名,无效为空字符串

    DWORD               m_dwRebornMapID;    //当前绑定的复活点mapId(无效为GT_INVALID)
    Vector3             m_vRebornPoint;     //复活点坐标

    DWORD               m_dwTransportID;    // 当前记录传送符绑定地图ID
    Vector3             m_vTransportPoint;  // 当前记录传送符绑定地图坐标

    DWORD			    m_dwLoverID;        // 爱人对应的roleid 没结婚则为GT_INVALID
    BOOL			    m_bHaveWedding;     // 是否举行过婚礼
	
	BYTE				m_byStallLevel;		// 摊位等级

	DWORD				m_dwMasterID;		// 师傅ID, default = GT_INVALID
	BOOL				m_bCurTeamBuf;		// 当前是否有组队增益Buffer
	BOOL				m_bLastTeamBuf;		// 是否有组队增益Buffer

	DWORD				m_dwSoarEffectTime;		// 飞升画面播放间隔时间
	DWORD				m_dwCurrSoarTime;		// 当前飞升间隔时间

	DWORD               m_dwClergyID;       // 神职ID
	DWORD               m_dwClergyEffect;   // 神职特效
private:
	FSM_RP*				m_pFSM;
	BOOL					m_bFSMIsInit;

	float				m_lastYaw;
	float				m_lastPitch;

protected:
	tagAvatarAtt		m_tempAtt;
	tagAvatarEquip		m_tempEp; 
	SceneGraph*			m_pTempSG;
	DWORD				m_dwTime;

	DECL_RTTI(Player);
};
