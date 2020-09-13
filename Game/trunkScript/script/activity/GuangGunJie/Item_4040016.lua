--ƒ‰√˚«È È
function I4040016_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Temp1 = math.random(100146,100158)
	local Temp2 = math.random(100152,100165)
	local i = math.random(100)
	local sex = role.GetRoleSex(TargetID)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4040027, 1, -1, 8, 420)
	--if i < 10 then
		if sex == 1 then
		local MsgID = msg.BeginMsgEvent();	--◊¢≤·π„≤•æ‰±˙
		msg.AddMsgEvent(MsgID,102,Temp1)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.DispatchBroadcast(MsgID, -1, -1, -1)
		elseif sex == 0 then
		local MsgID = msg.BeginMsgEvent();	--◊¢≤·π„≤•æ‰±˙
		msg.AddMsgEvent(MsgID,102,Temp2)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.DispatchBroadcast(MsgID, -1, -1, -1)

		end
	--end
end



function I4040016_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local FreeSize1 = role.GetBagFreeSize(RoleID)
	if FreeSize1 < 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100003)
		msg.AddMsgEvent(MsgID, 9, 1)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 40
		return bRet, bIgnore
	end
end
aux.RegisterItemEvent(4040016, 1, "I4040016_OnUse")
aux.RegisterItemEvent(4040016, 0, "I4040016_CanUse")
