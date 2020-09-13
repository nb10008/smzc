#pragma once

/** 旋转镜头游戏事件
*/
struct tagRotateCameraEvent : public tagGameEvent
{
	tagRotateCameraEvent(LPCTSTR szEventName, GameFrame* pSenderFrame) : tagGameEvent(szEventName,pSenderFrame)
	{}
};

class Role;
/**	\class ClientCamera 
	\brief 客户端镜头控制
*/

/** 摄像机轨迹动画 */
//@{
struct TCamPathNode {
	float mSpeed;		/**< 摄像机移动速度 */
	Vector3 mPos;		/**< 路点位置 */
	Quaternion mRot;	/**< 摄像机移动到该节点时的朝向 */
	bool mUseShortestPath;/**< 按最短弧旋转 */
	TCamPathNode():mSpeed(0),mUseShortestPath(true){}
};

enum ECamreaControlState
{
	ECCS_Null,
	ECCS_Roam,			// 漫游状态
	ECCS_UserContril,	// 用户输入控制状态
};
class ClientCamera : public CameraEuler
{
public:
	VOID SetDefault(FLOAT fZFar);
	VOID SetFov(float deg);

	VOID Update(Role *pLookRole,NavMap *pNavMap);
	
	VOID PlayQuake();	//! 镜头震动

	VOID EnableFlyMode(bool enable)	{m_bFly=enable;}
	bool IsFly() { return m_bFly; }

    VOID SetHeightRate(FLOAT rate) { 
        m_HeightScaleRate = rate;
    }
	VOID SetStopUpdate(bool bStop)	{StopUpdate=bStop;}
	bool IsStopUpdate(){ return StopUpdate;}
	
	// ！摄像机漫游
	// ！改变摄像机的控制模式
	VOID SetCamCtrlState(ECamreaControlState eState) {m_eCamCtrlState = eState;}

	// ！压入摄像机节点数据
	VOID AddCamPoint(TCamPathNode node){m_vecCamPathNodes.push_back(node);}

	// ！产生删词曲线
	VOID GenerateCamPathData();


	ClientCamera();

private:
	TObjRef<InputDX8>			m_pInput;
	TObjRef<Console>			m_pConsole;
	TObjRef<GameFrameMgr>		m_pFrmMgr;

	POINT m_ptLast;
	
	float m_fExpectDist;		// 期望距离
	float m_fOccultDist;		// 镜头被挡住时的dist
	DWORD m_dwLastOccultTime;	// 镜头上次被挡住的时间
	

	bool m_bFly;				// 是否是飞翔模式
	bool m_bRotating;			// 正在用鼠标旋转镜头

	bool m_bQuaking;			// 是否进行镜头震动处理
	DWORD m_dwQuakeStartTime;	// 镜头震动开始时间

	BOOL m_bDecTransparent;		
	FLOAT m_fTransparent;

    FLOAT m_HeightScaleRate;    //! 镜头LookAt的高度缩放比例 add by hyu

	virtual void Occult(NavMap *pNavMap);
	void UpdateQuake(Vector3& nextLookAt);

	void UpdateRoleTransparent();

	//！ 摄像机漫游工具需要
	bool				StopUpdate;			
	bool				mbLoop;				/**< 是否循环播放轨迹动画 */
	ECamreaControlState m_eCamCtrlState;	// 摄像机受控状态
	SimpleSpline		mCurve;				//控制生成position 三次曲线
	RotationalSpline	mRotCurve;			//控制生成摄像机向量四元数旋转的三次曲线
	std::vector<TCamPathNode> m_vecCamPathNodes;

	/** 播放摄像机路点动画 */
	VOID PlayPathAni( bool loop );
	VOID StopPathAni();
	VOID UpatePathAni( float deltaTime );
	/** 设置轨迹走完一次所需秒数，如果duration为-1，表示该设置失效。 */
	VOID SetPlayDuration( float duration );

	float mCurAniTime;/**< 当前动画时间（秒）*/
	float mTotalDuration;/**< 播放一次轨迹动画需持续总时间 */
};





