--1092003	仙妖后阿莱茜娅

function s5103301_Cast(MapID, InstanceID, SkillID, OwnerID)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	map.MapCreateCreature(MapID, InstanceID, 1060037, x+2, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 1060037, x-2, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 1060038, x+2, y, z-2)
	map.MapCreateCreature(MapID, InstanceID, 1060038, x-2, y, z-2)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    	--同步仇恨给范围为10个格子，高度为20的有方怪物
   	g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 5, 20)
	cre.MonsterSay(MapID, InstanceID, OwnerID, 20252)
end
aux.RegisterSkillEvent(5103301, 1, "s5103301_Cast")


