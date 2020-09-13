function Q_meigui_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	local sex = role.GetRoleSex(RoleID)
	local FreeSize = role.GetBagFreeSize(TargetID)
	local FreeSize1 = role.GetBagFreeSize(RoleID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local level1 = role.GetRoleLevel(Target_MapID, Target_InstanceID, TargetID)
	if (level1 < 30) then
		--提示玩家等级不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100009)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		bRet = 32
		return bRet, bIgnore
	elseif sex == 0 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206559)--此物品只能男性使用。
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     
		bRet = 8
		return bRet, bIgnore
	elseif (FreeSize < 1) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100010)   --目标玩家行囊的空间不足
		msg.AddMsgEvent(MsgID, 9, 1)
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
aux.RegisterItemEvent(4800052, 0, "Q_meigui_CanUse")--玫瑰


function Q_meigui_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	local sex = role.GetRoleSex(TargetID)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4800072, 1, -1, 8, 420)
	if sex == 1 then
		role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 4800077, 1, -1, 8, 420)--“同志的祝福”
		local MsgID = msg.BeginMsgEvent();	
		msg.AddMsgEvent(MsgID,13,206561)--XX送你一朵玫瑰……
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	if sex == 0 then
		role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 4800078, 1, -1, 8, 420)--“真爱情书”
		local MsgID = msg.BeginMsgEvent();	
		msg.AddMsgEvent(MsgID,13,206561)--XX送你一朵玫瑰……
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
end
aux.RegisterItemEvent(4800052, 1, "Q_meigui_OnUse")

function Q_qiaokeli_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	local sex = role.GetRoleSex(RoleID)
	local FreeSize = role.GetBagFreeSize(TargetID)
	local FreeSize1 = role.GetBagFreeSize(RoleID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	local level1 = role.GetRoleLevel(Target_MapID, Target_InstanceID, TargetID)
	if (level1 < 30) then
		--提示玩家等级不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100009)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     --发送消息
		bRet = 32
		return bRet, bIgnore
	elseif sex == 1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 206558)--此物品只能女性使用。
		msg.DispatchRoleMsgEvent(RoleID, MsgID)     
		bRet = 8
		return bRet, bIgnore
	elseif (FreeSize < 1) then
		--提示玩家背包空间不足
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 100010)   --目标玩家行囊的空间不足
		msg.AddMsgEvent(MsgID, 9, 1)
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
aux.RegisterItemEvent(4800051, 0, "Q_qiaokeli_CanUse")--巧克力

function Q_qiaokeli_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	local sex = role.GetRoleSex(TargetID)
	role.AddRoleItem(MapID, InstanceID, RoleID, 4800072, 1, -1, 8, 420)
	if sex == 1 then
		role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 4800078, 1, -1, 8, 420)--“真爱情书”
		local MsgID = msg.BeginMsgEvent();	
		msg.AddMsgEvent(MsgID,13,206560)--XX送你一块巧克力……
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
	if sex == 0 then
		role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, 4800079, 1, -1, 8, 420)--“蕾丝边”
		local MsgID = msg.BeginMsgEvent();	
		msg.AddMsgEvent(MsgID,13,206560)--XX送你一块巧克力……
		msg.AddMsgEvent(MsgID, 2, RoleID)
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
end
aux.RegisterItemEvent(4800051, 1, "Q_qiaokeli_OnUse")