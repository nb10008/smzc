--Äı¾Û¼¼ÄÜÊé

function skill_ningjuUse(MapID, InstanceID, ItemID, TargetID)
	role.AddSkill(TargetID, 9000501)
end
aux.RegisterItemEvent(4810007, 1, "skill_ningjuUse")
