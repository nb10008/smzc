--秋分礼包
function I4700186_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800131, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820040, 10, -1, 8, 420)
end

function I4700186_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700186, 1, "I4700186_GiftBag")
aux.RegisterItemEvent(4700186, 0, "I4700186_CanUseGiftBag")
