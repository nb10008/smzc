--½©Ê¬ËÀÍö£¬Ë¢ÐÂ±¦Ïä
function c1020021_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)

	map.MapCreateCreature(MapID, InstanceID, 6010066, x, y ,z)
end

aux.RegisterCreatureEvent(1020021, 4, "c1020021_OnDie")
