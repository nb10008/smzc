

--[[任务接取时函数
function YaoJing_OnAccept(MapID, InstanceID, QuestID, RoleID, AccepterID)
	local a = role.GetYaojingLevel(MapID, InstanceID, RoleID)
	local CanComplete = 0
	if QuestID == 40003 then
		if a >= 10 then CanComplete =1 end
	elseif QuestID == 40005 then
		if a >= 20 then CanComplete =1 end
	elseif QuestID == 40006 then
		if a >= 30 then CanComplete =1 end
	elseif QuestID == 40007 then
		if a >= 40 then CanComplete =1 end
	elseif QuestID == 40008 then
		if a >= 50 then CanComplete =1 end
	elseif QuestID == 40009 then
		if a >= 60 then CanComplete =1 end
	end

	if CanComplete == 1 then
	role.ModSpecialTargetValue(MapID, InstanceID, RoleID, QuestID, 2)
	end
end

--注册
aux.RegisterQuestEvent(40003, 0, "YaoJing_OnAccept")
aux.RegisterQuestEvent(40005, 0, "YaoJing_OnAccept")
aux.RegisterQuestEvent(40006, 0, "YaoJing_OnAccept")
aux.RegisterQuestEvent(40007, 0, "YaoJing_OnAccept")
aux.RegisterQuestEvent(40008, 0, "YaoJing_OnAccept")
aux.RegisterQuestEvent(40009, 0, "YaoJing_OnAccept")]]
