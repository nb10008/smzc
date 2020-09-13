---mapscriptdata[1] 记录进副本后是否已刷出怪物
--[2]记录老一是否已杀死
--[3]记录老二是否已杀死
--[4]记录老三是否已杀死
--[5]记录老四是否已杀死

function c3030013_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10061)
	--local MsgID = msg.BeginMsgEvent()
	--msg.AddMsgEvent(MsgID, 102, 100081)
	--msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	--关门，放狗
	--map.OpenCloseDoor(MapID, InstanceID, 1119, nil)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)


end

aux.RegisterCreatureEvent(3030013, 2, "c3030013_OnEnterCombat")

function c3030013_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1119, 1)
end
aux.RegisterCreatureEvent(3030013, 3, "c3030013_OnLeaveCombat")


---死亡刷出老四
function c3030013_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	map.OpenCloseDoor(MapID, InstanceID, 1119, 1)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10062)
	map.MapCreateCreature(MapID, InstanceID, 3030018, 141, 61, 243)

	map.SetMapScriptData(MapID,InstanceID,1,4,1)


end

aux.RegisterCreatureEvent(3030013, 4, "c3030013_OnDie")



