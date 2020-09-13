--任务接取时函数
function q20300_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
    role.SetRoleScriptData(RoleID, 1, RoleDataType["BeiZhouTianXue"], 1)
	--role.ModSpecialTargetValue(MapID, InstanceID, RoleID, 20300, 2)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["BeiZhouTianXue"])
	if k >= 1 then
		role.ModSpecialTargetValue(MapID, InstanceID, RoleID, QuestID, 2)
	end
end

function q20300_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["BeiZhouTianXue"])
	if k >= 1 then
		return 1
	else
	    return 0
	end
end

aux.RegisterQuestEvent(20300, 0, "q20300_OnAccept")
aux.RegisterQuestEvent(20300, 5, "q20300_CheckComplete")
