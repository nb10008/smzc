
--用于标记玩家在本次活动中是否已完成过任务的全局table，以玩家RoleID为索引
NvShenLegend_Quest_IsComplete = {}
--记录每次活动中任务被完成的总次数
NvShenLegend_CompleteQuestTime = 0
--被刷出的活动NPC“女神的侍从”的CreatureID
NvShen_CreatureID = {}

--每个被刷出的NPC的信息505, 23167, 304
NvShen_CreatureID[1] = {MapID=3424073512,x=505,y=23167,z=304,CreatureID=0}--斗战天城:[505, 23167, 304

NvShenLegend_TimerMin = 0	--记录活动进行了多长时间

function NvShenLegend_OnStart(actID)	--活动开始
	local TempTable = {}
	NvShenLegend_Quest_IsComplete = deepcopy(TempTable)
	--初始化活动全局变量
	NvShenLegend_TimerMin = 0
	NvShenLegend_CompleteQuestTime = 0

	--刷出活动NPC 女神的侍从[5510401]
	NvShen_CreatureID[1].CreatureID = map.MapCreateCreature(NvShen_CreatureID[1].MapID, -1, 5510401, NvShen_CreatureID[1].x, NvShen_CreatureID[1].y, NvShen_CreatureID[1].z)

	--活动开始播放广播：
	--“本次女神的英姿活动开始，各位玩家可在斗战天城[坐标，坐标]内找到“女神的侍从”接取活动任务，完成后会有丰厚的奖励！”
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,102,100009)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end

function NvShenLegend_OnTimerMin(actID)	--活动每经过1分钟

	NvShenLegend_TimerMin = NvShenLegend_TimerMin+1


	if NvShen_CreatureID[1].CreatureID == 0 then
		NvShen_CreatureID[1].CreatureID = map.MapCreateCreature(NvShen_CreatureID[1].MapID, -1, 5510401, NvShen_CreatureID[1].x, NvShen_CreatureID[1].y, NvShen_CreatureID[1].z)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100009)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end
	--“女神的英姿活动正在进行中，所有等级达到10级的玩家均可在斗战天城[505，304]的“女神的侍从”处接取活动任务，赚取丰厚奖励！”
	if NvShenLegend_TimerMin==5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100010)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)

	elseif NvShenLegend_TimerMin==10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100010)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif NvShenLegend_TimerMin==50 then
	--活动进行到50分钟时播放广播：
	--女神的侍从将在10分钟后返回天庭，需要交付任务者请尽快前往，以免因错过时间而占用下次活动的任务名额。
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100028)
		msg.AddMsgEvent(MsgID,5,5510401)
		msg.AddMsgEvent(MsgID,9,10)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif NvShenLegend_TimerMin==55 then
	--活动进行到55分钟时播放广播：
	--“女神的侍从”将在5分钟后返回天庭，需要交付任务的玩家请尽快前往，以免因错过时间而占用下次活动的任务名额。
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100028)
		msg.AddMsgEvent(MsgID,5,5510401)
		msg.AddMsgEvent(MsgID,9,5)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)

	end
end



function NvShenLegend_OnEnd(actID)	--活动结束

	--判断活动NPC是否已达到生存时间自动消失（若未消失则在场景中删除该NPC），并释放NPC的CreatureID
	if NvShen_CreatureID[1].CreatureID ~= nil and unit.IsDead(NvShen_CreatureID[1].MapID,-1,NvShen_CreatureID[1].CreatureID)==false then
		map.MapDeleteCreature(NvShen_CreatureID[1].MapID,-1,NvShen_CreatureID[1].CreatureID)
		NvShen_CreatureID[1].CreatureID = nil
	else
		NvShen_CreatureID[1].CreatureID = nil
	end

	NvShenLegend_Quest_IsComplete = {}
	NvShenLegend_CompleteQuestTime = 0
	NvShenLegend_TimerMin = 0
	--活动结束播放广播：
	--“本次封神传说活动已经结束，尚未完成任务的玩家可继续进行任务，并在下次活动时交付任务！”
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,102,100011)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end




function NvShenLegend_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)	--玩家完成任务

	NvShenLegend_Quest_IsComplete[RoleID] = 1--记录玩家在本次活动中已完成过任务
	NvShenLegend_CompleteQuestTime = NvShenLegend_CompleteQuestTime+1
	local NvShen_Reward = {}
	NvShen_Reward[30203] = {Exp=3510,Gold=1395}
	NvShen_Reward[30204] = {Exp=10350,Gold=3075}
	NvShen_Reward[30205] = {Exp=12600,Gold=3400}
	NvShen_Reward[30206] = {Exp=24480,Gold=5960}
	NvShen_Reward[30207] = {Exp=32880,Gold=6480}
	NvShen_Reward[30208] = {Exp=52800,Gold=8700}
	NvShen_Reward[30209] = {Exp=66300,Gold=9300}
	NvShen_Reward[30210] = {Exp=121500,Gold=9850}
	NvShen_Reward[30211] = {Exp=146250,Gold=10400}
	NvShen_Reward[30212] = {Exp=259200,Gold=16425}
	NvShen_Reward[30213] = {Exp=303075,Gold=17175}
	NvShen_Reward[30214] = {Exp=419580,Gold=21600}
	NvShen_Reward[30215] = {Exp=480330,Gold=22500}
	NvShen_Reward[30216] = {Exp=604800,Gold=26000}
	NvShen_Reward[30217] = {Exp=681300,Gold=27000}
	NvShen_Reward[30218] = {Exp=951750,Gold=34875}
	NvShen_Reward[30219] = {Exp=1058625,Gold=36000}

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 201004)--恭喜您完成<p1>任务，总计获得经验<p2>点及金钱<p3>银"
	msg.AddMsgEvent(MsgID, 11, QuestID)

	if NvShenLegend_CompleteQuestTime==1 then	--玩家第一个完成任务，额外给予任务的1倍经验和金钱奖励
		role.AddRoleExp(MapID, InstanceID, RoleID, NvShen_Reward[QuestID].Exp)
		role.AddRoleSilver(MapID, InstanceID, RoleID, NvShen_Reward[QuestID].Gold, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201001)--”恭喜您在本次活动中第一个完成任务，额外获得2倍经验奖励！”
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100012)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, NvShen_Reward[QuestID].Exp*2)
		msg.AddMsgEvent(MsgID, 9, NvShen_Reward[QuestID].Gold*2)
	elseif NvShenLegend_CompleteQuestTime==10 then	--玩家第10个完成任务，额外给予任务的0.5倍经验和金钱奖励
		local ex = math.floor(NvShen_Reward[QuestID].Exp/2)
		local go = math.floor(NvShen_Reward[QuestID].Gold/2)
		role.AddRoleExp(MapID, InstanceID, RoleID, ex)
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201002)--恭喜您在本次活动中第&lt;p1&gt;个完成任务，额外获得1.5倍经验奖励！
		msg.AddMsgEvent(MsgID_1, 9, 10)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100013)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, ShiShen_Reward[QuestID].Exp+ex)
		msg.AddMsgEvent(MsgID, 9, ShiShen_Reward[QuestID].Gold+go)
	elseif NvShenLegend_CompleteQuestTime==100 then	--玩家第100个完成任务，额外给予任务的0.5倍经验和金钱奖励
		local ex = math.floor(NvShen_Reward[QuestID].Exp/2)
		local go = math.floor(NvShen_Reward[QuestID].Gold/2)
		role.AddRoleExp(MapID, InstanceID, RoleID, ex)
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201002)--恭喜您在本次活动中第&lt;p1&gt;个完成任务，额外获得1.5倍经验奖励！
		msg.AddMsgEvent(MsgID_1, 9, 100)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100014)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, NvShen_Reward[QuestID].Exp+ex)
		msg.AddMsgEvent(MsgID, 9, NvShen_Reward[QuestID].Gold+go)
	else
		msg.AddMsgEvent(MsgID, 9, NvShen_Reward[QuestID].Exp)
		msg.AddMsgEvent(MsgID, 9, NvShen_Reward[QuestID].Gold)
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

	if role.GetTrialState(RoleID, 1009) == 1 then
		role.SetTrialCompleteNum(RoleID, 1009, role.GetTrialCompleteNum(RoleID,1009) + 1)
    end
	if role.GetTrialState(RoleID, 1009) == 1 and role.GetTrialCompleteNum(RoleID,1009) >= Trail_maxnum[1009] then
		role.TrialComplete(RoleID,1009)
	end
	--女神次数
	UpdateSeverData(49, 1)
	--玩家计数
	UpdateRoleData(RoleID, 12, 1)
end

function NvShenLegend_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
    local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	--判断背包空闲空间是否足够
	--[[if rolelevel >= 50 then
		local FreeSize = role.GetBagFreeSize(RoleID)
		if(FreeSize < 1) then
			--提示玩家背包空间不足
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 100003)--需要至少x格背包空间才可继续操作
			msg.AddMsgEvent(MsgID, 9, 1)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
			return 0
		end
	end]]
	--local FreeSize = role.GetBagFreeSize(RoleID)
	--[[if(FreeSize < 3) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)--需要至少x格背包空间才可继续操作
		msg.AddMsgEvent(MsgID, 9, 3)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		return 0
	end]]
	if NvShenLegend_Quest_IsComplete[RoleID] ~= nil then
		local MsgID_1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 26, 201003)--每次活动期间只能完成一次任务！
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		return 0
	end
	return 1
end


function NvShenLegend_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	if NvShenLegend_Quest_IsComplete[RoleID] ~= nil then
		local MsgID_1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 26, 201005)--本次任务已经完成，不可再接
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		return 0
	end
	for i = 30203,30219 do
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, i) == true then
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_2, 26, 100007)--您已经接取了任务
		msg.DispatchRoleMsgEvent(RoleID, MsgID_2)
		return 0
		end
	end
	return 1
end


--[[________________________注册活动相关事件________________________]]--

--活动开始
aux.RegisterActEvent(1, 2, "NvShenLegend_OnStart")
aux.RegisterActEvent(2, 2, "NvShenLegend_OnStart")
aux.RegisterActEvent(3, 2, "NvShenLegend_OnStart")
aux.RegisterActEvent(4, 2, "NvShenLegend_OnStart")

--活动结束
aux.RegisterActEvent(1, 3, "NvShenLegend_OnEnd")
aux.RegisterActEvent(2, 3, "NvShenLegend_OnEnd")
aux.RegisterActEvent(3, 3, "NvShenLegend_OnEnd")
aux.RegisterActEvent(4, 3, "NvShenLegend_OnEnd")
--活动以“分钟”为单位计时
aux.RegisterActEvent(1, 4, "NvShenLegend_OnTimerMin")
aux.RegisterActEvent(2, 4, "NvShenLegend_OnTimerMin")
aux.RegisterActEvent(3, 4, "NvShenLegend_OnTimerMin")
aux.RegisterActEvent(4, 4, "NvShenLegend_OnTimerMin")

--玩家完成“封神传说”任务
aux.RegisterQuestEvent(30203, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30204, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30205, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30206, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30207, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30208, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30209, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30210, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30211, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30212, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30213, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30214, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30215, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30216, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30217, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30218, 1, "NvShenLegend_OnComplete")
aux.RegisterQuestEvent(30219, 1, "NvShenLegend_OnComplete")

--检测玩家是否可接取“封神传说”任务
aux.RegisterQuestEvent(30203, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30204, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30205, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30206, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30207, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30208, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30209, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30210, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30211, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30212, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30213, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30214, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30215, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30216, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30217, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30218, 4, "NvShenLegend_OnCheckAccept")
aux.RegisterQuestEvent(30219, 4, "NvShenLegend_OnCheckAccept")


--检测玩家是否可完成“封神传说”任务
aux.RegisterQuestEvent(30203, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30204, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30205, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30206, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30207, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30208, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30209, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30210, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30211, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30212, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30213, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30214, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30215, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30216, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30217, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30218, 5, "NvShenLegend_OnCheckComplete")
aux.RegisterQuestEvent(30219, 5, "NvShenLegend_OnCheckComplete")
