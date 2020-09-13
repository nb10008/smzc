--任务接取时函数
function q40005_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local a = role.GetYaojingLevel(MapID, InstanceID, RoleID)
	local CanComplete = 0
	if QuestID == 40005 then
		if a >= 20 then CanComplete =1 end
	end

	if CanComplete == 1 then
	role.ModSpecialTargetValue(MapID, InstanceID, RoleID, QuestID, 2)
	end
end

function q40005_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local bRet = 0
	local b = role.GetYaojingLevel(MapID, InstanceID, RoleID)

	if b < 20 then
		bRet = 0
	else
		bRet = 1
	end

	return bRet

end

aux.RegisterQuestEvent(40005, 0, "q40005_OnAccept")
aux.RegisterQuestEvent(40005, 5, "q40005_CheckComplete")
