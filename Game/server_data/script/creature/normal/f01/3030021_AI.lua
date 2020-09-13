---mapscriptdata[1] 记录进副本后是否已刷出怪物
--[2]记录老一是否已杀死
--[3]记录老二是否已杀死
--[4]记录老三是否已杀死
--[5]记录老四是否已杀死

function c3030021_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10050)
	
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
	map.OpenCloseDoor(MapID, InstanceID, 1119, nil)
	
end

aux.RegisterCreatureEvent(3030021, 2, "c3030021_OnEnterCombat")


function c3030021_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1119, 1)
end
aux.RegisterCreatureEvent(3030021, 3, "c3030021_OnLeaveCombat")





---死亡刷出老三
function c3030021_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	
	

	map.OpenCloseDoor(MapID, InstanceID, 1119, 1)

	map.MapCreateCreature(MapID, InstanceID, 3030022, 416, 702, 240)
	
	map.MapCreateCreature(MapID, InstanceID, 3030025, 416, 702, 252)
	map.MapCreateCreature(MapID, InstanceID, 3030024, 429, 702, 240)
	map.MapCreateCreature(MapID, InstanceID, 3030024, 416, 702, 228)
	map.MapCreateCreature(MapID, InstanceID, 3030024, 400, 702, 240)
end

aux.RegisterCreatureEvent(3030021, 4, "c3030021_OnDie")



