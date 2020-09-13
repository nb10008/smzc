----scriptdata[0]标记进副本是否刷怪
----scriptdata[1]标记3040306是否死亡 0未死亡 1 死亡
----scriptdata[2]标记3040307是否死亡 0未死亡 1 死亡
----scriptdata[3]标记3040332是否存在 0不存在 1 存在

----scriptdata[4]标记3040317是否死亡 0未死亡 1 死亡
----scriptdata[5]标记3040318是否死亡 0未死亡 1 死亡
----scriptdata[6]标记3040319是否死亡 0未死亡 1 死亡
----scriptdata[7]记录3040329的id

--292，3289，408

function c3040326_OnEnterCombat(MapID, InstanceID, CreatureID)

	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100141)
	--最后boss
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10207)

	map.OpenCloseDoor(MapID, InstanceID, 1119, nil)

	
end

aux.RegisterCreatureEvent(3040326, 2, "c3040326_OnEnterCombat")


---脱离战斗开门

function c3040326_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1119, 1)
end

aux.RegisterCreatureEvent(3040326, 3, "c3040326_OnLeaveCombat")




function c3040326_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	
	
	map.MapCreateCreature(MapID, InstanceID, 3040328, 140,61,243)

	local bossid = map.MapCreateCreature(MapID, InstanceID, 3040329, 140,347,175)
	
	map.SetMapScriptData(MapID,InstanceID,1,7,bossid)
		
	map.OpenCloseDoor(MapID, InstanceID, 1119, 1)


end

aux.RegisterCreatureEvent(3040326, 4, "c3040326_OnDie")
