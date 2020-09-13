
--用于标记玩家在本次活动中是否已完成过任务的全局table，以玩家RoleID为索引
ZhanShenLegend_Quest_IsComplete = {}
--记录每次活动中任务被完成的总次数
ZhanShenLegend_CompleteQuestTime = 0
--各个城市被刷出的活动NPC“天界战神”的CreatureID
ZhanShen_CreatureID = {}

--设置每个被刷出的NPC的信息
ZhanShen_CreatureID[1] = {MapID=3424073512,x=505,y=23167,z=304,CreatureID=0}

ZhanShenLegend_TimerMin = 0		--记录活动进行了多长时间

function ZhanShenLegend_OnStart(actID)	--活动开始
	local TempTable = {}
	ZhanShenLegend_Quest_IsComplete = deepcopy(TempTable)

	--初始化活动全局变量
	ZhanShenLegend_TimerMin = 0
	ZhanShenLegend_CompleteQuestTime = 0

	--刷出活动NPC 天界战神[5510402]
	ZhanShen_CreatureID[1].CreatureID = map.MapCreateColCreature(ZhanShen_CreatureID[1].MapID, -1, 5510402, ZhanShen_CreatureID[1].x, ZhanShen_CreatureID[1].y, ZhanShen_CreatureID[1].z,1)

	--活动开始播放广播：
	--“本次战神传说活动开始，各位玩家可在斗战天城[坐标，坐标]找“天界战神”接取活动任务，完成后会有丰厚的奖励！”
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,102,100015)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end

function ZhanShenLegend_OnTimerMin(actID)	--活动每经过1分钟
	ZhanShenLegend_TimerMin = ZhanShenLegend_TimerMin+1
	if ZhanShen_CreatureID[1].CreatureID == 0 then
		ZhanShen_CreatureID[1].CreatureID = map.MapCreateCreature(ZhanShen_CreatureID[1].MapID, -1, 5510402, ZhanShen_CreatureID[1].x, ZhanShen_CreatureID[1].y, ZhanShen_CreatureID[1].z)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100015)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end
--战神传说活动正在进行中，所有10级以上的玩家均可在斗战天城[坐标，坐标]的“天界战神”处接取活动任务，赚取丰厚奖励！
	if ZhanShenLegend_TimerMin==5 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100016)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif ZhanShenLegend_TimerMin==10 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100016)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif ZhanShenLegend_TimerMin==50 then
	--活动进行到50分钟时播放广播：
	--“天界战神”将在10分钟后返回天庭，需要交付任务的玩家请尽快前往，以免因错过时间而占用下次活动的任务名额。
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100028)
		msg.AddMsgEvent(MsgID,5,5510402)
		msg.AddMsgEvent(MsgID,9,10)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	elseif ZhanShenLegend_TimerMin==55 then
	--活动进行到55分钟时
	--播放广播：
	--“天界战神”将在5分钟后返回天庭，需要交付任务的玩家请尽快前往，以免因错过时间而占用下次活动的任务名额。
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID,102,100028)
		msg.AddMsgEvent(MsgID,5,5510402)
		msg.AddMsgEvent(MsgID,9,5)
		msg.DispatchBroadcast(MsgID,-1,-1,-1)
	end
end

function ZhanShenLegend_OnEnd(actID)	--活动结束

	--判断活动NPC是否已达到生存时间自动消失（若未消失则在场景中删除该NPC），并释放NPC的CreatureID
	if ZhanShen_CreatureID[1].CreatureID ~= nil and unit.IsDead(ZhanShen_CreatureID[1].MapID,-1,ZhanShen_CreatureID[1].CreatureID)==false then
		map.MapDeleteCreature(ZhanShen_CreatureID[1].MapID,-1,ZhanShen_CreatureID[1].CreatureID)
		ZhanShen_CreatureID[1].CreatureID = nil
	else
		ZhanShen_CreatureID[1].CreatureID = nil
	end

	ZhanShenLegend_Quest_IsComplete = {}
	ZhanShenLegend_CompleteQuestTime = 0
	ZhanShenLegend_TimerMin = 0
	--活动结束播放广播：
	--“本次战神传说活动已经结束，尚未完成任务的玩家可继续进行任务，并在下次活动时交付任务！”
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,102,100017)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)
end



local Quest_30202 = {}
Quest_30202[1]={TargetType=QuestTargetType["Kill"],TargetID1=1020004,Num1=10,TargetID2=1020005,Num2=10,TargetID3=1020014,Num3=3,RewMoney=6975,RewXP=7020,RewItemID1=4080067,value1=1,MsgID=12001,}--
Quest_30202[2]={TargetType=QuestTargetType["Kill"],TargetID1=1030002,Num1=20,TargetID2=1030003,Num2=20,TargetID3=1030014,Num3=3,RewMoney=15375,RewXP=20700,RewItemID1=4080067,value1=1,MsgID=12002,}--
Quest_30202[3]={TargetType=QuestTargetType["Kill"],TargetID1=1030007,Num1=20,TargetID2=1030006,Num2=20,TargetID3=1030017,Num3=3,RewMoney=17000,RewXP=25200,RewItemID1=4080067,value1=1,MsgID=12003,}--
Quest_30202[4]={TargetType=QuestTargetType["Kill"],TargetID1=1040001,Num1=30,TargetID2=1040003,Num2=30,TargetID3=1040033,Num3=5,RewMoney=29800,RewXP=39900,RewItemID1=4080067,value1=1,MsgID=12004,}--
Quest_30202[5]={TargetType=QuestTargetType["Kill"],TargetID1=1040012,Num1=30,TargetID2=1040021,Num2=30,TargetID3=1040035,Num3=5,RewMoney=32400,RewXP=50400,RewItemID1=4080067,value1=1,MsgID=12005,}--
Quest_30202[6]={TargetType=QuestTargetType["Kill"],TargetID1=1040024,Num1=40,TargetID2=1040025,Num2=40,TargetID3=1040036,Num3=5,RewMoney=43500,RewXP=75600,RewItemID1=4080067,value1=1,MsgID=12006,}--
Quest_30202[7]={TargetType=QuestTargetType["Kill"],TargetID1=1040026,Num1=40,TargetID2=1040027,Num2=40,TargetID3=1040038,Num3=5,RewMoney=46500,RewXP=90300,RewItemID1=4080067,value1=1,MsgID=12007,}--
Quest_30202[8]={TargetType=QuestTargetType["Kill"],TargetID1=1050001,Num1=40,TargetID2=1050003,Num2=40,TargetID3=1050027,Num3=5,RewMoney=49250,RewXP=198450,RewItemID1=4080067,value1=1,MsgID=12008,}--
Quest_30202[9]={TargetType=QuestTargetType["Kill"],TargetID1=1050010,Num1=40,TargetID2=1050012,Num2=40,TargetID3=1050029,Num3=5,RewMoney=52000,RewXP=231525,RewItemID1=4080067,value1=1,MsgID=12009,}--
Quest_30202[10]={TargetType=QuestTargetType["Kill"],TargetID1=1050017,Num1=60,TargetID2=1050018,Num2=60,TargetID3=1050034,Num3=10,RewMoney=82125,RewXP=269325,RewItemID1=4080067,value1=1,MsgID=12010,}--
Quest_30202[11]={TargetType=QuestTargetType["Kill"],TargetID1=1060008,Num1=60,TargetID2=1060007,Num2=60,TargetID3=1060034,Num3=10,RewMoney=85875,RewXP=349650,RewItemID1=4080067,value1=1,MsgID=12011,}--
Quest_30202[12]={TargetType=QuestTargetType["Kill"],TargetID1=1060016,Num1=25,TargetID2=1060017,Num2=25,TargetID3=1060040,Num3=15,RewMoney=108000,RewXP=481950,RewItemID1=4080067,value1=1,MsgID=12012,}--
Quest_30202[13]={TargetType=QuestTargetType["Kill"],TargetID1=1060023,Num1=70,TargetID2=1060028,Num2=70,TargetID3=1060044,Num3=15,RewMoney=112500,RewXP=633150,RewItemID1=4080067,value1=1,MsgID=12013,}--
Quest_30202[14]={TargetType=QuestTargetType["Kill"],TargetID1=1070002,Num1=80,TargetID2=1070009,Num2=80,TargetID3=1070053,Num3=15,RewMoney=130000,RewXP=819000,RewItemID1=4080067,value1=1,MsgID=12014,}--
Quest_30202[15]={TargetType=QuestTargetType["Kill"],TargetID1=1070015,Num1=80,TargetID2=1070016,Num2=80,TargetID3=1070056,Num3=15,RewMoney=135000,RewXP=1015875,RewItemID1=4080067,value1=1,MsgID=12015,}--
Quest_30202[16]={TargetType=QuestTargetType["Kill"],TargetID1=1070028,Num1=20,TargetID2=1070029,Num2=20,TargetID3=1070050,Num3=20,RewMoney=174375,RewXP=1239525,RewItemID1=4080067,value1=1,MsgID=12016,}--
Quest_30202[17]={TargetType=QuestTargetType["Kill"],TargetID1=1070035,Num1=100,TargetID2=1070042,Num2=100,TargetID3=1070069,Num3=15,RewMoney=180000,RewXP=1489950,RewItemID1=4080067,value1=1,MsgID=12017,}--


function ZhanShenLegend_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)	--玩家完成任务
	ZhanShenLegend_Quest_IsComplete[RoleID] = 1
	ZhanShenLegend_CompleteQuestTime = ZhanShenLegend_CompleteQuestTime + 1

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
	if ZhanShenLegend_CompleteQuestTime==1 then	--玩家第一个完成任务，额外给予任务的1倍经验和金钱奖励
		role.AddRoleExp(MapID, InstanceID, RoleID, Quest_30202[index].RewXP)
		role.AddRoleSilver(MapID, InstanceID, RoleID, Quest_30202[index].RewMoney, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201001)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100018)	--恭喜玩家“<p1>”第一个完成本次战神传说活动任务，获得双倍经验与金钱奖励！
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewXP*2)
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewMoney*2)
	elseif ZhanShenLegend_CompleteQuestTime==10 then	--玩家第10个完成任务，额外给予任务的0.5倍经验和金钱奖励
		local ex = math.floor(Quest_30202[index].RewXP/2)
		local go = math.floor(Quest_30202[index].RewMoney/2)
		role.AddRoleExp(MapID, InstanceID, RoleID, ex)
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201002)--恭喜您在本次活动中第&lt;p1&gt;个完成任务，额外获得1.5倍经验奖励！
		msg.AddMsgEvent(MsgID_1, 9, 10)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100019)	--玩家“<p1>”在本次魔神的咆哮活动中第十个完成了活动任务，获得1.5倍经验与金钱奖励！
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewXP+ex)
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewMoney+go)
	elseif ZhanShenLegend_CompleteQuestTime==100 then	--玩家第100个完成任务，额外给予任务的0.5倍经验和金钱奖励
		local ex = math.floor(Quest_30202[index].RewXP/2)
		local go = math.floor(Quest_30202[index].RewMoney/2)
		role.AddRoleExp(MapID, InstanceID, RoleID, ex)
		role.AddRoleSilver(MapID, InstanceID, RoleID, go, 102)
		local MsgID_1 = msg.BeginMsgEvent()
		local MsgID_2 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 13, 0)
		msg.AddMsgEvent(MsgID_1, 1, 201002)--恭喜您在本次活动中第&lt;p1&gt;个完成任务，额外获得1.5倍经验奖励！
		msg.AddMsgEvent(MsgID_1, 9, 100)
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		msg.AddMsgEvent(MsgID_2, 102, 100020)
		msg.AddMsgEvent(MsgID_2, 2, RoleID)
		msg.DispatchBroadcast(MsgID_2,-1,-1,-1)
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewXP+ex)
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewMoney+go)
	else
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewXP)
		msg.AddMsgEvent(MsgID, 9, Quest_30202[index].RewMoney)
	end
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

	if role.GetTrialState(RoleID, 1008) == 1 then
		role.SetTrialCompleteNum(RoleID, 1008, role.GetTrialCompleteNum(RoleID,1008) + 1)
    end
	if role.GetTrialState(RoleID, 1008) == 1 and role.GetTrialCompleteNum(RoleID,1008) >= Trail_maxnum[1008] then
		role.TrialComplete(RoleID,1008)
	end

	role.NotifyFBBS(RoleID,15,0)
	--战神次数
	UpdateSeverData(49, 1)
	--玩家计数
	UpdateRoleData(RoleID, 12, 1)
end

--接取任务--
function q30202_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)

	local index = 0
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	rolelevel = math.floor(rolelevel/5)
	if rolelevel <= 3 or rolelevel==nil then index = 1
	elseif rolelevel >= 20 then index = 17
	else index = rolelevel -2
	end
	--初始化任务的动态数据
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_30202, index))
	--同步客户端总任务环数和任务说明
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 1, Quest_30202[index].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)
end
aux.RegisterQuestEvent(30202, 7, "q30202_OnQuestInit")



function ZhanShenLegend_OnCheckAccept(MapID, InstanceID, QuestID, RoleID, NPCID)
	if ZhanShenLegend_Quest_IsComplete[RoleID] ~= nil then
		local MsgID_1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 26, 201005)--本次任务已经完成，不可再接
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		return 0
	end
	return 1
end

function ZhanShenLegend_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

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

	if ZhanShenLegend_Quest_IsComplete[RoleID] ~= nil then
		local MsgID_1 = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID_1, 26, 201003)--每次活动期间只能完成一次任务！
		msg.DispatchRoleMsgEvent(RoleID, MsgID_1)
		return 0
	end

	return 1
end

--[[________________________注册活动相关事件________________________]]--

--活动开始
aux.RegisterActEvent(5, 2, "ZhanShenLegend_OnStart")
aux.RegisterActEvent(6, 2, "ZhanShenLegend_OnStart")
aux.RegisterActEvent(7, 2, "ZhanShenLegend_OnStart")
aux.RegisterActEvent(8, 2, "ZhanShenLegend_OnStart")

--活动结束
aux.RegisterActEvent(5, 3, "ZhanShenLegend_OnEnd")
aux.RegisterActEvent(6, 3, "ZhanShenLegend_OnEnd")
aux.RegisterActEvent(7, 3, "ZhanShenLegend_OnEnd")
aux.RegisterActEvent(8, 3, "ZhanShenLegend_OnEnd")

--活动以“分钟”为单位计时
aux.RegisterActEvent(5, 4, "ZhanShenLegend_OnTimerMin")
aux.RegisterActEvent(6, 4, "ZhanShenLegend_OnTimerMin")
aux.RegisterActEvent(7, 4, "ZhanShenLegend_OnTimerMin")
aux.RegisterActEvent(8, 4, "ZhanShenLegend_OnTimerMin")

--玩家完成“战神传说”任务
aux.RegisterQuestEvent(30202, 1, "ZhanShenLegend_OnComplete")

aux.RegisterQuestEvent(30202, 4, "ZhanShenLegend_OnCheckAccept")

--检测玩家是否可接取“战神传说”任务
aux.RegisterQuestEvent(30202, 5, "ZhanShenLegend_OnCheckComplete")
