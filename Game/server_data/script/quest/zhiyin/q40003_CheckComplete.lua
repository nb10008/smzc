--任务接取时函数
function q40003_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local a = role.GetYaojingLevel(MapID, InstanceID, RoleID)
	local CanComplete = 0
	if QuestID == 40003 then
		if a >= 10 then CanComplete =1 end
	end

	if CanComplete == 1 then
	role.ModSpecialTargetValue(MapID, InstanceID, RoleID, QuestID, 2)
	end
end

function q40003_CheckComplete(MapID, InstanceID, QuesiID, RoleID, NPCID)
	local bRet = 0
	local b = role.GetYaojingLevel(MapID, InstanceID, RoleID)

	if b < 10 then
		bRet = 0
	else
		bRet = 1
	end

	return bRet
end

aux.RegisterQuestEvent(40003, 0, "q40003_OnAccept")
aux.RegisterQuestEvent(40003, 5, "q40003_CheckComplete")
