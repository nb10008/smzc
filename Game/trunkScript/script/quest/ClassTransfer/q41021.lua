--变换职业为王者
function q41021_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 6);
end
aux.RegisterQuestEvent(41021, 1, "q41021_OnComplete")