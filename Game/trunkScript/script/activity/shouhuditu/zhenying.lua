function shouhuditu_OnPlayerEnter(MapID, InstanceID, RoleID)
	unit.SetCamp(MapID, InstanceID, RoleID, 1)
end
aux.RegisterMapEvent("dmap1",2, "shouhuditu_OnPlayerEnter")