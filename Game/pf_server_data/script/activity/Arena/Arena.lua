--竞技场开始时触发的脚本
Arena = {}                    --用来记录竞技场地图副本对应的竞技场副本号以及竞技场的时间状态以及6个玩家的ID
ArenaMapNum = 0               --用来记录竞技场地图开了多少个
ArenaPlayer = {}              --用来记录参与竞技场的玩家的属性
ArenaCreature_Timer = {}      --用来记录竞技场中的计时器怪物所在的副本ID和类型ID。
--[[ArenaCreature_Timer[creatureID]= {TimerTypeID}
	   TimerTypeID枚举值
	   1 1分钟的开门计时器
	   2 10分钟的倒计时计时器
	   3 30秒的过渡计时器（接15秒的）
	   4 15秒的屏显计时器（还有15秒就开门的计时器）
	   5 10分钟的结算计时器，在开门后刷出。加开关，如果没有人则不会触发效果
	   6 30秒的传送计时器
	   7 30秒后结束战斗,释放内存
	   9 15秒报时
	   ]]

ArenaMap = {}
ArenaMap[1] = {ArenaMapID = 2092183860,Door1 = 175,Door2 = 174,RedPoint = {x=210, y=207, z=71},BluePoint = {x=42, y=207, z=182},ExitPoint = {x=551, y=459, z=485},BoxPoint = {x=128, y=11330, z=118}}
ArenaMap[2] = {ArenaMapID = 2092184116,Door1 = 491,Door2 = 490,RedPoint = {x=39, y=14, z=169},BluePoint = {x=217, y=14, z=114},ExitPoint = {x=822, y=470, z=609},BoxPoint = {x=129, y=530, z=151}}
ArenaMap[0] = {ArenaMapID = 2092184372,Door1 = 174,Door2 = 175,RedPoint = {x=123, y=16, z=46},BluePoint = {x=260, y=16, z=338},ExitPoint = {x=286, y=470, z=749},BoxPoint = {x=191, y=1280, z=179}}


function Arena_ArenaOnStart(MapID, InstanceID, RoomID, RoleID1, RoleID2, RoleID3, RoleID4, RoleID5, RoleID6)

	--为竞技场分配地图和副本
	MapID, InstanceID = GetArenaMapID()

    --随机划分阵营
	RoleID1,RoleID2,RoleID3,RoleID4,RoleID5,RoleID6 = PlayerRandom_Arena(RoleID1,RoleID2,RoleID3,RoleID4,RoleID5,RoleID6)

	--为竞技场信息赋初始值
    DefineValue_Arena (InstanceID, RoomID, RoleID1, RoleID2, RoleID3, RoleID4, RoleID5, RoleID6)

	--向玩家弹出传送竞技场的消息,并假设他们没有进入竞技场，被扣掉分数
	local Role = {}  --玩家ID的索引
	Role[1] = RoleID1
	Role[2] = RoleID2
	Role[3] = RoleID3
	Role[4] = RoleID4
	Role[5] = RoleID5
	Role[6] = RoleID6

	for i=1,6 do
	    local MapID_Arena,InstanceID_Arena = role.GetRoleMapID(Role[i])
		if act.GetActIsStart(31) == true then
			msg.SendRoleSwitchMsg(1, MapID_Arena, InstanceID_Arena, Role[i], 1, 31)
        else
		    msg.SendRoleSwitchMsg(1, MapID_Arena, InstanceID_Arena, Role[i], 1, 32)
		end
		unit.AddBuff(MapID_Arena, InstanceID_Arena, Role[i], 6936001, Role[i])
		role.SetRoleScriptData(Role[i], 1, RoleDataType["Point_Arena"], Lose_Point_Arena(InstanceID, Role[i]))
	end

	--在竞技场中刷出宝箱
	if math.random(10) == 5 then
		map.MapCreateCreature(MapID, InstanceID, 5512005, ArenaMap[InstanceID%3].BoxPoint.x, ArenaMap[InstanceID%3].BoxPoint.y,ArenaMap[InstanceID%3].BoxPoint.z)
	else
	    map.MapCreateCreature(MapID, InstanceID, 5512004, ArenaMap[InstanceID%3].BoxPoint.x, ArenaMap[InstanceID%3].BoxPoint.y,ArenaMap[InstanceID%3].BoxPoint.z)
	end

	--在竞技场中刷出计时用的透明怪
	local creature1 = map.MapCreateCreature(MapID, InstanceID, 1094003, 6, 1, 9)
	local creature2 = map.MapCreateCreature(MapID, InstanceID, 1094002, 6, 1, 9)
	local creature3 = map.MapCreateCreature(MapID, InstanceID, 1094004, 6, 1, 9)
	local creature4 = map.MapCreateCreature(MapID, InstanceID, 1094001, 6, 1, 9)

	ArenaCreature_Timer[creature1]= {TypeID = 1}      --1分钟开门
	ArenaCreature_Timer[creature2]= {TypeID = 3}      --30秒计时
	ArenaCreature_Timer[creature3]= {TypeID = 2}      --10分钟计时
	ArenaCreature_Timer[creature4]= {TypeID = 9}      --15秒

end

function Arena_OnClick(MapID, InstanceID, RoleID, TOMsgID, ToMapID, x, y, z)

	if ArenaPlayer[RoleID] ~= nil then

		--获取将要进入的斗技场的InstanceID
		local InstanceID_Now = GetNowArenaInstanceID(RoleID)

		if InstanceID_Now ~= 0 then

			--获取斗技场开始的时间和现在时间
			local Hour 		= tonumber(os.date("%H"))
			local Min  		= tonumber(os.date("%M"))
			local Second	= tonumber(os.date("%S"))

			local Hour_open = math.floor(Arena[InstanceID_Now].OpenTime / 10000)
			local Min_open = math.floor((Arena[InstanceID_Now].OpenTime % 10000) / 100)
			local Second_open = Arena[InstanceID_Now].OpenTime % 100

			--只有1分钟内点击进入竞技场才能生效
			local function ShorterThan1Min()
				if Hour - Hour_open == 1 then
					if Min - Min_open == -59 then
						if Second <= Second_open then
							return true
						end
					end
				elseif Hour == Hour_open then
					if Min - Min_open == 1 then
						if Second <= Second_open then
							return true
						end
					elseif Min == Min_open then
						if Second >= Second_open then
							return true
						end
					end
				else
					return false
				end
			end

			if ShorterThan1Min() and unit.IsDead(MapID, InstanceID, RoleID) ~= true then
				if ArenaPlayer[RoleID] ~= nil and ArenaPlayer[RoleID].Info[InstanceID_Now] ~= nil then
					if ArenaPlayer[RoleID].Info[InstanceID_Now].Side == "red" then
						if role.IsRoleInStatus(RoleID, 1024) then
							role.CancelRoleStallState(MapID, InstanceID, RoleID)
						end
						role.RoleGotoNewMap(MapID, InstanceID, RoleID, ArenaMap[InstanceID_Now%3].ArenaMapID, ArenaMap[InstanceID_Now%3].RedPoint.x, ArenaMap[InstanceID_Now%3].RedPoint.y, ArenaMap[InstanceID_Now%3].RedPoint.z)
						role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)
					elseif ArenaPlayer[RoleID].Info[InstanceID_Now].Side == "blue" then
						if role.IsRoleInStatus(RoleID, 1024) then
							role.CancelRoleStallState(MapID, InstanceID, RoleID)
						end
						role.RoleGotoNewMap(MapID, InstanceID, RoleID, ArenaMap[InstanceID_Now%3].ArenaMapID, ArenaMap[InstanceID_Now%3].BluePoint.x, ArenaMap[InstanceID_Now%3].BluePoint.y, ArenaMap[InstanceID_Now%3].BluePoint.z)
						role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)
					end
				end
			elseif unit.IsDead(MapID, InstanceID, RoleID) == true then
			    --role.DeletRoleFromArenaTable(RoleID)
				local Pt = Lose_Point_Arena(InstanceID_Now, RoleID)
				--设置该玩家已经离开此斗技场
				ArenaPlayer[RoleID].Now = 0

				role.ModRoleAttValue(MapID, InstanceID, RoleID, 79, -Pt)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 206251)   --您正处于死亡状态，无法传送进入斗技场。您失去了X点荣誉点值。
				msg.AddMsgEvent(MsgID, 9, Pt)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
			else
			    --role.DeletRoleFromArenaTable(RoleID)
				local Pt = Lose_Point_Arena(InstanceID_Now, RoleID)
				--提示玩家已经无法进入竞技场
				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 71, 206252)   --斗技场的比赛已经开始，您无法参与此次比赛。您失去了X点荣誉值。
				msg.AddMsgEvent(MsgID, 9, Pt)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息

				--设置该玩家已经离开此斗技场
				ArenaPlayer[RoleID].Now = 0

				role.ModRoleAttValue(MapID, InstanceID, RoleID, 79, -Pt)
				role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)
			end

		else
		    role.DeletRoleFromArenaTable(RoleID)
			local rongyu = role.GetRoleAttValue(MapID, InstanceID, RoleID, 79)
			local function CountPoint_OnClick(Point)
				if Point <= 100 then
					return 0
				elseif Point <= 250 then
					return 1
				elseif Point <= 500 then
					return 2
				elseif Point <= 750 then
					return 4
				elseif Point > 750 then
					return math.floor((Point - 750)/150) + 6
				else
					return 0
				end
			end
			local Pt = CountPoint_OnClick(rongyu)
			--提示玩家已经无法进入竞技场
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 71, 206252)   --斗技场的比赛已经开始，您无法参与此次比赛。您失去了X点荣誉值。
			msg.AddMsgEvent(MsgID, 9, Pt)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息

			role.ModRoleAttValue(MapID, InstanceID, RoleID, 79, -Pt)
			role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)
		end
	end
end

--玩家进入竞技场
function Arena_OnPlayerEnter(MapID, InstanceID, RoleID)

	--为玩家回满血和蓝，同时刷新所有的技能
	unit.AddBuff(MapID, InstanceID, RoleID, 6936101, RoleID)

	--为玩家显示敌方信息
	ShowEnemyInformation(MapID, InstanceID, RoleID, 1)

	--将玩家加入斗技场的队伍
	LetPlayerJoinTeam(MapID, InstanceID, RoleID)

	--显示竞技场剩余时间
	ShowArenaTime(MapID, InstanceID, RoleID)

	--不会再对玩家进行多余的扣分
	role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)

	--记录人数
	local curTime = tonumber(os.date("%j"))
	if arena_Roles[RoleID] == nil then
		UpdateSeverData(56, 1)
		arena_Roles[RoleID] = curTime
	elseif arena_Roles[RoleID] ~= curTime then
		arena_Roles = {}
		arena_Roles[RoleID] = curTime
		UpdateSeverData(56, 1)
	end
end

--玩家在竞技场中杀人
function Arena_KillPeople(MapID, InstanceID, RoleID, KillerID)
	if ArenaPlayer[RoleID] ~= nil and ArenaPlayer[RoleID].Info[InstanceID] ~= nil and Arena[InstanceID] ~= nil then
	    if unit.IsPlayer(KillerID) then
			if ArenaPlayer[KillerID].Info[InstanceID] ~= nil then

				local MsgID = msg.BeginMsgEvent()
				msg.AddMsgEvent(MsgID, 101, 206253)           --您已经死亡，杀死您的是<color=0xffffff00>XXX<color=0xfffff7e0>！
				msg.AddMsgEvent(MsgID, 2, KillerID)
				msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息

				--显示玩家的X斩
				ArenaPlayer[KillerID].Info[InstanceID].Kill = ArenaPlayer[KillerID].Info[InstanceID].Kill + 1
				if ArenaPlayer[KillerID].Info[InstanceID].Kill > 3 then
				    ArenaPlayer[KillerID].Info[InstanceID].Kill = 3
				end
				role.ShowKillerPictureInArena(KillerID, ArenaPlayer[KillerID].Info[InstanceID].Kill)

				--显示击杀玩家的文字提示
				for i =1,6 do
					if role.IsRoleOnLine(MapID, InstanceID, Arena[InstanceID].Player[i]) == 1 and ArenaPlayer[Arena[InstanceID].Player[i]] ~= nil and ArenaPlayer[Arena[InstanceID].Player[i]].Info[InstanceID] ~= nil and Arena[InstanceID].Player[i] ~= RoleID then
						if ArenaPlayer[KillerID].Info[InstanceID].Side == "red" then
							if ArenaPlayer[Arena[InstanceID].Player[i]].Info[InstanceID].Side == "red" then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 101, 206254)          --<color=0xffffff00>XXX<color=0xfffff7e0>杀死了<color=0xffff0000>XXX<color=0xfffff7e0>
								msg.AddMsgEvent(MsgID, 2, KillerID)
								msg.AddMsgEvent(MsgID, 2, RoleID)
								msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --发送消息
							else
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 101, 206255)          --您的队友<color=0xffff0000>XXX<color=0xfffff7e0>被<color=0xffffff00>XXX<color=0xfffff7e0>杀死了
								msg.AddMsgEvent(MsgID, 2, RoleID)
								msg.AddMsgEvent(MsgID, 2, KillerID)
								msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --发送消息
							end
						else
							if ArenaPlayer[Arena[InstanceID].Player[i]].Info[InstanceID].Side == "red" then
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 101, 206255)          --您的队友<color=0xffff0000>XXX<color=0xfffff7e0>被<color=0xffffff00>XXX<color=0xfffff7e0>杀死了
								msg.AddMsgEvent(MsgID, 2, RoleID)
								msg.AddMsgEvent(MsgID, 2, KillerID)
								msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --发送消息
							else
								local MsgID = msg.BeginMsgEvent()
								msg.AddMsgEvent(MsgID, 101, 206254)          --<color=0xffffff00>XXX<color=0xfffff7e0>杀死了<color=0xffff0000>XXX<color=0xfffff7e0>
								msg.AddMsgEvent(MsgID, 2, KillerID)
								msg.AddMsgEvent(MsgID, 2, RoleID)
								msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --发送消息
							end
						end
					end
				end
			end
		end

		--为玩家显示战况
		ShowEnemyInformation(MapID, InstanceID, RoleID, 2)

		if ArenaPlayer[RoleID] ~= nil and ArenaPlayer[RoleID].Info[InstanceID] ~= nil then
			if ArenaPlayer[RoleID].Info[InstanceID].Side == "red" then
				Arena[InstanceID].RedNum = Arena[InstanceID].RedNum - 1
				if Arena[InstanceID].RedNum <= 0 then
					if Arena[InstanceID].Battle ~= 2 then

						Arena[InstanceID].Battle = 2 --0:未开始，1：开始了，2：结束了
						ShowArenaFinalFrame(MapID, InstanceID)

						local creature1 = map.MapCreateCreature(MapID, InstanceID, 1094003, 6, 1, 9)
						ArenaCreature_Timer[creature1]= {TypeID = 6}      --30秒传送计时
					end
				end
			elseif ArenaPlayer[RoleID].Info[InstanceID].Side == "blue" then
			    Arena[InstanceID].BlueNum = Arena[InstanceID].BlueNum - 1
			    if Arena[InstanceID].BlueNum <= 0 then
					if Arena[InstanceID].Battle ~= 2 then

						Arena[InstanceID].Battle = 2 --0:未开始，1：开始了，2：结束了
						ShowArenaFinalFrame(MapID, InstanceID)

						local creature1 = map.MapCreateCreature(MapID, InstanceID, 1094003, 6, 1, 9)
						ArenaCreature_Timer[creature1]= {TypeID = 6}      --30秒传送计时
					end
				end
			end
		end
	end
end

--玩家离开竞技场

function Arena_LeaveMap(MapID, InstanceID, RoleID)

	local TeamID_Old = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	if TeamID_Old ~= nil and TeamID_Old ~= 4294967295 then
		role.LetRoleLeaveTeam(TeamID_Old, RoleID)
	end

	role.DeletRoleFromArenaTable(RoleID)
	unit.UnsetState(MapID, InstanceID, RoleID, 10)
	unit.UnsetState(MapID, InstanceID, RoleID, 11)

	if ArenaPlayer[RoleID] ~= nil and ArenaPlayer[RoleID].Info[InstanceID] ~= nil and Arena[InstanceID] ~= nil then
		--如果战斗没有结束
		if Arena[InstanceID].Battle ~= 2 then
		    --role.DeletRoleFromArenaTable(RoleID)
			local TotleTimes_Arena = role.GetRoleScriptData(RoleID, 1, RoleDataType["TotleTimes_Arena"])
			role.SetRoleScriptData(RoleID, 1, RoleDataType["TotleTimes_Arena"], TotleTimes_Arena + 1)
			role.ModRoleAttValue(MapID, InstanceID, RoleID, 79, -Lose_Point_Arena(InstanceID, RoleID))
			ArenaPlayer[RoleID].Now = 0
			role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)
			for i=1,6 do
				if Arena[InstanceID].Player[i] ~= RoleID then
				    local Map_ID,Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
					if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID and role.IsRoleOnLine(MapID, InstanceID, Arena[InstanceID].Player[i]) == 1 then
						--提示玩家
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 26, 206256)   --XXX离开了竞技场
						msg.AddMsgEvent(MsgID, 2, RoleID)
						msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --发送消息
						local MsgID = msg.BeginMsgEvent()
						msg.AddMsgEvent(MsgID, 13, 0)
						msg.AddMsgEvent(MsgID, 1, 206256)   --XXX离开了竞技场
						msg.AddMsgEvent(MsgID, 2, RoleID)
						msg.DispatchRoleMsgEvent(Arena[InstanceID].Player[i], MsgID)     --发送消息
				    end
				end
			end

			--为玩家显示战况
			ShowEnemyInformation(MapID, InstanceID, RoleID, 3)

			--尝试结算
			if ArenaPlayer[RoleID].Info[InstanceID].Side == "red" then
			    Arena[InstanceID].RedNum = 0
			    for i = 1,3 do
				    local Map_ID,Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
					if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID and role.IsRoleOnLine(MapID, InstanceID, Arena[InstanceID].Player[i]) == 1 and unit.IsDead(MapID, InstanceID, Arena[InstanceID].Player[i]) ~= true and Arena[InstanceID].Player[i] ~= RoleID then
                        Arena[InstanceID].RedNum = Arena[InstanceID].RedNum + 1
					end
				end
				if Arena[InstanceID].RedNum <= 0 then
					if Arena[InstanceID].Battle == 1 then

						Arena[InstanceID].Battle = 2 --0:未开始，1：开始了，2：结束了
						ShowArenaFinalFrame(MapID, InstanceID)

						local creature1 = map.MapCreateCreature(MapID, InstanceID, 1094003, 6, 1, 9)
						ArenaCreature_Timer[creature1]= {TypeID = 6}      --30秒传送计时
					end
				end
			elseif ArenaPlayer[RoleID].Info[InstanceID].Side == "blue" then
			    Arena[InstanceID].BlueNum = 0
				for i = 4,6 do
				    local Map_ID,Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
					if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID and role.IsRoleOnLine(MapID, InstanceID, Arena[InstanceID].Player[i]) == 1 and unit.IsDead(MapID, InstanceID, Arena[InstanceID].Player[i]) ~= true and Arena[InstanceID].Player[i] ~= RoleID then
                        Arena[InstanceID].BlueNum = Arena[InstanceID].BlueNum + 1
					end
				end
			    if Arena[InstanceID].BlueNum <= 0 then
					if Arena[InstanceID].Battle == 1 then

						Arena[InstanceID].Battle = 2 --0:未开始，1：开始了，2：结束了
						ShowArenaFinalFrame(MapID, InstanceID)

						local creature1 = map.MapCreateCreature(MapID, InstanceID, 1094003, 6, 1, 9)
						ArenaCreature_Timer[creature1]= {TypeID = 6}      --30秒传送计时
					end
				end
			end
		end
	end
end

--玩家复活脚本
function Arena_RoloRevive(MapID, InstanceID, RoleID, Type, ReviveHP, ReviveMP, x, y, z, RebornMapID)

    ReviveHP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 8)
	ReviveMP = role.GetRoleAttValue(MapID, InstanceID, RoleID, 10)
    role.DeletRoleFromArenaTable(RoleID)
	local k = math.random(0,2)
	return ReviveHP, ReviveMP,ArenaMap[k].ExitPoint.x, ArenaMap[k].ExitPoint.y*51,ArenaMap[k].ExitPoint.z, 3424073512

end

--玩家请求进入房间
function JoinArenaRoom(MapID, InstanceID, Type, RoomID, RoleID)
    local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"])
	if ArenaPlayer[RoleID] ~= nil then
		--获取将要进入的斗技场的InstanceID
		local InstanceID_Now = GetNowArenaInstanceID(RoleID)
		if InstanceID_Now ~= 0 then
		    ArenaPlayer[RoleID].Now = 0
		end
	end
    if k > 0 then
	    role.ModRoleAttValue(MapID, InstanceID, RoleID, 79, -k)
	    role.SetRoleScriptData(RoleID, 1, RoleDataType["Point_Arena"], 0)
	end
	if act.GetActIsStart(31) ~= true and act.GetActIsStart(32) ~= true then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206257)   --随机竞技场活动尚未开始，无法执行该操作
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		return false
	elseif MapID == 3424073512 then
		return 1
	else
		return false
	end
end

-- 竞技场的敌我关系判断 -- 除同色以外全是敌人
function Arena_FriendEnemy(MapID, InstanceID, SrcID, TargetID)
    local a,b = -1,-1
	-- 都是玩家
	if Arena[InstanceID] ~= nil and unit.IsPlayer(SrcID) and unit.IsPlayer(TargetID) then
        for i=1, 6 do
		    if Arena[InstanceID].Player[i] == SrcID then
			    if i <= 3 then
			        a = 1
				    break
				else
				    a = 0
					break
				end
			end
		end

		for i=1, 6 do
		    if Arena[InstanceID].Player[i] == TargetID then
			    if i <= 3 then
			        b = 1
				    break
				else
				    b = 0
					break
				end
			end
		end

		if a ~= b and a~= -1 and b ~= -1 then
			return false, true, false, true
		elseif a == b and a~= -1 then
		    return true, false, false, true
		end
	end

	return false, false, false, false
end



--注册该活动的相关事件
aux.RegisterMapEvent("a01", 2, "Arena_OnPlayerEnter")
aux.RegisterMapEvent("a02", 2, "Arena_OnPlayerEnter")
aux.RegisterMapEvent("a03", 2, "Arena_OnPlayerEnter")
aux.RegisterActEvent(31, 8, "Arena_OnClick")
aux.RegisterActEvent(32, 8, "Arena_OnClick")
aux.RegisterWorldEvent(1, "Arena_ArenaOnStart")
aux.RegisterMapEvent("a01", 5, "Arena_KillPeople")
aux.RegisterMapEvent("a02", 5, "Arena_KillPeople")
aux.RegisterMapEvent("a03", 5, "Arena_KillPeople")
aux.RegisterMapEvent("a01", 3, "Arena_LeaveMap")
aux.RegisterMapEvent("a02", 3, "Arena_LeaveMap")
aux.RegisterMapEvent("a03", 3, "Arena_LeaveMap")
aux.RegisterMapEvent("a01", 11, "Arena_RoloRevive")
aux.RegisterMapEvent("a02", 11, "Arena_RoloRevive")
aux.RegisterMapEvent("a03", 11, "Arena_RoloRevive")
aux.RegisterRoleEvent(36, "JoinArenaRoom")
aux.RegisterRoleEvent(37, "JoinArenaRoom")
aux.RegisterMapEvent("a01", 16, "Arena_FriendEnemy")
aux.RegisterMapEvent("a02", 16, "Arena_FriendEnemy")
aux.RegisterMapEvent("a03", 16, "Arena_FriendEnemy")
