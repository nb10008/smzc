--
function I4700044_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211502,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4220902,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051,20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800098,1, -1, 8, 420)
end

function I4700044_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700044, 1, "I4700044_GiftBag")
aux.RegisterItemEvent(4700044, 0, "I4700044_CanUseGiftBag")








