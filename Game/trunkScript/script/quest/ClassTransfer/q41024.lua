--变换职业为占星
function q41024_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 9);
end
aux.RegisterQuestEvent(41024, 1, "q41024_OnComplete")