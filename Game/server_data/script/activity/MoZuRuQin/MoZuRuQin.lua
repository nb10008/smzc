--固定活动“二十八星宿上隆”

--生成随机数种子
math.randomseed(aux.TimeGetTime())
--每个场景刷新的NPC数量
MoZu_NPC_RandomQuantity = 10

--活动开始事件
function MoZu_OnStart(actID)
	--初始化ruqinmowu
	--Init_ruqinmowu()
	--通知全服活动开始	[魔族怪物大举入侵！大家可到各地图寻找“入侵魔物”并将其剿灭！]
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID,102,100029)
	msg.DispatchBroadcast(MsgID,-1,-1,-1)

	local MoZu_s04_MonsterPos = deepcopy(map_s04)
	local MoZu_s05_MonsterPos = deepcopy(map_s05)
	local MoZu_s06_MonsterPos = deepcopy(map_s06)
	local MoZu_s07_MonsterPos = deepcopy(map_s07)
	--local MoZu_s08_MonsterPos = deepcopy(map_s08)
	local i = 0
	local j = 30
		--在30个坐标中随机刷出10个战神分身怪物，地点不重复，怪物类型可重复

	for i = 1,MoZu_NPC_RandomQuantity do
		local Index_pos = math.random(1,j)
		local Temp1 = table.remove(MoZu_s04_MonsterPos,Index_pos)
		local Temp2 = table.remove(MoZu_s05_MonsterPos,Index_pos)
		local Temp3 = table.remove(MoZu_s06_MonsterPos,Index_pos)
		local Temp4 = table.remove(MoZu_s07_MonsterPos,Index_pos)
		--local Temp5 = table.remove(MoZu_s08_MonsterPos,Index_pos)

		map.MapCreateColCreature(3424071976, -1, 5510101, Temp1.x, Temp1.y, Temp1.z, 1)
		map.MapCreateColCreature(3424071720, -1, 5510101, Temp2.x, Temp2.y, Temp2.z, 1)
		map.MapCreateColCreature(3424072488, -1, 5510101, Temp3.x, Temp3.y, Temp3.z, 1)
		map.MapCreateColCreature(3424072232, -1, 5510101, Temp4.x, Temp4.y, Temp4.z, 1)
		--map.MapCreateColCreature(3424075048, -1, 5510101, Temp5.x, Temp5.y, Temp5.z, 1)
		j = j-1
	end
end

--注册该活动的相关时间
aux.RegisterActEvent(17, 2, "MoZu_OnStart")
aux.RegisterActEvent(18, 2, "MoZu_OnStart")
aux.RegisterActEvent(19, 2, "MoZu_OnStart")
aux.RegisterActEvent(20, 2, "MoZu_OnStart")
aux.RegisterActEvent(21, 2, "MoZu_OnStart")



function mowu_OnTalk(MapID, InstanceID, NPCID, NPCTypeID, TargetID, Index)
		if Index == -1 then
		  local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 	203001)	---“对话后提示：“要挑战入侵魔物，队伍人数至少要达到3人。您是否已经做好准备？”
			msg.AddMsgEvent(MsgID, 24,	NPCID)	--npcid
		    msg.AddMsgEvent(MsgID, 21,	4)		--确定按钮
			msg.AddMsgEvent(MsgID, 1,100001)	--“确定”
			msg.AddMsgEvent(MsgID, 21,	5)		--取消按钮
			msg.AddMsgEvent(MsgID, 1,	100002)
			msg.DispatchRoleMsgEvent(TargetID, MsgID)
		end
		if Index == 4 then
			--限制刷出怪物，队伍人数必须达到3人
			if GetTeamPlayerNum(MapID,InstanceID,TargetID) < 3  then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71,203002)--提示队伍人数不足3人
				msg.DispatchRoleMsgEvent(TargetID,MsgID)
				return -1
			else
				local localA, localB ,localC = unit.GetPosition(MapID, InstanceID, TargetID)
				local rolelevel = role.GetRoleLevel(MapID, InstanceID, TargetID)
				level = math.floor(rolelevel/5)*5
				--奔雷裂谷s04
				if MapID == 3424071976 then
					if level <= 20 then level = 20
					elseif level >= 25 then level = 25 end
					local bossid = 2013000 + (level-20)/5*2 + math.random(1,2)
					local monsterid = 2012000 + (level-20)/5 + 1
					local MonID = {}
					MonID[1] = map.MapCreateCreature(3424071976, InstanceID, bossid, localA+2, localB, localC+12)
					MonID[2] = map.MapCreateCreature(3424071976, InstanceID, monsterid, localA+15, localB, localC+15)
					MonID[3] = map.MapCreateCreature(3424071976, InstanceID, monsterid, localA+5, localB, localC-15)
					MonID[4] = map.MapCreateCreature(3424071976, InstanceID, monsterid, localA-15, localB, localC+15)
					MonID[5] = map.MapCreateCreature(3424071976, InstanceID, monsterid, localA-5, localB, localC-15)
					MonID[6] = map.MapCreateCreature(3424071976, InstanceID, monsterid, localA+17, localB, localC+17)
					for i =1,6 do
					    if math.random(10) > 8 then
							local k = math.random(3)
							unit.AddBuff(MapID, InstanceID, MonID[i], 6100000+k, MonID[i])
						end
					end
				--北洲天雪s05
				elseif MapID == 3424071720 then
					if level <= 30 then level = 30
					elseif level >= 45 then level = 45 end
					local bossid = 2013000 + (level-20)/5*2 + math.random(1,2)
					local monsterid = 2012000 + (level-20)/5 + 1
					local MonID = {}
					MonID[1] = map.MapCreateCreature(3424071720, InstanceID, bossid, localA+2, localB, localC+12)
					MonID[2] = map.MapCreateCreature(3424071720, InstanceID, monsterid, localA+15, localB, localC+15)
					MonID[3] = map.MapCreateCreature(3424071720, InstanceID, monsterid, localA+5, localB, localC-15)
					MonID[4] = map.MapCreateCreature(3424071720, InstanceID, monsterid, localA-15, localB, localC+15)
					MonID[5] = map.MapCreateCreature(3424071720, InstanceID, monsterid, localA-5, localB, localC-15)
					MonID[6] = map.MapCreateCreature(3424071720, InstanceID, monsterid, localA+17, localB, localC+17)
					for i =1,6 do
					    if math.random(10) > 8 then
							local k = math.random(3)
							unit.AddBuff(MapID, InstanceID, MonID[i], 6100000+k, MonID[i])
						end
					end
				--西洲幻林s06
				elseif MapID == 3424072488 then
					if level <= 50 then level = 50
					elseif level >= 60 then level = 60 end
					local bossid = 2013000 + (level-20)/5*2 + math.random(1,2)
					local monsterid = 2012000 + (level-20)/5 + 1
					local MonID = {}
					MonID[1] = map.MapCreateCreature(3424072488, InstanceID, bossid, localA+2, localB, localC+12)
					MonID[2] = map.MapCreateCreature(3424072488, InstanceID, monsterid, localA+15, localB, localC+15)
					MonID[3] = map.MapCreateCreature(3424072488, InstanceID, monsterid, localA+5, localB, localC-15)
					MonID[4] = map.MapCreateCreature(3424072488, InstanceID, monsterid, localA-15, localB, localC+15)
					MonID[5] = map.MapCreateCreature(3424072488, InstanceID, monsterid, localA-5, localB, localC-15)
					MonID[6] = map.MapCreateCreature(3424072488, InstanceID, monsterid, localA+17, localB, localC+17)

					for i =1,6 do
					    if math.random(10) > 8 then
							local k = math.random(3)
							unit.AddBuff(MapID, InstanceID, MonID[i], 6100000+k, MonID[i])
						end
					end					--南洲迷沼s07
				elseif MapID == 3424072232 then
					if level <= 65 then level = 65
					elseif level >= 75 then level = 75 end
					local bossid = 2013000 + (level-20)/5*2 + math.random(1,2)
					local monsterid = 2012000 + (level-20)/5 + 1
					local MonID = {}
					MonID[1] = map.MapCreateCreature(3424072232, InstanceID, bossid, localA+2, localB, localC+12)
					MonID[2] = map.MapCreateCreature(3424072232, InstanceID, monsterid, localA+15, localB, localC+15)
					MonID[3] = map.MapCreateCreature(3424072232, InstanceID, monsterid, localA+5, localB, localC-15)
					MonID[4] = map.MapCreateCreature(3424072232, InstanceID, monsterid, localA-15, localB, localC+15)
					MonID[5] = map.MapCreateCreature(3424072232, InstanceID, monsterid, localA-5, localB, localC-15)
					MonID[6] = map.MapCreateCreature(3424072232, InstanceID, monsterid, localA+17, localB, localC+17)
					for i =1,6 do
					    if math.random(10) > 8 then
							local k = math.random(3)
							unit.AddBuff(MapID, InstanceID, MonID[i], 6100000+k, MonID[i])
						end
					end
				--东洲冥山s08
				elseif MapID == 3424075048 then
					if level <= 80 then level = 80
					elseif level >= 100 then level = 100 end
					local bossid = 2013000 + (level-20)/5*2 + math.random(1,2)
					local monsterid = 2012000 + (level-20)/5 + 1
					local MonID = {}
					MonID[1] = map.MapCreateCreature(3424075048, InstanceID, bossid, localA+2, localB, localC+12)
					MonID[2] = map.MapCreateCreature(3424075048, InstanceID, monsterid, localA+15, localB, localC+15)
					MonID[3] = map.MapCreateCreature(3424075048, InstanceID, monsterid, localA+5, localB, localC-15)
					MonID[4] = map.MapCreateCreature(3424075048, InstanceID, monsterid, localA-15, localB, localC+15)
					MonID[5] = map.MapCreateCreature(3424075048, InstanceID, monsterid, localA-5, localB, localC-15)
					MonID[6] = map.MapCreateCreature(3424075048, InstanceID, monsterid, localA+17, localB, localC+17)
					for i =1,6 do
					    if math.random(10) > 8 then
							local k = math.random(3)
							unit.AddBuff(MapID, InstanceID, MonID[i], 6100000+k, MonID[i])
						end
					end
				end
			end
			map.MapDeleteCreature(MapID, InstanceID, NPCID)
		end
	return
end

aux.RegisterCreatureEvent(5510101, 7, "mowu_OnTalk")


function mozuruqin_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

    if role.GetTrialState(RoleID, 2014) == 1 then
		role.SetTrialCompleteNum(RoleID, 2014, role.GetTrialCompleteNum(RoleID,2014) + 1)
    end
	if role.GetTrialState(RoleID, 2014) == 1 and role.GetTrialCompleteNum(RoleID,2014) >= Trail_maxnum[2014] then
		role.TrialComplete(RoleID,2014)
	end

	role.NotifyFBBS(RoleID,12,0)

	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	if TeamID ~= nil and TeamID ~= 4294967295 then
		local Role = {}
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
		for i=1, 6 do
			if Role[i] and Role[i]~= 4294967295 then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
				if Target_MapID ~= nil and Target_InstanceID ~= nil and Target_MapID == MapID and Target_InstanceID == InstanceID then
					for k=30627,30643 do
						if role.IsRoleHaveQuest(MapID, InstanceID, Role[i], k) == true then
							role.ModSpecialTargetValue(MapID, InstanceID, Role[i], k, 2)
						    role.SetRoleScriptData(Role[i], 1, RoleDataType["ActivityZhiYin"], role.GetRoleScriptData(Role[i], 1, RoleDataType["ActivityZhiYin"]) % 10 + 10+math.floor(role.GetRoleScriptData(Role[i], 1, RoleDataType["ActivityZhiYin"])/100)*100)
						end
					end
				end
			end
		end
	else
		for k=30627,30643 do
			if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, k) == true then
				role.ModSpecialTargetValue(MapID, InstanceID, RoleID, k, 2)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"], role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"]) % 10 +10+math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"])/100)*100)
			end
		end
	end
	--玩家计数
	UpdateRoleData(RoleID, 14, 1)
end
aux.RegisterCreatureEvent(2013001, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013002, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013003, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013004, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013005, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013006, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013007, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013008, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013009, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013010, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013011, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013012, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013013, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013014, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013015, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013016, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013017, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013018, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013019, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013020, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013021, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013022, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013023, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013024, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013025, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013026, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013027, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013028, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013029, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013030, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013031, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013032, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013033, 4, "mozuruqin_OnDie")
aux.RegisterCreatureEvent(2013034, 4, "mozuruqin_OnDie")


