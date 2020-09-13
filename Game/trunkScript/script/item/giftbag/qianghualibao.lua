function I4530139_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020004, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530140, 1, -1, 8, 420)
end

function I4530139_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530139, 1, "I4530139_GiftBag")
aux.RegisterItemEvent(4530139, 0, "I4530139_CanUseGiftBag")




function I4530140_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020004, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530141, 1, -1, 8, 420)
end

function I4530140_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530140, 1, "I4530140_GiftBag")
aux.RegisterItemEvent(4530140, 0, "I4530140_CanUseGiftBag")




function I4530141_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020004, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530142, 1, -1, 8, 420)
end

function I4530141_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530141, 1, "I4530141_GiftBag")
aux.RegisterItemEvent(4530141, 0, "I4530141_CanUseGiftBag")




function I4530142_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020004, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530143, 1, -1, 8, 420)
end

function I4530142_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530142, 1, "I4530142_GiftBag")
aux.RegisterItemEvent(4530142, 0, "I4530142_CanUseGiftBag")




function I4530143_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020004, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530144, 1, -1, 8, 420)
end

function I4530143_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530143, 1, "I4530143_GiftBag")
aux.RegisterItemEvent(4530143, 0, "I4530143_CanUseGiftBag")




function I4530144_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020004, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530145, 1, -1, 8, 420)
end

function I4530144_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530144, 1, "I4530144_GiftBag")
aux.RegisterItemEvent(4530144, 0, "I4530144_CanUseGiftBag")



function I4530145_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530146, 1, -1, 8, 420)
end

function I4530145_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530145, 1, "I4530145_GiftBag")
aux.RegisterItemEvent(4530145, 0, "I4530145_CanUseGiftBag")



function I4530146_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530147, 1, -1, 8, 420)
end

function I4530146_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530146, 1, "I4530146_GiftBag")
aux.RegisterItemEvent(4530146, 0, "I4530146_CanUseGiftBag")



function I4530147_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530148, 1, -1, 8, 420)
end

function I4530147_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530147, 1, "I4530147_GiftBag")
aux.RegisterItemEvent(4530147, 0, "I4530147_CanUseGiftBag")




function I4530148_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530149, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020080, 5, -1, 8, 420)

end

function I4530148_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530148, 1, "I4530148_GiftBag")
aux.RegisterItemEvent(4530148, 0, "I4530148_CanUseGiftBag")




function I4530149_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530150, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020081, 3, -1, 8, 420)
end

function I4530149_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530149, 1, "I4530149_GiftBag")
aux.RegisterItemEvent(4530149, 0, "I4530149_CanUseGiftBag")




function I4530150_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
    role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020029, 1, -1, 8, 420)
end

function I4530150_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530150, 1, "I4530150_GiftBag")
aux.RegisterItemEvent(4530150, 0, "I4530150_CanUseGiftBag")
