--Éñ»¯
function s2300001_Cast(MapID, InstanceID, SkillID, TargetID)
	if role.GetRoleSex(TargetID) == 1 then
		unit.AddBuff(MapID, InstanceID, TargetID, 9004601, TargetID)
	else
		unit.AddBuff(MapID, InstanceID, TargetID, 9004801, TargetID)
	end
end
aux.RegisterSkillEvent(2300001, 1, "s2300001_Cast")
