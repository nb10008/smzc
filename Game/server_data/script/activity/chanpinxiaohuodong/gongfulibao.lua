function I4830066_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070024, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070018, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 10, -1, 8, 420)
end
function I4830066_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 5) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 5)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830066, 1, "I4830066_GiftBag")
aux.RegisterItemEvent(4830066, 0, "I4830066_CanUseGiftBag")

