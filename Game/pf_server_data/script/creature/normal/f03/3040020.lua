----scriptdata[1]记录3040018是否死亡
----scriptdata[2]记录3040019是否死亡
----scriptdata[3]记录是否刷过4个幻象
----scriptdata[4]记录3040020是否死亡，死亡几次
----scriptdata[5]记录3040021是否死亡，死亡几次
----scriptdata[6]记录3040024是否死亡
----scriptdata[7]记录3040025是否死亡
----scriptdata[8]标记宝箱
----scriptdata[9]标记进副本是否刷怪


function c3040020_OnDie(MapID, InstanceID, TargetID, TargetTypeID, RoleID)
	local a = map.GetMapScriptData(MapID, InstanceID, 1, 4)
	if a==0 then
		-------------------------addmsg还有一个幻象要杀

		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100051)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		map.SetMapScriptData(MapID, InstanceID, 1, 4, 1)
	elseif a==1 then
		-------------------------幻象被杀死2次从而刷出真身
		local MsgID = msg.BeginMsgEvent()
		msg.AddMsgEvent(MsgID, 102, 100047)    
		msg.DispatchMapMsgEvent(MsgID, MapID, InstanceID, -1)

		map.SetMapScriptData(MapID, InstanceID, 1, 4, 2)
		map.MapCreateCreature(MapID, InstanceID, 3040024, 926, 1820, 255)
	end	
end

aux.RegisterCreatureEvent(3040020, 4, "c3040020_OnDie")