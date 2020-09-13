----scriptdata[0]标记进副本是否刷怪
----scriptdata[1]标记3040306是否死亡 0未死亡 1 死亡
----scriptdata[2]标记3040307是否死亡 0未死亡 1 死亡
----scriptdata[3]标记3040332是否存在 0不存在 1 存在

----scriptdata[4]标记3040317是否死亡 0未死亡 1 死亡
----scriptdata[5]标记3040318是否死亡 0未死亡 1 死亡
----scriptdata[6]标记3040319是否死亡 0未死亡 1 死亡


--292，3289，408

function c3040317_OnEnterCombat(MapID, InstanceID, CreatureID)


	local MsgID = msg.BeginMsgEvent()
	msg.AddMsgEvent(MsgID, 102, 100140)
	msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10204)

	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)

	map.OpenCloseDoor(MapID, InstanceID, 1119, nil)

	
end

aux.RegisterCreatureEvent(3040317, 2, "c3040317_OnEnterCombat")


function c3040317_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	
	
	local bingxiisdead = map.GetMapScriptData(MapID,InstanceID,1,5)
	local heianxiisdead = map.GetMapScriptData(MapID,InstanceID,1,6)
	
	map.SetMapScriptData(MapID,InstanceID,1,4,1)
	--标记3040317死亡
	if bingxiisdead == 1 and heianxiisdead == 1 then

		cre.MonsterSay(MapID, InstanceID, TargetID, 10094)
	
		map.MapCreateCreature(MapID, InstanceID, 3040326, 292,3289,408)
		
		map.OpenCloseDoor(MapID, InstanceID, 1119, 1)

		


	end
	

end

aux.RegisterCreatureEvent(3040317, 4, "c3040317_OnDie")
