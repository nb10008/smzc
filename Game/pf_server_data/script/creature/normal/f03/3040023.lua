----scriptdata[1]记录3040018是否死亡
----scriptdata[2]记录3040019是否死亡
----scriptdata[3]记录是否刷过4个幻象
----scriptdata[4]记录3040020是否死亡，死亡几次
----scriptdata[5]记录3040021是否死亡，死亡几次
----scriptdata[6]记录3040024是否死亡
----scriptdata[7]记录3040025是否死亡
----scriptdata[8]标记宝箱
----scriptdata[9]标记进副本是否刷怪



function c3040023_OnEnterCombat(MapID, InstanceID, CreatureID)

	
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040023, 2, "c3040023_OnEnterCombat")


function c3040023_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	-------------------------addmsg刷出3040022
	cre.MonsterSay(MapID, InstanceID, TargetID, 10034)
	map.MapCreateCreature(MapID, InstanceID, 3040022, 1073, 1508, 245)
	map.MapCreateCreature(MapID, InstanceID, 3040026, 1059, 1425, 247)
	
		
	
end

aux.RegisterCreatureEvent(3040023, 4, "c3040023_OnDie")