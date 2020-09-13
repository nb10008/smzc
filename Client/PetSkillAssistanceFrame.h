#pragma once
#include "gameframe.h"
#include "ItemButton.h"
#include "PetDef.h"
#include "PetSkillFrame.h"
#include "PetSkillButton.h"
#include "../WorldDefine/pet_skill_define.h"

class StaticEx;

const int MAX_PET_ONLINE_SKILL = 5;
const int MAX_PET_EGG_BUY   = 6;
const tstring STR_SELECT_GOOD   = _T("data\\ui\\Common\\L_icon-l.bmp");
const tstring STR_BACK_GOOD     = _T("data\\ui\\sys\\l_diban-a.bmp");

//! \class  PetSkillAssistanceFrame
//! \brief  灵兽内挂技能整合
//! \detail
//! \author hyu
//! \date   2009-1-14
//! \base   GameFrame
//! \sa     PetSkillFrame
class PetSkillAssistanceFrame :
    public GameFrame
{
public:
    PetSkillAssistanceFrame(void);
    ~PetSkillAssistanceFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual BOOL EscCancel();

    void ShowWnd(bool bVisible);
    bool IsOpen();

private:
    //! 初始化UI
    void InitalUI();
    //! resize combobox
    void ResizeCombobox(GUIComboBox* pComboBox, int rowCount);

    // 刷新所有宠物
    void RefreshPets();
    // 清空技能
    void ClearSkill();
    // 打开训练
    void TurnOnSkillUI(EPetskillType eType, DWORD dwTypeID, bool bisOpen);
    // 刷新当前宠物技能
    void RefreshPetSkill();
    // 读取技能设置配置
    void LoadPetSkillCfg(EPetskillType ePetSkillType);

    // 显示购买宠物界面
    void ShowBuyWnd(bool visable);

    // 读取灵兽购买列表
    void LoadPets();

    // 清空购买界面
    void ClearBuyUI();

    // 刷新物品列表
    void RefreshItemBuy();

    //! 宠物属性发生变化
    DWORD OnPetAttrChange(tagPetAttrChange* pEvent);

    DWORD EventHandler(tagGUIEvent* pEvent);

    DWORD BuyEventHandler(tagGUIEvent* pEvent);

    DWORD                       m_CurrPetID;        //!< 当前灵兽ID
    int                         m_nSelectedRow;     //!< 当前选中行
    map<int, DWORD>             m_mapRow2Pet;       //!< 行与灵兽ID对照表

    int                         m_nCurrItem;        //!< 当前选中的第几个item

    // GUI Controls
    GUIWnd*                     m_pWnd;

    GUIButton*                  m_pBtClose;         //!< 关闭窗体
    GUIButton*                  m_pBtBuy;           //!< 购买灵兽
    
    GUIComboBox*                m_pCbPetList;       //!< 拥有灵兽

    GUIStatic*                  m_pStcSkillLeft;    //!< 还有x项技能未习得

    GUIPushButton*              m_pPbtFight;        //!< 战斗
    GUIPatch*                   m_pPchFight;        //!< 战斗
    GUIPushButton*              m_pPbtProduction;   //!< 生产
    GUIPatch*                   m_pPchProduction;   //!< 生产
    GUIPushButton*              m_pPbtOther;        //!< 其它
    GUIPatch*                   m_pPchOther;        //!< 其它

    // 采集
    PetSkillButton*             m_pPetSkillGather;  //!< 采集技能图标
    GUIButton*                  m_pPetLearnGather;  //!< 学习采集

    // 训练
    PetSkillButton*             m_pPetSkillTraining;    //!< 训练技能图标
    GUIButton*                  m_pPetLearnTraining;    //!< 学习训练
    GUIPushButton*              m_pPbtComposeSkill[ESSTE_End]; //!< 生产技能复选框
    GUIButton*                  m_pBtTrainingConfirm;       //!< 训练

    // 自动拾取
    PetSkillButton*             m_pPetSkillPickUp;    //!< 拾取技能图标
    GUIButton*                  m_pPetLearnPickUp;    //!< 学习拾取
    GUIPushButton*              m_pBtPickUpFunc[MAX_PICKUP_FUNC]; //!< 功能开启
    GUIButton*                  m_pBtPickConfirm;       //!< 关闭

    // 护理
    PetSkillButton*             m_pPetSkillMedicare;    //!< 护理技能图标
    GUIButton*                  m_pPetLearnMedicare;    //!< 学习护理

    GUIComboBox*                m_pCbRedCondition;      //!< 红药条件
    GUIPushButton*              m_pPbtRedEnable;        //!< 红药功能开启
    ItemButton*                 m_pBtRed;               //!< 红药

    GUIComboBox*                m_pCbBlueCondition;     //!< 蓝药条件
    GUIPushButton*              m_pPbtBlueEnable;       //!< 蓝药功能开启
    ItemButton*                 m_pBtBlue;              //!< 蓝药

    GUIComboBox*                m_pCbToughnessCondition;    //!< 持久力条件
    GUIPushButton*              m_pPbtToughnessEnable;  //!< 持久力功能开启
    ItemButton*                 m_pBtToughness;         //!< 持久力药

    // 贩卖
    PetSkillButton*             m_pPetSkillSale;        //!< 贩卖技能图标
    GUIButton*                  m_pPetLearnSale;        //!< 学习贩卖

    // 灵兽购买界面
    GUIWnd*                     m_pWndBuy;              //!< 购买窗体
    GUIStatic*                  m_pStcTitle;            //!< 说明
    StaticEx*                   m_pStcSelect[MAX_PET_EGG_BUY];  //!< 选择底板
    ItemButton*                 m_pIbtEgg[MAX_PET_EGG_BUY]; //!< 宠物精魂
    DWORD                       m_dwItemID[MAX_PET_EGG_BUY];//!< 精魂ID
    GUIStatic*                  m_pStcPrice[MAX_PET_EGG_BUY];   //!< 宠物蛋价格
    GUIButton*                  m_pBtBuyConfirm;        //!< 购买
    GUIButton*                  m_pBtBuyCanel;          //!< 取消
    GUIButton*                  m_pBtBuyExit;           //!< 关闭

    TSFPTrunk<PetSkillAssistanceFrame>  m_Trunk;
    TObjRef<GUISystem>			        m_pGUI;
    TObjRef<Util>				        m_pUtil;
    TObjRef<GameFrameMgr>		        m_pFrameMgr;
    TObjRef<NetSession>			        m_pSession;

    DWORD                       m_dwUpdateTime;         //!< 自动刷新时间
};
