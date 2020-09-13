--净化技能书

function skill_jinghuaUse(MapID, InstanceID, ItemID, TargetID)
	role.AddSkill(TargetID, 2010701)
end
aux.RegisterItemEvent(4810004, 1, "skill_jinghuaUse")
