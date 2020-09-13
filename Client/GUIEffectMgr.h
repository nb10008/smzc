#pragma once

#include "GUICameraData.h"

class GUIEffectMgr
{
public:
	//! 特效类型
	enum EEffectType
	{
		EET_Common,			// 普通特效
		EET_HP,				// 生命值
		EET_MP,				// 魔法值
		EET_Anger,			// 怒气
		EET_Enforcing,		// 强化中
		EET_EnforcSucess,	// 强化成功
		EET_EnforcFail,		// 强化失败
		EET_ComposSucess,	// 合成成功
		EET_ComposFail,		// 合成失败
		EET_Sprite,			// 妖精契约闪光
		EET_ShengLingCall,	// 圣灵召唤
	};

	//! 特效的缩放类型
	enum EEffectScaleType
	{
		EEST_Null,           // 不缩放
		EEST_Y,				 // 根据高度缩放
		EEST_XYZ,			 // 使用X,Y,Z的平均值缩放
		EEST_XZ,			 // 使用X,Z的平均值缩放
	};

	//! 混合模式类型
	enum EEffectBlendMode
	{
		EEBM_None = 0,		
		EEBM_Alpha_InAlpha,	// alpha混合
		EEBM_One_One,		// 1:1混合
	};

private:
	//! 特效基本数据
	struct tagBaseEffectData
	{
		EEffectType				eType;			// 特效类型
		EEffectScaleType		eScaleType;     // 缩放类型
		SGAttachableEffectNode*	pSGNode;        // 特效Scene node
		GUIWnd*					pWnd;			// 要渲染的目标位置, 优先
		tagRect					destRect;		// 要渲染的目标位置
		DWORD					dwHandle;		// 自己的渲染缓冲,只有界面特效使用自己的缓冲区，全屏特效不用
		EEffectBlendMode		eBlendMode;		// 特效的混合模式
		tstring					strEffectName;

		tagBaseEffectData( EEffectType type ) : 
		eType( type ), pSGNode( NULL ) , eScaleType( EEST_Null ),pWnd(NULL),dwHandle(GT_INVALID),eBlendMode(EEBM_One_One),strEffectName(_T(""))
		{
		}
	};
	typedef map<DWORD, tagBaseEffectData*> EffecMap;

public:
	GUIEffectMgr(void);
	~GUIEffectMgr(void);

	//! 初始化，由战斗系统调用
	VOID Init();

	//! 释放数据，由战斗系统调用
	VOID Destroy();

	//! 更新特效，由战斗系统调用
	VOID Update();

	//! 在FrameMgr.render3d 的时候渲染
	void Render3D();

	//! 将界面特效的颜色缓冲混合到界面上
	void Render();

	//! 播放特效，指定位置、缩放
	DWORD PlayGuiEffect( const TCHAR* szEffectName, GUIWnd* pWnd, tagRect& rect,const Vector3& vPos, const Vector3& vScale, const TCHAR* szEffectFolder = _T("ui") ,bool bReSetCamera = false);
	//! 通过配置文件读取特效信息
	DWORD PlayGuiEffect( const TCHAR* szEffectName, GUIWnd* pWnd,EEffectType eType= EET_Common,const TCHAR* szEffectFolder = _T("ui"));
	DWORD PlayScreenEffect( const TCHAR* szEffectName, const tagPoint& ptView, const tagPoint& ptSize, const Vector3& vPos, const Vector3& vScale, const TCHAR* szEffectFolder = _T("ui"),bool bReSetCamera = false );
	//! 通过配置文件读取特效信息x
	DWORD PlayScreenEffect( const TCHAR* szEffectName,const tagPoint& ptView, const tagPoint& ptSize,EEffectType eType= EET_Common,const TCHAR* szEffectFolder = _T("ui"));

	//! 播放alpha特效
	DWORD PlayScreenEffect_a( const TCHAR* szEffectName,const tagPoint& ptView, const tagPoint& ptSize,EEffectType eType= EET_Common,const TCHAR* szEffectFolder = _T("ui"));
	
	//! 停止一个特效
	VOID StopEffect( const DWORD dwEffectID );
	//停止界面上的某种特效,若无特效名则停止界面上的所有特效
	VOID StopGuiEffect( GUIWnd* pWnd , const TCHAR* szEffectName = NULL );

	VOID StopEffectByType( EEffectType eType );

	VOID RemoveEffect(const DWORD dwEffectID);

	VOID RemoveOneEffectByType(EEffectType eType);

	//! 显示\隐藏特效
	VOID ShowEffect( const DWORD dwEffectID, const bool bShow );
	// 窗口是否已经被隐藏
	bool WndIsHide(GUIWnd* pWnd);

	VOID SetUICamera( const Vector3& vPos, const Vector3& vLookAt);

	VOID CloseAllEffect();

	VOID	SetLoadingFlag( BOOL b ){ m_bLoading = b; }

	static GUIEffectMgr* Inst();

private:
	SGAttachableEffectNode*		LoadSFXFile( const TCHAR* szEffectName, const TCHAR* szFolderName = _T("ui") );
	DWORD						AddEffect( tagBaseEffectData* pData );
	VOID						DeleteEffect( tagBaseEffectData* pData );    // 不会删除map中的节点
	

private:
	TObjRef<AudioSys>		m_pAudioSys;

	IGUIRender*				m_pRender;
	GUIWnd*					m_pDeskTop;
	NullSceneGraph*			m_pSG;					 // 用于保存摄像机、光照等信息,不用于渲染
	CameraBase*				m_pCamera;				 // 用于给玩家拍照
	tagGUIImage*			m_pCanvasImg;			 // 画布

	EffecMap				m_mapEffect;			 // 特效容器
	DWORD					m_dwCurEffectID;         // 记录当前特效ID
	GUICameraData			m_CameraData;
	BOOL						m_bLoading;	 //loading界面标记
};
