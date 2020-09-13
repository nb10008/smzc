function I4700147_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(100)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530109, 1, -1, 8, 420)
    if r <= 30 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020011, 1, -1, 8, 420)
	end

end

function I4700147_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700147, 1, "I4700147_GiftBag")
aux.RegisterItemEvent(4700147, 0, "I4700147_CanUseGiftBag")



function I4700148_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(100)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530109, 3, -1, 8, 420)
    if r <= 30 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020012, 1, -1, 8, 420)
	end

end

function I4700148_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700148, 1, "I4700148_GiftBag")
aux.RegisterItemEvent(4700148, 0, "I4700148_CanUseGiftBag")




function I4700149_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(100)
    role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530109, 6, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400024, 1, -1, 8, 420)
    if r <= 30 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020012, 2, -1, 8, 420)
	end

end

function I4700149_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700149, 1, "I4700149_GiftBag")
aux.RegisterItemEvent(4700149, 0, "I4700149_CanUseGiftBag")
