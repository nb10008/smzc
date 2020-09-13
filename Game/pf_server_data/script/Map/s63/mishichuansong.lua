--妖精密室传送人对话
function c5600004_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 69)
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)			--
		msg.AddMsgEvent(MsgID, 1, 100001)			--“确定”
		msg.AddMsgEvent(MsgID, 21, -1)			--
		msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3424073512,610,23094,669 )
	end
end
aux.RegisterCreatureEvent(5600004, 7, "c5600004_On_Talk")
