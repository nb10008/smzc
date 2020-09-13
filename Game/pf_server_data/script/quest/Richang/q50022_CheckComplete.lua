--完成任务
function q50022_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local bRet = 0
	local a = role.GetRoleScriptData(RoleID, 1, RoleDataType["Quest_50022"])
	if a >= 15 then
	bRet = 1
	else
		bRet = 0
	end

	return bRet


end

aux.RegisterQuestEvent(50022, 5, "q50022_Complete")
