
function QiYue_LearnSkill(MapID, InstanceID, TypeID, TargetID)
	role.AddSkill(TargetID, 9000301)
end
aux.RegisterItemEvent(4080085, 1, "QiYue_LearnSkill")

