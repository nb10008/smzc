function CreatTianXuanRoleObj(RoleID)
    if TianXuan_Role[RoleID] == nil then
	    local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleID)
	    local rolelevel = role.GetRoleLevel(Target_MapID, Target_InstanceID, RoleID)
	    if rolelevel >= 50 then
            TianXuan_Roletotal = TianXuan_Roletotal + 1
			TianXuan_Role[RoleID] = {RoleID = RoleID, Time = 0, Side = "None", InstanceID = 0,State = "OutSide",Kill = 0}
	        local Guild = guild.GetRoleGuildID(RoleID)
			local FamilyID = role.GetRoleFamilyID(Target_MapID, Target_InstanceID, RoleID)
			if Guild ~= nil and Guild ~= 4294967295 then
			    TianXuan_Role[RoleID].Guild = Guild
			else
			    TianXuan_Role[RoleID].Guild = 0
			end
			if FamilyID ~= nil and FamilyID ~= 4294967295 then
			    TianXuan_Role[RoleID].FamilyID = FamilyID
			else
			    TianXuan_Role[RoleID].FamilyID = 0
			end
			TianXuan_Role[RoleID].ShowTianXuanTime = ShowTianXuanTime
			TianXuan_Role[RoleID].GetAttBuff_TianXuan = GetAttBuff_TianXuan
			TianXuan_Role[RoleID].WuDi_TianXuan = WuDi_TianXuan
			TianXuan_Role[RoleID].SetColor_TianXuan = SetColor_TianXuan
			TianXuan_Role[RoleID].SendInfo_TianXuan = SendInfo_TianXuan
			TianXuan_Role[RoleID].PlayerLeaveTianXuan = PlayerLeaveTianXuan
			TianXuan_Role[RoleID].PlayerEnterTianXuan = PlayerEnterTianXuan
			TianXuan_Role[RoleID].GetKillExp_TianXuan = GetKillExp_TianXuan
		end
	end
end

function CreatTianXuanSideObj()
    local k = #TianXuan_Side + 1
	TianXuan_Side[k] = {Player={},InSidePlayer={},Num = 0,Point = 0}
	if k%2 == 1 then
	    TianXuan_Side[k].Side = "Red"
	else
	    TianXuan_Side[k].Side = "Blue"
	end
	TianXuan_Side[k].PlayerJoinTianXuanSide = PlayerJoinTianXuanSide
	return TianXuan_Side[k]
end

function PlayerJoinTianXuanSide(Obj,PlayerID)
    table.insert(Obj.Player,PlayerID)
	TianXuan_Role[PlayerID].Side = Obj.Side
end

function CreatTianXuanAreaObj(InstanceID)
    local k = #TianXuan_Area + 1
	TianXuan_Area[k] = {area={},InstanceID=InstanceID}
	local buff = {}
	buff[5610077] = 6109301
	buff[5610078] = 6109101
	buff[5610079] = 6109201
	for i = 1,9 do
		local id = 5610076+math.random(3)
		local cre = map.MapCreateCreature(S59_MapID, InstanceID, id, S59_ItemPoint[S59_Area[i]].x, S59_ItemPoint[S59_Area[i]].y, S59_ItemPoint[S59_Area[i]].z)
		TianXuan_Area[k].area[S59_Area[i]] = {}
		TianXuan_Area[k].area[S59_Area[i]].CreID = cre
		TianXuan_Area[k].area[S59_Area[i]].TypeID = id
		TianXuan_Area[k].area[S59_Area[i]].BuffID = buff[id]
	end
	TianXuan_Area[k].ReSetTianXuanArea = ReSetTianXuanArea
	TianXuan_Area[k].IntoTianXuanArea = IntoTianXuanArea
	return TianXuan_Area[k]
end

function CreatTianXuanBattleObj(RedPlayer,BluePlayer)
    local k = #TianXuan_Battle + 1
    map.CreateInstance(S59_MapID, k)
	TianXuan_Battle[k] = {RedPlayer = RedPlayer,BluePlayer = BluePlayer,InstanceID=k}
	for k1,v in pairs(RedPlayer.Player) do
	    TianXuan_Role[v].InstanceID = k
	end
	for k1,v in pairs(BluePlayer.Player) do
	    TianXuan_Role[v].InstanceID = k
	end
    TianXuan_Battle[k].Area = CreatTianXuanAreaObj(k)
	TianXuan_Battle[k].GetTianXuanAreaObjFromBattle = GetTianXuanAreaObjFromBattle
	TianXuan_Battle[k].SendAllInfo_TianXuan = SendAllInfo_TianXuan
	TianXuan_Battle[k].ChooseTianXuanZhiRen = ChooseTianXuanZhiRen
	TianXuan_Battle[k].SetPlayerPoint_TianXuan = SetPlayerPoint_TianXuan
	TianXuan_Battle[k].SetPlayerFinalPoint_TianXuan = SetPlayerFinalPoint_TianXuan
    TianXuan_Battle[k].ResetInSidePlayer_TianXuan = ResetInSidePlayer_TianXuan
end

function ReSetTianXuanArea(AreaObj)
	local buff = {}
	buff[5610077] = 6109301
	buff[5610078] = 6109101
	buff[5610079] = 6109201
	for i = 1,9 do
        if AreaObj.area[S59_Area[i]].CreID == 0 then
			local id = 5610076+math.random(3)
			local cre = map.MapCreateCreature(S59_MapID, AreaObj.InstanceID, id, S59_ItemPoint[S59_Area[i]].x, S59_ItemPoint[S59_Area[i]].y, S59_ItemPoint[S59_Area[i]].z)
			AreaObj.area[S59_Area[i]].CreID = cre
			AreaObj.area[S59_Area[i]].TypeID = id
			AreaObj.area[S59_Area[i]].BuffID = buff[id]
		end
	end
end

function IntoTianXuanArea(AreaObj,AreaID,RoleID)
	if AreaObj.area[AreaID].CreID ~= 0 then
		unit.AddBuff(S59_MapID, AreaObj.InstanceID, RoleID, AreaObj.area[AreaID].BuffID, RoleID)
		map.MapDeleteCreature(S59_MapID, AreaObj.InstanceID, AreaObj.area[AreaID].CreID)
		AreaObj.area[AreaID].CreID = 0
		AreaObj.area[AreaID].TypeID = 0
		AreaObj.area[AreaID].BuffID = 0
	end
end

function GetTianXuanRoleObj(RoleID)
   return TianXuan_Role[RoleID]
end


function TianXuan_ChooseSide()
    if TianXuan_Roletotal > 159 then
        local k = math.floor(TianXuan_Roletotal/80)
		local av_player = math.floor(TianXuan_Roletotal/k)
		local num = {}
		for i = 1,k do
		    if i ~= k then
				num[i] = av_player
			else
			    num[i] = TianXuan_Roletotal - av_player*(k-1)
			end
		end
		local Player = {}
		local Playertotal = 0
		local function GetIndex(Playertotal)
			local total = 0
			for i = 1,k do
				total = total + num[i]
				if total >= Playertotal then
					return i
				end
			end
		end
		for k1,v in pairs(TianXuan_Role) do
            Playertotal = Playertotal + 1
			local index = GetIndex(Playertotal)
			--设设置玩家的副本ID
			--v.InstanceID = index
			if Player[index] == nil then
			    Player[index] = {}
			end
			table.insert(Player[index],k1)
		end
		for i = 1,k do
		    local RedSideObj = CreatTianXuanSideObj()
			local BlueSideObj = CreatTianXuanSideObj()
			local red = 0
			local times = 0
			while red < num[i]/2 do
			    RedSideObj:PlayerJoinTianXuanSide(Player[i][1])
				local Player1Obj = GetTianXuanRoleObj(Player[i][1])
				red = red + 1
		        for k1,v in pairs(Player[i]) do
				    local PlayerObj = GetTianXuanRoleObj(v)
					if PlayerObj.RoleID ~= v and ((PlayerObj.Guild ~= 0 and PlayerObj.Guild == Player1Obj.Guild) or (PlayerObj.FamilyID ~= 0 and PlayerObj.FamilyID == Player1Obj.FamilyID)) then
					    RedSideObj:PlayerJoinTianXuanSide(v)
						red = red + 1
						table.remove(Player[i],k1)
						if red >= num[i]/2 then
						    break
						end
					end
				end
				table.remove(Player[i],1)
				times = times + 1
				if times > 160 then
				    break
				end
			end
			--RedSideObj.Num = red
			for k1,v in pairs(Player[i]) do
			   BlueSideObj:PlayerJoinTianXuanSide(v)
			  -- BlueSideObj.Num = BlueSideObj.Num + 1
			end
			CreatTianXuanBattleObj(RedSideObj,BlueSideObj)
		end
	else
		local Player = {}
		for k,v in pairs(TianXuan_Role) do
			table.insert(Player,k)
			--v.InstanceID = 1
		end
		local RedSideObj = CreatTianXuanSideObj()
		local BlueSideObj = CreatTianXuanSideObj()
		local red = 0
		local times = 0
		while red < TianXuan_Roletotal/2 do
			RedSideObj:PlayerJoinTianXuanSide(Player[1])
			local Player1Obj = GetTianXuanRoleObj(Player[1])
			red = red + 1
			for k1,v in pairs(Player) do
				local PlayerObj = GetTianXuanRoleObj(v)
				if PlayerObj.RoleID ~= v and ((PlayerObj.Guild ~= 0 and PlayerObj.Guild == Player1Obj.Guild) or (PlayerObj.FamilyID ~= 0 and PlayerObj.FamilyID == Player1Obj.FamilyID)) then
					RedSideObj:PlayerJoinTianXuanSide(v)
					red = red + 1
					table.remove(Player,k1)
					if red >= TianXuan_Roletotal/2 then
						break
					end
				end
			end
			table.remove(Player,1)
			times = times + 1
			if times > 160 then
				break
			end
		end
		--RedSideObj.Num = red
		for k1,v in pairs(Player) do
		   BlueSideObj:PlayerJoinTianXuanSide(v)
		  -- BlueSideObj.Num = BlueSideObj.Num + 1
		end
		CreatTianXuanBattleObj(RedSideObj,BlueSideObj)
    end
end


--显示剩余时间
function ShowTianXuanTime(RoleObj)

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

function GetAttBuff_TianXuan(RoleObj)
	local rolelevel = role.GetRoleLevel(S59_MapID, RoleObj.InstanceID, RoleObj.RoleID)
	if rolelevel > 100 then
		rolelevel = 100
	end
    if rolelevel >= 50 then
	    local index = math.floor((rolelevel-50)/5) + 1
        unit.AddBuff(S59_MapID, RoleObj.InstanceID, RoleObj.RoleID, index*100+6107001, RoleObj.RoleID)
	end
end

function GetKillExp_TianXuan(RoleObj)
	local rolelevel = role.GetRoleLevel(S59_MapID, RoleObj.InstanceID, RoleObj.RoleID)
    if rolelevel < 50 then
		rolelevel = 50
	elseif rolelevel > 100 then
	    rolelevel = 100
	end
	RoleObj.Kill = RoleObj.Kill + 1
	local exp1 = math.floor(TianXuan_Exp[rolelevel] / 10)
	if RoleObj.Kill > 30 then
	    exp1 = 0
	else
		role.AddRoleExp(S59_MapID, RoleObj.InstanceID, RoleObj.RoleID, exp1)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13,	0)		--“击杀数：&lt;p1&gt;，获得经验值&lt;p2&gt;点。（最多只能获得30次经验）”
	msg.AddMsgEvent(MsgID, 1,	206731)		--“确定”
	msg.AddMsgEvent(MsgID, 9,	RoleObj.Kill)
	msg.AddMsgEvent(MsgID, 9,	exp1)
	msg.DispatchRoleMsgEvent(RoleObj.RoleID, MsgID)
end

function WuDi_TianXuan(RoleObj)
	unit.AddBuff(S59_MapID, RoleObj.InstanceID, RoleObj.RoleID, 6109401, RoleObj.RoleID)
end

function SetColor_TianXuan(RoleObj)
    if RoleObj.Side == "Red" then
	    unit.SetState(S59_MapID, RoleObj.InstanceID, RoleObj.RoleID, 10)
	else
		unit.SetState(S59_MapID, RoleObj.InstanceID, RoleObj.RoleID, 11)
	end
end

function SendInfo_TianXuan(RoleObj)
    local redpoint = TianXuan_Battle[RoleObj.InstanceID].RedPlayer.Point
	local rednum = TianXuan_Battle[RoleObj.InstanceID].RedPlayer.Num
	local bluepoint = TianXuan_Battle[RoleObj.InstanceID].BluePlayer.Point
	local bluenum = TianXuan_Battle[RoleObj.InstanceID].BluePlayer.Num
	local char = redpoint..","..rednum..","..bluepoint..","..bluenum
	role.DotaSendMsg(S59_MapID, RoleObj.InstanceID, RoleObj.RoleID, 1, char)
end

function PlayerEnterTianXuan(RoleObj)
	RoleObj.State = "InSide"
	local tbl = {}
    if RoleObj.Side == "Red" then
		TianXuan_Battle[RoleObj.InstanceID].RedPlayer.Num = TianXuan_Battle[RoleObj.InstanceID].RedPlayer.Num + 1
		tbl = TianXuan_Battle[RoleObj.InstanceID].RedPlayer.InSidePlayer
	elseif RoleObj.Side == "Blue" then
	    TianXuan_Battle[RoleObj.InstanceID].BluePlayer.Num = TianXuan_Battle[RoleObj.InstanceID].BluePlayer.Num + 1
		tbl = TianXuan_Battle[RoleObj.InstanceID].BluePlayer.InSidePlayer
	end
	table.insert(tbl,RoleObj.RoleID)
end

function PlayerLeaveTianXuan(RoleObj)
	RoleObj.State = "OutSide"
	local tbl = {}
    if RoleObj.Side == "Red" then
		TianXuan_Battle[RoleObj.InstanceID].RedPlayer.Num = TianXuan_Battle[RoleObj.InstanceID].RedPlayer.Num - 1
		tbl = TianXuan_Battle[RoleObj.InstanceID].RedPlayer.InSidePlayer
	elseif RoleObj.Side == "Blue" then
	    TianXuan_Battle[RoleObj.InstanceID].BluePlayer.Num = TianXuan_Battle[RoleObj.InstanceID].BluePlayer.Num - 1
		tbl = TianXuan_Battle[RoleObj.InstanceID].BluePlayer.InSidePlayer
	end
	for k,v in pairs(tbl) do
		if v == RoleObj.RoleID then
			table.remove(tbl,k)
		end
	end

end

function SendAllInfo_TianXuan(BattleObj)
    for k,v in pairs(BattleObj.RedPlayer.Player) do
	    local RoleObj = GetTianXuanRoleObj(v)
		local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleObj.RoleID)
		if Target_MapID ~= nil and Target_InstanceID ~= nil and Target_MapID == S59_MapID and RoleObj.InstanceID == Target_InstanceID then
		    RoleObj:SendInfo_TianXuan()
		end
	end
	for k,v in pairs(BattleObj.BluePlayer.Player) do
	    local RoleObj = GetTianXuanRoleObj(v)
		local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleObj.RoleID)
		if Target_MapID ~= nil and Target_InstanceID ~= nil and Target_MapID == S59_MapID and RoleObj.InstanceID == Target_InstanceID then
		    RoleObj:SendInfo_TianXuan()
		end
	end
end

function ChooseTianXuanZhiRen(BattleObj)
    local bluenum = 5
	if BattleObj.BluePlayer.Num < bluenum then
	    bluenum = BattleObj.BluePlayer.Num
	end
	for i = 1,bluenum do
		if BattleObj.BluePlayer.InSidePlayer[i]~= nil then
			unit.AddBuff(S59_MapID, BattleObj.InstanceID, BattleObj.BluePlayer.InSidePlayer[i], 6109501, BattleObj.BluePlayer.InSidePlayer[i])
		end
	end
    if BattleObj.BluePlayer.Num < BattleObj.RedPlayer.Num then
	    local k =  math.floor((BattleObj.RedPlayer.Num - BattleObj.BluePlayer.Num)/5)
		if k > 0 then
		    local m = k+5
			if m > #BattleObj.BluePlayer.InSidePlayer then
			    local tbl = {}
				for k,v in pairs(BattleObj.BluePlayer.InSidePlayer) do
				    tbl[k] = v
				end
				for i = 1,m do
				    local index = math.random[#tbl]
					unit.AddBuff(S59_MapID, BattleObj.InstanceID, tbl[index], 6109501, tbl[index])
					table.remove(tbl,index)
				end
			else
			    for k,v in pairs(BattleObj.BluePlayer.InSidePlayer) do
					unit.AddBuff(S59_MapID, BattleObj.InstanceID, v, 6109501, v)
				end
			end
		end
    end
	local rednum = 5
	if BattleObj.RedPlayer.Num < rednum then
	    bluenum = BattleObj.RedPlayer.Num
	end
	for i = 1,bluenum do
		if BattleObj.RedPlayer.InSidePlayer[i]~= nil then
			unit.AddBuff(S59_MapID, BattleObj.InstanceID, BattleObj.RedPlayer.InSidePlayer[i], 6109501, BattleObj.RedPlayer.InSidePlayer[i])
		end
	end
    if BattleObj.RedPlayer.Num < BattleObj.BluePlayer.Num then
	    local k =  math.floor((BattleObj.BluePlayer.Num - BattleObj.RedPlayer.Num)/5)
		if k > 0 then
		    local m = k+5
			if m > #BattleObj.RedPlayer.InSidePlayer then
			    local tbl = {}
				for k,v in pairs(BattleObj.RedPlayer.InSidePlayer) do
				    tbl[k] = v
				end
				for i = 1,m do
				    local index = math.random[#tbl]
					unit.AddBuff(S59_MapID, BattleObj.InstanceID, tbl[index], 6109501, tbl[index])
					table.remove(tbl,index)
				end
			else
			    for k,v in pairs(BattleObj.RedPlayer.InSidePlayer) do
					unit.AddBuff(S59_MapID, BattleObj.InstanceID, v, 6109501, v)
				end
			end
		end
    end
end


function GetTianXuanAreaObjFromBattle(BattleObj)
   return BattleObj.Area
end

function SetPlayerPoint_TianXuan(BattleObj)
	local RedSideObj = BattleObj.RedPlayer
	local BlueSideObj = BattleObj.BluePlayer
	for k,v in pairs(RedSideObj.Player) do
	    local RoleObj = TianXuan_Role[v]
		if RoleObj.State == "InSide" then
		    local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleObj.RoleID)
			if Target_MapID ~= nil then
			    if not unit.IsDead(Target_MapID, Target_InstanceID, RoleObj.RoleID) then
				    role.SetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["Tianxuanzhe_Point"], role.GetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["Tianxuanzhe_Point"])+1)
				end
			end
		end
	end
	for k,v in pairs(BlueSideObj.Player) do
	    local RoleObj = TianXuan_Role[v]
		if RoleObj.State == "InSide" then
		    local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleObj.RoleID)
			if Target_MapID ~= nil then
			    if not unit.IsDead(Target_MapID, Target_InstanceID, RoleObj.RoleID) then
				    role.SetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["Tianxuanzhe_Point"], role.GetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["Tianxuanzhe_Point"])+1)
				end
			end
		end
	end
end

function SetPlayerFinalPoint_TianXuan(BattleObj)
	local RedSideObj = BattleObj.RedPlayer
	local BlueSideObj = BattleObj.BluePlayer
	if RedSideObj.Point >= BlueSideObj.Point then
		for k,v in pairs(RedSideObj.Player) do
			local RoleObj = TianXuan_Role[v]
			if RoleObj.State == "InSide" then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleObj.RoleID)
				if Target_MapID ~= nil then
					if not unit.IsDead(Target_MapID, Target_InstanceID, RoleObj.RoleID) then
						role.SetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["Tianxuanzhe_Point"], role.GetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["Tianxuanzhe_Point"])*2)
					end
				end
			end
		end
	end
	if RedSideObj.Point <= BlueSideObj.Point then
		for k,v in pairs(BlueSideObj.Player) do
			local RoleObj = TianXuan_Role[v]
			if RoleObj.State == "InSide" then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(RoleObj.RoleID)
				if Target_MapID ~= nil then
					if not unit.IsDead(Target_MapID, Target_InstanceID, RoleObj.RoleID) then
						role.SetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["Tianxuanzhe_Point"], role.GetRoleScriptData(RoleObj.RoleID, 1, RoleDataType["Tianxuanzhe_Point"])*2)
					end
				end
			end
		end
	end
end

function ResetInSidePlayer_TianXuan(BattleObj)

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
