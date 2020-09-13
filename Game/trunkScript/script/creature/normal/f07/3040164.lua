----scriptdata[0]标记进副本是否刷怪
----scriptdata[1]标记召唤术到第几级了
----scriptdata[2]标记3040141是否死亡
----scriptdata[3]标记3040143是否死亡
----scriptdata[4]标记3040150是否死亡
----scriptdata[5]标记3040151是否死亡
----scriptdata[6]记录3040152的ID
----scriptdata[7]标记3040164是否死亡
----scriptdata[8]标记3040165是否死亡
----scriptdata[9]标记3040166是否死亡
----scriptdata[10]标记是否刷过宝箱




function c3040164_OnEnterCombat(MapID, InstanceID, CreatureID)

	--cre.MonsterSay(MapID, InstanceID, CreatureID, 10063)
	
end

aux.RegisterCreatureEvent(3040164, 2, "c3040164_OnEnterCombat")






function c3040164_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	--cre.MonsterSay(MapID, InstanceID, TargetID, 10064)
	
	

	local fenshenisdead_01 = map.GetMapScriptData(MapID,InstanceID,1,8)
	local fenshenisdead_02 = map.GetMapScriptData(MapID,InstanceID,1,9)

	map.SetMapScriptData(MapID,InstanceID,1,7,1)

	if fenshenisdead_01 == 1 and fenshenisdead_02 == 1 then
	
		map.OpenCloseDoor(MapID, InstanceID, 2218, 1)
		map.OpenCloseDoor(MapID, InstanceID, 1853, 1)

		map.MapCreateCreature(MapID, InstanceID, 3040153, 476, 5664, 1790)
		map.MapCreateCreature(MapID, InstanceID, 3040154, 501, 5664, 1790)


		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100116)
		--阿尔赛特和妖术师刷出
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	

	end

end

aux.RegisterCreatureEvent(3040164, 4, "c3040164_OnDie")
