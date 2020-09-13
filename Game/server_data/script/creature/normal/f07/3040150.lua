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


function c3040150_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10100)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040150, 2, "c3040150_OnEnterCombat")






function c3040150_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10101)
	
	

	local creisdead = map.GetMapScriptData(MapID,InstanceID,1,5)
	map.SetMapScriptData(MapID,InstanceID,1,4,1)

	if creisdead == 1 then
	
		
		local creID = map.MapCreateCreature(MapID, InstanceID, 3040152, 500, 5670, 1360)
		map.SetMapScriptData(MapID,InstanceID,1,6,creID)


		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100115)
		--阿尔赛特分身刷出
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	else
		local MsgID = msg.BeginMsgEvent()
		--msg.AddMsgEvent(MsgID, 102, 100086)
		--还有一个没杀
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	end

end

aux.RegisterCreatureEvent(3040150, 4, "c3040150_OnDie")
