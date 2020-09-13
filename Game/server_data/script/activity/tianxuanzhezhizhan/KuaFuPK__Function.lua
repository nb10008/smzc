function CreatKuaFuPKRoleObj(RoleID)
    if KuaFuPK_Role[RoleID] == nil then
	    local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleID)
	    local rolelevel = role.GetRoleLevel(Target_MapID, Target_InstanceID, RoleID)
	    if rolelevel >= 80 then
            KuaFuPK_Roletotal = KuaFuPK_Roletotal + 1
			KuaFuPK_Role[RoleID] = {RoleID = RoleID, Time = 0, Side = "None", InstanceID = 0,State = "OutSide",Kill = 0,die = 0}
	        local Guild = guild.GetRoleGuildID(RoleID)
			local FamilyID = role.GetRoleFamilyID(Target_MapID, Target_InstanceID, RoleID)
			if Guild ~= nil and Guild ~= 4294967295 then
			    KuaFuPK_Role[RoleID].Guild = Guild
			else
			    KuaFuPK_Role[RoleID].Guild = 0
			end
			if FamilyID ~= nil and FamilyID ~= 4294967295 then
			    KuaFuPK_Role[RoleID].FamilyID = FamilyID
			else
			    KuaFuPK_Role[RoleID].FamilyID = 0
			end
			KuaFuPK_Role[RoleID].ShowKuaFuPKTime = ShowKuaFuPKTime
			KuaFuPK_Role[RoleID].GetAttBuff_KuaFuPK = GetAttBuff_KuaFuPK
			KuaFuPK_Role[RoleID].WuDi_KuaFuPK = WuDi_KuaFuPK
			KuaFuPK_Role[RoleID].SetColor_KuaFuPK = SetColor_KuaFuPK
			KuaFuPK_Role[RoleID].SendInfo_KuaFuPK = SendInfo_KuaFuPK
			KuaFuPK_Role[RoleID].PlayerLeaveKuaFuPK = PlayerLeaveKuaFuPK
			KuaFuPK_Role[RoleID].PlayerEnterKuaFuPK = PlayerEnterKuaFuPK
			KuaFuPK_Role[RoleID].GetKillExp_KuaFuPK = GetKillExp_KuaFuPK
			local guildID = guild.GetRoleGuildID(RoleID)
			local temp = false
			if guildID ~= nil and GuildNum[guildID] == nil then
				GuildNum[guildID] = {guildID = guildID, lastNum = 20,curNum = 0,tempnum = 0, tag = true, instanceID = 0,side = 0,score = 0 , mark = 0}
			end
			local num =#GuildNum
			if num >= 1 then
				for k,v in pairs(GuildNum) do
					if guildID ~=nil and guildID == v then
						GuildNum[guildID].curNum = GuildNum[guildID].curNum + 1
						temp = true
					end
				end
				if temp == false then
					GuildNum[guildID].guildID = guildID
					GuildNum[guildID].curNum = 1
					GuildNum[guildID].tag = true
					GuildNum[guildID].instanceID = InstanceID
					GuildNum[guildID].mark = num
				end
			else
				GuildNum[guildID].guildID = guildID
				GuildNum[guildID].curNum = 1
				GuildNum[guildID].tag = true
				GuildNum[guildID].instanceID = InstanceID
				GuildNum[guildID].mark = 0
			end
		end
	end
end

function CreatKuaFuPKSideObj()
    local k = #KuaFuPK_Side + 1
	KuaFuPK_Side[k] = {Player={},InSidePlayer={},Num = 0,Point = 0}
	if k%2 == 1 then
	    KuaFuPK_Side[k].Side = "Red"
	else
	    KuaFuPK_Side[k].Side = "Blue"
	end
	KuaFuPK_Side[k].PlayerJoinKuaFuPKSide = PlayerJoinKuaFuPKSide
	return KuaFuPK_Side[k]
end

function PlayerJoinKuaFuPKSide(Obj,PlayerID)
    table.insert(Obj.Player,PlayerID)
	if KuaFuPK_Roletotal > 0 then
		KuaFuPK_Role[PlayerID].Side = Obj.Side
	end
end

function CreatKuaFuPKAreaObj(InstanceID)
    local k = #KuaFuPK_Area + 1
	KuaFuPK_Area[k] = {area={},InstanceID=InstanceID}
	local buff = {}
	buff[5610077] = 6109301
	buff[5610078] = 6109101
	buff[5610079] = 6109201
	for i = 1,9 do
		local id = 5610076+math.random(3)
		local cre = map.MapCreateCreature(S59_MapID, InstanceID, id, S59_ItemPoint[S59_Area[i]].x, S59_ItemPoint[S59_Area[i]].y, S59_ItemPoint[S59_Area[i]].z)
		KuaFuPK_Area[k].area[S59_Area[i]] = {}
		KuaFuPK_Area[k].area[S59_Area[i]].CreID = cre
		KuaFuPK_Area[k].area[S59_Area[i]].TypeID = id
		KuaFuPK_Area[k].area[S59_Area[i]].BuffID = buff[id]
	end
	KuaFuPK_Area[k].IntoKuaFuPKArea = IntoKuaFuPKArea
	return KuaFuPK_Area[k]
end

function CreatKuaFuPKBattleObj(RedPlayer,BluePlayer)
	local k = 0
	if KuaFuPK_Battle[100] ==nil then
		k=100
	else
		k=101
	end
    map.CreateInstance(S59_MapID, k)
	KuaFuPK_Battle[k] = {RedPlayer = RedPlayer,BluePlayer = BluePlayer,InstanceID=k}
	for k1,v in pairs(RedPlayer.Player) do
	    KuaFuPK_Role[v].InstanceID = k
	end
	for k1,v in pairs(BluePlayer.Player) do
	    KuaFuPK_Role[v].InstanceID = k
	end
    KuaFuPK_Battle[k].Area = CreatKuaFuPKAreaObj(k)
	KuaFuPK_Battle[k].GetKuaFuPKAreaObjFromBattle = GetKuaFuPKAreaObjFromBattle
	KuaFuPK_Battle[k].SendAllInfo_KuaFuPK = SendAllInfo_KuaFuPK
	KuaFuPK_Battle[k].SetPlayerPoint_KuaFuPK = SetPlayerPoint_KuaFuPK
	KuaFuPK_Battle[k].SetPlayerFinalPoint_KuaFuPK = SetPlayerFinalPoint_KuaFuPK
    KuaFuPK_Battle[k].ResetInSidePlayer_KuaFuPK = ResetInSidePlayer_KuaFuPK
end



function IntoKuaFuPKArea(AreaObj,AreaID,RoleID)
	if AreaObj.area[AreaID].CreID ~= 0 then
		unit.AddBuff(S59_MapID, AreaObj.InstanceID, RoleID, AreaObj.area[AreaID].BuffID, RoleID)
		map.MapDeleteCreature(S59_MapID, AreaObj.InstanceID, AreaObj.area[AreaID].CreID)
		AreaObj.area[AreaID].CreID = 0
		AreaObj.area[AreaID].TypeID = 0
		AreaObj.area[AreaID].BuffID = 0
	end
end

function GetKuaFuPKRoleObj(RoleID)
   return KuaFuPK_Role[RoleID]
end


function KuaFuPK_ChooseSide()
	local Player = {}
	Player[100] = {}
	Player[101] = {}
	local num = {}
	num[100] =0
	num[101] =0
	local battleNum = 0
	local battletemp = 1
	for k,v in pairs(KuaFuPK_Role) do
		if role.GetRoleItemNum(v.RoleID, 4800283) > 0 or role.GetRoleItemNum(v.RoleID, 4800284) > 0 then
			table.insert(Player[100],k)
			num[100] = num[100] + 1
		elseif role.GetRoleItemNum(v.RoleID, 4800285) > 0  or role.GetRoleItemNum(v.RoleID, 4800286) > 0 then
			table.insert(Player[101],k)
			num[101] = num[101] + 1
		end
	end
	if num[100] > 0 and num[101] > 0 then
		battleNum = 2
	end
	if num[100] <= 0 and num[101] > 0 then
		battleNum = 2
		battletemp = 2
	end
	if num[100] > 0 and num[101] <= 0 then
		battleNum = 1
	end
	for i = battletemp,battleNum do
		local RedSideObj = CreatKuaFuPKSideObj()
		local BlueSideObj = CreatKuaFuPKSideObj()
		local red = 0
		local times = 0
		local itemid = 0
		RedSideObj:PlayerJoinKuaFuPKSide(Player[99+i][1])
		local Player1Obj = GetKuaFuPKRoleObj(Player[99+i][1])
		table.remove(Player[99+i],1)
		if role.GetRoleItemNum(Player1Obj.RoleID, 4800281+2*i) > 0 then
			itemid = 4800281+2*i
		elseif role.GetRoleItemNum(Player1Obj.RoleID, 4800282+2*i) > 0 then
			itemid = 4800282+2*i
		end
		while red < num[99+i] do
		    for k1,v in pairs(Player[99+i]) do
			local PlayerObj = GetKuaFuPKRoleObj(v)
				if Player1Obj.RoleID ~= v and (PlayerObj.Guild ~= 0 and PlayerObj.Guild == Player1Obj.Guild and role.GetRoleItemNum(PlayerObj.RoleID, itemid) > 0) then
				    RedSideObj:PlayerJoinKuaFuPKSide(v)
					red = red + 1
					table.remove(Player[99+i],k1)
					if red >= num[99+i] then
					    break
					end
				end
			end
			times = times + 1
			if times > 80 then
			    break
			end
		end
		--RedSideObj.Num = red
		for k1,v in pairs(Player[99+i]) do
			if role.GetRoleItemNum(v, itemid+1) > 0 or role.GetRoleItemNum(v, itemid-1) > 0 then
				BlueSideObj:PlayerJoinKuaFuPKSide(v)
			end
		  -- BlueSideObj.Num = BlueSideObj.Num + 1
		end
		CreatKuaFuPKBattleObj(RedSideObj,BlueSideObj)
	end
end


--显示剩余时间
function ShowKuaFuPKTime(RoleObj)

    local leftSec = 3600

	local Min  		= tonumber(os.date("%M"))
	local Second	= tonumber(os.date("%S"))
	leftSec = leftSec - Min*60 - Second

	--同步客户端时间
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 16,	28)					--
	msg.AddMsgEvent(MsgID, 16,	leftSec)			--剩余时间
	msg.DispatchRoleMsgEvent(RoleObj.RoleID, MsgID)

end

function GetAttBuff_KuaFuPK(RoleObj)

end

function GetKillExp_KuaFuPK(RoleObj)
	RoleObj.Kill = RoleObj.Kill + 1

end

function WuDi_KuaFuPK(RoleObj)
	unit.AddBuff(S59_MapID, RoleObj.InstanceID, RoleObj.RoleID, 6109401, RoleObj.RoleID)
end

function SetColor_KuaFuPK(RoleObj)
    if RoleObj.Side == "Red" then
	    unit.SetState(S59_MapID, RoleObj.InstanceID, RoleObj.RoleID, 10)
	else
		unit.SetState(S59_MapID, RoleObj.InstanceID, RoleObj.RoleID, 11)
	end
end

function SendInfo_KuaFuPK(RoleObj)
	if KuaFuPK_Battle[RoleObj.InstanceID] ~= nil then
		local redpoint = KuaFuPK_Battle[RoleObj.InstanceID].RedPlayer.Point
		local rednum = KuaFuPK_Battle[RoleObj.InstanceID].RedPlayer.Num
		local bluepoint = KuaFuPK_Battle[RoleObj.InstanceID].BluePlayer.Point
		local bluenum = KuaFuPK_Battle[RoleObj.InstanceID].BluePlayer.Num
		local char = redpoint..","..rednum..","..bluepoint..","..bluenum
		role.DotaSendMsg(S59_MapID, RoleObj.InstanceID, RoleObj.RoleID, 1, char)
	end
end

function PlayerEnterKuaFuPK(RoleObj)
	if RoleObj.State == "OutSide" then
		RoleObj.State = "InSide"
		local tbl = {}
		if KuaFuPK_Battle[RoleObj.InstanceID] ~= nil then
			if RoleObj.Side == "Red" then
				KuaFuPK_Battle[RoleObj.InstanceID].RedPlayer.Num = KuaFuPK_Battle[RoleObj.InstanceID].RedPlayer.Num + 1
				tbl = KuaFuPK_Battle[RoleObj.InstanceID].RedPlayer.InSidePlayer
			elseif RoleObj.Side == "Blue" then
				KuaFuPK_Battle[RoleObj.InstanceID].BluePlayer.Num = KuaFuPK_Battle[RoleObj.InstanceID].BluePlayer.Num + 1
				tbl = KuaFuPK_Battle[RoleObj.InstanceID].BluePlayer.InSidePlayer
			end
			table.insert(tbl,RoleObj.RoleID)
		end
	end
end

function PlayerLeaveKuaFuPK(RoleObj)
	RoleObj.State = "OutSide"
	local tbl = {}
	if KuaFuPK_Battle[RoleObj.InstanceID] ~= nil then
		if RoleObj.Side == "Red" then
			KuaFuPK_Battle[RoleObj.InstanceID].RedPlayer.Num = KuaFuPK_Battle[RoleObj.InstanceID].RedPlayer.Num - 1
			tbl = KuaFuPK_Battle[RoleObj.InstanceID].RedPlayer.InSidePlayer

		elseif RoleObj.Side == "Blue" then
			KuaFuPK_Battle[RoleObj.InstanceID].BluePlayer.Num = KuaFuPK_Battle[RoleObj.InstanceID].BluePlayer.Num - 1
			tbl = KuaFuPK_Battle[RoleObj.InstanceID].BluePlayer.InSidePlayer
		end
		for k,v in pairs(tbl) do
			if v == RoleObj.RoleID then
				table.remove(tbl,k)
			end
		end
	end

end

function SendAllInfo_KuaFuPK(BattleObj)
    for k,v in pairs(BattleObj.RedPlayer.Player) do
	    local RoleObj = GetKuaFuPKRoleObj(v)
		local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleObj.RoleID)
		if Target_MapID ~= nil and Target_InstanceID ~= nil and Target_MapID == S59_MapID and RoleObj.InstanceID == Target_InstanceID then
		    RoleObj:SendInfo_KuaFuPK()
		end
	end
	for k,v in pairs(BattleObj.BluePlayer.Player) do
	    local RoleObj = GetKuaFuPKRoleObj(v)
		local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleObj.RoleID)
		if Target_MapID ~= nil and Target_InstanceID ~= nil and Target_MapID == S59_MapID and RoleObj.InstanceID == Target_InstanceID then
		    RoleObj:SendInfo_KuaFuPK()
		end
	end
end



function GetKuaFuPKAreaObjFromBattle(BattleObj)
   return BattleObj.Area
end

function SetPlayerPoint_KuaFuPK(BattleObj)
	local RedSideObj = BattleObj.RedPlayer
	local BlueSideObj = BattleObj.BluePlayer
	for k,v in pairs(RedSideObj.Player) do
	    local RoleObj = KuaFuPK_Role[v]
		if RoleObj.State == "InSide" then
		    local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleObj.RoleID)
			if Target_MapID ~= nil then
			    if not unit.IsDead(Target_MapID, Target_InstanceID, RoleObj.RoleID) then
				    role.SetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["KuaFuPKzhe_Point"], role.GetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["KuaFuPKzhe_Point"])+1)
				end
			end
		end
	end
	for k,v in pairs(BlueSideObj.Player) do
	    local RoleObj = KuaFuPK_Role[v]
		if RoleObj.State == "InSide" then
		    local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleObj.RoleID)
			if Target_MapID ~= nil then
			    if not unit.IsDead(Target_MapID, Target_InstanceID, RoleObj.RoleID) then
				    role.SetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["KuaFuPKzhe_Point"], role.GetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["KuaFuPKzhe_Point"])+1)
				end
			end
		end
	end
end

function SetPlayerFinalPoint_KuaFuPK(BattleObj)
	local RedSideObj = BattleObj.RedPlayer
	local BlueSideObj = BattleObj.BluePlayer
	if RedSideObj.Point >= BlueSideObj.Point then
		for k,v in pairs(RedSideObj.Player) do
			local RoleObj = KuaFuPK_Role[v]
			if RoleObj.State == "InSide" then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleObj.RoleID)
				if Target_MapID ~= nil then
					if not unit.IsDead(Target_MapID, Target_InstanceID, RoleObj.RoleID) then
						role.SetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["KuaFuPKzhe_Point"], role.GetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["KuaFuPKzhe_Point"])*2)
					end
				end
			end
		end
	end
	if RedSideObj.Point <= BlueSideObj.Point then
		for k,v in pairs(BlueSideObj.Player) do
			local RoleObj = KuaFuPK_Role[v]
			if RoleObj.State == "InSide" then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleObj.RoleID)
				if Target_MapID ~= nil then
					if not unit.IsDead(Target_MapID, Target_InstanceID, RoleObj.RoleID) then
						role.SetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["KuaFuPKzhe_Point"], role.GetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["KuaFuPKzhe_Point"])*2)
					end
				end
			end
		end
	end
end

function ResetInSidePlayer_KuaFuPK(BattleObj)

	local function reset(tbl)
		local tbl_new = {}
		if #tbl > 0 then
			for i = 1,#tbl do
				local index = math.random(#tbl)
				table.insert(tbl_new,tbl[index])
				table.remove(tbl,index)
			end
		end
		return tbl_new
	end

	BattleObj.BluePlayer.InSidePlayer = reset(BattleObj.BluePlayer.InSidePlayer)
	BattleObj.RedPlayer.InSidePlayer = reset(BattleObj.RedPlayer.InSidePlayer)


end
