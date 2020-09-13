--比翼双飞技能书

function skill_biyishuangfei(MapID, InstanceID, ItemID, TargetID)
	role.AddSkill(TargetID, 9000701)
end
aux.RegisterItemEvent(4800118, 1, "skill_biyishuangfei")

--比翼双飞技能书

function skill_zhenaizhiwen(MapID, InstanceID, ItemID, TargetID)
	role.AddSkill(TargetID, 9000601)
end
aux.RegisterItemEvent(4800119, 1, "skill_zhenaizhiwen")
