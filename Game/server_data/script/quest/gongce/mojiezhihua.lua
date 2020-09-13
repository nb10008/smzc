--RoleDataType["mojieVIP"]记录魔界之花完成日期
MoJieZhiHua_info = {}
MoJieZhiHua_Player = {}
--接取任务
function q30333_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)

	local Index = 30
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	Index = rolelevel
	if rolelevel < 30 then
	    Index = 30
	elseif rolelevel > 100 then
	    Index = 100
	end

	local Quest_MoJieZhiHua = {}
	Quest_MoJieZhiHua[30]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=30000,RewMoney=60000,MsgID=19301}
	Quest_MoJieZhiHua[31]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=35000,RewMoney=61500,MsgID=19301}
	Quest_MoJieZhiHua[32]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=40000,RewMoney=63100,MsgID=19301}
	Quest_MoJieZhiHua[33]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=45000,RewMoney=64700,MsgID=19301}
	Quest_MoJieZhiHua[34]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=50000,RewMoney=66400,MsgID=19301}
	Quest_MoJieZhiHua[35]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=55000,RewMoney=68100,MsgID=19301}
	Quest_MoJieZhiHua[36]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=64000,RewMoney=69900,MsgID=19301}
	Quest_MoJieZhiHua[37]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=72000,RewMoney=71700,MsgID=19301}
	Quest_MoJieZhiHua[38]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=85000,RewMoney=73500,MsgID=19301}
	Quest_MoJieZhiHua[39]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=103000,RewMoney=75400,MsgID=19301}
	Quest_MoJieZhiHua[40]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=126000,RewMoney=77300,MsgID=19301}
	Quest_MoJieZhiHua[41]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=141000,RewMoney=79300,MsgID=19301}
	Quest_MoJieZhiHua[42]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=157000,RewMoney=81300,MsgID=19301}
	Quest_MoJieZhiHua[43]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=173000,RewMoney=83400,MsgID=19301}
	Quest_MoJieZhiHua[44]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=189000,RewMoney=85500,MsgID=19301}
	Quest_MoJieZhiHua[45]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=209000,RewMoney=87700,MsgID=19301}
	Quest_MoJieZhiHua[46]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=231000,RewMoney=89900,MsgID=19301}
	Quest_MoJieZhiHua[47]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=255000,RewMoney=92200,MsgID=19301}
	Quest_MoJieZhiHua[48]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=280000,RewMoney=94600,MsgID=19301}
	Quest_MoJieZhiHua[49]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=307000,RewMoney=97000,MsgID=19301}
	Quest_MoJieZhiHua[50]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=349000,RewMoney=99500,MsgID=19302}
	Quest_MoJieZhiHua[51]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=365000,RewMoney=102000,MsgID=19302}
	Quest_MoJieZhiHua[52]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=383000,RewMoney=104600,MsgID=19302}
	Quest_MoJieZhiHua[53]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=401000,RewMoney=107300,MsgID=19302}
	Quest_MoJieZhiHua[54]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=417000,RewMoney=110000,MsgID=19302}
	Quest_MoJieZhiHua[55]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=456000,RewMoney=112800,MsgID=19302}
	Quest_MoJieZhiHua[56]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=477000,RewMoney=115700,MsgID=19302}
	Quest_MoJieZhiHua[57]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=498000,RewMoney=118600,MsgID=19302}
	Quest_MoJieZhiHua[58]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=519000,RewMoney=121600,MsgID=19302}
	Quest_MoJieZhiHua[59]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=540000,RewMoney=124700,MsgID=19302}
	Quest_MoJieZhiHua[60]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=551000,RewMoney=127900,MsgID=19302}
	Quest_MoJieZhiHua[61]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=561000,RewMoney=131100,MsgID=19302}
	Quest_MoJieZhiHua[62]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=566000,RewMoney=134400,MsgID=19302}
	Quest_MoJieZhiHua[63]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=572000,RewMoney=137800,MsgID=19302}
	Quest_MoJieZhiHua[64]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=579000,RewMoney=141300,MsgID=19302}
	Quest_MoJieZhiHua[65]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=587000,RewMoney=144900,MsgID=19303}
	Quest_MoJieZhiHua[66]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=594000,RewMoney=148600,MsgID=19303}
	Quest_MoJieZhiHua[67]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=602000,RewMoney=152400,MsgID=19303}
	Quest_MoJieZhiHua[68]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=608000,RewMoney=156300,MsgID=19303}
	Quest_MoJieZhiHua[69]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=615000,RewMoney=160300,MsgID=19303}
	Quest_MoJieZhiHua[70]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=623000,RewMoney=164400,MsgID=19303}
	Quest_MoJieZhiHua[71]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=629000,RewMoney=168600,MsgID=19303}
	Quest_MoJieZhiHua[72]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=638000,RewMoney=172900,MsgID=19303}
	Quest_MoJieZhiHua[73]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=646000,RewMoney=177300,MsgID=19303}
	Quest_MoJieZhiHua[74]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=656000,RewMoney=181800,MsgID=19303}
	Quest_MoJieZhiHua[75]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=663000,RewMoney=186400,MsgID=19303}
	Quest_MoJieZhiHua[76]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=672000,RewMoney=191100,MsgID=19303}
	Quest_MoJieZhiHua[77]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=682000,RewMoney=195900,MsgID=19303}
	Quest_MoJieZhiHua[78]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=690000,RewMoney=200800,MsgID=19303}
	Quest_MoJieZhiHua[79]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=701000,RewMoney=205900,MsgID=19303}
	Quest_MoJieZhiHua[80]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=710000,RewMoney=211100,MsgID=19304}
	Quest_MoJieZhiHua[81]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=719000,RewMoney=216400,MsgID=19304}
	Quest_MoJieZhiHua[82]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=731000,RewMoney=221900,MsgID=19304}
	Quest_MoJieZhiHua[83]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=740000,RewMoney=227500,MsgID=19304}
	Quest_MoJieZhiHua[84]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=750000,RewMoney=233200,MsgID=19304}
	Quest_MoJieZhiHua[85]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=761000,RewMoney=239100,MsgID=19304}
	Quest_MoJieZhiHua[86]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=773000,RewMoney=245100,MsgID=19304}
	Quest_MoJieZhiHua[87]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=783000,RewMoney=251300,MsgID=19304}
	Quest_MoJieZhiHua[88]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=796000,RewMoney=257600,MsgID=19304}
	Quest_MoJieZhiHua[89]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=807000,RewMoney=264100,MsgID=19304}
	Quest_MoJieZhiHua[90]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=819000,RewMoney=270800,MsgID=19304}
	Quest_MoJieZhiHua[91]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=831000,RewMoney=277600,MsgID=19304}
	Quest_MoJieZhiHua[92]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=843000,RewMoney=284600,MsgID=19304}
	Quest_MoJieZhiHua[93]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=856000,RewMoney=291800,MsgID=19304}
	Quest_MoJieZhiHua[94]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=868000,RewMoney=299100,MsgID=19304}
	Quest_MoJieZhiHua[95]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=881000,RewMoney=306600,MsgID=19304}
	Quest_MoJieZhiHua[96]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=893000,RewMoney=314300,MsgID=19304}
	Quest_MoJieZhiHua[97]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=906000,RewMoney=322200,MsgID=19304}
	Quest_MoJieZhiHua[98]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=920000,RewMoney=330300,MsgID=19304}
	Quest_MoJieZhiHua[99]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=934000,RewMoney=338600,MsgID=19304}
	Quest_MoJieZhiHua[100]={TargetType=QuestTargetType["Collect"],TargetID1=4400602,Num1=3,RewXP=947000,RewMoney=347100,MsgID=19304}



	--初始化任务的动态数据
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_MoJieZhiHua, Index))

	local k = math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["MoJieZhiHua"])/1000)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["MoJieZhiHua"],k*1000+Index)


	--同步客户端总任务环数和任务说明
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 1, Quest_MoJieZhiHua[Index].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end

--完成任务
function q30333_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	role.SetRoleScriptData(RoleID, 1, RoleDataType["MoJieZhiHua"], role.GetRoleScriptData(RoleID, 1, RoleDataType["MoJieZhiHua"])+1000)
	--玩家统计
	UpdateRoleData(RoleID, 18, 1)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level < 30 then
		level = 30
	elseif level > 100 then
		level = 100
	end
	--vip特殊奖励
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 90034) == true then
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["mojieVIP"]) == 0 or role.GetRoleScriptData(RoleID, 1, RoleDataType["mojieVIP"]) == nil then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["mojieVIP"], tonumber(os.date("%j")))
		end
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["mojieVIP"]) ~= tonumber(os.date("%j")) then
			local theDays = tonumber(os.date("%j")) - role.GetRoleScriptData(RoleID, 1, RoleDataType["mojieVIP"]) - 1
			local addedExp = {}
			addedExp[30]= 30000
			addedExp[31]= 35000
			addedExp[32]= 40000
			addedExp[33]= 45000
			addedExp[34]= 50000
			addedExp[35]= 55000
			addedExp[36]= 64000
			addedExp[37]= 72000
			addedExp[38]= 85000
			addedExp[39]= 103000
			addedExp[40]= 126000
			addedExp[41]= 141000
			addedExp[42]= 157000
			addedExp[43]= 173000
			addedExp[44]= 189000
			addedExp[45]= 209000
			addedExp[46]= 231000
			addedExp[47]= 255000
			addedExp[48]= 280000
			addedExp[49]= 307000
			addedExp[50]= 349000
			addedExp[51]= 365000
			addedExp[52]= 383000
			addedExp[53]= 401000
			addedExp[54]= 417000
			addedExp[55]= 456000
			addedExp[56]= 477000
			addedExp[57]= 498000
			addedExp[58]= 519000
			addedExp[59]= 540000
			addedExp[60]= 551000
			addedExp[61]= 561000
			addedExp[62]= 566000
			addedExp[63]= 572000
			addedExp[64]= 579000
			addedExp[65]= 587000
			addedExp[66]= 594000
			addedExp[67]= 602000
			addedExp[68]= 608000
			addedExp[69]= 615000
			addedExp[70]= 623000
			addedExp[71]= 629000
			addedExp[72]= 638000
			addedExp[73]= 646000
			addedExp[74]= 656000
			addedExp[75]= 663000
			addedExp[76]= 672000
			addedExp[77]= 682000
			addedExp[78]= 690000
			addedExp[79]= 701000
			addedExp[80]= 710000
			addedExp[81]= 719000
			addedExp[82]= 731000
			addedExp[83]= 740000
			addedExp[84]= 750000
			addedExp[85]= 761000
			addedExp[86]= 773000
			addedExp[87]= 783000
			addedExp[88]= 796000
			addedExp[89]= 807000
			addedExp[90]= 819000
			addedExp[91]= 831000
			addedExp[92]= 843000
			addedExp[93]= 856000
			addedExp[94]= 868000
			addedExp[95]= 881000
			addedExp[96]= 893000
			addedExp[97]= 906000
			addedExp[98]= 920000
			addedExp[99]= 934000
			addedExp[100]= 947000

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
	local quest_money = {}
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
	role.SetRoleScriptData(RoleID, 1, RoleDataType["mojieVIP"], tonumber(os.date("%j")))
end
aux.RegisterQuestEvent(30333, 1, "q30333_OnComplete")
aux.RegisterQuestEvent(30333, 7, "q30333_OnQuestInit")

function q30333_CheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	local CurTime = tonumber(os.date("%j"))
	local LastTime = role.GetRoleScriptData(RoleID, 1, RoleDataType["IsToday"])
	if CurTime ~= LastTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["IsToday"], CurTime)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["FengZhiShiZhe"], 0)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["MoJieZhiHua"], role.GetRoleScriptData(RoleID, 1, RoleDataType["MoJieZhiHua"])%1000)
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["InstantKillCre"], role.GetRoleScriptData(RoleID, 1, RoleDataType["InstantKillCre"])%100)
	end

	--local rolelevel = role.GetRoleQuestNum(RoleID)

	if role.GetRoleScriptData(RoleID, 1, RoleDataType["MoJieZhiHua"]) >= 3000 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 444001)			--您每天只能完成3次魔界之花任务
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	else
		return 1
	end
end

aux.RegisterQuestEvent(30333, 4, "q30333_CheckAccept")


function MoJieZhiHua_OnIalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then
	    if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) == 0 and cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2) == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444002)	-- 使用一个魔界花粉可以使这棵魔界花树开始成长。确定要这样做吗？
			msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
			msg.AddMsgEvent(MsgID, 1, 100001)	-- "好的，我知道了。"
			msg.AddMsgEvent(MsgID, 21, 20)	-- 选项一
			msg.AddMsgEvent(MsgID, 1, 100002)	-- "好的，我知道了。"
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444003)	-- 击败魔界之花周围的护花使魔可以使魔界之花继续成长。\n魔界之花成长度：xx/50
			msg.AddMsgEvent(MsgID, 9, cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0))
			msg.AddMsgEvent(MsgID, 21, 20)	-- 选项一
			msg.AddMsgEvent(MsgID, 1, 100001)	-- "好的，我知道了。"
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 4 then
        local mapcrc = {3424071720,3424072488,3424072232,3424075048}
		if role.GetRoleItemNum(RoleID,4400601) < 1 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444004)	-- 你没有魔界花粉，魔界之花没有发生任何的变化。
			msg.AddMsgEvent(MsgID, 21, 20)	-- 选项一
			msg.AddMsgEvent(MsgID, 1, 100001)	-- "好的，我知道了。"
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
		    local index = role.GetRoleScriptData(RoleID, 1, RoleDataType["MoJieZhiHua"])%1000
			local bool = true
			if index >= 80 and (MapID == mapcrc[1] or MapID == mapcrc[2] or MapID == mapcrc[3]) then
			    bool = false
			elseif index >= 65 and (MapID == mapcrc[1] or MapID == mapcrc[2]) then
			    bool = false
			elseif index >= 50 and MapID == mapcrc[1] then
			    bool = false
			end
			if not bool then
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444005)	-- 你无法在低等级场景中培育魔界之花。
				msg.AddMsgEvent(MsgID, 9, cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0))
				msg.AddMsgEvent(MsgID, 21, 20)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 100001)	-- "好的，我知道了。"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 1,TargetTypeID)
				cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 2, 1)
			    role.RemoveFromRole(MapID, InstanceID, RoleID, 4400601, 1, 101)
				--cre.SetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0, 1)
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444006)	-- 魔界之花开始生长，护花使魔出现！
				msg.AddMsgEvent(MsgID, 9, cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0))
				msg.AddMsgEvent(MsgID, 21, 20)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 100001)	-- "好的，我知道了。"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
				unit.AddBuff(MapID, InstanceID, TargetID, 6101401, TargetID)
				MoJieZhiHua_info[TargetID] = {MapID, InstanceID}
			    local x, y, z = unit.GetPosition(MapID, InstanceID, TargetID)
				local MonsterID = TargetTypeID - 5610027 + 1099025
				local cid = map.MapCreateColCreature(MapID, InstanceID, MonsterID, x, y, z, 1, "")
				cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 0, TargetID)
				if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) >= 20 then
				    unit.AddBuff(MapID, InstanceID, cid, 6101401, cid)
				elseif cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) >= 40 then
				    unit.AddBuff(MapID, InstanceID, cid, 6101401, cid)
					unit.AddBuff(MapID, InstanceID, cid, 6101401, cid)
				end
				local cid = map.MapCreateColCreature(MapID, InstanceID, MonsterID, x-2, y, z, 1, "")
				cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 0, TargetID)
				if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) >= 20 then
				    unit.AddBuff(MapID, InstanceID, cid, 6101401, cid)
				elseif cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) >= 40 then
				    unit.AddBuff(MapID, InstanceID, cid, 6101401, cid)
					unit.AddBuff(MapID, InstanceID, cid, 6101401, cid)
				end
				local cid = map.MapCreateColCreature(MapID, InstanceID, MonsterID, x, y, z-2, 1, "")
				cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 0, TargetID)
				if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) >= 20 then
				    unit.AddBuff(MapID, InstanceID, cid, 6101401, cid)
				elseif cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) >= 40 then
				    unit.AddBuff(MapID, InstanceID, cid, 6101401, cid)
					unit.AddBuff(MapID, InstanceID, cid, 6101401, cid)
				end
				local cid = map.MapCreateColCreature(MapID, InstanceID, MonsterID, x+2, y, z, 1, "")
				cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 0, TargetID)
				if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) >= 20 then
				    unit.AddBuff(MapID, InstanceID, cid, 6101401, cid)
				elseif cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) >= 40 then
				    unit.AddBuff(MapID, InstanceID, cid, 6101401, cid)
					unit.AddBuff(MapID, InstanceID, cid, 6101401, cid)
				end
				local cid = map.MapCreateColCreature(MapID, InstanceID, MonsterID, x, y, z+2, 1, "")
				cre.SetCreatureScriptData(MapID, InstanceID, cid, 1, 0, TargetID)
				if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) >= 20 then
				    unit.AddBuff(MapID, InstanceID, cid, 6101401, cid)
				elseif cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) >= 40 then
				    unit.AddBuff(MapID, InstanceID, cid, 6101401, cid)
					unit.AddBuff(MapID, InstanceID, cid, 6101401, cid)
				end
			end
		end
	end
end
aux.RegisterCreatureEvent(5610027, 7, "MoJieZhiHua_OnIalk")
aux.RegisterCreatureEvent(5610028, 7, "MoJieZhiHua_OnIalk")
aux.RegisterCreatureEvent(5610029, 7, "MoJieZhiHua_OnIalk")
aux.RegisterCreatureEvent(5610030, 7, "MoJieZhiHua_OnIalk")
aux.RegisterCreatureEvent(5610031, 7, "MoJieZhiHua_OnIalk")
aux.RegisterCreatureEvent(5610032, 7, "MoJieZhiHua_OnIalk")
aux.RegisterCreatureEvent(5610033, 7, "MoJieZhiHua_OnIalk")

function HuHuaShiMo_OnDie(MapID, InstanceID, TargetID, TypeID, KillerID)
    local k = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)
	if MoJieZhiHua_info[k] ~= nil then
	    local p = cre.GetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 1, 0)
	    cre.SetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 1, 0, p+1)
		unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 6101501, k)
		if (p+1)%5 == 0 and p ~= 0 then
		    local ID = map.MapCreateColCreature(MapID, InstanceID, 5610044, 1, 1, 1, 1, "")
			cre.SetCreatureScriptData(MapID, InstanceID, ID, 1, 0, k)
		end
		if p+1 == 50 then
		    local x, y, z = unit.GetPosition(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k)
			map.MapCreateColCreature(MapID, InstanceID, TypeID+4511009, x, y, z, 1, "")
		   -- map.MapDeleteCreature(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k)
			cre.SetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 1, 0, 0)
			cre.SetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 1, 1, 0)
			cre.SetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 1, 2, 0)
			unit.ChangeHP(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, -1000000)
			MoJieZhiHua_info[k] = nil
		end
	end
end
aux.RegisterCreatureEvent(1099025, 4, "HuHuaShiMo_OnDie")
aux.RegisterCreatureEvent(1099026, 4, "HuHuaShiMo_OnDie")
aux.RegisterCreatureEvent(1099027, 4, "HuHuaShiMo_OnDie")
aux.RegisterCreatureEvent(1099028, 4, "HuHuaShiMo_OnDie")
aux.RegisterCreatureEvent(1099029, 4, "HuHuaShiMo_OnDie")
aux.RegisterCreatureEvent(1099030, 4, "HuHuaShiMo_OnDie")
aux.RegisterCreatureEvent(1099031, 4, "HuHuaShiMo_OnDie")

function MoJieZhiHua_OnDie(MapID, InstanceID, TargetID, TypeID, KillerID)
    MoJieZhiHua_info[TargetID] = nil
end
aux.RegisterCreatureEvent(5610027, 4, "MoJieZhiHua_OnDie")
aux.RegisterCreatureEvent(5610028, 4, "MoJieZhiHua_OnDie")
aux.RegisterCreatureEvent(5610029, 4, "MoJieZhiHua_OnDie")
aux.RegisterCreatureEvent(5610030, 4, "MoJieZhiHua_OnDie")
aux.RegisterCreatureEvent(5610031, 4, "MoJieZhiHua_OnDie")
aux.RegisterCreatureEvent(5610032, 4, "MoJieZhiHua_OnDie")
aux.RegisterCreatureEvent(5610033, 4, "MoJieZhiHua_OnDie")
aux.RegisterCreatureEvent(5610027, 13, "MoJieZhiHua_OnDie")
aux.RegisterCreatureEvent(5610028, 13, "MoJieZhiHua_OnDie")
aux.RegisterCreatureEvent(5610029, 13, "MoJieZhiHua_OnDie")
aux.RegisterCreatureEvent(5610030, 13, "MoJieZhiHua_OnDie")
aux.RegisterCreatureEvent(5610031, 13, "MoJieZhiHua_OnDie")
aux.RegisterCreatureEvent(5610032, 13, "MoJieZhiHua_OnDie")
aux.RegisterCreatureEvent(5610033, 13, "MoJieZhiHua_OnDie")

function ChengShuMoHua_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    MoJieZhiHua_Player[TargetID] = nil
end

aux.RegisterCreatureEvent(5610034, 4, "ChengShuMoHua_OnDisappear")
aux.RegisterCreatureEvent(5610035, 4, "ChengShuMoHua_OnDisappear")
aux.RegisterCreatureEvent(5610036, 4, "ChengShuMoHua_OnDisappear")
aux.RegisterCreatureEvent(5610037, 4, "ChengShuMoHua_OnDisappear")
aux.RegisterCreatureEvent(5610038, 4, "ChengShuMoHua_OnDisappear")
aux.RegisterCreatureEvent(5610039, 4, "ChengShuMoHua_OnDisappear")
aux.RegisterCreatureEvent(5610040, 4, "ChengShuMoHua_OnDisappear")
aux.RegisterCreatureEvent(5610034, 13, "ChengShuMoHua_OnDisappear")
aux.RegisterCreatureEvent(5610035, 13, "ChengShuMoHua_OnDisappear")
aux.RegisterCreatureEvent(5610036, 13, "ChengShuMoHua_OnDisappear")
aux.RegisterCreatureEvent(5610037, 13, "ChengShuMoHua_OnDisappear")
aux.RegisterCreatureEvent(5610038, 13, "ChengShuMoHua_OnDisappear")
aux.RegisterCreatureEvent(5610039, 13, "ChengShuMoHua_OnDisappear")
aux.RegisterCreatureEvent(5610040, 13, "ChengShuMoHua_OnDisappear")

function MoHua10S_OnDisappear(MapID, InstanceID, TargetID, TargetTypeID)
    local k = cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0)
	if MoJieZhiHua_info[k] ~= nil then
		local x, y, z = unit.GetPosition(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k)
		TargetTypeID = cre.GetCreatureScriptData(MapID, InstanceID, k, 1, 1)
		local MonsterID = TargetTypeID - 5610027 + 1099025
		local cid = map.MapCreateColCreature(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], MonsterID, x, y, z, 1, "")
		cre.SetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 1, 0, k)
		if cre.GetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 1, 0) >= 20 then
			unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 6101401, cid)
		elseif cre.GetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 1, 0) >= 40 then
			unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 6101401, cid)
			unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 6101401, cid)
		end
		local cid = map.MapCreateColCreature(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], MonsterID, x-2, y, z, 1, "")
		cre.SetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 1, 0, k)
		if cre.GetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 1, 0) >= 20 then
			unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 6101401, cid)
		elseif cre.GetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 1, 0) >= 40 then
			unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 6101401, cid)
			unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 6101401, cid)
		end
		local cid = map.MapCreateColCreature(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], MonsterID, x, y, z-2, 1, "")
		cre.SetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 1, 0, k)
		if cre.GetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 1, 0) >= 20 then
			unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 6101401, cid)
		elseif cre.GetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 1, 0) >= 40 then
			unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 6101401, cid)
			unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 6101401, cid)
		end
		local cid = map.MapCreateColCreature(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], MonsterID, x+2, y, z, 1, "")
		cre.SetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 1, 0, k)
		if cre.GetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 1, 0) >= 20 then
			unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 6101401, cid)
		elseif cre.GetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 1, 0) >= 40 then
			unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 6101401, cid)
			unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 6101401, cid)
		end
		local cid = map.MapCreateColCreature(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], MonsterID, x, y, z+2, 1, "")
		cre.SetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 1, 0, k)
		if cre.GetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 1, 0) >= 20 then
			unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 6101401, cid)
		elseif cre.GetCreatureScriptData(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], k, 1, 0) >= 40 then
			unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 6101401, cid)
			unit.AddBuff(MoJieZhiHua_info[k][1], MoJieZhiHua_info[k][2], cid, 6101401, cid)
		end
	end
end
aux.RegisterCreatureEvent(5610044, 13, "MoHua10S_OnDisappear")

function ChengShuMoHua_OnIalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

    if MoJieZhiHua_Player[TargetID] == nil then
	    MoJieZhiHua_Player[TargetID] = {Player={},Num=0}
	end

	if TalkIndex == -1 then
	    if cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0) == 0 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444007)	-- 魔界之花已经成熟，想要采摘魔界之果吗？
			msg.AddMsgEvent(MsgID, 21, 4)	-- 选项一
			msg.AddMsgEvent(MsgID, 1, 100001)	-- "好的，我知道了。"
			msg.AddMsgEvent(MsgID, 21, 20)	-- 选项一
			msg.AddMsgEvent(MsgID, 1, 100002)	-- "好的，我知道了。"
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	elseif TalkIndex == 4 then
	    if not role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30333) then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 444008)	-- 只有接受了魔界之果任务的人才能从魔界之花上采摘魔界之果。
			msg.AddMsgEvent(MsgID, 9, cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0))
			msg.AddMsgEvent(MsgID, 21, 20)	-- 选项一
			msg.AddMsgEvent(MsgID, 1, 100001)	-- "好的，我知道了。"
			msg.AddMsgEvent(MsgID, 24, TargetID)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local mapcrc = {3424071720,3424072488,3424072232,3424075048}
			local index = role.GetRoleScriptData(RoleID, 1, RoleDataType["MoJieZhiHua"])%1000
			local bool = true
			if index >= 80 and (MapID == mapcrc[1] or MapID == mapcrc[2] or MapID == mapcrc[3]) then
				bool = false
			elseif index >= 65 and (MapID == mapcrc[1] or MapID == mapcrc[2]) then
				bool = false
			elseif index >= 50 and MapID == mapcrc[1] then
				bool = false
			end
			if not bool then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 444009)	-- 你无法在低等级场景中采集魔界之花
				msg.AddMsgEvent(MsgID, 9, cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0))
				msg.AddMsgEvent(MsgID, 21, 20)	-- 选项一
				msg.AddMsgEvent(MsgID, 1, 100001)	-- "好的，我知道了。"
				msg.AddMsgEvent(MsgID, 24, TargetID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
			    local bool1 = false
				for k,v in pairs(MoJieZhiHua_Player[TargetID].Player) do
				    if v == RoleID then
						bool1 = true
					end
				end
				if bool1 then
				    local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 444010)	-- 每个人只能在同一棵魔界之花上采集一次魔界之果
					msg.AddMsgEvent(MsgID, 9, cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0))
					msg.AddMsgEvent(MsgID, 21, 20)	-- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- "好的，我知道了。"
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				elseif role.GetRoleItemNum(RoleID,4400602) >= 3 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 444011)	-- 每个人只能携带3个魔界之果，你已经无法继续采摘了。
					msg.AddMsgEvent(MsgID, 9, cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0))
					msg.AddMsgEvent(MsgID, 21, 20)	-- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- "好的，我知道了。"
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				elseif role.GetBagFreeSize(RoleID) < 1 and role.GetRoleItemNum(RoleID,4400602) < 1 then
				    local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 444012)	-- 你的背包中已经没有空间携带魔界之果了，请您在清理后再次对魔界之花进行采摘。
					msg.AddMsgEvent(MsgID, 9, cre.GetCreatureScriptData(MapID, InstanceID, TargetID, 1, 0))
					msg.AddMsgEvent(MsgID, 21, 20)	-- 选项一
					msg.AddMsgEvent(MsgID, 1, 100001)	-- "好的，我知道了。"
					msg.AddMsgEvent(MsgID, 24, TargetID)
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				else
				    role.AddRoleItem(MapID, InstanceID, RoleID, 4400602, 1, -1, 8, 420)
				    if TargetTypeID >= 5610037 and role.GetBagFreeSize(RoleID) > 0 and math.random(1000) > 994 then
						role.AddRoleItem(MapID, InstanceID, RoleID, 4400003+math.random(6)*3, 1, -1, 8, 420)
						--4400006	1 	4400009	1 	4400012 1 	4400015 1 	4400018 1 	4400021 1 低级果实
                        --4400007	1 	4400010	1 	4400013	1	4400016	1	4400019	1	4400022	1 高级果实
                    end
					table.insert(MoJieZhiHua_Player[TargetID].Player,RoleID)
					MoJieZhiHua_Player[TargetID].Num = MoJieZhiHua_Player[TargetID].Num + 1
					if MoJieZhiHua_Player[TargetID].Num > 9 then
					    map.MapDeleteCreature(MapID, InstanceID, TargetID)
					    MoJieZhiHua_Player[TargetID] = nil
					end
				end
		    end
		end
	end
end

aux.RegisterCreatureEvent(5610034, 7, "ChengShuMoHua_OnIalk")
aux.RegisterCreatureEvent(5610035, 7, "ChengShuMoHua_OnIalk")
aux.RegisterCreatureEvent(5610036, 7, "ChengShuMoHua_OnIalk")
aux.RegisterCreatureEvent(5610037, 7, "ChengShuMoHua_OnIalk")
aux.RegisterCreatureEvent(5610038, 7, "ChengShuMoHua_OnIalk")
aux.RegisterCreatureEvent(5610039, 7, "ChengShuMoHua_OnIalk")
aux.RegisterCreatureEvent(5610040, 7, "ChengShuMoHua_OnIalk")
