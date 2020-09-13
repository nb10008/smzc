
function tianshenxiafan_Cast(MapID, InstanceID, SkillID, OwnerID, TargetID)
    if role.GetTrialState(OwnerID, 1005) == 1 then
		role.SetTrialCompleteNum(OwnerID, 1005, role.GetTrialCompleteNum(OwnerID,1005) + 1)
    end
	if role.GetTrialState(OwnerID, 1005) == 1 and role.GetTrialCompleteNum(OwnerID,1005) >= Trail_maxnum[1005] then
		role.TrialComplete(OwnerID,1005)
	end
end


--зЂВс
aux.RegisterSkillEvent(1011301, 1, "tianshenxiafan_Cast")
aux.RegisterSkillEvent(1011401, 1, "tianshenxiafan_Cast")
aux.RegisterSkillEvent(1011501, 1, "tianshenxiafan_Cast")
aux.RegisterSkillEvent(1011601, 1, "tianshenxiafan_Cast")
aux.RegisterSkillEvent(1011701, 1, "tianshenxiafan_Cast")
