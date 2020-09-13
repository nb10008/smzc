--台湾豪华大礼包
function I4700001_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070061, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070062, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070063, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4220101, 1, -1, 8, 420)
end

function I4700001_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700001, 1, "I4700001_GiftBag")
aux.RegisterItemEvent(4700001, 0, "I4700001_CanUseGiftBag")

