---mapscriptdata[1] 记录进副本后是否已刷出怪物
----[2]记录3040008是否被杀死
----[3]记录3040009是否被杀死
----[4]记录是否有过包围。
----[5]记录是否有埋伏。
----[6]记录3040010是否被杀死


function c3040009_OnEnterCombat(MapID, InstanceID, CreatureID)
	local maifu=map.GetMapScriptData(MapID, InstanceID, 1,5)
 	if maifu==0 then
		cre.MonsterSay(MapID, InstanceID, CreatureID, 10009)
		map.MapCreateCreature(MapID, InstanceID, 3040003, 821, 4295, 330)
		map.MapCreateCreature(MapID, InstanceID, 3040006, 824, 4295, 322)
		map.MapCreateCreature(MapID, InstanceID, 3040003, 827, 4295, 314)

		map.SetMapScriptData(MapID,InstanceID,1,5,1)
	end	

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040009, 2, "c3040009_OnEnterCombat")



function c3040009_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	local a = map.GetMapScriptData(MapID, InstanceID, 1, 2)
	map.SetMapScriptData(MapID, InstanceID, 1, 3, 1)
	if a==1 then
		--------3040008已死
		cre.MonsterSay(MapID, InstanceID, TargetID, 10011)
		map.MapCreateCreature(MapID, InstanceID, 3040010, 577, 4295, 147)
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100041)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100040)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
		--------3040008未死
		cre.MonsterSay(MapID, InstanceID, TargetID, 10010)
	end

end

aux.RegisterCreatureEvent(3040009, 4, "c3040009_OnDie")



