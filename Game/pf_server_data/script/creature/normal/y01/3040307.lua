----scriptdata[0]标记进副本是否刷怪
----scriptdata[1]标记3040306是否死亡 0未死亡 1 死亡
----scriptdata[2]标记3040307是否死亡 0未死亡 1 死亡
----scriptdata[3]标记3040332是否存在 0不存在 1 存在



function c3040307_OnEnterCombat(MapID, InstanceID, CreatureID)

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
	
	unit.AddBuff(MapID, InstanceID, CreatureID,5938301,CreatureID)
	
	map.OpenCloseDoor(MapID, InstanceID, 1110, nil)
end

aux.RegisterCreatureEvent(3040307, 2, "c3040307_OnEnterCombat")


function c3040307_OnLeaveCombat(MapID, InstanceID, CreatureID)
	map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
end
aux.RegisterCreatureEvent(3040307, 3, "c3040307_OnLeaveCombat")


function c3040307_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	
	
	local yanshiisdead = map.GetMapScriptData(MapID,InstanceID,1,1)
	local timerisdead = map.GetMapScriptData(MapID,InstanceID,1,3)
	--获得计时器状态
	map.SetMapScriptData(MapID,InstanceID,1,2,1)
	--标记3040307死亡
	if timerisdead == 1 and yanshiisdead == 1 then

		
	
		map.OpenCloseDoor(MapID, InstanceID, 1110, 1)
		map.OpenCloseDoor(MapID, InstanceID, 1111, 1)

		map.MapCreateCreature(MapID, InstanceID, 3040317, 414,701,254)
		map.MapCreateCreature(MapID, InstanceID, 3040318, 413,701,231)
		map.MapCreateCreature(MapID, InstanceID, 3040319, 427,702,243)
		

		local MsgID = msg.BeginMsgEvent()
		--msg.AddMsgEvent(MsgID, 102, 100139)
		--两个傀儡同时死亡了
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	else
		cre.MonsterSay(MapID, InstanceID, TargetID, 10203)
		
		local MsgID = msg.BeginMsgEvent()
		--msg.AddMsgEvent(MsgID, 102, 100137)
		--速度杀死另一个傀儡
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		map.MapCreateCreature(MapID, InstanceID, 3040332, 100, 100, 100)
		map.SetMapScriptData(MapID,InstanceID,1,3,1)
		--刷出计时器，把计时器状态位置为1

	end

	

end

aux.RegisterCreatureEvent(3040307, 4, "c3040307_OnDie")
