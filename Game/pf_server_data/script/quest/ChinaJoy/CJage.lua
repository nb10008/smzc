--任务接取时函数
function q30792_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local Step2 = role.GetRoleScriptData(RoleID, 1, RoleDataType["ShiMen_Step_qixu"])
	if Step2 >= 5 then
		role.ModSpecialTargetValue(MapID, InstanceID, RoleID, QuestID, 2)
	end
end

function q30792_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local Step2 = role.GetRoleScriptData(RoleID, 1, RoleDataType["ShiMen_Step_qixu"])
	if Step2 >= 5 then
		return 1
	else
	    return 0
	end
end

aux.RegisterQuestEvent(30792, 0, "q30792_OnAccept")
aux.RegisterQuestEvent(30792, 5, "q30792_CheckComplete")
