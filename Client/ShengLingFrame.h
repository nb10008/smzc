#pragma once
#include "IconStatic.h"
#include "IconStaticHL.h"
#include "ActiveProgress.h"
#include "ComposeStatic.h"
#include "..\WorldDefine\ItemDefine.h"

/*!
\class ShengLingFrame
\brief 圣灵界面
\author lib
\base GameFrame
\remark
*/
#define MAX_MOQI  100
#define MAX_DAY_EAT 5
struct tagUpdateLPItemEvent;
struct tagNS_SLEatFairy;
struct tagNS_RoleAttChangeSingle;
struct tagNS_EquipHoly;
struct tagNS_UnEquipHoly;
struct tagUpdateLPItemEvent;
struct tagNS_ChongNeng;
struct tagNS_LingNeng;
struct tagNS_HolyEquipChg;
struct tagNS_HolyStrength;
struct tagNS_HolyEquipmentNumber;
class Equipment;
class ShengLingFrame : public GameFrame
{
public:
	ShengLingFrame(void);
	~ShengLingFrame(void);

	virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
	virtual BOOL Destroy();
	virtual BOOL ReloadUI();
	virtual VOID Update();
	virtual BOOL EscCancel();

	VOID RefreshUI();							//刷新圣灵界面
	VOID RefreshTunShi();						//刷新吞噬界面
	VOID RefreshKeYin();						//刷新刻印界面
	VOID RefreshXiYin(BOOL bIsSelectAll);		//刷新洗印界面
	VOID RefreshChongNeng();					//刷新充能界面
	DWORD IsItemEnough(BYTE byQuality);
	VOID CloseShengLingWnd();
	VOID ChongNengEnable(BOOL bIsEnable);
	VOID ClearChongNeng();
private:
	DWORD EventHandler(tagGUIEvent* pEvent);
	DWORD OnItemEvent_UpdateItem(tagUpdateLPItemEvent* pGameEvent);
	DWORD OnShengLingEquipEvent(tagGameEvent* pGameEvent);
	DWORD OnShengLingUnEquipEvent(tagGameEvent* pGameEvent);
	DWORD OnItemEvent_EquipChange(tagUpdateLPItemEvent* pGameEvent);
	DWORD OnChaiEquipConfirmEvent(tagMsgBoxEvent* pMsgBoxEvent);
	DWORD OnUpdateHolyAtt(tagGameEvent* pEvent);
	DWORD OnShengWenKeYinConfirmEvent(tagMsgBoxEvent* pMsgBoxEvent);

	DWORD OnNS_SLEatFairy(tagNS_SLEatFairy* pMsg, LPVOID pParam);
	DWORD OnNS_RoleAttChangeSingle(tagNS_RoleAttChangeSingle* pMsg, LPVOID pParam);
	DWORD OnNS_EquipHoly(tagNS_EquipHoly* pMsg, LPVOID pParam);
	DWORD OnNS_UnEquipHoly(tagNS_UnEquipHoly* pMsg, LPVOID pParam);
	DWORD OnNS_ChongNeng(tagNS_ChongNeng* pMsg, LPVOID pParam);
	DWORD OnNS_LingNeng(tagNS_LingNeng* pmsg, LPVOID pParam);
	DWORD OnNS_HolyEquipChg(tagNS_HolyEquipChg* pMsg, LPVOID pParam);
	DWORD OnNS_HolyStrength(tagNS_HolyStrength* pmsg, LPVOID pPrarm);
	DWORD OnNS_HolyEquipmentNumber(tagNS_HolyEquipmentNumber* pmsg, LPVOID pPrarm);

	TSFPTrunk<ShengLingFrame>	m_Trunk;
	TObjRef<GUISystem>			m_pGUI;
	TObjRef<Util>				m_pUtil;
	TObjRef<NetCmdMgr>			m_pCmdMgr;
	TObjRef<GameFrameMgr>		m_pFrameMgr;
	TObjRef<NetSession>			m_pSession;

	//GUI控件
	GUIWnd*						m_pWnd;
	GUIWnd*						m_pWndFather;
	IconStatic*					m_pIStcItemPic;		//圣灵图标
	GUIStatic*					m_pStcItemName;		//圣灵名字
	ActiveProgress*				m_pProTunShi;		//吞噬进度条
	GUIStatic*					m_pStcTunShi;		//吞噬次数
	ActiveProgress*				m_pProLingNeng;		//灵能进度条
	GUIStatic*					m_pStcLingNeng;		//灵能数
	ActiveProgress*				m_pProMoQi;			//默契进度条
	GUIStatic*					m_pStcMoQi;			//默契数
	GUIButton*					m_pBtnTunShi;		//吞噬按钮
	GUIButton*					m_pBtnFusion;		//融合按钮(此功能貌似先不做)
	GUIStatic*					m_pStcShengLingDmg;	//圣灵伤害
	GUIStatic*					m_pStcShengLingDef;	//圣灵防御
	GUIStatic*					m_pStcCrit;			//致命
	GUIStatic*					m_pStcCritValue;	//致命量
	GUIStatic*					m_pStcDmgAdd;		//伤害加深
	GUIStatic*					m_pStcJingZhun;		//精准
	GUIStatic*					m_pStcLingQiao;		//灵巧
	IconStatic*					m_pIStcEquip[MaxHolyEquipNum];	//圣灵的6件装备
	GUIButton*					m_pBtnChong[MaxHolyEquipNum];		//装备充能的6个按钮
	GUIButton*					m_pBtnChai[MaxHolyEquipNum];		//拆装备的6个按钮
	GUIButton*					m_pBtnKeYin;		//刻印
	GUIStatic*					m_pStcShengLingName;

	//吞噬控件
	GUIWnd*						m_pWndTunShi;		
	ActiveProgress*				m_pProgress;			//吞噬进度条
	IconStatic*					m_pIStcShengLingIcon;	//圣灵图标
	ComposeStatic*				m_pCStcFairy;			//妖精图标
	GUIButton*					m_pBtnEat;				//吞噬按钮
	GUIButton*					m_pBtnTunShiCancel;		//取消
	GUIButton*					m_pBtnTunShiClose;		//关闭按钮
	GUIStatic*					m_pStcEatNum;			//吞噬次数
	GUIStatic*					m_pStcTunShiResult;		//吞噬结果（良好，一般啥的）
	GUIStatic*					m_pStcDmgNum;			//圣灵伤害数值
	GUIStatic*					m_pStcDefNum;			//圣灵防御数值
	GUIStatic*					m_pStcCirtNum;			//致命数值
	GUIStatic*					m_pStcCritValueNum;		//致命量数值
	GUIStatic*					m_pStcAttackTecNum;		//精准数值
	GUIStatic*					m_pStcNeglectNum;		//灵巧数值
	GUIStatic*					m_pStcDmgAddNum;		//伤害加深
	GUIStatic*					m_pStcLingNengNum;		

	//召唤控件
	GUIWnd*						m_pWndCall;	
	IconStatic*					m_pIStcCall;
	ActiveProgress*				m_pProShengLingValue;	//圣灵值进度条
	GUIStatic*					m_pStcShengLingValue;	//圣灵值

	//刻印控件
	GUIWnd*						m_pWndKeYin;
	IconStatic*					m_pIStcKShengLingIcon;	//圣灵图标
	ActiveProgress*				m_pProKTunShi;			//吞噬进度条
	GUIStatic*					m_pStcKTunShi;			
	ActiveProgress*				m_pProKLingNeng;		//灵能进度条
	GUIStatic*					m_pStcKLingNeng;
	ComposeStatic*				m_pCStcNewEquipIcon;	//待放装备图片
	IconStatic*					m_pIStcOldEquipIcon[MaxHolyEquipNum];	//已穿戴装备图标
	GUIStatic*					m_pStcEquipName[MaxHolyEquipNum];		//装备名字
	GUIStatic*					m_pStcShengLingLevel;	//要求圣灵吃妖精数
	GUIStatic*					m_pStcLingNengCost;		//灵能消耗
	GUIStatic*					m_pSteEquipAttName[X_ESAT_ATTA_NUM];
	GUIStatic*					m_pStcEquipAttValue[X_ESAT_ATTA_NUM];
	GUIButton*					m_pBtnEquipClose;		//关闭界面
	GUIButton*					m_pBtnBeginKeYin;		//刻印
	GUIButton*					m_pBtnCancelKeYin;		//取消刻印
	GUIStatic*					m_pStcShengWenName;		//刻印界面圣灵名字。。。

	//充能控件
	GUIWnd*						m_pWndChongNeng;		
	GUIButton*					m_pBtnCClose;			//充能关闭按钮
	GUIButton*					m_pBtnBeginChongNeng;	//冲能按钮
	GUIButton*					m_pBtnChongNengCancel;	//取消充能
	GUIStatic*					m_pStcRemainTimes;		//充能剩余次数
	GUIPushButton*				m_pPBtnBaseAtt;			//基础属性充能
	GUIPushButton*				m_pPBtnSpecAtt;			//灵能属性充能
	GUIPatch*					m_pPchBastAtt;
	GUIPatch*					m_pPchSpecAtt;
	IconStatic*					m_pIStcCEquip;			//充能装备图片
	ComposeStatic*				m_pCStcCIMIcon;			//充能道具
	ComposeStatic*				m_pCStcCDisIMIcon;		//偏向道具
	GUIStatic*					m_pStcAttAddValue;		//属性增涨值
	ComposeStatic*				m_pCStcCLingIMIcon;		//灵能道具
	GUIStatic*					m_pStcCurLingNeng;		//当前灵能
	GUIStatic*					m_pStcLingCost;			//灵能消耗减少量
	GUIComboBox*				m_pComBoxSelectAtt;		//选择属性组合框
	ActiveProgress*				m_pProChongNengTime;	//充能读条

	//充能成功界面
	GUIWnd*						m_pWndChongNengSucc;	//充能成功界面
	GUIStatic*					m_pStcChongNengAtt1;	//强化属性1
	GUIStatic*					m_pStcChongNengAtt2;	//强化属性2
	GUIStatic*					m_pStcChongNengAttValue1;//强化属性添加值1
	GUIStatic*					m_pStcChongNengAttValue2;//强化属性添加值2
	GUIButton*					m_pBtnConfirmChongNeng;	//确认充能
	GUIButton*					m_pBtnAbortChongNeng;	//放弃充能

	INT64						m_n64FairyID;			//即将被吃掉的妖精id
	INT64						m_n64KeYinEquipID;		//刻印放入的装备id

	BYTE						m_byXiYinIndex;			//将要洗印的索引
	DWORD						m_dwXiYinItemID;		//消耗道具

	Equipment*					m_pTempChongnengEquip;	//将要充能的装备
	INT64						m_n64ChongNengItemID;	//充能道具id
	INT64						m_n64PianXiangItemID;	//偏向道具id
	INT64						m_n64LingNengItemID;	//灵能定向道具id
	BYTE						m_byChongNengIndex;		//充能装备的索引
	BYTE						m_byLastSelectIndex;	//最后一次选择的索引
	map<BYTE, BYTE>				m_mapSelectIndex;
	BOOL						m_bIsSendMsg;			//是否正在圣纹强化读条
	BOOL						m_bIsLingNengChongNeng;	//是否强化灵能
};
