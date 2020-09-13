function I4700285_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020121, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600001, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820236, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820237, 3, -1, 8, 420)
end

function I4700285_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700285, 1, "I4700285_GiftBag")
aux.RegisterItemEvent(4700285, 0, "I4700285_CanUseGiftBag")

function I4700286_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020118, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600001, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820236, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820237, 3, -1, 8, 420)
end

function I4700286_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700286, 1, "I4700286_GiftBag")
aux.RegisterItemEvent(4700286, 0, "I4700286_CanUseGiftBag")
