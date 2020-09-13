
--指引任务，妖精喂养

function use40050(MapID, InstanceID, TypeID, RoleID)
	
	local quest = role.IsRoleHaveQuest(MapID, InstanceID, RoleID, 40050)
	if quest == true then
		--role.RoleCompleteQuest(RoleID, 40050, 5511027)
		role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 40050, 2)
	end
end

function q40050_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	return 1
end

aux.RegisterItemEvent(4400101, 1, "use40050")
aux.RegisterQuestEvent(40050, 5, "q40050_Complete")