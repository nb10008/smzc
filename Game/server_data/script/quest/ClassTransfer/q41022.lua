--变换职业为神甲
function q41022_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 7);
end
aux.RegisterQuestEvent(41022, 1, "q41022_OnComplete")