#pragma once
#include "gameframe.h"
#include "ItemButton.h"
#include "..\WorldDefine\compose_define.h"
#include "..\WorldDefine\msg_compose.h"

class FashionComposeFrame :
    public GameFrame
{
public:
    FashionComposeFrame(void);
    ~FashionComposeFrame(void);

    virtual BOOL Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam);
    virtual BOOL Destroy();
    virtual BOOL ReloadUI();
    virtual VOID Update();
    virtual VOID Render3D();
    virtual BOOL EscCancel();

    //! 打开界面
    VOID ShowWnd(BOOL bVal);

    VOID SetItem(Item* pItem);

private:
    //! 精纺返回消息
    DWORD OnNS_ConsolidateSpin(tagNS_ConsolidateSpin* pMsg, DWORD);

    //! 设置精纺信息
    //! \para bLeft,是否显示在左边
    VOID SetFashionMsg(bool bLeft, ESpinStep eStep, ESuitAttType eSuitType);

    //! 重置界面
    VOID ClearUI();

    VOID ClearData();
    VOID SetSuccessRate(int rate, bool bClear = false);
    VOID RefreshSuiteState(bool bClear = false);
    VOID RefreshSuiteAttrBegin(int i, bool bClear = false);
    VOID RefreshSuiteAttrEnd(int i, bool bClear = false);

    ESpinStep GetCurrFashionSpinSetp();
    ESuitAttType GetCurrFashionSuitType();
    ESuitAttType GetSelectedFashionSuitType();
    int GetSelectedFashionSpinRate();

    //! 处理GUI消息
    DWORD EventHandler(tagGUIEvent* pEvent);

    //! 支持流的类型互相转换
    template<typename T>
    tstring TtoString(T nSrc);

    DWORD                       m_dwNpcId;          // 当前npcid

    INT64                       m_pCurrFashionID;   // 当前时装ID
    INT64                       m_pCurrMatearial;   // 当前材料ID
    DWORD                       m_pCurrMatearialType;   // 当前材料type id
    int                         m_pMatearialCount;  // 材料数量

    bool                        m_bBegin;           // 是否开始精纺
    DWORD                       m_dwStartTime;      // 精纺开始时间
    DWORD                       m_updateTimeDistance;   // 检测与npc距离

    //GUI控件
    GUIWnd*						m_pWnd;

    ItemButton*                 m_pIbtFashion;      // 要精纺的时装
    GUIProgress*                m_pProgress;        // 进度条

    ItemButton*                 m_pIbtMateiral;     // 精纺材料
    GUIButton*                  m_pBtInc;           // 加号
    GUIButton*                  m_pBtDec;           // 减号
    GUIStatic*                  m_pStcSucessRate;   // 成功率

    GUIPushButton*              m_pPbtSuit[4];      // 套装选择
    GUIStatic*                  m_pStcSuit[4];      // 套装名称

    GUIStatic*                  m_pStcHP[2];        // 体力
    GUIStatic*                  m_pStcMP[2];        // 真气
    GUIStatic*                  m_pStcVitality[2];  // 活力

    GUIStatic*                  m_pStcSavvy[2];     // 悟性
    GUIStatic*                  m_pStcFortune[2];   // 福禄
    GUIStatic*                  m_pStcRein[2];      // 统御

    GUIStatic*                  m_pStcPhysique[2];  // 筋骨
    GUIStatic*                  m_pStcStrength[2];  // 劲力
    GUIStatic*                  m_pStcTechnique[2]; // 技力
    GUIStatic*                  m_pStcPneuma[2];    // 元气
    GUIStatic*                  m_pStcInnerForce[2];// 内力
    GUIStatic*                  m_pStcAgility[2];   // 身法

    GUIStatic*                  m_pStcExAttack[2];  // 外功攻击
    GUIStatic*                  m_pStcExDefense[2]; // 外功防御
    GUIStatic*                  m_pStcAttackTec[2]; // 攻击技巧
    GUIStatic*                  m_pStcArmor[2];     // 防具护甲
    GUIStatic*                  m_pStcDefenseTec[2];// 防御技巧
    GUIStatic*                  m_pStcInAttack[2];  // 内功攻击
    GUIStatic*                  m_pStcInDefense[2]; // 内功防御
    GUIStatic*                  m_pStcWeaponDmgMax[2];// 最大武器威力
    GUIStatic*                  m_pStcExDamage[2];    // 额外伤害
    GUIStatic*                  m_pStcDerateALL[2]; // 全部伤害减免
    GUIStatic*                  m_pStcExDamageAbsorb[2];    // 额外伤害吸收
    GUIStatic*                  m_pStcHitRate[2];   // 命中
    GUIStatic*                  m_pStcWeaponSoul[2];// 武魂
    GUIStatic*                  m_pStcDodge[2];     // 闪避
    GUIStatic*                  m_pStcCritRate[2];  // 致命
    GUIStatic*                  m_pStcRegainAddtion[2];      // 气力
    GUIStatic*                  m_pStcCritAmount[2];// 致命量
    GUIStatic*                  m_pStcBlockRate[2]; // 格挡率

    GUIButton*                  m_pBtStart;         // 开始
    GUIButton*                  m_pBtClose;         // 关闭
    GUIButton*                  m_pBtCancel;        // 关闭


    TSFPTrunk<FashionComposeFrame> m_Trunk;
    TObjRef<GUISystem>			m_pGUI;
    TObjRef<Util>				m_pUtil;
    TObjRef<NetCmdMgr>			m_pCmdMgr;
    TObjRef<GameFrameMgr>		m_pFrameMgr;
    TObjRef<NetSession>			m_pSession;
};

template<typename T>
tstring FashionComposeFrame::TtoString( T nSrc )
{
    if (nSrc <= 0)
        return _T("");

    tstringstream sstream;
    sstream << nSrc;
    tstring ret;
    sstream >> ret;
    ret = _T("+") + ret;
    return ret;
}