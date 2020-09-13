
function x_RoleFinishTrain(MapID, InstanceID, RoleID, TrialID)
	if role.GetTrialState(RoleID, TrialID) == 2 then
		return 1
	elseif role.GetTrialState(RoleID, TrialID) < 2 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 304001)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    return 0
	elseif role.GetTrialState(RoleID, TrialID) == 3 then
	    local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 26, 304002)
		msg.DispatchRoleMsgEvent(RoleID, MsgID)
	    return 0
	else
	    return 0
	end
end

aux.RegisterRoleEvent(28, "x_RoleFinishTrain")
