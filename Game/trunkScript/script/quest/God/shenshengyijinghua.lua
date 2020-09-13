--
function ISSYJH_Use(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, TypeID+4, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, TargetID, TypeID, 20, 101)
	return 1
end

function ISSYJH_CanUse(MapID, InstanceID, TypeID, TargetID)
	local b = role.GetRoleItemNum(TargetID,TypeID)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if b < 20 then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4090028, 1, "ISSYJH_Use")
aux.RegisterItemEvent(4090028, 0, "ISSYJH_CanUse")
aux.RegisterItemEvent(4090029, 1, "ISSYJH_Use")
aux.RegisterItemEvent(4090029, 0, "ISSYJH_CanUse")
aux.RegisterItemEvent(4090030, 1, "ISSYJH_Use")
aux.RegisterItemEvent(4090030, 0, "ISSYJH_CanUse")
aux.RegisterItemEvent(4090031, 1, "ISSYJH_Use")
aux.RegisterItemEvent(4090031, 0, "ISSYJH_CanUse")

