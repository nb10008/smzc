function I4700150_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(100)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530100, 1, -1, 8, 420)
    if r <= 10 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3060059, 1, -1, 8, 420)
	end

end

function I4700150_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700150, 1, "I4700150_GiftBag")
aux.RegisterItemEvent(4700150, 0, "I4700150_CanUseGiftBag")



function I4700151_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(100)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530101, 1, -1, 8, 420)
    if r <= 10 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3060059, 2, -1, 8, 420)
	end

end

function I4700151_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 2) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700151, 1, "I4700151_GiftBag")
aux.RegisterItemEvent(4700151, 0, "I4700151_CanUseGiftBag")




function I4700152_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local r = math.random(100)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4530102, 1, -1, 8, 420)
	if r <= 10 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3060059, 3, -1, 8, 420)
	end
    if r <= 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3060060, 1, -1, 8, 420)
	end

end

function I4700152_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700152, 1, "I4700152_GiftBag")
aux.RegisterItemEvent(4700152, 0, "I4700152_CanUseGiftBag")
