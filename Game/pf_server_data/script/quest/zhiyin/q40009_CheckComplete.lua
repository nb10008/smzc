--任务接取时函数
function q40009_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local a = role.GetYaojingLevel(MapID, InstanceID, RoleID)
	local CanComplete = 0
	if QuestID == 40009 then
		if a >= 60 then CanComplete =1 end
	end

	if CanComplete == 1 then
	role.ModSpecialTargetValue(MapID, InstanceID, RoleID, QuestID, 2)
	end
end

function q40009_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local bRet = 0
	local b = role.GetYaojingLevel(MapID, InstanceID, RoleID)

	if b < 60 then
		bRet = 0
	else
		bRet = 1
	end

	return bRet

end

aux.RegisterQuestEvent(40009, 0, "q40009_OnAccept")
aux.RegisterQuestEvent(40009, 5, "q40009_CheckComplete")
