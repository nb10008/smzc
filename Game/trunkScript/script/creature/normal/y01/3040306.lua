----scriptdata[0]标记进副本是否刷怪
----scriptdata[1]标记3040306是否死亡 0未死亡 1 死亡
----scriptdata[2]标记3040307是否死亡 0未死亡 1 死亡
----scriptdata[3]标记3040332是否存在 0不存在 1 存在


function c3040306_OnEnterCombat(MapID, InstanceID, CreatureID)
	
	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100134)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10202)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子,高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

	unit.AddBuff(MapID, InstanceID, CreatureID,5938201,CreatureID)

	map.OpenCloseDoor(MapID, InstanceID, 1110, nil)
end

aux.RegisterCreatureEvent(3040306, 2, "c3040306_OnEnterCombat")


---脱离战斗开门

function c3040306_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
end
aux.RegisterCreatureEvent(3040306, 3, "c3040306_OnLeaveCombat")




function c3040306_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	
	

	local timerisdead = map.GetMapScriptData(MapID,InstanceID,1,3)
	local yanmoisdead = map.GetMapScriptData(MapID,InstanceID,1,2)
	--获得计时器状态
	map.SetMapScriptData(MapID,InstanceID,1,1,1)
	--标记3040306死亡
	if timerisdead == 1 and yanmoisdead == 1 then

		
	
		map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
		map.OpenCloseDoor(MapID, InstanceID, 1111, 1)
		

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100139)
		--两个傀儡同时死亡了
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		map.MapCreateCreature(MapID, InstanceID, 3040317, 414,701,254)
		map.MapCreateCreature(MapID, InstanceID, 3040318, 413,701,231)
		map.MapCreateCreature(MapID, InstanceID, 3040319, 427,702,243)
		

	else

		cre.MonsterSay(MapID, InstanceID, TargetID, 10203)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100136)
		--速度杀死另一个傀儡
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		map.MapCreateCreature(MapID, InstanceID, 3040332, 100, 100, 100)
		map.SetMapScriptData(MapID,InstanceID,1,3,1)
		--刷出计时器,把计时器状态位置为0

	end

	

end

aux.RegisterCreatureEvent(3040306, 4, "c3040306_OnDie")
