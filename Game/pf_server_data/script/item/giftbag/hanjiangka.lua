--悍将卡1级
function I4530124_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300007, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520031, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070005, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010055, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530130, 1, -1, 8, 420)
end

function I4530124_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 9) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530124, 1, "I4530124_GiftBag")
aux.RegisterItemEvent(4530124, 0, "I4530124_CanUseGiftBag")


--悍将卡10级
function I4530130_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530131, 1, -1, 8, 420)
end

function I4530130_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530130, 1, "I4530130_GiftBag")
aux.RegisterItemEvent(4530130, 0, "I4530130_CanUseGiftBag")



--悍将卡20级
function I4530131_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050007, 9, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010035, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530132, 1, -1, 8, 420)
end

function I4530131_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530131, 1, "I4530131_GiftBag")
aux.RegisterItemEvent(4530131, 0, "I4530131_CanUseGiftBag")


--悍将卡30级
function I4530132_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050007, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070075, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4520031, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530133, 1, -1, 8, 420)
end

function I4530132_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530132, 1, "I4530132_GiftBag")
aux.RegisterItemEvent(4530132, 0, "I4530132_CanUseGiftBag")


--悍将卡40级
function I4530133_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010055, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090001, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 35, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090018, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530134, 1, -1, 8, 420)
end

function I4530133_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 16) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530133, 1, "I4530133_GiftBag")
aux.RegisterItemEvent(4530133, 0, "I4530133_CanUseGiftBag")


--悍将卡50级
function I4530134_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020080, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020010, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 35, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530135, 1, -1, 8, 420)
end

function I4530134_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 13) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530134, 1, "I4530134_GiftBag")
aux.RegisterItemEvent(4530134, 0, "I4530134_CanUseGiftBag")


--悍将卡60级
function I4530135_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4090002, 28, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020080, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030128, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070076, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070013, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070005, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090002, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530136, 1, -1, 8, 420)
end

function I4530135_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 15) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530135, 1, "I4530135_GiftBag")
aux.RegisterItemEvent(4530135, 0, "I4530135_CanUseGiftBag")


--悍将卡80级
function I4530136_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020082, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530095, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600003, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070080, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820024, 107, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530137, 1, -1, 8, 420)
end

function I4530136_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 15) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530136, 1, "I4530136_GiftBag")
aux.RegisterItemEvent(4530136, 0, "I4530136_CanUseGiftBag")



--悍将卡80级
function I4530137_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820029, 207, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090030, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211504, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4220903, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530096, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020027, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010021, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010021, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010026, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010026, 1, -1, 8, 420)
end

function I4530137_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 11) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530137, 1, "I4530137_GiftBag")
aux.RegisterItemEvent(4530137, 0, "I4530137_CanUseGiftBag")
