

function I4820021_Use(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820022, 1, -1, 8, 420)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 4820021, 10, 101)
	return 1
end

function I4820021_CanUse(MapID, InstanceID, TypeID, TargetID)
	local b = role.GetRoleItemNum(TargetID,4820021)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		return 40, false
	end
	if b <= 9 then
		return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4820021, 1, "I4820021_Use")
aux.RegisterItemEvent(4820021, 0, "I4820021_CanUse")
