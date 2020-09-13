
--严冬礼包（200）
function I4530093_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400024, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 3, -1, 8, 420)
	local k = math.random(1,100)
	if k < 4 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020011, 1, -1, 8, 420)
	end

end

function I4530093_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530093, 1, "I4530093_GiftBag")
aux.RegisterItemEvent(4530093, 0, "I4530093_CanUseGiftBag")





--严冬礼包（2000点）
function I4530094_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400031, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 6, -1, 8, 420)
	local k = math.random(1,100)
	if k <= 10 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020012, 1, -1, 8, 420)
	end
	local k = math.random(1,100)
	if k <= 20 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4090020, 1, -1, 8, 420)
	end

end

function I4530094_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530094, 1, "I4530094_GiftBag")
aux.RegisterItemEvent(4530094, 0, "I4530094_CanUseGiftBag")






