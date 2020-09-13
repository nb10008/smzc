--¶á±¦Ææ±øµÄ±¦Ïä
function I4500028_Use(MapID, InstanceID, TypeID, TargetID)
    role.AddRoleItem(MapID, InstanceID, TargetID, 4500002, 60, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3020007, 60, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070029, 10, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070011, 1, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 3070023, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4400031, 5, -1, 8, 420)
	role.AddRoleItem(MapID, InstanceID, TargetID, 4530179, 1, -1, 8, 420)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100119)
	msg.AddMsgEvent(MsgID, 2, TargetID)
	msg.DispatchBroadcast(MsgID, -1, -1, -1)
	--[[
	local k = role.GetRoleScriptData(TargetID, 1, RoleDataType["DuoBaoQiBing_Bag"])
	if k == 0 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100119)
		msg.AddMsgEvent(MsgID, 2, TargetID)
		msg.DispatchBroadcast(MsgID, -1, -1, -1)
		role.SetRoleScriptData(TargetID, 1, RoleDataType["DuoBaoQiBing_Bag"], k+1)
		role.AddRoleItem(MapID, InstanceID, TargetID, 3020027, 1, -1, 8, 420)
	else
	    if math.random(10) < 4 then
		    local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100119)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.DispatchBroadcast(MsgID, -1, -1, -1)
            role.AddRoleItem(MapID, InstanceID, TargetID, 3020027, 1, -1, 8, 420)
        else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 102, 100120)
			msg.AddMsgEvent(MsgID, 2, TargetID)
			msg.DispatchBroadcast(MsgID, -1, -1, -1)
		end
        role.SetRoleScriptData(TargetID, 1, RoleDataType["DuoBaoQiBing_Bag"], k+1)
    end
	--]]
end

--[[function I4500028_CanUse(MapID, InstanceID, TypeID, TargetID)
	--ÅÐ¶Ï±³°ü¿ÕÏÐ¿Õ¼äÊÇ·ñ×ã¹»
	local FreeSize = role.GetBagFreeSize(TargetID)
	if(FreeSize < 8) then
		--ÌáÊ¾Íæ¼Ò±³°ü¿Õ¼ä²»×ã
		return 40, false
	end
	return 0,false
end]]

aux.RegisterItemEvent(4500028, 1, "I4500028_Use")
--aux.RegisterItemEvent(4500028, 0, "I4500028_CanUse")

