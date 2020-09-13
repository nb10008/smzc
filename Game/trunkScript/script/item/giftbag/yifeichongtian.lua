
--资料片体验包
function I4700294_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070068, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070060, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211201, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4221701, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070040, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020112, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020006, 30, -1, 8, 420)


end

function I4700294_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 12) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4700294, 1, "I4700294_GiftBag")
aux.RegisterItemEvent(4700294, 0, "I4700294_CanUseGiftBag")








