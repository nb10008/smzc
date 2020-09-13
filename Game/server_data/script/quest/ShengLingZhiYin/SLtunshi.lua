function shenglingtunshi(MapID, InstanceID, TargetID,num)
	local quest = role.IsRoleHaveQuest(MapID, InstanceID, TargetID, 51002)
	if quest == true then
		if num >=1 then
			role.ModSpecialTargetValue(MapID, InstanceID, TargetID,51002,2)
			role.SetRoleScriptData(TargetID, 1, RoleDataType["shenglingyangcheng"], 1)
		end
	end
end
aux.RegisterRoleEvent(91,"shenglingtunshi")
function q51002_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local bRet = 0
	local a = role.GetRoleScriptData(RoleID, 1, RoleDataType["shenglingyangcheng"])
	if a == 1 then
		bRet = 1
	else
		bRet = 0
	end
	return bRet
end
aux.RegisterQuestEvent(51002, 5, "q51002_Complete")