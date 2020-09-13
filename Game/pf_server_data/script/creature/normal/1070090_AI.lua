
function s5000101_Cast(MapID, InstanceID, SkillID, OwnerID)
	
	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	map.MapCreateCreature(MapID, InstanceID, 1070091, x, y, z)
	map.MapCreateCreature(MapID, InstanceID, 1070091, x+2, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 1070091, x-2, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 1070091, x+2, y, z-2)
	map.MapCreateCreature(MapID, InstanceID, 1070091, x-2, y, z-2)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    	--同步仇恨给范围为10个格子，高度为20的有方怪物
   	g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 5, 20)
	
end
aux.RegisterSkillEvent(5000101, 1, "s5000101_Cast")


