--任务接取时函数

function MoKuZhiYin_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
    if role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"]) % 10 > 0 then
		return 1
	else
	    return 0
	end

end

aux.RegisterQuestEvent(30601, 5, "MoKuZhiYin_CheckComplete")
aux.RegisterQuestEvent(30602, 5, "MoKuZhiYin_CheckComplete")
aux.RegisterQuestEvent(30603, 5, "MoKuZhiYin_CheckComplete")
aux.RegisterQuestEvent(30604, 5, "MoKuZhiYin_CheckComplete")
aux.RegisterQuestEvent(30605, 5, "MoKuZhiYin_CheckComplete")
aux.RegisterQuestEvent(30606, 5, "MoKuZhiYin_CheckComplete")
aux.RegisterQuestEvent(30607, 5, "MoKuZhiYin_CheckComplete")
aux.RegisterQuestEvent(30608, 5, "MoKuZhiYin_CheckComplete")
aux.RegisterQuestEvent(30609, 5, "MoKuZhiYin_CheckComplete")
aux.RegisterQuestEvent(30610, 5, "MoKuZhiYin_CheckComplete")
aux.RegisterQuestEvent(30611, 5, "MoKuZhiYin_CheckComplete")
aux.RegisterQuestEvent(30612, 5, "MoKuZhiYin_CheckComplete")
aux.RegisterQuestEvent(30613, 5, "MoKuZhiYin_CheckComplete")
aux.RegisterQuestEvent(30614, 5, "MoKuZhiYin_CheckComplete")



--完成任务
function MoKuZhiYin_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	role.SetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"], math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"]) / 10)*10)
	for k=30601,30614 do
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, k) == true and QuestID ~= k then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"], math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"]) / 10)*10+1)
		end
	end
end

aux.RegisterQuestEvent(30601, 1, "MoKuZhiYin_OnComplete")
aux.RegisterQuestEvent(30602, 1, "MoKuZhiYin_OnComplete")
aux.RegisterQuestEvent(30603, 1, "MoKuZhiYin_OnComplete")
aux.RegisterQuestEvent(30604, 1, "MoKuZhiYin_OnComplete")
aux.RegisterQuestEvent(30605, 1, "MoKuZhiYin_OnComplete")
aux.RegisterQuestEvent(30606, 1, "MoKuZhiYin_OnComplete")
aux.RegisterQuestEvent(30607, 1, "MoKuZhiYin_OnComplete")
aux.RegisterQuestEvent(30608, 1, "MoKuZhiYin_OnComplete")
aux.RegisterQuestEvent(30609, 1, "MoKuZhiYin_OnComplete")
aux.RegisterQuestEvent(30610, 1, "MoKuZhiYin_OnComplete")
aux.RegisterQuestEvent(30611, 1, "MoKuZhiYin_OnComplete")
aux.RegisterQuestEvent(30612, 1, "MoKuZhiYin_OnComplete")
aux.RegisterQuestEvent(30613, 1, "MoKuZhiYin_OnComplete")
aux.RegisterQuestEvent(30614, 1, "MoKuZhiYin_OnComplete")


function RuQinZhiYin_CheckComplete(MapID, InstanceID, QuestID, RoleID, AccepterID)
    if math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"]) / 10) - math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"]) / 100)*10 > 0 then
		return 1
	else
	    return 0
	end

end

aux.RegisterQuestEvent(30627, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30628, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30629, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30630, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30631, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30632, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30633, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30634, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30635, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30636, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30637, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30638, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30639, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30640, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30641, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30642, 5, "RuQinZhiYin_CheckComplete")
aux.RegisterQuestEvent(30643, 5, "RuQinZhiYin_CheckComplete")

--完成任务
function RuQinZhiYin_OnComplete(MapID, InstanceID, QuestID, RoleID, NPCID)

	role.SetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"], role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"]) % 10 +math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"])/100)*100)
	for k=30627,30643 do
		if role.IsRoleHaveQuest(MapID, InstanceID, RoleID, k) == true and QuestID ~= k then
			role.SetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"], role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"]) % 10 +10+math.floor(role.GetRoleScriptData(RoleID, 1, RoleDataType["ActivityZhiYin"])/100)*100)
		end
	end
end

aux.RegisterQuestEvent(30627, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30628, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30629, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30630, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30631, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30632, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30633, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30634, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30635, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30636, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30637, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30638, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30639, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30640, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30641, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30642, 1, "RuQinZhiYin_OnComplete")
aux.RegisterQuestEvent(30643, 1, "RuQinZhiYin_OnComplete")
