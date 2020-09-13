--变换职业为破魔
function q41026_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 11);
end
aux.RegisterQuestEvent(41026, 1, "q41026_OnComplete")