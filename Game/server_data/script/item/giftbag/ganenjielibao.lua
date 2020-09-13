--祈福礼包
function I4800195_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600001,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820236,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820237,3, -1, 8, 420)
end

function I4800195_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800195, 1, "I4800195_GiftBag")
aux.RegisterItemEvent(4800195, 0, "I4800195_CanUseGiftBag")



--感恩祈福礼包
function I4800196_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600001,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820236,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820237,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820043,1, -1, 8, 420)
end

function I4800196_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800196, 1, "I4800196_GiftBag")
aux.RegisterItemEvent(4800196, 0, "I4800196_CanUseGiftBag")



--圣恩祈福礼包
function I4800197_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600001,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820236,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820237,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300008,1, -1, 8, 420)
end

function I4800197_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800197, 1, "I4800197_GiftBag")
aux.RegisterItemEvent(4800197, 0, "I4800197_CanUseGiftBag")




--天神祈福礼包
function I4800198_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 5600001,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820236,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820237,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020087,1, -1, 8, 420)
end

function I4800198_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4800198, 1, "I4800198_GiftBag")
aux.RegisterItemEvent(4800198, 0, "I4800198_CanUseGiftBag")
