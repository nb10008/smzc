----scriptdata[1]记录3040018是否死亡
----scriptdata[2]记录3040019是否死亡
----scriptdata[3]记录是否刷过4个幻象
----scriptdata[4]记录3040020是否死亡，死亡几次
----scriptdata[5]记录3040021是否死亡，死亡几次
----scriptdata[6]记录3040024是否死亡
----scriptdata[7]记录3040025是否死亡
----scriptdata[8]标记宝箱
----scriptdata[9]标记进副本是否刷怪

function c3040024_OnEnterCombat(MapID, InstanceID, CreatureID)

	cre.MonsterSay(MapID, InstanceID, CreatureID, 10030)
	local TargetID = cre.GetCreatureCurTargetID(MapID, InstanceID, CreatureID)
    	--同步仇恨给范围为40个格子，高度为20的有方怪物
   	 g_SynCreatureEnmity(MapID, InstanceID, CreatureID, TargetID, 1, 40, 20)
end

aux.RegisterCreatureEvent(3040024, 2, "c3040024_OnEnterCombat")


function c3040024_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	cre.MonsterSay(MapID, InstanceID, TargetID, 10032)
	map.SetMapScriptData(MapID, InstanceID, 1, 6, 1)
	local a = map.GetMapScriptData(MapID, InstanceID, 1, 7)
	if a==1 then
		-------------------------addmsg刷出3040023
		map.MapCreateCreature(MapID, InstanceID, 3040023, 853, 1381, 292)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 843, 1381, 282)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 843, 1381, 302)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 863, 1381, 282)
		map.MapCreateCreature(MapID, InstanceID, 3040026, 863, 1381, 302)

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100049)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)
	end	
	
end

aux.RegisterCreatureEvent(3040024, 4, "c3040024_OnDie")