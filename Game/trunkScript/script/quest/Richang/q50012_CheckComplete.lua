--完成任务
function q50012_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local bRet = 0
	local a = role.GetRoleScriptData(RoleID, 1, RoleDataType["Quest_50012"])
	if a >= 5 then
	bRet = 1
	else
		bRet = 0
	end

	return bRet


end

aux.RegisterQuestEvent(50012, 5, "q50012_Complete")
