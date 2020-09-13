--变换职业为天仪
function q41028_OnComplete(mapID, instanceID, questID, ownerID, enderID)
	role.SetRoleClass(mapID, instanceID, ownerID, 13);
end
aux.RegisterQuestEvent(41028, 1, "q41028_OnComplete")