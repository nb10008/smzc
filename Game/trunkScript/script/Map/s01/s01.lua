
function s01_OnEnterTrigger(MapID, InstanceID, RoleID, ObjID)
	if ObjID == 1038 then
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
aux.RegisterMapEvent("s01",6,"s01_OnEnterTrigger")

--Íæ¼ÒËÀÍö
function s01_RoleDie(MapID, InstanceID, RoleID, KillerID)
	if unit.IsPlayer(RoleID) and unit.IsPlayer(KillerID) then
		role.SetTrialCompleteNum(KillerID, 2016, role.GetTrialCompleteNum(KillerID,2016) + 1)
		if role.GetTrialState(KillerID, 2016) == 1 and role.GetTrialCompleteNum(KillerID,2016) >= Trail_maxnum[2016] then
			role.TrialComplete(KillerID,2016)
		end
	end
end

aux.RegisterMapEvent("s01", 5, "s01_RoleDie")
