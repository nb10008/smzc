function I4700316_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4212008, 1, -1, 8, 423)
	local sex = role.GetRoleSex(TargetID)
	if sex == 1 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3090035, 1, -1, 8, 424)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 3090036, 1, -1, 8, 425)
	end
	if TypeID == 4700316 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020126, 2, -1, 8, 421)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820061, 1000, -1, 8, 421)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4300025, 1, -1, 8, 430)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030145, 6, -1, 8, 426)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4222401, 1, -1, 8, 422)
	elseif TypeID == 4700317 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020124, 2, -1, 8, 421)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820061, 700, -1, 8, 421)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4300019, 1, -1, 8, 430)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030143, 6, -1, 8, 426)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4222004, 1, -1, 8, 422)
	else
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020124, 1, -1, 8, 421)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820061, 300, -1, 8, 421)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4300013, 1, -1, 8, 430)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030143, 3, -1, 8, 426)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4222004, 1, -1, 8, 422)
	end
	
end
function I4700316_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 9) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 9)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4700316, 1, "I4700316_GiftBag")
aux.RegisterItemEvent(4700317, 1, "I4700316_GiftBag")
aux.RegisterItemEvent(4700318, 1, "I4700316_GiftBag")
aux.RegisterItemEvent(4700316, 0, "I4700316_CanUseGiftBag")
aux.RegisterItemEvent(4700317, 0, "I4700316_CanUseGiftBag")
aux.RegisterItemEvent(4700318, 0, "I4700316_CanUseGiftBag")

