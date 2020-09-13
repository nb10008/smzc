#include "stdafx.h"
#include "ProduceData.h"
#include "OverSeaData.h"
ProduceData::ProduceData( void )
{

}

ProduceData::~ProduceData( void )
{

}
ProduceData g_Produce;
ProduceData* ProduceData::Inst()
{
	return &g_Produce;
}

void ProduceData::LoadFromFile()
{
	//m_OverData.Init();
	OverSeaData::Inst()->Init();

	const TCHAR* szProduceProto = _T("data\\system\\attdata\\compose_proto.xml");
	tstring strPath1 = g_strLocalPath + _T("\\attdata\\compose_name.xml");
	const TCHAR* szProduceName = strPath1.c_str();

	const TCHAR* szDeComposeProto = _T("data\\system\\attdata\\decompose_proto.xml");
	tstring strPath2 = g_strLocalPath + _T("\\attdata\\decompose_name.xml");
	const TCHAR* szDeComposeName = strPath2.c_str();

	const TCHAR* szRandomProduceFile = _T("data\\system\\attdata\\advanced_compose_proto.xml");

	const TCHAR* szExtractFile = _T("data\\system\\attdata\\extract_proto.xml");	//萃取配方

	list<tstring> produceList;
	list<tstring> proNameList;
	list<tstring> decompsoeList;
	list<tstring> deNameList;
	list<tstring> randomProduceList;
	list<tstring> extractList;
	list<tstring>::iterator iter;

	//读取生产合成文件
	TObjRef<VarContainer> varProduce = CreateObj( "ProduceProto", "VarContainer" );
	if(!varProduce->Load("VFS_System", szProduceProto, "id", &produceList))
		IMSG(_T("Load file compose_proto.xml failed\r\n"));

	for(iter=produceList.begin(); iter!=produceList.end(); ++iter)
	{
		tagProduceProto produceProto;
		produceProto.dwID = varProduce->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);

		if (OverSeaData::Inst()->IsFormualHide(produceProto.dwID))
			continue;
		//_tcscpy( produceProto.szName, varProduce->GetString(_T("name"), (*iter).c_str(), _T("")) );
		produceProto.eProType = (EProduceType)varProduce->GetDword(_T("produce_type"), (*iter).c_str(), EPCT_NULL);
		produceProto.eComType = (EComposeType)varProduce->GetDword(_T("compose_type"), (*iter).c_str(), ECOMT_NULL);
		produceProto.eFormType = (EFormulaType)varProduce->GetDword(_T("formula_type"), (*iter).c_str(), EFORMT_NULL);
		produceProto.eFormFrom = (EFormulaFrom)varProduce->GetDword(_T("formula_from"), (*iter).c_str(), EFormulaFrom_NULL);
		produceProto.dwFormLvl = varProduce->GetDword(_T("fromula_level"), (*iter).c_str(), GT_INVALID);
		produceProto.dwProItemTypeID = varProduce->GetDword(_T("compose_item"), (*iter).c_str(), GT_INVALID);
		produceProto.dwProQuantity = varProduce->GetDword(_T("compose_num"), (*iter).c_str(), GT_INVALID);
		produceProto.nSuccessRate = (INT)varProduce->GetDword(_T("success_rate"), (*iter).c_str(), GT_INVALID);
		produceProto.dwProduceTime = varProduce->GetDword(_T("produce_time"), (*iter).c_str(), GT_INVALID);
		produceProto.dwVitalityConsume = varProduce->GetDword(_T("vitality_consume"), (*iter).c_str(), GT_INVALID);
		produceProto.dwMasterIncre = varProduce->GetDword(_T("master_incre"), (*iter).c_str(), GT_INVALID);
		produceProto.dwMoneyConsume = varProduce->GetDword(_T("money_consume"), (*iter).c_str(), 0);
		for(int i=0; i<MAX_PRODUCE_STUFF_QUANTITY; i++)
		{
			TCHAR szBuff[64];
			_stprintf( szBuff, _T("%s%d%s"),  _T("stuff"), i+1, _T("_id") );
			produceProto.ProduceStuff[i].dwStuffID = varProduce->GetDword(szBuff, (*iter).c_str(), GT_INVALID);
			_stprintf( szBuff, _T("%s%d%s"), _T("stuff"), i+1, _T("_type") );
			produceProto.ProduceStuff[i].eStuffType = (EStuffType)varProduce->GetDword(szBuff, (*iter).c_str(), EST_Null);
			_stprintf( szBuff, _T("%s%d%s"), _T("stuff"), i+1, _T("_num") );
			produceProto.ProduceStuff[i].dwStuffNum = varProduce->GetDword(szBuff, (*iter).c_str(), GT_INVALID);
		}
		switch(produceProto.eProType)
		{
		case EPCT_Artisan:
			m_mapArtisan.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_DanTraining:
			m_mapDanTraining.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_Smith:
			m_mapSmith.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_Casting:
			m_mapCasting.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_Dressmaker:
			m_mapDressmaker.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_Aechnics:
			m_mapAechnics.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_Smilt:
			m_mapSmilt.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_GodArtisan:
			m_mapGodArtisan.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_Gem:
			m_mapBaoshi.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_Pet:
			m_mapPet.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_EquipMaster:
			m_mapEquipMaster.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_TreasureCom:
			m_mapTreasureCom.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_FoodGod1:
			m_mapFoodGod1.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_FoodGod2:
			m_mapFoodGod2.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_FoodGod3:
			m_mapFoodGod3.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_Perfume:
			m_mapPerfume.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_EquipRefine:
			m_mapEquipRefine.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_StuffRefine:
			m_mapStuffRefine.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_TimeLimitEquip:
			m_mapTimeLimitEquip.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_Crystal:
			m_mapCryStal.insert(make_pair(produceProto.dwID, produceProto));
			break;
		case EPCT_Stole_52:
		case EPCT_Stole_67:
		case EPCT_Stole_82:
		case EPCT_Stole_Blade:
		case EPCT_Stole_Bow:
		case EPCT_Stole_Valor:
		case EPCT_Stole_Swing:
		case EPCT_SoulLinker:
		case EPCT_MoonCake:
		case EPCT_HallowmasGift:
		case EPCT_HallowmasName:
		case EPCT_MoShenEquip:
		case EPCT_FairySoulStren://add by xp
		case EPCT_FairySkillbookStren://add by xp
		case EPCT_BattleFieldCompose://add by xp
		case EPCT_ReplaceFabao://add by xp
			m_mapStole.insert(make_pair(produceProto.dwID, produceProto));
			break;
		}
	}

	KillObj( "ProduceProto" );

	TObjRef<VarContainer> varProduceName = CreateObj( "ProduceName", "VarContainer" );
	if(!varProduceName->Load("VFS_System", szProduceName, "id", &proNameList))
		IMSG(_T("Load file compose_proto.xml failed\r\n"));
	for(iter=proNameList.begin(); iter!=proNameList.end(); iter++)
	{
		DWORD dwID = varProduceName->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		EProduceType eType = (EProduceType)varProduceName->GetDword(_T("produce_type"), (*iter).c_str(), EPCT_NULL);
		tagProduceProto* produceProto = FindProduceProto(eType, dwID);
		if( P_VALID(produceProto) )
		{
			_tcscpy( produceProto->szName, varProduceName->GetString(_T("name"), (*iter).c_str(), _T("")) );
		}
	}

	KillObj( "ProduceName" );


	//读取分解文件
	TObjRef<VarContainer> varDeCompose = CreateObj( "DeComposeProto", "VarContainer" );
	if(!varDeCompose->Load("VFS_System", szDeComposeProto, "id", &decompsoeList))
		IMSG(_T("Load file compose_proto.xml failed\r\n"));
	for(iter=decompsoeList.begin(); iter!=decompsoeList.end(); iter++)
	{
		tagDeComposeProto deCompose;
		deCompose.dwID = varDeCompose->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		deCompose.eComType = (EComposeType)varDeCompose->GetDword(_T("compose_type"), (*iter).c_str(), ECOMT_NULL);
		deCompose.byLevel = (BYTE)varDeCompose->GetDword(_T("level"), (*iter).c_str(), GT_INVALID);
		deCompose.byQuality = (BYTE)varDeCompose->GetDword(_T("quality"), (*iter).c_str(), GT_INVALID);
		deCompose.eType = (EItemType)varDeCompose->GetDword(_T("type"), (*iter).c_str(), EIT_Null);
		deCompose.eTypeEx = (EItemTypeEx)varDeCompose->GetDword(_T("type_ex"), (*iter).c_str(), EITE_Null);
		deCompose.ePos = (EEquipPos)varDeCompose->GetDword(_T("equip_pos"), (*iter).c_str(), GT_INVALID);
		deCompose.dwFormLvl = varDeCompose->GetDword(_T("formula_level"), (*iter).c_str(), GT_INVALID);
		deCompose.eFormFrom = (EFormulaFrom)varDeCompose->GetDword(_T("formula_from"), (*iter).c_str(), EFormulaFrom_NULL);
		deCompose.dwProduceTime = varDeCompose->GetDword(_T("produce_time"), (*iter).c_str(), GT_INVALID);
		deCompose.dwVitalityConsume = varDeCompose->GetDword(_T("vitality_consume"), (*iter).c_str(), GT_INVALID);
		deCompose.dwMasterIncre = varDeCompose->GetDword(_T("master_incre"), (*iter).c_str(), GT_INVALID);
		deCompose.dwMoneyConsume = varDeCompose->GetDword(_T("money_consume"), (*iter).c_str(), 0);
		for(int i=0; i<MAX_OUTPUT_QUANTITY; i++)
		{
			TCHAR szBuff[64];
			_stprintf(szBuff, _T("item%d_id"), i+1);
			deCompose.OutputStuff[i].dwStuffTypeID = varDeCompose->GetDword(szBuff, (*iter).c_str(), GT_INVALID);
			_stprintf(szBuff, _T("per_min%d_val"), i+1);
			deCompose.OutputStuff[i].nPerMinVal = varDeCompose->GetDword(szBuff, (*iter).c_str(), GT_INVALID);
			_stprintf(szBuff, _T("per_max%d_val"), i+1);
			deCompose.OutputStuff[i].nPerMaxVal = varDeCompose->GetDword(szBuff, (*iter).c_str(), GT_INVALID);
			_stprintf(szBuff, _T("suc_min%d_val"), i+1);
			deCompose.OutputStuff[i].nSucMinVal = varDeCompose->GetDword(szBuff, (*iter).c_str(), GT_INVALID);
			_stprintf(szBuff, _T("suc_max%d_val"), i+1);
			deCompose.OutputStuff[i].nSucMaxVal = varDeCompose->GetDword(szBuff, (*iter).c_str(), GT_INVALID);
			_stprintf(szBuff, _T("rate%d"), i+1);
			deCompose.OutputStuff[i].nRate = varDeCompose->GetDword(szBuff, (*iter).c_str(), GT_INVALID);
		}

		m_mapDeCompose.insert(make_pair(deCompose.dwID, deCompose));
	}

	KillObj( "DeComposeProto" );
	
	TObjRef<VarContainer> varDeName = CreateObj( "DeNameName", "VarContainer" );
	if(!varDeName->Load("VFS_System", szDeComposeName, "id", &deNameList))
		IMSG(_T("Load file compose_proto.xml failed\r\n"));
	for(iter=deNameList.begin(); iter!=deNameList.end(); iter++)
	{
		DWORD dwID = varDeName->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		tagDeComposeProto* deCompose = FinDeComposeProto(dwID);
		if( P_VALID(deCompose) )
		{
			_tcscpy( deCompose->szName, varDeName->GetString(_T("name"), (*iter).c_str(), _T("")) );
		}
	}
	
	KillObj( "DeNameName" );


	//读取随机生成物品文件
	TObjRef<VarContainer> varRandomProdue = CreateObj( "RandomProdue", "VarContainer" );
	if(!varRandomProdue->Load("VFS_System", szRandomProduceFile, "id", &randomProduceList))
		IMSG(_T("Load file advanced_compose_proto.xml failed\r\n"));
	for(iter=randomProduceList.begin(); iter!=randomProduceList.end(); iter++)
	{
		tagRandomProduceProto deRanPro;
		deRanPro.dwID = varRandomProdue->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		deRanPro.eRProType = (EAdvanceComposeType)varRandomProdue->GetDword(_T("type"), (*iter).c_str(), EADVCOMT_NULL);
		deRanPro.dwDesigner = varRandomProdue->GetDword(_T("design"), (*iter).c_str(), GT_INVALID);
		TCHAR szTemp[X_SHORT_NAME] = {0};
		for( int i = 0; i < MAX_RANDOMPRODUCE_STUFF; i++)
		{
			_sntprintf(szTemp, X_SHORT_NAME, _T("material%d"), i+1);
			deRanPro.dwStuff[i] = varRandomProdue->GetDword(szTemp, (*iter).c_str(), GT_INVALID);
			_sntprintf(szTemp, X_SHORT_NAME, _T("num%d"), i+1);
			deRanPro.n16StuffNum[i] = (INT16)varRandomProdue->GetInt(szTemp, (*iter).c_str(), GT_INVALID);
		}
		deRanPro.dwVitalityConsume = varRandomProdue->GetDword(_T("vitality"), (*iter).c_str(), GT_INVALID);
		deRanPro.dwMoneyConsume = varRandomProdue->GetDword(_T("money"), (*iter).c_str(), GT_INVALID);
		for( int i = 0; i < MAX_RANDOMPRODUCE_NUM; i++)
		{
			_sntprintf(szTemp, X_SHORT_NAME, _T("item%d"), i+1);
			deRanPro.dwItemTypeID[i] = varRandomProdue->GetDword(szTemp, (*iter).c_str(), GT_INVALID);
		}
		m_mapRandomProduce.insert(make_pair(deRanPro.dwDesigner, deRanPro));
	}

	KillObj( "RandomProdue" );

	//加载萃取配方
	TObjRef<VarContainer> varExtractProto = CreateObj( "ExtractProto", "VarContainer" );
	if(!varExtractProto->Load("VFS_System", szExtractFile, "id", &extractList))
		IMSG(_T("Load file extract_proto.xml failed\r\n"));
	for(iter=extractList.begin(); iter!=extractList.end(); iter++)
	{
		tagExtractProto extract;
		extract.dwEquipID = varExtractProto->GetDword(_T("id"), (*iter).c_str(), GT_INVALID);
		extract.dwProduceTime = varExtractProto->GetDword(_T("produce_time"), (*iter).c_str(), GT_INVALID);
		extract.dwVitalityConsume = varExtractProto->GetDword(_T("vitality_consume"), (*iter).c_str(), GT_INVALID);
		extract.dwMoneyConsume = varExtractProto->GetDword(_T("money_consume"), (*iter).c_str(), GT_INVALID);
		extract.byQuality = varExtractProto->GetInt(_T("quality"), (*iter).c_str(), GT_INVALID);

		for(int i = 0; i < X_EXTRACT_MAX_PRODUCTION; i++)
		{
			TCHAR szTemp[X_SHORT_NAME] = {0};
			_stprintf(szTemp, _T("item%d_id"), i+1);
			extract.dwItemID[i] = varExtractProto->GetDword(szTemp, (*iter).c_str(), GT_INVALID);
			_stprintf(szTemp, _T("suc_max%d_val"), i+1);
			extract.nItemMaxNum[i] = varExtractProto->GetInt(szTemp, (*iter).c_str(), GT_INVALID);
			_stprintf(szTemp, _T("suc_min%d_val"), i+1);
			extract.nItemMinNum[i] = varExtractProto->GetInt(szTemp, (*iter).c_str(), GT_INVALID);
			_stprintf(szTemp, _T("rate%d"), i+1);
			extract.dwRate[i] = varExtractProto->GetDword(szTemp, (*iter).c_str(), 0);
		}
		
		m_mapExtractProto.insert(make_pair(extract.dwEquipID, extract));
	}

	KillObj( "ExtractProto" );
}