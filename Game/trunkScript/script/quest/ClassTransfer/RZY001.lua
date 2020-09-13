--任务完成时
--转职为神兵
function q41001_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 2)
	unit.AddBuff(mapID, instanceID, ownerID, 3202901, ownerID)
	role.NotifyFBBS(ownerID,9,2)
	if role.GetTrialState(ownerID, 2003) == 1 then
		role.SetTrialCompleteNum(ownerID, 2003, 1)
    end
	if role.GetTrialState(ownerID, 2003) == 1 and role.GetTrialCompleteNum(ownerID,2003) >= Trail_maxnum[2003] then
		role.TrialComplete(ownerID,2003)
	end
	--完成20级转职任务玩家等级
	UpdateRoleData(ownerID, 2, role.GetRoleLevel(mapID, instanceID, ownerID))
end
--转职为王者
function q41005_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 6)
	role.NotifyFBBS(ownerID,9,6)
	if role.GetTrialState(ownerID, 6003) == 1 then
		role.SetTrialCompleteNum(ownerID, 6003, 1)
    end
	if role.GetTrialState(ownerID, 6003) == 1 and role.GetTrialCompleteNum(ownerID,6003) >= Trail_maxnum[6003] then
		role.TrialComplete(ownerID,6003)
	end
end
--转职为神甲
function q41006_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 7)
	role.NotifyFBBS(ownerID,9,7)
	if role.GetTrialState(ownerID, 6003) == 1 then
		role.SetTrialCompleteNum(ownerID, 6003, 1)
    end
	if role.GetTrialState(ownerID, 6003) == 1 and role.GetTrialCompleteNum(ownerID,6003) >= Trail_maxnum[6003] then
		role.TrialComplete(ownerID,6003)
	end
end
--变换职业神兵
function q41013_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 2)
end

--注册
aux.RegisterQuestEvent(41001, 1, "q41001_OnComplete")
aux.RegisterQuestEvent(41005, 1, "q41005_OnComplete")
aux.RegisterQuestEvent(41006, 1, "q41006_OnComplete")
aux.RegisterQuestEvent(41013, 1, "q41013_OnComplete")
