--装备成长宝箱
function I4820235_Use(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4820171, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820083, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820247, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820248, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020103, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090036, 5, -1, 8, 420)
	local roleLevel = role.GetRoleLevel(MapID, InstanceID, TargetID)
	if(roleLevel >= 40) then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4800190, 1, -1, 8, 420)
	end
end

function I4820235_CanUse(MapID, InstanceID, TypeID, TargetID)
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 7) then
		--提示玩家背包空间不足
		return 40, false
	end
	return 0,false
end

aux.RegisterItemEvent(4820235, 1, "I4820235_Use")
aux.RegisterItemEvent(4820235, 0, "I4820235_CanUse")
