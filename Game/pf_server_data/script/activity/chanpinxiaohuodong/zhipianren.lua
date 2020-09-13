function I4700304_GiftBag(MapID, InstanceID, TypeID, TargetID)
	
	role.AddRoleItem(MapID, InstanceID, TargetID, 4300008, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070051, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510022, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4510023, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070006, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070057, 2, -1, 8, 420)
	if TypeID == 4700304 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700305, 1, -1, 8, 420)
	elseif TypeID == 4700307 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700308, 1, -1, 8, 420)
	elseif TypeID == 4700310 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700311, 1, -1, 8, 420)
	end

end
function I4700304_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
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
aux.RegisterItemEvent(4700304, 1, "I4700304_GiftBag")
aux.RegisterItemEvent(4700307, 1, "I4700304_GiftBag")
aux.RegisterItemEvent(4700310, 1, "I4700304_GiftBag")
aux.RegisterItemEvent(4700304, 0, "I4700304_CanUseGiftBag")
aux.RegisterItemEvent(4700307, 0, "I4700304_CanUseGiftBag")
aux.RegisterItemEvent(4700310, 0, "I4700304_CanUseGiftBag")

function I4700305_GiftBag(MapID, InstanceID, TypeID, TargetID)
	
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070060, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070001, 20, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070014, 1, -1, 8, 420)
	if TypeID == 4700305 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 100, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700306, 1, -1, 8, 420)
	elseif TypeID == 4700308 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 8, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 80, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700309, 1, -1, 8, 420)
	elseif TypeID == 4700311 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 6, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 60, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4700312, 1, -1, 8, 420)
	end

end
function I4700305_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 6) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 6)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4700305, 1, "I4700305_GiftBag")
aux.RegisterItemEvent(4700308, 1, "I4700305_GiftBag")
aux.RegisterItemEvent(4700311, 1, "I4700305_GiftBag")
aux.RegisterItemEvent(4700305, 0, "I4700305_CanUseGiftBag")
aux.RegisterItemEvent(4700308, 0, "I4700305_CanUseGiftBag")
aux.RegisterItemEvent(4700311, 0, "I4700305_CanUseGiftBag")

function I4700306_GiftBag(MapID, InstanceID, TypeID, TargetID)
	
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020119, 2, -1, 8, 420)
	if TypeID == 4700306 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030131, 4, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030134, 4, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030140, 4, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030141, 4, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 10, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 100, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 500, -1, 8, 420)

	elseif TypeID == 4700309 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030131, 3, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030134, 3, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030140, 3, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030141, 3, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 8, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 80, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 300, -1, 8, 420)
	elseif TypeID == 4700312 then
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030131, 2, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030134, 2, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030140, 2, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3030141, 2, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4800242, 6, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3070056, 60, -1, 8, 420)
		role.AddRoleItem(MapID, InstanceID, TargetID, 4820081, 200, -1, 8, 420)
	end

end
function I4700306_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
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
aux.RegisterItemEvent(4700306, 1, "I4700306_GiftBag")
aux.RegisterItemEvent(4700309, 1, "I4700306_GiftBag")
aux.RegisterItemEvent(4700312, 1, "I4700306_GiftBag")
aux.RegisterItemEvent(4700306, 0, "I4700306_CanUseGiftBag")
aux.RegisterItemEvent(4700309, 0, "I4700306_CanUseGiftBag")
aux.RegisterItemEvent(4700312, 0, "I4700306_CanUseGiftBag")
