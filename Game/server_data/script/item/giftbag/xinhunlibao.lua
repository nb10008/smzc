--新婚礼包
function I4800115_CanUse(MapID, InstanceID, ItemTypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
		return bRet, bIgnore
	end
end

aux.RegisterItemEvent(4800115, 0, "I4800115_CanUse")

function I4800115_Use(MapID, InstanceID, ItemTypeID, TargetID)
	local num = math.random(100)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800116, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800117, 5, -1, 8, 420)
	if num < 20 then
		local ran = math.random(100)
		if ran < 50 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4100101, 1, -1, 8, 420)
		elseif ran < 54 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4100102, 1, -1, 8, 420)
		elseif ran < 58 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4100103, 1, -1, 8, 420)
		elseif ran < 62 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4100104, 1, -1, 8, 420)
		elseif ran < 66 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4100105, 1, -1, 8, 420)
		elseif ran < 70 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4100106, 1, -1, 8, 420)
		elseif ran < 74 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4100107, 1, -1, 8, 420)
		elseif ran < 84 then
			role.AddRoleItem(MapID, InstanceID, TargetID, 4400049, 1, -1, 8, 420)
		else
			role.AddRoleItem(MapID, InstanceID, TargetID, 3090017, 1, -1, 8, 420)
		end
	end
end
aux.RegisterItemEvent(4800115, 1, "I4800115_Use")
