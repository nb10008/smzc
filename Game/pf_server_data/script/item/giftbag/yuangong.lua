--内部员工包
function I4700018_GiftBag(MapID, InstanceID, TypeID, TargetID)
	--玩家ID， 生成物品TypeID，数量，品质，生成模式，log用网络消息枚举
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070058, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020061, 40, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070008, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 999, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020025, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020029, 12, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4220904, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211803, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
end

function I4700018_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 23) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700018, 1, "I4700018_GiftBag")
aux.RegisterItemEvent(4700018, 0, "I4700018_CanUseGiftBag")








