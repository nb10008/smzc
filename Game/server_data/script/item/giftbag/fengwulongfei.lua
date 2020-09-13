--凤舞龙飞卡
function I4530217_GiftBag(MapID, InstanceID, TypeID, TargetID)

	role.AddRoleItem(MapID, InstanceID, TargetID, 4530218, 1, -1, 8, 420)

	role.AddRoleItem(MapID, InstanceID, TargetID,3070057,2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070006,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510022,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510023,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4300006,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010054,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010054,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010055,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010055,1, -1, 8, 420)


end

function I4530217_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 10) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530217, 1, "I4530217_GiftBag")
aux.RegisterItemEvent(4530217, 0, "I4530217_CanUseGiftBag")

function I4530218_GiftBag(MapID, InstanceID, TypeID, TargetID)



	role.AddRoleItem(MapID, InstanceID, TargetID,3070051,25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070001,25, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070011,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070011,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070011,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070018,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4820081,45, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010035,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070028,15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3050007,20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510022,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510023,1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020011,2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020057,5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070068,3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070056,30, -1, 8, 420)


end

function I4530218_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 16) then
		--提示玩家背包空间不足
		bRet = 40
	end
	return bRet, bIgnore
end

aux.RegisterItemEvent(4530218, 1, "I4530218_GiftBag")
aux.RegisterItemEvent(4530218, 0, "I4530218_CanUseGiftBag")


