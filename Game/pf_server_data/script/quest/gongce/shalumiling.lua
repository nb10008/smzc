--role.GetRoleScriptData(RoleID, 1, RoleDataType["IsToday"])
--role.GetRoleScriptData(RoleID, 1, RoleDataType["InstantKillCre"]) 1000位
--RoleDataType["shaluVIP"]记录杀戮密令完成日期

--检测接取
function q30334_On_CheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local CurTime = tonumber(os.date("%j"))
	local UpdateTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["IsToday"])
	if CurTime ~= UpdateTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["IsToday"], CurTime)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FengZhiShiZhe"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["MoJieZhiHua"], role.GetRoleScriptData(RoleID, 1, RoleDataType["MoJieZhiHua"])%1000)
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["InstantKillCre"], role.GetRoleScriptData(RoleID, 1, RoleDataType["InstantKillCre"])%100)
	end
	if math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["InstantKillCre"])/100) <= 2 then
		return 1
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444013)				--您今天完成的任务数量已经达到上限
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end
end
aux.RegisterQuestEvent(30334, 4, "q30334_On_CheckAccept")
--接取任务
function q30334_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)
	local times = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["InstantKillCre"])/100)
	local Quest_RandomKill = {}
	Quest_RandomKill[30]={TargetType=QuestTargetType["Kill"],TargetID1=1040002,Num1=50,RewMoney=60000,RewXP=33000,MsgID=19401}
	Quest_RandomKill[31]={TargetType=QuestTargetType["Kill"],TargetID1=1040002,Num1=50,RewMoney=61500,RewXP=35000,MsgID=19402}
	Quest_RandomKill[32]={TargetType=QuestTargetType["Kill"],TargetID1=1040002,Num1=50,RewMoney=63100,RewXP=36000,MsgID=19403}
	Quest_RandomKill[33]={TargetType=QuestTargetType["Kill"],TargetID1=1040006,Num1=50,RewMoney=64700,RewXP=39000,MsgID=19404}
	Quest_RandomKill[34]={TargetType=QuestTargetType["Kill"],TargetID1=1040006,Num1=50,RewMoney=66400,RewXP=41000,MsgID=19405}
	Quest_RandomKill[35]={TargetType=QuestTargetType["Kill"],TargetID1=1040006,Num1=50,RewMoney=68100,RewXP=44000,MsgID=19406}
	Quest_RandomKill[36]={TargetType=QuestTargetType["Kill"],TargetID1=1040010,Num1=50,RewMoney=69900,RewXP=47000,MsgID=19407}
	Quest_RandomKill[37]={TargetType=QuestTargetType["Kill"],TargetID1=1040010,Num1=50,RewMoney=71700,RewXP=49000,MsgID=19408}
	Quest_RandomKill[38]={TargetType=QuestTargetType["Kill"],TargetID1=1040010,Num1=50,RewMoney=73500,RewXP=52000,MsgID=19409}
	Quest_RandomKill[39]={TargetType=QuestTargetType["Kill"],TargetID1=1040021,Num1=50,RewMoney=75400,RewXP=55000,MsgID=19410}
	Quest_RandomKill[40]={TargetType=QuestTargetType["Kill"],TargetID1=1040021,Num1=100,RewMoney=77300,RewXP=92000,MsgID=19411}
	Quest_RandomKill[41]={TargetType=QuestTargetType["Kill"],TargetID1=1040021,Num1=100,RewMoney=79300,RewXP=97000,MsgID=19412}
	Quest_RandomKill[42]={TargetType=QuestTargetType["Kill"],TargetID1=1040013,Num1=100,RewMoney=81300,RewXP=101000,MsgID=19413}
	Quest_RandomKill[43]={TargetType=QuestTargetType["Kill"],TargetID1=1040013,Num1=100,RewMoney=83400,RewXP=108000,MsgID=19414}
	Quest_RandomKill[44]={TargetType=QuestTargetType["Kill"],TargetID1=1040013,Num1=100,RewMoney=85500,RewXP=113000,MsgID=19415}
	Quest_RandomKill[45]={TargetType=QuestTargetType["Kill"],TargetID1=1040018,Num1=100,RewMoney=87700,RewXP=124000,MsgID=19416}
	Quest_RandomKill[46]={TargetType=QuestTargetType["Kill"],TargetID1=1040018,Num1=100,RewMoney=89900,RewXP=129000,MsgID=19417}
	Quest_RandomKill[47]={TargetType=QuestTargetType["Kill"],TargetID1=1040018,Num1=100,RewMoney=92200,RewXP=136000,MsgID=19418}
	Quest_RandomKill[48]={TargetType=QuestTargetType["Kill"],TargetID1=1040028,Num1=100,RewMoney=94600,RewXP=142000,MsgID=19419}
	Quest_RandomKill[49]={TargetType=QuestTargetType["Kill"],TargetID1=1040028,Num1=100,RewMoney=97000,RewXP=149000,MsgID=19420}
	Quest_RandomKill[50]={TargetType=QuestTargetType["Kill"],TargetID1=1040028,Num1=150,RewMoney=99500,RewXP=243000,MsgID=19421}
	Quest_RandomKill[51]={TargetType=QuestTargetType["Kill"],TargetID1=1050003,Num1=150,RewMoney=102000,RewXP=255000,MsgID=19422}
	Quest_RandomKill[52]={TargetType=QuestTargetType["Kill"],TargetID1=1050003,Num1=150,RewMoney=104600,RewXP=265000,MsgID=19423}
	Quest_RandomKill[53]={TargetType=QuestTargetType["Kill"],TargetID1=1050003,Num1=150,RewMoney=107300,RewXP=276000,MsgID=19424}
	Quest_RandomKill[54]={TargetType=QuestTargetType["Kill"],TargetID1=1050008,Num1=150,RewMoney=110000,RewXP=289000,MsgID=19425}
	Quest_RandomKill[55]={TargetType=QuestTargetType["Kill"],TargetID1=1050008,Num1=150,RewMoney=112800,RewXP=314000,MsgID=19426}
	Quest_RandomKill[56]={TargetType=QuestTargetType["Kill"],TargetID1=1050008,Num1=150,RewMoney=115700,RewXP=327000,MsgID=19427}
	Quest_RandomKill[57]={TargetType=QuestTargetType["Kill"],TargetID1=1050012,Num1=150,RewMoney=118600,RewXP=339000,MsgID=19428}
	Quest_RandomKill[58]={TargetType=QuestTargetType["Kill"],TargetID1=1050012,Num1=150,RewMoney=121600,RewXP=352000,MsgID=19429}
	Quest_RandomKill[59]={TargetType=QuestTargetType["Kill"],TargetID1=1050012,Num1=150,RewMoney=124700,RewXP=388000,MsgID=19430}
	Quest_RandomKill[60]={TargetType=QuestTargetType["Kill"],TargetID1=1050017,Num1=200,RewMoney=127900,RewXP=507000,MsgID=19431}
	Quest_RandomKill[61]={TargetType=QuestTargetType["Kill"],TargetID1=1050017,Num1=200,RewMoney=131100,RewXP=525000,MsgID=19432}
	Quest_RandomKill[62]={TargetType=QuestTargetType["Kill"],TargetID1=1050017,Num1=200,RewMoney=134400,RewXP=544000,MsgID=19433}
	Quest_RandomKill[63]={TargetType=QuestTargetType["Kill"],TargetID1=1050023,Num1=200,RewMoney=137800,RewXP=564000,MsgID=19434}
	Quest_RandomKill[64]={TargetType=QuestTargetType["Kill"],TargetID1=1050023,Num1=200,RewMoney=141300,RewXP=582000,MsgID=19435}
	Quest_RandomKill[65]={TargetType=QuestTargetType["Kill"],TargetID1=1050023,Num1=200,RewMoney=144900,RewXP=606000,MsgID=19436}
	Quest_RandomKill[66]={TargetType=QuestTargetType["Kill"],TargetID1=1060005,Num1=200,RewMoney=148600,RewXP=627000,MsgID=19437}
	Quest_RandomKill[67]={TargetType=QuestTargetType["Kill"],TargetID1=1060005,Num1=200,RewMoney=152400,RewXP=648000,MsgID=19438}
	Quest_RandomKill[68]={TargetType=QuestTargetType["Kill"],TargetID1=1060005,Num1=200,RewMoney=156300,RewXP=670000,MsgID=19439}
	Quest_RandomKill[69]={TargetType=QuestTargetType["Kill"],TargetID1=1060011,Num1=200,RewMoney=160300,RewXP=720000,MsgID=19440}
	Quest_RandomKill[70]={TargetType=QuestTargetType["Kill"],TargetID1=1060011,Num1=300,RewMoney=164400,RewXP=1073000,MsgID=19441}
	Quest_RandomKill[71]={TargetType=QuestTargetType["Kill"],TargetID1=1060011,Num1=300,RewMoney=168600,RewXP=1107000,MsgID=19442}
	Quest_RandomKill[72]={TargetType=QuestTargetType["Kill"],TargetID1=1060017,Num1=300,RewMoney=172900,RewXP=1143000,MsgID=19443}
	Quest_RandomKill[73]={TargetType=QuestTargetType["Kill"],TargetID1=1060017,Num1=300,RewMoney=177300,RewXP=1180000,MsgID=19444}
	Quest_RandomKill[74]={TargetType=QuestTargetType["Kill"],TargetID1=1060017,Num1=300,RewMoney=181800,RewXP=1217000,MsgID=19445}
	Quest_RandomKill[75]={TargetType=QuestTargetType["Kill"],TargetID1=1060023,Num1=300,RewMoney=186400,RewXP=1256000,MsgID=19446}
	Quest_RandomKill[76]={TargetType=QuestTargetType["Kill"],TargetID1=1060023,Num1=300,RewMoney=191100,RewXP=1293000,MsgID=19447}
	Quest_RandomKill[77]={TargetType=QuestTargetType["Kill"],TargetID1=1060023,Num1=300,RewMoney=195900,RewXP=1331000,MsgID=19448}
	Quest_RandomKill[78]={TargetType=QuestTargetType["Kill"],TargetID1=1060028,Num1=300,RewMoney=200800,RewXP=1370000,MsgID=19449}
	Quest_RandomKill[79]={TargetType=QuestTargetType["Kill"],TargetID1=1060028,Num1=300,RewMoney=205900,RewXP=2116000,MsgID=19450}
	Quest_RandomKill[80]={TargetType=QuestTargetType["Kill"],TargetID1=1060028,Num1=400,RewMoney=211100,RewXP=2178000,MsgID=19451}
	Quest_RandomKill[81]={TargetType=QuestTargetType["Kill"],TargetID1=1070005,Num1=400,RewMoney=216400,RewXP=2241000,MsgID=19452}
	Quest_RandomKill[82]={TargetType=QuestTargetType["Kill"],TargetID1=1070005,Num1=400,RewMoney=221900,RewXP=2303000,MsgID=19453}
	Quest_RandomKill[83]={TargetType=QuestTargetType["Kill"],TargetID1=1070005,Num1=400,RewMoney=227500,RewXP=2370000,MsgID=19454}
	Quest_RandomKill[84]={TargetType=QuestTargetType["Kill"],TargetID1=1070014,Num1=400,RewMoney=233200,RewXP=2438000,MsgID=19455}
	Quest_RandomKill[85]={TargetType=QuestTargetType["Kill"],TargetID1=1070014,Num1=400,RewMoney=239100,RewXP=2505000,MsgID=19456}
	Quest_RandomKill[86]={TargetType=QuestTargetType["Kill"],TargetID1=1070014,Num1=400,RewMoney=245100,RewXP=2572000,MsgID=19457}
	Quest_RandomKill[87]={TargetType=QuestTargetType["Kill"],TargetID1=1070016,Num1=400,RewMoney=251300,RewXP=2640000,MsgID=19458}
	Quest_RandomKill[88]={TargetType=QuestTargetType["Kill"],TargetID1=1070016,Num1=400,RewMoney=257600,RewXP=2709000,MsgID=19459}
	Quest_RandomKill[89]={TargetType=QuestTargetType["Kill"],TargetID1=1070016,Num1=400,RewMoney=264100,RewXP=2782000,MsgID=19460}
	Quest_RandomKill[90]={TargetType=QuestTargetType["Kill"],TargetID1=1070022,Num1=500,RewMoney=270800,RewXP=3490000,MsgID=19461}
	Quest_RandomKill[91]={TargetType=QuestTargetType["Kill"],TargetID1=1070022,Num1=500,RewMoney=277600,RewXP=3546000,MsgID=19462}
	Quest_RandomKill[92]={TargetType=QuestTargetType["Kill"],TargetID1=1070022,Num1=500,RewMoney=284600,RewXP=3602000,MsgID=19463}
	Quest_RandomKill[93]={TargetType=QuestTargetType["Kill"],TargetID1=1070030,Num1=500,RewMoney=291800,RewXP=3662000,MsgID=19464}
	Quest_RandomKill[94]={TargetType=QuestTargetType["Kill"],TargetID1=1070030,Num1=500,RewMoney=299100,RewXP=3720000,MsgID=19465}
	Quest_RandomKill[95]={TargetType=QuestTargetType["Kill"],TargetID1=1070030,Num1=500,RewMoney=306600,RewXP=3777000,MsgID=19466}
	Quest_RandomKill[96]={TargetType=QuestTargetType["Kill"],TargetID1=1070037,Num1=500,RewMoney=314300,RewXP=3833000,MsgID=19467}
	Quest_RandomKill[97]={TargetType=QuestTargetType["Kill"],TargetID1=1070037,Num1=500,RewMoney=322200,RewXP=3887000,MsgID=19468}
	Quest_RandomKill[98]={TargetType=QuestTargetType["Kill"],TargetID1=1070037,Num1=500,RewMoney=330300,RewXP=3941000,MsgID=19469}
	Quest_RandomKill[99]={TargetType=QuestTargetType["Kill"],TargetID1=1070045,Num1=500,RewMoney=338600,RewXP=3996000,MsgID=19470}
	Quest_RandomKill[100]={TargetType=QuestTargetType["Kill"],TargetID1=1070045,Num1=500,RewMoney=347100,RewXP=4050000,MsgID=19471}


	local Index = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if Index >100 then
		Index = 100
	end
	--记录要传送的posID和接取任务的次数
	role.SetRoleScriptData(RoleID, 1, RoleDataType["InstantKillCre"], times*100 + Quest_RandomKill[Index].MsgID%100)
	--初始化任务的动态数据
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_RandomKill, Index))
	--同步客户端总任务环数和任务说明
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 1, Quest_RandomKill[Index].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
aux.RegisterQuestEvent(30334, 7, "q30334_OnQuestInit")

--s05 d03 s06 d05 d06 s07 s08 d09可用
function i4400701_CanUse(MapID, InstanceID, ItemTypeID, TargetID)
    local toPos = {}
	toPos[1] = {3424071720,3951678273}
	toPos[2] = {3424071720,3951678273}
	toPos[3] = {3424071720,3951678273}
	toPos[4] = {3424071720,3951678273}
	toPos[5] = {3424071720,3951678273}
	toPos[6] = {3424071720,3951678273}
	toPos[7] = {3424071720,3951678273}
	toPos[8] = {3424071720,3951678273}
	toPos[9] = {3424071720,3951678273}
	toPos[10] = {3424071720,3951678273}
	toPos[11] = {3424071720,3951678273}
	toPos[12] = {3424071720,3951678273}
	toPos[13] = {3424071720,3951678273}
	toPos[14] = {3424071720,3951678273}
	toPos[15] = {3424071720,3951678273}
	toPos[16] = {3424071720,3951678273}
	toPos[17] = {3424071720,3951678273}
	toPos[18] = {3424071720,3951678273}
	toPos[19] = {3424071720,3951678273}
	toPos[20] = {3424071720,3951678273}
	toPos[21] = {3424071720,3951678273}
	toPos[22] = {3424072488,3951678785,3951679041}
	toPos[23] = {3424072488,3951678785,3951679041}
	toPos[24] = {3424072488,3951678785,3951679041}
	toPos[25] = {3424072488,3951678785,3951679041}
	toPos[26] = {3424072488,3951678785,3951679041}
	toPos[27] = {3424072488,3951678785,3951679041}
	toPos[28] = {3424072488,3951678785,3951679041}
	toPos[29] = {3424072488,3951678785,3951679041}
	toPos[30] = {3424072488,3951678785,3951679041}
	toPos[31] = {3424072488,3951678785,3951679041}
	toPos[32] = {3424072488,3951678785,3951679041}
	toPos[33] = {3424072488,3951678785,3951679041}
	toPos[34] = {3424072488,3951678785,3951679041}
	toPos[35] = {3424072488,3951678785,3951679041}
	toPos[36] = {3424072488,3951678785,3951679041}
	toPos[37] = {3424072232,3951679297}
	toPos[38] = {3424072232,3951679297}
	toPos[39] = {3424072232,3951679297}
	toPos[40] = {3424072232,3951679297}
	toPos[41] = {3424072232,3951679297}
	toPos[42] = {3424072232,3951679297}
	toPos[43] = {3424072232,3951679297}
	toPos[44] = {3424072232,3951679297}
	toPos[45] = {3424072232,3951679297}
	toPos[46] = {3424072232,3951679297}
	toPos[47] = {3424072232,3951679297}
	toPos[48] = {3424072232,3951679297}
	toPos[49] = {3424072232,3951679297}
	toPos[50] = {3424072232,3951679297}
	toPos[51] = {3424072232,3951679297}
	toPos[52] = {3424075048,3951679809}
	toPos[53] = {3424075048,3951679809}
	toPos[54] = {3424075048,3951679809}
	toPos[55] = {3424075048,3951679809}
	toPos[56] = {3424075048,3951679809}
	toPos[57] = {3424075048,3951679809}
	toPos[58] = {3424075048,3951679809}
	toPos[59] = {3424075048,3951679809}
	toPos[60] = {3424075048,3951679809}
	toPos[61] = {3424075048,3951679809}
	toPos[62] = {3424075048,3951679809}
	toPos[63] = {3424075048,3951679809}
	toPos[64] = {3424075048,3951679809}
	toPos[65] = {3424075048,3951679809}
	toPos[66] = {3424075048,3951679809}
	toPos[67] = {3424075048,3951679809}
	toPos[68] = {3424075048,3951679809}
	toPos[69] = {3424075048,3951679809}
	toPos[70] = {3424075048,3951679809}
	toPos[71] = {3424075048,3951679809}

	local toIndex = role.GetRoleScriptData(TargetID, 1, RoleDataType["InstantKillCre"])%100
	local bool = false
	for k,v in pairs(toPos[toIndex]) do
	    if MapID == v then
		    bool = true
			break
		end
	end

	if role.GetBagFreeSize(TargetID) < 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)		--背包空<1
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		return 32,false
	elseif bool then
		return 0,false
	else
	    return 47,false
	end
end
aux.RegisterItemEvent(4400701, 0, "i4400701_CanUse")

function i4400701_Use(MapID, InstanceID, ItemTypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4400702, 1, -1, 8, 420)
	local toIndex = role.GetRoleScriptData(TargetID, 1, RoleDataType["InstantKillCre"])%100
	local toPos = {}
	toPos[1] = {map=3424071720,x = 2302,y = 183,z = 2443}
	toPos[2] = {map=3424071720,x = 2302,y = 183,z = 2443}
	toPos[3] = {map=3424071720,x = 2302,y = 183,z = 2443}
	toPos[4] = {map=3951678273,x = 312,y = 9,z = 48}
	toPos[5] = {map=3951678273,x = 312,y = 9,z = 48}
	toPos[6] = {map=3951678273,x = 312,y = 9,z = 48}
	toPos[7] = {map=3424071720,x = 2203,y = 166,z = 1015}
	toPos[8] = {map=3424071720,x = 2203,y = 166,z = 1015}
	toPos[9] = {map=3424071720,x = 2203,y = 166,z = 1015}
	toPos[10] = {map=3424071720,x = 1101,y = 185,z = 586}
	toPos[11] = {map=3424071720,x = 1101,y = 185,z = 586}
	toPos[12] = {map=3424071720,x = 1101,y = 185,z = 586}
	toPos[13] = {map=3424071720,x = 1748,y = 233,z = 1650}
	toPos[14] = {map=3424071720,x = 1748,y = 233,z = 1650}
	toPos[15] = {map=3424071720,x = 1748,y = 233,z = 1650}
	toPos[16] = {map=3424071720,x = 836,y = 180,z = 3377}
	toPos[17] = {map=3424071720,x = 836,y = 180,z = 3377}
	toPos[18] = {map=3424071720,x = 836,y = 180,z = 3377}
	toPos[19] = {map=3424071720,x = 3344,y = 186,z = 2901}
	toPos[20] = {map=3424071720,x = 3344,y = 186,z = 2901}
	toPos[21] = {map=3424071720,x = 3344,y = 186,z = 2901}
	toPos[22] = {map=3424072488,x = 683,y = 141,z = 398}
	toPos[23] = {map=3424072488,x = 683,y = 141,z = 398}
	toPos[24] = {map=3424072488,x = 683,y = 141,z = 398}
	toPos[25] = {map=3424072488,x = 2560,y = 100,z = 523}
	toPos[26] = {map=3424072488,x = 2560,y = 100,z = 523}
	toPos[27] = {map=3424072488,x = 2560,y = 100,z = 523}
	toPos[28] = {map=3951678785,x = 287,y = 32,z = 21}
	toPos[29] = {map=3951678785,x = 287,y = 32,z = 21}
	toPos[30] = {map=3951678785,x = 287,y = 32,z = 21}
	toPos[31] = {map=3424072488,x = 1889,y = 120,z = 1755}
	toPos[32] = {map=3424072488,x = 1889,y = 120,z = 1755}
	toPos[33] = {map=3424072488,x = 1889,y = 120,z = 1755}
	toPos[34] = {map=3951679041,x = 454,y = 0,z = 328}
	toPos[35] = {map=3951679041,x = 454,y = 0,z = 328}
	toPos[36] = {map=3951679041,x = 454,y = 0,z = 328}
	toPos[37] = {map=3424072232,x = 871,y = 264,z = 1775}
	toPos[38] = {map=3424072232,x = 871,y = 264,z = 1775}
	toPos[39] = {map=3424072232,x = 871,y = 264,z = 1775}
	toPos[40] = {map=3424072232,x = 1472,y = 73,z = 962}
	toPos[41] = {map=3424072232,x = 1472,y = 73,z = 962}
	toPos[42] = {map=3424072232,x = 1472,y = 73,z = 962}
	toPos[43] = {map=3951679297,x = 863,y = 0,z = 508}
	toPos[44] = {map=3951679297,x = 863,y = 0,z = 508}
	toPos[45] = {map=3951679297,x = 863,y = 0,z = 508}
	toPos[46] = {map=3424072232,x = 1936,y = 66,z = 932}
	toPos[47] = {map=3424072232,x = 1936,y = 66,z = 932}
	toPos[48] = {map=3424072232,x = 1936,y = 66,z = 932}
	toPos[49] = {map=3424072232,x = 1626,y = 90,z = 2412}
	toPos[50] = {map=3424072232,x = 1626,y = 90,z = 2412}
	toPos[51] = {map=3424072232,x = 1626,y = 90,z = 2412}
	toPos[52] = {map=3424075048,x = 1956,y = 217,z = 2559}
	toPos[53] = {map=3424075048,x = 1956,y = 217,z = 2559}
	toPos[54] = {map=3424075048,x = 1956,y = 217,z = 2559}
	toPos[55] = {map=3424075048,x = 2107,y = 163,z = 1696}
	toPos[56] = {map=3424075048,x = 2107,y = 163,z = 1696}
	toPos[57] = {map=3424075048,x = 2107,y = 163,z = 1696}
	toPos[58] = {map=3424075048,x = 1876,y = 164,z = 1523}
	toPos[59] = {map=3424075048,x = 1876,y = 164,z = 1523}
	toPos[60] = {map=3424075048,x = 1876,y = 164,z = 1523}
	toPos[61] = {map=3424075048,x = 1274,y = 200,z = 2166}
	toPos[62] = {map=3424075048,x = 1274,y = 200,z = 2166}
	toPos[63] = {map=3424075048,x = 1274,y = 200,z = 2166}
	toPos[64] = {map=3951679809,x = 326,y = 0,z = 171}
	toPos[65] = {map=3951679809,x = 326,y = 0,z = 171}
	toPos[66] = {map=3951679809,x = 326,y = 0,z = 171}
	toPos[67] = {map=3424075048,x = 1257,y = 86,z = 956}
	toPos[68] = {map=3424075048,x = 1257,y = 86,z = 956}
	toPos[69] = {map=3424075048,x = 1257,y = 86,z = 956}
	toPos[70] = {map=3424075048,x = 662,y = 203,z = 1147}
	toPos[71] = {map=3424075048,x = 662,y = 203,z = 1147}

	role.RoleGotoNewMap(MapID, InstanceID, TargetID, toPos[toIndex].map, toPos[toIndex].x, toPos[toIndex].y, toPos[toIndex].z)
end
aux.RegisterItemEvent(4400701, 1, "i4400701_Use")

--s05 d03 s06 d05 d06 s07 s08 d09可用
function i4400702_CanUse(MapID, InstanceID, ItemTypeID, TargetID)
    local toPos = {}
	toPos[1] = {3424071720,3951678273}
	toPos[2] = {3424071720,3951678273}
	toPos[3] = {3424071720,3951678273}
	toPos[4] = {3424071720,3951678273}
	toPos[5] = {3424071720,3951678273}
	toPos[6] = {3424071720,3951678273}
	toPos[7] = {3424071720,3951678273}
	toPos[8] = {3424071720,3951678273}
	toPos[9] = {3424071720,3951678273}
	toPos[10] = {3424071720,3951678273}
	toPos[11] = {3424071720,3951678273}
	toPos[12] = {3424071720,3951678273}
	toPos[13] = {3424071720,3951678273}
	toPos[14] = {3424071720,3951678273}
	toPos[15] = {3424071720,3951678273}
	toPos[16] = {3424071720,3951678273}
	toPos[17] = {3424071720,3951678273}
	toPos[18] = {3424071720,3951678273}
	toPos[19] = {3424071720,3951678273}
	toPos[20] = {3424071720,3951678273}
	toPos[21] = {3424071720,3951678273}
	toPos[22] = {3424072488,3951678785,3951679041}
	toPos[23] = {3424072488,3951678785,3951679041}
	toPos[24] = {3424072488,3951678785,3951679041}
	toPos[25] = {3424072488,3951678785,3951679041}
	toPos[26] = {3424072488,3951678785,3951679041}
	toPos[27] = {3424072488,3951678785,3951679041}
	toPos[28] = {3424072488,3951678785,3951679041}
	toPos[29] = {3424072488,3951678785,3951679041}
	toPos[30] = {3424072488,3951678785,3951679041}
	toPos[31] = {3424072488,3951678785,3951679041}
	toPos[32] = {3424072488,3951678785,3951679041}
	toPos[33] = {3424072488,3951678785,3951679041}
	toPos[34] = {3424072488,3951678785,3951679041}
	toPos[35] = {3424072488,3951678785,3951679041}
	toPos[36] = {3424072488,3951678785,3951679041}
	toPos[37] = {3424072232,3951679297}
	toPos[38] = {3424072232,3951679297}
	toPos[39] = {3424072232,3951679297}
	toPos[40] = {3424072232,3951679297}
	toPos[41] = {3424072232,3951679297}
	toPos[42] = {3424072232,3951679297}
	toPos[43] = {3424072232,3951679297}
	toPos[44] = {3424072232,3951679297}
	toPos[45] = {3424072232,3951679297}
	toPos[46] = {3424072232,3951679297}
	toPos[47] = {3424072232,3951679297}
	toPos[48] = {3424072232,3951679297}
	toPos[49] = {3424072232,3951679297}
	toPos[50] = {3424072232,3951679297}
	toPos[51] = {3424072232,3951679297}
	toPos[52] = {3424075048,3951679809}
	toPos[53] = {3424075048,3951679809}
	toPos[54] = {3424075048,3951679809}
	toPos[55] = {3424075048,3951679809}
	toPos[56] = {3424075048,3951679809}
	toPos[57] = {3424075048,3951679809}
	toPos[58] = {3424075048,3951679809}
	toPos[59] = {3424075048,3951679809}
	toPos[60] = {3424075048,3951679809}
	toPos[61] = {3424075048,3951679809}
	toPos[62] = {3424075048,3951679809}
	toPos[63] = {3424075048,3951679809}
	toPos[64] = {3424075048,3951679809}
	toPos[65] = {3424075048,3951679809}
	toPos[66] = {3424075048,3951679809}
	toPos[67] = {3424075048,3951679809}
	toPos[68] = {3424075048,3951679809}
	toPos[69] = {3424075048,3951679809}
	toPos[70] = {3424075048,3951679809}
	toPos[71] = {3424075048,3951679809}
	local toIndex = role.GetRoleScriptData(TargetID, 1, RoleDataType["InstantKillCre"])%100
	local bool = false
	for k,v in pairs(toPos[toIndex]) do
	    if MapID == v then
		    bool = true
			break
		end
	end

	if bool then
		return 0,false
	else
		return 47,false
	end
end
aux.RegisterItemEvent(4400702, 0, "i4400702_CanUse")

function i4400702_Use(MapID, InstanceID, ItemTypeID, TargetID)
	local fromIndex = role.GetRoleScriptData(TargetID, 1, RoleDataType["InstantKillCre"])%100
	if fromIndex <= 21 then
		role.RoleGotoNewMap(MapID, InstanceID, TargetID, 3424071720, 2602, 148, 2567)
	elseif fromIndex <= 36 then
		role.RoleGotoNewMap(MapID, InstanceID, TargetID, 3424072488, 1088, 78, 1398)
	elseif fromIndex <= 51 then
		role.RoleGotoNewMap(MapID, InstanceID, TargetID, 3424072232, 565, 259, 1991)
	else
		role.RoleGotoNewMap(MapID, InstanceID, TargetID, 3424075048, 1645, 229, 2078)
	end
end
aux.RegisterItemEvent(4400702, 1, "i4400702_Use")


--完成任务
function q30334_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["InstantKillCre"], role.GetRoleScriptData(RoleID, 1, RoleDataType["InstantKillCre"])-role.GetRoleScriptData(RoleID, 1, RoleDataType["InstantKillCre"])%100 + 100)
	--玩家统计
	UpdateRoleData(RoleID, 19, 1)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level < 30 then
		level = 30
	elseif level > 100 then
		level = 100
	end
	--vip特殊奖励
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 90034) == true then
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["shaluVIP"]) == 0 or role.GetRoleScriptData(RoleID, 1, RoleDataType["shaluVIP"]) == nil then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["shaluVIP"], tonumber(os.date("%j")))
		end
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["shaluVIP"]) ~= tonumber(os.date("%j")) then
			local theDays = tonumber(os.date("%j")) - role.GetRoleScriptData(RoleID, 1, RoleDataType["shaluVIP"]) - 1
			local addedExp = {}
			addedExp[30]= 33000
			addedExp[31]= 35000
			addedExp[32]= 36000
			addedExp[33]= 39000
			addedExp[34]= 41000
			addedExp[35]= 44000
			addedExp[36]= 47000
			addedExp[37]= 49000
			addedExp[38]= 52000
			addedExp[39]= 55000
			addedExp[40]= 92000
			addedExp[41]= 97000
			addedExp[42]= 101000
			addedExp[43]= 108000
			addedExp[44]= 113000
			addedExp[45]= 124000
			addedExp[46]= 129000
			addedExp[47]= 136000
			addedExp[48]= 142000
			addedExp[49]= 149000
			addedExp[50]= 243000
			addedExp[51]= 255000
			addedExp[52]= 265000
			addedExp[53]= 276000
			addedExp[54]= 289000
			addedExp[55]= 314000
			addedExp[56]= 327000
			addedExp[57]= 339000
			addedExp[58]= 352000
			addedExp[59]= 388000
			addedExp[60]= 507000
			addedExp[61]= 525000
			addedExp[62]= 544000
			addedExp[63]= 564000
			addedExp[64]= 582000
			addedExp[65]= 606000
			addedExp[66]= 627000
			addedExp[67]= 648000
			addedExp[68]= 670000
			addedExp[69]= 720000
			addedExp[70]= 1073000
			addedExp[71]= 1107000
			addedExp[72]= 1143000
			addedExp[73]= 1180000
			addedExp[74]= 1217000
			addedExp[75]= 1256000
			addedExp[76]= 1293000
			addedExp[77]= 1331000
			addedExp[78]= 1370000
			addedExp[79]= 2116000
			addedExp[80]= 2178000
			addedExp[81]= 2241000
			addedExp[82]= 2303000
			addedExp[83]= 2370000
			addedExp[84]= 2438000
			addedExp[85]= 2505000
			addedExp[86]= 2572000
			addedExp[87]= 2640000
			addedExp[88]= 2709000
			addedExp[89]= 2782000
			addedExp[90]= 3490000
			addedExp[91]= 3546000
			addedExp[92]= 3602000
			addedExp[93]= 3662000
			addedExp[94]= 3720000
			addedExp[95]= 3777000
			addedExp[96]= 3833000
			addedExp[97]= 3887000
			addedExp[98]= 3941000
			addedExp[99]= 3996000
			addedExp[100]= 4050000



			if theDays >= 4 or theDays >= -361 and theDays < 0 then
				role.AddRoleExp(MapID, InstanceID, RoleID, addedExp[level]*4)
			elseif theDays == 3 or theDays == -362 then
				role.AddRoleExp(MapID, InstanceID, RoleID, addedExp[level]*3)
			elseif theDays == 2 or theDays == -363 then
				role.AddRoleExp(MapID, InstanceID, RoleID, addedExp[level]*2)
			elseif theDays == 1 or theDays == -364 then
				role.AddRoleExp(MapID, InstanceID, RoleID, addedExp[level])
			end
		end
	end
	local quest_money={}
			quest_money[30]=60000
			quest_money[31]=61500
			quest_money[32]=63100
			quest_money[33]=64700
			quest_money[34]=66400
			quest_money[35]=68100
			quest_money[36]=69900
			quest_money[37]=71700
			quest_money[38]=73500
			quest_money[39]=75400
			quest_money[40]=77300
			quest_money[41]=79300
			quest_money[42]=81300
			quest_money[43]=83400
			quest_money[44]=85500
			quest_money[45]=87700
			quest_money[46]=89900
			quest_money[47]=92200
			quest_money[48]=94600
			quest_money[49]=97000
			quest_money[50]=99500
			quest_money[51]=102000
			quest_money[52]=104600
			quest_money[53]=107300
			quest_money[54]=110000
			quest_money[55]=112800
			quest_money[56]=115700
			quest_money[57]=118600
			quest_money[58]=121600
			quest_money[59]=124700
			quest_money[60]=127900
			quest_money[61]=131100
			quest_money[62]=134400
			quest_money[63]=137800
			quest_money[64]=141300
			quest_money[65]=144900
			quest_money[66]=148600
			quest_money[67]=152400
			quest_money[68]=156300
			quest_money[69]=160300
			quest_money[70]=164400
			quest_money[71]=168600
			quest_money[72]=172900
			quest_money[73]=177300
			quest_money[74]=181800
			quest_money[75]=186400
			quest_money[76]=191100
			quest_money[77]=195900
			quest_money[78]=200800
			quest_money[79]=205900
			quest_money[80]=211100
			quest_money[81]=216400
			quest_money[82]=221900
			quest_money[83]=227500
			quest_money[84]=233200
			quest_money[85]=239100
			quest_money[86]=245100
			quest_money[87]=251300
			quest_money[88]=257600
			quest_money[89]=264100
			quest_money[90]=270800
			quest_money[91]=277600
			quest_money[92]=284600
			quest_money[93]=291800
			quest_money[94]=299100
			quest_money[95]=306600
			quest_money[96]=314300
			quest_money[97]=322200
			quest_money[98]=330300
			quest_money[99]=338600
			quest_money[100]=347100
	local Money = quest_money[level]
	local jinqiantequan = 0
	for i = 1,6 do
		if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 90048+i) then
			jinqiantequan = 1
		elseif role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 90048+i+6) then
			jinqiantequan = 2
		elseif role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 90048+i+12) then
			jinqiantequan = 3
		end
	end
	if jinqiantequan == 1 then
		local Money2 = 0.2*Money
		role.AddRoleSilver(MapID, InstanceID, RoleID, Money2)
	elseif jinqiantequan == 2 then
		local Money2 = 1*Money
		role.AddRoleSilver(MapID, InstanceID, RoleID, Money2)
	elseif jinqiantequan == 3 then
		local Money2 = 2*Money
		role.AddRoleSilver(MapID, InstanceID, RoleID, Money2)
	end
	role.AddRoleItem(MapID, InstanceID, RoleID, 4800287, 2, -1, 8, 420)
	if level >= 60 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 3500011, 1, -1, 8, 420)
	end
	role.SetRoleScriptData(RoleID, 1, RoleDataType["shaluVIP"], tonumber(os.date("%j")))
end

aux.RegisterQuestEvent(30334, 1, "q30334_OnComplete")
