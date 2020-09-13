--妖精秘境之门对话
function c5600006_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
	if TalkIndex == -1 then
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 20, 208001)		--妖精秘境里有各种妖精，战胜它们后可以契约妖精！现在就进去吗？
		msg.AddMsgEvent(MsgID, 24, TargetID)
		msg.AddMsgEvent(MsgID, 21, 4)			--
		msg.AddMsgEvent(MsgID, 1, 100001)			--“确定”
		msg.AddMsgEvent(MsgID, 21, -1)			--
		msg.AddMsgEvent(MsgID, 1, 100002)			--“取消”
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	elseif TalkIndex == 4 then
		role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3390519080,126,1989,183)
	end
end
aux.RegisterCreatureEvent(5600006, 7, "c5600006_On_Talk")



--妖精秘境传送人对话
function c5600005_On_Talk(MapID, InstanceID, TargetID, TargetTypeID, RoleID, TalkIndex)
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
aux.RegisterCreatureEvent(5600005, 7, "c5600005_On_Talk")
