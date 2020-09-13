
--10级YY公民卡
function I4530061_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530062, 1, -1, 8, 420)
end

function I4530061_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530061, 1, "I4530061_GiftBag")
aux.RegisterItemEvent(4530061, 0, "I4530061_CanUseGiftBag")




--20级YY公民卡
function I4530062_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010060, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070009, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530063, 1, -1, 8, 420)
end

function I4530062_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530062, 1, "I4530062_GiftBag")
aux.RegisterItemEvent(4530062, 0, "I4530062_CanUseGiftBag")



--30级YY公民卡
function I4530063_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010060, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
end

function I4530063_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530063, 1, "I4530063_GiftBag")
aux.RegisterItemEvent(4530063, 0, "I4530063_CanUseGiftBag")



--10级YY尊贵卡
function I4530071_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070026, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530072, 1, -1, 8, 420)
end

function I4530071_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530071, 1, "I4530071_GiftBag")
aux.RegisterItemEvent(4530071, 0, "I4530071_CanUseGiftBag")



--20级YY尊贵卡
function I4530072_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010060, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070009, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530073, 1, -1, 8, 420)
end

function I4530072_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530072, 1, "I4530072_GiftBag")
aux.RegisterItemEvent(4530072, 0, "I4530072_CanUseGiftBag")



--30级YY尊贵卡
function I4530073_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010060, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530074, 1, -1, 8, 420)
end

function I4530073_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530073, 1, "I4530073_GiftBag")
aux.RegisterItemEvent(4530073, 0, "I4530073_CanUseGiftBag")


--40级YY尊贵卡
function I4530074_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010061, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530075, 1, -1, 8, 420)
end

function I4530074_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530074, 1, "I4530074_GiftBag")
aux.RegisterItemEvent(4530074, 0, "I4530074_CanUseGiftBag")




--50级YY尊贵卡
function I4530075_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
end

function I4530075_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530075, 1, "I4530075_GiftBag")
aux.RegisterItemEvent(4530075, 0, "I4530075_CanUseGiftBag")




--10级YY皇室卡
function I4530081_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070026, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211001, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530082, 1, -1, 8, 420)
end

function I4530081_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530081, 1, "I4530081_GiftBag")
aux.RegisterItemEvent(4530081, 0, "I4530081_CanUseGiftBag")



--20级YY皇室卡
function I4530082_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010060, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070009, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530083, 1, -1, 8, 420)
end

function I4530082_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530082, 1, "I4530082_GiftBag")
aux.RegisterItemEvent(4530082, 0, "I4530082_CanUseGiftBag")



--30级YY皇室卡
function I4530083_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010056, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010060, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070010, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530084, 1, -1, 8, 420)
end

function I4530083_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530083, 1, "I4530083_GiftBag")
aux.RegisterItemEvent(4530083, 0, "I4530083_CanUseGiftBag")


--40级YY皇室卡
function I4530084_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010061, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530085, 1, -1, 8, 420)
end

function I4530084_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530084, 1, "I4530084_GiftBag")
aux.RegisterItemEvent(4530084, 0, "I4530084_CanUseGiftBag")




--50级YY皇室卡
function I4530085_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
end

function I4530085_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530085, 1, "I4530085_GiftBag")
aux.RegisterItemEvent(4530085, 0, "I4530085_CanUseGiftBag")
























