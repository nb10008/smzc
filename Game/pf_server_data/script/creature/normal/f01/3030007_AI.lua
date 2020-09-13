---mapscriptdata[1] 记录进副本后是否已刷出怪物
--[2]记录老一是否已杀死
--[3]记录老二是否已杀死
--[4]记录老三是否已杀死
--[5]记录老四是否已杀死

function c3030007_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10052)
	
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
	
	
end

aux.RegisterCreatureEvent(3030007, 2, "c3030007_OnEnterCombat")



---死亡刷出老三
function c3030007_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	
	cre.MonsterSay(MapID, InstanceID, TargetID, 10053)
	map.MapCreateCreature(MapID, InstanceID, 3030013, 287, 3289, 414)
	
	map.SetMapScriptData(MapID,InstanceID,1,3,1)
	map.OpenCloseDoor(MapID, InstanceID, 1119, 1)

end

aux.RegisterCreatureEvent(3030007, 4, "c3030007_OnDie")



