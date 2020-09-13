--调查熔魔炉刷新怪物
function c6010009_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local x, y ,z = unit.GetPosition(MapID, InstanceID, TargetID)
	map.MapCreateCreature(MapID, InstanceID, 1050047, x, y, z)
end

aux.RegisterCreatureEvent(6010009, 4, "c6010009_OnDie")



