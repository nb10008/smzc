
--冬季大礼包（600点）
function I4530091_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060004, 6, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010036, 1, -1, 8, 420)
	local k = math.random(1,10)
	if k == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020008, 1, -1, 8, 420)
	end

end

function I4530091_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530091, 1, "I4530091_GiftBag")
aux.RegisterItemEvent(4530091, 0, "I4530091_CanUseGiftBag")





--冬季大礼包（1000点）
function I4530092_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060004, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010036, 2, -1, 8, 420)
	local k = math.random(1,6)
	if k == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020008, 1, -1, 8, 420)
	end

end

function I4530092_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530092, 1, "I4530092_GiftBag")
aux.RegisterItemEvent(4530092, 0, "I4530092_CanUseGiftBag")






