--龙腾盛世卡
function I4530221_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530222, 1, -1, 8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID,3070057,2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070006,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510022,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510023,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4300006,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010054,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010055,1, -1, 8, 420)

end

function I4530221_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530221, 1, "I4530221_GiftBag")
aux.RegisterItemEvent(4530221, 0, "I4530221_CanUseGiftBag")

function I4530222_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530223, 1, -1, 8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID,3070011,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070018,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4820081,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010035,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070028,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3050007,20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3090017,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020009,10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020057,20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020005,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070051,15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070001,15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070056,20, -1, 8, 420)

end

function I4530222_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 14) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530222, 1, "I4530222_GiftBag")
aux.RegisterItemEvent(4530222, 0, "I4530222_CanUseGiftBag")


function I4530223_GiftBag(MapID, InstanceID, TypeID, TargetID)


	role.AddRoleItem(MapID, InstanceID, TargetID,4510022,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510023,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070051,15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070001,15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4820081,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020049,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070068,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020057,15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020005,30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020011,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070056,30, -1, 8, 420)


end

function I4530223_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 11) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530223, 1, "I4530223_GiftBag")
aux.RegisterItemEvent(4530223, 0, "I4530223_CanUseGiftBag")

