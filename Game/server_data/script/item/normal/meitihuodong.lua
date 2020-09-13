
function meiti_CanUse(MapID, InstanceID, TypeID, RoleID, TargetID)
	local bRet, bIgnore = 0, false
	--判断背包空闲空间是否足够
	local FreeSize = role.GetBagFreeSize(TargetID)
	local FreeSize1 = role.GetBagFreeSize(RoleID)

	if (FreeSize < 1) then
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
		bRet = 32
		return bRet, bIgnore
	end
end


function meiti_OnUse(MapID, InstanceID, TypeID, RoleID, TargetID)
    local item = {}
	item[4520020] = {id1=4520029,id2=4520010}
	item[4520021] = {id1=4520030,id2=4520011}
	item[4520022] = {id1=4520030,id2=4520012}
	item[4520023] = {id1=4520030,id2=4520013}
	item[4520024] = {id1=4520030,id2=4520015}
	item[4520025] = {id1=4520030,id2=4520016}
	item[4520026] = {id1=4520030,id2=4520017}
	item[4520027] = {id1=4520030,id2=4520018}
	item[4520028] = {id1=4520030,id2=4520019}
	item[4520034] = {id1=4520030,id2=4520035}
	item[4520038] = {id1=4520030,id2=4520036}
	item[4520039] = {id1=4520030,id2=4520037}

	local Target_MapID, Target_InstanceID = role.GetRoleMapID(TargetID)
	role.AddRoleItem(MapID, InstanceID, RoleID, item[TypeID].id1, 10, -1, 8, 420)
	role.AddRoleItem(Target_MapID, Target_InstanceID, TargetID, item[TypeID].id2, 1, -1, 8, 420)
	local MsgID = msg.BeginMsgEvent();
	msg.AddMsgEvent(MsgID, 13, 1)
	msg.AddMsgEvent(MsgID, 1, 206001)
	msg.AddMsgEvent(MsgID, 2, RoleID)
	msg.AddMsgEvent(MsgID, 4, item[TypeID].id2)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)
end


aux.RegisterItemEvent(4520020, 0, "meiti_CanUse")
aux.RegisterItemEvent(4520020, 1, "meiti_OnUse")
aux.RegisterItemEvent(4520021, 0, "meiti_CanUse")
aux.RegisterItemEvent(4520021, 1, "meiti_OnUse")
aux.RegisterItemEvent(4520022, 0, "meiti_CanUse")
aux.RegisterItemEvent(4520022, 1, "meiti_OnUse")
aux.RegisterItemEvent(4520023, 0, "meiti_CanUse")
aux.RegisterItemEvent(4520023, 1, "meiti_OnUse")
aux.RegisterItemEvent(4520024, 0, "meiti_CanUse")
aux.RegisterItemEvent(4520024, 1, "meiti_OnUse")
aux.RegisterItemEvent(4520025, 0, "meiti_CanUse")
aux.RegisterItemEvent(4520025, 1, "meiti_OnUse")
aux.RegisterItemEvent(4520026, 0, "meiti_CanUse")
aux.RegisterItemEvent(4520026, 1, "meiti_OnUse")
aux.RegisterItemEvent(4520027, 0, "meiti_CanUse")
aux.RegisterItemEvent(4520027, 1, "meiti_OnUse")
aux.RegisterItemEvent(4520028, 0, "meiti_CanUse")
aux.RegisterItemEvent(4520028, 1, "meiti_OnUse")
aux.RegisterItemEvent(4520034, 0, "meiti_CanUse")
aux.RegisterItemEvent(4520034, 1, "meiti_OnUse")
aux.RegisterItemEvent(4520038, 0, "meiti_CanUse")
aux.RegisterItemEvent(4520038, 1, "meiti_OnUse")
aux.RegisterItemEvent(4520039, 0, "meiti_CanUse")
aux.RegisterItemEvent(4520039, 1, "meiti_OnUse")
