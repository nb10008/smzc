
function q40001_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetTrialCompleteNum(ownerID, 1003, 1)
	if role.GetTrialState(ownerID, 1003) == 1 then
	    role.TrialComplete(ownerID,1003)
	end
end

--зЂВс
aux.RegisterQuestEvent(40001, 1, "q40001_OnComplete")








