--完成任务
function q50015_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local bRet = 0
	local a = role.GetRoleScriptData(RoleID, 1, RoleDataType["Quest_50015"])
	if a >= 6 then
	bRet = 1
	else
		bRet = 0
	end

	return bRet


end

aux.RegisterQuestEvent(50015, 5, "q50015_Complete")
