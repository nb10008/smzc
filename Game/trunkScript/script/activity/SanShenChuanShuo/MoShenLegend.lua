local Quest_30201 = {}
Quest_30201[1]={TargetType=QuestTargetType["Kill"],TargetID1=1020016,Num1=1,RewMoney=6277,RewXP=7020,RewItemID1=4080067,value1=1,MsgID=11001,}--
Quest_30201[2]={TargetType=QuestTargetType["Kill"],TargetID1=1030018,Num1=1,RewMoney=13837,RewXP=20700,RewItemID1=4080067,value1=1,MsgID=11002,}--
Quest_30201[3]={TargetType=QuestTargetType["Kill"],TargetID1=1030019,Num1=1,RewMoney=15300,RewXP=25200,RewItemID1=4080067,value1=1,MsgID=11003,}--
Quest_30201[4]={TargetType=QuestTargetType["Kill"],TargetID1=1040043,Num1=1,TargetID2=1040044,Num2=1,RewMoney=26820,RewXP=39900,RewItemID1=4080067,value1=1,MsgID=11004,}--
Quest_30201[5]={TargetType=QuestTargetType["Kill"],TargetID1=1040046,Num1=1,TargetID2=1040044,Num2=1,RewMoney=29160,RewXP=50400,RewItemID1=4080067,value1=1,MsgID=11005,}--
Quest_30201[6]={TargetType=QuestTargetType["Kill"],TargetID1=1040047,Num1=1,TargetID2=1040048,Num2=1,RewMoney=39150,RewXP=75600,RewItemID1=4080067,value1=1,MsgID=11006,}--
Quest_30201[7]={TargetType=QuestTargetType["Kill"],TargetID1=1040045,Num1=1,TargetID2=1040049,Num2=1,RewMoney=41850,RewXP=90300,RewItemID1=4080067,value1=1,MsgID=11007,}--
Quest_30201[8]={TargetType=QuestTargetType["Kill"],TargetID1=1050037,Num1=1,TargetID2=1050038,Num2=1,RewMoney=44325,RewXP=198450,RewItemID1=4080067,value1=1,MsgID=11008,}--
Quest_30201[9]={TargetType=QuestTargetType["Kill"],TargetID1=1050038,Num1=1,TargetID2=1050041,Num2=1,RewMoney=46800,RewXP=231525,RewItemID1=4080067,value1=1,MsgID=11009,}--
Quest_30201[10]={TargetType=QuestTargetType["Kill"],TargetID1=1050041,Num1=1,TargetID2=1050042,Num2=1,RewMoney=73912,RewXP=269325,RewItemID1=4080067,value1=1,MsgID=11010,}--
Quest_30201[11]={TargetType=QuestTargetType["Kill"],TargetID1=1060048,Num1=1,TargetID2=1060047,Num2=1,TargetID3=1060049,Num3=1,RewMoney=77287,RewXP=349650,RewItemID1=4080067,value1=1,MsgID=11011,}--
Quest_30201[12]={TargetType=QuestTargetType["Kill"],TargetID1=1060051,Num1=1,TargetID2=1060052,Num2=1,TargetID3=1060053,Num3=1,RewMoney=97200,RewXP=481950,RewItemID1=4080067,value1=1,MsgID=11012,}--
Quest_30201[13]={TargetType=QuestTargetType["Kill"],TargetID1=1060054,Num1=1,TargetID2=1060055,Num2=1,RewMoney=101250,RewXP=633150,RewItemID1=4080067,value1=1,MsgID=11013,}--
Quest_30201[14]={TargetType=QuestTargetType["Kill"],TargetID1=1070073,Num1=1,TargetID2=1070074,Num2=1,TargetID3=1070075,Num3=1,RewMoney=117000,RewXP=819000,RewItemID1=4080067,value1=1,MsgID=11014,}--
Quest_30201[15]={TargetType=QuestTargetType["Kill"],TargetID1=1070076,Num1=1,TargetID2=1070077,Num2=1,TargetID3=1070078,Num3=1,RewMoney=121500,RewXP=1015875,RewItemID1=4080067,value1=1,MsgID=11015,}--
Quest_30201[16]={TargetType=QuestTargetType["Kill"],TargetID1=1070079,Num1=1,TargetID2=1070080,Num2=1,RewMoney=156937,RewXP=1239525,RewItemID1=4080067,value1=1,MsgID=11016,}--
Quest_30201[17]={TargetType=QuestTargetType["Kill"],TargetID1=1070082,Num1=1,TargetID2=1070085,Num2=1,RewMoney=162000,RewXP=1489950,RewItemID1=4080067,value1=1,MsgID=11017,}--


--用于标记玩家在本次活动中是否已完成过任务的全局table，以玩家RoleID为索引，对应记录不同玩家
MoShenLegend_Quest_IsComplete = {}
--记录每次活动中任务被完成的总次数
MoShenLegend_CompleteQuestTime = 0
--各个城市被刷出的活动NPC“猎魔使者”的CreatureID
MoShen_CreatureID = {}
--设置每个被刷出的NPC的信息
MoShen_CreatureID[1] = {MapID=3424073512,x=505,y=23167,z=304,CreatureID=0}
MoShenLegend_TimerMin = 0		--记录活动进行了多长时间

function MoShenLegend_OnStart(actID)	--活动开始
	local TempTable = {}
	MoShenLegend_Quest_IsComplete = deepcopy(TempTable)
	MoShenLegend_TimerMin = 0
	MoShenLegend_CompleteQuestTime = 0

	--刷出活动NPC 猎魔使者[5510403]
	MoShen_CreatureID[1].CreatureID = map.MapCreateCreature(MoShen_CreatureID[1].MapID, -1, 5510403, MoShen_CreatureID[1].x, MoShen_CreatureID[1].y, MoShen_CreatureID[1].z)

	--活动开始播放广播：
	--“本次魔神的咆哮活动开始，各位玩家可在斗战天城[坐标，坐标]找“猎魔使者”接取活动任务，完成后会有丰厚的奖励！”
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,102,100021)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end

function MoShenLegend_OnTimerMin(actID)	--活动每经过1分钟

	MoShenLegend_TimerMin = MoShenLegend_TimerMin + 1
	if MoShen_CreatureID[1].CreatureID == 0 then
		MoShen_CreatureID[1].CreatureID = map.MapCreateCreature(MoShen_CreatureID[1].MapID, -1, 5510403, MoShen_CreatureID[1].x, MoShen_CreatureID[1].y, MoShen_CreatureID[1].z)
		--魔神的咆哮活动正在进行中，所有10级以上的玩家均可在斗战天城[坐标，坐标]的“猎魔使者”处接取活动任务，赚取丰厚奖励！
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100022)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end
	if MoShenLegend_TimerMin==5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100022)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)

	elseif MoShenLegend_TimerMin==10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100022)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif MoShenLegend_TimerMin==50 then
	--活动进行到50分钟时播放广播：
	--“猎魔使者”将在10分钟后返回天庭，需要交付任务的玩家请尽快前往，以免因错过时间而占用下次活动的任务名额。
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100028)
		msg.AddMsgEvent(MsgID,5,5510403)
		msg.AddMsgEvent(MsgID,9,10)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif MoShenLegend_TimerMin==55 then
	--活动进行到55分钟时播放广播：
	--“猎魔使者”将在5分钟后返回天庭，需要交付任务的玩家请尽快前往，以免因错过时间而占用下次活动的任务名额。
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100028)
		msg.AddMsgEvent(MsgID,5,5510403)
		msg.AddMsgEvent(MsgID,9,5)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end
end



function MoShenLegend_OnEnd(actID)	--活动结束
	--判断活动NPC是否已达到生存时间自动消失（若未消失则在场景中删除该NPC），并释放NPC的CreatureID
	if MoShen_CreatureID[1].CreatureID ~= nil and unit.IsDead(MoShen_CreatureID[1].MapID,-1,MoShen_CreatureID[1].CreatureID)==false then
		map.MapDeleteCreature(MoShen_CreatureID[1].MapID,-1,MoShen_CreatureID[1].CreatureID)
		MoShen_CreatureID[1].CreatureID = nil
	else
		MoShen_CreatureID[1].CreatureID = nil
	end

	MoShenLegend_Quest_IsComplete = {}
	MoShenLegend_CompleteQuestTime = 0
	MoShenLegend_TimerMin = 0
	--活动结束播放广播：
	--“本次魔神的咆哮活动已经结束，尚未完成任务的玩家可继续进行任务，并在下次活动时交付任务！”
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,102,100023)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end


--接取任务--
function q30201_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)

	local index = 0
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	rolelevel = math.floor(rolelevel/5)
	if rolelevel <= 3 or rolelevel==nil then index = 1
	elseif rolelevel >= 20 then index = 17
	else index = rolelevel -2
	end
	--初始化任务的动态数据
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_30201, index))
	--同步客户端总任务环数和任务说明
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 1, Quest_30201[index].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
aux.RegisterQuestEvent(30201, 7, "q30201_OnQuestInit")


function MoShenLegend_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)	--玩家完成任务
	MoShenLegend_CompleteQuestTime = MoShenLegend_CompleteQuestTime + 1
	MoShenLegend_Quest_IsComplete[RoleID] = 1
	local index = 0
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)

	if rolelevel >= 80 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4820081, 25, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 3500011, 1, -1, 8, 420)
	elseif rolelevel >= 60 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4820081, 22, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, RoleID, 3500011, 1, -1, 8, 420)
	elseif rolelevel >= 40 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 4820081, 20, -1, 8, 420)
	else
	    role.AddRoleItem(MapID, InstanceID, RoleID, 4820081, 5, -1, 8, 420)
    end

	rolelevel = math.floor(rolelevel/5)
	if rolelevel <= 3 or rolelevel==nil then index = 1
	elseif rolelevel >= 20 then index = 17
	else index = rolelevel -2
	end

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 201004)--恭喜您完成<p1>任务，总计获得经验<p2>点及金钱<p3>银"
	msg.AddMsgEvent(MsgID, 11, QuestID)
	if MoShenLegend_CompleteQuestTime ==1 then	--玩家第一个完成任务，额外给予任务的1倍经验和金钱奖励
		role.AddRoleExp(MapID, InstanceID, RoleID, Quest_30201[index].RewXP)
		role.AddRoleSilver(MapID, InstanceID, RoleID, Quest_30201[index].RewMoney, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201001)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100024)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewXP*2)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewMoney*2)
	elseif MoShenLegend_CompleteQuestTime==10 then	--玩家第10个完成任务，额外给予任务的0.5倍经验和金钱奖励
		local ex = math.floor(Quest_30201[index].RewXP/2)
		local go = math.floor(Quest_30201[index].RewMoney/2)
		role.AddRoleExp(MapID, InstanceID, RoleID, ex)
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201002)--恭喜您在本次活动中第&lt;p1&gt;个完成任务，额外获得1.5倍经验奖励！
		msg.AddMsgEvent(MsgID_1, 9, 10)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100025)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewXP+ex)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewMoney+go)
	elseif MoShenLegend_CompleteQuestTime==50 then	--玩家第50个完成任务，额外给予任务的0.5倍经验和金钱奖励
		local ex = math.floor(Quest_30201[index].RewXP/2)
		local go = math.floor(Quest_30201[index].RewMoney/2)
		role.AddRoleExp(MapID, InstanceID, RoleID, ex)
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201002)--恭喜您在本次活动中第&lt;p1&gt;个完成任务，额外获得1.5倍经验奖励！
		msg.AddMsgEvent(MsgID_1, 9, 50)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100027)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewXP+ex)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewMoney+go)
	elseif MoShenLegend_CompleteQuestTime==100 then	--玩家第100个完成任务，额外给予任务的0.5倍经验和金钱奖励
		local ex = math.floor(Quest_30201[index].RewXP/2)
		local go = math.floor(Quest_30201[index].RewMoney/2)
		role.AddRoleExp(MapID, InstanceID, RoleID, ex)
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201002)--恭喜您在本次活动中第&lt;p1&gt;个完成任务，额外获得1.5倍经验奖励！
		msg.AddMsgEvent(MsgID_1, 9, 100)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100026)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewXP+ex)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewMoney+go)
	else
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewXP)
		msg.AddMsgEvent(MsgID, 9, Quest_30201[index].RewMoney)
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

	if role.GetTrialState(RoleID, 1010) == 1 then
		role.SetTrialCompleteNum(RoleID, 1010, role.GetTrialCompleteNum(RoleID,1010) + 1)
    end
	if role.GetTrialState(RoleID, 1010) == 1 and role.GetTrialCompleteNum(RoleID,1010) >= Trail_maxnum[1010] then
		role.TrialComplete(RoleID,1010)
	end

	role.NotifyFBBS(RoleID,14,0)
	--魔神次数
	UpdateSeverData(49, 1)
	--玩家计数
	UpdateRoleData(RoleID, 12, 1)
end



function MoShenLegend_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel >= 50 then
		--判断背包空闲空间是否足够
		local FreeSize = role.GetBagFreeSize(RoleID)
		if(FreeSize < 2) then
			--提示玩家背包空间不足
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)--需要至少x格背包空间才可继续操作
			msg.AddMsgEvent(MsgID, 9, 2)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	elseif rolelevel >= 40 then
		--判断背包空闲空间是否足够
		local FreeSize = role.GetBagFreeSize(RoleID)
		if(FreeSize < 1) then
			--提示玩家背包空间不足
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)--需要至少x格背包空间才可继续操作
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	elseif rolelevel >= 30 then
	    --判断背包空闲空间是否足够
		local FreeSize = role.GetBagFreeSize(RoleID)
		if(FreeSize < 1) then
			--提示玩家背包空间不足
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)--需要至少x格背包空间才可继续操作
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	end

	if MoShenLegend_Quest_IsComplete[RoleID] ~= nil then
		local MsgID_1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 26, 201003)--每次活动期间只能完成一次任务！
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		return 0
	end
	return 1
end

function MoShenLegend_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	if MoShenLegend_Quest_IsComplete[RoleID] ~= nil then
		local MsgID_1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 26, 201005)--本次任务已经完成，不可再接
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		return 0
	end
	return 1
end

--[[________________________注册活动相关事件________________________]]--

--活动开始
aux.RegisterActEvent(9, 2, "MoShenLegend_OnStart")
aux.RegisterActEvent(10, 2, "MoShenLegend_OnStart")
aux.RegisterActEvent(11, 2, "MoShenLegend_OnStart")
aux.RegisterActEvent(12, 2, "MoShenLegend_OnStart")
--活动结束
aux.RegisterActEvent(9, 3, "MoShenLegend_OnEnd")
aux.RegisterActEvent(10, 3, "MoShenLegend_OnEnd")
aux.RegisterActEvent(11, 3, "MoShenLegend_OnEnd")
aux.RegisterActEvent(12, 3, "MoShenLegend_OnEnd")

--活动以“分钟”为单位计时
aux.RegisterActEvent(9, 4, "MoShenLegend_OnTimerMin")
aux.RegisterActEvent(10, 4, "MoShenLegend_OnTimerMin")
aux.RegisterActEvent(11, 4, "MoShenLegend_OnTimerMin")
aux.RegisterActEvent(12, 4, "MoShenLegend_OnTimerMin")



--玩家完成“魔神的咆哮”任务
aux.RegisterQuestEvent(30201, 1, "MoShenLegend_OnComplete")

--检测玩家是否可接取“魔神的咆哮”任务
aux.RegisterQuestEvent(30201, 4, "MoShenLegend_OnCheckAccept")

--检测玩家是否可接取“魔神的咆哮”任务
aux.RegisterQuestEvent(30201, 5, "MoShenLegend_OnCheckComplete")

