function I4830063_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070058, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300008, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4830064, 1, -1, 8, 420)

end
function I4830063_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 7) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 7)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830063, 1, "I4830063_GiftBag")
aux.RegisterItemEvent(4830063, 0, "I4830063_CanUseGiftBag")

function I4830064_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070058, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 35, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020114, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3060005, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030129, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030132, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030120, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030122, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020048, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4830065, 1, -1, 8, 420)

end
function I4830064_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 15) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 15)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830064, 1, "I4830064_GiftBag")
aux.RegisterItemEvent(4830064, 0, "I4830064_CanUseGiftBag")

function I4830065_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070028, 8, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030131, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030134, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030140, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3030141, 2, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020057, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070068, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020116, 3, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 50, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020049, 8, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820047, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4820046, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4700278, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4211504, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4220903, 1, -1, 8, 420)

end
function I4830065_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
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
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830065, 1, "I4830065_GiftBag")
aux.RegisterItemEvent(4830065, 0, "I4830065_CanUseGiftBag")