
--Íæ¼ÒËÀÍö
function s08_RoleDie(MapID, InstanceID, RoleID, KillerID)
	if unit.IsPlayer(RoleID) and unit.IsPlayer(KillerID) then
		role.SetTrialCompleteNum(KillerID, 2016, role.GetTrialCompleteNum(KillerID,2016) + 1)
		if role.GetTrialState(KillerID, 2016) == 1 and role.GetTrialCompleteNum(KillerID,2016) >= Trail_maxnum[2016] then
			role.TrialComplete(KillerID,2016)
		end
	end
end

aux.RegisterMapEvent("s08", 5, "s08_RoleDie")

