--为竞技场分配地图和副本
function GetArenaMapID()

	ArenaMapNum = ArenaMapNum + 1

	if ArenaMapNum >= 1000 then
	    ArenaMapNum = 1
	end

	map.CreateInstance(ArenaMap[ArenaMapNum % 3].ArenaMapID, ArenaMapNum)

	return ArenaMap[ArenaMapNum % 3].ArenaMapID, ArenaMapNum

end


--为玩家随机分阵营
function PlayerRandom_Arena(RoleID1,RoleID2,RoleID3,RoleID4,RoleID5,RoleID6)
    local table_Role = {1,2,3,4,5,6}
	local table_Role1 = {}
	for i = 1,6 do
		local index = math.random(7-i)
		table_Role1[i] = table_Role[index]
		table.remove(table_Role, index)
	end

	local iRole = {}  --玩家ID的索引
	iRole[1] = RoleID1
	iRole[2] = RoleID2
	iRole[3] = RoleID3
	iRole[4] = RoleID4
	iRole[5] = RoleID5
	iRole[6] = RoleID6

	return iRole[table_Role1[1]], iRole[table_Role1[2]], iRole[table_Role1[3]], iRole[table_Role1[4]], iRole[table_Role1[5]], iRole[table_Role1[6]]
end

--定义竞技场相关的参数
function DefineValue_Arena (InstanceID, RoomID, RoleID1, RoleID2, RoleID3, RoleID4, RoleID5, RoleID6)

	local Hour 		= tonumber(os.date("%H"))
	local Min  		= tonumber(os.date("%M"))
	local Second	= tonumber(os.date("%S"))
	--地图相关信息      房间序列号      竞技场开始时间                            战斗状态    红方人数    蓝方人数     玩家ID
    Arena[InstanceID] = {Room = RoomID, OpenTime = Hour*10000 + Min*100 + Second, Battle = 0, RedNum = 0, BlueNum = 0, Player = {RoleID1, RoleID2, RoleID3, RoleID4, RoleID5, RoleID6}}                --记录该地图对应的房间号

	--个人相关信息
	local RoleID = {}
	RoleID[1] = RoleID1
	RoleID[2] = RoleID2
	RoleID[3] = RoleID3
	RoleID[4] = RoleID4
	RoleID[5] = RoleID5
	RoleID[6] = RoleID6

    local TotalPoint_Red = 0
	local TotalPoint_Blue = 0
	for i = 1,6 do
	    local MapID, Instance_ID = role.GetRoleMapID(RoleID[i])
	    if i <= 3 then
		    if ArenaPlayer[RoleID[i]] == nil then
				ArenaPlayer[RoleID[i]] = {Now=0,Info={}}
			end
			ArenaPlayer[RoleID[i]].Now = InstanceID
		              --                          阵营          杀人数    自己荣誉值                                                           敌方荣誉值平均值     获得荣誉          结果     个人胜率
	        ArenaPlayer[RoleID[i]].Info[InstanceID] = {Side = "red", Kill = 0, RankPoint = role.GetRoleAttValue(MapID, Instance_ID, RoleID[i], 79), EnemyPoint = 0, GetRankPoint = 0, Win = 0, WinTimes = role.GetRoleScriptData(RoleID[i], 1, RoleDataType["WinTimes_Arena"]), TotleTimes = role.GetRoleScriptData(RoleID[i], 1, RoleDataType["TotleTimes_Arena"])}      --红方
            TotalPoint_Red = TotalPoint_Red + ArenaPlayer[RoleID[i]].Info[InstanceID].RankPoint
		else
		    if ArenaPlayer[RoleID[i]] == nil then
				ArenaPlayer[RoleID[i]] = {Now=0,Info={}}
			end
			ArenaPlayer[RoleID[i]].Now = InstanceID
		    ArenaPlayer[RoleID[i]].Info[InstanceID] = {Side = "blue", Kill = 0, RankPoint = role.GetRoleAttValue(MapID, Instance_ID, RoleID[i], 79), EnemyPoint = 0, GetRankPoint = 0, Win = 0, WinTimes = role.GetRoleScriptData(RoleID[i], 1, RoleDataType["WinTimes_Arena"]), TotleTimes = role.GetRoleScriptData(RoleID[i], 1, RoleDataType["TotleTimes_Arena"])}      --蓝方
            TotalPoint_Blue = TotalPoint_Blue + ArenaPlayer[RoleID[i]].Info[InstanceID].RankPoint
		end
	end
	for i = 1,6 do
	    if i <= 3 then
	        ArenaPlayer[RoleID[i]].Info[InstanceID].EnemyPoint = TotalPoint_Blue
		else
            ArenaPlayer[RoleID[i]].Info[InstanceID].EnemyPoint = TotalPoint_Red
		end
	end

end

--删除相关树值，释放内存
function RemoveValue_Arena (InstanceID)
	if Arena[InstanceID] ~= nil then
		for i=1,6 do
			if ArenaPlayer[Arena[InstanceID].Player[i]] ~= nil then
				ArenaPlayer[Arena[InstanceID].Player[i]].Info[InstanceID] = nil
			end
			ArenaPlayer[Arena[InstanceID].Player[i]].Now = 0
			if ArenaPlayer[Arena[InstanceID].Player[i]].Info == nil then
			    ArenaPlayer[Arena[InstanceID].Player[i]] = nil
			end
		end
		Arena[InstanceID] = nil
	end
end

--获取将要进入的斗技场的InstanceID
function GetNowArenaInstanceID(RoleID)
	local InstanceID_Now = 0
	--[[for k,v in pairs(ArenaPlayer[RoleID]) do
		if v.Now == 1 then
			InstanceID_Now = k
			break
		end
	end]]
	if ArenaPlayer[RoleID].Now ~= nil then
	    InstanceID_Now = ArenaPlayer[RoleID].Now
	end
	return InstanceID_Now
end

--玩家扣分和得分的计算
function Win_Point_Arena(InstanceID, RoleID)
	if ArenaPlayer[RoleID] ~= nil and ArenaPlayer[RoleID].Info[InstanceID] ~= nil then
		local k = math.floor((ArenaPlayer[RoleID].Info[InstanceID].EnemyPoint/3-ArenaPlayer[RoleID].Info[InstanceID].RankPoint)/30) + 10
	    if k >= 3 then
			return k
		else
		    return 3
		end
	else
	    return 0
	end
end

function Lose_Point_Arena(InstanceID, RoleID)
	if ArenaPlayer[RoleID] ~= nil and ArenaPlayer[RoleID].Info[InstanceID] ~= nil then
	    if ArenaPlayer[RoleID].Info[InstanceID].RankPoint <= 50 then
		    return 0
		elseif ArenaPlayer[RoleID].Info[InstanceID].RankPoint <= 100 then
		    return 3
		elseif ArenaPlayer[RoleID].Info[InstanceID].RankPoint <= 150 then
		    return 5
		elseif ArenaPlayer[RoleID].Info[InstanceID].RankPoint <= 250 then
		    return 8
		elseif ArenaPlayer[RoleID].Info[InstanceID].RankPoint > 250 then
		    return math.floor((ArenaPlayer[RoleID].Info[InstanceID].RankPoint - 250)/100)*3 + 8
		else
		    return 0
		end
	else
	    return 0
	end
end

--玩家进入竞技场后的入队操作
function LetPlayerJoinTeam(MapID, InstanceID, RoleID)
    local TeamID_Old = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
	if TeamID_Old ~= nil and TeamID_Old ~= 4294967295 then
		role.LetRoleLeaveTeam(TeamID_Old, RoleID)
	end
	if Arena[InstanceID] ~= nil then
		if ArenaPlayer[RoleID].Info[InstanceID] ~= nil then
			if ArenaPlayer[RoleID].Info[InstanceID].Side == "red" then
				role.SetForceInArena(MapID, InstanceID, RoleID, 1)
				if Arena[InstanceID].RedNum == 0 then
					Arena[InstanceID].RedNum = Arena[InstanceID].RedNum + 1
				elseif Arena[InstanceID].RedNum == 1 then
					Arena[InstanceID].RedNum = Arena[InstanceID].RedNum + 1
					local RoleID2 = 0
					for i= 1,3 do
					    local Map_ID, Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
					    if Arena[InstanceID].Player[i] ~= RoleID and Map_ID == MapID and InstanceID == Instance_ID then
					       RoleID2 = Arena[InstanceID].Player[i]
						   break
						end
					end
					if RoleID2 ~= 0 then
						role.CreatANewTeam(RoleID, RoleID2)
					end
				elseif Arena[InstanceID].RedNum == 2 then
				    Arena[InstanceID].RedNum = Arena[InstanceID].RedNum + 1
				    local RoleID2 = 0
				    for i= 1,3 do
					    if Arena[InstanceID].Player[i] ~= RoleID then
					       RoleID2 = Arena[InstanceID].Player[i]
						   break
						end
					end
					if RoleID2 ~= 0 then
						local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID2)
						if TeamID ~= nil and TeamID ~= 4294967295 then
							role.LetRoleJoinTeam(TeamID, RoleID)
						end
					end
				end
			else
				role.SetForceInArena(MapID, InstanceID, RoleID, 0)
				if Arena[InstanceID].BlueNum == 0 then
					Arena[InstanceID].BlueNum = Arena[InstanceID].BlueNum + 1
				elseif Arena[InstanceID].BlueNum == 1 then
					Arena[InstanceID].BlueNum = Arena[InstanceID].BlueNum + 1
					local RoleID2 = 0
					for i= 4,6 do
					    local Map_ID, Instance_ID = role.GetRoleMapID(Arena[InstanceID].Player[i])
					    if Arena[InstanceID].Player[i] ~= RoleID and Map_ID == MapID and InstanceID == Instance_ID then
					       RoleID2 = Arena[InstanceID].Player[i]
						   break
						end
					end
					if RoleID2 ~= 0 then
						role.CreatANewTeam(RoleID, RoleID2)
					end
				elseif Arena[InstanceID].BlueNum == 2 then
				    Arena[InstanceID].BlueNum = Arena[InstanceID].BlueNum + 1
				    local RoleID2 = 0
				    for i= 4,6 do
					    if Arena[InstanceID].Player[i] ~= RoleID then
					       RoleID2 = Arena[InstanceID].Player[i]
						   break
						end
					end
					if RoleID2 ~= 0 then
						local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID2)
						if TeamID ~= nil and TeamID ~= 4294967295 then
							role.LetRoleJoinTeam(TeamID, RoleID)
						end
					end
				end
			end
		end
	end
end

--为玩家显示战况。Condition:1进入2死亡3离开
function ShowEnemyInformation(MapID, InstanceID, RoleID, Condition)
    if Arena[InstanceID] ~= nil then
		if ArenaPlayer[RoleID].Info[InstanceID] ~= nil then
			if Condition == 1 then
			    local EnemyID = {}
                if ArenaPlayer[RoleID].Info[InstanceID].Side == "red" then
					EnemyID[1] = Arena[InstanceID].Player[4]
					EnemyID[2] = Arena[InstanceID].Player[5]
					EnemyID[3] = Arena[InstanceID].Player[6]
				else
				    EnemyID[1] = Arena[InstanceID].Player[1]
					EnemyID[2] = Arena[InstanceID].Player[2]
					EnemyID[3] = Arena[InstanceID].Player[3]
				end
				local EState = {}		--1离开，2死亡，3活着
				for i = 1,3 do
					local Emap,EInstance = role.GetRoleMapID(EnemyID[i])
					if (Emap == nil or Emap ~= MapID) or (EInstance == nil or EInstance ~= InstanceID) then
						EState[i] = 1
					elseif unit.IsDead(MapID, InstanceID, EnemyID[i]) then
						EState[i] = 2
					else
						EState[i] = 3
						role.ShowAlivePeopleNameInArena(EnemyID[i], RoleID, 3)
					end
					role.ShowAlivePeopleNameInArena(RoleID, EnemyID[i], EState[i])
				end
			elseif Condition == 2 then
                local EnemyID = {}
                if ArenaPlayer[RoleID].Info[InstanceID].Side == "red" then
					EnemyID[1] = Arena[InstanceID].Player[4]
					EnemyID[2] = Arena[InstanceID].Player[5]
					EnemyID[3] = Arena[InstanceID].Player[6]
				else
				    EnemyID[1] = Arena[InstanceID].Player[1]
					EnemyID[2] = Arena[InstanceID].Player[2]
					EnemyID[3] = Arena[InstanceID].Player[3]
				end
				for i = 1,3 do
					local Emap,EInstance = role.GetRoleMapID(EnemyID[i])
					if (Emap == nil or Emap ~= MapID) or (EInstance == nil or EInstance ~= InstanceID) then
					else
						role.ShowAlivePeopleNameInArena(EnemyID[i], RoleID, 2)
					end
				end
			elseif Condition == 3 then
                local EnemyID = {}
                if ArenaPlayer[RoleID].Info[InstanceID].Side == "red" then
					EnemyID[1] = Arena[InstanceID].Player[4]
					EnemyID[2] = Arena[InstanceID].Player[5]
					EnemyID[3] = Arena[InstanceID].Player[6]
				else
				    EnemyID[1] = Arena[InstanceID].Player[1]
					EnemyID[2] = Arena[InstanceID].Player[2]
					EnemyID[3] = Arena[InstanceID].Player[3]
				end
				for i = 1,3 do
					local Emap,EInstance = role.GetRoleMapID(EnemyID[i])
					if (Emap == nil or Emap ~= MapID) or (EInstance == nil or EInstance ~= InstanceID) then
					else
						role.ShowAlivePeopleNameInArena(EnemyID[i], RoleID, 1)
					end
				end
			end
		end
	end
end

--显示竞技场剩余时间
function ShowArenaTime(MapID, InstanceID, RoleID)

    local leftSec = 660

	local Min  		= tonumber(os.date("%M"))
	local Second	= tonumber(os.date("%S"))
	local Min_open = math.floor((Arena[InstanceID].OpenTime % 10000)/100)
	local Second_open = Arena[InstanceID].OpenTime % 100

	if Second == Second_open and Min == Min_open then
	    leftSec = 660
	elseif Second <= Second_open then
	    leftSec = 660 - (Second + 60 - Second_open)
	else
	    leftSec = 660 - (Second - Second_open)
	end

	--同步客户端时间
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 16,	28)								--
	msg.AddMsgEvent(MsgID, 16,	leftSec)							--剩余时间
	msg.DispatchRoleMsgEvent(RoleID, MsgID)


end

--结算分数并且给所有还在竞技场中的玩家算分，组成字符串，然后发送给服务器
function ShowArenaFinalFrame(MapID, InstanceID)
--如果此时一方人数为0，则结束战斗，Arena[InstanceID].Battle = 0没奖励。
	local acount = {}
	acount[1] = {-1,-1,-1,-1,-1,-1} --名字 荣誉 荣誉增量 击杀数 胜率 结果（1赢2输3完胜4平局）
	acount[2] = {-1,-1,-1,-1,-1,-1}
	acount[3] = {-1,-1,-1,-1,-1,-1}
	acount[4] = {-1,-1,-1,-1,-1,-1}
	acount[5] = {-1,-1,-1,-1,-1,-1}
	acount[6] = {-1,-1,-1,-1,-1,-1}

	for i=1,6 do
		acount[i][1] = Arena[InstanceID].Player[i]
	end


	if Arena[InstanceID].RedNum == 0 or Arena[InstanceID].RedNum < Arena[InstanceID].BlueNum then
		acount[1][6] = 2
		acount[2][6] = 2
		acount[3][6] = 2
		if Arena[InstanceID].BlueNum >= 3 then
			acount[4][6] = 3
			acount[5][6] = 3
			acount[6][6] = 3
		else
			acount[4][6] = 1
			acount[5][6] = 1
			acount[6][6] = 1
		end
		ArenaPlayer[acount[4][1]].Info[InstanceID].Win = 1
		ArenaPlayer[acount[5][1]].Info[InstanceID].Win = 1
		ArenaPlayer[acount[6][1]].Info[InstanceID].Win = 1
	elseif Arena[InstanceID].BlueNum == 0 or Arena[InstanceID].BlueNum < Arena[InstanceID].RedNum then
		acount[4][6] = 2
		acount[5][6] = 2
		acount[6][6] = 2
		if Arena[InstanceID].BlueNum >= 3 then
			acount[1][6] = 3
			acount[2][6] = 3
			acount[3][6] = 3
		else
			acount[1][6] = 1
			acount[2][6] = 1
			acount[3][6] = 1
		end
		ArenaPlayer[acount[1][1]].Info[InstanceID].Win = 1
		ArenaPlayer[acount[2][1]].Info[InstanceID].Win = 1
		ArenaPlayer[acount[3][1]].Info[InstanceID].Win = 1
	elseif Arena[InstanceID].BlueNum == Arena[InstanceID].RedNum then
		acount[1][6] = 4
		acount[2][6] = 4
		acount[3][6] = 4
		acount[4][6] = 4
		acount[5][6] = 4
		acount[6][6] = 4
	end

	for i=1,6 do

		acount[i][2] = ArenaPlayer[acount[i][1]].Info[InstanceID].RankPoint
		acount[i][4] = ArenaPlayer[acount[i][1]].Info[InstanceID].Kill

		local Map_ID,Instance_ID = role.GetRoleMapID(acount[i][1])
		if (Map_ID ==nil or Instance_ID == nil) or (Map_ID ~= MapID or Instance_ID ~= InstanceID) then
		    acount[i][6] = 2
		end
		if acount[i][6] == 2 then
		    acount[i][3] = -Lose_Point_Arena(InstanceID, acount[i][1])

		elseif acount[i][6] ~= 4 then
		    acount[i][3] = Win_Point_Arena(InstanceID, acount[i][1])
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 101, 206283)          --<color=0xffffff00>胜利方玩家开启宝箱后，将会为所有队友添加奖励道具<color=0xfffff7e0>
			msg.DispatchRoleMsgEvent(acount[i][1], MsgID)     --发送消息
		else
		    acount[i][3] = 1
		end

		if acount[i][6] == 1 or  acount[i][6] == 3 then
			acount[i][5] = math.floor((ArenaPlayer[acount[i][1]].Info[InstanceID].WinTimes + 1 ) * 10000 / (ArenaPlayer[acount[i][1]].Info[InstanceID].TotleTimes+1))
		else
            acount[i][5] = math.floor((ArenaPlayer[acount[i][1]].Info[InstanceID].WinTimes) * 10000 / (ArenaPlayer[acount[i][1]].Info[InstanceID].TotleTimes+1))
		end

		--只有当前还在场景中的玩家才会扣分和加分
		if Map_ID ~=nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID then
			role.ModRoleAttValue(MapID, InstanceID, acount[i][1], 79, acount[i][3])
	   end
	end


	for q= 1,6 do

	    local Map_ID,Instance_ID = role.GetRoleMapID(acount[q][1])
		if Map_ID ~= nil and Instance_ID ~= nil and Map_ID == MapID and Instance_ID == InstanceID and role.IsRoleOnLine(MapID, InstanceID, acount[q][1]) == 1 then

			--计算玩家的最终分数，并将斗技场相关信息记录在数据库
			SetScript_Arena(acount[q][1],acount[q][6])

			--无敌
			unit.AddBuff(MapID, InstanceID, acount[q][1], 6936201, acount[q][1])

			--组合字符串
			local acount_1 = {}
			for i = 1,6 do
			    acount_1[i] = {}
			    for j = 1,6 do
					acount_1[i][j] = acount[i][j]
				end
			end

			local temp = {}
			for i = 1,6 do
				temp[i] = acount_1[1][i]
				acount_1[1][i] = acount_1[q][i]
				acount_1[q][i] = temp[i]
			end

			local char = nil
			for i=1,6 do
				for j =1,5 do
					if i == 1 and j == 1 then
						char = tostring(acount_1[1][1])
					else
						char = char..","..tostring(acount_1[i][j])
					end
				end
			end
			char = char..","..tostring(acount_1[1][6])
			local TimesToday = role.GetRoleScriptData(acount_1[1][1], 1, RoleDataType["TimesToday_Arena"])
			if TimesToday > 6 then
			    TimesToday = 6
			end
			char = char..","..tostring(TimesToday)
			role.ShowArenaEndFrame(acount_1[1][1], char)
		end
	end
end

--计算玩家的最终分数，并将斗技场相关信息记录在数据库
function SetScript_Arena(RoleID,value) -- 0离开1赢2输3完胜4平局

    local Date = role.GetRoleScriptData(RoleID, 1, RoleDataType["Date_Arena"])
	local CurTime = tonumber(os.date("%j"))

	if Date ~= CurTime then
		role.SetRoleScriptData(RoleID, 1, RoleDataType["Date_Arena"], CurTime)
		role.SetRoleScriptData(RoleID, 1, RoleDataType["TimesToday_Arena"], 0)
	end

	if value == 1 or value == 3 then
		local TotleTimes_Arena = role.GetRoleScriptData(RoleID, 1, RoleDataType["TotleTimes_Arena"])
		role.SetRoleScriptData(RoleID, 1, RoleDataType["TotleTimes_Arena"], TotleTimes_Arena + 1)

		local WinTimes_Arena = role.GetRoleScriptData(RoleID, 1, RoleDataType["WinTimes_Arena"])
		role.SetRoleScriptData(RoleID, 1, RoleDataType["WinTimes_Arena"], WinTimes_Arena + 1)

		local TimesToday = role.GetRoleScriptData(RoleID, 1, RoleDataType["TimesToday_Arena"])
		TimesToday = TimesToday + 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["TimesToday_Arena"], TimesToday)
    elseif value == 0 or value == 2 or value == 4 then
	    local TotleTimes_Arena = role.GetRoleScriptData(RoleID, 1, RoleDataType["TotleTimes_Arena"])
		role.SetRoleScriptData(RoleID, 1, RoleDataType["TotleTimes_Arena"], TotleTimes_Arena + 1)

		local TimesToday = role.GetRoleScriptData(RoleID, 1, RoleDataType["TimesToday_Arena"])
		TimesToday = TimesToday + 1
		role.SetRoleScriptData(RoleID, 1, RoleDataType["TimesToday_Arena"], TimesToday)
	end
end
--为玩家选择需要给予的奖励
function ArenaBox_GetItem(MapID, InstanceID, TargetTypeID, RoleID)
    if role.GetBagFreeSize(RoleID) >= 1 then
		if TargetTypeID == 5512004 then
		    role.AddRoleItem(MapID, InstanceID, RoleID, 4600001, 1, -1, 8, 420)
			if role.GetBagFreeSize(RoleID) >= 1 and math.random(250) == 57 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 4631001, 1, -1, 8, 420)
			end
		elseif TargetTypeID == 5512005 then
		    local k = math.random(10)
			if k <= 5 then
				role.AddRoleItem(MapID, InstanceID, RoleID, 4600001, 2, -1, 8, 420)
		    elseif k <= 8 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 4600001, 3, -1, 8, 420)
			elseif k <= 9 then
			    role.AddRoleItem(MapID, InstanceID, RoleID, 4090015, 1, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4600001, 1, -1, 8, 420)
			else
			    role.AddRoleItem(MapID, InstanceID, RoleID, 4600002, 1, -1, 8, 420)
				role.AddRoleItem(MapID, InstanceID, RoleID, 4600001, 1, -1, 8, 420)
			end
		end
	end
end
