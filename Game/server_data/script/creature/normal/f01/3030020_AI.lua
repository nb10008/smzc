---mapscriptdata[1] 记录进副本后是否已刷出怪物
--[2]记录老一是否已杀死
--[3]记录老二是否已杀死
--[4]记录老三是否已杀死
--[5]记录老四是否已杀死
--[6]记录是否刷出宝箱


function c3030020_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10052)
	
	
	--local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	-- g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 5, 20)
	
	
end

aux.RegisterCreatureEvent(3030020, 2, "c3030020_OnEnterCombat")






---死亡刷出老二
function c3030020_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	
	
	map.MapCreateCreature(MapID, InstanceID, 3030021, 287, 3289, 414)
	
	map.MapCreateCreature(MapID, InstanceID, 3030026, 287, 3289, 384)
	map.MapCreateCreature(MapID, InstanceID, 3030026, 260, 3289, 428)
	


end

aux.RegisterCreatureEvent(3030020, 4, "c3030020_OnDie")



