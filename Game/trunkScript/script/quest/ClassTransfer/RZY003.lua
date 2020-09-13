--任务完成时
--转职为乾坤
function q41003_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 4)
	unit.AddBuff(mapID, instanceID, ownerID, 3202901, ownerID)
	role.NotifyFBBS(ownerID,9,4)
	if role.GetTrialState(ownerID, 2003) == 1 then
		role.SetTrialCompleteNum(ownerID, 2003, 1)
    end
	if role.GetTrialState(ownerID, 2003) == 1 and role.GetTrialCompleteNum(ownerID,2003) >= Trail_maxnum[2003] then
		role.TrialComplete(ownerID,2003)
	end
	--完成20级转职任务玩家等级
	UpdateRoleData(ownerID, 2, role.GetRoleLevel(mapID, instanceID, ownerID))
end
--转职为雷神
function q41009_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 10)
	role.NotifyFBBS(ownerID,9,10)
	if role.GetTrialState(ownerID, 6003) == 1 then
		role.SetTrialCompleteNum(ownerID, 6003, 1)
    end
	if role.GetTrialState(ownerID, 6003) == 1 and role.GetTrialCompleteNum(ownerID,6003) >= Trail_maxnum[6003] then
		role.TrialComplete(ownerID,6003)
	end
end
--转职为破魔
function q41010_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 11)
	role.NotifyFBBS(ownerID,9,11)
	if role.GetTrialState(ownerID, 6003) == 1 then
		role.SetTrialCompleteNum(ownerID, 6003, 1)
    end
	if role.GetTrialState(ownerID, 6003) == 1 and role.GetTrialCompleteNum(ownerID,6003) >= Trail_maxnum[6003] then
		role.TrialComplete(ownerID,6003)
	end
end
--职业转变为乾坤

function q41015_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 4);
end

--注册
aux.RegisterQuestEvent(41003, 1, "q41003_OnComplete")
aux.RegisterQuestEvent(41009, 1, "q41009_OnComplete")
aux.RegisterQuestEvent(41010, 1, "q41010_OnComplete")
aux.RegisterQuestEvent(41015, 1, "q41015_OnComplete")
