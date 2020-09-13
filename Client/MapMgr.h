#pragma once

/** \class MapMgr
	\brief 游戏场景管理器
*/

struct	PromptInfo
{
	POINT pt;
	tstring desc;
	DWORD mapID;
};
class MapMgr
{
public:
	enum{MAX_VIEW_DIST=8};
	typedef map<DWORD,list<PromptInfo>> MAPPROMPT;
public:
	MapMgr(void);
	~MapMgr(void);

	/** 初始化，进入游戏世界时调用
	*/
	void Init();
	/** 释放数据，离开游戏世界时调用
	*/
	void Destroy();

	void OpenMap(const TCHAR* szMapName,DWORD dwMapID,CameraBase* pBindCamera);
	void CloseMap();

	DWORD GetCurMapID(){ return m_curMapID;}
	const tstring& GetCurMapName() { return m_curMapName; }

	/** \远景视野
	*/
	void SetViewDist(int viewDist);
	int GetViewDist(){ return m_viewDist;}
	/** \环境细节
	*/
	void SetSceneDetailLevel(int level);
	int GetSceneDetailLevel(){ return m_sceneDetailLevel;}
	/** \太阳光晕
	*/
	void EnableSunFlare( const bool bEnable );

	void Update();
	DWORD OnGameEvent(tagGameEvent* pEvent);
	DWORD OnEnterLoadingFrame(tagGameEvent* pEvent);
	DWORD OnExitLoadingFrame(tagGameEvent* pEvent);

	/** 同步更新可区域
	*/
	void SyncInvalidViewZone();

	SceneGraph* GetSceneGraph(){ return m_pSG;}
	GameMap& GetGameMap(){ return m_gameMap;}
	NavMap*  GetNavMap() { return &m_navMap; }

	int		GetMapAreaIndex( const Vector3& vPos, const Vector3& vSize, EMapArea eAreaType = EMA_Null );
	DWORD	GetCurTriggerID() const;
	BOOL	GetTriggerPos(DWORD questSerial,Vector3& pos,AABBox& box);
	BOOL	GetTriggerPos(DWORD triggerID, Vector3& pos);
	static MapMgr* Inst();
	//! 设置视角2.5D
	void SetCamera25D( bool b25D );
private:
	void SetViewZoneSize();
	void UpdateTriggers();
	void UpdateMapArea();
	void ResetSceneFog( const bool bFade );
	/*
	*	注册对应地图中提示信息mapID是地图的ID，是CRC值，PromptInfo里面有对应的坐标和要显示的信息
	*/
	//void RegisterMapPrompt(DWORD mapID,PromptInfo mapInfo);
	void InitMapPrompt(DWORD mapID,float posX,float posZ,tstring desc);
	void PromptMapInit();
	void UpdateMapPrompt();
	BOOL IsInPromptArea(POINT pos);

	/*
	*  新手村，龙城FPS过低弹出提示修改默认设置
	*/
	void FPScheck();
	void InitFPScheckMap();
	void SendFixFPSsuggestionEvent();

private:
	int						m_viewDist;
	int                     m_sceneDetailLevel;
	bool                    m_bEnableSunFlare;
	HugeQuadTreeSG*			m_pSG;
	GameMap					m_gameMap;
	NavMap					m_navMap;
	DWORD					m_curMapID;
	tstring					m_curMapName;

	vector<HRGN>			m_triggerRgn;			//触发器区域
	vector<HRGN>			m_mapAreaRgn;			//地图区域
	bool					m_bInTrigger;			//玩家当前是否在触发器内
	int						m_curTriggerIndex;		//当前玩家所在触发器索引
	int						m_curMapAreaIndex;		//当前玩家所在地图区域索引

	//MAPPROMPT				m_mapPrompt;			//地图区域提示信息

	float					m_lastUpdateMapAreaTime;//最后一次更新MapArea的时间
	int						m_PromptTimeDelta;		//更新Prompt的时间标识位

	vector<DWORD>			m_vecFPSWatchmaps;		//监视FPS的地图
	int						m_nFPSmark;				//当前的FPS
	bool                    m_bFPSCanShow;			//FPS是否显示过的标志位
	float					m_FPStimeMark;
	bool					m_bIsLoading;
	vector<PromptInfo>		m_vecPrompt;			//地图区域提示信息

	TObjRef<NetSession>		m_pZoneSession;
	TObjRef<GameFrameMgr>	m_pGameFrameMgr;
	TSFPTrunk<MapMgr>		m_trunk;
};
