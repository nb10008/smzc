---mapscriptdata[1] 记录进副本后是否已刷出怪物
--[2]记录老一是否已杀死
--[3]记录老二是否已杀死
--[4]记录老三是否已杀死
--[5]记录老四是否已杀死

function c3030022_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10050)
	
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
	
	
end

aux.RegisterCreatureEvent(3030022, 2, "c3030022_OnEnterCombat")



---死亡刷出老三
function c3030022_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	
	
	
	
	map.MapCreateCreature(MapID, InstanceID, 3030023, 338, 50, 56)

	map.MapCreateCreature(MapID, InstanceID, 3030024, 349, 50, 68)
	map.MapCreateCreature(MapID, InstanceID, 3030024, 345, 50, 75)
	map.MapCreateCreature(MapID, InstanceID, 3030024, 340, 50, 84)	
	map.MapCreateCreature(MapID, InstanceID, 3030024, 335, 50, 95)

	map.MapCreateCreature(MapID, InstanceID, 3030025, 320, 50, 51)
	map.MapCreateCreature(MapID, InstanceID, 3030025, 316, 50, 59)
	map.MapCreateCreature(MapID, InstanceID, 3030025, 312, 50, 69)	
	map.MapCreateCreature(MapID, InstanceID, 3030025, 307, 50, 80)

end

aux.RegisterCreatureEvent(3030022, 4, "c3030022_OnDie")



