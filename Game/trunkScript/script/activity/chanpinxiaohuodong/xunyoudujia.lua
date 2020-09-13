function I4830045_GiftBag(MapID, InstanceID, TypeID, TargetID)
	
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020116, 4, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 150, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070058, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300008, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070060, 30, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 15, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 100, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800039, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800041, 10, -1, 8, 420)


	
end
function I4830045_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 17) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 17)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return  bRet, bIgnore
end
aux.RegisterItemEvent(4830045, 1, "I4830045_GiftBag")
aux.RegisterItemEvent(4830045, 0, "I4830045_CanUseGiftBag")

