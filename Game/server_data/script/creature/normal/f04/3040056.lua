

function c3040056_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10045)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040056, 2, "c3040056_OnEnterCombat")

function c3040056_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10046)

	map.MapCreateCreature(MapID, InstanceID, 3040057, 272, 5968, 251)
	
	
	local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100050)    
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

end

aux.RegisterCreatureEvent(3040056, 4, "c3040056_OnDie")