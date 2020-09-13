--完成任务
function q50010_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local bRet = 0
	local a = role.GetRoleScriptData(RoleID, 1, RoleDataType["Quest_50010"])
	if a >= 2 then
		bRet = 1
	else
		bRet = 0
	end

	return bRet


end

aux.RegisterQuestEvent(50010, 5, "q50010_Complete")
