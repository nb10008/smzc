function I4830072_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020116, 2, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4800242, 6, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070051, 20, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070056, 100, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070006, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4222003, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4211504, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4300008, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4800287, 120, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,8112009, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,8112008, 1, -1, 8, 421)
	
end
function I4830072_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 11) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 11)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830072, 1, "I4830072_GiftBag")
aux.RegisterItemEvent(4830072, 0, "I4830072_CanUseGiftBag")

function I4830073_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070057, 2, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070006, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510022, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510023, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4300008, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010054, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010055, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4830077, 1, -1, 8, 421)
	
end
function I4830073_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 8)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830073, 1, "I4830073_GiftBag")
aux.RegisterItemEvent(4830073, 0, "I4830073_CanUseGiftBag")
function I4830077_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070051, 15, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070001, 15, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070011, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070018, 5, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4820081, 20, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010035, 5, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070028, 5, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070056, 30, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4830078, 1, -1, 8, 421)
end
function I4830077_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
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
aux.RegisterItemEvent(4830077, 1, "I4830077_GiftBag")
aux.RegisterItemEvent(4830077, 0, "I4830077_CanUseGiftBag")
function I4830078_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070011, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070018, 5, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4820081, 30, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010035, 5, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070028, 5, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020114, 2, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3090017, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020049, 5, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020009, 10, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020057, 20, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070056, 30, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020005, 30, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4800242, 3, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4830079, 1, -1, 8, 421)

end
function I4830078_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 14) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 14)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830078, 1, "I4830078_GiftBag")
aux.RegisterItemEvent(4830078, 0, "I4830078_CanUseGiftBag")
function I4830079_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070011, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070018, 5, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4820081, 30, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3010035, 5, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070028, 5, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020115, 2, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070057, 2, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510022, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510023, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020011, 6, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020057, 10, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070051, 15, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070056, 30, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070069, 5, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4820249, 5, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3060005, 35, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4530107, 5, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4800242, 3, -1, 8, 421)

end
function I4830079_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 18) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 18)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830079, 1, "I4830079_GiftBag")
aux.RegisterItemEvent(4830079, 0, "I4830079_CanUseGiftBag")
function I4830074_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070006, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070057, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070058, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510022, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4510023, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4300008, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4830075, 1, -1, 8, 421)
end
function I4830074_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
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
aux.RegisterItemEvent(4830074, 1, "I4830074_GiftBag")
aux.RegisterItemEvent(4830074, 0, "I4830074_CanUseGiftBag")
function I4830075_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070057, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070058, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070014, 1, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070056, 35, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020114, 2, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4820081, 50, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3060005, 30, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3030129, 2, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3030132, 2, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3030120, 2, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3030122, 2, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070028, 10, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4800242, 3, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020048, 10, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4830076, 1, -1, 8, 421)
end
function I4830075_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
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
aux.RegisterItemEvent(4830075, 1, "I4830075_GiftBag")
aux.RegisterItemEvent(4830075, 0, "I4830075_CanUseGiftBag")
function I4830076_GiftBag(MapID, InstanceID, TypeID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070056, 60, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070028, 10, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3030130, 2, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3030133, 2, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3030121, 2, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3030123, 2, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020057, 10, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3070068, 10, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020115, 2, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4820081, 50, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,3020049, 10, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4700278, 5, -1, 8, 421)
	role.AddRoleItem(MapID, InstanceID, TargetID,4800242, 3, -1, 8, 421)

end
function I4830076_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 13) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 13)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4830076, 1, "I4830076_GiftBag")
aux.RegisterItemEvent(4830076, 0, "I4830076_CanUseGiftBag")