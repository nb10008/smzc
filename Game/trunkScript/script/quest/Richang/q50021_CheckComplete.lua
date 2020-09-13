--完成任务
function q50021_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local bRet = 0
	local a = role.GetRoleScriptData(RoleID, 1, RoleDataType["Quest_50021"])
	if a >= 15 then
	bRet = 1
	else
		bRet = 0
	end

	return bRet


end

aux.RegisterQuestEvent(50021, 5, "q50021_Complete")
