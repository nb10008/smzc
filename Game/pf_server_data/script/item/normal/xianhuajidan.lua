--œ ª®
function I4800190_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local i = math.random(100)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	if i <= 1 then
		local MsgID = msg.BeginMsgEvent();	--◊¢≤·π„≤•æ‰±˙
		msg.AddMsgEvent(MsgID,102,206801)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.DispatchBroadcast(MsgID, -1, -1, -1)
		role.ModRoleAttValue(Target_MapID, Target_InstanceID, TargetID,128,9)
	else
		role.ModRoleAttValue(Target_MapID, Target_InstanceID, TargetID,128,1)
	end
	local MsgID_1 = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID_1, 13, 0)
	msg.AddMsgEvent(MsgID_1, 1, 206805)--°±&lt;p1&gt;∏¯ƒ„‘˘ÀÕ“ª∂‰œ ª®£°°°
	msg.AddMsgEvent(MsgID_1, 2, RoleID)
	msg.DispatchRoleMsgEvent(TargetID, MsgID_1)
	local num = role.GetRoleAttValue(Target_MapID, Target_MapID, TargetID,128)
	if(num~=nil and num%10 == 0) then
		local MsgID = msg.BeginMsgEvent();	--◊¢≤·π„≤•æ‰±˙
		msg.AddMsgEvent(MsgID,102,206808)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 9, num)
		msg.DispatchBroadcast(MsgID, -1, -1, -1)
	end
end



function I4800190_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	local sex = role.GetRoleSex(TargetID)
	if sex ~= 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206803)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 999
		return bRet, bIgnore
	end
end
aux.RegisterItemEvent(4800190, 1, "I4800190_OnUse")
aux.RegisterItemEvent(4800190, 0, "I4800190_CanUse")


--ª® ¯
function I4800193_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	role.ModRoleAttValue(Target_MapID, Target_MapID, TargetID,128,10)
	local MsgID_1 = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID_1, 13, 0)
	msg.AddMsgEvent(MsgID_1, 1, 206806)--°±&lt;p1&gt;∏¯ƒ„‘˘ÀÕ“ª ¯œ ª®£°°°
	msg.AddMsgEvent(MsgID_1, 2, RoleID)
	msg.DispatchRoleMsgEvent(TargetID, MsgID_1)
	local MsgID = msg.BeginMsgEvent();	--◊¢≤·π„≤•æ‰±˙
	msg.AddMsgEvent(MsgID,102,206810)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.DispatchBroadcast(MsgID, -1, -1, -1)
	local num = role.GetRoleAttValue(Target_MapID, Target_MapID, TargetID,128)
	if(num~=nil and num%10 == 0) then
		local MsgID = msg.BeginMsgEvent();	--◊¢≤·π„≤•æ‰±˙
		msg.AddMsgEvent(MsgID,102,206808)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 9, num)
		msg.DispatchBroadcast(MsgID, -1, -1, -1)
	end
end



function I4800193_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	local sex = role.GetRoleSex(TargetID)
	if sex ~= 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206803)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 999
		return bRet, bIgnore
	end
end
aux.RegisterItemEvent(4800193, 1, "I4800193_OnUse")
aux.RegisterItemEvent(4800193, 0, "I4800193_CanUse")


--º¶µ∞
function I4800191_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	local i = math.random(100)
	if i <= 1 then
		local MsgID = msg.BeginMsgEvent();	--◊¢≤·π„≤•æ‰±˙
		msg.AddMsgEvent(MsgID,102,206802)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.DispatchBroadcast(MsgID, -1, -1, -1)
		role.ModRoleAttValue(Target_MapID, Target_InstanceID, TargetID,129,1)
	else
		role.ModRoleAttValue(Target_MapID, Target_InstanceID, TargetID,129,1)
	end
	local MsgID_1 = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID_1, 13, 0)
	msg.AddMsgEvent(MsgID_1, 1, 206807)--°±&lt;p1&gt;∏¯ƒ„»”¡À“ª∏ˆº¶µ∞°°
	msg.AddMsgEvent(MsgID_1, 2, RoleID)
	msg.DispatchRoleMsgEvent(TargetID, MsgID_1)
	local num = role.GetRoleAttValue(Target_MapID, Target_InstanceID, TargetID,129)
	if(num~=nil and num%10 == 0) then
		local MsgID = msg.BeginMsgEvent();	--◊¢≤·π„≤•æ‰±˙
		msg.AddMsgEvent(MsgID,102,206809)
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.AddMsgEvent(MsgID, 9, num)
		msg.DispatchBroadcast(MsgID, -1, -1, -1)
	end
end



function I4800191_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	local sex = role.GetRoleSex(TargetID)
	if sex ~= 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206803)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 999
		return bRet, bIgnore
	end
end
aux.RegisterItemEvent(4800191, 1, "I4800191_OnUse")
aux.RegisterItemEvent(4800191, 0, "I4800191_CanUse")


--∞Æ÷ÆÀÆ
function I4800192_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	role.ModRoleAttValue(MapID, InstanceID, RoleID,129,-1)
end



function I4800192_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	local sex = role.GetRoleSex(RoleID)
	if sex ~= 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206558)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 999
		return bRet, bIgnore
	end
	local eggs = role.GetRoleAttValue(MapID, InstanceID,RoleID,129)
	if eggs==nil or eggs <= 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206804)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
		bRet = 999
		return bRet, bIgnore
	end
end
aux.RegisterItemEvent(4800192, 1, "I4800192_OnUse")
aux.RegisterItemEvent(4800192, 0, "I4800192_CanUse")
