---mapscriptdata[1] 记录进副本后是否已刷出怪物
--[2]记录老一是否已杀死

function c3030003_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10050)
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100080)    
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	--关门，放狗
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
	
	map.OpenCloseDoor(MapID, InstanceID, 1110, nil)
end

aux.RegisterCreatureEvent(3030003, 2, "c3030003_OnEnterCombat")

---脱离战斗关门

function c3030003_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
end
aux.RegisterCreatureEvent(3030003, 3, "c3030003_OnLeaveCombat")


---死亡刷出老二
function c3030003_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
	map.OpenCloseDoor(MapID, InstanceID, 1111, 1)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100082)    
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10051)
	map.MapCreateCreature(MapID, InstanceID, 3030007, 446, 702, 243)
	map.SetMapScriptData(MapID,InstanceID,1,2,1)


end

aux.RegisterCreatureEvent(3030003, 4, "c3030003_OnDie")



