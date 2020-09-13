
function m10_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID == 1071 then
		local RoleLevel = role.GetRoleLevel(MapID, InstanceID, RoleID)
		if RoleLevel >= 10 then
			role.RoleGotoNewMap(MapID, InstanceID, RoleID, 3424073512, 616, 22980, 239)
		else
			local MsgID = msg.BeginMsgEvent()
			msg.AddMsgEvent(MsgID, 26, 400001)
			msg.DispatchRoleMsgEvent(RoleID, MsgID)
		end
	end
end
aux.RegisterMapEvent("m10",6,"m10_OnEnterTrigger")

--Íæ¼ÒËÀÍö
function m10_RoleDie(MapID, InstanceID, RoleID, KillerID)
	if unit.IsPlayer(RoleID) and unit.IsPlayer(KillerID) then
		role.SetTrialCompleteNum(KillerID, 2016, role.GetTrialCompleteNum(KillerID,2016) + 1)
		if role.GetTrialState(KillerID, 2016) == 1 and role.GetTrialCompleteNum(KillerID,2016) >= Trail_maxnum[2016] then
			role.TrialComplete(KillerID,2016)
		end
	end
end

aux.RegisterMapEvent("m10", 5, "m10_RoleDie")
