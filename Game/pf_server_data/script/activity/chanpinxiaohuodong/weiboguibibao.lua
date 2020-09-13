function I4830046_GiftBag(MapID, InstanceID, TypeID, TargetID)
	
	role.AddRoleItem(MapID, InstanceID, TargetID, 3090002, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020081, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070040, 3, -1, 8, 420)

	
end
function I4830046_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
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
end
aux.RegisterItemEvent(4830046, 1, "I4830046_GiftBag")
aux.RegisterItemEvent(4830046, 0, "I4830046_CanUseGiftBag")

