--任务接取时函数
function q40059_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["God_Quest"])
	if k%10 >= 1 then
		role.ModSpecialTargetValue(MapID, InstanceID, RoleID, QuestID, 2)
	end
end

function q40059_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["God_Quest"])
	if k%10 >= 1 then
		return 1
	else
	    return 0
	end

end

aux.RegisterQuestEvent(40059, 0, "q40059_OnAccept")
aux.RegisterQuestEvent(40059, 5, "q40059_CheckComplete")


function q40060_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["God_Quest"])
	if k%100 >= 10 then
		role.ModSpecialTargetValue(MapID, InstanceID, RoleID, QuestID, 2)
	end
end

function q40060_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["God_Quest"])
	if k%100 >= 10 then
		return 1
	else
	    return 0
	end

end

aux.RegisterQuestEvent(40060, 0, "q40060_OnAccept")
aux.RegisterQuestEvent(40060, 5, "q40060_CheckComplete")


function q40061_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["God_Quest"])
	if k/100 >= 1 then
		role.ModSpecialTargetValue(MapID, InstanceID, RoleID, QuestID, 2)
	end
end

function q40061_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["God_Quest"])
	if k/100 >= 1 then
		return 1
	else
	    return 0
	end

end

aux.RegisterQuestEvent(40061, 0, "q40061_OnAccept")
aux.RegisterQuestEvent(40061, 5, "q40061_CheckComplete")

--任务接取时函数
function q40064_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["God_Quest"])
	if k%10 >= 3 then
		role.ModSpecialTargetValue(MapID, InstanceID, RoleID, QuestID, 2)
	end
end

function q40064_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["God_Quest"])
	if k%10 >= 3 then
		return 1
	else
	    return 0
	end

end

aux.RegisterQuestEvent(40064, 0, "q40064_OnAccept")
aux.RegisterQuestEvent(40064, 5, "q40064_CheckComplete")

--任务接取时函数
function q40065_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["God_Equip"])
	if k >= 1 then
		role.ModSpecialTargetValue(MapID, InstanceID, RoleID, QuestID, 2)
	end
end

function q40065_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local k = role.GetRoleScriptData(RoleID, 1, RoleDataType["God_Quest"])
	if k >= 1 then
		return 1
	else
	    return 0
	end

end

aux.RegisterQuestEvent(40065, 0, "q40065_OnAccept")
aux.RegisterQuestEvent(40065, 5, "q40065_CheckComplete")
