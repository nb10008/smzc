function I3541001_CanUse(MapID, InstanceID, ItemID, TargetID)
	local bRet, bIgnore = 0, false
	if role.GetRoleItemNum(TargetID, 3541001) < 20 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 440010)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 32
	else
		local FreeSize = role.GetBagFreeSize(TargetID)
		if(FreeSize < 1) then
			bRet = 40
		end
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(3541001, 0, "I3541001_CanUse")

function I3541001_Use(MapID, InstanceID, ItemID, TargetID)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3540103, 1, -1, 1, 104)
	role.RemoveFromRole(MapID, InstanceID, TargetID, 3541001, 20, 300)
end
aux.RegisterItemEvent(3541001, 1, "I3541001_Use")


