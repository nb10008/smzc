--1092009	冥神姬尤莉雅娜

function s5109301_Cast(MapID, InstanceID, SkillID, OwnerID)

	local x, y ,z = unit.GetPosition(MapID, InstanceID, OwnerID)
	map.MapCreateCreature(MapID, InstanceID, 1070052, x+2, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 1070052, x-2, y, z+2)
	map.MapCreateCreature(MapID, InstanceID, 1070051, x+2, y, z-2)
	map.MapCreateCreature(MapID, InstanceID, 1070051, x-2, y, z-2)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, OwnerID)
    	--同步仇恨给范围为10个格子，高度为20的有方怪物
   	g_SynCreatureEnmity(MapID, InstanceID, OwnerID, TargetID, 1, 5, 20)
	cre.MonsterSay(MapID, InstanceID, OwnerID, 20253)
end
aux.RegisterSkillEvent(5109301, 1, "s5109301_Cast")


