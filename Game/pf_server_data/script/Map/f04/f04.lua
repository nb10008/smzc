---mapscriptdata[1] 记录进副本后是否已刷出怪物



function f04_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100068)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	local monsterhave = map.GetMapScriptData(MapID,InstanceID,1,1)
	if monsterhave==0 then
		map.MapCreateCreature(MapID, InstanceID, 3040044, 444, 5845, 114)
		map.SetMapScriptData(MapID,InstanceID,1,1,1)
	end
	--副本统计
	--instanceData(4, RoleID, 27, 34)
	--玩家统计
	--UpdateRoleData(RoleID, 15, 1)
	--加体验buff
	if experienceTeams ~= nil then
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		for k in pairs(experienceTeams) do
			if experienceTeams[k].teamid == TeamID then
				local lev = math.floor(role.GetRoleLevel(MapID, InstanceID, RoleID)/10)
				if lev == 4 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109702, RoleID)
				elseif lev == 5 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109705, RoleID)
				end
				unit.AddBuff(MapID, InstanceID, RoleID, 6109601, RoleID)
				break
			end
		end
	end
end

--注册
aux.RegisterMapEvent("f04", 2, "f04_OnPlayerEnter")
