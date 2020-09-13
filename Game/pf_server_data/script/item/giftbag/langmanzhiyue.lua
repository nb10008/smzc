--圣洁婚纱礼包
function I4700045_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	local k4 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8400001,1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8400002,1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8400003,1, k3, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8400004,1, k4, 8, 420)
end

function I4700045_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 4) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700045, 1, "I4700045_GiftBag")
aux.RegisterItemEvent(4700045, 0, "I4700045_CanUseGiftBag")



--浪漫之约
function I4700046_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	local k1 = math.random(5,6)
	local k2 = math.random(5,6)
	local k3 = math.random(5,6)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8393001,1, k1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8393002,1, k2, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 8393004,1, k3, 8, 420)
end

function I4700046_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 3) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700046, 1, "I4700046_GiftBag")
aux.RegisterItemEvent(4700046, 0, "I4700046_CanUseGiftBag")








