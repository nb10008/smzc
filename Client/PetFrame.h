#pragma once
#include "gameframe.h"
#include "ItemButton.h"
#include "PetSkillButton.h"
#include "StaticEx.h"
#include "../WorldDefine/msg_pet.h"
#include "PetEvent.h"
#include "../WorldDefine/time.h"

const INT MAX_PET_NUM           =   5;	//初始宠物和骑乘各自的最大栏位数
const INT MAX_PET_EQUIP         =   4;
const INT MAX_PET_SKILL         =   4;
const INT MAX_PET_RIDER_TOTAL_NUM  =   20; ////最终宠物和骑乘公用的最大栏位数
const tstring SELECT_ICON_PATH  =   _T("data\\ui\\Common\\L_icon-l2.bmp");
const tstring SELECT_DIBAN      =   _T("data\\ui\\sys\\l_diban-a.bmp");
const tstring SELECT_LOCK       =   _T("data\\ui\\pet\\L_hui.bmp");
const tstring ZUOQI_PIC			=   _T("data\\ui\\pet\\zuoqi.bmp");
const tstring FUZHU_PIC			=   _T("data\\ui\\pet\\fuzhu.bmp");
const tstring GREY_PIC			=	_T("data\\ui\\pet\\l_xiaofangkuai.dds");
class PetShowWindow;
class PetBagFrame;

//当前点击分页枚举
enum E_Click_Page
{
	Click_Pet_Page = 0,
	Click_Rider_Page =1,
};
//! \class  PetFrame
//! \brief  本地玩家宠物管理界面
//! \author hyu
//! \base   GameFrame
//! \sa     PetManager
class PetFrame :
    public GameFrame
{
public:
    PetFrame(void);
    ~PetFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual VOID Render3D();
	virtual BOOL EscCancel();

    bool IsWindowVisible();
    VOID SetPageType(INT type)
	{
		m_eCurClickPage = (E_Click_Page)type;
	}
private:
    //! 处理GUI消息
    DWORD EventHandler(tagGUIEvent* pEvent);

    //! 打开/关闭宠物界面游戏事件
    DWORD OnOpen_Pet(tagGameEvent* pMsg);
	DWORD OnClose_Pet(tagGameEvent* pMsg);
    
    DWORD OnOpen_PetByID(tagOpenPetbyID* pMsg);
    
	//响应宠物乘骑的栏位扩充变化
	DWORD OnMaxNumChanged();
    //! 打开界面
    VOID ShowWnd(BOOL bVal);

    //! 刷新宠物列表
    VOID RefreshPetList();
	//! 更新宠物数据
	VOID UpdateData();
    //! 刷新界面数据
    VOID RefreshToUI();
    ////! 刷新当前装备
    //VOID RefreshEquip();
    //! 刷新当前技能
    VOID RefreshSkill();
    //! 刷新当前模型
    VOID RefreshPetShow();

    //! 获得等级文字
    tstring GetLevelText(int nLevel);
    //! 获得品级文字
    LPCTSTR GetQualityText(int nQuality);

    //! 重置界面
    VOID ClearUI();
    //! 重置單個寵物數據
    VOID ClearUIData();
	//！清空客户端数据
	VOID ClearPetInfo();
    //! 确定删除宠物
    DWORD OnCommitDeletePet(tagMsgBoxEvent* pEvent);
	DWORD OnCommitCallBackPet(tagMsgBoxEvent* pEvent);//确定回收宠物
    //! 输入完改名消息框返回的事件
    DWORD OnCommitPetName(tagMsgInputBoxEvent* pEvent);
    //! 更名后的反馈
    DWORD OnNS_PetRename(tagNS_PetRename* pMsg, DWORD);

    //! 确定灌注经验
    DWORD OnCommitPourExp(tagMsgBoxEvent* pEvent);

    //! 处理宠物穿装备事件
    DWORD OnUsePetEquipHandler(tagUsePetEquip* pEvent);

    //! 宠物属性发生变化
    DWORD OnPetAttrChange(tagPetAttrChange* pEvent);

	DWORD OnEvent_Refresh_PetUI(tagGameEvent* pEvent);

	DWORD OnPetDispInfo(tagPetDispChange* pEvent );

	DWORD OnMsgBoxClickEvent(tagMsgBoxEvent* pGameEvent);
    //! 支持流的类型互相转换
    template<typename T>
    tstring TtoString(T nSrc);

	//由ID获得宠物乘骑剩余时间字串，并返回是否死亡
	BOOL ID2RestTime(DWORD dwID,tstring *refTstr);
	//翻页后相关刷新
	VOID PageChangeRefresh();

	E_Click_Page				  m_eCurClickPage;         //!< 当前选中页面类型
	E_Click_Page                 m_bCurExtendMutex;		//!< 当前选扩充的类型
	//宠物分页(pet)
    DWORD                       m_dwCurrSelectPetID;     //!< 当前选中的宠物ID(如无选择则为GT_INVALID)
    INT                         m_nSelectRow;           //!< 当前选中的行
    map<INT, DWORD>              m_mapRow2Pet;           //!< 位置和宠物id对照表
    INT                         m_nCurrPage;            //!< 当前页
	BOOL						  m_bCurrPetDead;			//!< 现在的宠物是否已经到达时间限制
	INT							  m_iCurPetNum;				//!< 现在的宠物数量		
	INT							  m_iCurPetGridNum;			//!< 当前的宠物栏数量
	//乘骑分页(pet1)
	DWORD                       m_dwCurrSelectPetID1;     //!< 当前选中的乘骑ID(如无选择则为GT_INVALID)
	INT                         m_nSelectRow1;           //!< 当前选中的行
	map<INT, DWORD>              m_mapRow2Pet1;           //!< 位置和乘骑id对照表
	INT                         m_nCurrPage1;            //!< 当前页
	BOOL						  m_bCurrPetDead1;			 //!< 现在的宠物是否已经到达时间限制
	INT							  m_iCurRiderNum1;			 //!< 现在的乘骑数量		
	INT							  m_iCurRiderGridNum;		 //!< 当前的乘骑栏数量

    TSFPTrunk<PetFrame>          m_Trunk;
    TObjRef<GUISystem>			  m_pGUI;
    TObjRef<Util>				  m_pUtil;
    TObjRef<NetCmdMgr>			  m_pCmdMgr;
    TObjRef<GameFrameMgr>		  m_pFrameMgr;
    TObjRef<NetSession>			  m_pSession;

    //GUI控件
    GUIWnd*						  m_pWnd;


	//宠物分页
	GUIPatch*                   m_pPchPetPage;                 //宠物分页wnd
	GUIPushButton*				  m_pBtPetPage;					 //宠物分页按钮
	//GUIStatic*                  m_pStcState;                    //!< 状态
    //GUIListBox*                 m_pLbPetList;                   //!< 宠物列表
    GUIButton*                  m_pBtPrevPage;                  //!< 上一页
    GUIButton*                  m_pBtNextPage;                  //!< 下一页
    GUIStatic*                  m_pStcPage;                     //!< 显示页数
    //GUIButton*                  m_pBtPerfuse;                   //!< 灌注
    //ItemButton*                 m_pBtEquipA[MAX_PET_EQUIP];     //!< 宠物装备
    PetSkillButton*             m_pBtSkillA[MAX_PET_SKILL];     //!< 宠物技能
	PetShowWindow*              m_pStcPetShow;					  //!< 宠物Avata
	//ItemButton*                 m_pBtSkillB[MAX_PET_SKILL];     //!< 宠物装备
    //GUIStatic*                  m_pStcCarryLevel;               //!< 携带等级...
    //GUIStatic*                  m_pStcQuality;                  //!< 宠物品质
    //GUIStatic*                  m_pStcMatureTalent;             //!< 成长资质
    //GUIStatic*                  m_pStcPotential;                //!< 潜能
    //GUIStatic*                  m_pStcMagic;                    //!< 仙灵之力(原五行力)
    //GUIStatic*                  m_pStcRidingSpeed;              //!< 骑乘速度
    GUIPatch*                   m_pPchBack[MAX_PET_NUM];        //!< 宠物选择底板...
    GUIButton*                  m_pBtBack[MAX_PET_NUM];        //!< 宠物选择按钮...
    StaticEx*                   m_pStcOption[MAX_PET_NUM];      //!< 宠物选择高亮底板...
    StaticEx*                   m_pStcIcon[MAX_PET_NUM];        //!< 宠物图标...
    GUIStatic*                  m_pStcNick[MAX_PET_NUM];        //!< 宠物昵称
    GUIStatic*                  m_pStcIsCalled[MAX_PET_NUM];    //!< 是否召唤中...
    GUIButton*                  m_pBtCall[MAX_PET_NUM];        //!< 召唤 
	GUIStatic*                  m_pStcIsBinding[MAX_PET_NUM];   //!< 是否绑定...
	GUIStatic*                  m_pStcTime[MAX_PET_NUM];		  //!< 宠物寿命
	//GUIStatic*                  m_pStcLevel[MAX_PET_NUM];       //!< 级别
    //GUIStatic*                  m_pStcEnergy[MAX_PET_NUM];      //!< 精气(原灵力)
    //GUIProgress*                m_pProgressEnergy[MAX_PET_NUM]; //!< 精气进度条
    //GUIStatic*                  m_pStcExp[MAX_PET_NUM];         //!< 经验
    //GUIProgress*                m_pProgressExp[MAX_PET_NUM];    //!< 精气进度条
	//ItemButton*				  m_pStcRideing[MAX_PET_NUM];		//!< 坐骑图标

	//乘骑分页
	GUIPatch*                   m_pPchRiderPage;                 //乘骑分页wnd
	GUIPushButton*				  m_pBtRiderPage;					  //乘骑分页按钮
	GUIPatch*                   m_pPchBack1[MAX_PET_NUM];         //!< 宠物选择底板...
	GUIButton*                  m_pBtBack1[MAX_PET_NUM];          //!< 宠物选择按钮...
	StaticEx*                   m_pStcOption1[MAX_PET_NUM];       //!< 宠物选择高亮底板...
	StaticEx*                   m_pStcIcon1[MAX_PET_NUM];         //!< 宠物图标...
	GUIStatic*                  m_pStcNick1[MAX_PET_NUM];         //!< 宠物昵称
	GUIStatic*                  m_pStcIsPreparing1[MAX_PET_NUM];   //!< 是否预备驾驭中...
	GUIButton*                  m_pBtRide1[MAX_PET_NUM];          //!< 骑乘   
	GUIStatic*                  m_pStcIsBinding1[MAX_PET_NUM];     //!< 是否绑定...
	GUIStatic*                  m_pStcTime1[MAX_PET_NUM];			 //!< 宠物寿命
	PetShowWindow*              m_pStcPetShow1; 
	GUIButton*                  m_pBtPrevPage1;                  //!< 上一页
	GUIButton*                  m_pBtNextPage1;                  //!< 下一页
	GUIStatic*                  m_pStcPage1;                     //!< 显示页数
	PetSkillButton*             m_pBtSkillA1[MAX_PET_SKILL];     //!< 乘骑技能

	GUIButton*                  m_pBtTrade;                     //!< 交易
	GUIButton*                  m_pBtExtend;                     //!< 扩充
	GUIButton*                  m_pBtChangeName;                //!< 更名...
	GUIButton*                  m_pBtSetFree;                   //!< 放生
	GUIButton*                  m_pBtCallBack;                   //!< 回收

    GUIButton*                  m_pBtClose;                     //!< 关闭

    PetBagFrame*                m_pPetBagFrame;                 //!< pet bag frame

    DWORD                       m_dwStartTime;                  //!< 更新开始时间

};

template<typename T>
tstring PetFrame::TtoString( T nSrc )
{
    tstringstream sstream;
    sstream << nSrc;
    tstring ret;
    sstream >> ret;
    return ret;
}