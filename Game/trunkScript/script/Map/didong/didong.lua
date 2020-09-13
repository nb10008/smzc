
--Íæ¼ÒËÀÍö
function didong_RoleDie(MapID, InstanceID, RoleID, KillerID)
	if unit.IsPlayer(RoleID) and unit.IsPlayer(KillerID) then
		role.SetTrialCompleteNum(KillerID, 2016, role.GetTrialCompleteNum(KillerID,2016) + 1)
		if role.GetTrialState(KillerID, 2016) == 1 and role.GetTrialCompleteNum(KillerID,2016) >= Trail_maxnum[2016] then
			role.TrialComplete(KillerID,2016)
		end
	end
end

aux.RegisterMapEvent("d01", 5, "didong_RoleDie")
aux.RegisterMapEvent("d02", 5, "didong_RoleDie")
aux.RegisterMapEvent("d03", 5, "didong_RoleDie")
aux.RegisterMapEvent("d04", 5, "didong_RoleDie")
aux.RegisterMapEvent("d05", 5, "didong_RoleDie")
aux.RegisterMapEvent("d06", 5, "didong_RoleDie")
aux.RegisterMapEvent("d07", 5, "didong_RoleDie")
aux.RegisterMapEvent("d08", 5, "didong_RoleDie")
aux.RegisterMapEvent("d09", 5, "didong_RoleDie")
aux.RegisterMapEvent("d10", 5, "didong_RoleDie")
aux.RegisterMapEvent("d11", 5, "didong_RoleDie")

