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




function c3040143_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10095)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040143, 2, "c3040143_OnEnterCombat")


function c3040143_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)

	cre.MonsterSay(MapID, InstanceID, TargetID, 10096)
	

	local creisdead = map.GetMapScriptData(MapID,InstanceID,1,2)
	map.SetMapScriptData(MapID,InstanceID,1,3,1)

	if creisdead == 1 then
	
		map.OpenCloseDoor(MapID, InstanceID, 2213, 1)
		
		map.MapCreateCreature(MapID, InstanceID, 3040150, 258, 3840, 1503)
		map.MapCreateCreature(MapID, InstanceID, 3040151, 908, 3840, 1372)


		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100114)
		--门打开
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	else
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100113)
		--门未打开
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

	end

	

end

aux.RegisterCreatureEvent(3040143, 4, "c3040143_OnDie")
