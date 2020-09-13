

--完成任务
function q30905_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level <= 60 then
		role.AddRoleExp(MapID, InstanceID, RoleID, level*100)
	else
		role.AddRoleExp(MapID, InstanceID, RoleID, level*1000)
	end
end

aux.RegisterQuestEvent(30905, 1, "q30905_OnComplete")
