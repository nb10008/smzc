--变换职业为神侍
function q41027_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 12);
end
aux.RegisterQuestEvent(41027, 1, "q41027_OnComplete")