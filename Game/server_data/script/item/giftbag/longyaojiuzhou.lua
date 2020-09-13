--龙耀九州卡
function I4530211_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530212, 1, -1, 8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300007, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010054, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010055, 1, -1, 8, 420)
end

function I4530211_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530211, 1, "I4530211_GiftBag")
aux.RegisterItemEvent(4530211, 0, "I4530211_CanUseGiftBag")

function I4530212_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530213, 1, -1, 8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010035, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050007, 20, -1, 8, 420)
end

function I4530212_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530212, 1, "I4530212_GiftBag")
aux.RegisterItemEvent(4530212, 0, "I4530212_CanUseGiftBag")


function I4530213_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530214, 1, -1, 8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010035, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050007, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090017, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020049, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020009, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020005, 30, -1, 8, 420)

end

function I4530213_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 13) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530213, 1, "I4530213_GiftBag")
aux.RegisterItemEvent(4530213, 0, "I4530213_CanUseGiftBag")

function I4530214_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3010035, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3050007, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020114, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020011, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070069, 5, -1, 8, 420)


end

function I4530214_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 15) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530214, 1, "I4530214_GiftBag")
aux.RegisterItemEvent(4530214, 0, "I4530214_CanUseGiftBag")

