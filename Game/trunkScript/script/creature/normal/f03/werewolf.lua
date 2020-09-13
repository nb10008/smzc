----scriptdata[1]记录3040018是否死亡
----scriptdata[2]记录3040019是否死亡
----scriptdata[3]记录是否刷过4个幻象
----scriptdata[4]记录3040020是否死亡，死亡几次
----scriptdata[5]记录3040021是否死亡，死亡几次
----scriptdata[6]记录3040024是否死亡
----scriptdata[7]记录3040025是否死亡
----scriptdata[8]标记宝箱
----scriptdata[9]标记进副本是否刷怪




function c3040012_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10020)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040012, 2, "c3040012_OnEnterCombat")






function c3040013_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10021)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040013, 2, "c3040013_OnEnterCombat")




function c3040014_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10022)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040014, 2, "c3040014_OnEnterCombat")




function werewolf_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	
	cre.MonsterSay(MapID, InstanceID, TargetID, 10023)
	

end

aux.RegisterCreatureEvent(3040012, 4, "werewolf_OnDie")
aux.RegisterCreatureEvent(3040013, 4, "werewolf_OnDie")
aux.RegisterCreatureEvent(3040014, 4, "werewolf_OnDie")

		



