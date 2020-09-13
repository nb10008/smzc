--固定活动“承安之殿“

--地图ScriptData字段意义说明。0:杀死小怪的数量。1：杀死boss的数量。
--击杀Boss次数足够后刷出大BOSS

MoKu_TimerMin = 0
MoKu_Bossnum = 0
MoKu_s04_NPC_RandomQuantity = 15
MoKu_s05_NPC_RandomQuantity = 25



function MoKu_OnStart(actID)
	--重置TimerMin
	MoKu_TimerMin = 0
	MoKu_Bossnum = 0

	--勇闯魔窟活动已经开始，各位玩家可在奔雷裂谷和北洲天雪里找到魔窟之门，组队进入后击败里面的魔物，获得高额奖励
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,102,100033)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)

	local MoKu_s04_MonsterPos = deepcopy(map_s04)
	local MoKu_s05_MonsterPos = deepcopy(map_s05)

	local j = 30
	local m = 30

	--s04在30个坐标中随机刷出15个魔窟之门
	for i = 1,MoKu_s04_NPC_RandomQuantity do
		local Index_pos = math.random(1,j)
		local Temp1 = table.remove(MoKu_s04_MonsterPos,Index_pos)
		map.MapCreateColCreature(3424071976, -1, 5510501, Temp1.x, Temp1.y, Temp1.z, 1)
		j = j-1
	end
	--s05在30个坐标中随机刷出25个魔窟之门
	for k = 1,MoKu_s05_NPC_RandomQuantity do
		local Index_pos = math.random(1,m)
		local Temp1 = table.remove(MoKu_s05_MonsterPos,Index_pos)
		map.MapCreateColCreature(3424071720, -1, 5510501, Temp1.x, Temp1.y, Temp1.z, 1)
		m = m-1
	end

end


function MoKu_OnTimerMin(actID)

	local broadmin={}
	broadmin[1] = 14
	broadmin[2] = 29
	broadmin[3] = 44

	local respawnmin={}
	respawnmin[1] = 15
	respawnmin[2] = 30
	respawnmin[3] = 45


	MoKu_TimerMin = MoKu_TimerMin+1

	for i=1,3 do
		if MoKu_TimerMin == broadmin[i] then
			--勇闯魔窟活动已经开始，各位玩家可在奔雷裂谷和北洲天雪里找到魔窟之门，组队进入后击败里面的魔物，获得高额奖励
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID,102,100034)
			msg.DispatchBroadcast(MsgID,-1,-1,-1)
		end
	end

	for i=1,3 do
		if MoKu_TimerMin == respawnmin[i] then
			local MoKu_s04_MonsterPos = deepcopy(map_s04)
			local MoKu_s05_MonsterPos = deepcopy(map_s05)

			local j = 30
			local m = 30

			--s04在30个坐标中随机刷出15个魔窟之门
			for a = 1,MoKu_s04_NPC_RandomQuantity do
				local Index_pos = math.random(1,j)
				local Temp1 = table.remove(MoKu_s04_MonsterPos,Index_pos)
				map.MapCreateColCreature(3424071976, -1, 5510501, Temp1.x, Temp1.y, Temp1.z, 1)
				j = j-1
			end
			--s05在30个坐标中随机刷出25个魔窟之门
			for b = 1,MoKu_s05_NPC_RandomQuantity do
				local Index_pos = math.random(1,m)
				local Temp1 = table.remove(MoKu_s05_MonsterPos,Index_pos)
				map.MapCreateColCreature(3424071720, -1, 5510501, Temp1.x, Temp1.y, Temp1.z, 1)
				m = m-1
			end
		end
	end
end

--注册该活动的相关事件
aux.RegisterActEvent(13, 2, "MoKu_OnStart")
aux.RegisterActEvent(14, 2, "MoKu_OnStart")
aux.RegisterActEvent(15, 2, "MoKu_OnStart")
aux.RegisterActEvent(16, 2, "MoKu_OnStart")

aux.RegisterActEvent(13, 4, "MoKu_OnTimerMin")
aux.RegisterActEvent(14, 4, "MoKu_OnTimerMin")
aux.RegisterActEvent(15, 4, "MoKu_OnTimerMin")
aux.RegisterActEvent(16, 4, "MoKu_OnTimerMin")

--与魔窟之门对话传送进副本

function x5510501_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)

	--判断玩家是否可以进入副本
	if TeamID == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 	100004)	--您没有组队，不能进入该副本
		msg.AddMsgEvent(MsgID, 21, 	5)		--取消按钮
		msg.AddMsgEvent(MsgID, 1, 	100002)	--取消
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	else
		if GetTeamPlayerNum(MapID,InstanceID,RoleID) < 3 then -- 得到玩家队伍人数[common_function.lua]
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 	202001)	--队伍中至少要有3人才能进入。
			msg.AddMsgEvent(MsgID, 21, 	5)		--取消按钮
			msg.AddMsgEvent(MsgID, 1, 	100002)	--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			--获取队伍玩家等级
			local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
			if rolelevel < 30 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 	202002)	--“您等级不足30，无法进入该副本”
				msg.AddMsgEvent(MsgID, 21, 	5)		--取消按钮
				msg.AddMsgEvent(MsgID, 1, 	100002)	--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				if TalkIndex == -1 then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20,	100005)		--“是否进入该副本？”
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.AddMsgEvent(MsgID, 21,	4)			--确定按钮
					msg.AddMsgEvent(MsgID, 1,	100001)		--“确定”
					msg.AddMsgEvent(MsgID, 21,	5)			--取消按钮
					msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				elseif TalkIndex == 4 and rolelevel >= 30 then
					role.RoleGotoNewMap(MapID, InstanceID, RoleID, 619033530, 170, 0, 178)
					map.MapDeleteCreature(MapID, InstanceID, TargetID)
				end
			end
		end
	end
end
--注册魔窟之门对话事件
aux.RegisterCreatureEvent(5510501, 7, "x5510501_OnTalk")

--清完小怪刷出BOSS
function MoKu_ZaKu_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	--获得副本已杀小怪数量
	local CreatureNum = map.GetMapScriptData(MapID, InstanceID, 1, 0) + 1
	map.SetMapScriptData(MapID, InstanceID, 1, 0, CreatureNum)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 26, 202003)--”已击杀魔窟喽啰&lt;p1&gt;名，击杀30个后魔窟首领即会现身！“
	msg.AddMsgEvent(MsgID, 9, CreatureNum)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID)

	if CreatureNum == 30 then
		local a = map.GetMapScriptData(MapID, InstanceID, 1, 2)
		map.MapCreateCreature(MapID, InstanceID, 2053001 + (a-30)/5, 42, 288, 128)
	end
end



aux.RegisterCreatureEvent(2051001, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051002, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051003, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051004, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051005, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051006, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051007, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051008, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051009, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051010, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051011, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051012, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051013, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051014, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051015, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051016, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051017, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051018, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051019, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051020, 4, "MoKu_ZaKu_OnDie")
--[[aux.RegisterCreatureEvent(2051021, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051022, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051023, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051024, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051025, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051026, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051027, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051028, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051029, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051030, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051031, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051032, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051033, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051034, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051035, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051036, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051037, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051038, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051039, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051040, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051041, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051042, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051043, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051044, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051045, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051046, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051047, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051048, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051049, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051050, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051051, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051052, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051053, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051054, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051055, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051056, 4, "MoKu_ZaKu_OnDie")
]]--
aux.RegisterCreatureEvent(2051057, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051058, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051059, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051060, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051061, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051062, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051063, 4, "MoKu_ZaKu_OnDie")
aux.RegisterCreatureEvent(2051064, 4, "MoKu_ZaKu_OnDie")

																-----------------------------------------------------------------标记线-------------------------------------------

function h01_OnPlayerEnter(MapID, InstanceID, RoleID)

	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end

	if map.GetMapScriptData(MapID, InstanceID, 1, 1) ~= 1 then
		map.SetMapScriptData(MapID, InstanceID, 1, 1, 1)--已经有人进入了副本
		local montypeid = MoKu_level(MapID, InstanceID, RoleID)--随机出的怪物等级
		map.SetMapScriptData(MapID, InstanceID, 1, 2, montypeid)--记录随机任务目标等级,即魔窟首领等级
		montypeid = 2051000 + ((montypeid-30)/5)*4

		local MoKu_pos = {}
		MoKu_pos[1] = {x=210,y=2000,z=139}
		MoKu_pos[2] = {x=203,y=2000,z=140}
		MoKu_pos[3] = {x=208,y=2000,z=129}
		MoKu_pos[4] = {x=194,y=2000,z=130}
		MoKu_pos[5] = {x=195,y=2000,z=119}
		MoKu_pos[6] = {x=180,y=2000,z=90}
		MoKu_pos[7] = {x=185,y=2000,z=75}
		MoKu_pos[8] = {x=180,y=2000,z=77}
		MoKu_pos[9] = {x=178,y=2000,z=67}
		MoKu_pos[10] = {x=166,y=2000,z=86}
		MoKu_pos[11] = {x=160,y=2000,z=86}
		MoKu_pos[12] = {x=161,y=2000,z=80}
		MoKu_pos[13] = {x=157,y=2000,z=63}
		MoKu_pos[14] = {x=169,y=2000,z=76}
		MoKu_pos[15] = {x=150,y=2000,z=35}
		MoKu_pos[16] = {x=143,y=2000,z=31}
		MoKu_pos[17] = {x=133,y=2000,z=41}
		MoKu_pos[18] = {x=146,y=2000,z=48}
		MoKu_pos[19] = {x=140,y=2000,z=24}
		MoKu_pos[20] = {x=123,y=2000,z=31}
		MoKu_pos[21] = {x=129,y=5700,z=23}
		MoKu_pos[22] = {x=121,y=7000,z=20}
		MoKu_pos[23] = {x=65,y=11050,z=57}
		MoKu_pos[24] = {x=102,y=6000,z=29}
		MoKu_pos[25] = {x=97,y=6200,z=37}
		MoKu_pos[26] = {x=85,y=8250,z=31}
		MoKu_pos[27] = {x=74,y=9200,z=49}
		MoKu_pos[28] = {x=73,y=9350,z=59}
		MoKu_pos[29] = {x=61,y=10350,z=68}
		MoKu_pos[30] = {x=75,y=12350,z=78}
		MoKu_pos[31] = {x=210,y=2000,z=139}
		MoKu_pos[32] = {x=47,y=12700,z=135}------BOSS

		for k = 1,31 do
			local x = math.random(1,3)
			local y = math.random(100)
			if y > 96 then	x = 4 end

			local MonID = map.MapCreateColCreature(MapID, InstanceID, montypeid + x, MoKu_pos[k].x, MoKu_pos[k].y/50, MoKu_pos[k].z, 1)
		    if math.random(100) > 85 then
			    local k = math.random(3)
			    unit.AddBuff(MapID, InstanceID, MonID, 6100000+k, MonID)
			end
		end
	end
	--勇闯魔窟每日人数
	yongChuang_Roles = {}
	local curTime = tonumber(os.date("%j"))
	if yongChuang_Roles[RoleID] == nil then
		UpdateSeverData(48, 1)
		yongChuang_Roles[RoleID] = curTime
	elseif yongChuang_Roles[RoleID] ~= curTime then
		yongChuang_Roles = {}
		yongChuang_Roles[RoleID] = curTime
		UpdateSeverData(48, 1)
	end
end

--注册
aux.RegisterMapEvent("h01", 2, "h01_OnPlayerEnter")

--接取任务
function q30301_OnQuestInit(MapID, InstanceID, QuestID, RoleID, NPCID)


	local level = map.GetMapScriptData(MapID, InstanceID, 1, 2)
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel > 100 then
		rolelevel = 100
	end
	rolelevel = math.floor(rolelevel/5)*5

	local MoKu_reward = {}
	MoKu_reward[30] = {expe=27930,gold=8195}
	MoKu_reward[35] = {expe=35280,gold=8910}
	MoKu_reward[40] = {expe=52920,gold=11962}
	MoKu_reward[45] = {expe=63210,gold=12787}
	MoKu_reward[50] = {expe=139125,gold=13543}
	MoKu_reward[55] = {expe=162750,gold=14300}
	MoKu_reward[60] = {expe=188527,gold=22584}
	MoKu_reward[65] = {expe=244755,gold=23615}
	MoKu_reward[70] = {expe=337365,gold=29700}
	MoKu_reward[75] = {expe=443205,gold=30937}
	MoKu_reward[80] = {expe=573300,gold=35750}
	MoKu_reward[85] = {expe=711112,gold=37125}
	MoKu_reward[90] = {expe=867667,gold=47953}
	MoKu_reward[95] = {expe=1042965,gold=49500}
	MoKu_reward[100] = {expe=1042965,gold=80000}

	local bossid = 2053001 + (level-30)/5

	if level > rolelevel then --在人物等级和BOSS等级之间选取小的给经验
		level = rolelevel
	end

	local expe = MoKu_reward[level].expe
	local gold = MoKu_reward[level].gold
	local Quest_30301 = {}
	Quest_30301[1]={TargetType=QuestTargetType["Kill"], TargetID1=bossid, Num1=1,MsgID=14001,RewMoney=gold, RewXP=expe}

	--初始化任务的动态数据
	role.QuestInit(RoleID, QuestID, GetRandQuestTable(Quest_30301, 1))
	--同步客户端总任务环数和任务说明
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 11, QuestID)
	msg.AddMsgEvent(MsgID, 1, Quest_30301[1].MsgID)
	msg.DispatchRoleMsgEvent(RoleID, MsgID)

end
aux.RegisterQuestEvent(30301, 7, "q30301_OnQuestInit")

function q30301_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
    --传说装备相关
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel >= 75 then
        role.AddRoleItem(MapID, InstanceID, RoleID, 4820081, 4, -1, 8, 420)
	elseif rolelevel >= 55 then
		role.AddRoleItem(MapID, InstanceID, RoleID, 4820081, 3, -1, 8, 420)
	elseif rolelevel >= 40 then
	    role.AddRoleItem(MapID, InstanceID, RoleID, 4820081, 2, -1, 8, 420)
    end

	if role.GetTrialState(RoleID, 2015) == 1 then
		role.SetTrialCompleteNum(RoleID, 2015, role.GetTrialCompleteNum(RoleID,2015) + 1)
    end
	if role.GetTrialState(RoleID, 2015) == 1 and role.GetTrialCompleteNum(RoleID,2015) >= Trail_maxnum[2015] then
		role.TrialComplete(RoleID,2015)
	end

	role.NotifyFBBS(RoleID,13,0)
	--玩家计数
	UpdateRoleData(RoleID, 10, 1)
end

aux.RegisterQuestEvent(30301, 1, "q30301_OnComplete")

function MoKu_ZaKu_Boos_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	if TeamID ~= nil and TeamID ~= 4294967295 then
		local Role = {}
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
		for i=1, 6 do
			if Role[i] and Role[i]~= 4294967295 then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
				if Target_MapID ~= nil and Target_InstanceID ~= nil and Target_MapID == MapID and Target_InstanceID == InstanceID then
					for k=30601,30614 do
						if role.IsRoleHaveQuest(MapID, InstanceID, Role[i], k) == true then
							role.ModSpecialTargetValue(MapID, InstanceID, Role[i], k, 2)
						    role.SetRoleScriptData(Role[i], 1, RoleDataType["ActivityZhiYin"], math.floor(role.GetRoleScriptData(Role[i], 1, RoleDataType["ActivityZhiYin"]) / 10)*10+1)
						end
					end
					--账号回购
					local LiLianTable = {}
					LiLianTable[30805] = 17
					LiLianTable[30806] = 17
					LiLianTable[30807] = 15
					LiLianTable[30808] = 10
					LiLianTable[30809] = 5
					LiLianTable[30810] = 4
					LiLianTable[30811] = 2
					for k,v in pairs(LiLianTable) do
					if role.IsRoleHaveQuest(MapID, InstanceID, Role[i], k) == true then
						HuoDeLiLian(MapID, InstanceID, Role[i], v)
						LiLian_CheakComplete(MapID, InstanceID, Role[i])
						break
					end
				end
					--完事赶紧删了
				end
			end
		end
	else
		for k=30601,30614 do
			if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, k) == true then
				role.ModSpecialTargetValue(MapID, InstanceID, RoleID, k, 2)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"], math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"]) / 10)*10+1)
			end
		end
		--账号回购
		local LiLianTable = {}
		LiLianTable[30805] = 17
		LiLianTable[30806] = 17
		LiLianTable[30807] = 15
		LiLianTable[30808] = 10
		LiLianTable[30809] = 5
		LiLianTable[30810] = 4
		LiLianTable[30811] = 2
		for k,v in pairs(LiLianTable) do
			if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, k) == true then
				HuoDeLiLian(MapID, InstanceID, RoleID, v)
				LiLian_CheakComplete(MapID, InstanceID, RoleID)
				break
			end
		end
		--完事赶紧删了
	end
end
aux.RegisterCreatureEvent(2053001, 4, "MoKu_ZaKu_Boos_OnDie")
aux.RegisterCreatureEvent(2053002, 4, "MoKu_ZaKu_Boos_OnDie")
aux.RegisterCreatureEvent(2053003, 4, "MoKu_ZaKu_Boos_OnDie")
aux.RegisterCreatureEvent(2053004, 4, "MoKu_ZaKu_Boos_OnDie")
aux.RegisterCreatureEvent(2053005, 4, "MoKu_ZaKu_Boos_OnDie")
aux.RegisterCreatureEvent(2053006, 4, "MoKu_ZaKu_Boos_OnDie")
aux.RegisterCreatureEvent(2053007, 4, "MoKu_ZaKu_Boos_OnDie")
aux.RegisterCreatureEvent(2053008, 4, "MoKu_ZaKu_Boos_OnDie")
aux.RegisterCreatureEvent(2053009, 4, "MoKu_ZaKu_Boos_OnDie")
aux.RegisterCreatureEvent(2053010, 4, "MoKu_ZaKu_Boos_OnDie")
aux.RegisterCreatureEvent(2053011, 4, "MoKu_ZaKu_Boos_OnDie")
aux.RegisterCreatureEvent(2053012, 4, "MoKu_ZaKu_Boos_OnDie")
aux.RegisterCreatureEvent(2053013, 4, "MoKu_ZaKu_Boos_OnDie")
aux.RegisterCreatureEvent(2053014, 4, "MoKu_ZaKu_Boos_OnDie")
aux.RegisterCreatureEvent(2053015, 4, "MoKu_ZaKu_Boos_OnDie")
