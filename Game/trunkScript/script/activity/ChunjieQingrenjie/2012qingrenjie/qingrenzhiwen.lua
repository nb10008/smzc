function q30912_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local num = role.GetRoleScriptData(RoleID, 1, RoleDataType["qingrenjieqinwenNum"])
	local qinwenID = role.GetRoleScriptData(RoleID, 1, RoleDataType["qingrenjieqinwenID"])
	local titleID = 0
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if(num == nil or num == 0) then
		titleID = 4800262
	else
		titleID = 4800263
	end
	role.AddRoleItem(MapID, InstanceID, RoleID, titleID, 1, -1, 8, 420)
	role.AddRoleExp(MapID, InstanceID, RoleID, level*5000)
end

aux.RegisterQuestEvent(30912, 1, "q30912_Complete")



function q30906_Complete(MapID, InstanceID, QuestID, RoleID, NPCID)
	local level = role.GetRoleLevel(MapID, InstanceID, RoleID)
	if level~=nil then
		if level > 40 and level <= 60 then
			role.AddRoleExp(MapID, InstanceID, RoleID, level*2500)
		else
			role.AddRoleExp(MapID, InstanceID, RoleID, level*10000)
		end
	end
end

aux.RegisterQuestEvent(30906, 1, "q30906_Complete")
aux.RegisterQuestEvent(30907, 1, "q30906_Complete")
aux.RegisterQuestEvent(30908, 1, "q30906_Complete")
aux.RegisterQuestEvent(30909, 1, "q30906_Complete")
aux.RegisterQuestEvent(30910, 1, "q30906_Complete")
aux.RegisterQuestEvent(30911, 1, "q30906_Complete")
aux.RegisterQuestEvent(30913, 1, "q30906_Complete")
aux.RegisterQuestEvent(30914, 1, "q30906_Complete")
aux.RegisterQuestEvent(30915, 1, "q30906_Complete")
aux.RegisterQuestEvent(30916, 1, "q30906_Complete")
