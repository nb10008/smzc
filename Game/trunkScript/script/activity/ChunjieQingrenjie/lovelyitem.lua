--情人节物品加BUFF
--男性使用给队友男性加buff


--使用效果
function i4800077_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, TargetID)
    local sex = role.GetRoleSex(TargetID)
	if TeamID ~= nil and TeamID ~= 4294967295 then
		local Role = {}
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
		for i=1, 6 do
			if Role[i] and Role[i]~= 4294967295 then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i]) --获取队友的所在地图
				if MapID == Target_MapID and role.GetRoleSex(Role[i]) == 1 then
					unit.AddBuff(MapID, InstanceID, Role[i], 6000801, Role[i])--加BUFF
				end
			end
		end
	elseif TeamID == 4294967295 and sex == 1 then
	     unit.AddBuff(MapID, InstanceID,TargetID, 6000801, TargetID)--加BUFF
	end
end
aux.RegisterItemEvent(4800077, 1, "i4800077_QuestUsable")





--女性使用物品可以给女性队友加buff


--使用效果
function i4800079_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, TargetID)
    local sex = role.GetRoleSex(TargetID)
	if TeamID ~= nil and TeamID ~= 4294967295 then
		local Role = {}
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
		for i=1, 6 do
			if Role[i] and Role[i]~= 4294967295 then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i]) --获取队友的所在地图
				if MapID == Target_MapID and role.GetRoleSex(Role[i]) == 0 then
					unit.AddBuff(MapID, InstanceID, Role[i], 6000901, Role[i])--加BUFF
				end
			end
		end
	elseif TeamID == 4294967295 and sex == 0 then
	     unit.AddBuff(MapID, InstanceID, TargetID, 6000901, TargetID)--加BUFF
	end
end
aux.RegisterItemEvent(4800079, 1, "i4800079_QuestUsable")



--真爱情书给同区域队友加buff
function i4800078_QuestUsable(MapID, InstanceID, TypeID, TargetID)
	local TeamID = role.IsRoleHaveTeam(MapID, InstanceID, TargetID)
	if TeamID ~= nil and TeamID ~= 4294967295 then
		local Role = {}
		Role[1], Role[2], Role[3], Role[4], Role[5], Role[6] = role.GetRoleTeamMemID(TeamID)
		for i=1, 6 do
			if Role[i] and Role[i]~= 4294967295 then
				local Target_MapID, Target_InstanceID = role.GetRoleMapID(Role[i]) --获取队友的所在地图
				if MapID == Target_MapID  then
					unit.AddBuff(MapID, InstanceID, Role[i], 6001001, Role[i])--加BUFF
				end
			end
		end
	elseif TeamID == 4294967295  then
	     unit.AddBuff(MapID, InstanceID, TargetID,6001001, TargetID)--加BUFF
	end
end


aux.RegisterItemEvent(4800078, 1, "i4800078_QuestUsable")

