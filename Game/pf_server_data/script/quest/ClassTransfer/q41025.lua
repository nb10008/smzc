--变换职业为雷神
function q41025_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 10);
end
aux.RegisterQuestEvent(41025, 1, "q41025_OnComplete")