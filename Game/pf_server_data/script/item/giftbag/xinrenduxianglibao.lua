--18级新手礼包
function I4700279_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700280, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010060, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 5, -1, 8, 420)
end

function I4700279_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700279, 1, "I4700279_GiftBag")
aux.RegisterItemEvent(4700279, 0, "I4700279_CanUseGiftBag")

--28级新手包
function I4700280_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700281, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070026, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010060, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070068, 10, -1, 8, 420)

end

function I4700280_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700280, 1, "I4700280_GiftBag")
aux.RegisterItemEvent(4700280, 0, "I4700280_CanUseGiftBag")

--38级新手包
function I4700281_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700282, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070060, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070065, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020080, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010061, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030096, 2, -1, 8, 420)
end

function I4700281_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 9) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700281, 1, "I4700281_GiftBag")
aux.RegisterItemEvent(4700281, 0, "I4700281_CanUseGiftBag")

--48级新手包
function I4700282_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700283, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820087, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070065, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030099, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020081, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010061, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010057, 1, -1, 8, 420)
end

function I4700282_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 9) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700282, 1, "I4700282_GiftBag")
aux.RegisterItemEvent(4700282, 0, "I4700282_CanUseGiftBag")

--58级新手包
function I4700283_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700284, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070032, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 40, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070069, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010061, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300007, 1, -1, 8, 420)

end

function I4700283_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 9) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700283, 1, "I4700283_GiftBag")
aux.RegisterItemEvent(4700283, 0, "I4700283_CanUseGiftBag")

--68级新手包
function I4700284_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060062, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070034, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800131, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820257, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030093, 2, -1, 8, 420)
end

function I4700284_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700284, 1, "I4700284_GiftBag")
aux.RegisterItemEvent(4700284, 0, "I4700284_CanUseGiftBag")

