---mapscriptdata[0] 记录进副本后是否已刷出怪物
---mapscriptdata[1] 记录第一个boss的id

---mapscriptdata[2] 记录第几波怪物
---mapscriptdata[3] 记录boss1是否被杀死

function c3040249_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10121)
	
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100129)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

end

aux.RegisterCreatureEvent(3040249, 2, "c3040249_OnEnterCombat")

function c3040249_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	
	--cre.MonsterSay(MapID, InstanceID, TargetID, 10029)
	
	map.MapCreateCreature(MapID, InstanceID, 3040228, 222, 1151, 636)
	map.MapCreateCreature(MapID, InstanceID, 3040232, 462, 1151, 688)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100130)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	map.OpenCloseDoor(MapID, InstanceID, 877, 1)
	map.OpenCloseDoor(MapID, InstanceID, 880, 1)
	map.OpenCloseDoor(MapID, InstanceID, 881, 1)
	map.OpenCloseDoor(MapID, InstanceID, 882, 1)
	map.OpenCloseDoor(MapID, InstanceID, 883, 1)
	

end
aux.RegisterCreatureEvent(3040249, 4, "c3040249_OnDie")
