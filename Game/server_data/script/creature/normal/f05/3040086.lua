----scriptdata[0]标记进副本是否刷怪
----scriptdata[1]标记3040075
----scriptdata[2]标记3040076
----scriptdata[3]标记3040077
----scriptdata[4]标记3040078
----scriptdata[5]标记3040086


function c3040086_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10065)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040086, 2, "c3040086_OnEnterCombat")






function c3040086_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10040)
	

	map.SetMapScriptData(MapID, InstanceID, 1, 5, 1)
	
	
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100087)
	--3040086死亡
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	--刷出3040087
	
	map.MapCreateCreature(MapID, InstanceID, 3040087, 422, 11166, 380)
	
	


end

aux.RegisterCreatureEvent(3040086, 4, "c3040086_OnDie")
