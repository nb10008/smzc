

function I4800232_OnUse(MapID, InstanceID, TypeID, TargetID)
    local gold = 10000000
	role.AddRoleSilver(MapID, InstanceID, TargetID, gold, 101)
end

function I4800232_CanuseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
    local freegold=role.GetRoleSilver(MapID,InstanceID,TargetID)
		--判断金钱是否超出上限
    if(freegold > 9979999999) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 306001)--需要至少x格背包空间才可继续操作
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 999
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4800232, 1, "I4800232_OnUse")
aux.RegisterItemEvent(4800232, 0, "I4800232_CanuseGiftBag")

function I4800233_OnUse(MapID, InstanceID, TypeID, TargetID)
    local gold = 20000000
	role.AddRoleSilver(MapID, InstanceID, TargetID, gold, 101)
end

function I4800233_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
    local freegold=role.GetRoleSilver(MapID,InstanceID,TargetID)
		--判断金钱是否超出上限
    if(freegold > 9979999999) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 306001)--需要至少x格背包空间才可继续操作
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 999
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4800233, 1, "I4800233_OnUse")
aux.RegisterItemEvent(4800233, 0, "I4800233_CanUseGiftBag")


function I4800234_OnUse(MapID, InstanceID, TypeID, TargetID)
    local gold = 50000000
	role.AddRoleSilver(MapID, InstanceID, TargetID, gold, 101)
end

function I4800234_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
    local freegold=role.GetRoleSilver(MapID,InstanceID,TargetID)
		--判断金钱是否超出上限
    if(freegold > 9979999999) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 306001)--需要至少x格背包空间才可继续操作
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 999
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4800234, 1, "I4800234_OnUse")
aux.RegisterItemEvent(4800234, 0, "I4800234_CanUseGiftBag")

function I4800235_OnUse(MapID, InstanceID, TypeID, TargetID)
    local gold = 100000000
	role.AddRoleSilver(MapID, InstanceID, TargetID, gold, 101)
end

function I4800235_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
    local freegold=role.GetRoleSilver(MapID,InstanceID,TargetID)
		--判断金钱是否超出上限
    if(freegold > 9979999999) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 306001)--需要至少x格背包空间才可继续操作
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 999
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4800235, 1, "I4800235_OnUse")
aux.RegisterItemEvent(4800235, 0, "I4800235_CanUseGiftBag")

function I4800260_OnUse(MapID, InstanceID, TypeID, TargetID)
    local gold = 30000000
	role.AddRoleSilver(MapID, InstanceID, TargetID, gold, 101)
end

function I4800260_CanUseGiftBag(MapID, InstanceID, TypeID, TargetID)
	local bRet, bIgnore = 0, false
    local freegold=role.GetRoleSilver(MapID,InstanceID,TargetID)
		--判断金钱是否超出上限
    if(freegold > 9969999999) then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 306001)--需要至少x格背包空间才可继续操作
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
		bRet = 999
	end
	return bRet, bIgnore
end


aux.RegisterItemEvent(4800260, 1, "I4800260_OnUse")
aux.RegisterItemEvent(4800260, 0, "I4800260_CanUseGiftBag")




