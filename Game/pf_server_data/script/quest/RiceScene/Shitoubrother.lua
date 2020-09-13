
function I4080078_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	local FreeSize1 = role.GetBagFreeSize(RoleID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)

	if (level < 20) then
		--提示玩家等级不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100008)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		bRet = 32
		return bRet, bIgnore
	elseif (FreeSize1 < 1) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
		return bRet, bIgnore
	end
end


function I4080078_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	local k = role.GetRoleLevel(MapID, InstanceID, RoleID)
	role.AddRoleExp(MapID, InstanceID, RoleID, k+60)
	role.AddRoleExp(Target_MapID, Target_InstanceID, TargetID, k+40)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4080077, 1, -1, 8, 420)
	--role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 4080056, 2, -1, 8, 420)
		local i = math.random(100)

		if i < 15 then

		local MsgID = msg.BeginMsgEvent();	--注册广播句柄
		msg.AddMsgEvent(MsgID,102,100101)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.DispatchBroadcast(MsgID,3424071976,-1,-1);
		end



end


aux.RegisterItemEvent(4080078, 0, "I4080078_CanUse")
aux.RegisterItemEvent(4080078, 1, "I4080078_OnUse")
