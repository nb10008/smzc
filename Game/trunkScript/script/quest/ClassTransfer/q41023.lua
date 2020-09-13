--变换职业为幻矢
function q41023_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 8);
end
aux.RegisterQuestEvent(41023, 1, "q41023_OnComplete")