--地图ScriptData字段意义说明。
--[[0:是否第一个人进来。
    1：记录困魔天球的ID。
	2:记录现在剩余的怪物数。
	3：记录现在的波数
	4:记录已经交任务的人的个数
	]]
--RoleDataType["baiguiVIP"]记录百鬼炼狱完成日期
BaiGui_Pos = {}
BaiGui_Pos[1] = {x=104,y=10750,z=108}
BaiGui_Pos[2] = {x=118,y=2000,z=178}
BaiGui_Pos[3] = {x=127,y=2000,z=180}
BaiGui_Pos[4] = {x=131,y=2000,z=164}
BaiGui_Pos[5] = {x=139,y=2000,z=160}
BaiGui_Pos[6] = {x=147,y=2000,z=168}
BaiGui_Pos[7] = {x=141,y=2000,z=175}
BaiGui_Pos[8] = {x=147,y=2000,z=196}
BaiGui_Pos[9] = {x=157,y=2000,z=201}
BaiGui_Pos[10] = {x=157,y=2000,z=186}
BaiGui_Pos[11] = {x=159,y=2000,z=167}
BaiGui_Pos[12] = {x=175,y=2000,z=212}
BaiGui_Pos[13] = {x=184,y=2000,z=209}
BaiGui_Pos[14] = {x=182,y=2000,z=181}
BaiGui_Pos[15] = {x=174,y=2000,z=186}
BaiGui_Pos[16] = {x=172,y=2000,z=168}
BaiGui_Pos[17] = {x=179,y=2000,z=158}
BaiGui_Pos[18] = {x=185,y=2000,z=170}
BaiGui_Pos[19] = {x=199,y=2000,z=179}
BaiGui_Pos[20] = {x=201,y=2000,z=157}
BaiGui_Pos[21] = {x=208,y=2000,z=149}
BaiGui_Pos[22] = {x=200,y=2000,z=140}
BaiGui_Pos[23] = {x=189,y=2000,z=147}
BaiGui_Pos[24] = {x=178,y=2000,z=137}
BaiGui_Pos[25] = {x=183,y=2000,z=118}
BaiGui_Pos[26] = {x=128,y=2000,z=73}
BaiGui_Pos[27] = {x=118,y=2000,z=81}
BaiGui_Pos[28] = {x=104,y=2000,z=70}
BaiGui_Pos[29] = {x=93,y=2000,z=53}
BaiGui_Pos[30] = {x=100,y=2000,z=82}
BaiGui_Pos[31] = {x=108,y=2000,z=93}
BaiGui_Pos[32] = {x=91,y=2000,z=88}
BaiGui_Pos[33] = {x=82,y=2000,z=78}
BaiGui_Pos[34] = {x=83,y=2000,z=72}
BaiGui_Pos[35] = {x=69,y=2000,z=61}
BaiGui_Pos[36] = {x=79,y=2000,z=52}
BaiGui_Pos[37] = {x=68,y=2000,z=41}
BaiGui_Pos[38] = {x=59,y=2000,z=51}
BaiGui_Pos[39] = {x=59,y=2000,z=66}
BaiGui_Pos[40] = {x=67,y=2000,z=74}
BaiGui_Pos[41] = {x=69,y=2000,z=87}
BaiGui_Pos[42] = {x=54,y=2000,z=89}
BaiGui_Pos[43] = {x=51,y=2000,z=76}
BaiGui_Pos[44] = {x=42,y=2000,z=83}
BaiGui_Pos[45] = {x=34,y=2000,z=68}
BaiGui_Pos[46] = {x=35,y=2000,z=98}
BaiGui_Pos[47] = {x=45,y=2000,z=107}
BaiGui_Pos[48] = {x=54,y=2000,z=102}
BaiGui_Pos[49] = {x=67,y=2000,z=106}
BaiGui_Pos[50] = {x=60,y=2000,z=119}
BaiGui_Pos[51] = {x=73,y=2000,z=126}
BaiGui_Pos[52] = {x=69,y=2000,z=140}
BaiGui_Pos[53] = {x=80,y=2000,z=70}--BOSS
BaiGui_Pos[54] = {x=160,y=127,z=144}--传送和封印

--505,23167,304
--521,23167,304
BaiGui_TimerMin = 0
BaiGuiquest_complete = {}
BaiGuiinstance_times = {}
BaiGui_creature = {}
shenmozaowurenwu = 0


function BaiGui_OnStart(actID)

	--重置TimerMin
	BaiGui_TimerMin = 1
    BaiGuiquest_complete = {}
	BaiGuiinstance_times = {}
	BaiGui_creature = {}

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100030)  --百鬼炼狱活动已经开始，挺过十次鬼怪的猛烈攻击就可以获得大量的经验和金钱！可找斗战天城[坐标，坐标]百鬼修炼人参与活动！
	msg.DispatchBroadcast(MsgID, -1, -1, -1)
	BaiGui_creature[1] = map.MapCreateCreature(3424073512, -1, 5510201, 517, 23167, 306) --刷新百鬼修炼人
	--msg.SendWorldSwitchMsg(1, 40, 55, 1, actID)
end


function BaiGui_OnTimerMin(actID)
	BaiGui_TimerMin = BaiGui_TimerMin + 1
	--[[if BaiGui_TimerMin == 2 then
	    msg.SendWorldSwitchMsg(2, 56, 75, 1, actID)
	elseif BaiGui_TimerMin == 3 then
	    msg.SendWorldSwitchMsg(3, 76, 90, 1, actID)
	elseif BaiGui_TimerMin == 4 then
	    msg.SendWorldSwitchMsg(4, 91, 100, 1, actID)
	end]]
	if BaiGui_TimerMin <= 10 and BaiGui_TimerMin % 5 == 0 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100030)  --百鬼炼狱活动已经开始，挺过十次鬼怪的攻击就可以获得大量的经验和金钱！可找斗战天城[坐标，坐标]百鬼修炼人参与活动！
		msg.DispatchBroadcast(MsgID, -1, -1, -1)
	end

end

-- npc生存时间到，活动结束，在各城市中删除此npc
function BaiGui_OnEnd(actID)

	map.MapDeleteCreature(3424073512, -1, BaiGui_creature[1])

end

--[[function BaiGui_OnClick(MapID, InstanceID, RoleID, TOMsgID, ToMapID, x, y, z)

	if BaiGui_TimerMin >= 1 and BaiGui_TimerMin <= 4 then
		if role.IsRoleInStatus(RoleID, 1024) then
			role.CancelRoleStallState(MapID, InstanceID, RoleID)
		end
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, ToMapID, x, y, z)      --传送
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 71, 100006) -- 您点击确定的时间过晚，已经超出了时间限制，请自行前往。
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
	end
end]]

--注册该活动的相关事件
--aux.RegisterActEvent(22, 8, "BaiGui_OnClick")
aux.RegisterActEvent(22, 2, "BaiGui_OnStart")
aux.RegisterActEvent(22, 4, "BaiGui_OnTimerMin")
aux.RegisterActEvent(22, 3, "BaiGui_OnEnd")

--传送进副本

function x5510201_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
    --获取队伍玩家等级
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
    if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20,	204001)		--“百鬼修炼人：\n     40级以上的玩家进入百鬼炼狱完成我交给你的挑战任务可以获得大量的经验。您想现在就进入百鬼炼狱进行挑战吗？”
		msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
		msg.AddMsgEvent(MsgID, 21,	6)			--确定按钮
		msg.AddMsgEvent(MsgID, 1,	204002)		--“接取挑战任务”
		msg.AddMsgEvent(MsgID, 21,	4)			--确定按钮
		msg.AddMsgEvent(MsgID, 1,	204003)		--“进入修罗界”
		msg.AddMsgEvent(MsgID, 21,	5)			--取消按钮
		msg.AddMsgEvent(MsgID, 1,	100002)		--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 6 then
		if BaiGuiquest_complete[RoleID] == 1 then
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 204004)		--“每次活动期间只能完成一次任务！”
			msg.AddMsgEvent(MsgID, 21, -1)			--取消按钮
			msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
			msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			local bool = false
			for i=30101,30112 do
				if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, i) == true then
					bool = true
				end
			end
			if bool == true then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 204005)		--“您已经接取过任务了，请进入百鬼炼狱挑战。”
				msg.AddMsgEvent(MsgID, 21, -1)			--取消按钮
				msg.AddMsgEvent(MsgID, 1, 100002)		--“取消”
				msg.AddMsgEvent(MsgID, 24, TargetID)	--npcid
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif rolelevel < 40 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 	204006)		--“您等级不足40级，不能进入百鬼炼狱”
				msg.AddMsgEvent(MsgID, 21, 	5)			--取消按钮
				msg.AddMsgEvent(MsgID, 1, 	100002)		--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				--等级和怪物任务分配
				local quest = 0
				if rolelevel >= 100 then
					quest = 30112
				elseif rolelevel >= 95 and rolelevel <= 99 then
					quest = 30111
				elseif rolelevel >= 90 and rolelevel <= 94 then
					quest = 30110
				elseif rolelevel >= 85 and rolelevel <= 89 then
					quest = 30109
				elseif rolelevel >= 80 and rolelevel <= 84 then
					quest = 30108
				elseif rolelevel >= 75 and rolelevel <= 79 then
					quest = 30107
				elseif rolelevel >= 70 and rolelevel <= 74 then
					quest = 30106
				elseif rolelevel >= 65 and rolelevel <= 69 then
					quest = 30105
				elseif rolelevel >= 60 and rolelevel <= 64 then
					quest = 30104
				elseif rolelevel >= 55 and rolelevel <= 59 then
					quest = 30103
				elseif rolelevel >= 50 and rolelevel <= 54 then
					quest = 30102
				elseif rolelevel >= 40 and rolelevel <= 49 then
					quest = 30101
				end
				if quest ~= 0 then
				    role.AddQuest(RoleID, quest)
				end
			end
		end
	elseif TalkIndex == 4 then
		--判断玩家是否可以进入副本
		if TeamID == 4294967295 then
		local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 20, 	100004)		--“您没有组队，不能进入该副本”
			msg.AddMsgEvent(MsgID, 21, 	5)			--取消按钮
			msg.AddMsgEvent(MsgID, 1, 	100002)		--“取消”
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		else
			if rolelevel < 40 then
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 	204006)		--“您等级不足40级，不能进入百鬼炼狱。”
				msg.AddMsgEvent(MsgID, 21, 	5)			--取消按钮
				msg.AddMsgEvent(MsgID, 1, 	100002)		--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			elseif GetTeamPlayerNum(MapID,InstanceID,RoleID) < 4 then----------------------------------------
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 20, 	204007)	--“队伍中需要有至少4人才能参加此活动。”
				msg.AddMsgEvent(MsgID, 21, 	5)		--取消按钮
				msg.AddMsgEvent(MsgID, 1, 	100002)	--“取消”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			else
				local bool = false
				for i=30101,30112 do
					if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, i) == true then
						bool = true
					end
				end
				if bool == false then
					local MsgID = msg.BeginMsgEvent()
					msg.AddMsgEvent(MsgID, 20, 	204008)		--“您需要接取了挑战任务之后才能进入百鬼炼狱。”
					msg.AddMsgEvent(MsgID, 21, 	-1)			--取消按钮
					msg.AddMsgEvent(MsgID, 1, 	100002)		--“取消”
					msg.AddMsgEvent(MsgID, 24,	TargetID)	--npcid
					msg.DispatchRoleMsgEvent(RoleID, MsgID)
				elseif bool == true then
				    if BaiGuiinstance_times[RoleID] ~= nil and BaiGuiinstance_times[RoleID] > 5 then
					    local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 20, 204009)	--“每次活动您只能进入5次修罗界，您已经达到此上限，请下次活动再来。”
						msg.AddMsgEvent(MsgID, 21, -1)		--取消按钮
						msg.AddMsgEvent(MsgID, 1, 100002)	--“取消”
						msg.AddMsgEvent(MsgID, 24, TargetID)
						msg.DispatchRoleMsgEvent(RoleID, MsgID)
					else
						if BaiGuiinstance_times[RoleID] == nil then
							BaiGuiinstance_times[RoleID] = 1
						else
							BaiGuiinstance_times[RoleID] = BaiGuiinstance_times[RoleID] + 1
						end
						role.RoleGotoNewMap(MapID, InstanceID, RoleID, 619032762, BaiGui_Pos[53].x, 0, BaiGui_Pos[53].z)
					end
				end
			end
		end
	end
end
--注册百鬼修炼人对话事件
aux.RegisterCreatureEvent(5510201, 7, "x5510201_OnTalk")

function h02_OnPlayerEnter(MapID, InstanceID, RoleID)
    local bool = false
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	for i=30101,30112 do
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, i) == true then
			bool = true
		end
	end
	if BaiGuiquest_complete[RoleID] ~= 1 and rolelevel >= 40 and bool == false then
	    local quest = 0
		if rolelevel >= 100 then
			quest = 30112
		elseif rolelevel >= 95 and rolelevel <= 99 then
			quest = 30111
		elseif rolelevel >= 90 and rolelevel <= 94 then
			quest = 30110
		elseif rolelevel >= 85 and rolelevel <= 89 then
			quest = 30109
		elseif rolelevel >= 80 and rolelevel <= 84 then
			quest = 30108
		elseif rolelevel >= 75 and rolelevel <= 79 then
			quest = 30107
		elseif rolelevel >= 70 and rolelevel <= 74 then
			quest = 30106
		elseif rolelevel >= 65 and rolelevel <= 69 then
			quest = 30105
		elseif rolelevel >= 60 and rolelevel <= 64 then
			quest = 30104
		elseif rolelevel >= 55 and rolelevel <= 59 then
			quest = 30103
		elseif rolelevel >= 50 and rolelevel <= 54 then
			quest = 30102
		elseif rolelevel >= 40 and rolelevel <= 49 then
			quest = 30101
		end
		if quest ~= 0 then
			role.AddQuest(RoleID, quest)
		end
	end
	if map.GetMapScriptData(MapID, InstanceID, 1, 0) ~= 1 then
		map.SetMapScriptData(MapID, InstanceID, 1, 0, 1)--已经有人进入了副本
		local kunmotianqiuid = map.MapCreateCreature(MapID, InstanceID, 5510202, BaiGui_Pos[54].x, BaiGui_Pos[54].y, BaiGui_Pos[54].z) --刷新封印
		map.SetMapScriptData(MapID, InstanceID, 1, 1, kunmotianqiuid)
    end
	--百鬼总人数
	local curTime = tonumber(os.date("%j"))
	if baiGui_Roles[RoleID] == nil then
		UpdateSeverData(47, 1)
		baiGui_Roles[RoleID] = curTime
	elseif baiGui_Roles[RoleID] ~= curTime then
		baiGui_Roles = {}
		baiGui_Roles[RoleID] = curTime
		UpdateSeverData(47, 1)
	end
end

--注册
aux.RegisterMapEvent("h02", 2, "h02_OnPlayerEnter")


function x5510202_OnTalk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)

	if TalkIndex == -1 then
	    local Wave = map.GetMapScriptData(MapID, InstanceID, 1, 3)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 	204010)	--“您想要放出百鬼封印中的怪物吗？”
		msg.AddMsgEvent(MsgID, 24,	TargetID)
		msg.AddMsgEvent(MsgID, 21, 	4)		--确定按钮
		msg.AddMsgEvent(MsgID, 1, 	204011)	--释放魔物
		msg.AddMsgEvent(MsgID, 21, 	5)		--取消按钮
		msg.AddMsgEvent(MsgID, 1, 	100002)	--取消
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then     --计算应该刷出什么难度的怪物
	    local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if TeamID ~= 4294967295 then
            local Wave = map.GetMapScriptData(MapID, InstanceID, 1, 3) + 1
			map.SetMapScriptData(MapID, InstanceID, 1, 3, Wave)
		    BaiGui_CreateCreature(MapID, InstanceID, rolelevel, Wave)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100031)    --第X波魔物出现
			msg.AddMsgEvent(MsgID, 9, Wave)
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
		else
			local Wave = map.GetMapScriptData(MapID, InstanceID, 1, 3) + 1
			map.SetMapScriptData(MapID, InstanceID, 1, 3, Wave)
		    BaiGui_CreateCreature(MapID, InstanceID, rolelevel, Wave)
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100031)    --第X波魔物出现
			msg.AddMsgEvent(MsgID, 9, Wave)
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
		end
		map.MapDeleteCreature(MapID, InstanceID, TargetID)
	end
end
--注册
aux.RegisterCreatureEvent(5510202, 7, "x5510202_OnTalk")

--清完小怪刷出封印
function BaiGui_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
    if TargetTypeID ~= 2023014 then
		local Num = map.GetMapScriptData(MapID, InstanceID, 1, 2)
		Num = Num - 1
		if Num < 0 or Num == nil then Num = 0 end
		map.SetMapScriptData(MapID, InstanceID, 1, 2, Num)
		local Wave = map.GetMapScriptData(MapID, InstanceID, 1, 3)
		if Num == 0 and Wave ~= 10 then
			local fengyinid = map.MapCreateCreature(MapID, InstanceID, 5510202, BaiGui_Pos[54].x, BaiGui_Pos[54].y, BaiGui_Pos[54].z) --刷新百鬼封印
			map.SetMapScriptData(MapID, InstanceID, 1, 1, fengyinid)
			BaiGui_addexp(MapID, InstanceID, RoleID, Wave)
		elseif Num == 0 and Wave == 10 then
			map.MapCreateCreature(MapID, InstanceID, 5510203, BaiGui_Pos[54].x, BaiGui_Pos[54].y, BaiGui_Pos[54].z)
			BaiGui_addexp(MapID, InstanceID, RoleID, Wave)
		else
			local Num2 = 0
			if Wave == 1 then
				Num2 = 11
			elseif Wave == 2 then
				Num2 = 16
			elseif Wave == 3 then
				Num2 = 21
			elseif Wave == 4 then
				Num2 = 21
			elseif Wave == 5 then
				Num2 = 21
			elseif Wave == 6 then
				Num2 = 21
			elseif Wave == 7 then
				Num2 = 21
			elseif Wave == 8 then
				Num2 = 21
			elseif Wave == 9 then
				Num2 = 1
			elseif Wave == 10 then
				Num2 = 50
			end
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 204012)    --第X波怪物：XX/XX
			msg.AddMsgEvent(MsgID, 9, Wave)
			msg.AddMsgEvent(MsgID, 9, Num2-Num)
			msg.AddMsgEvent(MsgID, 9, Num2)
			msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
		end
	--击杀最终BOSS之后会为所有玩家完成任务
	else
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		if TeamID ~= nil and TeamID ~= 4294967295 then
			local Role = {}
			Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
			for i=1, 6 do
				if Role[i] and Role[i]~= 4294967295 then
					local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
					if Target_MapID == MapID and Target_InstanceID == InstanceID then
						for k=30101,30112 do
							if role.IsRoleHaveQuest(MapID, InstanceID, Role[i], k) == true then
								role.ModSpecialTargetValue(MapID, InstanceID, Role[i], k, 2)
							    role.SetRoleScriptData(Role[i], 1, RoleDataType["ActivityZhiYin"], role.GetRoleScriptData(Role[i], 1, RoleDataType["ActivityZhiYin"]) % 100 + 100)
							end
						end
					end
				end
			end
		else
		    for k=30101,30112 do
				if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, k) == true then
					role.ModSpecialTargetValue(MapID, InstanceID, RoleID, k, 2)
					role.SetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"], role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"]) % 100 + 100)
				end
			end
		end
	end
	--回购任务
	if TargetTypeID >= 2023001 and TargetTypeID <= 2023012 then
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		if TeamID ~= nil and TeamID ~= 4294967295 then
			local Role = {}
			Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
			for i=1, 6 do
				if Role[i] and Role[i]~= 4294967295 then
					local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i])
					if Target_MapID == MapID and Target_InstanceID == InstanceID then
						local LiLianBaiGui = {}
						LiLianBaiGui[30807] = 36
						LiLianBaiGui[30808] = 30
						LiLianBaiGui[30809] = 30
						LiLianBaiGui[30810] = 25
						LiLianBaiGui[30811] = 22
						LiLianBaiGui[30812] = 18
						LiLianBaiGui[30813] = 17
						for k,v in pairs(LiLianBaiGui) do
							if role.IsRoleHaveQuest(MapID, InstanceID, Role[i], k) == true then
								HuoDeLiLian(MapID, InstanceID, Role[i], v)
								LiLian_CheakComplete(MapID, InstanceID, Role[i])
								break
							end
						end
					end
				end
			end
		else
			local LiLianBaiGui = {}
			LiLianBaiGui[30807] = 36
			LiLianBaiGui[30808] = 30
			LiLianBaiGui[30809] = 30
			LiLianBaiGui[30810] = 25
			LiLianBaiGui[30811] = 22
			LiLianBaiGui[30812] = 18
			LiLianBaiGui[30813] = 17
			for k,v in pairs(LiLianBaiGui) do
				if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, k) == true then
					HuoDeLiLian(MapID, InstanceID, RoleID, v)
					LiLian_CheakComplete(MapID, InstanceID, RoleID)
					break
				end
			end
		end
	end
	--完事赶紧删了
end

aux.RegisterCreatureEvent(2021001, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021002, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021003, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021004, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021005, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021006, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021007, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021008, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021009, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021010, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021011, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021012, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021013, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021014, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021015, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021016, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021017, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021018, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021019, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021020, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021021, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021022, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021023, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021024, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021025, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021026, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021027, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021028, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021029, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021030, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021031, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021032, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021033, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021034, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021035, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021036, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021037, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021038, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021039, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021040, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021041, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021042, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021043, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021044, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021045, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021046, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021047, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2021048, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022001, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022002, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022003, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022004, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022005, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022006, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022007, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022008, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022009, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022010, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022011, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022012, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022013, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022014, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022015, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022016, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022017, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022018, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022019, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022020, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022021, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022022, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022023, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022024, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022025, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022026, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022027, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022028, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022029, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022030, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022031, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022032, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022033, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022034, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022035, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022036, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022037, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022038, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022039, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022040, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022041, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022042, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022043, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022044, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022045, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022046, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022047, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022048, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022049, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022050, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022051, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022052, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022053, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022054, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022055, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022056, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022057, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022058, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022059, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022060, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022061, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022062, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022063, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022064, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022065, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022066, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022067, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022068, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022069, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022070, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022071, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022072, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022073, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022074, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022075, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022076, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022077, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022078, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022079, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022080, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022081, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022082, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022083, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022084, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022085, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022086, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022087, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022088, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022089, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022090, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022091, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022092, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022093, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022094, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022095, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2022096, 4, "BaiGui_OnDie")


aux.RegisterCreatureEvent(2023001, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2023002, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2023003, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2023004, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2023005, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2023006, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2023007, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2023008, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2023009, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2023010, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2023011, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2023012, 4, "BaiGui_OnDie")
aux.RegisterCreatureEvent(2023014, 4, "BaiGui_OnDie")


--完成任务
function BaiGui_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	BaiGuiquest_complete[RoleID] = 1
    map.SetMapScriptData(MapID, InstanceID, 1, 4, map.GetMapScriptData(MapID, InstanceID, 1, 4) + 1)
	role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3424073512, 30532/50, 23094, 33563/50)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4800287, 4, -1, 8, 420)
	if role.GetTrialState(RoleID, 4015) == 1 then
		role.SetTrialCompleteNum(RoleID, 4015, role.GetTrialCompleteNum(RoleID,4015) + 1)
    end
	if role.GetTrialState(RoleID, 4015) == 1 and role.GetTrialCompleteNum(RoleID,4015) >= Trail_maxnum[4015] then
		role.TrialComplete(RoleID,4015)
	end

	role.SetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"], role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"]) % 100)
	for k=30101,30112 do
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, k) == true and QuestID ~= k then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"], role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"]) % 100 + 100)
		end
	end
	--[[奖励神之残躯
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel >= 50 then
        role.AddRoleItem(MapID, InstanceID, RoleID, 4820081, 2, -1, 8, 420)
	end]]
	--玩家计数
	UpdateRoleData(RoleID, 9, 1)
	--vip特殊奖励
	if role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 90034) == true then
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["baiguiVIP"]) == 0 or role.GetRoleScriptData(RoleID, 1, RoleDataType["baiguiVIP"]) == nil then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["baiguiVIP"], tonumber(os.date("%j")))
		end
		if role.GetRoleScriptData(RoleID, 1, RoleDataType["baiguiVIP"]) ~= tonumber(os.date("%j")) then
			local theDays = tonumber(os.date("%j")) - role.GetRoleScriptData(RoleID, 1, RoleDataType["baiguiVIP"]) - 1
			local addedExp = {}
			addedExp[30101] = 165000
			addedExp[30102] = 432000
			addedExp[30103] = 499500
			addedExp[30104] = 612000
			addedExp[30105] = 787500
			addedExp[30106] = 1066500
			addedExp[30107] = 1381500
			addedExp[30108] = 1615500
			addedExp[30109] = 2002500
			addedExp[30110] = 2443500
			addedExp[30111] = 2925000
			addedExp[30112] = 2925000
			if theDays >= 4 or theDays >= -361 and theDays < 0 then
				role.AddRoleExp(MapID, InstanceID, RoleID, addedExp[QuestID]*4)
			elseif theDays == 3 or theDays == -362 then
				role.AddRoleExp(MapID, InstanceID, RoleID, addedExp[QuestID]*3)
			elseif theDays == 2 or theDays == -363 then
				role.AddRoleExp(MapID, InstanceID, RoleID, addedExp[QuestID]*2)
			elseif theDays == 1 or theDays == -364 then
				role.AddRoleExp(MapID, InstanceID, RoleID, addedExp[QuestID])
			end
		end
	end
	role.SetRoleScriptData(RoleID, 1, RoleDataType["baiguiVIP"], tonumber(os.date("%j")))
	
	---任务完成完成神魔造物任务
	if QuestID==30102 or QuestID==30103 or QuestID==30104 or QuestID==30105 or QuestID==30106 then
		local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30118)
		if quest == true then
			role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 30118, 2)
			shenmozaowurenwu = 1
		else
			shenmozaowurenwu = 0
		end
	elseif QuestID==30107 or QuestID==30108 then 
		local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30119)
		if quest == true then
			role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 30119, 2)
			shenmozaowurenwu = 1
		else
			shenmozaowurenwu = 0
		end
	elseif  QuestID==30109 or  QuestID==30110 or  QuestID==30111 then
		local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 30120)
		if quest == true then
			role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 30120, 2)
			shenmozaowurenwu = 1
		else
			shenmozaowurenwu = 0
		end
	end
end
aux.RegisterQuestEvent(30101, 1, "BaiGui_OnComplete")
aux.RegisterQuestEvent(30102, 1, "BaiGui_OnComplete")
aux.RegisterQuestEvent(30103, 1, "BaiGui_OnComplete")
aux.RegisterQuestEvent(30104, 1, "BaiGui_OnComplete")
aux.RegisterQuestEvent(30105, 1, "BaiGui_OnComplete")
aux.RegisterQuestEvent(30106, 1, "BaiGui_OnComplete")
aux.RegisterQuestEvent(30107, 1, "BaiGui_OnComplete")
aux.RegisterQuestEvent(30108, 1, "BaiGui_OnComplete")
aux.RegisterQuestEvent(30109, 1, "BaiGui_OnComplete")
aux.RegisterQuestEvent(30110, 1, "BaiGui_OnComplete")
aux.RegisterQuestEvent(30111, 1, "BaiGui_OnComplete")
aux.RegisterQuestEvent(30112, 1, "BaiGui_OnComplete")

--检测完成任务
function BaiGui_OnCheckComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local a = map.GetMapScriptData(MapID, InstanceID, 1, 4)
	if a ==nil then a = 0 end
    if role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"]) >= 100 then
        if MapID == 619032762 then
		    local a = map.GetMapScriptData(MapID, InstanceID, 1, 4)
			if a ==nil then a = 0 end
		    if a < 6 then
				return 1
			else
			    local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 26, 204013)	--“每个百鬼副本只有前6个玩家可以交付任务。”
				msg.DispatchRoleMsgEvent(RoleID, MsgID)
			    return 0
			end
		else
		    return 1
		end
	else
        return 0
	end
	local rolelevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if rolelevel >= 50 then
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
	
end

aux.RegisterQuestEvent(30101, 5, "BaiGui_OnCheckComplete")
aux.RegisterQuestEvent(30102, 5, "BaiGui_OnCheckComplete")
aux.RegisterQuestEvent(30103, 5, "BaiGui_OnCheckComplete")
aux.RegisterQuestEvent(30104, 5, "BaiGui_OnCheckComplete")
aux.RegisterQuestEvent(30105, 5, "BaiGui_OnCheckComplete")
aux.RegisterQuestEvent(30106, 5, "BaiGui_OnCheckComplete")
aux.RegisterQuestEvent(30107, 5, "BaiGui_OnCheckComplete")
aux.RegisterQuestEvent(30108, 5, "BaiGui_OnCheckComplete")
aux.RegisterQuestEvent(30109, 5, "BaiGui_OnCheckComplete")
aux.RegisterQuestEvent(30110, 5, "BaiGui_OnCheckComplete")
aux.RegisterQuestEvent(30111, 5, "BaiGui_OnCheckComplete")
aux.RegisterQuestEvent(30112, 5, "BaiGui_OnCheckComplete")


function shenmozaowu_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	if shenmozaowurenwu == 1 then
		return 1
	end
end
aux.RegisterQuestEvent(30118, 5, "shenmozaowu_Complete")
aux.RegisterQuestEvent(30119, 5, "shenmozaowu_Complete")
aux.RegisterQuestEvent(30120, 5, "shenmozaowu_Complete")




