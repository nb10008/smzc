function I4800167_Use(MapID, InstanceID, TypeID, TargetID)
    local level =  role.GetRoleLevel(MapID, InstanceID, TargetID)
    if(level < 70) then
		local high,low = role.GetRoleLevelUpExp(MapID,InstanceID,TargetID)
		local Exp = 0
		if (high ~=nil and high >0 ) then
			Exp = high * 1000000000 + low
		else
			Exp = low
		end
		role.AddRoleExp(MapID, InstanceID, TargetID,Exp)
    elseif(level >= 70) then
		role.AddRoleExp(MapID, InstanceID, TargetID,30000000)
		role.RemoveFromRole(MapID, InstanceID, TargetID, 4800167, 1, 420)
    end
end

aux.RegisterItemEvent(4800167, 1, "I4800167_Use")
