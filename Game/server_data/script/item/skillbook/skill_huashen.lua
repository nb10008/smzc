--化身技能书
function skill_huashenUse(MapID, InstanceID, ItemID, TargetID)
	role.AddSkill(TargetID, 2300001)
end
aux.RegisterItemEvent(4622001, 1, "skill_huashenUse")
