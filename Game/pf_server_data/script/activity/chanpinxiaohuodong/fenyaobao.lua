function I4700314_GiftBag(MapID, InstanceID, TypeID, TargetID)
	local class = role.GetRoleClass(TargetID)
	if TypeID == 4700314 then
		if class==1 or class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then--物理
			role.AddRoleItem(MapID, InstanceID, TargetID, 8160055, 1, -1, 8, 428)
		else
			role.AddRoleItem(MapID, InstanceID, TargetID, 8170138, 1, -1, 8, 427)
		end
	else
		if class==1 or class==2 or class==3  or class==6 or class==7  or class==8 or class==9 then--物理
			role.AddRoleItem(MapID, InstanceID, TargetID, 8161011, 1, -1, 8, 428)
		else
			role.AddRoleItem(MapID, InstanceID, TargetID, 8171011, 1, -1, 8, 427)
		end
	end

end
function I4700314_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 1) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
	end
	return bRet, bIgnore
end
aux.RegisterItemEvent(4700314, 1, "I4700314_GiftBag")
aux.RegisterItemEvent(4700315, 1, "I4700314_GiftBag")
aux.RegisterItemEvent(4700314, 0, "I4700314_CanUseGiftBag")
aux.RegisterItemEvent(4700315, 0, "I4700314_CanUseGiftBag")
