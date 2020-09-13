--尊享卡1级
function I4530123_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070005, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010055, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530126, 1, -1, 8, 420)
end

function I4530123_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530123, 1, "I4530123_GiftBag")
aux.RegisterItemEvent(4530123, 0, "I4530123_CanUseGiftBag")


--尊享卡25级
function I4530126_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050007, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020080, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070075, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020011, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070067, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530127, 1, -1, 8, 420)
end

function I4530126_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 17) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530126, 1, "I4530126_GiftBag")
aux.RegisterItemEvent(4530126, 0, "I4530126_CanUseGiftBag")


--尊享卡25级
function I4530127_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050017, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020010, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020081, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070076, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020011, 6, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070069, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070067, 2, -1, 8, 420)
end

function I4530127_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 17) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530127, 1, "I4530127_GiftBag")
aux.RegisterItemEvent(4530127, 0, "I4530127_CanUseGiftBag")
