--
function I4800131_Use(MapID, InstanceID, TypeID, TargetID)
    local time1 = {}
	time1[4800131] = 30
	time1[4800132] = 240
	time1[4800133] = 720
	local k = role.GetRoleScriptData(TargetID, 1, RoleDataType["XiuLianChang_Time"])
	k = k+time1[TypeID]
	role.SetRoleScriptData(TargetID, 1, RoleDataType["XiuLianChang_Time"], k)
	local hour = math.floor(k/60)
	local minute = k%60
    local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 13, 0)
	msg.AddMsgEvent(MsgID, 1, 605016) 			--累积可进入修炼场时间增加到XX小时XX分钟
	msg.AddMsgEvent(MsgID, 9, hour)
	msg.AddMsgEvent(MsgID, 9, minute)
	msg.DispatchRoleMsgEvent(TargetID, MsgID)

	if MapID == 2748433913 or MapID == 2748434169 or MapID == 2748434425 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 16,	28)								--
		msg.AddMsgEvent(MsgID, 16,	k*60)							--剩余时间
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	end
end

function I4800131_CanUse(MapID, InstanceID, TypeID, TargetID)

	local time1 = {}
	time1[4800131] = 30
	time1[4800132] = 240
	time1[4800133] = 720

	local k = role.GetRoleScriptData(TargetID, 1, RoleDataType["XiuLianChang_Time"])
	if k + time1[TypeID] > 2880 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 605017) 			--累积可修炼时间最高不能超过48小时
		msg.DispatchRoleMsgEvent(TargetID, MsgID)
	    return 32, false
	end
	return 0,false
end

aux.RegisterItemEvent(4800131, 1, "I4800131_Use")
aux.RegisterItemEvent(4800131, 0, "I4800131_CanUse")
aux.RegisterItemEvent(4800132, 1, "I4800131_Use")
aux.RegisterItemEvent(4800132, 0, "I4800131_CanUse")
aux.RegisterItemEvent(4800133, 1, "I4800131_Use")
aux.RegisterItemEvent(4800133, 0, "I4800131_CanUse")
