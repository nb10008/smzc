--任务接取时函数
function q40007_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local a = role.GetYaojingLevel(MapID, InstanceID, RoleID)
	local CanComplete = 0
	if QuestID == 40007 then
		if a >= 40 then CanComplete =1 end
	end

	if CanComplete == 1 then
	role.ModSpecialTargetValue(MapID, InstanceID, RoleID, QuestID, 2)
	end
end

function q40007_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local bRet = 0
	local b = role.GetYaojingLevel(MapID, InstanceID, RoleID)

	if b < 40 then
		bRet = 0
	else
		bRet = 1
	end

	return bRet

end

aux.RegisterQuestEvent(40007, 0, "q40007_OnAccept")
aux.RegisterQuestEvent(40007, 5, "q40007_CheckComplete")
