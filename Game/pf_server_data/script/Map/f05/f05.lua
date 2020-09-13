----scriptdata[0]标记进副本是否刷怪
----scriptdata[1]标记3040075
----scriptdata[2]标记3040076
----scriptdata[3]标记3040077
----scriptdata[4]标记3040078
----scriptdata[5]标记3040086
----scriptdata[6]标记3040087
----scriptdata[7]标记3040088

function f05_OnPlayerEnter(MapID, InstanceID,RoleID)
	local bhave = role.IsRoleHaveBuff(MapID, InstanceID, RoleID, 99996)
	if bhave == true then
	 	unit.CancelBuff(MapID, InstanceID, RoleID, 9999601)
	end
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100084)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	local monsterhave = map.GetMapScriptData(MapID,InstanceID,1,0)
	if monsterhave==0 then
		map.MapCreateCreature(MapID, InstanceID, 3040075, 1400, 15576, 600)
		map.MapCreateCreature(MapID, InstanceID, 3040077, 1458, 15187, 400)
		map.MapCreateCreature(MapID, InstanceID, 3040078, 1000, 14973, 679)
		map.MapCreateCreature(MapID, InstanceID, 3040076, 1121, 14983, 358)
		map.MapCreateCreature(MapID, InstanceID, 3040082, 717, 10094, 554)
		map.MapCreateCreature(MapID, InstanceID, 3040082, 723, 10105, 750)
		map.MapCreateCreature(MapID, InstanceID, 3040082, 645, 10240, 733)
		map.MapCreateCreature(MapID, InstanceID, 3040082, 530, 10100, 717)
		map.MapCreateCreature(MapID, InstanceID, 3040086, 477, 10165, 679)
		map.SetMapScriptData(MapID,InstanceID,1,0,1)
	end
	--副本统计
	--instanceData(5, RoleID, 28, 35)
	--玩家统计
	--UpdateRoleData(RoleID, 16, 1)
	--加体验buff
	if experienceTeams ~= nil then
		local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, RoleID)
		for k in pairs(experienceTeams) do
			if experienceTeams[k].teamid == TeamID then
				local lev = math.floor(role.GetRoleLevel(MapID, InstanceID, RoleID)/10)
				if lev == 4 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109703, RoleID)
				elseif lev == 5 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109706, RoleID)
				elseif lev == 6 then
					unit.AddBuff(MapID, InstanceID, RoleID, 6109709, RoleID)
				end
				unit.AddBuff(MapID, InstanceID, RoleID, 6109601, RoleID)
				break
			end
		end
	end
end

aux.RegisterMapEvent("f05", 2, "f05_OnPlayerEnter")
