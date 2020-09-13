--
function I3020014_Use(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020015, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3020014, 5, 101)
	return 1
end

function I3020014_CanUse(MapID, InstanceID, TypeID, TargetID)
	local b = role.GetRoleItemNum(TargetID,3020014)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if b <= 4 then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(3020014, 1, "I3020014_Use")
aux.RegisterItemEvent(3020014, 0, "I3020014_CanUse")

function I3020062_Use(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020014, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3020062, 20, 101)
	return 1
end

function I3020062_CanUse(MapID, InstanceID, TypeID, TargetID)
	local b = role.GetRoleItemNum(TargetID,3020062)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if b <= 19 then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(3020062, 1, "I3020062_Use")
aux.RegisterItemEvent(3020062, 0, "I3020062_CanUse")
