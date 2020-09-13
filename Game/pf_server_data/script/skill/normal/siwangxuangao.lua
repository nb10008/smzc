function SWXG_Cast2SingleTarget(MapID, InstanceID, SkillID, RoleID, TargetID)
	local x,y = role.GetBuffLayer(TargetID, 3501501)
	local Buff = role.IsRoleHaveBuff(MapID,InstanceID,TargetID, 35015)
	if Buff == true then
		if x == 1 then
			unit.AddBuff(MapID, InstanceID, TargetID, 3505901, RoleID)
		elseif x == 2 then
			unit.AddBuff(MapID, InstanceID, TargetID, 3505902, RoleID)
		elseif x == 3 then
			unit.AddBuff(MapID, InstanceID, TargetID, 3505903, RoleID)
		elseif x == 4 then
			unit.AddBuff(MapID, InstanceID, TargetID, 3505904, RoleID)
		elseif x == 5 then
			unit.AddBuff(MapID, InstanceID, TargetID, 3505905, RoleID)
		elseif x == 6 then
			unit.AddBuff(MapID, InstanceID, TargetID, 3505906, RoleID)
		elseif x == 7 then
			unit.AddBuff(MapID, InstanceID, TargetID, 3505907, RoleID)
		elseif x == 8 then
			unit.AddBuff(MapID, InstanceID, TargetID, 3505908, RoleID)
		elseif x == 9 then
			unit.AddBuff(MapID, InstanceID, TargetID, 3505909, RoleID)
		elseif x == 10 then
			unit.AddBuff(MapID, InstanceID, TargetID, 3505910, RoleID)
		elseif x == 11 then
			unit.AddBuff(MapID, InstanceID, TargetID, 3505911, RoleID)
		elseif x == 12 then
			unit.AddBuff(MapID, InstanceID, TargetID, 3505912, RoleID)
		elseif x == 13 then
			unit.AddBuff(MapID, InstanceID, TargetID, 3505913, RoleID)
		elseif x == 14 then
			unit.AddBuff(MapID, InstanceID, TargetID, 3505914, RoleID)
		elseif x == 15 then
			unit.AddBuff(MapID, InstanceID, TargetID, 3505915, RoleID)
		end
	end
end
aux.RegisterSkillEvent( 2010501, 5, "SWXG_Cast2SingleTarget")

function SWXG_CanCast2SingleTarget(MapID, InstanceID, SkillID, RoleID, TargetID)
	local Buff = role.IsRoleHaveBuff(MapID,InstanceID,TargetID, 35015)
	if Buff == true then
		return 0
	else
		return 7
	end
end
aux.RegisterSkillEvent( 2010501, 0, "SWXG_CanCast2SingleTarget")
