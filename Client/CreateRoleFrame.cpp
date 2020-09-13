#include "StdAfx.h"
#include "CreateRoleFrame.h"
#include ".\Player.h"
#include ".\AvatarCfg.h"
#include "..\WorldDefine\RoleDefine.h"
#include ".\SelectRoleFrame.h"
#include ".\SeleRoleMgr.h"
#include "FilterData.h"
#include "GameSet.h"
#include "AudioSys.h"
#include "CursorMgr.h"
#include "LoadingFrame.h"
#include "ItemProtoData.h"
#include "CameraSetData.h"
#include "SaveAccount.h"

#define ROTATEBTN_YOFFSET 530
#define LROTATEBTN_XOFFSET 850
#define RROTATEBTN_XOFFSET 500
const float ROTATE_SPEED = 60;		//人物旋转的速度


CreateRoleFrame::CreateRoleFrame(void):m_Trunk(this),
m_fRoleYaw(2.14f),
m_eStatus(EPCS_OutKungSword),
m_bRotate(false),
m_bLRotate(false),
m_bRRotate(false),
m_bSentCreate(false),
m_pTex(NULL)
{
	Clear();
	m_pPlayer	= NULL;
	ZeroMemory(&m_avatarEquip,sizeof(m_avatarEquip));
	m_step = EStep_End;
	m_szEnterTrackName.assign( _T("") );
	m_szLeaveTrackName.assign( _T("") );
	m_bPlayPreview = false;
	m_eEquipType = ECRP_Null;
	m_eLeftEquipType = ECRP_Null;

	//m_iCurTextNum=1;
	//m_iMaxTextNum=1;

}

CreateRoleFrame::~CreateRoleFrame(void)
{

}

void CreateRoleFrame::Clear()
{
	m_pWnd				= NULL;
	m_pWndCaption		= NULL;
	m_pLBtnRotate		= NULL;
	m_pRBtnRotate		= NULL;
	m_pStcErrHint		= NULL;
	m_pStcSex			= NULL;
	m_pName				= NULL;
	m_pSexMutex			= NULL;
	//m_pStatusMutex		= NULL;
	m_pMalePBtn			= NULL;
	m_pFmalePBtn		= NULL;
	m_pLBtnHType		= NULL;
	m_pRBtnHType		= NULL;
	m_pStcHType			= NULL;
	m_pLBtnHColor		= NULL;
	m_pRBtnHColor		= NULL;
	m_pStcHColor		= NULL;
	m_pLBtnFace			= NULL;
	m_pRBtnFace			= NULL;
	m_pStcFace			= NULL;
	m_pLBtnFDetail		= NULL;
	m_pRBtnFDetail		= NULL;
	m_pStcFDetail		= NULL;
	m_pLBtnDress		= NULL;
	m_pRBtnDress		= NULL;
	m_pStcFDress		= NULL;
	memset( m_pStatusPBtn, 0x0, sizeof( m_pStatusPBtn ) );
	m_pBtnOK			= NULL;
	m_pBtnRandom		= NULL;
	m_pBtnCancel		= NULL;
	m_bPickRole			= false;
	m_pWnd_Control		= NULL;
	m_pLBtnRotate		= NULL;
	m_pRBtnRotate		= NULL;
	m_pPBtn_Push		= NULL;
	m_pPBtn_Pull		= NULL;

	//m_pWndWeaponIntro	= NULL;;  //武器说明窗口
	//m_pStcIntroNum		= NULL;;     //武器说明页数显示
	//m_pStcIntroText		= NULL;;    //武器说明文本显示
	//m_pBtnPreIntro		= NULL;;     //武器说明页前一页按钮
	//m_pBtnNextIntro		= NULL;;    //武器说明页后一页按钮

	//m_pBtnSword = NULL;
	//m_pBtnBlade = NULL;
	//m_pBtnWand  = NULL;
	//m_pBtnBow   = NULL;
	//m_pBtnOrgan = NULL;
	//m_pBtnLongShou	= NULL;
	//m_pBtnQin = NULL;
	//m_pBtnAmorSuit = NULL;
	//m_pBtnClothAmorSuit	= NULL;
	//m_pBtnFashion1 = NULL;
	//m_pBtnFashion2 = NULL;
}

BOOL CreateRoleFrame::Init(GameFrameMgr* pMgr, GameFrame* pFather, DWORD dwParam)
{
	if( !GameFrame::Init(pMgr, pFather, dwParam) )
		return FALSE;

	BeginCreateRole();

	m_pCmdMgr->Register("NS_CreateRole", (NETMSGPROC)m_Trunk.sfp2(&CreateRoleFrame::NetRecvCreateRole), _T("NS_CreateRole"));

	m_pVarContainer = CreateObj("CreateRole", "VarContainer");
	m_pVarContainer->Load(NULL, _T("config\\CreateRole.xml"));
	m_nRoleNameLength = m_pVarContainer->GetDword(_T("Role_Name_Length"));
	CreateBackGroud((INT)ETT_Swing);

	//加载创建角色预览文件
	m_previewData.LoadFromFile();

	TObjRef<AudioSys>()->StopMusic();
	// 播放选人音乐
	TObjRef<AudioSys>()->PlayMusic(_T("music_create_role"), TRUE, SOUND_LOUD);

	return TRUE;
}

BOOL CreateRoleFrame::Destroy()
{
	GameFrame::Destroy();

	if(P_VALID(m_pTex))
		IDraw2D::Inst()->ReleaseTexture(m_pTex);

	if( P_VALID(SeleRoleMgr::Inst()->GetHugeSG()) )
	{
		m_pPlayer->DetachSceneNodeFromSG(SeleRoleMgr::Inst()->GetHugeSG());		//从场景中删除结点
	}

	if( P_VALID(m_pPlayer) )						//释放结点
		SAFE_DELETE(m_pPlayer);

	if( P_VALID(m_pWnd) )
		m_pGUI->DestroyWnd(m_pWnd);

	if( P_VALID(m_pWnd_Control) )				
		m_pGUI->DestroyWnd(m_pWnd_Control);

	//if( P_VALID(m_pWndWeaponIntro) )				
	//	m_pGUI->DestroyWnd(m_pWndWeaponIntro);


	m_pCmdMgr->UnRegister("NS_CreateRole", (NETMSGPROC)m_Trunk.sfp2(&CreateRoleFrame::NetRecvCreateRole));

	Clear();
	KillObj("CreateRole");


	TObjRef<AudioSys>()->StopMusic();

	// 播放登陆音乐
	TObjRef<AudioSys>()->PlayMusic(_T("music_login0"), FALSE, SOUND_LOUD);

	return TRUE;
}

//-----------------------------------------------------------------------------
// !加载控件
//-----------------------------------------------------------------------------
BOOL CreateRoleFrame::ReloadUI()
{
	XmlElement element;
	tstring strPath = g_strLocalPath + _T("\\ui\\creatrole.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath.c_str());

	// 调整位置
	//GUIWnd* pDesktop = m_pGUI->GetDesktop();
	//std::stringstream streamX, streamY;
	//streamX << (pDesktop->GetSize().x - 425);
	//streamY << (pDesktop->GetSize().y - 600);
	//m_pGUI->ChangeXml(&element, "cr_win", "LogicPos_x", streamX.str().c_str());
	//m_pGUI->ChangeXml(&element, "cr_win", "LogicPos_y", streamY.str().c_str());
	//m_pGUI->ChangeXml(&element, "cr_win", "AlignType", "NoAlign");
	m_pWnd = m_pGUI->CreateWnd(_T("\\desktop"), &element);


	m_pWndCaption	= m_pWnd->GetChild(_T("cr_pic"));
	m_pName			= (GUIEditBox*)m_pWndCaption->GetChild(_T("cr_name"));	
	m_pStcErrHint	= (GUIStatic*)m_pWndCaption->GetChild(_T("reg"));
	m_pStcSex		= (GUIStatic*)m_pWndCaption->GetChild(_T("rolesex"));
	m_pSexMutex		= (GUIFrameMutex*)m_pWndCaption->GetChild(_T("rolesex\\choose1"));
	m_pMalePBtn		= (GUIPushButton*)m_pWndCaption->GetChild(_T("rolesex\\choose1\\mbutt"));
	m_pFmalePBtn	= (GUIPushButton*)m_pWndCaption->GetChild(_T("rolesex\\choose1\\fmbutt"));

	m_pLBtnHType	= (GUIButton*)m_pWndCaption->GetChild(_T("rh_butt1"));		//发型
	m_pRBtnHType	= (GUIButton*)m_pWndCaption->GetChild(_T("rh_butt2"));
	m_pStcHType		= (GUIStatic*)m_pWndCaption->GetChild(_T("rh_type"));

	m_pLBtnHColor	= (GUIButton*)m_pWndCaption->GetChild(_T("hc_butt1"));		//发色
	m_pRBtnHColor	= (GUIButton*)m_pWndCaption->GetChild(_T("hc_butt2"));
	m_pStcHColor	= (GUIStatic*)m_pWndCaption->GetChild(_T("hc_type"));

	m_pLBtnFace		= (GUIButton*)m_pWndCaption->GetChild(_T("rf_butt1"));		//面部
	m_pRBtnFace		= (GUIButton*)m_pWndCaption->GetChild(_T("rf_butt2"));
	m_pStcFace		= (GUIStatic*)m_pWndCaption->GetChild(_T("rf_type"));

	m_pLBtnFDetail	= (GUIButton*)m_pWndCaption->GetChild(_T("rfd_butt1"));		//面部细节
	m_pRBtnFDetail	= (GUIButton*)m_pWndCaption->GetChild(_T("rfd_butt2"));
	m_pStcFDetail	= (GUIStatic*)m_pWndCaption->GetChild(_T("rfd_type"));

	m_pLBtnDress	= (GUIButton*)m_pWndCaption->GetChild(_T("rc_butt1"));		//服式
	m_pRBtnDress	= (GUIButton*)m_pWndCaption->GetChild(_T("rc_butt2"));
	m_pStcFDress	= (GUIStatic*)m_pWndCaption->GetChild(_T("rc_type"));

	m_pWndJob		= (GUIWnd*)m_pWndCaption->GetChild(_T("intro_win"));
	m_pBtnShenBing	= (GUIButton*)m_pWndCaption->GetChild(_T("jobpreview\\shenbing"));		//   神兵
	m_pBtnXuanLing	= (GUIButton*)m_pWndCaption->GetChild(_T("jobpreview\\youxia"));		//   玄翎
	m_pBtnQianKun	= (GUIButton*)m_pWndCaption->GetChild(_T("jobpreview\\qiankun"));		//   乾坤
	m_pBtnXianYin	= (GUIButton*)m_pWndCaption->GetChild(_T("jobpreview\\xianyin"));		//   仙音
	m_pStcIntor		= (GUIStaticEx*)m_pWndCaption->GetChild(_T("intro_win\\introword"));
	m_pScrollBar	= (GUIScrollBar*)m_pWndCaption->GetChild(_T("intro_win\\scr"));

	//m_pBtnSword = (GUIButton*)m_pWndCaption->GetChild( _T("swordbutt") );			//刀
	//m_pBtnBlade = (GUIButton*)m_pWndCaption->GetChild( _T("knifebutt") );			//剑
	//m_pBtnWand  = (GUIButton*)m_pWndCaption->GetChild( _T("weaponbutt") );			//长兵
	//m_pBtnBow   = (GUIButton*)m_pWndCaption->GetChild( _T("bowbutt") );				//弓
	//m_pBtnOrgan = (GUIButton*)m_pWndCaption->GetChild( _T("warebutt") );			//法器
	//m_pBtnLongShou	= (GUIButton*)m_pWndCaption->GetChild( _T("handbutt") );		//拢手
	//m_pBtnQin = (GUIButton*)m_pWndCaption->GetChild( _T("pianobutt") );				//琴
	//m_pBtnAmorSuit = (GUIButton*)m_pWndCaption->GetChild( _T("suitbutt") );			//铠甲套装
	//m_pBtnClothAmorSuit	= (GUIButton*)m_pWndCaption->GetChild( _T("suitbutt2") );	//布甲套装
	//m_pBtnFashion1 = (GUIButton*)m_pWndCaption->GetChild( _T("clothesbutt") );		//时装1
	//m_pBtnFashion2 = (GUIButton*)m_pWndCaption->GetChild( _T("clothesbutt1") );		//时装2

	m_pBtnOK		= (GUIButton*)m_pWndCaption->GetChild(_T("surebutt"));		//确定
	m_pBtnOK->SetEnable(FALSE);
	m_pBtnRandom	= (GUIButton*)m_pWndCaption->GetChild(_T("randbutt"));		//随机
	m_pBtnCancel	= (GUIButton*)m_pWndCaption->GetChild(_T("backbutt"));		//返回


	tstring strPath1 = g_strLocalPath + _T("\\ui\\camerabutt.xml");
	m_pGUI->LoadXml(&element, "VFS_System", strPath1.c_str());
	m_pWnd_Control  = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	m_pLBtnRotate = (GUIButton*)m_pWnd_Control->GetChild(_T("turnright"));
	m_pRBtnRotate = (GUIButton*)m_pWnd_Control->GetChild(_T("turnleft"));
	m_pPBtn_Push = (GUIPushButton*)m_pWnd_Control->GetChild(_T("fra\\zoomin"));
	m_pPBtn_Pull = (GUIPushButton*)m_pWnd_Control->GetChild(_T("fra\\zoomout"));
	m_pPBtn_Pull->SetState(EGUIBS_PopUp);


	m_pGUI->RegisterEventHandler(m_pWnd->GetFullName().c_str(),			m_Trunk.sfp1(&CreateRoleFrame::EventHandler));
	m_pGUI->RegisterEventHandler(m_pWnd_Control->GetFullName().c_str(), m_Trunk.sfp1(&CreateRoleFrame::EventHandler));


	if( P_VALID(m_pWnd_Control) )
		m_pWnd_Control->SetInvisible( FALSE );

	//tstring strPath2 = g_strLocalPath + _T("\\ui\\introduce.xml");
	//m_pGUI->LoadXml(&element, "VFS_System", strPath2.c_str());
	//m_pWndWeaponIntro = m_pGUI->CreateWnd(_T("\\desktop"), &element);
	//m_pStcIntroNum  =  (GUIStatic*)m_pWndWeaponIntro->GetChild(_T("introduce_pic\\num0"));
	//m_pStcIntroText = (GUIStaticEx*)m_pWndWeaponIntro->GetChild(_T("introduce_pic\\word0"));
	//m_pBtnPreIntro  = (GUIButton*)m_pWndWeaponIntro->GetChild(_T("introduce_pic\\backbutt"));
	//m_pBtnNextIntro = (GUIButton*)m_pWndWeaponIntro->GetChild(_T("introduce_pic\\nextbutt"));
	//m_pGUI->RegisterEventHandler(m_pWndWeaponIntro->GetFullName().c_str(), m_Trunk.sfp1(&CreateRoleFrame::WeaponIntroEnvent));
 //   m_pWndWeaponIntro->SetInvisible( TRUE );
       

	m_pGUI->SetActive(m_pName);

	return TRUE;
}

//-----------------------------------------------------------------------------
//! 事件处理
//! 
//-----------------------------------------------------------------------------
DWORD CreateRoleFrame::EventHandler(tagGUIEvent* pEvent)
{
	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
	if( !P_VALID(pWnd) )
		return FALSE;

	switch( pEvent->eEvent )
	{
	case EGUIE_EditBoxEnter:
	case EGUIE_Click:
		{
			if( pWnd == m_pBtnShenBing )
			{
				CreateBackGroud((INT)ETT_Blade);
				m_pStcIntor->SetText(g_StrTable[_T("Introduce_ShenBing")]);
				RefreshCareerIntro();
				ShowCareerEquip((INT)ETT_Blade);
			}
			else if( pWnd == m_pBtnXuanLing )
			{
				CreateBackGroud((INT)ETT_Bow);
				m_pStcIntor->SetText(g_StrTable[_T("Introduce_XuanLing")]);
				RefreshCareerIntro();
				ShowCareerEquip((INT)ETT_Bow);
			}
			else if( pWnd == m_pBtnQianKun )
			{
				CreateBackGroud((INT)ETT_Valor);
				m_pStcIntor->SetText(g_StrTable[_T("Introduce_QianKun")]);
				RefreshCareerIntro();
				ShowCareerEquip((INT)ETT_Valor);
			}
			else if( pWnd == m_pBtnXianYin )
			{
				CreateBackGroud((INT)ETT_Swing);
				m_pStcIntor->SetText(g_StrTable[_T("Introduce_XianYin")]);
				RefreshCareerIntro();
				ShowCareerEquip((INT)ETT_Swing);
			}
			else if( pWnd==m_pBtnOK || pWnd==m_pName )		//确定
			{
				tstring szName = m_pName->GetText();
				this->FilterString(szName);
				if( _tcslen(szName.c_str())!=0 && (szName[0]>='0' && szName[0]<='9') )
				{
					m_pStcErrHint->SetText(g_StrTable[_T("CreateRole_NameBad")]);
					m_pName->SetText(_T(""));
					break;
				}

				if( FilterData::Inst()->IsValidName(szName.c_str(), m_nRoleNameLength)!=E_Success )
				{
					m_pStcErrHint->SetText(g_StrTable[_T("CreateRole_NameBad")]);
					m_pName->SetText(_T(""));
					break;
				}

				if(SaveAccount::Inst()->GetLoginMode() == 11  && FilterData::Inst()->IsValidName(szName.c_str(), m_nRoleNameLength,2)!=E_Success)
				{
					m_pStcErrHint->SetText(g_StrTable[_T("CreateRole_NameBad")]);
					m_pName->SetText(_T(""));
					break;
				}

				if(!m_bSentCreate)							//没有发送过创建角色消息
				{
					m_bSentCreate = true;
					//与服务器消息传递
					tagNC_CreateRole msgCreateRole;
					_tcscpy(msgCreateRole.szRoleName, szName.c_str());
					msgCreateRole.eStatus		= m_eStatus;
					msgCreateRole.AvatarAtt		= m_avatarAtt;
					m_pSession->Send(&msgCreateRole);
					SwitchToState(ECRState_WaitRepose);
				}
			}
			else if(pWnd == m_pBtnRandom)					//随机
			{
				RandAvatarAtt(m_avatarAtt, true);
				AvatarCfg::Inst()->MakeNewPlayerFahion(m_avatarAtt.bySex,m_avatarAtt.wDressMdlID,m_avatarEquip);
				m_pPlayer->UpdateAvatar(m_avatarAtt,m_avatarEquip,SeleRoleMgr::Inst()->GetHugeSG());
				UpdateAttUI(false);
				
				m_step = EStep_End;
			}
			else if(pWnd == m_pBtnCancel)					//返回
			{
				SwitchToState(ECRState_Close);
			}
			else if(pWnd == m_pLBtnHType)					//发型左按钮
			{
				int id = m_avatarAtt.wHairMdlID;
				id--;
				if(id<0)
					id = AvatarCfg::Inst()->GetNumHairType()-1;

				m_avatarAtt.wHairMdlID = id;
				AvatarCfg::Inst()->MakeNewPlayerFahion( m_avatarAtt.bySex, m_avatarAtt.wDressMdlID, m_avatarEquip );
				m_pPlayer->UpdateAvatar(m_avatarAtt,m_avatarEquip,SeleRoleMgr::Inst()->GetHugeSG());

				UpdateAttUI(false);
				//RefreshCamera(true);
				//m_pPBtn_Pull->SetState(EGUIBS_PushDown, FALSE);
				//m_pPBtn_Push->SetState(EGUIBS_PopUp, FALSE);

				//角色装备预览
				m_eEquipType = ECRP_Null;
				m_eLeftEquipType = ECRP_Null;
				m_step = EStep_End;
			}
			else if(pWnd == m_pRBtnHType)					//发型右按钮
			{
				int id = m_avatarAtt.wHairMdlID;
				id++;
				if(id>=AvatarCfg::Inst()->GetNumHairType())
					id = 0;

				m_avatarAtt.wHairMdlID = id;
				AvatarCfg::Inst()->MakeNewPlayerFahion( m_avatarAtt.bySex, m_avatarAtt.wDressMdlID, m_avatarEquip );
				m_pPlayer->UpdateAvatar(m_avatarAtt,m_avatarEquip,SeleRoleMgr::Inst()->GetHugeSG());

				UpdateAttUI(false);
				//RefreshCamera(true);
				//m_pPBtn_Pull->SetState(EGUIBS_PushDown, FALSE);
				//m_pPBtn_Push->SetState(EGUIBS_PopUp, FALSE);

				//角色装备预览
				m_eEquipType = ECRP_Null;
				m_eLeftEquipType = ECRP_Null;
				m_step = EStep_End;
			}
			else if(pWnd == m_pLBtnHColor)					//发色左按钮
			{
				int id = m_avatarAtt.wHairTexID;
				id--;
				if(id<0)
					id = AvatarCfg::Inst()->GetNumHairColor()-1;

				m_avatarAtt.wHairTexID = id;
				AvatarCfg::Inst()->MakeNewPlayerFahion( m_avatarAtt.bySex, m_avatarAtt.wDressMdlID, m_avatarEquip );
				m_pPlayer->UpdateAvatar(m_avatarAtt,m_avatarEquip,SeleRoleMgr::Inst()->GetHugeSG());

				UpdateAttUI(false);
				//RefreshCamera(true);
				//m_pPBtn_Pull->SetState(EGUIBS_PushDown, FALSE);
				//m_pPBtn_Push->SetState(EGUIBS_PopUp, FALSE);

				//角色装备预览
				m_eEquipType = ECRP_Null;
				m_eLeftEquipType = ECRP_Null;
				m_step = EStep_End;
			}
			else if(pWnd == m_pRBtnHColor)					//发色右按钮
			{
				int id = m_avatarAtt.wHairTexID;
				id++;
				if(id>=AvatarCfg::Inst()->GetNumHairColor())
					id = 0;

				m_avatarAtt.wHairTexID = id;
				AvatarCfg::Inst()->MakeNewPlayerFahion( m_avatarAtt.bySex, m_avatarAtt.wDressMdlID, m_avatarEquip );
				m_pPlayer->UpdateAvatar(m_avatarAtt,m_avatarEquip,SeleRoleMgr::Inst()->GetHugeSG());

				UpdateAttUI(false);
				//RefreshCamera(true);
				//m_pPBtn_Pull->SetState(EGUIBS_PushDown, FALSE);
				//m_pPBtn_Push->SetState(EGUIBS_PopUp, FALSE);

				//角色装备预览
				m_eEquipType = ECRP_Null;
				m_eLeftEquipType = ECRP_Null;
				m_step = EStep_End;
			}
			else if(pWnd == m_pLBtnFace)					//面部左按钮
			{
				int id = m_avatarAtt.wFaceMdlID;
				id--;
				if(id<0)
					id = AvatarCfg::Inst()->GetNumFace()-1;

				m_avatarAtt.wFaceMdlID = id;
				AvatarCfg::Inst()->MakeNewPlayerFahion( m_avatarAtt.bySex, m_avatarAtt.wDressMdlID, m_avatarEquip );
				m_pPlayer->UpdateAvatar(m_avatarAtt,m_avatarEquip,SeleRoleMgr::Inst()->GetHugeSG());

				UpdateAttUI(false);
				//RefreshCamera(false);
				//m_pPBtn_Push->SetState(EGUIBS_PushDown, FALSE);
				//m_pPBtn_Pull->SetState(EGUIBS_PopUp, FALSE);

				//角色装备预览
				m_eEquipType = ECRP_Null;
				m_eLeftEquipType = ECRP_Null;
				m_step = EStep_End;
			}
			else if(pWnd == m_pRBtnFace)					//面部右按钮
			{
				int id = m_avatarAtt.wFaceMdlID;
				id++;
				if(id>=AvatarCfg::Inst()->GetNumFace())
					id = 0;

				m_avatarAtt.wFaceMdlID = id;
				AvatarCfg::Inst()->MakeNewPlayerFahion( m_avatarAtt.bySex, m_avatarAtt.wDressMdlID, m_avatarEquip );
				m_pPlayer->UpdateAvatar(m_avatarAtt,m_avatarEquip,SeleRoleMgr::Inst()->GetHugeSG());

				UpdateAttUI(false);
				//RefreshCamera(false);
				//m_pPBtn_Push->SetState(EGUIBS_PushDown, FALSE);
				//m_pPBtn_Pull->SetState(EGUIBS_PopUp, FALSE);

				//角色装备预览
				m_eEquipType = ECRP_Null;
				m_eLeftEquipType = ECRP_Null;
				m_step = EStep_End;
			}
			else if(pWnd == m_pLBtnFDetail)					//面部细节左按钮
			{
				int id = m_avatarAtt.wFaceDetailTexID;
				id--;
				if(id<0)
					id = AvatarCfg::Inst()->GetNumFaceDetail()-1;

				m_avatarAtt.wFaceDetailTexID = id;
				AvatarCfg::Inst()->MakeNewPlayerFahion( m_avatarAtt.bySex, m_avatarAtt.wDressMdlID, m_avatarEquip );
				m_pPlayer->UpdateAvatar(m_avatarAtt,m_avatarEquip,SeleRoleMgr::Inst()->GetHugeSG());

				UpdateAttUI(false);
				//RefreshCamera(false);
				//m_pPBtn_Push->SetState(EGUIBS_PushDown, FALSE);
				//m_pPBtn_Pull->SetState(EGUIBS_PopUp, FALSE);

				//角色装备预览
				m_eEquipType = ECRP_Null;
				m_eLeftEquipType = ECRP_Null;
				m_step = EStep_End;
			}
			else if(pWnd == m_pRBtnFDetail)					//面部细节右按钮
			{
				int id = m_avatarAtt.wFaceDetailTexID;
				id++;
				if(id>=AvatarCfg::Inst()->GetNumFaceDetail())
					id = 0;

				m_avatarAtt.wFaceDetailTexID = id;
				AvatarCfg::Inst()->MakeNewPlayerFahion( m_avatarAtt.bySex, m_avatarAtt.wDressMdlID, m_avatarEquip );
				m_pPlayer->UpdateAvatar(m_avatarAtt,m_avatarEquip,SeleRoleMgr::Inst()->GetHugeSG());

				UpdateAttUI(false);
				//RefreshCamera(false);
				//m_pPBtn_Push->SetState(EGUIBS_PushDown, FALSE);
				//m_pPBtn_Pull->SetState(EGUIBS_PopUp, FALSE);

				//角色装备预览
				m_eEquipType = ECRP_Null;
				m_eLeftEquipType = ECRP_Null;
				m_step = EStep_End;
			}
			else if(pWnd == m_pLBtnDress)					//服式左按钮
			{
				int id = m_avatarAtt.wDressMdlID;
				id--;
				if(id<0)
					id = AvatarCfg::Inst()->GetNumDress()-1;
				m_avatarAtt.wDressMdlID = id;
				AvatarCfg::Inst()->MakeNewPlayerFahion( m_avatarAtt.bySex, id, m_avatarEquip );
				m_pPlayer->UpdateAvatar(m_avatarAtt,m_avatarEquip,SeleRoleMgr::Inst()->GetHugeSG());
				UpdateAttUI(false);
				//RefreshCamera(true);
				m_pPBtn_Pull->SetState(EGUIBS_PushDown, FALSE);
				m_pPBtn_Push->SetState(EGUIBS_PopUp, FALSE);

				//角色装备预览
				m_eEquipType = ECRP_Null;
				m_eLeftEquipType = ECRP_Null;
				m_step = EStep_End;
			}
			else if(pWnd == m_pRBtnDress)					//服式右按钮
			{
				int id = m_avatarAtt.wDressMdlID;
				id++;
				if(id>=AvatarCfg::Inst()->GetNumDress())
					id = 0;
				m_avatarAtt.wDressMdlID = id;
				AvatarCfg::Inst()->MakeNewPlayerFahion( m_avatarAtt.bySex, id, m_avatarEquip );
				m_pPlayer->UpdateAvatar(m_avatarAtt,m_avatarEquip,SeleRoleMgr::Inst()->GetHugeSG());
				UpdateAttUI(false);
				//RefreshCamera(true);
				m_pPBtn_Pull->SetState(EGUIBS_PushDown, FALSE);
				m_pPBtn_Push->SetState(EGUIBS_PopUp, FALSE);

				//角色装备预览
				m_eEquipType = ECRP_Null;
				m_eLeftEquipType = ECRP_Null;
				m_step = EStep_End;
			}
			else if(pWnd==m_pLBtnRotate || pWnd==m_pRBtnRotate)
			{
				m_bRotate = false;
				m_bRRotate = false;
				m_bLRotate = false;
			}
			//else
			//{
			//	ECreateRolePreview eCreateRole = ParseEquipSelect( pWnd );
			//	if( eCreateRole == ECRP_Null )
			//		break;
			//	WeaponIntro(eCreateRole);
			//	ProcessEquipPreview( eCreateRole );	
			//}
		}
		break;
	case EGUIE_PushButtonDown:
		{
			if(pEvent->dwParam1 != 1)
				break;
			if( pWnd == m_pMalePBtn )				//男
			{
				memset(&m_avatarAtt, 0x0, sizeof(m_avatarAtt));
				m_avatarAtt.bySex = 1;
				AvatarCfg::Inst()->MakeNewPlayerFahion( m_avatarAtt.bySex, 0, m_avatarEquip );
				m_pPlayer->UpdateAvatar(m_avatarAtt,m_avatarEquip,SeleRoleMgr::Inst()->GetHugeSG());
				m_pPlayer->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Stand,AvatarAniMixer::EAP_All,_T("std1"),true, true, 0.0f, 0.0f);
				m_pPlayer->SetYaw( 0, false );			
				m_pPlayer->Update();	
				UpdateAttUI(false);
			}
			else if( pWnd == m_pFmalePBtn )			//女
			{
				memset(&m_avatarAtt, 0x0, sizeof(m_avatarAtt));
				ZeroMemory(&m_avatarEquip,sizeof(m_avatarEquip));
				m_avatarAtt.bySex = 0;
				AvatarCfg::Inst()->MakeNewPlayerFahion( m_avatarAtt.bySex, 0, m_avatarEquip );
				m_pPlayer->UpdateAvatar(m_avatarAtt,m_avatarEquip,SeleRoleMgr::Inst()->GetHugeSG());
				m_pPlayer->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Stand,AvatarAniMixer::EAP_All,_T("std1"),true, true, 0.0f, 0.0f);
				m_pPlayer->SetYaw( 0, false );			
				m_pPlayer->Update();	
				UpdateAttUI(false);
			}
			else if( pWnd==m_pPBtn_Push && EGUIBS_PushDown==pEvent->dwParam1 )
			{
				RefreshCamera(false);
			}
			else if( pWnd==m_pPBtn_Pull && EGUIBS_PushDown==pEvent->dwParam1 )
			{
				RefreshCamera(true);
			}
		}
		break;
	case EMouse_LeftDown:
		{
			if(	pWnd == m_pLBtnRotate )
			{
				m_bRotate = true;
				m_bRRotate = false;
				m_bLRotate = true;
			}
			else if( pWnd == m_pRBtnRotate )
			{
				m_bRotate = true;
				m_bRRotate = true;
				m_bLRotate = false;
			}
		}
		break;
	}

	return FALSE;
}

VOID CreateRoleFrame::Render3D()
{
	float deltaTime=Kernel::Inst()->GetDeltaTime();

	SeleRoleMgr::Inst()->UpdateSG(deltaTime);
	if(P_VALID(m_pTex))
	{
		IDraw2D::Inst()->BeginDraw(IDraw2D::EBlend_Alpha, true);
		IDraw2D::Inst()->Draw( &m_rectDest, &m_rectSrc, m_pTex, 0xFFFFFFFF, ETFilter_POINT, 0, IDraw2D::EAWT_Write, 0.5f );
		IDraw2D::Inst()->EndDraw();	
	}
		
	if(m_bRotate == true )
	{	
		m_pPlayer->SetYaw( m_fRoleYaw, false );			//绕Y轴旋转
		m_pPlayer->Update();	
	}	

	GameFrame::Render3D();
}


//-----------------------------------------------------------------------------
// update
//-----------------------------------------------------------------------------
VOID CreateRoleFrame::Update()
{
	float deltaTime = Kernel::Inst()->GetDeltaTime();

	switch(m_curState)
	{
	case ECRState_Cartoon:
		{

		}
		break;
	case ECRState_Inputing:					//等待玩家输入信息
		{
			CanRolerAnimation(deltaTime);

			CheckMouseLeftDown(deltaTime);			//左右旋转鼠标按下

			if(CheckInputedName())			//当玩家姓名，安全码不为空时，且没有发送过创建消息时，确定按钮才可用
			{
				m_pBtnOK->SetEnable(TRUE);
				m_pBtnOK->SetRefresh();
			}
			else
			{
				m_pBtnOK->SetEnable(FALSE);
				m_pBtnOK->SetRefresh();
			}

			RoleRotate(deltaTime);			//角色旋转角度计算

		}
		break;
	case ECRState_WaitRepose:
		{

		}
		break;
	case ECRState_PlayAction:				//输入数据后,播放一段动画
		{
			if( m_pPlayer->GetAniPlayer()->IsTrackEnd(AvatarAniPlayer::EAT_Stand,_T("std3")) )
			{
				SwitchToState(ECRState_Close);
			}
		}
		break;
	}

	//if( m_bPlayPreview )
	//	PlayEquipSkeleton();			//展示装备的不同阶段播放不同的动画

	GameFrame::Update();
}

bool CreateRoleFrame::IsPickRole(int mouseX, int mouseY)
{
	if(m_pPlayer==NULL) return false;
	Ray ray;
	CameraEuler* camera = SeleRoleMgr::Inst()->GetCamera();
	camera->GetMouseRay(mouseX,mouseY,ray);

	TResult tr;

	AABBox box;
	m_pPlayer->GetSceneNode()->GetBox(box);

	tr=ray.Intersect(box);

	if(tr.first && tr.second<FLOAT_MAX)
		return true;
	else
		return false;
}

//角色旋转
void CreateRoleFrame::RoleRotate(float deltaTime)
{
	if(m_bLRotate==true && m_bRotate==true)			//角色左旋转
	{
		if(P_VALID(m_pLBtnRotate))
			m_fRoleYaw = m_pPlayer->GetYaw() - _DegToRad(2)*deltaTime*ROTATE_SPEED;
	}
	else if(m_bRotate==true && m_bRRotate==true)	//角色右旋转
	{
		if(P_VALID(m_pRBtnRotate))
			m_fRoleYaw = m_pPlayer->GetYaw() + _DegToRad(2)*deltaTime*ROTATE_SPEED;								
	}	
}

const tagCameraData* pCameraData	= CameraSetData::Inst()->GetData();

//开始创建角色
void CreateRoleFrame::BeginCreateRole()
{
	PREROLE_POS	= Vector3(pCameraData->dwCreateRolePos_x, pCameraData->dwCreateRolePos_y, pCameraData->dwCreateRolePos_z);
	Lookat		= Vector3(pCameraData->dwCreateCameraLookat_x,pCameraData->dwCreateCameraLookat_y,pCameraData->dwCreateCameraLookat_z);
	LookatNear	= Vector3(pCameraData->dwCreateCameraNearLookat_x,pCameraData->dwCreateCameraNearLookat_y,pCameraData->dwCreateCameraNearLookat_z);
	const float CamYaw=_DegToRad(pCameraData->dwCreateCamYaw);
	const float CamPitch=_DegToRad(pCameraData->dwCreateCamPitch);
	const float CamRoll=0.0f;
	const float MAXCamDist=pCameraData->dwCreateMAXCamDist;

	SwitchToState(ECRState_Inputing);

	m_avatarAtt.bySex = 0;			//0:女 1:男

	m_pPlayer			= new Player;						//创建一个Node
	if( P_VALID(m_pPlayer) )
	{
		SeleRoleMgr::Inst()->OpenMap(_T("cj-1"), SeleRoleMgr::Inst()->GetCamera(), SeleRoleMgr::ECreate_Role);

		AvatarCfg::Inst()->MakeNewPlayerFahion( m_avatarAtt.bySex, 0, m_avatarEquip );
		tagDisplaySet displaySet;
		m_pPlayer->LoadAvatar(m_avatarAtt, m_avatarEquip, displaySet);
		m_pPlayer->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_All,_T("std1"),true, false, 0.0f, 0.0f);
		m_pPlayer->SetPos(PREROLE_POS);
		m_pPlayer->Update();
		//把Node加入到场景中
		m_pPlayer->AttachSceneNodeToSG(SeleRoleMgr::Inst()->GetHugeSG());	
		SeleRoleMgr::Inst()->SetCamera(Lookat, CamYaw, CamPitch, CamRoll, MAXCamDist);
	}

	//设置界面
	RandAvatarAtt(m_avatarAtt);	
	UpdateAttUI(false);
}


//结束创建角色
void CreateRoleFrame::EndCreateRole()
{
	m_pPlayer->DetachSceneNodeFromSG(SeleRoleMgr::Inst()->GetHugeSG());		//从场景中删除结点
	SeleRoleMgr::Inst()->CloseMap();
	SelectRoleFrame* selRoleFrame = (SelectRoleFrame *)m_pFrameMgr->CreateFrame(_T("Root"), _T("SelectRole"), _T("SelectRoleFrame"), 0);
	selRoleFrame->SetCurState(SelectRoleFrame::ESRS_Loading);

	LoadingFrame* pFrame = (LoadingFrame*)m_pFrameMgr->CreateFrame(_T("Root"), _T("Loading"), _T("LoadingFrame"), 0);
	if( P_VALID(pFrame) )
		pFrame->SetLoadingType(LoadingFrame::ELT_SelectRole);
	m_pFrameMgr->AddToDestroyList(this);		

}


//更新属性设置
void CreateRoleFrame::UpdateAttUI(bool bSave)
{
	if(m_avatarAtt.bySex==1)			//男
	{
		if(P_VALID(m_pSexMutex))
			m_pSexMutex->SetPush(m_pMalePBtn);
	}
	else								//女
	{
		if(P_VALID(m_pSexMutex))
			m_pSexMutex->SetPush(m_pFmalePBtn);
	}

	TCHAR szTemp[256] = {0};
	_stprintf(szTemp, _T("%d"), m_avatarAtt.wHairMdlID+1);			//设置发型
	if(P_VALID(m_pStcHType))
		m_pStcHType->SetText(szTemp);

	_stprintf(szTemp, _T("%d"), m_avatarAtt.wHairTexID+1);			//设置发色
	if(P_VALID(m_pStcHColor))
		m_pStcHColor->SetText(szTemp);

	_stprintf(szTemp, _T("%d"), m_avatarAtt.wFaceMdlID+1);			//设置面部
	if(P_VALID(m_pStcFace))
		m_pStcFace->SetText(szTemp);

	_stprintf(szTemp, _T("%d"), m_avatarAtt.wFaceDetailTexID+1);	//设置面部细节
	if(P_VALID(m_pStcFDetail))
		m_pStcFDetail->SetText(szTemp);

	_stprintf(szTemp, _T("%d"), m_avatarAtt.wDressMdlID+1);			//设置服式
	if(P_VALID(m_pStcFDress))
		m_pStcFDress->SetText(szTemp);

}


//状态转换开关
void CreateRoleFrame::SwitchToState(ECRState nextState)
{
	m_curState = nextState;
	switch(m_curState)
	{
	case ECRState_Inputing:
		{
			m_bSentCreate = false;
			if( P_VALID(m_pName) )
				m_pName->SetText(_T(""));
			if( P_VALID(m_pBtnOK) )
				m_pBtnOK->SetEnable(true);
		}
		break;
	case ECRState_WaitRepose:
		{
			if( P_VALID(m_pStcErrHint) )
				m_pStcErrHint->SetText(g_StrTable[_T("WaitAuth")]);
			m_pBtnOK->SetEnable(false);
		}
		break;
	case ECRState_PlayAction:
		{
			if( P_VALID(m_pWnd) )
				m_pWnd->SetInvisible(TRUE);
			if( P_VALID(m_pWnd_Control) )
				m_pWnd_Control->SetInvisible(TRUE);
			m_pPlayer->GetAniPlayer()->PlayTrack(AvatarAniPlayer::EAT_Stand,AvatarAniMixer::EAP_All,_T("std3"),false/*, false, 0.0f, 0.0f*/);
		}
		break;
	case ECRState_Close:
		{	
			EndCreateRole();
		}
		break;
	}
}

//当玩家输入了姓名后并且没有发过创建消息，确定按钮才可用
BOOL CreateRoleFrame::CheckInputedName()
{
	if( !P_VALID(m_pName) )
		return FALSE;
	tstring name = m_pName->GetText();
	if( name.c_str()==NULL || _tcslen(name.c_str())==0 || m_bSentCreate)
		return FALSE;
	return TRUE;
}

//随机设置角色属性
void CreateRoleFrame::RandAvatarAtt(tagAvatarAtt& att,bool bRandAll)
{
	if(bRandAll)
	{
		att.wHairMdlID			= m_pUtil->Rand() % AvatarCfg::Inst()->GetNumHairType();
		att.wHairTexID			= m_pUtil->Rand() % AvatarCfg::Inst()->GetNumHairColor();
		att.wFaceMdlID			= m_pUtil->Rand() % AvatarCfg::Inst()->GetNumFace();
		//att.wFaceDetailTexID	= m_pUtil->Rand() % AvatarCfg::Inst()->GetNumFaceDetail();
		att.wDressMdlID			= rand() % AvatarCfg::Inst()->GetNumDress();
	}
}

//鼠标左键按下旋转按钮
void CreateRoleFrame::CheckMouseLeftDown(float deltaTime)
{
	if(!P_VALID(m_pLBtnRotate))
		return;
	if(!P_VALID(m_pRBtnRotate))
		return;
	if(m_pLBtnRotate->GetState()==1 && m_pGUI->GetMousePoint()==m_pLBtnRotate)				//左旋转鼠标按下
	{
		tagGUIEvent event(m_pLBtnRotate, (EGUIEvent)EMouse_LeftDown);
		m_pLBtnRotate->SendEvent(&event);
	}
	else if(m_pRBtnRotate->GetState()==1 && m_pGUI->GetMousePoint()==m_pRBtnRotate)			//右旋转鼠标按下
	{
		tagGUIEvent event(m_pRBtnRotate, (EGUIEvent)EMouse_LeftDown);
		m_pRBtnRotate->SendEvent(&event);
	}
	//拾取角色
	else if(m_pInput->IsMouseDownOnce(0)
		&& m_pGUI->GetActive()==m_pGUI->GetDesktop())
	{
		const tagPoint& ptCursor = CursorMgr::Inst()->GetCursorPos();
		if(IsPickRole(ptCursor.x, ptCursor.y))
			m_bPickRole = true;
		else 
			m_bPickRole = false;
	}
	//通过鼠标旋转角色
	else if(m_pInput->IsMouseDown(0)
		&& m_pInput->IsMouseMove() 
		&& m_bPickRole)
	{
		FLOAT fRelativeX = m_pInput->GetRelativeX();
		m_fRoleYaw = m_pPlayer->GetYaw() + _DegToRad(2)*fRelativeX*ROTATE_SPEED*deltaTime;
		m_bRotate = true;
	}
	else
	{
		m_bRotate = false;
		m_bRRotate = false;
		m_bLRotate = false;
	}	
}

//接收服务器消息(参数待定)
DWORD CreateRoleFrame::NetRecvCreateRole(tagNS_CreateRole* pMsg, DWORD pPrama)
{
	if( pMsg->dwID != m_pUtil->Crc32("NS_CreateRole") )
		return 0;

	//创建角色成功后，播放一段动画
	if( pMsg->dwErrorCode == E_Success )
	{
		tagSimRole* pRole = (tagSimRole *)pMsg->bySimRoleInfo;
		Player* pPlayer = new Player;
		SeleRoleMgr::Inst()->CreateRole(*pRole, pPlayer);
		m_pPlayer->SetYaw( 0, false );			//绕Y轴旋转
		m_pPlayer->Update();	
		RefreshCamera(true);
		SwitchToState(ECRState_PlayAction);
		SeleRoleMgr::Inst()->SetSelRoleID( pRole->dwRoleID );
	}
	else
	{
		switch(pMsg->dwErrorCode)
		{
		case E_CreateRole_RoleCreating:
			m_pStcErrHint->SetText(g_StrTable[_T("CreateRole_RoleCreating")]);
			break;
		case E_CreateRole_RoleEnumNotDone:
			m_pStcErrHint->SetText(g_StrTable[_T("CreateRole_RoleEnumNotDone")]);
			break;
		case E_CreateRole_RoleDeleting:
			m_pStcErrHint->SetText(g_StrTable[_T("CreateRole_RoleDeleting")]);
			break;
		case E_CreateRole_RoleNumFull:
			m_pStcErrHint->SetText(g_StrTable[_T("CreateRole_RoleNumFull")]);
			break;
		case E_CreateRole_NameExist:
			m_pStcErrHint->SetText(g_StrTable[_T("CreateRole_NameExist")]);
			break;
		case E_CreateRole_NameInvalid:
			m_pStcErrHint->SetText(g_StrTable[_T("CreateRole_NameInvalid")]);
			break;
		case E_CreateRole_SafeCodeInvalid:
			m_pStcErrHint->SetText(g_StrTable[_T("CreateRole_SafeCodeInvalid")]);
			break;
		case E_CreateRole_InWorld:
			m_pStcErrHint->SetText(g_StrTable[_T("CreateRole_InWorld")]);
			break;
		case E_CreateRole_RoleLoading:
			m_pStcErrHint->SetText(g_StrTable[_T("CreateRole_RoleLoading")]);
			break;
		case E_SystemError:
			m_pStcErrHint->SetText(g_StrTable[_T("SystemError")]);
			break;
		case E_Filter_Name_TooLong:
			m_pStcErrHint->SetText(g_StrTable[_T("Common_Error30")]);
			break;
		case E_Filter_Name_Null:
			m_pStcErrHint->SetText(g_StrTable[_T("Common_Error31")]);
			break;
		case E_Filter_Name_Illegal:
		case E_Filter_Name_InFilterFile:
			m_pStcErrHint->SetText(g_StrTable[_T("CreateRole_NameInvalid")]);
			break;
		case E_CreateRole_CreateTimesFull:
			m_pStcErrHint->SetText(g_StrTable[_T("CreateRole_CreateTimesFull")]);
			break;
		default:
			m_pStcErrHint->SetText(g_StrTable[_T("SystemError")]);
			break;
		}
		m_bSentCreate = false;
		m_pBtnOK->SetEnable(true);
		m_pWnd->SetRefresh(TRUE);
		SwitchToState(ECRState_Inputing);
	}

	return 0;
}

void CreateRoleFrame::CanRolerAnimation(float time)
{
	switch(m_bAnimation)		// 检测镜头的位置，0初始值，1表示拉近，-1表示还原
	{
	case 1:		// 拉近
		{
			//if(m_fRolerTimer > 5.0f)
			//{	// 拉近角色后持续5秒没有动作则还原
			//	RefreshCamera(true);
			//	m_pPBtn_Push->SetState( EGUIBS_PopUp );
			//}
			//else
			//{
			//	m_fRolerTimer += time;		// 累计拉近角色后持续的时间
			//}
			if(m_fAnimDelay < 0.033f*4.0f)
			{
				m_fAnimDelay  += time;		// 延迟一段时间再改变镜头
			}
			else
			{
				SeleRoleMgr::Inst()->GetCamera()->Update(LookatNear);
			}
		}
		break;
	case -1:		// 拉远
		{			
			if(m_fAnimDelay < 0.033f*2.0f)
			{
				m_fAnimDelay  += time;		// 延迟一段时间再改变镜头
			}
			else 
			{
				SeleRoleMgr::Inst()->GetCamera()->Update(Lookat);
			}
		}
		break;
	case 0:		// Init
		{
			m_fRolerTimer = 0.0f;
			m_fAnimDelay  = 0.0f;
		}
		break;
	default:
		break;
	}    	
}

void CreateRoleFrame::RefreshCamera( bool bFar )
{
	if( bFar )
	{
		m_bAnimation  = -1;
		m_fRolerTimer = 0.0f;		
		m_fAnimDelay  = 0.0f;
	}
	else 
	{
		m_bAnimation  = 1;
		m_fRolerTimer = 0.0f;		
		m_fAnimDelay  = 0.0f;
	}
}

VOID CreateRoleFrame::FilterString( tstring& str )
{
	// 去掉回车
	INT nFind = 0;
	while( (nFind = str.find(_T('\r'), nFind)) != str.npos )
		str.replace(nFind, 1, _T(""));

	// 去掉换行
	nFind = 0;
	while( (nFind = str.find(_T('\n'), nFind)) != str.npos )
		str.replace(nFind, 1, _T(""));

}


//CreateRoleFrame::ECreateRolePreview CreateRoleFrame::ParseEquipSelect( GUIWnd* pWnd )
//{
//	if( pWnd == m_pBtnSword )
//	{
//		return ECRP_Sword;
//	}
//
//	if( pWnd == m_pBtnBlade )
//	{
//		return ECRP_Blade;	
//	}
//
//	if( pWnd == m_pBtnWand )
//	{
//		return ECRP_Wand;
//	}
//
//	if( pWnd == m_pBtnBow )
//	{
//		return ECRP_Bow;
//	}
//
//	if( pWnd == m_pBtnLongShou )
//	{
//		return ECRP_LongShou;
//	}
//
//	if( pWnd == m_pBtnOrgan )
//	{
//		return ECRP_Organ;
//	}
//
//	if( pWnd == m_pBtnQin )
//	{
//		return ECRP_Qin;
//	}
//
//	if( pWnd == m_pBtnAmorSuit )
//	{
//		return ECRP_Amor;
//	}
//
//	if( pWnd == m_pBtnClothAmorSuit )
//	{
//		return ECRP_ClothAmor;
//	}
//
//	if( pWnd == m_pBtnFashion1 )
//	{
//		return ECRP_FashionOne;
//	}
//
//	if( pWnd == m_pBtnFashion2 )
//	{
//		return ECRP_FashionTwo;
//	}
//
//	return ECRP_Null;
//}
//
//void CreateRoleFrame::PlayEquipSkeleton()
//{
//	switch( m_step )
//	{
//	case EStep_EnterCombat:
//		{
//			m_pPlayer->GetAniPlayer()->PlayTrack( AvatarAniPlayer::EAT_Other, AvatarAniMixer::EAP_Upper, m_szEnterTrackName.c_str(),false, false, 0.0f, 0.2f, AvatarAniMixer::EPri_Normal );
//			m_pPlayer->HandUpWeapon( true );	//--武器挂载到手上
//
//			m_step = EStep_HandUpWeapon;
//		}
//		break;
//	case EStep_HandUpWeapon:					//拔武器
//		{
//			if( m_pPlayer->GetAniPlayer()->IsTrackEnd( AvatarAniPlayer::EAT_Other,m_szEnterTrackName.c_str() ) )
//			{
//				//--上身同步到下身动作
//				m_pPlayer->GetAniPlayer()->Sync2Lower();
//				m_pPlayer->OpenAllEquip( true );
//				m_step = EStep_OpenEquip;
//			}
//		}
//		break;
//	case EStep_OpenEquip:						//展开装备（变形）
//		{
//			if( m_pPlayer->IsAllEquipOpened() )
//			{
//				m_step = EStep_Fighting;
//
//				//开始备战状态计时
//				m_dwStartTime = Kernel::Inst()->GetAccumTimeDW();
//			}
//		}
//		break;
//	case EStep_Fighting:						//战斗状态
//		{
//			if( Kernel::Inst()->GetAccumTimeDW() - m_dwStartTime >= 1000 )
//			{
//				m_pPlayer->OpenAllEquip( false );
//				m_step = EStep_CloseEquip;
//			}
//		}
//		break;
//	case EStep_CloseEquip:						//关闭装备（变形）
//		{
//			if( m_pPlayer->IsAllEquipClosed() )
//			{
//				//--播放收回武器动作
//				m_pPlayer->GetAniPlayer()->PlayTrack( AvatarAniPlayer::EAT_Other,AvatarAniMixer::EAP_Upper, m_szLeaveTrackName.c_str(),false, false, 0.0f, 0.2f, AvatarAniMixer::EPri_Normal );
//				m_step = EStep_HandDownWeapon;
//			}
//		}
//		break;
//	case EStep_HandDownWeapon:					//收武器
//		{
//			if( m_pPlayer->GetAniPlayer()->IsTrackEnd( AvatarAniPlayer::EAT_Other, m_szLeaveTrackName.c_str() ) )
//			{
//				m_pPlayer->HandUpWeapon( false );
//				m_pPlayer->GetAniPlayer()->Sync2Lower();		//--上身同步到下身动作
//				m_step = EStep_End;
//			}
//		}
//		break;
//	case EStep_End:
//		{
//			m_pPlayer->GetAniPlayer()->PlayTrack( AvatarAniPlayer::EAT_Stand,AvatarAniMixer::EAP_All,_T("std1"),true, false, 0.0f, 0.0f );
//			m_bPlayPreview = false;
//			m_szEnterTrackName.clear();
//			m_szLeaveTrackName.clear();
//			m_step = EStep_Over;
//		}
//	default:
//		break;
//	}
//}
//
//void CreateRoleFrame::ProcessEquipPreview( ECreateRolePreview eCreateRole )
//{
//	//按照性别得到配置信息
//	const tagCreateRolePreviewItem* pCfgItem = m_previewData.FindPreviewInfoBySex( m_avatarAtt.bySex );
//	if( !P_VALID(pCfgItem) )
//		return;
//
//	bool bHaveRight = true;
//	DWORD dwRTypeId = m_avatarEquip.AvatarEquip[EAE_RWeapon].dwTypeID;
//	if( dwRTypeId == 0 )
//		bHaveRight = false;
//
//	//if (eCreateRole>=ECRP_Sword&&eCreateRole<=ECRP_Qin)
//	//{
//	//	 m_iMaxTextNum = PageMaxNum(eCreateRole);
// //        m_iCurTextNum = 1;
//	//}
//	//
//
//
//	switch( eCreateRole )
//	{
//	case ECRP_Sword:
//		{
//			if( pCfgItem->SwordTypeId == 0 )
//				return;
//
//			if( bHaveRight )	//主手有武器，武器放到副手然后把当前选择的武器放到主手
//			{
//				m_avatarEquip.Set( EAE_LWeapon, m_avatarEquip.AvatarEquip[EAE_RWeapon].dwTypeID, 0, 0 );
//				m_avatarEquip.Set( EAE_RWeapon, pCfgItem->SwordTypeId, 0, 0 );
//				m_eLeftEquipType = m_eEquipType;
//			}
//			else				//主手没有武器直接将武器放到主手
//			{
//				m_avatarEquip.Set( EAE_RWeapon, pCfgItem->SwordTypeId, 0, 0 );
//			}
//
//			m_eEquipType = ECRP_Sword;
//			m_szEnterTrackName = CombatSysUtil::Inst()->GetEnterCombatTrackName( EITE_Sword );
//			m_szLeaveTrackName = CombatSysUtil::Inst()->GetLeaveCombatTrackName( EITE_Sword );
//			PrepareEffectSkeleton( pCfgItem, ECRP_Sword, m_eLeftEquipType );
//		}
//		break;
//	case ECRP_Blade:
//		{
//			if( pCfgItem->BladeTypeId == 0 )
//				return;
//
//			if( bHaveRight )	//主手有武器，武器放到副手然后把当前选择的武器放到主手
//			{
//				m_avatarEquip.Set( EAE_LWeapon, m_avatarEquip.AvatarEquip[EAE_RWeapon].dwTypeID, 0, 0 );
//				m_avatarEquip.Set( EAE_RWeapon,pCfgItem->BladeTypeId, 0, 0 );
//				m_eLeftEquipType = m_eEquipType;
//			}
//			else
//			{
//				m_avatarEquip.Set( EAE_RWeapon,pCfgItem->BladeTypeId, 0, 0 );
//			}
//
//			m_eEquipType = ECRP_Blade;
//			m_szEnterTrackName = CombatSysUtil::Inst()->GetEnterCombatTrackName( EITE_Blade );
//			m_szLeaveTrackName = CombatSysUtil::Inst()->GetLeaveCombatTrackName( EITE_Blade );
//			PrepareEffectSkeleton( pCfgItem, ECRP_Blade, m_eLeftEquipType );
//		}
//		break;
//	case ECRP_Wand:
//		{
//			if( pCfgItem->WandTypeId == 0 )
//				return;
//
//			if( bHaveRight )	//主手有武器，武器放到副手然后把当前选择的武器放到主手
//			{
//				m_avatarEquip.Set( EAE_LWeapon, m_avatarEquip.AvatarEquip[EAE_RWeapon].dwTypeID, 0, 0 );
//				m_avatarEquip.Set( EAE_RWeapon,pCfgItem->WandTypeId, 0, 0 );
//				m_eLeftEquipType = m_eEquipType;
//			}
//			else
//			{
//				m_avatarEquip.Set( EAE_RWeapon,pCfgItem->WandTypeId, 0, 0 );
//			}
//
//			m_eEquipType = ECRP_Wand;
//			m_szEnterTrackName = CombatSysUtil::Inst()->GetEnterCombatTrackName( EITE_Wand );
//			m_szLeaveTrackName = CombatSysUtil::Inst()->GetLeaveCombatTrackName( EITE_Wand );
//			PrepareEffectSkeleton( pCfgItem, ECRP_Wand, m_eLeftEquipType );
//		}
//		break;
//	case ECRP_Bow:
//		{
//			if( pCfgItem->BowTypeId == 0 )
//				return;
//
//			if( bHaveRight )	//主手有武器，武器放到副手然后把当前选择的武器放到主手
//			{
//				m_avatarEquip.Set( EAE_LWeapon, m_avatarEquip.AvatarEquip[EAE_RWeapon].dwTypeID, 0, 0 );
//				m_avatarEquip.Set( EAE_RWeapon, pCfgItem->BowTypeId, 0, 0 );
//				m_eLeftEquipType = m_eEquipType;
//			}
//			else
//			{
//				m_avatarEquip.Set( EAE_RWeapon, pCfgItem->BowTypeId, 0, 0 );
//			}
//
//			m_eEquipType = ECRP_Bow;
//			m_szEnterTrackName = CombatSysUtil::Inst()->GetEnterCombatTrackName( EITE_Bow );
//			m_szLeaveTrackName = CombatSysUtil::Inst()->GetLeaveCombatTrackName( EITE_Bow );
//			PrepareEffectSkeleton( pCfgItem, ECRP_Bow, m_eLeftEquipType );
//		}
//		break;
//	case ECRP_Organ:
//		{
//			if( pCfgItem->OrganTypeId == 0 )
//				return;
//
//			if( bHaveRight )	//主手有武器，武器放到副手然后把当前选择的武器放到主手
//			{
//				m_avatarEquip.Set( EAE_LWeapon, m_avatarEquip.AvatarEquip[EAE_RWeapon].dwTypeID, 0, 0 );
//				m_avatarEquip.Set( EAE_RWeapon, pCfgItem->OrganTypeId, 0, 0 );
//				m_eLeftEquipType = m_eEquipType;
//			}
//			else
//			{
//				m_avatarEquip.Set( EAE_RWeapon,pCfgItem->OrganTypeId, 0, 0 );
//			}
//
//			m_eEquipType = ECRP_Organ;
//			m_szEnterTrackName = CombatSysUtil::Inst()->GetEnterCombatTrackName( EITE_Faqi );
//			m_szLeaveTrackName = CombatSysUtil::Inst()->GetLeaveCombatTrackName( EITE_Faqi );
//			PrepareEffectSkeleton( pCfgItem, ECRP_Organ, m_eLeftEquipType );
//		}
//		break;
//	case ECRP_LongShou:
//		{
//			if( pCfgItem->LongShouTypeId == 0 )
//				return;
//
//			if( bHaveRight )	//主手有武器，武器放到副手然后把当前选择的武器放到主手
//			{
//				m_avatarEquip.Set( EAE_LWeapon, m_avatarEquip.AvatarEquip[EAE_RWeapon].dwTypeID, 0, 0 );
//				m_avatarEquip.Set( EAE_RWeapon,pCfgItem->LongShouTypeId, 0, 0 );
//				m_eLeftEquipType = m_eEquipType;
//			}
//			else
//			{
//				m_avatarEquip.Set( EAE_RWeapon,pCfgItem->LongShouTypeId, 0, 0 );
//			}
//
//			m_eEquipType = ECRP_LongShou;
//			m_szEnterTrackName = CombatSysUtil::Inst()->GetEnterCombatTrackName( EITE_Faqi );
//			m_szLeaveTrackName = CombatSysUtil::Inst()->GetLeaveCombatTrackName( EITE_Faqi );
//			PrepareEffectSkeleton( pCfgItem, ECRP_LongShou, m_eLeftEquipType  );
//		}
//		break;
//	case ECRP_Qin:
//		{
//			if( pCfgItem->QinTypeId == 0 )
//				return;
//
//			if( bHaveRight )	//主手有武器，武器放到副手然后把当前选择的武器放到主手
//			{
//				m_avatarEquip.Set( EAE_LWeapon, m_avatarEquip.AvatarEquip[EAE_RWeapon].dwTypeID, 0, 0 );
//				m_avatarEquip.Set( EAE_RWeapon,pCfgItem->QinTypeId, 0, 0 );
//				m_eLeftEquipType = m_eEquipType;
//			}
//			else
//			{
//				m_avatarEquip.Set( EAE_RWeapon,pCfgItem->QinTypeId, 0, 0 );
//			}
//
//			m_eEquipType = ECRP_Qin;
//
//			m_szEnterTrackName = CombatSysUtil::Inst()->GetEnterCombatTrackName( EITE_Qin );
//			m_szLeaveTrackName = CombatSysUtil::Inst()->GetLeaveCombatTrackName( EITE_Qin );
//			PrepareEffectSkeleton( pCfgItem, ECRP_Qin, m_eLeftEquipType );
//		}
//		break;
//	case ECRP_Amor:
//		{
//			//装备展开(变形)过程中不允许预览，否则会导致暂停在这个阶段
//			if( m_step == EStep_OpenEquip )
//				break;
//
//			//如果当前 角色上身或者下身有一件套装 和 选择的一样，就认为是 同一个套装
//			if( m_avatarEquip.AvatarEquip[EAE_Upper].dwTypeID == pCfgItem->Amor_Upper_TypeID || 
//				m_avatarEquip.AvatarEquip[EAE_Lower].dwTypeID == pCfgItem->Amor_Lower_TypeID )
//			{
//				return;
//			}
//
//			m_avatarEquip.Set( EAE_Head, pCfgItem->Amor_Head_TypeID, 0, 0 );
//			m_avatarEquip.Set( EAE_Upper, pCfgItem->Amor_Upper_TypeID, 0, 0 );
//			m_avatarEquip.Set( EAE_Glove, pCfgItem->Amor_Glove_TypeID, 0, 0 );
//			m_avatarEquip.Set( EAE_Back, pCfgItem->Amor_Back_TypeID, 0, 0 );
//			m_avatarEquip.Set( EAE_Lower, pCfgItem->Amor_Lower_TypeID, 0, 0 );
//			m_avatarEquip.Set( EAE_Boot, pCfgItem->Amor_Foot_TypeID, 0, 0 );
//
//			m_avatarEquip.dwSuitEffectID = pCfgItem->AmorEffect;
//			PrepareEffectSkeleton( pCfgItem, m_eEquipType, m_eLeftEquipType, FALSE );
//		}
//		break;
//	case ECRP_ClothAmor:
//		{
//			//装备展开(变形)过程中不允许预览，否则会导致暂停在这个阶段
//			if( m_step == EStep_OpenEquip )
//				break;
//
//			//如果当前 角色上身或者下身有一件套装 和 选择的一样，就认为是 同一个套装
//			if( m_avatarEquip.AvatarEquip[EAE_Upper].dwTypeID == pCfgItem->ClothAmor_Upper_TypeID || 
//				m_avatarEquip.AvatarEquip[EAE_Lower].dwTypeID == pCfgItem->ClothAmor_Lower_TypeID )
//			{
//				return;
//			}
//
//			m_avatarEquip.Set( EAE_Head, pCfgItem->ClothAmor_Head_TypeID, 0, 0 );
//			m_avatarEquip.Set( EAE_Upper, pCfgItem->ClothAmor_Upper_TypeID, 0, 0 );
//			m_avatarEquip.Set( EAE_Glove, pCfgItem->ClothAmor_Glove_TypeID, 0, 0 );
//			m_avatarEquip.Set( EAE_Back, pCfgItem->ClothAmor_Back_TypeID, 0, 0 );
//			m_avatarEquip.Set( EAE_Lower, pCfgItem->ClothAmor_Lower_TypeID, 0, 0 );
//			m_avatarEquip.Set( EAE_Boot, pCfgItem->ClothAmor_Foot_TypeID, 0, 0 );
//
//			m_avatarEquip.dwSuitEffectID = pCfgItem->ClothAmorEffect;
//			PrepareEffectSkeleton( pCfgItem, m_eEquipType, m_eLeftEquipType, FALSE );
//		}
//		break;
//	case ECRP_FashionOne:
//		{
//			//装备展开(变形)过程中不允许预览，否则会导致暂停在这个阶段
//			if( m_step == EStep_OpenEquip )
//				break;
//
//			//如果当前 角色上身或者下身有一件套装 和 选择的一样，就认为是 同一个套装
//			if( m_avatarEquip.AvatarEquip[EAE_Upper].dwTypeID == pCfgItem->FashionOne_Upper_TypeID || 
//				m_avatarEquip.AvatarEquip[EAE_Lower].dwTypeID == pCfgItem->FashionOne_Lower_TypeID )
//			{
//				return;
//			}
//
//			if( pCfgItem->FashionOne_Head_TypeID != 0 )
//			{
//				if( GT_VALID( pCfgItem->FashionOne_HeadColor ) )
//					m_avatarEquip.Set( EAE_Head, pCfgItem->FashionOne_Head_TypeID, 0, pCfgItem->FashionOne_HeadColor );
//				else
//					m_avatarEquip.Set( EAE_Head, pCfgItem->FashionOne_Head_TypeID, 0, 0 );
//			}
//				
//			if( pCfgItem->FashionOne_Upper_TypeID != 0 )
//			{
//				if( GT_VALID( pCfgItem->FashionOne_UpperColor ) )
//					m_avatarEquip.Set( EAE_Upper, pCfgItem->FashionOne_Upper_TypeID, 0, pCfgItem->FashionOne_UpperColor );
//				else
//					m_avatarEquip.Set( EAE_Upper, pCfgItem->FashionOne_Upper_TypeID, 0, 0 );
//			}
//				
//			if( pCfgItem->FashionOne_Glove_TypeID != 0 )
//			{
//				if( GT_VALID( pCfgItem->FashionOne_GloveColor ) )
//					m_avatarEquip.Set( EAE_Glove, pCfgItem->FashionOne_Glove_TypeID, 0, pCfgItem->FashionOne_GloveColor );
//				else
//					m_avatarEquip.Set( EAE_Glove, pCfgItem->FashionOne_Glove_TypeID, 0, 0 );
//			}
//				
//
//			if( pCfgItem->FashionOne_Back_TypeID != 0 )
//			{
//				if( GT_VALID( pCfgItem->FashionOne_BackColor ) )
//					m_avatarEquip.Set( EAE_Back, pCfgItem->FashionOne_Back_TypeID, 0, pCfgItem->FashionOne_BackColor );
//				else
//					m_avatarEquip.Set( EAE_Back, pCfgItem->FashionOne_Back_TypeID, 0, 0 );
//			}
//
//			if( pCfgItem->FashionOne_Lower_TypeID != 0 )
//			{
//				if( GT_VALID( pCfgItem->FashionOne_LowerColor ) )
//					m_avatarEquip.Set( EAE_Lower, pCfgItem->FashionOne_Lower_TypeID, 0, pCfgItem->FashionOne_LowerColor );
//				else
//					m_avatarEquip.Set( EAE_Lower, pCfgItem->FashionOne_Lower_TypeID, 0, 0 );
//			}
//
//			if( pCfgItem->FashionOne_Foot_TypeID != 0 )
//			{
//				if( GT_VALID( pCfgItem->FashionOne_FootColor ) )
//					m_avatarEquip.Set( EAE_Boot, pCfgItem->FashionOne_Foot_TypeID, 0, pCfgItem->FashionOne_FootColor );
//				else
//					m_avatarEquip.Set( EAE_Boot, pCfgItem->FashionOne_Foot_TypeID, 0, 0 );
//			}
//
//			m_avatarEquip.dwSuitEffectID = pCfgItem->FashionOne_SuitEffect;
//			PrepareEffectSkeleton( pCfgItem, m_eEquipType, m_eLeftEquipType, FALSE );
//		}
//		break;
//	case ECRP_FashionTwo:
//		{
//			//装备展开(变形)过程中不允许预览，否则会导致暂停在这个阶段
//			if( m_step == EStep_OpenEquip )
//				break;
//
//			//如果当前 角色上身或者下身有一件套装 和 选择的一样，就认为是 同一个套装
//			if( m_avatarEquip.AvatarEquip[EAE_Upper].dwTypeID == pCfgItem->FashionTwo_Upper_TypeID || 
//				m_avatarEquip.AvatarEquip[EAE_Lower].dwTypeID == pCfgItem->FashionTwo_Lower_TypeID )
//			{
//				return;
//			}
//
//			if( pCfgItem->FashionTwo_Head_TypeID != 0 )
//			{
//				if( GT_VALID( pCfgItem->FashionTwo_HeadColor ) )
//					m_avatarEquip.Set( EAE_Head, pCfgItem->FashionTwo_Head_TypeID, 0, pCfgItem->FashionTwo_HeadColor );
//				else
//					m_avatarEquip.Set( EAE_Head, pCfgItem->FashionTwo_Head_TypeID, 0, 0 );
//			}
//				
//			if( pCfgItem->FashionTwo_Upper_TypeID != 0 )
//			{
//				if( GT_VALID( pCfgItem->FashionTwo_UpperColor ) )
//					m_avatarEquip.Set( EAE_Upper, pCfgItem->FashionTwo_Upper_TypeID, 0, pCfgItem->FashionTwo_UpperColor );
//				else
//					m_avatarEquip.Set( EAE_Upper, pCfgItem->FashionTwo_Upper_TypeID, 0, 0 );
//			}
//
//			if( pCfgItem->FashionTwo_Glove_TypeID != 0 )
//			{
//				if( GT_VALID( pCfgItem->FashionTwo_GloveColor ) )
//					m_avatarEquip.Set( EAE_Glove, pCfgItem->FashionTwo_Glove_TypeID, 0, pCfgItem->FashionTwo_GloveColor );
//				else
//					m_avatarEquip.Set( EAE_Glove, pCfgItem->FashionTwo_Glove_TypeID, 0, 0 );
//			}
//
//			if( pCfgItem->FashionTwo_Back_TypeID != 0 )
//			{
//				if( GT_VALID( pCfgItem->FashionTwo_BackColor ) )
//					m_avatarEquip.Set( EAE_Back, pCfgItem->FashionTwo_Back_TypeID, 0, pCfgItem->FashionTwo_BackColor );
//				else
//					m_avatarEquip.Set( EAE_Back, pCfgItem->FashionTwo_Back_TypeID, 0, 0 );
//			}
//
//			if( pCfgItem->FashionTwo_Lower_TypeID != 0 )
//			{
//				if( GT_VALID( pCfgItem->FashionTwo_LowerColor ) )
//					m_avatarEquip.Set( EAE_Lower, pCfgItem->FashionTwo_Lower_TypeID, 0, pCfgItem->FashionTwo_LowerColor );
//				else
//					m_avatarEquip.Set( EAE_Lower, pCfgItem->FashionTwo_Lower_TypeID, 0, 0 );
//			}
//
//			if( pCfgItem->FashionTwo_Foot_TypeID != 0 )
//			{
//				if( GT_VALID( pCfgItem->FashionTwo_FootColor ) )
//					m_avatarEquip.Set( EAE_Boot, pCfgItem->FashionTwo_Foot_TypeID, 0, pCfgItem->FashionTwo_FootColor );
//				else
//					m_avatarEquip.Set( EAE_Boot, pCfgItem->FashionTwo_Foot_TypeID, 0, 0 );
//			}
//
//			m_avatarEquip.dwSuitEffectID = pCfgItem->FashionTwo_SuitEffect;
//			PrepareEffectSkeleton( pCfgItem, m_eEquipType, m_eLeftEquipType, FALSE );
//		}
//		break;
//	default:
//		return;
//	}
//}
//
//void CreateRoleFrame::PrepareEffectSkeleton( const tagCreateRolePreviewItem* pCfgItem,ECreateRolePreview ePreview, ECreateRolePreview eLeftPreview, BOOL bPlayCombat )
//{
//	m_pPlayer->SetEquipEffectEx( EAE_RWeapon, GetEffectName(ePreview,0), 0 );
//	m_pPlayer->SetEquipEffectEx( EAE_RWeapon, GetEffectName(ePreview,1), 1 );
//	m_pPlayer->SetEquipEffectEx( EAE_RWeapon, GetEffectName(ePreview,2), 2 );
//	
//	m_pPlayer->SetEquipEffectEx( EAE_LWeapon, GetEffectName(eLeftPreview,0), 0 );
//	m_pPlayer->SetEquipEffectEx( EAE_LWeapon, GetEffectName(eLeftPreview,1), 1 );
//	m_pPlayer->SetEquipEffectEx( EAE_LWeapon, GetEffectName(eLeftPreview,2), 2 );
//
//	m_pPlayer->UpdateAvatar( m_avatarAtt, m_avatarEquip, SeleRoleMgr::Inst()->GetHugeSG() );
//
//	//更新主手龙魂特效，龙魂特效文件 后缀没有0，从1开始
//	BYTE byLoongHun = 0;
//	GetEffectName( ePreview,0, &byLoongHun );
//	if( byLoongHun > 0 )
//		m_pPlayer->UpdataLongHunEffect( EAE_RWeapon,byLoongHun );
//
//	//更新副手龙魂特效
//	GetEffectName( eLeftPreview,0, &byLoongHun );
//	if( byLoongHun > 0 )
//		m_pPlayer->UpdataLongHunEffect( EAE_LWeapon,byLoongHun );
//
//	if( bPlayCombat == TRUE )
//	{
//		m_step = EStep_EnterCombat;
//		m_bPlayPreview = true;
//	}
//}
//
//tstring CreateRoleFrame::GetEffectName( ECreateRolePreview eEquipType, BYTE index, LPBYTE pByte )
//{
//	const tagCreateRolePreviewItem* pCfgItem = m_previewData.FindPreviewInfoBySex( m_avatarAtt.bySex );
//	if( !P_VALID(pCfgItem) )
//		return _T("");
//
//	switch( eEquipType )
//	{
//	case ECRP_Sword:
//		{
//			if( P_VALID(pByte) )
//				*pByte = pCfgItem->SwordLoongHunEffect;
//			
//			if( index <0 || index >= pCfgItem->vecSwordEffect.size() )
//				return _T("");
//			else
//				return pCfgItem->vecSwordEffect[index];
//		}
//		break;
//	case ECRP_Blade:
//		{
//			if( P_VALID(pByte) )
//				*pByte = pCfgItem->BladeLoongHunEffect;
//
//			if( index <0 || index >= pCfgItem->vecBladeEffect.size() )
//				return _T("");
//			else
//				return pCfgItem->vecBladeEffect[index];
//		}
//		break;
//	case ECRP_Wand:
//		{
//			if( P_VALID(pByte) )
//				*pByte = pCfgItem->WandLoongHunEffect;
//
//			if( index <0 || index >= pCfgItem->vecWandEffect.size() )
//				return _T("");
//			else
//				return pCfgItem->vecWandEffect[index];
//		}
//		break;
//	case ECRP_Bow:
//		{
//			if( P_VALID(pByte) )
//				*pByte = pCfgItem->BowLoongHunEffect;
//
//			if( index <0 || index >= pCfgItem->vecBowEffect.size() )
//				return _T("");
//			else
//				return pCfgItem->vecBowEffect[index];
//		}
//		break;
//	case ECRP_LongShou:
//		{
//			if( P_VALID(pByte) )
//				*pByte = pCfgItem->LongShouLoongHunEffect;
//
//			if( index <0 || index >= pCfgItem->vecLongShouEffect.size() )
//				return _T("");
//			else
//				return pCfgItem->vecLongShouEffect[index];
//		}
//		break;
//	case ECRP_Qin:
//		{
//			if( P_VALID(pByte) )
//				*pByte = pCfgItem->QinLoongHunEffect;
//
//			if( index <0 || index >= pCfgItem->vecQinEffect.size() )
//				return _T("");
//			else
//				return pCfgItem->vecQinEffect[index];
//		}
//		break;
//	case ECRP_Organ:
//		{
//
//			if( P_VALID(pByte) )
//				*pByte = pCfgItem->OrganLoongHunEffect;
//
//			if( index <0 || index >= pCfgItem->vecOrganEffect.size() )
//				return _T("");
//			else
//				return pCfgItem->vecOrganEffect[index];
//		}
//		break;
//	}
//	return _T("");
//}
//
//void CreateRoleFrame::WeaponIntro(ECreateRolePreview eWeaponType)	 //显示武器说明页面
//
//{
// 
//	if (eWeaponType<ECRP_Sword||eWeaponType>ECRP_Qin)
//	{
//          return;
//	}
//	m_ECurWeapon = eWeaponType;
//
//	if( P_VALID(m_pWndWeaponIntro) )
//	{
//		m_pWndWeaponIntro->SetInvisible( FALSE );
//		m_iMaxTextNum = PageMaxNum(eWeaponType);
//		
//		tstringstream    tempPageTstr1;
//		tempPageTstr1 << m_iMaxTextNum;
//		tstring          showPageTstr;
//		showPageTstr = _T("1/")+tempPageTstr1.str();
//		m_pStcIntroNum->SetText(showPageTstr.c_str());
//
//		switch(eWeaponType)
//		{	 
//		//GUIStatic*						m_pStcIntroNum;     //武器说明页数显示
//		//GUIStaticEx*				   	    m_pStcIntroText;    //武器说明文本显示
//		//GUIButton*						m_pBtnPreIntro;     //武器说明页前一页按钮
//		//GUIButton*						m_pBtnNextIntro;    //武器说明页后一页按钮
//		case ECRP_Sword:  
//            m_pStcIntroText->SetText(g_StrTable[_T("CRWI_Sword1")]);
//			break;
//		case ECRP_Blade:   
//			m_pStcIntroText->SetText(g_StrTable[_T("CRWI_Blade1")]);
//			break;
//		case ECRP_Wand:   
//			m_pStcIntroText->SetText(g_StrTable[_T("CRWI_Wand1")]);
//			break;
//		case ECRP_Bow:  
//			m_pStcIntroText->SetText(g_StrTable[_T("CRWI_Bow1")]);
//			break;
//		case ECRP_Organ:  
//			m_pStcIntroText->SetText(g_StrTable[_T("CRWI_Organ1")]);
//			break;
//		case ECRP_LongShou:  
//			m_pStcIntroText->SetText(g_StrTable[_T("CRWI_LongShou1")]);
//			break;
//		case ECRP_Qin:  
//			m_pStcIntroText->SetText(g_StrTable[_T("CRWI_Qin1")]);
//		    break;
//		}
//		m_pStcIntroText->SetTopMost(TRUE);
//		m_pStcIntroText->FlipToTop();
//		m_pStcIntroText->SetRefresh();
//	}
//
//	//do something 与eWeaponType有关
//	
// 
//
//}
//
//const INT				m_csi_maxPageSword =3;
//const INT				m_csi_maxPageBlade =2;
//const INT				m_csi_maxPageWand =3;
//const INT				m_csi_maxPageBow =2;
//const INT				m_csi_maxPageOrgan =9;
//const INT				m_csi_maxPageLongShou =9;
//const INT				m_csi_maxPageQin =3;
//
//INT  CreateRoleFrame::PageMaxNum(ECreateRolePreview eWeaponType)
//{
//   
//	   switch (eWeaponType)
//	   {
//	   case ECRP_Sword:
//		   m_iMaxTextNum = m_csi_maxPageSword;
//	           break;
//	   case ECRP_Blade:
//		   m_iMaxTextNum = m_csi_maxPageBlade;
//			   break;
//	   case ECRP_Wand:
//		   m_iMaxTextNum = m_csi_maxPageWand;
//			   break;
//	   case ECRP_Bow:
//		   m_iMaxTextNum = m_csi_maxPageBow;
//			   break;
//	   case ECRP_Organ:
//		   m_iMaxTextNum = m_csi_maxPageOrgan;
//			   break;
//	   case ECRP_LongShou:
//		   m_iMaxTextNum = m_csi_maxPageLongShou;
//			   break;
//	   case ECRP_Qin:
//		   m_iMaxTextNum = m_csi_maxPageQin;
//			   break;
//	   default:
//		       break;
//	   }
//	   return m_iMaxTextNum;
//}
//
//bool CreateRoleFrame::WeaponIntroEnvent(tagGUIEvent* pEvent )    //武器说明页面时间响应
//{
//
//	GUIWnd* pWnd = m_pGUI->GetWnd(pEvent->dwWndID);
//	if(!P_VALID(pWnd)) 
//		return FALSE;
//	switch(pEvent->eEvent)
//	{
// 
//	case EGUIE_Click:
//		{
//			if (pWnd == m_pBtnPreIntro)
//			{
//                if (m_iCurTextNum>1)
//                {
//                     m_iCurTextNum--;
//                }
//			} 
//			else if(pWnd == m_pBtnNextIntro)
//			{
//                 if (m_iCurTextNum<m_iMaxTextNum)
//                 {
//					 m_iCurTextNum++;
//                 }
//			}
//		}
//
//	default:
//		break;
//	}
//	if( P_VALID(m_pWndWeaponIntro) )
//	{
//		//显示最大页数和当前页数 
//		tstringstream    tempPageTstr1,tempPageTstr2;  
//		tempPageTstr1 << m_iMaxTextNum;
//		tempPageTstr2 << m_iCurTextNum;
//		tstring          showPageTstr;
//		showPageTstr = tempPageTstr2.str()+_T("/")+tempPageTstr1.str();
//		m_pStcIntroNum->SetText(showPageTstr.c_str());
//		//用以载入当前页的说明文字string
//		tstring          showPageCurTstr;
//
//
//		switch(m_ECurWeapon)
//		{	 
//		//GUIStatic*						m_pStcIntroNum;     //武器说明页数显示
//		//GUIStaticEx*				   	    m_pStcIntroText;    //武器说明文本显示
//		//GUIButton*						m_pBtnPreIntro;     //武器说明页前一页按钮
//		//GUIButton*						m_pBtnNextIntro;    //武器说明页后一页按钮
//
//		case ECRP_Sword:  
//			showPageCurTstr = _T("CRWI_Sword")+tempPageTstr2.str();
//			m_pStcIntroText->SetText(g_StrTable[showPageCurTstr.c_str()]);
//			break;
//		case ECRP_Blade:   
//			showPageCurTstr = _T("CRWI_Blade")+tempPageTstr2.str();
//			m_pStcIntroText->SetText(g_StrTable[showPageCurTstr.c_str()]);
//			break;
//		case ECRP_Wand:   
//			showPageCurTstr = _T("CRWI_Wand")+tempPageTstr2.str();
//			m_pStcIntroText->SetText(g_StrTable[showPageCurTstr.c_str()]);
//			break;
//		case ECRP_Bow:  
//			showPageCurTstr = _T("CRWI_Bow")+tempPageTstr2.str();
//			m_pStcIntroText->SetText(g_StrTable[showPageCurTstr.c_str()]);
//			break;
//		case ECRP_Organ:  
//			showPageCurTstr = _T("CRWI_Organ")+tempPageTstr2.str();
//			m_pStcIntroText->SetText(g_StrTable[showPageCurTstr.c_str()]);
//			break;
//		case ECRP_LongShou:  
//			showPageCurTstr = _T("CRWI_LongShou")+tempPageTstr2.str();
//			m_pStcIntroText->SetText(g_StrTable[showPageCurTstr.c_str()]);
//			break;
//		case ECRP_Qin:  
//			showPageCurTstr = _T("CRWI_Qin")+tempPageTstr2.str();
//			m_pStcIntroText->SetText(g_StrTable[showPageCurTstr.c_str()]);
//			break;
//		}
//		m_pStcIntroNum->SetTopMost(TRUE);
//		m_pStcIntroNum->FlipToTop();
//		m_pStcIntroNum->SetRefresh();
//
//		m_pStcIntroText->SetTopMost(TRUE);
//		m_pStcIntroText->FlipToTop();
//		m_pStcIntroText->SetRefresh();
//	}
//	return true;
//}

VOID CreateRoleFrame::RefreshCareerIntro()
{
	tagGUIEvent event(m_pScrollBar, EGUIE_Scroll);
	event.dwParam1 = (DWORD)0;
	event.dwParam2 = (DWORD)0;
	event.dwParam3 = (DWORD)0;
	m_pScrollBar->SendEvent(&event);
	m_pScrollBar->SetValue(0);
	tagPoint size = m_pStcIntor->GetSize();
	if( m_pStcIntor->GetTextTotalHeight() > size.y )
	{
		size.y = m_pStcIntor->GetTextTotalHeight();
		m_pStcIntor->SetResizable(TRUE);
		m_pStcIntor->Resize( size );
		m_pStcIntor->SetResizable(FALSE);
	}
	m_pWndJob->SetRefresh(TRUE);
}

VOID CreateRoleFrame::ShowCareerEquip( INT nCareerType )
{
	TCHAR szEffect[X_LONG_NAME] = {0};
	_sntprintf( szEffect, X_LONG_NAME, _T("txcre0%d"), nCareerType + 1 );
	m_pPlayer->SetEquipEffectEx( EAE_Boot, szEffect, 0 );
	const tagCreateRolePreviewItem *pEquip = m_previewData.FindPreviewInfoBySexAndCareer( m_avatarAtt.bySex, nCareerType );
	if(!P_VALID(pEquip))
		return;
	tagAvatarEquip AvatarEquip;
	ZeroMemory( &AvatarEquip, sizeof(tagAvatarEquip) );
	AvatarEquip.Set( EAE_Head, pEquip->dwHat, 0, 0 );
	AvatarEquip.Set( EAE_RWeapon, pEquip->dwWeapon, 0, 0 );
	m_pPlayer->SetEquipEffectEx( EAE_RWeapon, pEquip->strWeaponEffect1, 0 );
	m_pPlayer->SetEquipEffectEx( EAE_RWeapon, pEquip->strWeaponEffect2, 1 );
	m_pPlayer->SetEquipEffectEx( EAE_RWeapon, pEquip->strWeaponEffect3, 2 );
	AvatarEquip.Set( EAE_Upper, pEquip->dwUpper, 0, 0 );
	AvatarEquip.Set( EAE_Lower, pEquip->dwLower, 0, 0 );
	AvatarEquip.Set( EAE_Glove, pEquip->dwGlove, 0, 0 );
	AvatarEquip.Set( EAE_Boot, pEquip->dwBoot, 0, 0 );
	AvatarEquip.dwSuitEffectID = pEquip->dwEffectID;
	m_pPlayer->UpdateAvatar( m_avatarAtt, AvatarEquip, SeleRoleMgr::Inst()->GetHugeSG() );
}

void CreateRoleFrame::CreateBackGroud( INT nCareer )
{
	tstring strFlag;
	switch(nCareer)
	{
	case 0:
		strFlag = _T("data\\ui\\login\\cj1.tga");
		break;
	case 1:
		strFlag = _T("data\\ui\\login\\cj2.tga");
		break;
	case 2:
		strFlag = _T("data\\ui\\login\\cj3.tga");
		break;
	case 3:
		strFlag = _T("data\\ui\\login\\cj4.tga");
		break;
	default:
		strFlag = _T("data\\ui\\login\\cj1.tga");
		break;
	}
	if(P_VALID(m_pTex))
		IDraw2D::Inst()->ReleaseTexture(m_pTex);
	IFS* pIFS = (IFS*)(VOID*)TObjRef<VirtualFileSys>("VFS_System");
	m_pTex = IDraw2D::Inst()->NewTexture(strFlag.c_str(), pIFS);
	if (!P_VALID(m_pTex))
		return;
	IDeviceTex *pRealTex = m_pTex->GetDeviceTex();
	if (!P_VALID(pRealTex))
		return;

	m_rectSrc.top = m_rectSrc.left = 0;
	m_rectSrc.bottom = m_rectSrc.top + pRealTex->Height();
	m_rectSrc.right = m_rectSrc.left + pRealTex->Width();

	GUIWnd* pDesktop = m_pGUI->GetDesktop();
	m_rectDest.left = 0;
	m_rectDest.top = 0;
	m_rectDest.right = pDesktop->GetSize().x;
	m_rectDest.bottom = pDesktop->GetSize().y;
}